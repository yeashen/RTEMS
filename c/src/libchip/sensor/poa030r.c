/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : poa030r.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/05/14
  Last Modified : 
  Description   : sensor POA030R driver 
  History       : 
  1.Date        : 2016/05/14
   Author       : Juven
   Modification : Created file

******************************************************************************/
#include <rtems.h>
#include <rtems/libi2c.h>

#include <libchip/poa030r.h>
#include <i2c.h>

#include <rtems/libio.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))


static SENSOR_DATA_S poa030r_init_para_640_480[] = {
};

/* debug */
static SENSOR_DATA_S poa030r_init_para_640_480_colorbar[] = {
};


/* download poa030r settings to sensor through i2c */
static int poa030r_download_firmware(SENSOR_DATA_S *pModeSetting, int ArySize)
{
	int i, ret;
	uint32_t reg_addr, val;

	for(i = 0; i < ArySize; ++i, ++pModeSetting){
		reg_addr = pModeSetting->reg_addr;
		val = pModeSetting->data;
		ret = hi_i2c_write(SENSOR_I2C_ADDR, reg_addr, 1, val, 1);
		if(ret < 0){
			printk("i2c write addr=0x%x val=0x%x error!\n", reg_addr, val);
			return ret;
		}
		//usleep(100);
	}

	return ret;
}

int sensor_poa030r_init(SENSOR_RES_E mode)
{
	SENSOR_DATA_S *pModeSetting = NULL;
	int ArySize = 0, ret = 0;

	hi_i2c_init();

	switch(mode){
		case MODE_640x480:
			pModeSetting = poa030r_init_para_640_480;
			ArySize = ARRAY_SIZE(poa030r_init_para_640_480);
			break;
		case MODE_640x480_COLORBAR:
			pModeSetting = poa030r_init_para_640_480_colorbar;
			ArySize = ARRAY_SIZE(poa030r_init_para_640_480_colorbar);
			break;
		default:
			printk("invalid sensor mode, please check again!\n");
			return -1;
	}
	ret = poa030r_download_firmware(pModeSetting, ArySize);
	if (ret < 0){
		printk("poa030r_download_firmware failed! try again\n");
		return -1;
	}

	return 0;
}

int sensor_poa030r_read(SENSOR_DATA_S *sensor_data)
{
	return hi_i2c_read(SENSOR_I2C_ADDR, sensor_data->reg_addr, 1, &sensor_data->data, 1);
}

int sensor_poa030r_write(SENSOR_DATA_S *sensor_data)
{
	return hi_i2c_write(SENSOR_I2C_ADDR, sensor_data->reg_addr, 1, sensor_data->data, 1);
} 

