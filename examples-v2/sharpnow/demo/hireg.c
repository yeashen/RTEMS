/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : hireg.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/05/17
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/05/17
   Author       : Juven
   Modification : Created file

******************************************************************************/
#include <debug.h>
#include <i2c.h>

#include <stdio.h>
#include "system.h"

extern unsigned int fcc, fstart;

static int hex_atoi(char * pstr)
{
	int res = 0;
	int sign = 1;

	if(pstr == NULL)
	{
		printf("Point is NULL");
		return -1;
	}

	while(*pstr == ' ')
		pstr++;
	if(*pstr == '-')
		sign = -1;
	if(*pstr == '-' || *pstr == '+')
		pstr++;

	while((*pstr >= '0' && *pstr <= '9') || (*pstr >= 'a' && *pstr <= 'f')
				|| (*pstr >= 'A' && *pstr <= 'F'))
	{
		if(*pstr >= '0' && *pstr <= '9')
			res = res * 16 + *pstr - '0';
		else if(*pstr >= 'a' && *pstr <= 'f')
			res = res * 16 + *pstr - '0' - 39;
		else
			res = res * 16 + *pstr - '0' - 7;
		pstr++;
	}
	res *= sign;

	return res;
}


int himd_reg(int argc, char *argv[])
{
	unsigned int reg_base = 0x20580000;
	unsigned int val = 0;

	if(argc < 2)
	{
		printf("Useage: himd regaddr. ie: himd 20580000\n");
		return -1;
	}

	reg_base = hex_atoi(argv[1]);
	val = phyreg_read(reg_base);
	printf("\nreg: 0x%8X --> 0x%x\n\n", reg_base, val);

	return 0;
}

int himm_reg(int argc, char *argv[])
{
	unsigned int reg_base = 0x20580000;
	unsigned int val = 0;

	if(argc < 3)
	{
		printf("Useage: himm regaddr value. ie: himd 20580000 1F\n");
		return -1;
	}

	reg_base = hex_atoi(argv[1]);
	val = hex_atoi(argv[2]);
	phyreg_write(reg_base, val);
	printf("\nreg: 0x%8X --> 0x%x\n\n", reg_base, val);

	return 0;
}

int i2creg_read(int argc, char *argv[])
{
	int ret = 0;
	unsigned int i2c_minor;
	i2c_para_s i2c_para;

	if(argc < 4)
	{
		printf("Useage: i2crd i2c_num devaddr regaddr.  ie: i2crd 0 60(hex) 00(hex)\n");
		return -1;
	}

	i2c_minor = argv[1][0] - '0';
	i2c_para.dev_addr = hex_atoi(argv[2]);
	i2c_para.reg_addr = hex_atoi(argv[3]);
	i2c_para.reg_byte = 1;
	i2c_para.data_byte = 1;
	ret = hi_i2c_read(i2c_minor, &i2c_para);
	if(ret < 0)
	{
		printf("\ni2c read 0x%x failed,please check and try again!\n\n", i2c_para.dev_addr);
		return 0;
	}

	printf("\n[0x%x]: 0x%x --> 0x%x\n\n", i2c_para.dev_addr, i2c_para.reg_addr, i2c_para.data&0xFF);

	return 0;
}

int i2creg_write(int argc, char *argv[])
{
	int ret = 0;
	unsigned int i2c_minor;
	i2c_para_s i2c_para;

	if(argc < 5)
	{
		printf("Useage: i2cwr i2c_num devaddr regaddr value.  ie: i2cwr 0 60(hex) 00(hex) 00(hex)\n");
		return -1;
	}

	i2c_minor = argv[1][0] - '0';
	i2c_para.dev_addr = hex_atoi(argv[2]);
	i2c_para.reg_addr = hex_atoi(argv[3]);
	i2c_para.data = hex_atoi(argv[4]);
	i2c_para.reg_byte = 1;
	i2c_para.data_byte = 1;
	ret = hi_i2c_write(i2c_minor, &i2c_para);
	if(ret < 0)
	{
		printf("\ni2c write 0x%x failed,please check and try again!\n\n", i2c_para.dev_addr);
		return 0;
	}

	return 0;
}

int show_vicap_int(int argc, char *argv[])
{
	printf("\ncapture: %d frame, detect %d start\n\n", fcc, fstart);

	return 0;
}

int system_reboot(int argc, char *argv[])
{
	printf("\nafter 1s system will reboot...\n\n");
	usleep(1000000);
	phyreg_write(0x20050044, 0x1ACCE551);
	phyreg_write(0x20050004, 0x1);

	return 0;
}