/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : virreg_hal.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/08/20
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/08/20
   Author       : Juven
   Modification : Created file

******************************************************************************/

#include <rtems.h>
#include <libchip/virreg.h>
#include <libchip/virreg_hal.h>

#define REG_SIZE 			0xFF

uint8_t   Area[REG_SIZE] CPU_STRUCTURE_ALIGNMENT;
rtems_id   Partition_id;   /* array of partition ids */
rtems_name Partition_name; /* array of partition names */

volatile virtual_regs_s * volatile g_virtualreg = NULL;
//virtual_regs_s *g_virtualreg = NULL;

unsigned int virtualreg_init_reg(void)
{
	unsigned int ret = 0;
	void *virtualreg_addr;

	Partition_name =  rtems_build_name( 'P', 'R', 0, ' ' );
	(void)rtems_partition_create(
	    Partition_name,
	    Area,
	    REG_SIZE,
	    REG_SIZE,
	    RTEMS_DEFAULT_ATTRIBUTES,
	    &Partition_id
	);

	ret = rtems_partition_get_buffer( Partition_id, &virtualreg_addr);

	g_virtualreg = (virtual_regs_s *)virtualreg_addr;
	if(NULL == g_virtualreg){
		ret = -1;
		printk("init virtual reg failed\n");
	}

	g_virtualreg->sys_info_model.bits.model = 0x3518E200;
	g_virtualreg->sys_info_version.bits.hw_version = 1;
	g_virtualreg->sys_info_version.bits.fw_version = 1;
	g_virtualreg->sys_info_version.bits.sw_version = 1;

	return ret;
}

void virreg_get_info_model(unsigned int *model)
{
	*model = g_virtualreg->sys_info_model.u32;
}

void virreg_set_info_model(unsigned int model)
{
	g_virtualreg->sys_info_model.bits.model = model;
}

void virreg_get_info_version(sys_version_s * version)
{
	u_sys_info_version sys_info = g_virtualreg->sys_info_version;

	version->hw_version = sys_info.bits.hw_version;
	version->fw_version = sys_info.bits.fw_version;
	version->sw_version = sys_info.bits.sw_version;
}

void virreg_get_sys_start(unsigned int *start)
{
	*start= g_virtualreg->sys_power.bits.start;
}

void virreg_set_sys_start(unsigned int start)
{
	u_sys_power sys_power;

	sys_power = g_virtualreg->sys_power;
	sys_power.bits.start = start;
	g_virtualreg->sys_power = sys_power;
}

void virreg_get_interrupt_ctrl(unsigned int *ctrl)
{
	*ctrl = g_virtualreg->sys_interrupt_ctrl.u32;
}

void virreg_set_interrupt_ctrl(unsigned int ctrl)
{
	u_sys_interrupt_ctrl interrupt_ctrl;

	interrupt_ctrl.u32 = ctrl;
	g_virtualreg->sys_interrupt_ctrl = interrupt_ctrl;
}

void virreg_get_interrupt_config(unsigned int *config)
{
	*config = g_virtualreg->sys_interrupt_ctrl.u32;
}

void virreg_set_interrupt_config(unsigned int config)
{
	u_sys_interrupt_config interrupt_config;

	interrupt_config = g_virtualreg->sys_interrupt_config;
	interrupt_config.bits.config = config;
	g_virtualreg->sys_interrupt_config = interrupt_config;
}

void virreg_get_interrupt_status(unsigned int *status)
{
	*status = g_virtualreg->sys_interrupt_status.u32;
}