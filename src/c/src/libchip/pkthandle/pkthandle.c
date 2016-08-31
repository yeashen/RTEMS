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

int packet_handle(danmu_pkt_s *pkt)
{
	int ret = 0;
	unsigned int reg;
	unsigned int param[256];
	danmu_pkt_s *pkts = pkt;
	sys_version_s version;
	unsigned int model;

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
		switch(reg){
			case CMD_SYS_RESET:		/*system reset*/
				printk("system will reset\n");
				break;
			case CMD_SYS_START:		/*system start/stop*/
				param[0] = pkts->params[0];
				virreg_set_sys_start(param[0]);
				break;
			case CMD_INT_CONFIG:	/* interrupt configure*/
				param[0] = pkts->params[0];
				virreg_set_interrupt_config(param[0]);
				break;
		}
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
				pkts->type = PKT_RSP_GET;
				/* compute checksum */
				checksum_check(GEN_CHECKSUM, pkts);
				/* send packet */
				hi_uart0_fns.deviceWrite(DAMNU_UART_MINOR, pkts, sizeof(pkts));
				break;
		}
	}

	//printk("start: 0x%x, type: 0x%x, cmd: 0x%x, len: 0x%x, checksum: 0x%x\n", pkt->start,
	//	pkt->type, pkt->cmd, pkt->len, pkt->checksum);

	return ret;
}

