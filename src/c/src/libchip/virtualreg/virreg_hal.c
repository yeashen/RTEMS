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

void virreg_get_sys_reset(unsigned int *reset)
{
	*reset= g_virtualreg->sys_power.bits.reset;
}

void virreg_set_sys_reset(unsigned int reset)
{
	u_sys_power sys_power;

	sys_power = g_virtualreg->sys_power;
	sys_power.bits.reset = reset;
	g_virtualreg->sys_power = sys_power;
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

void virreg_get_global_int_ctrl(unsigned int *ctrl)
{
	*ctrl = g_virtualreg->global_int_ctrl.bits.global;
}

void virreg_set_global_int_ctrl(unsigned int ctrl)
{
	u_global_int_ctrl int_ctrl;

	int_ctrl = g_virtualreg->global_int_ctrl;
	int_ctrl.bits.global = ctrl;
	g_virtualreg->global_int_ctrl = int_ctrl;
}

void virreg_get_trans_int_ctrl(unsigned int *ctrl)
{
	*ctrl = g_virtualreg->global_int_ctrl.bits.trans;
}

void virreg_set_trans_int_ctrl(unsigned int ctrl)
{
	u_global_int_ctrl int_ctrl;

	int_ctrl = g_virtualreg->global_int_ctrl;
	int_ctrl.bits.trans = ctrl;
	g_virtualreg->global_int_ctrl = int_ctrl;
}

void virreg_get_wave_int_ctrl(unsigned int *ctrl)
{
	*ctrl = g_virtualreg->wave_int_ctrl.u32;
}

void virreg_set_wave_int_ctrl(unsigned int ctrl)
{
	u_gest_int_ctrl int_ctrl;

	int_ctrl = g_virtualreg->wave_int_ctrl;
	int_ctrl.u32 = ctrl;
	g_virtualreg->wave_int_ctrl = int_ctrl;
}

void virreg_get_stay_int_ctrl(unsigned int *ctrl)
{
	*ctrl = g_virtualreg->stay_int_ctrl.u32;
}

void virreg_set_stay_int_ctrl(unsigned int ctrl)
{
	u_gest_int_ctrl int_ctrl;

	int_ctrl = g_virtualreg->stay_int_ctrl;
	int_ctrl.u32 = ctrl;
	g_virtualreg->stay_int_ctrl = int_ctrl;
}

void virreg_get_global_int_status(unsigned int *status)
{
	*status = g_virtualreg->global_int_status.bits.global;
	g_virtualreg->global_int_status.bits.global = 0x0;
}

void virreg_set_global_int_status(unsigned int status)
{
	u_global_int_status int_status;

	int_status = g_virtualreg->global_int_status;
	int_status.bits.global = status;
	g_virtualreg->global_int_status = int_status;
}

void virreg_get_trans_int_status(unsigned int *status)
{
	*status = g_virtualreg->global_int_status.bits.trans;
	g_virtualreg->global_int_status.bits.trans = 0x0;
}

void virreg_set_trans_int_status(unsigned int status)
{
	u_global_int_status int_status;

	int_status = g_virtualreg->global_int_status;
	int_status.bits.trans = status;
	g_virtualreg->global_int_status = int_status;
}

void virreg_get_wave_int_status(unsigned int *status)
{
	*status = g_virtualreg->wave_int_status.u32;
	g_virtualreg->wave_int_status.u32 = 0x0;
}

void virreg_set_wave_int_status(unsigned int status)
{
	u_gest_int_status int_status;

	int_status = g_virtualreg->wave_int_status;
	int_status.u32 = status;
	g_virtualreg->wave_int_status = int_status;
}

void virreg_get_stay_int_status(unsigned int *status)
{
	*status = g_virtualreg->stay_int_status.u32;
	g_virtualreg->stay_int_status.u32 = 0x0;
}

void virreg_set_stay_int_status(unsigned int status)
{
	u_gest_int_status int_status;

	int_status = g_virtualreg->stay_int_status;
	int_status.u32 = status;
	g_virtualreg->stay_int_status = int_status;
}

void virreg_get_pos_lc_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_lc_xy.bits.x;
	*y = g_virtualreg->pos_lc_xy.bits.y;
}

