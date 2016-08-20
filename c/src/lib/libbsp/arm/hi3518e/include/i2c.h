#ifndef _I2C_H
#define _I2C_H

#include <rtems.h>

//#define USE_DMAC

#define HI_I2C_UNLOCK_VALUE	0x1ACCE551
#define HI_I2C_ENABLE		(1 << 0)
#define HI_I2C_AUTO_MODE_OFF	0x0f000000
#define HI_I2C_FAST_MODE		0x65
#define HI_I2C_RX_FIFO			0x8
#define HI_I2C_TX_FIFO			0x8
#define DEFAULT_I2C_REG_IMSC		0x0UL
#define DISABLE_ALL_INTERRUPTS		((~DEFAULT_I2C_REG_IMSC) & 0xfff)
#define ENABLE_ALL_INTERRUPTS		DEFAULT_I2C_REG_IMSC
#define I2C_WAIT_IDLE_TIME_OUT  0x1000000
#define I2C_RAW_TX_ABORT            (1 << 6)
#define HI_I2C_WRITE		0x80000000
#define HI_I2C_READ		0xc0000000
#define I2C_AUTO_DATA               (1 << 28)
#define I2C_AUTO_ADDR               (1 << 29)


/* I2C_STATUS */
#define I2C_AUTO_RX_FIFO_NOT_EMPTY  (1 << 8)
#define I2C_AUTO_TX_FIFO_EMPTRY     (1 << 20)
#define I2C_AUTO_TX_FIFO_NOT_FULL   (1 << 21)
#define I2C_STATUS_WORKING          (1 << 0)

#define IS_TX_FIFO_EMPTY(status)        (((status) &\
			I2C_AUTO_TX_FIFO_EMPTRY) == I2C_AUTO_TX_FIFO_EMPTRY)
#define IS_RX_FIFO_EMPTY(status)        (((status) &\
			I2C_AUTO_RX_FIFO_NOT_EMPTY) == 0)
#define IS_FIFO_EMPTY(status)           (IS_RX_FIFO_EMPTY(status) &&\
			IS_TX_FIFO_EMPTY(status))
#define IS_I2C_IDLE(status)             (((status) & I2C_STATUS_WORKING) == 0)

#define I2C_WAIT_TIME_OUT       0x5000 
#define CFG_I2C_IRQEN				(1<<20)

/* Asic apb clk is 100M */
#define I2C_DFT_CLK            (100000000)    //(25000000)
#define I2C_DFT_RATE           (100000)

#define READ_OPERATION     (1)
#define WRITE_OPERATION    0xfe
#define CMD_I2C_WRITE      0x01
#define CMD_I2C_READ       0x03

/* I2C_CTRL_REG */
#define I2C_ENABLE             (1 << 8)
#define I2C_UNMASK_TOTAL       (1 << 7)
#define I2C_UNMASK_START       (1 << 6)
#define I2C_UNMASK_END         (1 << 5)
#define I2C_UNMASK_SEND        (1 << 4)
#define I2C_UNMASK_RECEIVE     (1 << 3)
#define I2C_UNMASK_ACK         (1 << 2)
#define I2C_UNMASK_ARBITRATE   (1<< 1)
#define I2C_UNMASK_OVER        (1 << 0)
#define I2C_UNMASK_ALL         (I2C_UNMASK_START | I2C_UNMASK_END | \
                                I2C_UNMASK_SEND | I2C_UNMASK_RECEIVE | \
                                I2C_UNMASK_ACK | I2C_UNMASK_ARBITRATE | \
                                I2C_UNMASK_OVER)

/* I2C_COM_REB */
#define I2C_SEND_ACK (~(1 << 4))
#define I2C_START (1 << 3)
#define I2C_READ (1 << 2)
#define I2C_WRITE (1 << 1)
#define I2C_STOP (1 << 0)

/* I2C_ICR_REG */
#define I2C_CLEAR_START (1 << 6)
#define I2C_CLEAR_END (1 << 5)
#define I2C_CLEAR_SEND (1 << 4)
#define I2C_CLEAR_RECEIVE (1 << 3)
#define I2C_CLEAR_ACK (1 << 2)
#define I2C_CLEAR_ARBITRATE (1 << 1)
#define I2C_CLEAR_OVER (1 << 0)
#define I2C_CLEAR_ALL (I2C_CLEAR_START | I2C_CLEAR_END | \
                       I2C_CLEAR_SEND | I2C_CLEAR_RECEIVE | \
                       I2C_CLEAR_ACK | I2C_CLEAR_ARBITRATE | \
                       I2C_CLEAR_OVER)

/* I2C_SR_REG */
#define I2C_BUSY (1 << 7)
#define I2C_START_INTR (1 << 6)
#define I2C_END_INTR (1 << 5)
#define I2C_SEND_INTR (1 << 4)
#define I2C_RECEIVE_INTR (1 << 3)
#define I2C_ACK_INTR (1 << 2)
#define I2C_ARBITRATE_INTR (1 << 1)
#define I2C_OVER_INTR (1 << 0)

typedef enum i2c_mode_e {
	I2C_MODE_AUTO,
	I2C_MODE_DMA,
	I2C_MODE_NONE,
} i2c_mode_e;

typedef struct
{
	uint8_t dev_addr;
	uint32_t reg_addr;
	uint32_t reg_byte;
	uint32_t data;
	uint32_t data_byte;
}i2c_para_s;

void hi_i2c_init(int minor);
int hi_i2c_write(int minor, i2c_para_s *i2c_para);
int hi_i2c_read(int minor, i2c_para_s *i2c_para);

#endif /* _I2C_H */
