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
#include <uart.h>

unsigned int fstart = 0;
unsigned int fcc = 0;
static ring_buffer r_buf;
static rtems_id   v_mutex;
static  rtems_id user_task_id;

#define INC_WRAP(pos)    ((pos+1) == BUF_NUM ? 0 : (pos+1))

static inline void rtems_vicap_mutex_lock (rtems_id *mutex){
	rtems_status_code sc = rtems_semaphore_obtain(*mutex, RTEMS_NO_WAIT, 0);
	if (sc != RTEMS_SUCCESSFUL){
		printk("rtems-vicap: mutex: obtain failed\n");
	}
}

static inline void rtems_vicap_mutex_unlock (rtems_id *mutex){
	rtems_status_code sc = rtems_semaphore_release(*mutex);
	if (sc != RTEMS_SUCCESSFUL){
		printk("rtems-vicap: mutex: release failed\n");
	}
}

rtems_isr video_capture_isr(rtems_vector_number vector)
{
	volatile uint32_t int_sta;
	unsigned char index = 0;
	int status = 0;

	int_sta = vicap_get_ch_int_status();

	if(int_sta&CH_FSTART){
		/* modify buffer addr to next */
		//printk("detect one frame start\n");
		fstart++;
		
		rtems_vicap_mutex_lock(&v_mutex);
		index = r_buf.w_pos;
		vicap_set_y_faddr(r_buf.addr[INC_WRAP(index)]);
		rtems_vicap_mutex_unlock(&v_mutex);
		
		vicap_clear_ch_int(CH_FSTART);
	}else if(int_sta&CH_CC_INT){
		//printk("one frame capture finish\n");
		fcc++;
		
		rtems_vicap_mutex_lock(&v_mutex);
		index = r_buf.w_pos;
		r_buf.w_pos = INC_WRAP(index);
		if(r_buf.num < (BUF_NUM-1)){
			r_buf.num++;
		}else{
			r_buf.r_pos = INC_WRAP(r_buf.r_pos);
		}
		rtems_vicap_mutex_unlock(&v_mutex);

		status = rtems_event_send(user_task_id, RTEMS_EVENT_0);
		if(status != RTEMS_SUCCESSFUL){
			printk("vicap isr rtems_event_send failed!\n");
		}
		/* capture one frame to mem addr */
		vicap_clear_ch_int(CH_CC_INT);
	}
	vicap_reg_newer();
}

int video_capture_open(rtems_id taskid)
{
	int status = 0;

	user_task_id = taskid;

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

	hi_uart2_fns.deviceFirstOpen(0, 2, NULL);

	return 0;
}
	
void video_capture_init(sensor_type_e sns_type, vicap_para_s *vicap_para)
{
	int i = 0;
	int status = 0;

	vicap_pin_init(sns_type);
	vicap_reg_init(sns_type);

	r_buf.num = 0;
	r_buf.r_pos = 0;
	r_buf.w_pos = 0;
	for(i = 0; i < BUF_NUM; i++){
		r_buf.addr[i] = vicap_para->yaddr[i];
	}

	vicap_set_y_faddr(vicap_para->yaddr[0]);
	vicap_set_y_width(vicap_para->width);
	vicap_set_y_height(vicap_para->height-1);
	vicap_set_y_stride(vicap_para->width*2);
	vicap_set_pack_y_width(vicap_para->width*16-1);
	vicap_set_c_faddr(vicap_para->caddr);	
	vicap_set_c_width(vicap_para->width);
	vicap_set_c_height(vicap_para->height-1);
	vicap_set_c_stride(vicap_para->width);
	vicap_set_pack_c_width(vicap_para->width*16-1);

	vicap_set_crop_width(vicap_para->width-1);
	vicap_set_crop_height(vicap_para->height-1);
	vicap_set_hact(vicap_para->width);
	vicap_set_vact(vicap_para->height);

	status = rtems_semaphore_create(
		rtems_build_name('S', 'P', 'M', '0'),
		1,
		RTEMS_DEFAULT_ATTRIBUTES,
		RTEMS_NO_PRIORITY,
		&v_mutex
	);
}

int video_capture_ioctl(VICAP_CTRL_CMD cmd, buf_info *b_info)
{
	int status = 0;
	rtems_event_set   eventout;
	int i = 0;

	eventout = 10;
	switch(cmd){
		case VICAP_START_CAPTURE:
			vicap_reg_newer();
			break;
		case VICAP_GET_DATA_STA:
			// TODO: event
			status = rtems_event_receive(
	    		RTEMS_EVENT_0,
	    		RTEMS_DEFAULT_OPTIONS,
	   	 		RTEMS_WAIT,
	    		&eventout
  			);
			if(status != RTEMS_SUCCESSFUL){
				return -1;
			}else{
				status = 1;
				if(r_buf.num > 0){
					rtems_vicap_mutex_lock(&v_mutex);
					i = r_buf.r_pos;
					r_buf.r_pos = INC_WRAP(i);
					r_buf.num--;
					b_info->cur_num = i;
					b_info->valid_num = r_buf.num;
					b_info->w_pos = r_buf.w_pos;
					rtems_vicap_mutex_unlock(&v_mutex);
				}
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
	rtems_semaphore_delete(v_mutex);

	hi_uart2_fns.deviceLastClose(0, 2, NULL);
}

