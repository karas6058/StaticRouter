#pragma once
#include "baselayer.h"

class CARPLayer :
	public CBaseLayer
{
public:
	CARPLayer(char* pName);
	~CARPLayer(void);

	BOOL Send(unsigned char* ppayload, int nlength, unsigned short dev_num);
	BOOL Receive(unsigned char* ppayload, unsigned short dev_num);
	void UpdateCacheTable(IP_ADDR ip, ETHERNET_ADDR mac, int time, int type);
	int* getCache_top(void);
	static void timeCountThread(void* param);
	cache_entry* getcache_entry(void);
	void setForwardingIp(unsigned char* forwardingIp);
	unsigned char* getNextHopMac(unsigned char* forwardingIp);

private:
	int				msg_flag;
	unsigned char	storedMsg[MAX_IP_SIZE];
	cache_entry		cache_entry[300];
	int				cache_top;
	unsigned char	forwardingIp[4];
};
