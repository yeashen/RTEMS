#ifndef	__HI_CHIP_REGS_H__
#define	__HI_CHIP_REGS_H__

#define HI_REG_WR(a, v)		(*(volatile unsigned int *)(a) = (v))
#define HI_REG_RD(a)		(*(volatile unsigned int *)(a))

/*
 * Get frequency of bus clock.
 * Clock for arm9 is supplied by APLL.
 * Clock for DDR/AXI/APB bus is supplied by BPLL.
 * BPLL : DDR : AXI : APB(AHB)
 *    x :  4  :	2   : 1
 */
#define HW_REG(a) (*(unsigned long *)(a))
#define get_bus_clk()({\
	unsigned long fbdiv, refdiv, pstdiv1, pstdiv2; \
	unsigned long tmp_reg, foutvco, busclk;\
	tmp_reg = HW_REG(REG_CRG4_OFFSET);\
	pstdiv1 = (tmp_reg >> 24) & 0x7;\
	pstdiv2 = (tmp_reg >> 27) & 0x7;\
	tmp_reg = HW_REG(REG_CRG5_OFFSET);\
	refdiv = (tmp_reg >> 12) & 0x3f;\
	fbdiv = tmp_reg & 0xfff;\
	foutvco = 24000000/refdiv;\
	foutvco *= fbdiv;\
	busclk = foutvco/(2 * pstdiv1 * pstdiv2);\
	busclk;\
})

#define CFG_CLK_BUS		get_bus_clk()

/* cpu version */
#define SYSID_RGE_BASE					0x20050EE0

/* irq */
#define REG_BASE_INTC					0x10140000
#define REG_INTC_IRQSTATUS		0x000
#define REG_INTC_FIQSTATUS			0x004
#define REG_INTC_RAWSTATUS		0x008
#define REG_INTC_INTSELECT			0x00C
#define REG_INTC_INTENABLE			0x010
#define REG_INTC_INTENCLEAR		0x014
#define REG_INTC_SOFTINT				0x018
#define REG_INTC_SOFTINTCLEAR	0x01C
#define REG_INTC_PROTECTION		0x020
#define INT_RD_REG(offset)				(HI_REG_RD(REG_BASE_INTC+offset))
#define INT_WR_REG(offset, val)		(HI_REG_WR(REG_BASE_INTC+offset, val))

#define DDR_MEM_BASE			0x80000000
#define SFC_MEM_BASE			0x58000000
#define ARM_REG_BASE			0x20300000
#define GPIO11_REG_BASE			0x201F0000
#define GPIO10_REG_BASE			0x201E0000
//#define GPIO9_REG_BASE			0x201D0000
#define GPIO8_REG_BASE			0x201C0000
#define GPIO7_REG_BASE			0x201B0000
#define GPIO6_REG_BASE			0x201A0000
#define GPIO5_REG_BASE			0x20190000
#define GPIO4_REG_BASE			0x20180000
#define GPIO3_REG_BASE			0x20170000
#define GPIO2_REG_BASE			0x20160000
#define GPIO1_REG_BASE			0x20150000
#define GPIO0_REG_BASE			0x20140000
#define PWM_REG_BASE			0x20130000
#define DDR_PHY_BASE			0x20120000
#define DDRC_REG_BASE			0x20110000

#define RTC_REG_BASE			0x20060000
#define REG_BASE_SCTL			0x20050000
#define REG_SC_CTRL			0
#define REG_SC_SYSRES			0x4

#define WDG_REG_BASE			0x20040000
#define CRG_REG_BASE			0x20030000
#define REG_ETH_CRG			0xCC

