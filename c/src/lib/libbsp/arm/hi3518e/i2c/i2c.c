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

//#define I2C_DEBUG

#ifdef I2C_DEBUG
#define i2c_debug(fmt, args...)	printk("[i2c] " fmt, ##args);
#else
#define i2c_debug(fmt, args...)
#endif

typedef struct {
    int minor;
    volatile hi_i2c_regs_s *regs;
} hi_i2c_data_s;

static hi_i2c_data_s i2c_data[I2C_NUM];
static volatile hi_pinmux_regs_s *pinmux_regs = NULL;

static uint8_t i2c_init_flag = 0;
static uint32_t g_last_dev_addr = 0;
static uint32_t g_last_work_mode = 0;

#ifdef HI3518EV200
static void i2c_abortprocess(int minor)
{
	uint32_t auto_status;

	/*disable i2c*/
	i2c_data[minor].regs->en = 0;

	auto_status = i2c_data[minor].regs->mst_ctrl;
	
	/* clear 0xB0 err status */
	/* auto_mst_tx_abrt_clr
	   auto_tx_cmd_fifo_over_clr
	   auto_rx_cmd_fifo_under_clr
	   auto_rx_cmd_fifo_over_clr
	 */
	 auto_status |= 0x0f000000;
	i2c_data[minor].regs->mst_ctrl = auto_status;
	i2c_data[minor].regs->int_clr = 1;

	/* enable i2c */
	i2c_data[minor].regs->en = 1;
}
#endif

static void i2c_set_rate(int minor, uint32_t i2c_rate)
{
#ifdef HI3518EV100	
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
#else defined HI3518EV200
	uint32_t apb_clk, sclH, sclL;
	/* get apb bus clk for diff plat */
	apb_clk = CFG_CLK_BUS/4;
	sclH =  (apb_clk / i2c_rate) / 2;
	sclL = sclH;

	i2c_data[minor].regs->scl_h = sclH;
	i2c_data[minor].regs->scl_l = sclL;

	i2c_data[minor].regs->sda_hold = sclH/2;
#endif //end HI3518EV100
}

#ifdef HI3518EV100
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
#else defined HI3518EV200
static int i2c_wait_idle(int minor)
{
	uint32_t work_status, auto_status, int_raw_status;
	uint32_t i = 0;
	int ret = 0;

	work_status = i2c_data[minor].regs->stat;
	auto_status = i2c_data[minor].regs->mst_ctrl;

	while (!(IS_FIFO_EMPTY(auto_status) && IS_I2C_IDLE(work_status))) {
		if (i > I2C_WAIT_IDLE_TIME_OUT) {
			i2c_debug("wait i2c idle timeout!" "auto_status: 0x%x, work_status: 0x%x\n", auto_status, work_status);
			ret = -1;
			break;
		}
		i++;
		
		work_status = i2c_data[minor].regs->stat;
		auto_status = i2c_data[minor].regs->mst_ctrl;
	};
#ifdef USE_DMAC
	uint32_t dmac_finish;
	dmac_finish = HI_REG_RD(0x10060000+0x010c);
	while (dmac_finish & 0xfff) {
		if (i > 0x10000)
			break;
		i++;
		dmac_finish = HI_REG_RD(0x10060000+0x010c);
	}
#endif	
	int_raw_status = i2c_data[minor].regs->int_raw;

	if ((int_raw_status & I2C_RAW_TX_ABORT) == I2C_RAW_TX_ABORT) {
 		i2c_debug("transmit error, int_raw_status: 0x%x!\n", int_raw_status);
		i2c_abortprocess(minor);
		ret = -1;
	}

	return ret;
}

