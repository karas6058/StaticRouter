#pragma once
#include "baselayer.h"

typedef struct {
	unsigned short srcPort;
	unsigned short dstPort;
	unsigned short len;
	unsigned short checksum;
	unsigned char  data[MAX_UDP_DATA];
}UDP, *PUDP;

class CUDPLayer :
	public CBaseLayer
{
public:
	CUDPLayer(char* pName);
	~CUDPLayer(void);

	BOOL		Receive( unsigned char* ppayload, unsigned short dev_num) ;
	BOOL		Send(unsigned char* ppayload, int length, unsigned short type);

private :
	UDP m_UDP;
};
