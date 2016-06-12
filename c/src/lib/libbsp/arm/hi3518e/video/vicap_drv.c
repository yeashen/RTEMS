/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : vicap_drv.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/04/19
  Last Modified : 
  Description   :  Hi3518e video capture interface driver
  History       : 
  1.Date        : 2016/04/19
   Author       : Juven
   Modification : Created file

******************************************************************************/

#include <hi3518e.h>
#include <vicap_drv.h>

static hi_vi_regs_s *vi_reg = NULL;
static hi_pinmux_regs_s *pinmux_reg = NULL;
static hi_irq_regs_s *irq_reg = NULL;

void vicap_set_y_faddr(unsigned int addr)
{
	vi_reg->des_y_addr = addr;
}

void vicap_set_y_width(unsigned int width)
{
	unsigned int val = 0;
	
	val = vi_reg->des_y_size;
	val = (val&0xFFFFF000)|(width&0xFFF);
	vi_reg->des_y_size = val;
}

void vicap_set_y_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->des_y_size;
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	vi_reg->des_y_size = val;
}

void vicap_set_y_stride(unsigned int stride)
{
	vi_reg->des_y_stride = stride;
}

void vicap_set_c_faddr(unsigned int addr)
{
	vi_reg->des_c_addr = addr;
}

void vicap_set_c_width(unsigned int width)
{
	unsigned int val = 0;

	val = vi_reg->des_c_size;
	val = (val&0xFFFFF000)|(width&0xFFF);
	vi_reg->des_c_size = val;
}

void vicap_set_c_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->des_c_size;
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	vi_reg->des_c_size = val;
}

void vicap_set_c_stride(unsigned int stride)
{
	vi_reg->des_c_stride = stride;
}

void vicap_set_hact(unsigned int hact)
{
	vi_reg->hact = hact;
}

void vicap_set_vact(unsigned int vact)
{
	vi_reg->vact = vact;
}

void vicap_set_crop_width(unsigned int width)
{
	unsigned int val = 0;
	
	val = vi_reg->crop_size;
	val = (val&0xFFFF0000)|(width&0xFFFF);
	vi_reg->crop_size = val;
}

void vicap_set_crop_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->crop_size;
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	vi_reg->crop_size = val;
}

void vicap_set_pack_y_width(unsigned int width)
{
	vi_reg->pack_y_width = width;
}

void vicap_set_pack_c_width(unsigned int width)
{
	vi_reg->pack_c_width = width;
}

void vicap_reg_newer()
{
	vi_reg->new = 0x1;
}

unsigned int vicap_get_ch_int_en()
{
	return vi_reg->ch_int_en;
}

unsigned int vicap_get_ch_int_status()
{
	return vi_reg->ch_int;
}

unsigned int vicap_get_pt_height()
{
	unsigned int val;

	val = vi_reg->pt_size;
	
	return (val&0xFFFF0000)>>16;
}

unsigned int vicap_get_pt_width()
{
	unsigned int val;

	val = vi_reg->pt_size;
	
	return (val&0xFFFF);
}


void vicap_clear_ch_int(unsigned int int_mask)
{
	vi_reg->ch_int = int_mask;
}

void vicap_pin_init(sensor_type_e sns_type)
{
	pinmux_reg =  (hi_pinmux_regs_s *)PINMUX_REG_BASE;
	irq_reg = (hi_irq_regs_s *)IRQ_REG_BASE;

	/* VICAP clk */
	HI_REG_WR(CRG_REG_BASE+CRG_VICAP_CLK,  0xa);//VICAP, unreset & clock enable

	pinmux_reg->vi_clk = 0x0;
	pinmux_reg->vi_vs = 0x0;
	pinmux_reg->vi_hs = 0x0;
	if(sns_type == SENSOR_OV9712){
		pinmux_reg->vi_dat11 = 0x0;
		pinmux_reg->vi_dat10 = 0x0;
		pinmux_reg->vi_dat9 = 0x0;
		pinmux_reg->vi_dat8 = 0x0;
		pinmux_reg->vi_dat7 = 0x0;
		pinmux_reg->vi_dat6 = 0x0;
		pinmux_reg->vi_dat5 = 0x0;
		pinmux_reg->vi_dat4 = 0x0;
		pinmux_reg->vi_dat3 = 0x0;
		pinmux_reg->vi_dat2 = 0x0;
		pinmux_reg->vi_dat1 = 0x0;
		pinmux_reg->vi_dat0 = 0x0;
	}
	else if(sns_type == SENSOR_POA030R){
		pinmux_reg->vi_dat8 = 0x0;
		pinmux_reg->vi_dat7 = 0x0;
		pinmux_reg->vi_dat6 = 0x0;
		pinmux_reg->vi_dat5 = 0x0;
		pinmux_reg->vi_dat4 = 0x0;
		pinmux_reg->vi_dat3 = 0x0;
		pinmux_reg->vi_dat2 = 0x0;
		pinmux_reg->vi_dat1 = 0x0;
		pinmux_reg->vi_dat0 = 0x0;

		/* special pin */
		pinmux_reg->spi0_sdi = 0x0;
		pinmux_reg->gpio0_7 = 0x1;
	}
}

void vicap_reg_init(sensor_type_e sns_type)
{
	vi_reg = (hi_vi_regs_s *)VI_REG_BASE;

	/* relation reg config start*/
	vi_reg->lowpower = 0x1;
	vi_reg->apb = 0x80000100;
	if(sns_type == SENSOR_OV9712){
		vi_reg->pt_offset0 = 0xFFC00000;
		vi_reg->pt_offset1 = 0xFFF00010;
		vi_reg->pt_offset2 = 0xFFF00020;
	}
	else if(sns_type == SENSOR_POA030R){
		vi_reg->pt_offset0 = 0x1FF00000;
		vi_reg->pt_offset1 = 0xFFF00010;
		vi_reg->pt_offset2 = 0xFFF00020;
	}
	vi_reg->timing_cfg = 0x82001;
	vi_reg->data_cfg = 0x4;
	vi_reg->hfb = 0x198;
	vi_reg->hbb = 0x0;
	vi_reg->vfb = 0x6;
	vi_reg->vbb = 0x6;
	vi_reg->vbfb = 0x0;
	vi_reg->vbact = 0x0;
	vi_reg->vbbb = 0x0;
	vi_reg->adapter = 0x3;
	vi_reg->pack_y = 0x10;
	vi_reg->crop_cfg = 0x1;
	/* relation reg config end */

	/* config AXI_CFG */
	vi_reg->axi = 0x3;

	/* port config */
	vi_reg->pt_sel = 0x0;

	/* channel config */
	vi_reg->ch_sel = 0x0;
	vi_reg->ch_int_en = FSTART_EN|CC_INT_EN;

	/* vicap global interrupt config */
	vi_reg->g_int_en = CH0_INT_EN;

	/* interface config */
	vi_reg->pt_mode = PT_EN;

	/* enable channel */
	vi_reg->ch_ctrl = CH_EN;
}

