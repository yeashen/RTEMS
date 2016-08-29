#
# RTEMS Tools Project (http://www.rtems.org/)
# Copyright 2010-2016 Chris Johns (chrisj@rtems.org)
# All rights reserved.
#
# This file is part of the RTEMS Tools package in 'rtems-tools'.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#
# Execute commands or scripts.
#
# Note, the subprocess module is only in Python 2.4 or higher.
#

from __future__ import print_function

import functools
import os
import re
import sys
import subprocess
import threading

import error
import log

# Redefine the PIPE from subprocess
PIPE = subprocess.PIPE

# Regular expression to find quotes.
qstr = re.compile('[rR]?\'([^\\n\'\\\\]|\\\\.)*\'|[rR]?"([^\\n"\\\\]|\\\\.)*"')

def check_type(command):
    """Checks the type of command we have. The types are spawn and
    shell."""
    if command in ['spawn', 'shell']:
        return True
    return False

def arg_list(args):
    """Turn a string of arguments into a list suitable for
    spawning a command. If the args are already a list return
    it."""
    if type(args) is list:
        return args
    argstr = args
    args = []
    while len(argstr):
        qs = qstr.search(argstr)
        if not qs:
            args.extend(argstr.split())
            argstr= ''
        else:
            # We have a quoted string. Get the string before
            # the quoted string and splt on white space then
            # add the quoted string as an option then remove
            # the first + quoted string and try again
            front = argstr[:qs.start()]
            args.extend(front.split())
            args.append(argstr[qs.start() + 1:qs.end() - 1])
            argstr = argstr[qs.end():]
    return args

def arg_subst(command, substs):
    """Substitute the %[0-9] in the command with the subst values."""
    args = arg_list(command)
    if substs:
        for a in range(0, len(args)):
            for r in range(0, len(substs)):
                args[a] = re.compile(('%%%d' % (r))).sub(substs[r], args[a])
    return args

def arg_subst_str(command, subst):
    cmd = arg_subst(command, subst)
    def add(x, y): return x + ' ' + str(y)
    return functools.reduce(add, cmd, '')

