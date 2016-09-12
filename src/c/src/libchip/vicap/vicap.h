#ifndef _VICAP_H
#define _VICAP_H

#include <rtems.h>
#include <rtems/libi2c.h>
#include <vicap_drv.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CH_REG_NEW	(1<<4)
#define CH_FIELD_THROW  (1<<3)
#define CH_BUF_OVF		(1<<2)
#define CH_CC_INT		(1<<1)
#define CH_FSTART		(1<<0)

#define BUF_NUM		3

typedef struct
{
	unsigned char w_pos;
	unsigned char r_pos;
	unsigned char num;
	unsigned int addr[BUF_NUM];
}ring_buffer;

typedef struct
{
	unsigned char cur_num;
	unsigned char valid_num;
	unsigned char w_pos;
}buf_info;

typedef struct
{
	unsigned int width;
	unsigned int height;
	unsigned int yaddr[BUF_NUM];
	unsigned int caddr;
}vicap_para_s;

typedef enum
{
	VICAP_START_CAPTURE = 0,
	VICAP_STOP_CAPTURE,
	VICAP_GET_DATA_STA,
	VICAP_INT_DEBUG,
	VICAP_BUTT
}VICAP_CTRL_CMD;

int video_capture_open(rtems_id taskid);

void video_capture_init(sensor_type_e sns_type, vicap_para_s *vicap_para);

int video_capture_ioctl(VICAP_CTRL_CMD cmd, buf_info *b_info);

void video_capture_close(void);

#ifdef __cplusplus
}
#endif

#endif /* _VICAP_H */
