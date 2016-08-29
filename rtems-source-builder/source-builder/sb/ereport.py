#
# RTEMS Tools Project (http://www.rtems.org/)
# Copyright 2010-2014 Chris Johns (chrisj@rtems.org)
# All rights reserved.
#
# This file is part of the RTEMS Tools package in 'rtems-testing'.
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
# Create an error log.
#

import os

import error
import log

def generate(name, opts, header = None, footer = None):
    r = ['RTEMS Tools Project - Source Builder Error Report'] + []
    if header:
        r += [' %s' % (header)]
    r += [opts.info()]
    if opts.defaults.get_value('%{_sbgit_valid}') == '1':
        r += [' %s/%s' % (opts.defaults.get_value('%{_sbgit_remotes}'),
                          opts.defaults.get_value('%{_sbgit_id}'))]
    else:
        r += [' RSB: not a valid repo']
    if os.name == 'nt':
        r += [' Windows']
    else:
        r += [' %s' % (' '.join(os.uname()))]
    r += []
    r += ['Tail of the build log:']
    r += log.tail()
    if footer:
        r += [footer]
    try:
        name = name.replace('/', '-')
        l = open(name, 'w')
        l.write(os.linesep.join(r))
        l.close()
        log.notice('  See error report: %s' % (name))
    except:
        log.stderr('error: failure to create error report')
        raise
