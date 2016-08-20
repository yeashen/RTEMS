/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : uart.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/03/16
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/03/16
   Author       : Juven
   Modification : Created file

******************************************************************************/
#include <bsp.h>                /* Must be before libio.h */
#include <rtems/libio.h>
#include <termios.h>
#include <rtems/bspIo.h>
#include <assert.h>
#include <rtems/error.h>

#include <libchip/serial.h>
#include <libchip/sersupp.h>

#include <hi3518e.h>
#include <uart.h>

/* static function prototypes */
static int     hi_uart_first_open(int major, int minor, void *arg);
static int     hi_uart_last_close(int major, int minor, void *arg);
static int     hi_uart_read(int minor);
static ssize_t hi_uart_write(int minor, const char *buf, size_t len);
static void    hi_uart_initialize(int minor);
static void    hi_uart_write_polled(int minor, char c);
int  hi_uart_read_polled(int minor);
static int     hi_uart_set_attributes(int minor, const struct termios *t);

/* Pointers to functions for handling the UART. */
const console_fns hi_uart_fns = {
  .deviceProbe = libchip_serial_default_probe,
  .deviceFirstOpen = hi_uart_first_open,
  .deviceLastClose = hi_uart_last_close,
  .deviceRead = hi_uart_read,
  .deviceWrite = hi_uart_write,
  .deviceInitialize = hi_uart_initialize,
  .deviceWritePolled = hi_uart_write_polled,
  .deviceSetAttributes = hi_uart_set_attributes,
  .deviceOutputUsesInterrupts = false
};


/*
 * There's one item in array for each UART.
 *
 * Some of these fields are marked "NOT USED". They are not used
 * by console.c, but may be used by drivers in libchip
 *
 */
console_tbl Console_Configuration_Ports[] = {
	{
		.sDeviceName = "/dev/com0",
		.deviceType = SERIAL_CUSTOM,
		.pDeviceFns = &hi_uart_fns,
		.deviceProbe = NULL,
		.pDeviceFlow = NULL,
		.ulMargin = 0,
		.ulHysteresis = 0,
		.pDeviceParams = (void *) 115200,
		.ulCtrlPort1 = UART0_REG_BASE,
		.ulCtrlPort2 = 0,
		.ulDataPort = 0,
		.getRegister = NULL,
		.setRegister = NULL,
		.getData = NULL,
		.setData = NULL,
		.ulClock = 0,
		.ulIntVector = 0
	},
	{
		.sDeviceName = "/dev/com1",
		.deviceType = SERIAL_CUSTOM,
		.pDeviceFns = &hi_uart_fns,
		.deviceProbe = NULL,
		.pDeviceFlow = NULL,
		.ulMargin = 0,
		.ulHysteresis = 0,
		.pDeviceParams = (void *) 115200,
		.ulCtrlPort1 = UART1_REG_BASE,
		.ulCtrlPort2 = 0,
		.ulDataPort = 0,
		.getRegister = NULL,
		.setRegister = NULL,
		.getData = NULL,
		.setData = NULL,
		.ulClock = 0,
		.ulIntVector = 0
	},
	{
		.sDeviceName = "/dev/com2",
		.deviceType = SERIAL_CUSTOM,
		.pDeviceFns = &hi_uart_fns,
		.deviceProbe = NULL,
		.pDeviceFlow = NULL,
		.ulMargin = 0,
		.ulHysteresis = 0,
		.pDeviceParams = (void *) 115200,
		.ulCtrlPort1 = UART2_REG_BASE,
		.ulCtrlPort2 = 0,
		.ulDataPort = 0,
		.getRegister = NULL,
		.setRegister = NULL,
		.getData = NULL,
		.setData = NULL,
		.ulClock = 0,
		.ulIntVector = 0
	}
};

/* These are used by code in console.c */
unsigned long Console_Configuration_Count = RTEMS_ARRAY_SIZE(Console_Configuration_Ports);

typedef struct {
    int minor;
    volatile hi_uart_regs_s * regs;
    volatile const char *buf;
    volatile int len;
    volatile int idx;
    void *tty;
} hi_uart_data_s;

static hi_uart_data_s hi_uart_data[3];
static int uart_inited_flag = 0;

/*********************************************************************/
/* Functions called via termios callbacks (i.e. the ones in uart_fns */
/*********************************************************************/

/*
 * This is called the first time each device is opened. If the driver
 * is interrupt driven, you should enable interrupts here. Otherwise,
 * it's probably safe to do nothing.
 *
 * Since micromonitor already set up the UART, we do nothing.
 */
static int hi_uart_first_open(int major, int minor, void *arg)
{
    return 0;
}


/*
 * This is called the last time each device is closed. If the driver
 * is interrupt driven, you should disable interrupts here. Otherwise,
 * it's probably safe to do nothing.
 */
