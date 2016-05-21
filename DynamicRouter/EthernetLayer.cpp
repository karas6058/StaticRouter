#include "StdAfx.h"
#include "EthernetLayer.h"

CEthernetLayer::CEthernetLayer( char* pName )
: CBaseLayer( pName )
{
}

CEthernetLayer::~CEthernetLayer(void)
{
}
BOOL CEthernetLayer::Send(unsigned char* ppayload, int length, unsigned short type, unsigned short dev_num)
{
	ETHERNET Ether;
	memcpy(Ether.enet_data,ppayload,length);
	if(dev_num == 0)
	{
		memcpy(&Ether.enet_srcaddr,Mac_Dev_1,6);
		memcpy(&Ether.enet_dstaddr,DstMac_Dev_1,6);
	}
	else if(dev_num == 1)
	{
		memcpy(&Ether.enet_srcaddr,Mac_Dev_2,6);
		memcpy(&Ether.enet_dstaddr,DstMac_Dev_2,6);
	}
	Ether.enet_type = htons(type);
	return mp_UnderLayer->Send((unsigned char*)&Ether,length+ETHER_HEADER_SIZE,dev_num);
}
BOOL CEthernetLayer::Receive( unsigned char* ppayload, unsigned short dev_num )
{
	PETHERNET pFrame = (PETHERNET) ppayload ;
	unsigned char broad[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
	unsigned char tmp[6] = {0x01,0x02,0x03,0x04,0x05,0x06};

	if((memcmp(pFrame->enet_srcaddr.addr_,Mac_Dev_1,6) && memcmp(pFrame->enet_srcaddr.addr_,Mac_Dev_2,6))
		&& (!memcmp(pFrame->enet_dstaddr.addr_,broad,6) || !memcmp(pFrame->enet_dstaddr.addr_,Mac_Dev_1,6) || !memcmp(pFrame->enet_dstaddr.addr_,Mac_Dev_2,6)))
	{
		if(pFrame->enet_type == ntohs(0x0800) || pFrame->enet_type == ntohs(0x0520) || pFrame->enet_type == htons(0x0806))
			return mp_aUpperLayer[0]->Receive((unsigned char*)pFrame->enet_data,pFrame->enet_type,dev_num);
	}
	return false;
}
void CEthernetLayer::SetMac_Dev_1(unsigned char *mac)
{
	memcpy(Mac_Dev_1,mac,6);
}
void CEthernetLayer::SetMac_Dev_2(unsigned char *mac)
{
	memcpy(Mac_Dev_2,mac,6);
}
unsigned char* CEthernetLayer::GetMac_Dev_1(void)
{
	return Mac_Dev_1;
}
unsigned char* CEthernetLayer::GetMac_Dev_2(void)
{
	return Mac_Dev_2;
}
void CEthernetLayer::SetDstMac_Dev_1(unsigned char *mac)
{
	memcpy(DstMac_Dev_1,mac,6);
}
void CEthernetLayer::SetDstMac_Dev_2(unsigned char *mac)
{
	memcpy(DstMac_Dev_2,mac,6);
}
unsigned char* CEthernetLayer::GetDstMac_Dev_1(void)
{
	return DstMac_Dev_1;
}
unsigned char* CEthernetLayer::GetDstMac_Dev_2(void)
{
	return DstMac_Dev_2;
}