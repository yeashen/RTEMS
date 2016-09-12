#ifndef _IRQ_H_
#define _IRQ_H_

#include <rtems.h>
#include <rtems/irq.h>
#include <rtems/irq-extension.h>

#define BSP_INT_GBL			0
#define BSP_INT_WDT			1
#define BSP_INT_RTC			2
#define BSP_INT_TIMER0_1	3
#define BSP_INT_TIMER2_3	4
#define BSP_INT_UART0_1	5
#define BSP_INT_SSP0			6
#define BSP_INT_SSP1			7
#define _res0          8
#define BSP_INT_SIO			9
#define BSP_INT_TMPCAP	10
#define BSP_INT_SFC			11
#define BSP_INT_ETH			12
#define BSP_INT_CHIPER      13
#define BSP_INT_DMAC		14
#define BSP_INT_USB_EHCI	15
#define BSP_INT_USB_OHCI	16
#define BSP_INT_VPSS			17
#define BSP_INT_SDIO         18
#define BSP_INT_IR_ADC		19
#define BSP_INT_IIC				20
#define BSP_INT_IVE				21
#define BSP_INT_VICAP		22
#define BSP_INT_VOU			23
#define BSP_INT_VEDU			24
#define BSP_INT_UART2		25
#define BSP_INT_JPGE			26
#define BSP_INT_TDE          27
#define BSP_INT_MDU_DDRT			28
#define BSP_INT_GPIO_0_1_2_11		29
#define BSP_INT_GPIO_3_4_5_10		30
#define BSP_INT_GPIO_6_7_8_9		31
#define BSP_MAX_INT          32

#define BSP_INTERRUPT_VECTOR_MIN 0

#define BSP_INTERRUPT_VECTOR_MAX (BSP_MAX_INT - 1)

#endif /* _IRQ_H */
