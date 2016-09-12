#ifndef _VIRREG_H
#define _VIRREG_H

/* system control */
typedef union{
	/* define struct bits */
	struct{
		unsigned int model:32;		/* [31:0] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_sys_info_model;

typedef union{
	/* define struct bits */
	struct{
		unsigned int hw_version:8;		/* [7:0] */
		unsigned int fw_version:8;		/* [15:8] */
		unsigned int sw_version:8;		/* [23:16] */
		unsigned int res:8;		/* [31:24] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_sys_info_version;

typedef union{
	/* define struct bits */
	struct{
		unsigned int reset:1;		/* [0] */
		unsigned int start:2;		/* [2:1] */
		unsigned int res:29;		/* [31:3] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_sys_power;

/* interrupt control */
typedef union{
	/* define struct bits */
	struct{
		unsigned int global:1;	/* [0] */
		unsigned int trans:1;	/* [1] */
		unsigned int res:30;		/* [31:2] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_global_int_ctrl;

/* interrupt control */
typedef union{
	/* define struct bits */
	struct{
		unsigned int global:1;	/* [0] */
		unsigned int trans:1;	/* [1] */
		unsigned int res:30;		/* [31:2] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_global_int_status;

/* interrupt control */
typedef union{
	/* define struct bits */
	struct{
		unsigned int zero:1;	/* [0] */
		unsigned int one:1;	/* [1] */
		unsigned int two:1;	/* [2] */
		unsigned int three:1;/* [3] */
		unsigned int four:1;	/* [4] */
		unsigned int five:1;	/* [5] */
		unsigned int res:26;		/* [31:6] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_gest_int_ctrl;

/* interrupt control */
typedef union{
	/* define struct bits */
	struct{
		unsigned int zero:1;	/* [0] */
		unsigned int one:1;	/* [1] */
		unsigned int two:1;	/* [2] */
		unsigned int three:1;/* [3] */
		unsigned int four:1;	/* [4] */
		unsigned int five:1;	/* [5] */
		unsigned int res:26;		/* [31:6] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_gest_int_status;

typedef union{
	/* define struct bits */
	struct{
		unsigned int x:16;		/* [15:0] */
		unsigned int y:16;		/* [31:16] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_pos_xy;

typedef union{
	/* define struct bits */
	struct{
		unsigned int z:16;		/* [15:0] */
		unsigned int res:16;	/* [32:16] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_pos_z;

typedef union{
	/* define struct bits */
	struct{
		unsigned int zero:1;		/* [0] */
		unsigned int one:1;			/* [1] */
		unsigned int two:1;			/* [2] */
		unsigned int three:1;		/* [3] */
		unsigned int four:1;		/* [4] */
		unsigned int five:1;		/* [5] */
		unsigned int res:26;		/* [31:6] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_static_gesture;

typedef union{
	/* define struct bits */
	struct{
		unsigned int up:1;			/* [0] */
		unsigned int down:1;		/* [1] */
		unsigned int left:1;		/* [2] */
		unsigned int right:1;		/* [3] */
		unsigned int press:1;		/* [4] */
		unsigned int res:27;		/* [31:5] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_wave_gesture;

typedef struct{
	u_sys_info_model sys_info_model;					/* 0x00 */
	u_sys_info_version sys_info_version;				/* 0x04 */
	u_sys_power sys_power;									/* 0x08 */
	u_global_int_ctrl global_int_ctrl;				/* 0x0C */
	u_global_int_status global_int_status;	/* 0x10 */
	u_gest_int_ctrl wave_int_ctrl;					/* 0x14 */
	u_gest_int_ctrl stay_int_ctrl;					/* 0x18 */
	u_gest_int_status wave_int_status;			/* 0x1C */
	u_gest_int_status stay_int_status;			/* 0x20 */
	u_pos_xy pos_lc_xy;	/* 0x24 */
	u_pos_z pos_lc_z; 		/* 0x28 */
	u_pos_xy pos_l1_xy; 	/* 0x2C */
	u_pos_z pos_l1_z;		/* 0x30 */
	u_pos_xy pos_l2_xy; 	/* 0x34 */
	u_pos_z pos_l2_z;		/* 0x38 */
	u_pos_xy pos_l3_xy; 	/* 0x3C */
	u_pos_z pos_l3_z;		/* 0x40 */
	u_pos_xy pos_l4_xy; 	/* 0x44 */
	u_pos_z pos_l4_z;		/* 0x48 */
	u_pos_xy pos_l5_xy; 	/* 0x4C */
	u_pos_z pos_l5_z;		/* 0x50 */
	u_pos_xy pos_rc_xy;	/* 0x54 */
	u_pos_z pos_rc_z; 		/* 0x58 */
	u_pos_xy pos_r1_xy; 	/* 0x5C */
	u_pos_z pos_r1_z;		/* 0x60 */
	u_pos_xy pos_r2_xy; 	/* 0x64 */
	u_pos_z pos_r2_z;		/* 0x68 */
	u_pos_xy pos_r3_xy; 	/* 0x6C */
	u_pos_z pos_r3_z;		/* 0x70 */
	u_pos_xy pos_r4_xy; 	/* 0x74 */
	u_pos_z pos_r4_z;		/* 0x78 */
	u_pos_xy pos_r5_xy; 	/* 0x7C */
	u_pos_z pos_r5_z;		/* 0x80 */
	u_static_gesture static_gest;		/* 0x84 */
	u_wave_gesture wave_gest;			/* 0x88 */
	u_static_gesture stay_gest;			/* 0x8C */
	u_static_gesture trans_gest_from;		/* 0x90 */
	u_static_gesture trans_gest_to;			/* 0x94 */
}virtual_regs_s;


#endif /* _VIRREG_H */
