/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : i2c.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/03/22
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/03/22
   Author       : Juven
   Modification : Created file

******************************************************************************/

#include <dev/i2c/i2c.h>
#include <i2c.h>
#include <hi3518e.h>
#include <unistd.h>

#define I2C_NUM		1

typedef struct {
    int minor;
    hi_i2c_regs_s * regs;
} hi_i2c_data_s;

static hi_i2c_data_s i2c_data[I2C_NUM];
static hi_pinmux_regs_s *pinmux_regs = NULL;

uint8_t i2c_init_flag = 0;

static void i2c_set_rate(int minor, uint32_t i2c_rate)
{
	uint32_t val, sclH, sclL;

	/* read ctrl */
	val = i2c_data[minor].regs->ctrl;

	/* mask i2c int */
	i2c_data[minor].regs->ctrl = val & (~I2C_UNMASK_TOTAL);
	sclH = (I2C_DFT_CLK / (i2c_rate * 2)) / 2 - 1;
	i2c_data[minor].regs->scl_h = sclH;
	sclL = (I2C_DFT_CLK / (i2c_rate * 2)) / 2 - 1;
	i2c_data[minor].regs->scl_l = sclL;
	
	/* unmask i2c int */
	i2c_data[minor].regs->ctrl = val;
}

static int hi_i2c_wait_writeend(int minor)
{
	uint32_t src_reg, i = 0;

	do{
		src_reg = i2c_data[minor].regs->sr;
		if(i > I2C_WAIT_TIME_OUT){
			return -1;
		}
		i++;
	}while((src_reg & I2C_OVER_INTR) != I2C_OVER_INTR);
	i2c_data[minor].regs->icr = I2C_CLEAR_ALL;
	return 0;
}

static int hi_i2c_wait_readend(int minor)
{
	uint32_t src_reg, i = 0;

	do{
		src_reg = i2c_data[minor].regs->sr;
		if(i > I2C_WAIT_TIME_OUT){
			return -1;
		}
		i++;
	}while((src_reg & I2C_RECEIVE_INTR) != I2C_RECEIVE_INTR);

	return 0;		
}

void hi_i2c_init(int minor)
{
	i2c_data[minor].minor = minor;

	if(i2c_init_flag)
		return;

	i2c_data[minor].regs = (hi_i2c_regs_s *) I2C_REG_BASE;
	pinmux_regs =  (hi_pinmux_regs_s *)PINMUX_REG_BASE;

	pinmux_regs->sns_clk = 0x1;
	
	/* set i2c pinmux */
	pinmux_regs->i2c_sda = 0x1;
	pinmux_regs->i2c_scl = 0x1;

	i2c_set_rate(minor, I2C_DFT_RATE);

	/* sensor clk */
	HI_REG_WR(CRG_REG_BASE+CRG_SENSOR_CLK, 0x1);//Sensor clock 24 MHz

	i2c_init_flag = 1;
}

int hi_i2c_write(int minor, i2c_para_s *i2c_para)
{
	uint32_t i;
	uint8_t dev_addr, reg_byte, data_byte;
	uint32_t reg_addr, data;
	uint32_t addr, tdata = 0;
	int ret = 0;

	dev_addr = i2c_para->dev_addr;
	reg_addr = i2c_para->reg_addr;
	reg_byte = i2c_para->reg_byte;
	data = i2c_para->data;
	data_byte = i2c_para->data_byte;

	/* clear int mask */
	i2c_data[minor].regs->icr = 0x03;
	i2c_data[minor].regs->ctrl = 0x187;
	/* send dev address */
	i2c_data[minor].regs->txr = dev_addr & WRITE_OPERATION;
	i2c_data[minor].regs->com = I2C_WRITE | I2C_START;
	ret = hi_i2c_wait_writeend(minor);
	if(ret < 0)
		return -1;

	/* send reg addr */
	for (i = 0; i < reg_byte; i++) {
		addr = reg_addr >> ((reg_byte-i-1)*8);
		i2c_data[minor].regs->txr = addr;
		i2c_data[minor].regs->com = I2C_WRITE;
		ret = hi_i2c_wait_writeend(minor);
		if(ret < 0)
			return -1;
	}

	/* send data */
	for (i = 0; i < data_byte; i++) {
		tdata = data >> ((data_byte-i-1)*8);
		i2c_data[minor].regs->txr = tdata;
		i2c_data[minor].regs->com = I2C_WRITE;
		ret = hi_i2c_wait_writeend(minor);
		if(ret < 0)
			return -1;
		i2c_data[minor].regs->icr = I2C_CLEAR_ALL;
	}
	/* send stop */
	i2c_data[minor].regs->com = I2C_STOP;
	hi_i2c_wait_writeend(minor);
	i2c_data[minor].regs->icr = 0x01;
	
	return 0;
}

int hi_i2c_read(int minor, i2c_para_s *i2c_para)
{
	uint8_t dev_addr, reg_byte, data_byte;
	uint32_t reg_addr;
	uint32_t tmp, i, addr;
	uint32_t tdata = 0;
	int ret = 0;

	dev_addr = i2c_para->dev_addr;
	reg_addr = i2c_para->reg_addr;
	reg_byte = i2c_para->reg_byte;
	data_byte = i2c_para->data_byte;

	/* clear int mask */
	i2c_data[minor].regs->icr = 0x03;
	i2c_data[minor].regs->ctrl = 0x187;
	/* send dev address */
	i2c_data[minor].regs->txr = dev_addr & WRITE_OPERATION;
	i2c_data[minor].regs->com = I2C_WRITE | I2C_START;
	ret = hi_i2c_wait_writeend(minor);
	if(ret < 0)
		return -1;
	/* send reg addr */
	for (i = 0; i < reg_byte; i++) {
		addr = reg_addr >> ((reg_byte-i-1)*8);
		i2c_data[minor].regs->txr = addr;
		i2c_data[minor].regs->com = I2C_WRITE;
		ret = hi_i2c_wait_writeend(minor);
		if(ret < 0)
			return -1;
	}

	/* send readdev address */
	i2c_data[minor].regs->txr = dev_addr & READ_OPERATION;
	i2c_data[minor].regs->com = I2C_WRITE | I2C_START;
	ret = hi_i2c_wait_writeend(minor);
	if(ret < 0)
		return -1;
	
	/* read data */
	for (i = 0; i < data_byte; i++) {
		if(i == (data_byte-1)){
			i2c_data[minor].regs->com = I2C_READ | (~I2C_SEND_ACK);
		}else{
			i2c_data[minor].regs->com = I2C_READ;
		}
		ret = hi_i2c_wait_readend(minor);
		if(ret < 0)
			return -1;
		tmp = i2c_data[minor].regs->rxr;
		tdata |= (tmp << ((data_byte-i-1)*8));
		ret = hi_i2c_wait_writeend(minor);
		if(ret < 0)
			return -1;
	}

	/* send stop */
	i2c_data[minor].regs->com = I2C_STOP;
	ret = hi_i2c_wait_writeend(minor);
	if(ret < 0)
		return -1;
	
	i2c_para->data = tdata;
	
	return 0;
}
