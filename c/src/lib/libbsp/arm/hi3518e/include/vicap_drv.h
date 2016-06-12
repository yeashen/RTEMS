#ifndef _VICAP_DRV_H
#define _VICAP_DRV_H

#define PT_EN			(1<<31)
#define CH_EN			(1<<31)
#define ISP_INT_EN		(1<<24)
#define PT0_INT_EN		(1<<16)
#define CH0_INT_EN		(1<<0)
#define FIELD_THROW_EN  (1<<3)
#define BUF_OVF_EN		(1<<2)
#define CC_INT_EN		(1<<1)
#define FSTART_EN		(1<<0)

typedef enum{
	SENSOR_OV9712,
	SENSOR_POA030R,
}sensor_type_e;

void vicap_set_y_faddr(unsigned int addr);

void vicap_set_y_width(unsigned int width);

void vicap_set_y_height(unsigned int height);

void vicap_set_y_stride(unsigned int stride);

void vicap_set_c_faddr(unsigned int addr);

void vicap_set_c_width(unsigned int width);

void vicap_set_c_height(unsigned int height);

void vicap_set_c_stride(unsigned int stride);

void vicap_set_hact(unsigned int hact);

void vicap_set_vact(unsigned int vact);

void vicap_set_crop_width(unsigned int width);

void vicap_set_crop_height(unsigned int height);

void vicap_set_pack_y_width(unsigned int width);

void vicap_set_pack_c_width(unsigned int width);


unsigned int vicap_get_ch_int_en();

unsigned int vicap_get_ch_int_status(void);

void vicap_reg_newer(void);

void vicap_clear_ch_int(unsigned int int_mask);

unsigned int vicap_get_pt_height();

unsigned int vicap_get_pt_width();

void vicap_pin_init(sensor_type_e sns_type);

void vicap_reg_init(sensor_type_e sns_type);


#endif /* _VICAP_DRV_H */
