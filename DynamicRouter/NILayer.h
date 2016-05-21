#pragma once
#include "baselayer.h"
#include "pcap.h"
#include "stdafx.h"

class CNILayer :
	public CBaseLayer
{
public:
	CNILayer(char* pName);
	~CNILayer(void);

	BOOL Send(unsigned char* ppayload, int nlength, unsigned short dev_num);
	BOOL Receive(unsigned char* ppayload, unsigned short dev_num);
	void setDev_1(pcap_t* dev);
	void setDev_2(pcap_t* dev);

private :
	pcap_t* Dev_1;
	pcap_t* Dev_2;
};