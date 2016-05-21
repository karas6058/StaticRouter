#pragma once
#include "BaseLayer.h"

typedef struct _IP{
	unsigned char ip_verlen;
	unsigned char ip_tos;
	unsigned short ip_len;
	unsigned short ip_id;
	unsigned short ip_fragoff;
	unsigned char ip_ttl;
	unsigned char ip_proto;
	unsigned short ip_cksum;
	unsigned char ip_src[4];
	unsigned char ip_dst[4];

	unsigned char ip_data[MAX_IP_DATA];
}IP, *PIP;

class CIPLayer : public CBaseLayer
{
public:
	CIPLayer(char* pName);
	~CIPLayer(void);

	BOOL			Send(unsigned char* ppayload, int length, unsigned short dev_num);
	BOOL			Receive(unsigned char* ppayload, unsigned short type, unsigned short dev_num);
	void			setDev_1_IP(unsigned char* ip);
	void			setDev_2_IP(unsigned char* ip);
	unsigned char*	getDev_1_IP(void);
	unsigned char*	getDev_2_IP(void);
	unsigned char*	getForwardingIp(unsigned char* dstip);
	void			setRouting_entry(Routing_table *routing_entry);
	void			setRouting_top(int *top);
	int				getForwardDev(unsigned char* dst);
	void			setDev_1_name(char *dev_name);
	void			setDev_2_name(char *dev_name);

private :
	unsigned char	Dev_1_IP[4];
	unsigned char	Dev_2_IP[4];
	Routing_table*	routing_entry;
	int*			routing_top;
	unsigned char	defaultGateway[4];
	char			dev_1_name[100];
	char			dev_2_name[100];
};