/***************************MuxPin Ctrl	********************************/
#define IO_CONFIG_REG_BASE		0x200F0000
#define SENSOR_CLK_PIN				0x008
#define I2C_SDA_PIN					0x018
#define I2C_SCL_PIN						0x01C
#define UART1_RXD_PIN				0x024
#define UART1_TXD_PIN				0x028
#define GPIO9_0_PIN					0x0C8
#define GPIO9_1_PIN					0x0CC
#define GPIO9_2_PIN					0x0D0
#define GPIO9_3_PIN					0x0D4
#define GPIO9_4_PIN					0x0D8
#define GPIO9_5_PIN					0x0DC
#define GPIO9_6_PIN					0x0E0
#define GPIO9_7_PIN					0x0E4
#define VIU_CLK_PIN					0x140
#define VIU_VS_PIN						0x144
#define VIU_HS_PIN						0x148
#define VIU_DAT11_PIN				0x14C
#define VIU_DAT10_PIN				0x150
#define VIU_DAT9_PIN					0x154
#define VIU_DAT8_PIN					0x158
#define VIU_DAT7_PIN					0x15C
#define VIU_DAT6_PIN					0x160
#define VIU_DAT5_PIN					0x164
#define VIU_DAT4_PIN					0x168
#define VIU_DAT3_PIN					0x16C
#define VIU_DAT2_PIN					0x170
#define VIU_DAT1_PIN					0x174
#define VIU_DAT0_PIN					0x178
#define MUXPIN_RD_REG(offset)				(HI_REG_RD(IO_CONFIG_REG_BASE+offset))
#define MUXPIN_WR_REG(offset, val)		(HI_REG_WR(IO_CONFIG_REG_BASE+offset, val))

/*************************Timer****************************************/
#define TIMER1_REG_BASE			0x20010000
#define TIMER0_REG_BASE			0x20000000
#define REG_TIMER_RELOAD		0x0
#define REG_TIMER_VALUE			0x4
#define REG_TIMER_CONTROL		0x8
#define REG_TIMER0_INTCLR		0xC

#define CFG_TIMER_ENABLE			(1 << 7)
#define CFG_TIMER_PERIODIC		(1 << 6)
#define CFG_TIMER_INTMASK		(1 << 5)
#define CFG_TIMER_32BIT			(1 << 1)

#define CFG_TIMER_IRQEN			(1<<3)

#define CFG_TIMER_CONTROL		(CFG_TIMER_ENABLE | CFG_TIMER_PERIODIC\
					| CFG_TIMER_INTMASK | CFG_TIMER_32BIT)

#define CFG_TIMER_PRESCALE		2 /* AXI:APB = 2:1 */
#define HZ									100 /* 10ms */
#define BUSCLK_TO_TIMER_RELOAD(busclk)	 (((busclk)/CFG_TIMER_PRESCALE)/HZ)

#define TIMER0_RD_REG(offset)				(HI_REG_RD(TIMER0_REG_BASE+offset))
#define TIMER0_WR_REG(offset, val)		(HI_REG_WR(TIMER0_REG_BASE+offset, val))
#define TIMER1_RD_REG(offset)				(HI_REG_RD(TIMER1_REG_BASE+offset))
#define TIMER1_WR_REG(offset, val)		(HI_REG_WR(TIMER1_REG_BASE+offset, val))


#define REG_BASE_SF			0x10090000
#define SFC_REG_BASE			0x10010000
/* for u-boot enable TCM */
#define MEM_BASE_ITCM			0x00000000
#define MEM_SIZE_ITCM			0x00000800
#define MEM_CONF_ITCM_SIZE		3

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define REG_SYSSTAT			0x8C
/* boot mode */
#define BOOT_FROM_SPI			0

/******************PLL CONFIG START************************************/
/* APLL CONFIG */
#define REG_CRG0_OFFSET			(CRG_REG_BASE + 0)
#define REG_CRG0_VALUE			0x09000000 /* reset value */
#define REG_CRG1_OFFSET			(CRG_REG_BASE + 0x4)
#define REG_CRG1_VALUE			0x006c309B /* reset value */

/* BPLL CONFIG */
#define REG_CRG4_OFFSET			(CRG_REG_BASE + 0x10)
#define REG_CRG5_OFFSET			(CRG_REG_BASE + 0x14)

