#ifndef	__HI_CHIP_REGS_H__
#define	__HI_CHIP_REGS_H__

#include <rtems.h>

#define HI_REG_WR(a, v)		(*(volatile unsigned int *)(a) = (v))
#define HI_REG_RD(a)		(*(volatile unsigned int *)(a))

/*************************System****************************************/

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

#define REG_BASE_SCTL			0x20050000
#define REG_SC_CTRL				0x0
#define CRG_REG_BASE			0x20030000
/* BPLL CONFIG */
#define REG_CRG4_OFFSET			(CRG_REG_BASE + 0x10)
#define REG_CRG5_OFFSET			(CRG_REG_BASE + 0x14)
#define CRG_VICAP_CLK				0x002C
#define CRG_SENSOR_CLK			0x0030

/*************************Base Address****************************************/
/* PinMux */
#define PINMUX_REG_BASE			0x200F0000
/* Interrupt */
#define IRQ_REG_BASE					0x10140000
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
/*************************End Base Address************************************/

/*************************Reg Structure****************************************/
/* PinMux */
typedef struct{
	volatile uint32_t res0[2];		/* 0x000~0x004 */
	volatile uint32_t sns_clk;		/* 0x008 */
	volatile uint32_t res1[3];		/* 0x00C~0x014 */
	volatile uint32_t i2c_sda;		/* 0x018 */
	volatile uint32_t i2c_scl;		/* 0x01C */
	volatile uint32_t res2[30];	/* 0x020~0x0C4 */
	volatile uint32_t gpio9_0;	/* 0x0C8 */
	volatile uint32_t gpio9_1;	/* 0x0CC */
	volatile uint32_t gpio9_2;	/* 0x0D0 */
	volatile uint32_t gpio9_3;	/* 0x0D4 */
	volatile uint32_t gpio9_4;	/* 0x0D8 */
	volatile uint32_t gpio9_5;	/* 0x0DC */
	volatile uint32_t gpio9_6;	/* 0x0E0 */
	volatile uint32_t gpio9_7;	/* 0x0E4 */
	volatile uint32_t res3[8];		/* 0x0E8~0x104 */
	volatile uint32_t uart2_rxd;	/* 0x108 */
	volatile uint32_t uart2_txd; /* 0x10C */
	volatile uint32_t res4[12];	/* 0x110~0x13C */
	volatile uint32_t vi_clk;		/* 0x140 */
	volatile uint32_t vi_vs;			/* 0x144 */
	volatile uint32_t vi_hs;			/* 0x148 */
	volatile uint32_t vi_dat11;	/* 0x14C */
	volatile uint32_t vi_dat10;	/* 0x150 */
	volatile uint32_t vi_dat9;		/* 0x154 */
	volatile uint32_t vi_dat8;		/* 0x158 */
	volatile uint32_t vi_dat7;		/* 0x15C */
	volatile uint32_t vi_dat6;		/* 0x160 */
	volatile uint32_t vi_dat5;		/* 0x164 */
	volatile uint32_t vi_dat4;		/* 0x168 */
	volatile uint32_t vi_dat3;		/* 0x16C */
	volatile uint32_t vi_dat2;		/* 0x170 */
	volatile uint32_t vi_dat1;		/* 0x174 */
	volatile uint32_t vi_dat0;		/* 0x178 */
}hi_pinmux_regs_s;

/* Interrupt */
typedef struct{
	volatile uint32_t irq_status;		/* 0x00 */
	volatile uint32_t fiq_status;		/* 0x04 */
	volatile uint32_t raw_int;			/* 0x08 */
	volatile uint32_t src;				/* 0x0C */
	volatile uint32_t enable;			/* 0x10 */
	volatile uint32_t en_clr;			/* 0x14 */
	volatile uint32_t softint;			/* 0x18 */
	volatile uint32_t softclr;			/* 0x1C */
	volatile uint32_t protect;			/* 0x20 */
}hi_irq_regs_s;

