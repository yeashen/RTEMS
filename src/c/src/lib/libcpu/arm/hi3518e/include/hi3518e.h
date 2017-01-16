#ifndef	__HI_CHIP_REGS_H__
#define	__HI_CHIP_REGS_H__

#include <rtems.h>

//#define HI3518EV100
//#define HI3518EV200_DEMO
#define HI3518EV200_ASHU

#define HI_REG_WR(a, v)		(*( unsigned int *)(a) = (v))
#define HI_REG_RD(a)		(*( unsigned int *)(a))

/*************************System****************************************/

/*
 * Get frequency of bus clock.
 * Clock for arm9 is supplied by APLL.
 * Clock for DDR/AXI/APB bus is supplied by BPLL.
 * BPLL : DDR : AXI : APB(AHB)
 *    x :  4  :	2   : 1
 */
#define HW_REG(a) (*(unsigned long *)(a))
#ifdef HI3518EV100
#define UART_PL011_CLOCK			3000000 /* hi3518 use fixed clk 3MHZ */
/* BPLL CONFIG */
#define REG_CRG4_OFFSET			(CRG_REG_BASE + 0x10)
#define REG_CRG5_OFFSET			(CRG_REG_BASE + 0x14)
#define CRG_VICAP_CLK				0x002C
#define CRG_SENSOR_CLK			0x0030

#define SCPER_LOCK					(0x1ACCE551)

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
#else defined(HI3518EV200_DEMO) || defined(HI3518EV200_ASHU)
#define UART_PL011_CLOCK			24000000 /* Except bootrom, hi3518ev200 UART use XTAL OSC clk 24M */
#define CRG_PERI11		0x002C

/*-----------------------------------------------------------------------
 * cpu_init configuration
 * if bit[0] = 0b, AXI = 24M
 * if bit[0] = 1b, AXI = 198M
 *----------------------------------------------------------------------*/
#define get_bus_clk() ({\
	unsigned long tmp_reg, busclk = 0;\
	tmp_reg = HW_REG(CRG_REG_BASE+0x30);\
	tmp_reg &= 0x1;\
	if (0x0 == tmp_reg) {\
		busclk = 24000000;\
	} else {\
		busclk = 198000000;\
	} \
	busclk;\
	})
#endif

#define CFG_CLK_BUS		get_bus_clk()

/*************************Base Address****************************************/
#define CRG_REG_BASE			0x20030000
/* System */
#define SYSCTRL_REG_BASE			0x20050000
/* PinMux */
#define PINMUX_REG_BASE			0x200F0000
/* Interrupt */
#ifdef HI3518EV100
#define IRQ_REG_BASE					0x10140000
#else defined(HI3518EV200_DEMO) || defined(HI3518EV200_ASHU)
#define IRQ_REG_BASE					0x100D0000
#endif
/* Timer */
#define TIMER0_REG_BASE			0x20000000
#define TIMER1_REG_BASE			0x20010000
/* UART */
#define UART0_REG_BASE			0x20080000
#define UART1_REG_BASE			0x20090000
#define UART2_REG_BASE			0x200A0000
/* I2C */
#define I2C_REG_BASE					0x200D0000
/* Vicap */
#define VI_REG_BASE					0x20580000
/* GPIO */
#define GPIO0_REG_BASE			0x20140000
#define GPIO1_REG_BASE			0x20150000
#define GPIO2_REG_BASE			0x20160000
#define GPIO3_REG_BASE			0x20170000
#define GPIO4_REG_BASE			0x20180000
#define GPIO5_REG_BASE			0x20190000
#define GPIO6_REG_BASE			0x201A0000
#define GPIO7_REG_BASE			0x201B0000
#define GPIO8_REG_BASE			0x201C0000
#define GPIO9_REG_BASE			0x201D0000
#define GPIO10_REG_BASE		0x201E0000
#define GPIO11_REG_BASE		0x201F0000
/* USB */
#define USB_EHCI_REG_BASE 	0x100B0000
#define USB_OHCI_REG_BASE 	0x100A0000
/*************************End Base Address************************************/