/* EPLL CONFIG */
#define REG_CRG6_OFFSET			(CRG_REG_BASE + 0x18)
#define REG_CRG7_OFFSET			(CRG_REG_BASE + 0x1c)
#define REG_CRG8_OFFSET			(CRG_REG_BASE + 0x20)

/* ARM9 freq mode A9:AXI CONFIG */
#define REG_CRG10_OFFSET		(CRG_REG_BASE + 0x28)

/* PLL LOCK READ CONFIG */
#define REG_CRG58_OFFSET		(CRG_REG_BASE + 0xE8)

/* APLL WAIT TIME CONFIG */
#define REG_PLLCTL_OFFSET		(REG_BASE_SCTL + 0x14)

/*wait time: 0x2000000-0x1fff000*/
#define BITOFFSET_PLLCTL_PLLTIME	3
#define BITMASK_PLLCTL_PLLTIME		0xf0000007
#define BITVALUE_PLLCTL_PLLTIME		0x1fff000

/*0: apll controlled by mode switching*/
#define BITOFFSET_PLLCTL_PLLOVER	0
#define BITMASK_PLLCTL_PLLOVER		0xfffffffe
#define BITVALUE_PLLCTL_PLLOVER		0x0

#define SC_LOCKEN_OFFSET		(REG_BASE_SCTL + 0x44)
#define SC_LOCKEN_VALUE			0x1acce551

/* nand flash */
#define NANDC_REG_BASE			0x10000000
#define NAND_MEM_BASE			0x50000000
/******************PLL CONFIG END************************************/

/******************SVB VOLTAGE ADJUSTMENT****************************/
#define PWM_FREQ_OFFSET			0x200	/* for pwm frequency */
#define SPACE_RARIO_DEFAULT		0x204	/* for default voltage(1.2v) */

#define MAX_CORNER_AA			0x208
#define MAX_CORNER_BB			0x20c

#define SPACE_RATIO_AA			0x210
#define SPACE_RATIO_BB			0x214
#define SPACE_RATIO_CC			0x218

/******************UART  PL011************************************/
#define UART0_REG_BASE			0x20080000
#define UART1_REG_BASE			0x20090000
#define UART2_REG_BASE			0x200A0000

#define UART_PL01x_DR                   0x00	 /*  Data read or written from the interface. */
#define UART_PL01x_RSR                  0x04	 /*  Receive status register (Read). */
#define UART_PL01x_ECR                  0x04	 /*  Error clear register (Write). */
#define UART_PL01x_FR                   0x18	 /*  Flag register (Read only). */

#define UART_PL01x_RSR_OE               0x08
#define UART_PL01x_RSR_BE               0x04
#define UART_PL01x_RSR_PE               0x02
#define UART_PL01x_RSR_FE               0x01

#define UART_PL01x_FR_TXFE              0x80
#define UART_PL01x_FR_RXFF              0x40
#define UART_PL01x_FR_TXFF              0x20
#define UART_PL01x_FR_RXFE              0x10
#define UART_PL01x_FR_BUSY              0x08
#define UART_PL01x_FR_TMSK              (UART_PL01x_FR_TXFF + UART_PL01x_FR_BUSY)

#define UART_BAUDRATE				115200
#define UART_PL011_CLOCK			3000000 /* hi3518 use fixed clk 3MHZ */

#define UART_PL011_IBRD                 0x24
#define UART_PL011_FBRD                 0x28
#define UART_PL011_LCRH                 0x2C
#define UART_PL011_CR                   0x30
#define UART_PL011_IMSC                 0x38
#define UART_PL011_PERIPH_ID0           0xFE0

