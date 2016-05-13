#ifndef _VICAP_H
#define _VICAP_H

#include <rtems.h>
#include <rtems/libi2c.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CH_REG_NEW	(1<<4)
#define CH_FIELD_THROW  (1<<3)
#define CH_BUF_OVF		(1<<2)
#define CH_CC_INT		(1<<1)
#define CH_FSTART		(1<<0)

typedef struct
{
	unsigned int addr;
	unsigned int width;
	unsigned int height;
	unsigned int stride;
}DES_INFO;

typedef struct
{
	DES_INFO stDesY; 
	DES_INFO stDesC; 
}VICAP_PARA_S;

typedef enum
{
	VICAP_START_CAPTURE = 0,
	VICAP_GET_DATA_STA,
	VICAP_INT_DEBUG,
	VICAP_BUTT
}VICAP_CTRL_CMD;

int video_capture_open(void);

void video_capture_init(VICAP_PARA_S *vicap_para);

int video_capture_ioctl(VICAP_CTRL_CMD cmd);

void video_capture_close(void);

#ifdef __cplusplus
}
#endif

#endif /* _VICAP_H */