/* Timer */
typedef struct{							/*  T0/T2 - T1/T3 */
	 volatile uint32_t load;			/* 0x00 - 0x20 */
	 volatile uint32_t value;		/* 0x04 - 0x24 */
	 volatile uint32_t ctrl;			/* 0x08 - 0x28 */
	 volatile uint32_t icr;			/* 0x0C - 0x2C */
	 volatile uint32_t ris;			/* 0x10 - 0x30 */
	 volatile uint32_t mis;			/* 0x14 - 0x34 */
	 volatile uint32_t bgload;		/* 0x18 - 0x38 */
}hi_timer_regs_s;

/* UART */
typedef struct {
    volatile uint32_t dr;				/* 0x00 */
	volatile uint32_t rsr;			/* 0x04 */
	volatile uint32_t res0[4];		/* 0x08~0x14 */
	volatile uint32_t fr;				/* 0x18 */
	volatile uint32_t res1[2];		/* 0x1C~0x20 */
	volatile uint32_t ibrd;			/* 0x24 */
	volatile uint32_t fbrd;			/* 0x28 */
	volatile uint32_t lcr_h;			/* 0x2C */
	volatile uint32_t cr;				/* 0x30 */
	volatile uint32_t ifls;			/* 0x34 */
	volatile uint32_t imsc;			/* 0x38 */
	volatile uint32_t ris;				/* 0x3C */
	volatile uint32_t mis;			/* 0x40 */
	volatile uint32_t icr;				/* 0x44 */
	volatile uint32_t dmacr;		/* 0x48 */
} hi_uart_regs_s;
 
/* I2C */
typedef struct{
	volatile uint32_t ctrl;			/* 0x00 */
	volatile uint32_t com;			/* 0x04 */
	volatile uint32_t icr;				/* 0x08 */
	volatile uint32_t sr;				/* 0x0C */
	volatile uint32_t scl_h;			/* 0x10 */
	volatile uint32_t scl_l;			/* 0x14 */
	volatile uint32_t txr;			/* 0x18 */
	volatile uint32_t rxr;			/* 0x1C */
}hi_i2c_regs_s;

