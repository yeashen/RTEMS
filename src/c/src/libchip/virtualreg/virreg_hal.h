#ifndef _VIRREG_HAL_H
#define _VIRREG_HAL_H

typedef struct{
	unsigned char hw_version;
	unsigned char fw_version;
	unsigned char sw_version;
}sys_version_s;

unsigned int virtualreg_init_reg(void);
void virreg_get_info_model(unsigned int *model);
void virreg_set_info_model(unsigned int model);
void virreg_get_info_version(sys_version_s * version);
void virreg_get_sys_start(unsigned int *start);
void virreg_set_sys_start(unsigned int start);
void virreg_get_interrupt_ctrl(unsigned int *ctrl);
void virreg_set_interrupt_ctrl(unsigned int ctrl);
void virreg_get_interrupt_config(unsigned int *config);
void virreg_set_interrupt_config(unsigned int config);
void virreg_get_interrupt_status(unsigned int *status);

#endif /* _VIRREG_HAL_H */
