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

static int i2c_minor = 0;

/* download poa030r settings to sensor through i2c */
static int poa030r_download_firmware(SENSOR_DATA_S *pModeSetting, int ArySize)
{
	int i, ret;
	uint32_t reg_addr, val;
	i2c_para_s i2c_para;

	i2c_para.dev_addr = SENSOR_I2C_ADDR;
	i2c_para.reg_byte = 1;
	i2c_para.data_byte = 1;

	for(i = 0; i < ArySize; ++i, ++pModeSetting){
		i2c_para.reg_addr = pModeSetting->reg_addr;
		i2c_para.data = pModeSetting->data;
		ret = hi_i2c_write(i2c_minor, &i2c_para);
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

	hi_i2c_init(0);

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
	i2c_para_s i2c_para;

	i2c_para.dev_addr = SENSOR_I2C_ADDR;
	i2c_para.reg_addr = sensor_data->reg_addr;
	i2c_para.reg_byte = 1;
	i2c_para.data_byte = 1;
	hi_i2c_read(i2c_minor, &i2c_para);

	return i2c_para.data;
}


int sensor_poa030r_write(SENSOR_DATA_S *sensor_data)
{
	i2c_para_s i2c_para;

	i2c_para.dev_addr = SENSOR_I2C_ADDR;
	i2c_para.reg_addr = sensor_data->reg_addr;
	i2c_para.reg_byte = 1;
	i2c_para.data = sensor_data->data;
	i2c_para.data_byte = 1;

	return hi_i2c_write(i2c_minor, &i2c_para);
} 

