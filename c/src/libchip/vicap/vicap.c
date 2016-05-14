/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : vicap.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/04/19
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/04/19
   Author       : Juven
   Modification : Created file

******************************************************************************/

#include <rtems.h>
#include <libchip/vicap.h>
#include <vicap_drv.h>
#include <bsp/irq.h>

int fstart = 0;
int fcc = 0;

rtems_isr video_capture_isr(rtems_vector_number vector)
{
	volatile uint32_t int_sta;

	int_sta = vicap_get_ch_int_status();

	if(int_sta&CH_FSTART){
		/* modify buffer addr to next */
		//printk("detect one frame start\n");
		fstart++;
		vicap_clear_ch_int(CH_FSTART);
	}else if(int_sta&CH_CC_INT){
		printk("one frame capture finish\n");
		fcc++;
		if(fcc%500 == 0)
			printk("detect %d frame, cc=%d\n", fstart, fcc);
		/* capture one frame to mem addr */
		vicap_clear_ch_int(CH_CC_INT);
	}
	vicap_clear_src_int();
	//vicap_reg_newer();
}

int video_capture_open()
{
	int status = 0;

	status = rtems_interrupt_handler_install(
			BSP_INT_VICAP,
			"VICAP",
			RTEMS_INTERRUPT_UNIQUE,
			(rtems_interrupt_handler) video_capture_isr,
			NULL
			);
	if(status != RTEMS_SUCCESSFUL){
		printk("vicap install interrupt handler failed!\n");
		return -1;
	}

	return 0;
}
	
void video_capture_init(VICAP_PARA_S *vicap_para)
{
	vicap_pin_init();
	vicap_reg_init();

	vicap_set_y_faddr(vicap_para->stDesY.addr);
	vicap_set_y_width(vicap_para->stDesY.width*16/128-1);
	vicap_set_y_height(vicap_para->stDesY.height-1);
	vicap_set_y_stride(vicap_para->stDesY.width*2);
	vicap_set_pack_y_width(vicap_para->stDesY.width*16-1);
	vicap_set_c_faddr(vicap_para->stDesC.addr);
	vicap_set_c_width(vicap_para->stDesC.width*16/128-1);
	vicap_set_c_height(vicap_para->stDesC.height-1);
	vicap_set_c_stride(vicap_para->stDesC.width);
	vicap_set_pack_c_width(vicap_para->stDesC.width*16-1);

	vicap_set_crop_width(vicap_para->stDesY.width-1);
	vicap_set_crop_height(vicap_para->stDesY.height-1);
	vicap_set_hact(vicap_para->stDesY.width);
	vicap_set_vact(vicap_para->stDesY.height);
	
}

int video_capture_ioctl(VICAP_CTRL_CMD cmd)
{
	int status = 0;

	switch(cmd){
		case VICAP_START_CAPTURE:
			vicap_reg_newer();
			break;
		case VICAP_GET_DATA_STA:
			// TODO: event
			if(status != RTEMS_SUCCESSFUL){
				return -1;
			}else{
				status = 1;
				return status;
			}
			break;
		case VICAP_INT_DEBUG:
			status = vicap_get_ch_int_status();
			break;
		default:
			break;
	}

	return status;
}

void video_capture_close()
{
	rtems_interrupt_handler_remove(
				BSP_INT_VICAP,
				video_capture_isr,
				NULL);
}