/* VI */
typedef struct{
	volatile uint32_t lowpower;		/* 0x0000 */
	volatile uint32_t res0[3];			/* 0x0004~0x000C */
	volatile uint32_t axi;				/* 0x0010 */
	volatile uint32_t res1[3];			/* 0x0014~0x001C */
	volatile uint32_t pt_sel;			/* 0x0020 */
	volatile uint32_t res2[3];			/* 0x0024~0x002C */
	volatile uint32_t ch_sel;			/* 0x0030 */
	volatile uint32_t res3[43];		/* 0x0034~0x00DC */
	volatile uint32_t apb;				/* 0x00E0 */
	volatile uint32_t res4[3];			/* 0x00E4~0x00EC */
	volatile uint32_t g_int;				/* 0x00F0 */
	volatile uint32_t res5;				/* 0x00F4 */
	volatile uint32_t g_int_en;		/* 0x00F8 */
	volatile uint32_t res6;				/* 0x00FC */
	volatile uint32_t pt_mode;		/* 0x0100 */
	volatile uint32_t res7[3];			/* 0x0104~0x010C */
	volatile uint32_t pt_offset0;	/* 0x0110 */
	volatile uint32_t pt_offset1;	/* 0x0114 */
	volatile uint32_t pt_offset2;	/* 0x0118 */
	volatile uint32_t res8[5];			/* 0x011C~0x012C*/
	volatile uint32_t timing_cfg;	/* 0x0130 */
	volatile uint32_t res9[3];			/* 0x0134~0x013C */
	volatile uint32_t data_cfg;		/* 0x0140 */
	volatile uint32_t res10[15];		/* 0x0144~0x017C */
	volatile uint32_t hfb;				/* 0x0180 */
	volatile uint32_t hact;				/* 0x0184 */
	volatile uint32_t hbb;				/* 0x0188 */
	volatile uint32_t vfb;				/* 0x018C */
	volatile uint32_t vact;				/* 0x0190 */
	volatile uint32_t vbb;				/* 0x0194 */
	volatile uint32_t vbfb;				/* 0x0198 */
	volatile uint32_t vbact;			/* 0x019C */
	volatile uint32_t vbbb;				/* 0x01A0 */
	volatile uint32_t res11[15];		/* 0x01A4~0x01DC */
	volatile uint32_t pt_status;		/* 0x01E0 */
	volatile uint32_t res12;			/* 0x01E4 */
	volatile uint32_t pt_size;			/* 0x01E8 */
	volatile uint32_t res13;			/* 0x01EC */
	volatile uint32_t pt_int;			/* 0x01F0 */
	volatile uint32_t res14;			/* 0x01F4 */
	volatile uint32_t pt_int_en;		/* 0x01F8 */
	volatile uint32_t res15[897];	/* 0x01FC~0x0FFC */
	volatile uint32_t ch_ctrl;			/* 0x1000 */
	volatile uint32_t new;				/* 0x1004 */
	volatile uint32_t res16[2];		/* 0x1008~0x100C */
	volatile uint32_t adapter;		/* 0x1010 */
	volatile uint32_t res17[27];		/* 0x1014~0x107C */
	volatile uint32_t pack_y;			/* 0x1080 */
	volatile uint32_t pack_y_width;	/* 0x1084 */
	volatile uint32_t res18[2];		/* 0x1088~0x108C */
	volatile uint32_t pack_c;			/* 0x1090 */
	volatile uint32_t pack_c_width;	/* 0x1094 */
	volatile uint32_t res19[6];		/* 0x1098~0x10AC */
	volatile uint32_t des_y;			/* 0x10B0 */
	volatile uint32_t des_y_addr;	/* 0x10B4 */
	volatile uint32_t des_y_size;	/* 0x10B8 */
	volatile uint32_t des_y_stride;	/* 0x10BC */
	volatile uint32_t des_c;			/* 0x10C0 */
	volatile uint32_t des_c_addr;	/* 0x10C4 */
	volatile uint32_t des_c_size;	/* 0x10C8 */
	volatile uint32_t des_c_stride;	/* 0x10CC */
	volatile uint32_t res20[8];		/* 0x10D0~0x10EC */
	volatile uint32_t ch_int;			/* 0x10F0 */
	volatile uint32_t res21;			/* 0x10F4 */
	volatile uint32_t ch_int_en;		/* 0x10F8 */
	volatile uint32_t res22;			/* 0x10FC */
	volatile uint32_t crop_cfg;		/* 0x1100 */
	volatile uint32_t crop_win;		/* 0x1104 */
	volatile uint32_t res23[2];		/* 0x1108~0x110C */
	volatile uint32_t crop_start;	/* 0x1110 */
	volatile uint32_t crop_size;		/* 0x1114 */
}hi_vi_regs_s;

typedef struct{
	volatile uint32_t res0;			/* 0x000 */
	volatile uint32_t data0;		/* 0x004 */
	volatile uint32_t data1;		/* 0x008 */
	volatile uint32_t res1;			/* 0x00C */
	volatile uint32_t data2;		/* 0x010 */
	volatile uint32_t res2[3];		/* 0x014~0x01C */
	volatile uint32_t data3;		/* 0x020 */
	volatile uint32_t res3[7];		/* 0x24~0x3C */
	volatile uint32_t data4;		/* 0x040 */
	volatile uint32_t res4[15];	/* 0x044~0x07C */
	volatile uint32_t data5;		/* 0x080 */
	volatile uint32_t res5[31];	/* 0x084~0x0FC */
	volatile uint32_t data6;		/* 0x100 */
	volatile uint32_t res6[63];	/* 0x104~0x1FC */
	volatile uint32_t data7;		/* 0x200 */
	volatile uint32_t res7[127];	/* 0x204~0x3FC */
	volatile uint32_t dir;				/* 0x400 */
	volatile uint32_t is;				/* 0x404 */
	volatile uint32_t ibe;			/* 0x408 */
	volatile uint32_t iev;				/* 0x40C */
	volatile uint32_t ie;				/* 0x410 */
	volatile uint32_t ris;				/* 0x414 */
	volatile uint32_t mis;			/* 0x418 */
	volatile uint32_t ic;				/* 0x41C */
}hi_gpio_regs_s;

/*************************End Reg Structure****************************************/

#endif /* __HI_CHIP_REGS_H__ */
