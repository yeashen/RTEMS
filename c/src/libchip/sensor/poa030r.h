#ifndef _POA030R_H
#define _POA030R_H

#include <rtems.h>
#include <rtems/libi2c.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SENSOR_I2C_ADDR			0xdc

typedef struct
{
	uint32_t 	reg_addr; 
	uint32_t 	data; 
}SENSOR_DATA_S ;

typedef enum
{
	MODE_640x480_30FPS = 0,
	MODE_640x480_60FPS,
	MODE_320x240_30FPS,
	MODE_320x240_60FPS,
}SENSOR_RES_E;

int sensor_poa030r_init(SENSOR_RES_E mode);

int sensor_poa030r_read(SENSOR_DATA_S *sensor_data);

int sensor_poa030r_write(SENSOR_DATA_S *sensor_data);

#ifdef __cplusplus
}
#endif

#endif /* _POA030R_H */