#define UART_PL011_LCRH_SPS             (1 << 7)
#define UART_PL011_LCRH_WLEN_8          (3 << 5)
#define UART_PL011_LCRH_WLEN_7          (2 << 5)
#define UART_PL011_LCRH_WLEN_6          (1 << 5)
#define UART_PL011_LCRH_WLEN_5          (0 << 5)
#define UART_PL011_LCRH_FEN             (1 << 4)
#define UART_PL011_LCRH_STP2            (1 << 3)
#define UART_PL011_LCRH_EPS             (1 << 2)
#define UART_PL011_LCRH_PEN             (1 << 1)
#define UART_PL011_LCRH_BRK             (1 << 0)

#define UART_PL011_CR_CTSEN             (1 << 15)
#define UART_PL011_CR_RTSEN             (1 << 14)
#define UART_PL011_CR_OUT2              (1 << 13)
#define UART_PL011_CR_OUT1              (1 << 12)
#define UART_PL011_CR_RTS               (1 << 11)
#define UART_PL011_CR_DTR               (1 << 10)
#define UART_PL011_CR_RXE               (1 << 9)
#define UART_PL011_CR_TXE               (1 << 8)
#define UART_PL011_CR_LPE               (1 << 7)
#define UART_PL011_CR_IIRLP             (1 << 2)
#define UART_PL011_CR_SIREN             (1 << 1)
#define UART_PL011_CR_UARTEN            (1 << 0)

#define UART_PL011_IMSC_OEIM            (1 << 10)
#define UART_PL011_IMSC_BEIM            (1 << 9)
#define UART_PL011_IMSC_PEIM            (1 << 8)
#define UART_PL011_IMSC_FEIM            (1 << 7)
#define UART_PL011_IMSC_RTIM            (1 << 6)
#define UART_PL011_IMSC_TXIM            (1 << 5)
#define UART_PL011_IMSC_RXIM            (1 << 4)
#define UART_PL011_IMSC_DSRMIM          (1 << 3)
#define UART_PL011_IMSC_DCDMIM          (1 << 2)
#define UART_PL011_IMSC_CTSMIM          (1 << 1)
#define UART_PL011_IMSC_RIMIM           (1 << 0)
#define UART_RD_REG(base, offset)				(HI_REG_RD(base+offset))
#define UART_WR_REG(base, offset, val)		(HI_REG_WR(base+offset, val))

/*************************I2C****************************************/
#define I2C_REG_BASE		0x200D0000
#define I2C_CTRL_REG		0x000
#define I2C_COM_REG			0x004
#define I2C_ICR_REG			0x008
#define I2C_SR_REG			0x00C
#define I2C_SCL_H_REG		0x010
#define I2C_SCL_L_REG		0x014
#define I2C_TXR_REG			0x018
#define I2C_RXR_REG			0x01C

#define I2C_RD_REG(offset)				(HI_REG_RD(I2C_REG_BASE+offset))
#define I2C_WR_REG(offset, val)		(HI_REG_WR(I2C_REG_BASE+offset, val))

/*************************CRG****************************************/
#define CRG_REG_BASE		0x20030000
#define CRG_VICAP_CLK		0x002C
#define CRG_SENSOR_CLK	0x0030

#define CRG_RD_REG(offset)			(HI_REG_RD(CRG_REG_BASE+offset))
#define CRG_WR_REG(offset, val)		(HI_REG_WR(CRG_REG_BASE+offset, val))

