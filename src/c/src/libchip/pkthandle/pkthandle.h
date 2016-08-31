#ifndef _PKTHANDLE_H
#define _PKTHANDLE_H

typedef struct{
	unsigned char start; 	/* packet head */
	unsigned char type;		/* packet type */
	unsigned char cmd; 		/* command */
	unsigned char len;		/* param length */
	unsigned char params[256];		/* params data */
	unsigned short checksum;	/* packet checksum */
}danmu_pkt_s;

typedef enum{
	CHECK_CHECKSUM = 0,
	GEN_CHECKSUM
}pkt_type_checksum;

typedef enum{
	PKT_REQ_SET = 0,
	PKT_REQ_GET,
	PKT_RSP_SET,
	PKT_RSP_GET
}pkt_type_e;

#define PACKET_START 		0x18

#define DAMNU_UART_MINOR		0

/* command define */
#define CMD_SYS_INFO		0x00
#define CMD_SYS_RESET		0x10
#define CMD_SYS_START		0x11
#define CMD_INT_ENABLE_GLOBAL 		0x80
#define CMD_INT_ENABLE_SEQ 			0x81
#define CMD_INT_ENABLE_WAVE 		0x82
#define CMD_INT_ENABLE_STAY 			0x83
#define CMD_INT_ENABLE_TRANS 		0x84
#define CMD_INT_STATUS_GLOBAL		0x85
#define CMD_INT_STATUS_SEQ 			0x86
#define CMD_INT_STATUS_WAVE 		0x87
#define CMD_INT_STATUS_STAY 		0x88
#define CMD_INT_STATUS_TRANS 		0x89
#define CMD_INT_CONFIG 					0x90
#define CMD_GDATA_POS_LC				0xc0
#define CMD_GDATA_POS_L1				0xc1
#define CMD_GDATA_POS_L2				0xc2
#define CMD_GDATA_POS_L3				0xc3
#define CMD_GDATA_POS_L4				0xc4
#define CMD_GDATA_POS_L5				0xc5
#define CMD_GDATA_POS_RC				0xc6
#define CMD_GDATA_POS_R1				0xc7
#define CMD_GDATA_POS_R2				0xc8
#define CMD_GDATA_POS_R3				0xc9
#define CMD_GDATA_POS_R4				0xca
#define CMD_GDATA_POS_R5				0xcb
#define CMD_STATIC_GEST1				0xd0
#define CMD_STATIC_GEST2				0xd1
#define CMD_STATIC_GEST3				0xd2
#define CMD_STATIC_GEST4				0xd3
#define CMD_WAVE_GEST1					0xd4
#define CMD_WAVE_GEST2					0xd5
#define CMD_WAVE_GEST3					0xd6
#define CMD_WAVE_GEST4					0xd7
#define CMD_STAY_GEST1					0xd8
#define CMD_STAY_GEST2					0xd9
#define CMD_STAY_GEST3					0xda
#define CMD_STAY_GEST4					0xdb
#define CMD_TRANS_GEST1				0xdc
#define CMD_TRANS_GEST2				0xdd
#define CMD_TRANS_GEST3				0xde
#define CMD_TRANS_GEST4				0xdf


int packet_handle(danmu_pkt_s *pkt);

#endif /* _PKTHANDLE_H */