static inline int i2c_set_dev_addr_and_mode(int minor, uint32_t dev_addr, uint32_t work_mode)
{
	if ((g_last_dev_addr == dev_addr) && (g_last_work_mode == work_mode))
		return 0;
	
	if (i2c_wait_idle(minor) < 0) {
		i2c_debug("wait i2c idle time out.\n");
		return -1;
	}

	/* disable i2c */
	i2c_data[minor].regs->en = 0;
	
	/* clear interrupt */
	i2c_data[minor].regs->int_clr = 1;
	
	/* enable interrupt mask */
	i2c_data[minor].regs->int_mask = DISABLE_ALL_INTERRUPTS;
	
	/* clear err status */
	i2c_data[minor].regs->mst_ctrl = 0x0f000000;

	/* different device, need to reinit i2c ctrl */
	if (g_last_dev_addr != dev_addr) {
		/* set slave dev addr */
		i2c_data[minor].regs->tar = (dev_addr & 0xff)>>1;
		g_last_dev_addr = dev_addr;
	}

	if (g_last_work_mode != work_mode) {
		/* set auto mode */
		if (work_mode == I2C_MODE_AUTO) {
			i2c_data[minor].regs->mst_cmd0 = 0;
			i2c_data[minor].regs->mst_ctrl = 0x80000000;
			g_last_work_mode = work_mode;
		} else if (work_mode == I2C_MODE_DMA) {
			i2c_data[minor].regs->mst_ctrl = 0;
			g_last_work_mode = work_mode;
		} else {
			i2c_debug("invalid i2c mode\n");
			return -1;
		}
	}

	/*  enable i2c */
	i2c_data[minor].regs->en = 1;

	return 0;
}

/* wait until tx fifo is not full */
static int i2c_wait_txfifo_notfull(int minor)
{
	uint32_t auto_status, int_raw_status;
	uint32_t i = 0;
	int ret = 0;

	auto_status = i2c_data[minor].regs->mst_ctrl;

	while((auto_status & I2C_AUTO_TX_FIFO_NOT_FULL) != I2C_AUTO_TX_FIFO_NOT_FULL){
		if (i > I2C_WAIT_TIME_OUT) {
			i2c_debug("wait timeout, auto_status: 0x%x!\n", auto_status);
			ret = -1;
			break;
		}
		i++;
		auto_status = i2c_data[minor].regs->mst_ctrl;
	}
	int_raw_status = i2c_data[minor].regs->int_raw;
	if((int_raw_status & I2C_RAW_TX_ABORT) == I2C_RAW_TX_ABORT){
		i2c_debug("transmit error, int_raw_status: 0x%x!\n", int_raw_status);
		i2c_debug("tx_abrt_cause is %x.\n", i2c_data[minor].regs->tx_src);
		i2c_abortprocess(minor);
		ret = -1;
	}

	return ret;
}

/* wait until tx fifo is not empty */
static int i2c_wait_rxfifo_notempty(int minor)
{
	uint32_t auto_status, int_raw_status;
	uint32_t i = 0;
	int ret = 0;

	auto_status = i2c_data[minor].regs->mst_ctrl;

	while((auto_status & I2C_AUTO_RX_FIFO_NOT_EMPTY) != I2C_AUTO_RX_FIFO_NOT_EMPTY){
		if (i > I2C_WAIT_TIME_OUT) {
			i2c_debug("wait timeout, auto_status: 0x%x!\n", auto_status);
			ret = -1;
			break;
		}
		i++;
		auto_status = i2c_data[minor].regs->mst_ctrl;
	}

	if(ret == -1){
		int_raw_status = i2c_data[minor].regs->int_raw;
		if((int_raw_status & I2C_RAW_TX_ABORT) == I2C_RAW_TX_ABORT){
			i2c_debug("transmit error, int_raw_status: 0x%x!\n", int_raw_status);
			i2c_debug("tx_abrt_cause is %x.\n", i2c_data[minor].regs->tx_src);
			i2c_abortprocess(minor);
			ret = -1;
		}
	}
	
	return ret;
}

#endif

