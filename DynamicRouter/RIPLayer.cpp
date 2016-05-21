#include "StdAfx.h"
#include "RIPLayer.h"

CRIPLayer::CRIPLayer(char *pName) : CBaseLayer(pName)
{
}
CRIPLayer::~CRIPLayer(void)
{
}
BOOL CRIPLayer::Send(unsigned char* ppayload, int length, unsigned short dev_num)
{
	//SetTimer()를 동작시켜 시간이 지나면 table을 보낸다.
	return 0;
}
BOOL CRIPLayer::Receive( unsigned char* ppayload, unsigned short dev_num)
{
	PRIP pFrame = (PRIP)ppayload;
	/////////////////////////////////////
	//DVRP alorithm 에 의해 테이블 갱신//
	/////////////////////////////////////
	return 0;
}