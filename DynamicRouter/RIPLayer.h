#pragma once
#include "baselayer.h"

typedef struct{
	unsigned short	family;
	unsigned short	no_use1;
	unsigned int	dst_ip;
	unsigned int	no_use2;
	unsigned int	no_use3;
	unsigned int	metric;
}RIP_BODY, *PRIP_BODY;

typedef struct{
	BYTE			cmd;
	BYTE			varsion;
	unsigned short	no_use;
	RIP_BODY		RIP_entry[MAX_ENTRY];
}RIP, *PRIP;

class CRIPLayer :
	public CBaseLayer
{
public:
	CRIPLayer(char* pName);
	~CRIPLayer(void);

	BOOL		Receive( unsigned char* ppayload, unsigned short dev_num) ;
	BOOL		Send(unsigned char* ppayload, int length, unsigned short dev_num);
	RIP			m_RIP;
};
