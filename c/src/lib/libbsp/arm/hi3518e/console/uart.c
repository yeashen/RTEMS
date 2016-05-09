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

#include <libchip/serial.h>
#include <libchip/sersupp.h>

#include <platform.h>

/* How many serial ports? */
#define NUM_DEVS       1

int     uart_poll_read(int minor);

int dbg_dly;

/* static function prototypes */
static int     uart_first_open(int major, int minor, void *arg);
static int     uart_last_close(int major, int minor, void *arg);
static int     uart_read(int minor);
static ssize_t uart_write(int minor, const char *buf, size_t len);
static void    uart_init(int minor);
static void    uart_write_polled(int minor, char c);
static int     uart_set_attributes(int minor, const struct termios *t);

/* These are used by code in console.c */
unsigned long Console_Configuration_Count = NUM_DEVS;

/* Pointers to functions for handling the UART. */
const console_fns uart_fns =
{
    libchip_serial_default_probe,
    uart_first_open,
    uart_last_close,
    uart_read,
    uart_write,
    uart_init,
    uart_write_polled,   /* not used in this driver */
    uart_set_attributes,
    FALSE      /* TRUE if interrupt driven, FALSE if not. */
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
        "/dev/com0",                      /* sDeviceName */
        SERIAL_CUSTOM,                    /* deviceType */
        &uart_fns,                        /* pDeviceFns */
        NULL,                             /* deviceProbe */
        NULL,                             /* pDeviceFlow */
        0,                                /* ulMargin - NOT USED */
        0,                                /* ulHysteresis - NOT USED */
        NULL,          					/* pDeviceParams */
        UART0_REG_BASE,      /* ulCtrlPort1  - NOT USED */
        0,                                /* ulCtrlPort2  - NOT USED */
        0,                                /* ulDataPort  - NOT USED */
        NULL,                             /* getRegister - NOT USED */
        NULL,                             /* setRegister - NOT USED */
        NULL,                             /* getData - NOT USED */
        NULL,                             /* setData - NOT USED */
        0,                                /* ulClock - NOT USED */
        0                                 /* ulIntVector - NOT USED */
    }
};

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
static int uart_first_open(int major, int minor, void *arg)
{
    return 0;
}


/*
 * This is called the last time each device is closed. If the driver
 * is interrupt driven, you should disable interrupts here. Otherwise,
 * it's probably safe to do nothing.
 */
static int uart_last_close(int major, int minor, void *arg)
{
    return 0;
}


/*
 * Read one character from UART.
 *
 * Return -1 if there's no data, otherwise return
 * the character in lowest 8 bits of returned int.
 */
static int uart_read(int minor)
{
	unsigned int data;

	if (minor == 0) {
		/* Wait until there is data in the FIFO */
		while(UART_RD_REG(UART_PL01x_FR) & UART_PL01x_FR_RXFE);
		data = UART_RD_REG(UART_PL01x_DR);

		/* Check for an error flag */
		if (data & 0xFFFFFF00) {
			/* Clear the error */
			UART_WR_REG(UART_PL01x_ECR, 0xFFFFFFFF);
			return -1;
		}

		return (int) data;
	} else {
		printk("Unknown console minor number: %d\n", minor);
		return -1;
	}
}



/*
 * Write buffer to UART
 *
 * return 1 on success, -1 on error
 */
static ssize_t uart_write(int minor, const char *buf, size_t len)
{
	/* Wait until there is space in the FIFO */
	while (UART_RD_REG(UART_PL01x_FR) & UART_PL01x_FR_TXFF);

	/* Send the character */
	UART_WR_REG (UART_PL01x_DR, buf[0]);

    return 1;
}


/* Set up the UART. */
static void uart_init(int minor)
{
	unsigned int temp;
	unsigned int divider;
	unsigned int remainder;
	unsigned int fraction;

	/*
	 ** First, disable everything.
	 */
	 UART_WR_REG(UART_PL011_CR, 0x0);
	/*
	 ** Set baud rate
	 **
	 ** IBRD = UART_CLK / (16 * BAUD_RATE)
	 ** FBRD = ROUND((64 * MOD(UART_CLK,(16 * BAUD_RATE))) / (16 * BAUD_RATE))
	 */
	temp = 16 * UART_BAUDRATE;
	divider = UART_PL011_CLOCK / temp;
	remainder = UART_PL011_CLOCK % temp;
	temp = (8 * remainder) / UART_BAUDRATE;
	fraction = (temp >> 1) + (temp & 1);

	UART_WR_REG(UART_PL011_IBRD, divider);
	UART_WR_REG(UART_PL011_FBRD, fraction);

	/*
	 ** Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled.
	 */
	UART_WR_REG(UART_PL011_LCRH, (UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN));

	/*
	 ** Finally, enable the UART
	 */
	UART_WR_REG(UART_PL011_CR, (UART_PL011_CR_UARTEN | 
			UART_PL011_CR_TXE | UART_PL011_CR_RXE)); 
}

/* I'm not sure this is needed for the shared console driver. */
static void    uart_write_polled(int minor, char c)
{
    uart_write(minor, &c, 1);
}

/* This is for setting baud rate, bits, etc. */
static int     uart_set_attributes(int minor, const struct termios *t)
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
int uart_poll_read(int minor)
{
    return uart_read(minor);
}


/*
 * Write a character to the console. This is used by printk() and
 * maybe other low level functions. It should not use interrupts or any
 * RTEMS system calls. It needs to be very simple
 */
static void _BSP_put_char( char c ) {
    uart_write_polled(0, c);
    if (c == '\n') {
        uart_write_polled(0, '\r');
    }
}

BSP_output_char_function_type BSP_output_char = _BSP_put_char;

static int _BSP_get_char(void)
{
  return uart_poll_read(0);
}

BSP_polling_getchar_function_type BSP_poll_char = _BSP_get_char;
