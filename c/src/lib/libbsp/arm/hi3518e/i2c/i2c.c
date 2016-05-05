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
#include <platform.h>
#include <unistd.h>

uint8_t i2c_init_flag = 0;

static void i2c_set_rate(uint32_t i2c_rate)
{
	uint32_t val, sclH, sclL;

	/* read ctrl */
	val = I2C_RD_REG(I2C_CTRL_REG);

	/* mask i2c int */
	I2C_WR_REG(I2C_CTRL_REG, (val & (~I2C_UNMASK_TOTAL)));
	sclH = (I2C_DFT_CLK / (i2c_rate * 2)) / 2 - 1;
	I2C_WR_REG(I2C_SCL_H_REG, sclH);
	sclL = (I2C_DFT_CLK / (i2c_rate * 2)) / 2 - 1;
	I2C_WR_REG(I2C_SCL_L_REG, sclL);
	
	/* unmask i2c int */
	I2C_WR_REG(I2C_CTRL_REG, val);
}

static int hi_i2c_wait_writeend(void)
{
	uint32_t src_reg, i = 0;

	do{
		src_reg = I2C_RD_REG(I2C_SR_REG);
		if(i > I2C_WAIT_TIME_OUT){
			return -1;
		}
		i++;
	}while((src_reg & I2C_OVER_INTR) != I2C_OVER_INTR);
	I2C_WR_REG(I2C_ICR_REG, I2C_CLEAR_ALL);
	return 0;
}

static int hi_i2c_wait_readend(void)
{
	uint32_t src_reg, i = 0;

	do{
		src_reg = I2C_RD_REG(I2C_SR_REG);
		if(i > I2C_WAIT_TIME_OUT){
			return -1;
		}
		i++;
	}while((src_reg & I2C_RECEIVE_INTR) != I2C_RECEIVE_INTR);

	return 0;		
}

void hi_i2c_init(void)
{
	if(i2c_init_flag)
		return;

	MUXPIN_WR_REG(SENSOR_CLK_PIN, 0x1);//0:GPIO1_2   /1:SENSOR_CLK
	
	/* set i2c pinmux */
	MUXPIN_WR_REG(I2C_SDA_PIN, 0x1); //0:GPIO2_0   / 1:I2C_SDA
	MUXPIN_WR_REG(I2C_SCL_PIN, 0x1); //0:GPIO2_1   / 1:I2C_SCL

	i2c_set_rate(I2C_DFT_RATE);

	/* sensor clk */
	CRG_WR_REG(CRG_SENSOR_CLK, 0x1);//Sensor clock 24 MHz

	i2c_init_flag = 1;
}

int hi_i2c_write(uint8_t dev_addr, uint32_t reg_addr, uint32_t reg_byte, uint32_t data, uint32_t data_byte)
{
	uint32_t i;
	uint32_t addr, tdata = 0;
	int ret = 0;

	/* clear int mask */
	I2C_WR_REG(I2C_ICR_REG, 0x03);
	I2C_WR_REG(I2C_CTRL_REG, 0x187);
	/* send dev address */
	I2C_WR_REG(I2C_TXR_REG, (dev_addr & WRITE_OPERATION));
	I2C_WR_REG(I2C_COM_REG, (I2C_WRITE | I2C_START));
	ret = hi_i2c_wait_writeend();
	if(ret < 0)
		return -1;
	/* send reg addr */
	for (i = 0; i < reg_byte; i++) {
		addr = reg_addr >> ((reg_byte-i-1)*8);
		I2C_WR_REG(I2C_TXR_REG, addr);
		I2C_WR_REG(I2C_COM_REG, I2C_WRITE);
		ret = hi_i2c_wait_writeend();
		if(ret < 0)
			return -1;
	}

	/* send data */
	for (i = 0; i < data_byte; i++) {
		tdata = data >> ((data_byte-i-1)*8);
		I2C_WR_REG(I2C_TXR_REG, tdata);
		I2C_WR_REG(I2C_COM_REG, I2C_WRITE);
		ret = hi_i2c_wait_writeend();
		if(ret < 0)
			return -1;
		I2C_WR_REG(I2C_ICR_REG, I2C_CLEAR_ALL);
	}
	/* send stop */
	I2C_WR_REG(I2C_COM_REG, I2C_STOP);
	hi_i2c_wait_writeend();
	I2C_WR_REG(I2C_ICR_REG, 0x01);
	
	return 0;
}

int hi_i2c_read(uint8_t dev_addr, uint32_t reg_addr, uint32_t reg_byte, uint32_t *data, uint32_t data_byte)
{
	uint32_t tmp, i, addr;
	uint32_t tdata = 0;
	int ret = 0;

	/* clear int mask */
	I2C_WR_REG(I2C_ICR_REG, 0x03);
	I2C_WR_REG(I2C_CTRL_REG, 0x187);
	/* send dev address */
	I2C_WR_REG(I2C_TXR_REG, (dev_addr & WRITE_OPERATION));
	I2C_WR_REG(I2C_COM_REG, (I2C_WRITE | I2C_START));
	ret = hi_i2c_wait_writeend();
	if(ret < 0)
		return -1;
	/* send reg addr */
	for (i = 0; i < reg_byte; i++) {
		addr = reg_addr >> ((reg_byte-i-1)*8);
		I2C_WR_REG(I2C_TXR_REG, addr);
		I2C_WR_REG(I2C_COM_REG, I2C_WRITE);
		ret = hi_i2c_wait_writeend();
		if(ret < 0)
			return -1;
	}

	/* send readdev address */
	I2C_WR_REG(I2C_TXR_REG, (dev_addr & READ_OPERATION));
	I2C_WR_REG(I2C_COM_REG, (I2C_WRITE | I2C_START));
	ret = hi_i2c_wait_writeend();
	if(ret < 0)
		return -1;
	
	/* read data */
	for (i = 0; i < data_byte; i++) {
		if(i == (data_byte-1)){
			I2C_WR_REG(I2C_COM_REG, (I2C_READ | (~I2C_SEND_ACK)));
		}else{
			I2C_WR_REG(I2C_COM_REG, I2C_READ);	
		}
		ret = hi_i2c_wait_readend();
		if(ret < 0)
			return -1;
		tmp = I2C_RD_REG(I2C_RXR_REG);
		tdata |= (tmp << ((data_byte-i-1)*8));
		ret = hi_i2c_wait_writeend();
		if(ret < 0)
			return -1;
	}

	/* send stop */
	I2C_WR_REG(I2C_COM_REG, I2C_STOP);
	ret = hi_i2c_wait_writeend();
	if(ret < 0)
		return -1;
	
	*data = tdata;
	
	return 0;
}
