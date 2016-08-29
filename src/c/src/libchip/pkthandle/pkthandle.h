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

#define DAMNU_UART_MINOR		2

int packet_handle(danmu_pkt_s *pkt);

#endif /* _PKTHANDLE_H */