/*************************Reg Structure****************************************/
/* System @0x20050000 */
typedef struct{
	 uint32_t sc_ctrl;		/* 0x000 */
	 uint32_t sc_reset;	/* 0x004 */
	 uint32_t int_ctrl;		/* 0x008 */
	 uint32_t int_stat;	/* 0x00C */
	 uint32_t xtal_ctrl;	/* 0x010 */
	 uint32_t pll_ctrl;		/* 0x014 */
	 uint8_t res_0x018[0x044-0x018]; /* 0x018*/
	 uint32_t sc_lock;		/* 0x044 */
}hi_sysctrl_regs_s;

/* PinMux @0x200F0000 */
typedef struct{
#ifdef HI3518EV100
	 uint8_t res_0x000[0x008-0x000];	/* 0x000 */
	 uint32_t sns_clk;		/* 0x008 */
	 uint32_t spi0_sck;	/* 0x00C */
	 uint32_t spi0_sdo;	/* 0x010 */
	 uint32_t spi0_sdi;	/* 0x014 */
	 uint32_t i2c_sda;		/* 0x018 */
	 uint32_t i2c_scl;		/* 0x01C */
	 uint8_t res_0x020[0x0C8-0x020]; /* 0x020 */
	 uint32_t gpio9_0;	/* 0x0C8 */
	 uint32_t gpio9_1;	/* 0x0CC */
	 uint32_t gpio9_2;	/* 0x0D0 */
	 uint32_t gpio9_3;	/* 0x0D4 */
	 uint32_t gpio9_4;	/* 0x0D8 */
	 uint32_t gpio9_5;	/* 0x0DC */
	 uint32_t gpio9_6;	/* 0x0E0 */
	 uint32_t gpio9_7;	/* 0x0E4 */
	 uint8_t res_0x0e8[0x108-0x0E8]; /* 0x0E8 */
	 uint32_t uart2_rxd;	/* 0x108 */
	 uint32_t uart2_txd; /* 0x10C */
	 uint8_t res_0x110[0x134-0x110]; /* 0x110 */
	 uint32_t gpio0_5;	/* 0x134 */
	 uint32_t gpio0_6;	/* 0x138 */
	 uint32_t gpio0_7;	/* 0x13C */
	 uint32_t vi_clk;		/* 0x140 */
	 uint32_t vi_vs;			/* 0x144 */
	 uint32_t vi_hs;			/* 0x148 */
	 uint32_t vi_dat11;	/* 0x14C */
	 uint32_t vi_dat10;	/* 0x150 */
	 uint32_t vi_dat9;		/* 0x154 */
	 uint32_t vi_dat8;		/* 0x158 */
	 uint32_t vi_dat7;		/* 0x15C */
	 uint32_t vi_dat6;		/* 0x160 */
	 uint32_t vi_dat5;		/* 0x164 */
	 uint32_t vi_dat4;		/* 0x168 */
	 uint32_t vi_dat3;		/* 0x16C */
	 uint32_t vi_dat2;		/* 0x170 */
	 uint32_t vi_dat1;		/* 0x174 */
	 uint32_t vi_dat0;		/* 0x178 */
#else defined(HI3518EV200_DEMO) || defined(HI3518EV200_ASHU)
	 uint32_t sns_clk;		/* 0x000 */
	 uint32_t sns_rstn;	/* 0x004 */
	 uint32_t flash_trig;	/* 0x008 */
	 uint32_t shutter_trig;	/* 0x00C */
	 uint8_t res_0x010[0x040-0x010];	 /* 0x010 */
	 uint32_t spi0_sck;	/* 0x040 */
	 uint32_t spi0_sdo;	/* 0x044 */
	 uint32_t spi0_sdi;	/* 0x048 */
	 uint32_t spi0_csn;	/* 0x04C */
	 uint32_t spi1_sck;	/* 0x050 */
	 uint32_t spi1_sdo;	/* 0x054 */
	 uint32_t spi1_sdi;	/* 0x058 */
	 uint32_t spi1_csn;	/* 0x05C */
	 uint32_t i2c2_sda;	/* 0x060 */
	 uint32_t i2c2_scl;		/* 0x064 */
	 uint32_t usb_ovrcur;	/* 0x068 */
	 uint32_t usb_pwren;	/* 0x06C */
	 uint32_t gpio0_0;	/* 0x070 */
	 uint32_t gpio0_1;	/* 0x074 */
	 uint32_t gpio0_2;	/* 0x078 */
	 uint32_t gpio1_0;	/* 0x07C */
	 uint32_t gpio1_1;	/* 0x080 */
	 uint32_t gpio1_2;	/* 0x084 */
	 uint32_t gpio1_3;	/* 0x088 */
	 uint32_t gpio1_4;	/* 0x08C */
	 uint32_t gpio1_5;	/* 0x090 */
	 uint32_t gpio1_6;	/* 0x094 */
	 uint8_t res_0x098[0x0CC-0x098];	/* 0x098 */
	 uint32_t uart2_rxd;	/* 0x0CC */
	 uint32_t uart2_txd; /* 0x0D0 */
	 uint8_t res_0x0d4[0x0F8-0x0D4];	/* 0x0D4 */
	 uint32_t adc_ch0;	/* 0x0F8 */
#endif

}hi_pinmux_regs_s;

