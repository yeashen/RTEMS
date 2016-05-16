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

void vicap_set_y_faddr(unsigned int addr)
{
	VICAP_WR_REG(VICAP_CH_DES_Y_FADDR, addr);
}

void vicap_set_y_width(unsigned int width)
{
	unsigned int val = 0;

	val = VICAP_RD_REG(VICAP_CH_DES_Y_SIZE);
	val = (val&0xFFFFF000)|(width&0xFFF);
	VICAP_WR_REG(VICAP_CH_DES_Y_SIZE, val);
}

void vicap_set_y_height(unsigned int height)
{
	unsigned int val = 0;

	val = VICAP_RD_REG(VICAP_CH_DES_Y_SIZE);
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	VICAP_WR_REG(VICAP_CH_DES_Y_SIZE, val);
}

void vicap_set_y_stride(unsigned int stride)
{
	VICAP_WR_REG(VICAP_CH_DES_Y_STRIDE, stride);
}

void vicap_set_c_faddr(unsigned int addr)
{
	VICAP_WR_REG(VICAP_CH_DES_C_FADDR, addr);
}

void vicap_set_c_width(unsigned int width)
{
	unsigned int val = 0;

	val = VICAP_RD_REG(VICAP_CH_DES_C_SIZE);
	val = (val&0xFFFFF000)|(width&0xFFF);
	VICAP_WR_REG(VICAP_CH_DES_C_SIZE, val);
}

void vicap_set_c_height(unsigned int height)
{
	unsigned int val = 0;

	val = VICAP_RD_REG(VICAP_CH_DES_C_SIZE);
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	VICAP_WR_REG(VICAP_CH_DES_C_SIZE, val);
}

void vicap_set_c_stride(unsigned int stride)
{
	VICAP_WR_REG(VICAP_CH_DES_C_STRIDE, stride);
}

void vicap_set_hact(unsigned int hact)
{
	VICAP_WR_REG(VICAP_PT_HACT, hact);
}

void vicap_set_vact(unsigned int vact)
{
	VICAP_WR_REG(VICAP_PT_VACT, vact);
}

void vicap_set_crop_width(unsigned int width)
{
	unsigned int val = 0;
	
	val = VICAP_RD_REG(VICAP_CROP0_SIZE);
	val = (val&0xFFFF0000)|(width&0xFFFF);
	VICAP_WR_REG(VICAP_CROP0_SIZE, val);
}

void vicap_set_crop_height(unsigned int height)
{
	unsigned int val = 0;

	val = VICAP_RD_REG(VICAP_CROP0_SIZE);
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	VICAP_WR_REG(VICAP_CROP0_SIZE, val);
}

void vicap_set_pack_y_width(unsigned int width)
{
	VICAP_WR_REG(VICAP_CH_PACK_Y_WIDTH, width);
}

void vicap_set_pack_c_width(unsigned int width)
{
	VICAP_WR_REG(VICAP_CH_PACK_C_WIDTH, width);
}

void vicap_reg_newer()
{
	VICAP_WR_REG(VICAP_CH_REG_NEW, 0x1);	
}

unsigned int vicap_get_ch_int_en()
{
	return VICAP_RD_REG(VICAP_CH_INT_MASK);
}

unsigned int vicap_get_ch_int_status()
{
	return VICAP_RD_REG(VICAP_CH_INT_STA);
}

unsigned int vicap_get_pt_height()
{
	unsigned int val;

	val = VICAP_RD_REG(VICAP_PT_SIZE);
	
	return (val&0xFFFF0000)>>16;
}

unsigned int vicap_get_pt_width()
{
	unsigned int val;

	val = VICAP_RD_REG(VICAP_PT_SIZE);
	
	return (val&0xFFFF);
}


void vicap_clear_ch_int(unsigned int int_mask)
{
	VICAP_WR_REG(VICAP_CH_INT_STA, int_mask);
}

void vicap_clear_src_int()
{
	INT_WR_REG(REG_INTC_SOFTINTCLEAR, CFG_VICAP_IRQEN);
}

void vicap_pin_init()
{
	/* VICAP clk */
	CRG_WR_REG(CRG_VICAP_CLK, 0xa); //VICAP, unreset & clock enable

	MUXPIN_WR_REG(VIU_CLK_PIN, 0x0);
	MUXPIN_WR_REG(VIU_VS_PIN, 0x0);
	MUXPIN_WR_REG(VIU_HS_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT11_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT10_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT9_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT8_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT7_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT6_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT5_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT4_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT3_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT2_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT1_PIN, 0x0);
	MUXPIN_WR_REG(VIU_DAT0_PIN, 0x0);
}

void vicap_reg_init(void)
{
	/* relation reg config start*/
	VICAP_WR_REG(VICAP_WK_MODE, 0x1); 
	VICAP_WR_REG(VICAP_APB_TIMEOUT, 0x80000100);
	VICAP_WR_REG(VICAP_PT_OFFSET0, 0xFFC00000);
	VICAP_WR_REG(VICAP_PT_OFFSET1, 0xFFF00010); 
	VICAP_WR_REG(VICAP_PT_OFFSET2, 0xFFF00020); 
	VICAP_WR_REG(VICAP_TIMMING_CFG, 0x82001); 
	VICAP_WR_REG(VICAP_DATA_CFG, 0x4); 
	VICAP_WR_REG(VICAP_PT_HFB, 0x198); 
	VICAP_WR_REG(VICAP_PT_HBB, 0x0); 
	VICAP_WR_REG(VICAP_PT_VFB, 0x6); 
	VICAP_WR_REG(VICAP_PT_VBB, 0x6); 
	VICAP_WR_REG(VICAP_PT_VBFB, 0x0);
	VICAP_WR_REG(VICAP_PT_VBACT, 0x0);
	VICAP_WR_REG(VICAP_PT_VBBB, 0x0);
	VICAP_WR_REG(VICAP_CH_ADAPTER_CFG, 0x3);
	VICAP_WR_REG(VICAP_CH_PACK_Y_CFG, 0x10);
	VICAP_WR_REG(VICAP_CH_CROP_CFG, 0x1); 
	/* relation reg config end */

	/* config AXI_CFG */
	VICAP_WR_REG(VICAP_AXI_CFG, 0x3);

	/* port config */
	VICAP_WR_REG(VICAP_PT_SEL, 0x0);	//external input

	/* channel config */
	VICAP_WR_REG(VICAP_CH_SEL, 0x0);	//isp bypass
	VICAP_WR_REG(VICAP_CH_INT_MASK, FSTART_EN|CC_INT_EN);

	/* vicap global interrupt config */
	VICAP_WR_REG(VICAP_INT_MASK, CH0_INT_EN);

	/* interface config */
	VICAP_WR_REG(VICAP_PT_INTF_MOD, PT_EN);

	/* enable channel */
	VICAP_WR_REG(VICAP_CH_CTRL, CH_EN);	//enable channel
}