void hi_i2c_init(int minor)
{
	i2c_data[minor].minor = minor;

	if(i2c_init_flag)
		return;

	i2c_data[minor].regs = (hi_i2c_regs_s *) I2C_REG_BASE;
	pinmux_regs =  (hi_pinmux_regs_s *)PINMUX_REG_BASE;

	pinmux_regs->sns_clk = 0x1;
#ifdef HI3518EV100	
	/* set i2c pinmux */
	pinmux_regs->i2c_sda = 0x1;
	pinmux_regs->i2c_scl = 0x1;

	i2c_set_rate(minor, I2C_DFT_RATE);

	/* sensor clk */
	//HI_REG_WR(CRG_REG_BASE+CRG_SENSOR_CLK, 0x1);//Sensor clock 24 MHz
	HI_REG_WR(CRG_REG_BASE+CRG_SENSOR_CLK, 0x5);//Sensor clock 27 MHz
#else defined HI3518EV200
	/* set i2c pinmux */
	pinmux_regs->spi0_sck = 0x2;
	pinmux_regs->spi0_sdo = 0x2;

	/* unlock hi_i2c controller to access */
	i2c_data[minor].regs->lock = HI_I2C_UNLOCK_VALUE;

	/* disable hi_i2c controller */
	i2c_data[minor].regs->en &= (~HI_I2C_ENABLE);
	
	/* disable hi_i2c auto_mode */
	i2c_data[minor].regs->mst_ctrl = HI_I2C_AUTO_MODE_OFF;
	
	/* set hi_i2c in fast mode */
	i2c_data[minor].regs->ctrl = HI_I2C_FAST_MODE;

	i2c_set_rate(minor, I2C_DFT_RATE);

	/* set hi_i2c fifo */
	i2c_data[minor].regs->rx_tl = HI_I2C_RX_FIFO;
	i2c_data[minor].regs->tx_tl = HI_I2C_TX_FIFO;

	/* enable interrupt mask */
	i2c_data[minor].regs->int_mask = DISABLE_ALL_INTERRUPTS;

	/* enable hi_i2c controller */
	i2c_data[minor].regs->en |=  HI_I2C_ENABLE;
	
	HI_REG_WR(CRG_REG_BASE+CRG_PERI11, 0xB4001);//Sensor clock 27 MHz VI 99MHz
#endif

	i2c_init_flag = 1;
}

int hi_i2c_write(int minor, i2c_para_s *i2c_para)
{
	uint8_t dev_addr, reg_byte, data_byte;
	uint32_t reg_addr, data;
	int ret = 0;

	dev_addr = i2c_para->dev_addr;
	reg_addr = i2c_para->reg_addr;
	reg_byte = i2c_para->reg_byte;
	data = i2c_para->data;
	data_byte = i2c_para->data_byte;
#ifdef HI3518EV100	
	uint32_t tdata = 0, i = 0, addr;

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
#else defined HI3518EV200
	uint32_t temp_auto_reg; 

	if(i2c_set_dev_addr_and_mode(minor, dev_addr, I2C_MODE_AUTO) < 0){
		return -1;
	}

	temp_auto_reg = HI_I2C_WRITE;

	if(reg_byte == 2){
		temp_auto_reg |= I2C_AUTO_ADDR;
	}

	if(data_byte == 2){
		temp_auto_reg |= I2C_AUTO_DATA;
	}

	i2c_data[minor].regs->mst_ctrl = temp_auto_reg;

	ret = i2c_wait_txfifo_notfull(minor);
	if(ret < 0){
		return -1;
	}

	/* set write reg&data */
	i2c_data[minor].regs->mst_cmd = ((reg_addr << 16) | data);
#endif
	return 0;
}

int hi_i2c_read(int minor, i2c_para_s *i2c_para)
{
	uint8_t dev_addr, reg_byte, data_byte;
	uint32_t reg_addr, tdata;
	int ret = 0;

	dev_addr = i2c_para->dev_addr;
	reg_addr = i2c_para->reg_addr;
	reg_byte = i2c_para->reg_byte;
	data_byte = i2c_para->data_byte;
#ifdef HI3518EV100	
	uint32_t i = 0, tmp, addr;

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
	i2c_data[minor].regs->txr = dev_addr | READ_OPERATION;
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
#else defined HI3518EV200
	uint32_t temp_auto_reg; 

	if(i2c_set_dev_addr_and_mode(minor, dev_addr, I2C_MODE_AUTO) < 0)
		return -1;

	temp_auto_reg = HI_I2C_READ;

	if(reg_byte == 2){
		temp_auto_reg |= I2C_AUTO_ADDR;
	}

	if(data_byte == 2){
		temp_auto_reg |= I2C_AUTO_DATA;
	}

	i2c_data[minor].regs->mst_ctrl = temp_auto_reg;

	ret = i2c_wait_txfifo_notfull(minor);
	if(ret < 0)
		return -1;

	/* 1. write addr */
	i2c_data[minor].regs->mst_cmd =reg_addr<<16;

	/* 2. read return data */
	/* wait until rx fifo not empty  */
	ret = i2c_wait_rxfifo_notempty(minor);
	if (ret < 0)
		return -1;

	tdata = i2c_data[minor].regs->mst_cmd & 0xFFFF;
	i2c_data[minor].regs->int_clr = 1;
#endif	
	i2c_para->data = tdata;
	
	return 0;
}