/*************************VICAP****************************************/
#define VICAP_REG_BASE			0x20580000
#define VICAP_WK_MODE			0x0000
#define VICAP_AXI_CFG			0x0010
#define VICAP_PT_SEL			0x0020
#define VICAP_CH_SEL			0x0030
#define VICAP_APB_TIMEOUT 	0x00E0
#define VICAP_INT_STA			0x00F0
#define VICAP_INT_MASK			0x00F8
#define VICAP_PT_INTF_MOD		0x0100
#define VICAP_PT_OFFSET0		0x0110
#define VICAP_PT_OFFSET1		0x0114
#define VICAP_PT_OFFSET2		0x0118
#define VICAP_TIMMING_CFG	0x0130
#define VICAP_DATA_CFG			0x0140
#define VICAP_PT_HFB			0x0180
#define VICAP_PT_HACT			0x0184
#define VICAP_PT_HBB			0x0188
#define VICAP_PT_VFB			0x018C
#define VICAP_PT_VACT			0x0190
#define VICAP_PT_VBB			0x0194
#define VICAP_PT_VBFB			0x0198
#define VICAP_PT_VBACT			0x019C
#define VICAP_PT_VBBB			0x01A0
#define VICAP_PT_STA			0x01E0
#define VICAP_PT_SIZE			0x01EC
#define VICAP_PT_INT_STA		0x01F0
#define VICAP_PT_INT_MASK		0x01F8
#define VICAP_CH_CTRL			0x1000
#define VICAP_CH_REG_NEW		0x1004
#define VICAP_CH_ADAPTER_CFG 	0x1010
#define VICAP_CH_PACK_Y_CFG	0x1080
#define VICAP_CH_PACK_Y_WIDTH	0x1084
#define VICAP_CH_PACK_C_WIDTH	0x1094
#define VICAP_CH_DES_Y_CFG		0x10B0
#define VICAP_CH_DES_Y_FADDR	0x10B4
#define VICAP_CH_DES_Y_SIZE		0x10B8
#define VICAP_CH_DES_Y_STRIDE	0x10BC
#define VICAP_CH_DES_C_CFG		0x10C0
#define VICAP_CH_DES_C_FADDR	0x10C4
#define VICAP_CH_DES_C_SIZE		0x10C8
#define VICAP_CH_DES_C_STRIDE	0x10CC
#define VICAP_CH_INT_STA		0x10F0
#define VICAP_CH_INT_MASK		0x10F8
#define VICAP_CH_CROP_CFG		0x1100
#define VICAP_CH_CROP_WIN		0x1104
#define VICAP_CROP0_START		0x1110
#define VICAP_CROP0_SIZE		0x1114

#define VICAP_RD_REG(offset)				(HI_REG_RD(VICAP_REG_BASE+offset))
#define VICAP_WR_REG(offset, val)		(HI_REG_WR(VICAP_REG_BASE+offset, val))

#define PT_EN			(1<<31)
#define CH_EN			(1<<31)
#define ISP_INT_EN		(1<<24)
#define PT0_INT_EN		(1<<16)
#define CH0_INT_EN		(1<<0)
#define FIELD_THROW_EN  (1<<3)
#define BUF_OVF_EN		(1<<2)
#define CC_INT_EN		(1<<1)
#define FSTART_EN		(1<<0)

#define CFG_VICAP_IRQEN			(1<<22)

/*************************GPIO****************************************/
#define GPIO9_REG_BASE			0x201D0000
#define GPIO_0_DATA				0x004
#define GPIO_1_DATA				0x008
#define GPIO_2_DATA				0x010
#define GPIO_3_DATA				0x020
#define GPIO_4_DATA				0x040
#define GPIO_5_DATA				0x080
#define GPIO_6_DATA				0x100
#define GPIO_7_DATA				0x200

#define GPIO_DIR				0x400
#define GPIO_IS					0x404
#define GPIO_IBE				0x408
#define GPIO_IEV				0x40C
#define GPIO_IE					0x410
#define GPIO_RIS				0x414
#define GPIO_MIS				0x418
#define GPIO_IC					0x41C

#define GPIO_RD_REG(offset)				(HI_REG_RD(GPIO9_REG_BASE+offset))
#define GPIO_WR_REG(offset, val)		(HI_REG_WR(GPIO9_REG_BASE+offset, val))

#define CFG_GPIO0_IRQEN				(1<<29)
#define CFG_GPIO3_IRQEN				(1<<30)
#define CFG_GPIO6_IRQEN				(1<<31)

#endif /* __HI_CHIP_REGS_H__ */