class execute:
    """Execute commands or scripts. The 'output' is a funtion
    that handles the output from the process."""
    def __init__(self, output = None, error_prefix = '', verbose = False):
        self.output = output
        self.error_prefix = error_prefix
        self.verbose = verbose
        self.shell_exe = None
        self.shell_commands = False
        self.path = None
        self.environment = None

    def capture(self, proc, timeout = None):
        """Create 2 threads to read stdout and stderr and send to the
        output handler. Based on the 'communicate' code in the subprocess
        module."""
        def _readthread(fh, out, prefix = ''):
            """Read from a file handle and write to the output handler
            until the file closes."""
            count = 0
            while True:
                line = fh.readline()
                # str and bytes are the same type in Python2
                if type(line) is not str and type(line) is bytes:
                    line = line.decode(sys.stdout.encoding)
                count += 1
                if len(line) == 0:
                    break
                if out:
                    out(prefix + line)
                else:
                    log.output(prefix + line)
                    if count > 10:
                        log.flush()
                        count = 0

        def _timerthread(proc, timer):
            """Timer thread calls the timer handler if one
            is present once a second. The user provides a handler
            and returns False to kill the process or True continue."""
            while True:
                time.sleep(1)
                if not timer(proc):
                    proc.stdout.close()
                    proc.stderr.close()

        if proc.stdout:
            stdout_thread = threading.Thread(target = _readthread,
                                             args = (proc.stdout,
                                                     self.output,
                                                     ''))
            stdout_thread.setDaemon(True)
            stdout_thread.start()
        if proc.stderr:
            stderr_thread = threading.Thread(target = _readthread,
                                             args = (proc.stderr,
                                                     self.output,
                                                     self.error_prefix))
            stderr_thread.setDaemon(True)
            stderr_thread.start()
        if proc.stdout:
            stdout_thread.join()
        if proc.stderr:
            stderr_thread.join()
        return proc.wait()

    def open(self, command, capture = True, shell = False,
             cwd = None, env = None,
             stdin = None, stdout = None, stderr = None):
        """Open a command with arguments. Provide the arguments as a list or
        a string."""
        if self.verbose:
            s = command
            if type(command) is list:
                def add(x, y): return x + ' ' + str(y)
                s = functools.reduce(add, command, '')[1:]
            what = 'spawn'
            if shell:
                what = 'shell'
            log.output(what + ': ' + s)
        if shell and self.shell_exe:
            command = arg_list(command)
            command[:0] = self.shell_exe
        if not stdout:
            stdout = subprocess.PIPE
        if not stderr:
            stderr = subprocess.PIPE
        proc = None
        if cwd is None:
            cwd = self.path
        if env is None:
            env = self.environment
        try:
            # Work around a problem on Windows with commands that
            # have a '.' and no extension. Windows needs the full
            # command name.
            if sys.platform == "win32" and type(command) is list:
                if command[0].find('.') >= 0:
                    r, e = os.path.splitext(command[0])
                    if e not in ['.exe', '.com', '.bat']:
                        command[0] = command[0] + '.exe'
            log.trace('exe: %s' % (command))
            proc = subprocess.Popen(command, shell = shell,
                                    cwd = cwd, env = env,
                                    stdin = stdin, stdout = stdout,
                                    stderr = stderr)
            if not capture:
                return (0, proc)
            exit_code = self.capture(proc)
            if self.verbose:
                log.output('exit: ' + str(exit_code))
        except OSError as ose:
            exit_code = ose.errno
            if self.verbose:
                log.output('exit: ' + str(ose))
        return (exit_code, proc)

    def spawn(self, command, capture = True, cwd = None, env = None,
              stdin = None, stdout = None, stderr = None):
        """Spawn a command with arguments. Provide the arguments as a list or
        a string."""
        return self.open(command, capture, False, cwd, env,
                         stdin, stdout, stderr)

    def shell(self, command, capture = True, cwd = None, env = None,
              stdin = None, stdout = None, stderr = None):
        """Execute a command within a shell context. The command can contain
        argumments. The shell is specific to the operating system. For example
        it is cmd.exe on Windows XP."""
        return self.open(command, capture, True, cwd, env,
                         stdin, stdout, stderr)

    def command(self, command, args = None, capture = True, shell = False,
                cwd = None, env = None,
                stdin = None, stdout = None, stderr = None):
        """Run the command with the args. The args can be a list
        or a string."""
        if args and not type(args) is list:
            args = arg_list(args)
        cmd = [command]
        if args:
            cmd.extend(args)
        return self.open(cmd, capture = capture, shell = shell,
                         cwd = cwd, env = env,
                         stdin = stdin, stdout = stdout, stderr = stderr)

    def command_subst(self, command, substs, capture = True, shell = False,
                      cwd = None, env = None,
                      stdin = None, stdout = None, stderr = None):
        """Run the command from the config data with the
        option format string subsituted with the subst variables."""
        args = arg_subst(command, substs)
        return self.command(args[0], args[1:], capture = capture,
                            shell = shell or self.shell_commands,
                            cwd = cwd, env = env,
                            stdin = stdin, stdout = stdout, stderr = stderr)

    def set_shell(self, execute):
        """Set the shell to execute when issuing a shell command."""
        args = arg_list(execute)
        if len(args) == 0 or not os.path.isfile(args[0]):
            raise error.general('could find shell: ' + execute)
        self.shell_exe = args

    def command_use_shell(self):
        """Force all commands to use a shell. This can be used with set_shell
        to allow Unix commands be executed on Windows with a Unix shell such
        as Cygwin or MSYS. This may cause piping to fail."""
        self.shell_commands = True

    def set_output(self, output):
        """Set the output handler. The stdout of the last process in a pipe
        line is passed to this handler."""
        old_output = self.output
        self.output = output
        return old_output

    def set_path(self, path):
        """Set the path changed to before the child process is created."""
        old_path = self.path
        self.path = path
        return old_path

    def set_environ(self, environment):
        """Set the environment passed to the child process when created."""
        old_environment = self.environment
        self.environment = environment
        return old_environment