static int hi_uart_last_close(int major, int minor, void *arg)
{
    return 0;
}


/*
 * Read one character from UART.
 *
 * Return -1 if there's no data, otherwise return
 * the character in lowest 8 bits of returned int.
 */
static int hi_uart_read(int minor)
{
	unsigned int data;

	/* Wait until there is data in the FIFO */
	while(hi_uart_data[minor].regs->fr & UART_PL01x_FR_RXFE);
	data = hi_uart_data[minor].regs->dr;

	/* Check for an error flag */
	if (data & 0xFFFFFF00) {
		/* Clear the error */
		hi_uart_data[minor].regs->rsr = 0xFFFFFFFF;
		return -1;
	}

	return (int) data;
}


/*
 * Write buffer to UART
 *
 * return 1 on success, -1 on error
 */
static ssize_t hi_uart_write(int minor, const char *buf, size_t len)
{
	int i;

	if(!uart_inited_flag){
		hi_uart_initialize(minor);
	}	

	 for (i = 0; i < len; i++){
		/* Wait until there is space in the FIFO */
		while(hi_uart_data[minor].regs->fr & UART_PL01x_FR_TXFF);
		/* Send the character */
		hi_uart_data[minor].regs->dr = buf[i];
	 }

		return 1;
}

static void hi_uart_set_baud(int minor, int baud)
{
	unsigned int temp;
	unsigned int divider;
	unsigned int remainder;
	unsigned int fraction;

	temp = 16 * baud;
	divider = UART_PL011_CLOCK / temp;
	remainder = UART_PL011_CLOCK % temp;
	temp = (8 * remainder) / UART_BAUDRATE;
	fraction = (temp >> 1) + (temp & 1);

	hi_uart_data[minor].regs->ibrd = divider;
	hi_uart_data[minor].regs->fbrd = fraction;
}

/* Set up the UART. */
static void hi_uart_initialize(int minor)
{
	if(uart_inited_flag){
		return;
	}	

	hi_uart_data[minor].minor = minor;
	hi_uart_data[minor].buf 	= NULL;
	hi_uart_data[minor].len 	= 0;
	hi_uart_data[minor].idx 	= 0;

	switch(minor)
	{
		case 0:
			hi_uart_data[minor].regs = (hi_uart_regs_s *) UART0_REG_BASE;
			break;
		case 1:
			hi_uart_data[minor].regs = (hi_uart_regs_s *) UART1_REG_BASE;
			break;
		case 2:
			hi_uart_data[minor].regs = (hi_uart_regs_s *) UART2_REG_BASE;
			break;
		default:
			rtems_panic("%s:%d Unknown UART minor number %d\n", __FUNCTION__, __LINE__, minor);
			break;
	}
	
	/*
	 ** First, disable everything.
	 */
	 hi_uart_data[minor].regs->cr = 0x0;

	/*
		** Set baudrate
		*/
	hi_uart_set_baud(minor, 115200);	

	/*
	 ** Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled.
	 */
	hi_uart_data[minor].regs->lcr_h = UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN;

#if USE_INTERRUPTS
	hi_uart_data[minor].regs->imsc = UART_PL011_IMSC_RXIM|UART_PL011_IMSC_TXIM;
#endif

	/*
	 ** Finally, enable the UART
	 */
	hi_uart_data[minor].regs->cr = UART_PL011_CR_UARTEN | UART_PL011_CR_TXE | UART_PL011_CR_RXE;

	uart_inited_flag = 1;
}


/* I'm not sure this is needed for the shared console driver. */
static void    hi_uart_write_polled(int minor, char c)
{
    hi_uart_write(minor, &c, 1);
}

/* This is for setting baud rate, bits, etc. */
static int     hi_uart_set_attributes(int minor, const struct termios *t)
{
    return 0;
}

/***********************************************************************/
/*
 * The following functions are not used by TERMIOS, but other RTEMS
 * functions use them instead.
 */
/***********************************************************************/
/*
 * Read from UART. This is used in the exit code, and can't
 * rely on interrupts.
*/
int hi_uart_read_polled(int minor)
{
    return hi_uart_read(minor);
}


/*
 * Write a character to the console. This is used by printk() and
 * maybe other low level functions. It should not use interrupts or any
 * RTEMS system calls. It needs to be very simple
 */
static void _BSP_put_char( char c ) {
    hi_uart_write_polled(0, c);
    if (c == '\n') {
        hi_uart_write_polled(0, '\r');
    }
}

BSP_output_char_function_type BSP_output_char = _BSP_put_char;

static int _BSP_get_char(void)
{
  return hi_uart_read_polled(0);
}

BSP_polling_getchar_function_type BSP_poll_char = _BSP_get_char;
