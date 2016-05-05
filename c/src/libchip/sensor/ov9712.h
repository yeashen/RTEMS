#ifndef _OV9712_H
#define _OV9712_H

#include <rtems.h>
#include <rtems/libi2c.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SENSOR_I2C_ADDR			0x60

typedef struct
{
	uint32_t 	reg_addr; 
	uint32_t 	data; 
}SENSOR_DATA_S ;

typedef enum
{
	MODE_640x400 = 0,
	MODE_1280x720,
}SENSOR_RES_E;

int sensor_ov9712_init(SENSOR_RES_E mode);

int sensor_ov9712_read(SENSOR_DATA_S *sensor_data);

int sensor_ov9712_write(SENSOR_DATA_S *sensor_data);

#ifdef __cplusplus
}
#endif

#endif /* _OV9712_H */