class capture_execution(execute):
    """Capture all output as a string and return it."""

    class _output_snapper:
        def __init__(self, log = None, dump = False):
            self.output = ''
            self.log = log
            self.dump = dump

        def handler(self, text):
            if not self.dump:
                if self.log is not None:
                    self.log.output(text)
                else:
                    self.output += text

        def get_and_clear(self):
            text = self.output
            self.output = ''
            return text.strip()

    def __init__(self, log = None, dump = False, error_prefix = '', verbose = False):
        self.snapper = capture_execution._output_snapper(log = log, dump = dump)
        execute.__init__(self, output = self.snapper.handler,
                         error_prefix = error_prefix,
                         verbose = verbose)

    def open(self, command, capture = True, shell = False, cwd = None, env = None,
             stdin = None, stdout = None, stderr = None):
        if not capture:
            raise error.general('output capture must be true; leave as default')
        #self.snapper.get_and_clear()
        exit_code, proc = execute.open(self, command, capture = True, shell = shell,
                                       cwd = cwd, env = env,
                                       stdin = stdin, stdout = stdout, stderr = stderr)
        return (exit_code, proc, self.snapper.get_and_clear())

    def set_output(self, output):
        raise error.general('output capture cannot be overrided')

if __name__ == "__main__":
    def run_tests(e, commands, use_shell):
        for c in commands['shell']:
            e.shell(c)
        for c in commands['spawn']:
            e.spawn(c)
        for c in commands['cmd']:
            if type(c) is str:
                e.command(c, shell = use_shell)
            else:
                e.command(c[0], c[1], shell = use_shell)
        for c in commands['csubsts']:
            e.command_subst(c[0], c[1], shell = use_shell)
        ec, proc = e.command(commands['pipe'][0], commands['pipe'][1],
                             capture = False, stdin = subprocess.PIPE)
        if ec == 0:
            print('piping input into ' + commands['pipe'][0] + ': ' + \
                  commands['pipe'][2])
            proc.stdin.write(bytes(commands['pipe'][2], sys.stdin.encoding))
            proc.stdin.close()
            e.capture(proc)
            del proc

    cmd_shell_test = 'if "%OS%" == "Windows_NT" (echo It is WinNT) else echo Is is not WinNT'
    sh_shell_test = 'x="me"; if [ $x = "me" ]; then echo "It was me"; else "It was him"; fi'

    commands = {}
    commands['windows'] = {}
    commands['unix'] = {}
    commands['windows']['shell'] = ['cd', 'dir /w', '.\\xyz', cmd_shell_test]
    commands['windows']['spawn'] = ['hostname', 'hostnameZZ', ['netstat', '/e']]
    commands['windows']['cmd'] = [('ipconfig'), ('nslookup', 'www.python.org')]
    commands['windows']['csubsts'] = [('netstat %0', ['-a']),
                                      ('netstat %0 %1', ['-a', '-n'])]
    commands['windows']['pipe'] = ('ftp', None, 'help\nquit')
    commands['unix']['shell'] = ['pwd', 'ls -las', './xyz', sh_shell_test]
    commands['unix']['spawn'] = ['ls', 'execute.pyc', ['ls', '-i']]
    commands['unix']['cmd'] = [('date'), ('date', '-R'), ('date', ['-u', '+%d %D']),
                               ('date', '-u "+%d %D %S"')]
    commands['unix']['csubsts'] = [('date %0 "+%d %D %S"', ['-u']),
                                   ('date %0 %1', ['-u', '+%d %D %S'])]
    commands['unix']['pipe'] = ('grep', 'hello', 'hello world')

    print(arg_list('cmd a1 a2 "a3 is a string" a4'))
    print(arg_list('cmd b1 b2 "b3 is a string a4'))
    print(arg_subst(['nothing', 'xx-%0-yyy', '%1', '%2-something'],
                    ['subst0', 'subst1', 'subst2']))

    e = execute(error_prefix = 'ERR: ', verbose = True)
    if sys.platform == "win32":
        run_tests(e, commands['windows'], False)
        if os.path.exists('c:\\msys\\1.0\\bin\\sh.exe'):
            e.set_shell('c:\\msys\\1.0\\bin\\sh.exe --login -c')
            commands['unix']['pipe'] = ('c:\\msys\\1.0\\bin\\grep',
                                        'hello', 'hello world')
            run_tests(e, commands['unix'], True)
    else:
        run_tests(e, commands['unix'], False)
    del e
