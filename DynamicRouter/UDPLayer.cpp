#include "StdAfx.h"
#include "UDPLayer.h"

CUDPLayer::CUDPLayer(char *pName) : CBaseLayer(pName)
{
}
CUDPLayer::~CUDPLayer(void)
{
}
BOOL CUDPLayer::Send(unsigned char* ppayload, int length, unsigned short dev_num)
{
	PUDP pFrame = (PUDP)ppayload;
//	pFrame->checksum;
//	pFrame->dstPort;
//	pFrame->srcPort;
//	pFrame->len;
	mp_UnderLayer->Send((unsigned char*)pFrame,pFrame->len,dev_num);
	return 0;
}
BOOL CUDPLayer::Receive( unsigned char* ppayload, unsigned short dev_num)
{
	//checksum을 계산해서 틀리면 패킷을 버린다.
	PUDP pFrame = (PUDP)ppayload;
	mp_aUpperLayer[0]->Receive(pFrame->data,dev_num);
	return 0;
}