/* Interrupt @0x10140000 */
typedef struct{
	 uint32_t irq_status;		/* 0x00 */
	 uint32_t fiq_status;		/* 0x04 */
	 uint32_t raw_int;			/* 0x08 */
	 uint32_t src;				/* 0x0C */
	 uint32_t enable;			/* 0x10 */
	 uint32_t en_clr;			/* 0x14 */
	 uint32_t softint;			/* 0x18 */
	 uint32_t softclr;			/* 0x1C */
	 uint32_t protect;			/* 0x20 */
}hi_irq_regs_s;

/* Timer @0x20000000/0x20010000 */
typedef struct{							/*  T0/T2 - T1/T3 */
	  uint32_t load;			/* 0x00 - 0x20 */
	  uint32_t value;		/* 0x04 - 0x24 */
	  uint32_t ctrl;			/* 0x08 - 0x28 */
	  uint32_t icr;			/* 0x0C - 0x2C */
	  uint32_t ris;			/* 0x10 - 0x30 */
	  uint32_t mis;			/* 0x14 - 0x34 */
	  uint32_t bgload;		/* 0x18 - 0x38 */
}hi_timer_regs_s;

/* UART @0x20080000 */
typedef struct {
     uint32_t dr;				/* 0x00 */
	 uint32_t rsr;			/* 0x04 */
	 uint8_t res_0x08[0x18-0x08]; /* 0x08 */
	 uint32_t fr;				/* 0x18 */
	 uint8_t res_0x1c[0x24-0x1C]; /* 0x1C */
	 uint32_t ibrd;			/* 0x24 */
	 uint32_t fbrd;			/* 0x28 */
	 uint32_t lcr_h;			/* 0x2C */
	 uint32_t cr;				/* 0x30 */
	 uint32_t ifls;			/* 0x34 */
	 uint32_t imsc;			/* 0x38 */
	 uint32_t ris;				/* 0x3C */
	 uint32_t mis;			/* 0x40 */
	 uint32_t icr;				/* 0x44 */
	 uint32_t dmacr;		/* 0x48 */
} hi_uart_regs_s;
 
/* I2C @0x200D0000 */
typedef struct{
#ifdef HI3518EV100
	 uint32_t ctrl;			/* 0x00 */
	 uint32_t com;			/* 0x04 */
	 uint32_t icr;				/* 0x08 */
	 uint32_t sr;				/* 0x0C */
	 uint32_t scl_h;			/* 0x10 */
	 uint32_t scl_l;			/* 0x14 */
	 uint32_t txr;			/* 0x18 */
	 uint32_t rxr;			/* 0x1C */
#else defined(HI3518EV200_DEMO) || defined(HI3518EV200_ASHU)
	 uint32_t ctrl;			/* 0x00 */
	 uint32_t tar;			/* 0x04 */
	 uint8_t res_0x08[0x10-0x08]; /* 0x08 */
	 uint32_t data;			/* 0x10 */
	 uint8_t res_0x14[0x1C-0x14]; /* 0x14 */
	 uint32_t scl_h;			/* 0x1C */
	 uint32_t scl_l;			/* 0x20 */
	 uint8_t res_0x24[0x2C-0x24]; /* 0x24 */
	 uint32_t int_sta;		/* 0x2C */
	 uint32_t int_mask;	/* 0x30 */
	 uint32_t int_raw;		/* 0x34 */
	 uint32_t rx_tl;			/* 0x38 */
	 uint32_t tx_tl;			/* 0x3C */
	 uint32_t int_clr;		/* 0x40 */
	 uint8_t res_0x44[0x6C-0x44]; /* 0x44 */
	 uint32_t en;				/* 0x6C */
	 uint32_t stat;			/* 0x70 */
	 uint32_t txflr;			/* 0x74 */
	 uint32_t rxflr;			/* 0x78 */
	 uint32_t sda_hold;	/* 0x7C */
	 uint32_t tx_src;		/* 0x80 */
	 uint32_t res4;			/* 0x84 */
	 uint32_t dma_cr;		/* 0x88 */
	 uint32_t dma_tdlr;	/* 0x8C */
	 uint32_t dma_rdlr;	/* 0x90 */
	 uint8_t res_0x94[0xA0-0x94]; /* 0x94 */
	 uint32_t scl_switch;		/* 0xA0 */
	 uint32_t scl_sim;			/* 0xA4 */
	 uint32_t res6;			/* 0xA8 */
	 uint32_t lock;			/* 0xAC */
	 uint32_t mst_ctrl;	/* 0xB0 */
	 uint32_t mst_cmd;	/* 0xB4 */
	 uint32_t mst_cmd0;	/* 0xB8 */
	 uint32_t mst_cmd1;	/* 0xBC */
	 uint32_t mst_cmd2;	/* 0xC0 */
#endif
}hi_i2c_regs_s;

