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
		unsigned int globel_en:1;		/* [0] */
		unsigned int seq_wave1_en:1;	/* [1] */
		unsigned int seq_wave2_en:1;	/* [2] */
		unsigned int seq_wave3_en:1;	/* [3] */
		unsigned int seq_wave4_en:1;	/* [4] */
		unsigned int seq_stay1_en:1;	/* [5] */
		unsigned int seq_stay2_en:1;	/* [6] */
		unsigned int seq_stay3_en:1;	/* [7] */
		unsigned int seq_stay4_en:1;	/* [8] */
		unsigned int seq_trans1_en:1;	/* [9] */
		unsigned int seq_trans2_en:1;	/* [10] */
		unsigned int seq_trans3_en:1;	/* [11] */
		unsigned int seq_trans4_en:1;	/* [12] */
		unsigned int wave_gest1_en:1;	/* [13] */
		unsigned int wave_gest2_en:1;	/* [14] */
		unsigned int wave_gest3_en:1;	/* [15] */
		unsigned int wave_gest4_en:1;	/* [16] */
		unsigned int stay_gest1_en:1;	/* [17] */
		unsigned int stay_gest2_en:1;	/* [18] */
		unsigned int stay_gest3_en:1;	/* [19] */
		unsigned int stay_gest4_en:1;	/* [20] */
		unsigned int trans_gest1_en:1;	/* [21] */
		unsigned int trans_gest2_en:1;	/* [22] */
		unsigned int trans_gest3_en:1;	/* [23] */
		unsigned int trans_gest4_en:1;	/* [24] */
		unsigned int res:7;				/* [31:25] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_sys_interrupt_ctrl;

typedef union{
	/* define struct bits */
	struct{
		unsigned int globel:1;		/* [0] */
		unsigned int seq_wave1:1;	/* [1] */
		unsigned int seq_wave2:1;	/* [2] */
		unsigned int seq_wave3:1;	/* [3] */
		unsigned int seq_wave4:1;	/* [4] */
		unsigned int seq_stay1:1;	/* [5] */
		unsigned int seq_stay2:1;	/* [6] */
		unsigned int seq_stay3:1;	/* [7] */
		unsigned int seq_stay4:1;	/* [8] */
		unsigned int seq_trans1:1;	/* [9] */
		unsigned int seq_trans2:1;	/* [10] */
		unsigned int seq_trans3:1;	/* [11] */
		unsigned int seq_trans4:1;	/* [12] */
		unsigned int wave_gest1:1;	/* [13] */
		unsigned int wave_gest2:1;	/* [14] */
		unsigned int wave_gest3:1;	/* [15] */
		unsigned int wave_gest4:1;	/* [16] */
		unsigned int stay_gest1:1;	/* [17] */
		unsigned int stay_gest2:1;	/* [18] */
		unsigned int stay_gest3:1;	/* [19] */
		unsigned int stay_gest4:1;	/* [20] */
		unsigned int trans_gest1:1;	/* [21] */
		unsigned int trans_gest2:1;	/* [22] */
		unsigned int trans_gest3:1;	/* [23] */
		unsigned int trans_gest4:1;	/* [24] */
		unsigned int res:7;				/* [31:25] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_sys_interrupt_status;

typedef union{
	/* define struct bits */
	struct{
		unsigned int config:2;		/* [1:0] */
		unsigned int res:30;		/* [31:2] */
	}bits;
	/* define an unsigned member */
	unsigned int u32;
}u_sys_interrupt_config;

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
	u_sys_interrupt_ctrl sys_interrupt_ctrl;			/* 0x0C */
	u_sys_interrupt_config sys_interrupt_config;	/* 0x10 */
	u_sys_interrupt_status sys_interrupt_status;	/* 0x14 */
	u_pos_xy pos_lc_xy;	/* 0x18 */
	u_pos_z pos_lc_z; 		/* 0x1C */
	u_pos_xy pos_l1_xy; 	/* 0x20 */
	u_pos_z pos_l1_z;		/* 0x24 */
	u_pos_xy pos_l2_xy; 	/* 0x28 */
	u_pos_z pos_l2_z;		/* 0x2C */
	u_pos_xy pos_l3_xy; 	/* 0x30 */
	u_pos_z pos_l3_z;		/* 0x34 */
	u_pos_xy pos_l4_xy; 	/* 0x38 */
	u_pos_z pos_l4_z;		/* 0x3C */
	u_pos_xy pos_l5_xy; 	/* 0x40 */
	u_pos_z pos_l5_z;		/* 0x44 */
	u_pos_xy pos_rc_xy;	/* 0x48 */
	u_pos_z pos_rc_z; 		/* 0x4C */
	u_pos_xy pos_r1_xy; 	/* 0x50 */
	u_pos_z pos_r1_z;		/* 0x54 */
	u_pos_xy pos_r2_xy; 	/* 0x58 */
	u_pos_z pos_r2_z;		/* 0x5C */
	u_pos_xy pos_r3_xy; 	/* 0x60 */
	u_pos_z pos_r3_z;		/* 0x64 */
	u_pos_xy pos_r4_xy; 	/* 0x68 */
	u_pos_z pos_r4_z;		/* 0x6C */
	u_pos_xy pos_r5_xy; 	/* 0x70 */
	u_pos_z pos_r5_z;		/* 0x74 */
	u_static_gesture static_gest;		/* 0x78 */
	u_wave_gesture wave_gest;			/* 0x7C */
	u_static_gesture stay_gest;			/* 0x80 */
	u_static_gesture trans_gest_from;		/* 0x84 */
	u_static_gesture trans_gest_to;			/* 0x88 */
}virtual_regs_s;


#endif /* _VIRREG_H */