void virreg_set_pos_lc_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_lc_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_lc_xy = pos;
}

void virreg_get_pos_lc_z(unsigned int *z)
{
	*z = g_virtualreg->pos_lc_z.bits.z;
}

void virreg_set_pos_lc_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_lc_z;
	pos.bits.z = z;
	g_virtualreg->pos_lc_z = pos;
}

void virreg_get_pos_l1_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_l1_xy.bits.x;
	*y = g_virtualreg->pos_l1_xy.bits.y;
}

void virreg_set_pos_l1_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_l1_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_l1_xy = pos;
}

void virreg_get_pos_l1_z(unsigned int *z)
{
	*z = g_virtualreg->pos_l1_z.bits.z;
}

void virreg_set_pos_l1_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_l1_z;
	pos.bits.z = z;
	g_virtualreg->pos_l1_z = pos;
}

void virreg_get_pos_l2_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_l2_xy.bits.x;
	*y = g_virtualreg->pos_l2_xy.bits.y;
}

void virreg_set_pos_l2_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_l2_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_l2_xy = pos;
}

void virreg_get_pos_l2_z(unsigned int *z)
{
	*z = g_virtualreg->pos_l2_z.bits.z;
}

void virreg_set_pos_l2_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_l2_z;
	pos.bits.z = z;
	g_virtualreg->pos_l2_z = pos;
}

void virreg_get_pos_l3_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_l3_xy.bits.x;
	*y = g_virtualreg->pos_l3_xy.bits.y;
}

void virreg_set_pos_l3_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_l3_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_l3_xy = pos;
}

void virreg_get_pos_l3_z(unsigned int *z)
{
	*z = g_virtualreg->pos_l3_z.bits.z;
}

void virreg_set_pos_l3_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_l3_z;
	pos.bits.z = z;
	g_virtualreg->pos_l3_z = pos;
}

void virreg_get_pos_l4_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_l4_xy.bits.x;
	*y = g_virtualreg->pos_l4_xy.bits.y;
}

void virreg_set_pos_l4_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_l4_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_l4_xy = pos;
}

void virreg_get_pos_l4_z(unsigned int *z)
{
	*z = g_virtualreg->pos_l4_z.bits.z;
}

void virreg_set_pos_l4_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_l4_z;
	pos.bits.z = z;
	g_virtualreg->pos_l4_z = pos;
}

void virreg_get_pos_l5_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_l5_xy.bits.x;
	*y = g_virtualreg->pos_l5_xy.bits.y;
}

void virreg_set_pos_l5_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_l5_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_l5_xy = pos;
}

void virreg_get_pos_l5_z(unsigned int *z)
{
	*z = g_virtualreg->pos_l5_z.bits.z;
}

void virreg_set_pos_l5_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_l5_z;
	pos.bits.z = z;
	g_virtualreg->pos_l5_z = pos;
}

void virreg_get_pos_rc_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_rc_xy.bits.x;
	*y = g_virtualreg->pos_rc_xy.bits.y;
}

void virreg_set_pos_rc_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_rc_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_rc_xy = pos;
}

void virreg_get_pos_rc_z(unsigned int *z)
{
	*z = g_virtualreg->pos_rc_z.bits.z;
}

void virreg_set_pos_rc_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_rc_z;
	pos.bits.z = z;
	g_virtualreg->pos_rc_z = pos;
}

void virreg_get_pos_r1_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_r1_xy.bits.x;
	*y = g_virtualreg->pos_r1_xy.bits.y;
}

void virreg_set_pos_r1_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_r1_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_r1_xy = pos;
}

void virreg_get_pos_r1_z(unsigned int *z)
{
	*z = g_virtualreg->pos_r1_z.bits.z;
}

void virreg_set_pos_r1_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_r1_z;
	pos.bits.z = z;
	g_virtualreg->pos_r1_z = pos;
}

void virreg_get_pos_r2_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_r2_xy.bits.x;
	*y = g_virtualreg->pos_r2_xy.bits.y;
}