/* VI @0x20580000 */
typedef struct{
#ifdef HI3518EV100
	 uint32_t lowpower;		/* 0x0000 */
	 uint8_t res_0x0004[0x0010-0x0004]; /* 0x0004 */
	 uint32_t axi;				/* 0x0010 */
	 uint8_t res_0x0014[0x0020-0x0014]; /* 0x0014 */
	 uint32_t pt_sel;			/* 0x0020 */
	 uint8_t res_0x0024[0x0030-0x0024]; /* 0x0024 */
	 uint32_t ch_sel;			/* 0x0030 */
	 uint8_t res_0x0034[0x00E0-0x0034]; /* 0x0034 */
	 uint32_t apb;				/* 0x00E0 */
	 uint8_t res_0x00e4[0x00F0-0x00E4]; /* 0x00E4 */
	 uint32_t g_int;				/* 0x00F0 */
	 uint8_t res_0x00f4[0x00F8-0x00F4];	 /* 0x00F4 */
	 uint32_t g_int_en;		/* 0x00F8 */
	 uint8_t res_0x00fc[0x0100-0x00FC];	 /* 0x00FC */
	 uint32_t pt_mode;		/* 0x0100 */
	 uint8_t res_0x0104[0x0110-0x0104]; /* 0x0104 */
	 uint32_t pt_offset0;	/* 0x0110 */
	 uint32_t pt_offset1;	/* 0x0114 */
	 uint32_t pt_offset2;	/* 0x0118 */
	 uint8_t res_0x011c[0x0130-0x011C]; /* 0x011C*/
	 uint32_t timing_cfg;	/* 0x0130 */
	 uint8_t res_0x0134[0x0140-0x0134]; /* 0x0134 */
	 uint32_t data_cfg;		/* 0x0140 */
	 uint8_t res_0x0144[0x0180-0x0144]; /* 0x0144 */
	 uint32_t hfb;				/* 0x0180 */
	 uint32_t hact;				/* 0x0184 */
	 uint32_t hbb;				/* 0x0188 */
	 uint32_t vfb;				/* 0x018C */
	 uint32_t vact;				/* 0x0190 */
	 uint32_t vbb;				/* 0x0194 */
	 uint32_t vbfb;				/* 0x0198 */
	 uint32_t vbact;			/* 0x019C */
	 uint32_t vbbb;				/* 0x01A0 */
	 uint8_t res_0x01A4[0x01E0-0x01A4]; /* 0x01A4 */
	 uint32_t pt_status;		/* 0x01E0 */
	 uint32_t res12;			/* 0x01E4 */
	 uint32_t res13;			/* 0x01E8 */
	 uint32_t pt_size;			/* 0x01EC */
	 uint32_t pt_int;			/* 0x01F0 */
	 uint32_t res14;			/* 0x01F4 */
	 uint32_t pt_int_en;		/* 0x01F8 */
	 uint8_t res_0x01fc[0x1000-0x01FC]; /* 0x01FC */
	 uint32_t ch_ctrl;			/* 0x1000 */
	 uint32_t ch_newer;		/* 0x1004 */
	 uint8_t res_0x1008[0x1010-0x1008]; /* 0x1008 */
	 uint32_t adapter;		/* 0x1010 */
	 uint8_t res_0x1014[0x1080-0x1014]; /* 0x1014 */
	 uint32_t pack_y;			/* 0x1080 */
	 uint32_t pack_y_width;	/* 0x1084 */
	 uint8_t res_0x1088[0x1090-0x1088]; /* 0x1088 */
	 uint32_t pack_c;			/* 0x1090 */
	 uint32_t pack_c_width;	/* 0x1094 */
	 uint8_t res_0x1098[0x10B0-0x1098]; /* 0x1098 */
	 uint32_t des_y;			/* 0x10B0 */
	 uint32_t des_y_addr;	/* 0x10B4 */
	 uint32_t des_y_size;	/* 0x10B8 */
	 uint32_t des_y_stride;	/* 0x10BC */
	 uint32_t des_c;			/* 0x10C0 */
	 uint32_t des_c_addr;	/* 0x10C4 */
	 uint32_t des_c_size;	/* 0x10C8 */
	 uint32_t des_c_stride;	/* 0x10CC */
	 uint8_t res_0x10d0[0x10F0-0x10D0]; /* 0x10D0 */
	 uint32_t ch_int;			/* 0x10F0 */
	 uint32_t res21;			/* 0x10F4 */
	 uint32_t ch_int_en;		/* 0x10F8 */
	 uint32_t res22;			/* 0x10FC */
	 uint32_t crop_cfg;		/* 0x1100 */
	 uint32_t crop_win;		/* 0x1104 */
	 uint8_t res_0x1108[0x1110-0x1108]; /* 0x1108 */
	 uint32_t crop_start;	/* 0x1110 */
	 uint32_t crop_size;		/* 0x1114 */
#else defined(HI3518EV200_DEMO) || defined(HI3518EV200_ASHU)
	 uint32_t wk_mode;			/* 0x0000 */
	 uint8_t res_0x0004[0x0010-0x0004]; /* 0x0004 */
	 uint32_t axi;				/* 0x0010 */
	 uint32_t mac;				/* 0x0014 */
	 uint8_t res_0x0018[0x0040-0x0018]; /* 0x0018 */
	 uint32_t des_sel;			/* 0x0040 */
	 uint8_t res_0x0044[0x0050-0x0044]; /* 0x0044 */
	 uint32_t isp_sel;			/* 0x0050 */
	 uint8_t res_0x0054[0x0060-0x0054]; /* 0x0054 */
	 uint32_t chn_mode;			/* 0x0060 */
	 uint8_t res_0x0064[0x0070-0x0064]; /* 0x0064 */
	 uint32_t buf_mode;			/* 0x0070 */
	 uint8_t res_0x0074[0x00E0-0x0074]; /* 0x0074 */
	 uint32_t apb;				/* 0x00E0 */
	 uint8_t res_0x00e4[0x00F0-0x00E4]; /* 0x00E4 */
	 uint32_t vi_int;			/* 0x00F0 */
	 uint8_t res_0x00f4[0x00F8-0x00F4];	 /* 0x00F4 */ 
	 uint32_t vi_int_en;		/* 0x00F8 */
	 uint8_t res_0x00fc[0x0100-0x00FC];	 /* 0x00FC */
	 uint32_t pt_mode;			/* 0x0100 */
	 uint8_t res_0x0104[0x0110-0x0104]; /* 0x0104 */
	 uint32_t pt_offset0;		/* 0x0110 */
	 uint32_t pt_offset1;		/* 0x0114 */
	 uint32_t pt_offset2;		/* 0x0118 */
	 uint8_t res_0x011c[0x0120-0x011C]; /* 0x011C */
	 uint32_t bt656;			/* 0x0120 */
	 uint8_t res_0x0124[0x0130-0x0124]; /* 0x0124 */
	 uint32_t timing_cfg;		/* 0x0130 */
	 uint32_t gen_timing_cfg;	/* 0x0134 */
	 uint8_t res_0x0138[0x0140-0x0138]; /* 0x0138 */
	 uint32_t data_cfg;			/* 0x0140 */
	 uint8_t res_0x0144[0x0150-0x0144]; /* 0x0144 */
	 uint32_t yuv444_cfg;		/* 0x0150 */
	 uint8_t res_0x0154[0x0180-0x0154]; /* 0x0154 */
	 uint32_t hfb;				/* 0x0180 */
	 uint32_t hact;				/* 0x0184 */
	 uint32_t hbb;				/* 0x0188 */
	 uint32_t vfb;				/* 0x018C */
	 uint32_t vact;				/* 0x0190 */
	 uint32_t vbb;				/* 0x0194 */
	 uint32_t vbfb;				/* 0x0198 */
	 uint32_t vbact;			/* 0x019C */
	 uint32_t vbbb;				/* 0x01A0 */
	 uint32_t id_cfg;			/* 0x01A4 */
	 uint8_t res_0x01A8[0x01E0-0x01A8]; /* 0x01A8 */
	 uint32_t pt_status;		/* 0x01E0 */
	 uint32_t bt656_statys;		/* 0x01E4 */
	 uint8_t res_0x01e8[0x01EC-0x01E8]; /* 0x01E8 */
	 uint32_t pt_size;			/* 0x01EC */
	 uint32_t pt_int;			/* 0x01F0 */
	 uint8_t res_0x01f4[0x01F8-0x01F4];	/* 0x01F4 */
	 uint32_t pt_int_en;		/* 0x01F8 */
	 uint8_t res_0x0fc[0x0200-0x01FC]; /* 0x01FC */
	 uint32_t des_ctl;			/* 0x0200 */
	 uint32_t des_newer;		/* 0x0204 */
	 uint8_t res_0x0208[0x0210-0x0208]; /* 0x0208 */
	 uint32_t des_id;			/* 0x0210 */
	 uint8_t res_0x0214[0x0220-0x0214]; /* 0x0214 */
	 uint32_t des_crop_cfg;		/* 0x0220 */
	 uint8_t res_0x0224[0x0230-0x0224]; /* 0x0224~0x022C */
	 uint32_t des_crop_start;	/* 0x0230 */
	 uint32_t des_crop_size;	/* 0x0234 */
	 uint8_t res_0x0238[0x0270-0x0238]; /* 0x0238 */
	 uint32_t des_cfg;			/* 0x0270 */
	 uint8_t res_0x0274[0x0284-0x0274]; /* 0x0274 */
	 uint32_t des_size;			/* 0x0284 */
	 uint8_t res_0x0288[0x0290-0x0288]; /* 0x0288 */
	 uint32_t des_faddr;		/* 0x0290 */
	 uint32_t des_stride;		/* 0x0294 */
	 uint8_t res_0x298[0x029C-0x0298]; /* 0x0298 */
	 uint32_t des_buf;			/* 0x029C */
	 uint8_t res_0x02a0[0x02F0-0x02A0]; /* 0x02A0 */
	 uint32_t des_int;			/* 0x02F0 */
	 uint8_t res_0x02f4[0x02F8-0x02F4];	 /* 0x02F4 */
	 uint32_t des_int_en;		/* 0x02F8 */
	 uint8_t res_0x02fc[0x0500-0x02FC];	 /* 0x02FC */
	 uint32_t lb_cfg;			/* 0x0500 */
	 uint32_t lb_newer;			/* 0x0504 */
	 uint8_t res_0x0508[0x0510-0x0508]; /* 0x0508 */
	 uint32_t lb_id;			/* 0x0510 */
	 uint8_t res_0x0514[0x0520-0x0514]; /* 0x0514 */
	 uint32_t lb_crop_cfg;		/* 0x0520 */
	 uint8_t res_0x0524[0x0530-0x0524]; /* 0x0524 */
	 uint32_t lb_crop_start;	/* 0x0530 */
	 uint32_t lb_crop_size;		/* 0x0534 */
	 uint8_t res_0x0538[0x0550-0x0538]; /* 0x0538 */
	 uint32_t lb_width;			/* 0x0550 */
	 uint32_t lb_height;		/* 0x0554 */
	 uint8_t res_0x0558[0x05F0-0x0558]; /* 0x0558 */
	 uint32_t lb_int;			/* 0x05F0 */
	 uint8_t re_0x05f4[0x05F8-0x05F4]; /* 0x05F4 */
	 uint32_t lb_int_en;		/* 0x05F8 */
	 uint8_t res_0x05fc[0x0700-0x05FC]; /* 0x05FC */
	 uint32_t src_ctl;			/* 0x0700 */
	 uint32_t src_newer;		/* 0x0704 */
	 uint8_t res_0x0708[0x0770-0x0708]; /* 0x0708 */
	 uint32_t src_cfg;			/* 0x0770 */
	 uint8_t res_0x0774[0x0784-0x0774]; /* 0x0774 */
	 uint32_t src_size;			/* 0x0784 */
	 uint8_t res_0x0788[0x0790-0x0788]; /* 0x0788 */
	 uint32_t src_faddr;		/* 0x0790 */
	 uint32_t src_stride;		/* 0x0794 */
	 uint8_t res_0x0798[0x079C-0x798];	/* 0x0798 */
	 uint32_t src_buf;			/* 0x079C */
	 uint32_t res40[20];		/* 0x07A0~0x07EC */
	 uint32_t src_int;			/* 0x07F0 */
	 uint8_t res_0x07f4[0x07F8-0x7F4]; /* 0x07F4 */
	 uint32_t src_int_en;		/* 0x07F8 */
	 uint8_t res_0x07fc[0x1004-0x07FC]; /* 0x07FC */
	 uint32_t ch_newer;			/* 0x1004 */
	 uint8_t res_0x1008[0x1010-0x1008]; /* 0x1008 */
	 uint32_t ch_adapter;		/* 0x1010 */
	 uint8_t res_0x1014[0x1080-0x1014]; /* 0x1014 */
	 uint32_t ch_y_cfg;			/* 0x1080 */
	 uint32_t ch_y_size;		/* 0x1084 */
	 uint8_t res_0x1088[0x1090-0x1088]; /* 0x1088 */
	 uint32_t ch_y_faddr;		/* 0x1090 */
	 uint32_t ch_y_haddr;		/* 0x1094 */
	 uint32_t ch_y_stride;		/* 0x1098 */
	 uint8_t res_0x109c[0x10A0-0x109C]; /* 0x109C */
	 uint32_t ch_c_cfg;			/* 0x10A0 */
	 uint32_t ch_c_size;		/* 0x10A4 */
	 uint8_t res_0x10a8[0x10B0-0x10A8]; /* 0x10A8 */
	 uint32_t ch_c_faddr;		/* 0x10B0 */
	 uint32_t ch_c_haddr;		/* 0x10B4 */
	 uint32_t ch_c_stride;		/* 0x10B8 */
	 uint8_t res_0x10bc[0x10F0-0x10BC]; /* 0x10BC */
	 uint32_t ch_int;			/* 0x10F0 */
	 uint8_t res_0x10f8[0x10F8-0x10F4]; /* 0x10F4 */
	 uint32_t ch_int_en;		/* 0x10F8 */
#endif
}hi_vi_regs_s;

