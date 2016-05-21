// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#define MAX_LAYER_NUMBER		10
#define Max_Num_Adapter			10
#define ETHER_MAX_SIZE			1514
#define ETHER_HEADER_SIZE		14
#define ETHER_MAX_DATA_SIZE		( ETHER_MAX_SIZE - ETHER_HEADER_SIZE )
#define IP_HEADER_SIZE			20
#define MAX_IP_DATA				1480
#define MAX_IP_SIZE				1500
#define UDP_HEADER_SIZE			8
#define MAX_UDP_DATA			1472
#define MAX_ENTRY				16
#define MAX_ARP_SIZE			28

typedef struct ip_addr {
        union {
                struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
				unsigned char S_ip_addr[4];
        } S_un;
} IP_ADDR;

typedef struct _ETHERNET_ADDR{
	union{
		struct {unsigned char e0,e1,e2,e3,e4,e5;} s_un_byte;
		unsigned char s_ether_addr[6];
	}S_un;
#define addr0 S_un.s_un_byte.e0
#define addr1 S_un.s_un_byte.e1
#define addr2 S_un.s_un_byte.e2
#define addr3 S_un.s_un_byte.e3
#define addr4 S_un.s_un_byte.e4
#define addr5 S_un.s_un_byte.e5
#define addr_ S_un.s_ether_addr
}ETHERNET_ADDR, *LPETHERNET_ADDR;

typedef struct _ETHERNET{

	ETHERNET_ADDR	enet_dstaddr;		// destination address of ethernet layer
	ETHERNET_ADDR	enet_srcaddr;		// source address of ethernet layer
	unsigned short	enet_type	;		// type of ethernet layer
	unsigned char	enet_data[ ETHER_MAX_DATA_SIZE ] ; // frame data

} ETHERNET, *PETHERNET;

typedef struct _cache_Entry
{
	IP_ADDR			cache_ipaddr;
	ETHERNET_ADDR	cache_enetaddr;
	unsigned char	cache_type;
	unsigned short	cache_ttl;
}cache_entry, *Pcache_entry;

typedef struct _routing_table{
	IP_ADDR DesIp;
	IP_ADDR Netmask;
	IP_ADDR Gateway;
	int		flag;
	unsigned char	div_name[100];
	int		metric;
	int		routing_ttl;
}Routing_table, *PRouting_table;

typedef struct _ARP
{
	unsigned short arp_dhtype;
	unsigned short apr_prototype;
	unsigned char arp_hdlength;
	unsigned char arp_protolength;
	unsigned short arp_op;

	ETHERNET_ADDR arp_srchdaddr;
	IP_ADDR arp_srcprotoaddr;
	ETHERNET_ADDR arp_desthdaddr;
	IP_ADDR arp_destprotoaddr;
}ARP, *PARP;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


