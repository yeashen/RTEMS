/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : pkthandle.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/08/23
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/08/23
   Author       : Juven
   Modification : Created file

******************************************************************************/
#include <rtems.h>
#include <uart.h>
#include <libchip/pkthandle.h>
#include <libchip/virreg_hal.h>
/*
  *  type: 0 --check packet checksum;		1 --generate packet checksum
  */
static int checksum_check(pkt_type_checksum type, danmu_pkt_s *pkt)
{
	int i = 0;
	unsigned int sum = 0, param_s = 0;

	for(i = 0; i < (pkt->len); i++){
		param_s += pkt->params[i];
	}

	sum = pkt->start + pkt->type + pkt->cmd + pkt->len + param_s;
	sum &= 0xFFFF;

	if(type == GEN_CHECKSUM){
		pkt->checksum = sum;
		return 0;
	}else if(type == CHECK_CHECKSUM){
		if(sum == pkt->checksum){
			return 1;
		}else{
			printk("c_sum: 0x%x r_sum: 0x%x\n", sum, pkt->checksum);
			return -1;
		}
	}
}

void packet_send(danmu_pkt_s *pkt)
{
	unsigned int len = pkt->len;

	checksum_check(GEN_CHECKSUM, pkt);

	pkt->params[len] = pkt->checksum & 0xFF;
	pkt->params[len+1] = (pkt->checksum & 0xFF00)>>8;
	
	/* send packet */
	hi_uart_fns.deviceWrite(DAMNU_UART_MINOR, pkt, 6+len);
}

static unsigned char irled = 1;
int packet_handle(danmu_pkt_s *pkt)
{
	int ret = 0, i;
	unsigned int reg;
	unsigned int param[256];
	danmu_pkt_s *pkts = pkt;
	sys_version_s version;
	unsigned int model, val, x, y, z;

	if(NULL == pkt){
		ret = -1;
		printk("packet is NULL\n");
		return ret;
	}

	ret = checksum_check(CHECK_CHECKSUM, pkts);
	if(ret < 0){
		printk("receive packet checksum error!");
		return ret;
	}

	reg = pkts->cmd;
	if(pkts->type == PKT_REQ_SET){
		for(i = 0; i < pkt->len; i++){
			param[i] = pkts->params[i];
		}
		switch(reg){
			case CMD_SYS_RESET:		/*system reset*/
				virreg_set_sys_reset(param[0]);
				break;
			case CMD_SYS_START:		/*system start/stop*/
				virreg_set_sys_start(param[0]);
				break;
			case CMD_INT_ENABLE_GLOBAL:
				virreg_set_global_int_ctrl(param[0]);
				break;
			case CMD_INT_ENABLE_WAVE:
				virreg_set_wave_int_ctrl(param[0]);
				break;
			case CMD_INT_ENABLE_STAY:
				virreg_set_stay_int_ctrl(param[0]);
				break;
			case CMD_INT_ENABLE_TRANS:
				virreg_set_trans_int_ctrl(param[0]);
				break;
			default:
				printk("invalid cmd!\n");
				break;
		}
		pkts->type = PKT_RSP_SET;
	}else if(pkts->type == PKT_REQ_GET){
		switch(reg){
			case CMD_SYS_INFO:		/*system info*/
				/* get param */
				virreg_get_info_version(&version);
				virreg_get_info_model(&model);
				/* fill packet */
				pkts->params[0] = model & 0xFF;
				pkts->params[1] = (model >> 8) & 0xFF;
				pkts->params[2] = (model >> 16) & 0xFF;
				pkts->params[3] = (model >> 24) & 0xFF;
				pkts->params[4] = version.hw_version;
				pkts->params[5] = version.fw_version;
				pkts->params[6] = version.sw_version;
				pkts->len = 7;
				break;
			case CMD_INT_ENABLE_GLOBAL:
				virreg_get_global_int_ctrl(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_INT_ENABLE_WAVE:
				virreg_get_wave_int_ctrl(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_INT_ENABLE_STAY:
				virreg_get_stay_int_ctrl(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_INT_ENABLE_TRANS:
				virreg_get_trans_int_ctrl(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_INT_STATUS_GLOBAL:
				virreg_get_global_int_status(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_INT_STATUS_WAVE:
				virreg_get_wave_int_status(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_INT_STATUS_STAY:
				virreg_get_stay_int_status(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_INT_STATUS_TRANS:
				virreg_get_trans_int_status(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_GDATA_POS_LC:
				virreg_get_pos_lc_xy(&x, &y);
				virreg_get_pos_lc_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_L1:
				virreg_get_pos_l1_xy(&x, &y);
				virreg_get_pos_l1_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_L2:
				virreg_get_pos_l2_xy(&x, &y);
				virreg_get_pos_l2_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_L3:
				virreg_get_pos_l3_xy(&x, &y);
				virreg_get_pos_l3_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_L4:
				virreg_get_pos_l4_xy(&x, &y);
				virreg_get_pos_l4_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_L5:
				virreg_get_pos_l5_xy(&x, &y);
				virreg_get_pos_l5_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_RC:
				virreg_get_pos_rc_xy(&x, &y);
				virreg_get_pos_rc_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_R1:
				virreg_get_pos_r1_xy(&x, &y);
				virreg_get_pos_r1_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_R2:
				virreg_get_pos_r2_xy(&x, &y);
				virreg_get_pos_r2_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_R3:
				virreg_get_pos_r3_xy(&x, &y);
				virreg_get_pos_r3_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_R4:
				virreg_get_pos_r4_xy(&x, &y);
				virreg_get_pos_r4_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_GDATA_POS_R5:
				virreg_get_pos_r5_xy(&x, &y);
				virreg_get_pos_r5_z(&z);
				pkts->params[0] = x&0xFF;
				pkts->params[1] = (x&0xFF00)>>8;
				pkts->params[2] = y&0xFF;
				pkts->params[3] = (y&0xFF00)>>8;
				pkts->params[4] = z&0xFF;
				pkts->params[5] = (z&0xFF00)>>8;
				pkts->len = 6;
				break;
			case CMD_STATIC_GEST:
				virreg_get_static_gest(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_WAVE_GEST:
				virreg_get_wave_gest(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_STAY_GEST:
				virreg_get_stay_gest(&val);
				pkts->params[0] = val;
				pkts->len = 1;
				break;
			case CMD_TRANS_GEST:
				virreg_get_trans_from(&val);
				pkts->params[0] = val;
				virreg_get_trans_to(&val);
				pkts->params[1] = val;
				pkts->len = 2;
				break;
		}
		pkts->type = PKT_RSP_GET;
	}

	packet_send(pkt);

	//printk("start: 0x%x, type: 0x%x, cmd: 0x%x, len: 0x%x, checksum: 0x%x\n", pkt->start,
	//	pkt->type, pkt->cmd, pkt->len, pkt->checksum);

	return ret;
}