/* GPIO@0x201D0000 */
typedef struct{
	 uint32_t res0;			/* 0x000 */
	 uint32_t data0;		/* 0x004 */
	 uint32_t data1;		/* 0x008 */
	 uint32_t res1;			/* 0x00C */
	 uint32_t data2;		/* 0x010 */
	 uint8_t res_0x014[0x020-0x014]; /* 0x014 */
	 uint32_t data3;		/* 0x020 */
	 uint8_t res_0x024[0x040-0x024]; /* 0x24 */
	 uint32_t data4;		/* 0x040 */
	 uint8_t res_0x044[0x080-0x044];	/* 0x044 */
	 uint32_t data5;		/* 0x080 */
	 uint8_t res_0x084[0x100-0x084];	/* 0x084 */
	 uint32_t data6;		/* 0x100 */
	 uint8_t res_0x104[0x200-0x104];	/* 0x104 */
	 uint32_t data7;		/* 0x200 */
	 uint8_t res_0x204[0x400-0x204];	/* 0x204 */
	 uint32_t dir;				/* 0x400 */
	 uint32_t is;				/* 0x404 */
	 uint32_t ibe;			/* 0x408 */
	 uint32_t iev;				/* 0x40C */
	 uint32_t ie;				/* 0x410 */
	 uint32_t ris;				/* 0x414 */
	 uint32_t mis;			/* 0x418 */
	 uint32_t ic;				/* 0x41C */
}hi_gpio_regs_s;

/*************************End Reg Structure****************************************/

#endif /* __HI_CHIP_REGS_H__ */
