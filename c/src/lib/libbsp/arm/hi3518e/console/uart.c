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
#include <bsp.h>

#include <platform.h>
#include <bspopts.h>

#include <libchip/sersupp.h>

#include <bsp/uart.h>


static void hi_uart_initialize(int minor)
{
	unsigned int temp;
	unsigned int divider;
	unsigned int remainder;
	unsigned int fraction;
	unsigned int baseaddr = hi_uart_get_baseaddr(minor);

	/*
	 ** First, disable everything.
	 */
	 UART_WR_REG(baseaddr, UART_PL011_CR, 0x0);
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

	UART_WR_REG(baseaddr, UART_PL011_IBRD, divider);
	UART_WR_REG(baseaddr, UART_PL011_FBRD, fraction);

	/*
	 ** Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled.
	 */
	UART_WR_REG(baseaddr, UART_PL011_LCRH, (UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN));

	/*
	 ** Finally, enable the UART
	 */
	UART_WR_REG(baseaddr, UART_PL011_CR, (UART_PL011_CR_UARTEN | 
			UART_PL011_CR_TXE | UART_PL011_CR_RXE)); 
}


static unsigned int hi_uart_get_baseaddr(int minor)
{
  const console_tbl *ct = Console_Port_Tbl != NULL ?
    Console_Port_Tbl[minor] : &Console_Configuration_Ports[minor];

  return ct->ulCtrlPort1;
}

static int hi_uart_first_open(int major, int minor, void *arg)
{
	rtems_libio_open_close_args_t *oc = (rtems_libio_open_close_args_t *) arg;
	struct rtems_termios_tty *tty = (struct rtems_termios_tty *) oc->iop->data1;
	console_data *cd = &Console_Port_Data[minor];
	const console_tbl *ct = Console_Port_Tbl[minor];

	cd->termios_data = tty;
	rtems_termios_set_initial_baud(tty, (rtems_termios_baud_t) ct->pDeviceParams);

	return 0;
}

static int hi_uart_last_close(int major, int minor, void *arg)
{
  return 0;
}

static int hi_uart_read_polled(int minor)
{
	unsigned int data;
	unsigned int baseaddr = hi_uart_get_baseaddr(minor);

	/* Wait until there is data in the FIFO */
	while(UART_RD_REG(baseaddr, UART_PL01x_FR) & UART_PL01x_FR_RXFE);
	data = UART_RD_REG(baseaddr, UART_PL01x_DR);

	/* Check for an error flag */
	if (data & 0xFFFFFF00) {
		/* Clear the error */
		UART_WR_REG(baseaddr, UART_PL01x_ECR, 0xFFFFFFFF);
		return -1;
	}

	return (int) data;
}

static void hi_uart_write_polled(int minor, char c)
{
	unsigned int baseaddr = hi_uart_get_baseaddr(minor);

	/* Wait until there is space in the FIFO */
	while (UART_RD_REG(baseaddr, UART_PL01x_FR) & UART_PL01x_FR_TXFF);

	/* Send the character */
	UART_WR_REG (baseaddr, UART_PL01x_DR, buf[0]);
}


static ssize_t hi_uart_write_support_polled(int minor, const char *s, size_t n)
{
	ssize_t i = 0;

	for (i = 0; i < n; ++i) {
		hi_uart_write_polled(minor, s[i]);
	}

	return n;
}

static int hi_uart_set_attribues(int minor, const struct termios *term)
{
	return 0;
}

const console_fns hi3518e_uart_fns = {
  .deviceProbe = libchip_serial_default_probe,
  .deviceFirstOpen = hi_uart_first_open,
  .deviceLastClose = hi_uart_last_close,
  .deviceRead = hi_uart_read_polled,
  .deviceWrite = hi_uart_write_support_polled,
  .deviceInitialize = hi_uart_initialize,
  .deviceWritePolled = hi_uart_write_polled,
  .deviceSetAttributes = hi_uart_set_attribues,
  .deviceOutputUsesInterrupts = false
};

