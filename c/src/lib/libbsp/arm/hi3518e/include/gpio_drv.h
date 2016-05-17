#ifndef _GPIO_DRV_H
#define _GPIO_DRV_H

#define CFG_GPIO0_IRQEN				(1<<29)
#define CFG_GPIO3_IRQEN				(1<<30)
#define CFG_GPIO6_IRQEN				(1<<31)

typedef enum
{
	GPIO_PIN0 = 0,
	GPIO_PIN1,
	GPIO_PIN2,
	GPIO_PIN3,
	GPIO_PIN4,
	GPIO_PIN5,
	GPIO_PIN6,
	GPIO_PIN7,
}GPIO_PIN_NUM;

typedef enum
{
	GPIO_INPUT = 0,
	GPIO_OUTPUT,
}GPIO_DIRECTION;

typedef enum
{
	GPIO_EDGE_TRIG = 0,
	GPIO_LEVEL_TRIG,
}GPIO_TRIG_TYPE;

typedef enum
{
	GPIO_SIGGLE_EDGE = 0,
	GPIO_BOTH_EDGE,
}GPIO_EDGE_TYPE;

typedef enum
{
	GPIO_FALLING_TRIG = 0,
	GPIO_LOW_TRIG = 0,
	GPIO_RISING_TRIG = 1,
	GPIO_HIGH_TRIG = 1,
}GPIO_IEV_TYPE;

typedef enum
{
	GPIO_INT_MASK = 0,
	GPIO_INT_UNMASK,
}GPIO_INT_EN;

typedef enum
{
	GPIO_HAS_INT = 0,
	GPIO_NO_INT,
}GPIO_INT_STA;

typedef struct
{
	GPIO_PIN_NUM pin_int;
	GPIO_TRIG_TYPE type;
	GPIO_IEV_TYPE iev;
	GPIO_EDGE_TYPE edge_type;
}GPIO_INT_PATA;

void hi_gpio_set_derection(GPIO_PIN_NUM pin, GPIO_DIRECTION dir);
unsigned int hi_gpio_get(GPIO_PIN_NUM pin);
void hi_gpio_set(GPIO_PIN_NUM pin, unsigned int val);
void hi_gpio_int_config(GPIO_INT_PATA *para);
unsigned int hi_gpio_int_query();
void hi_gpio_int_clear(GPIO_PIN_NUM pin);

#endif /* _GPIO_DRV_H */
