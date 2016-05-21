// LayerManager.cpp: implementation of the CLayerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LayerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayerManager::CLayerManager()
: m_nLayerCount( 0 ),
  mp_sListHead( NULL ),
  mp_sListTail( NULL ),
  m_nTop( -1 )
{

}

CLayerManager::~CLayerManager()
{

}

void CLayerManager::AddLayer(CBaseLayer *pLayer)
{
	mp_aLayers[ m_nLayerCount++ ] = pLayer ;
}

CBaseLayer* CLayerManager::GetLayer(int nindex)
{
	return mp_aLayers[ nindex ] ;
}

CBaseLayer* CLayerManager::GetLayer(char *pName)
{
	for ( int i = 0 ; i < m_nLayerCount ; i++ )
	{
		CString stName( pName ) ;
		if ( !stName.Compare( mp_aLayers[i]->GetLayerName( ) ) )
			return mp_aLayers[i] ;
	}

	return NULL ;
}

void CLayerManager::ConnectLayers(char *pcList)
{
	MakeList( pcList ) ;
	LinkLayer( mp_sListHead ) ;
}

void CLayerManager::MakeList(char *pcList)
{
	while( pcList != (char*) 0x01 )
	{
		char sBuff[ 100 ] ;
		sscanf( pcList, "%s", sBuff ) ;
		pcList = strchr( pcList, ' ' ) + 1 ;

		PNODE pNode = AllocNode( sBuff ) ;
		AddNode( pNode ) ;
	}
}

CLayerManager::PNODE CLayerManager::AllocNode(char *pcName)
{
	PNODE node = new NODE ;
	ASSERT( node ) ;

	strcpy( node->token, pcName ) ;
	node->next = NULL ;

	return node ;
}

void CLayerManager::AddNode(PNODE pNode)
{
	if ( !mp_sListHead )
	{
		mp_sListHead = mp_sListTail = pNode ;
	}
	else
	{
		mp_sListTail->next = pNode ;
		mp_sListTail = pNode ;
	}
}


void CLayerManager::Push(CBaseLayer *pLayer)
{
	if ( m_nTop >= MAX_LAYER_NUMBER )
	{
#ifdef _DEBUG
		TRACE( "The Stack is full.. so cannot run the push operation.. \n" ) ;
#endif
		return ;
	}

	mp_Stack[ ++m_nTop ] = pLayer ;
}

CBaseLayer* CLayerManager::Pop()
{
	if ( m_nTop < 0 )
	{
#ifdef _DEBUG
		TRACE( "The Stack is empty.. so cannot run the pop operation.. \n" ) ;
#endif
		return NULL ;
	}

	CBaseLayer* pLayer = mp_Stack[ m_nTop ] ;
	mp_Stack[ m_nTop ] = NULL ;
	m_nTop-- ;

	return pLayer ;
}

CBaseLayer* CLayerManager::Top()
{
	if ( m_nTop < 0 )
	{
#ifdef _DEBUG
		TRACE( "The Stack is empty.. so cannot run the top operation.. \n" ) ;
#endif
		return NULL ;
	}

	return mp_Stack[ m_nTop ] ;
}

void CLayerManager::LinkLayer(PNODE pNode)
{
	CBaseLayer* pLayer = NULL ;

	while ( pNode )
	{
		if ( !pLayer )
			pLayer = GetLayer( pNode->token ) ;
		else
		{
			if ( *pNode->token == '(' )
				Push( pLayer ) ;
			else if ( *pNode->token == ')' )
				Pop( ) ;
			else
			{
				char cMode = *pNode->token ;
				char* pcName = pNode->token + 1 ;
				
				pLayer = GetLayer( pcName ) ;

				switch( cMode )
				{
				case '*' : Top( )->SetUpperUnderLayer( pLayer ) ; break ;
				case '+' : Top( )->SetUpperLayer( pLayer ) ; break ;
				case '-' : Top( )->SetUnderLayer( pLayer ) ; break ;
				}
			}
		}

		pNode = pNode->next ;
	}
}

void CLayerManager::DeAllocLayer()
{
	for ( int i = 0 ; i < this->m_nLayerCount ; i ++ )
		delete this->mp_aLayers[ i ] ;
}