void virreg_set_pos_r2_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_r2_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_r2_xy = pos;
}

void virreg_get_pos_r2_z(unsigned int *z)
{
	*z = g_virtualreg->pos_r2_z.bits.z;
}

void virreg_set_pos_r2_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_r2_z;
	pos.bits.z = z;
	g_virtualreg->pos_r2_z = pos;
}

void virreg_get_pos_r3_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_r3_xy.bits.x;
	*y = g_virtualreg->pos_r3_xy.bits.y;
}

void virreg_set_pos_r3_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_r3_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_r3_xy = pos;
}

void virreg_get_pos_r3_z(unsigned int *z)
{
	*z = g_virtualreg->pos_r3_z.bits.z;
}

void virreg_set_pos_r3_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_r3_z;
	pos.bits.z = z;
	g_virtualreg->pos_r3_z = pos;
}

void virreg_get_pos_r4_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_r4_xy.bits.x;
	*y = g_virtualreg->pos_r4_xy.bits.y;
}

void virreg_set_pos_r4_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_r4_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_r4_xy = pos;
}

void virreg_get_pos_r4_z(unsigned int *z)
{
	*z = g_virtualreg->pos_r4_z.bits.z;
}

void virreg_set_pos_r4_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_r4_z;
	pos.bits.z = z;
	g_virtualreg->pos_r4_z = pos;
}

void virreg_get_pos_r5_xy(unsigned int *x, unsigned int *y)
{
	*x = g_virtualreg->pos_r5_xy.bits.x;
	*y = g_virtualreg->pos_r5_xy.bits.y;
}

void virreg_set_pos_r5_xy(unsigned int x, unsigned int y)
{
	u_pos_xy pos;

	pos = g_virtualreg->pos_r5_xy;
	pos.bits.x = x;
	pos.bits.y = y;
	g_virtualreg->pos_r5_xy = pos;
}

void virreg_get_pos_r5_z(unsigned int *z)
{
	*z = g_virtualreg->pos_r5_z.bits.z;
}

void virreg_set_pos_r5_z(unsigned int z)
{
	u_pos_z pos;

	pos = g_virtualreg->pos_r5_z;
	pos.bits.z = z;
	g_virtualreg->pos_r5_z = pos;
}

void virreg_get_static_gest(unsigned int *gest)
{
	*gest = g_virtualreg->static_gest.u32;
}

void virreg_set_static_gest(unsigned int gest)
{
	u_static_gesture gesture;

	gesture = g_virtualreg->static_gest;
	gesture.u32 = gest;
	g_virtualreg->static_gest = gesture;
}

void virreg_get_wave_gest(unsigned int *gest)
{
	*gest = g_virtualreg->wave_gest.u32;
}

void virreg_set_wave_gest(unsigned int gest)
{
	u_wave_gesture gesture;

	gesture = g_virtualreg->wave_gest;
	gesture.u32 = gest;
	g_virtualreg->wave_gest = gesture;
}

void virreg_get_stay_gest(unsigned int *gest)
{
	*gest = g_virtualreg->stay_gest.u32;
}

void virreg_set_stay_gest(unsigned int gest)
{
	u_static_gesture gesture;

	gesture = g_virtualreg->stay_gest;
	gesture.u32 = gest;
	g_virtualreg->stay_gest = gesture;
}

void virreg_get_trans_from(unsigned int *gest)
{
	*gest = g_virtualreg->trans_gest_from.u32;
}

void virreg_set_trans_from(unsigned int gest)
{
	u_static_gesture gesture;

	gesture = g_virtualreg->trans_gest_from;
	gesture.u32 = gest;
	g_virtualreg->trans_gest_from = gesture;
}

void virreg_get_trans_to(unsigned int *gest)
{
	*gest = g_virtualreg->trans_gest_to.u32;
}

void virreg_set_trans_to(unsigned int gest)
{
	u_static_gesture gesture;

	gesture = g_virtualreg->trans_gest_to;
	gesture.u32 = gest;
	g_virtualreg->trans_gest_to = gesture;
}

