// BaseLayer.h: interface for the CBaseLayer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#if !defined(AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_)
#define AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseLayer  
{
public:
	char*			GetLayerName( );

	CBaseLayer*		GetUnderLayer( );
	CBaseLayer*		GetUpperLayer( int nindex );
	void			SetUnderUpperLayer( CBaseLayer* pUULayer = NULL );
	void			SetUpperUnderLayer( CBaseLayer* pUULayer = NULL );
	void			SetUnderLayer( CBaseLayer* pUnderLayer = NULL ) ;
	void			SetUpperLayer( CBaseLayer* pUpperLayer = NULL ) ;

	CBaseLayer( char* pName = NULL );
	virtual ~CBaseLayer();

	// param : unsigned char*	- the data of the upperlayer
	//         int				- the length of data
	virtual	BOOL	Send( unsigned char*, int ) 	{ return FALSE ; }
	virtual BOOL	Send( unsigned char*, int, unsigned short ) 	{ return FALSE ; }
	virtual BOOL	Send( unsigned char*, int, unsigned short type, unsigned short ) 	{ return FALSE ; }
	// param : unsigned char*	- the data of the underlayer
	virtual	BOOL	Receive( unsigned char* ppayload ) 	{ return FALSE ; } 
	virtual BOOL	Receive(unsigned char* ppayload, unsigned short DeviceNum) { return FALSE;}
	virtual BOOL	Receive(unsigned char* ppayload, unsigned short type, unsigned short DeviceNum) { return FALSE;}
	virtual	BOOL	Receive( ) 	{ return FALSE ; }

protected:
	char*			m_pLayerName;
	CBaseLayer*		mp_UnderLayer;							// UnderLayer pointer
	CBaseLayer*		mp_aUpperLayer[ MAX_LAYER_NUMBER ];		// UpperLayer pointer
	int				m_nUpperLayerCount;						// UpperLayer Count
};

#endif // !defined(AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_)
