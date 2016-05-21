#pragma once
#include "baselayer.h"

class CEthernetLayer :
	public CBaseLayer
{
public:
	CEthernetLayer( char* pName );
	~CEthernetLayer(void);

	BOOL			Receive( unsigned char* ppayload, unsigned short dev_num) ;
	BOOL			Send(unsigned char* ppayload, int length, unsigned short type, unsigned short dev_num);
	void			SetMac_Dev_1(unsigned char *mac);
	void			SetMac_Dev_2(unsigned char *mac);
	unsigned char*	GetMac_Dev_1(void);
	unsigned char*	GetMac_Dev_2(void);
	void			SetDstMac_Dev_1(unsigned char *mac);
	void			SetDstMac_Dev_2(unsigned char *mac);
	unsigned char*	GetDstMac_Dev_1(void);
	unsigned char*	GetDstMac_Dev_2(void);

private :
	unsigned char Mac_Dev_1[6];
	unsigned char Mac_Dev_2[6];
	unsigned char DstMac_Dev_1[6];
	unsigned char DstMac_Dev_2[6];
};
