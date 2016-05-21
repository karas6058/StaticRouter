#include "stdafx.h"
#include "NILayer.h"


CNILayer::CNILayer(char* pName) : CBaseLayer(pName)
{
}

CNILayer::~CNILayer(void)
{
}
BOOL CNILayer::Send(unsigned char* ppayload, int nlength, unsigned short dev_num)
{
	if(dev_num == 0)
		return pcap_sendpacket(Dev_1, ppayload, nlength);
	else if(dev_num == 1)
		return pcap_sendpacket(Dev_2, ppayload, nlength);
	else
		return false;
}
BOOL CNILayer::Receive(unsigned char* ppayload, unsigned short dev_num)
{
	//받은 패킷을 그대로 올린다
	return mp_aUpperLayer[0]->Receive(ppayload,dev_num);
}
void CNILayer::setDev_1(pcap_t* dev)
{
	Dev_1 = dev;
}
void CNILayer::setDev_2(pcap_t* dev)
{
	Dev_2 = dev;
}