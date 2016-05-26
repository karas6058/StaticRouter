// ProxyAddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ARP.h"
#include "ProxyAddDlg.h"
#include "afxmsg_.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProxyAddDlg dialog

CProxyAddDlg::CProxyAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProxyAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProxyAddDlg)
	m_etherAddr = _T("");
	//}}AFX_DATA_INI
}

void CProxyAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxyAddDlg)
	DDX_Control(pDX, IDC_IPADDRESS_IP_ADDR, m_IPAddr);
	DDX_Control(pDX, IDC_COMBO_DEVICE, cb_device);
	DDX_Text(pDX, IDC_EDIT_ETHER_ADDR, m_etherAddr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProxyAddDlg, CDialog)
	//{{AFX_MSG_MAP(CProxyAddDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, OnSelchangeComboDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxyAddDlg message handlers

void CProxyAddDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	
	bool isRight = false;
	
	if(m_etherAddr.GetLength() == 17) 
	{
		int i = 0;
		for(i; i < 17; i++) 
		{
			char c = m_etherAddr.GetAt(i);
			if( (i + 1) % 3 == 0 ) 
			{
				if( c != '-' )
					break;
			}
			else if( !( ( c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9') ) )
				break;
		}
		if(i == 17)
			isRight = true;
	}
	if(m_etherAddr.IsEmpty() || m_IPAddr.IsBlank()) 
	{
		AfxMessageBox("입력되지 않은 항목이 있습니다.");
		return;
	}	
	else if(isRight == false) 
	{
		AfxMessageBox("주소형식이 올바르지 않습니다.( FORMAT : xx-xx-xx-xx-xx-xx )");
		return;
	}
	cb_device.GetLBText( cb_device.GetCurSel(), m_buffer.device );
	
	m_IPAddr.GetAddress(m_buffer.buffer_IPAddr[0], m_buffer.buffer_IPAddr[1], 
		m_buffer.buffer_IPAddr[2], m_buffer.buffer_IPAddr[3]);
	
	for(int i = 0; i < 10; i++) 
	{
		CString temp;
		unsigned char char_temp[10];
		
		AfxExtractSubString(temp, m_etherAddr, i, '-');
		sscanf(temp,"%x", &char_temp[i]);
		m_buffer.buffer_EtherAddr[i] = char_temp[i];
	}

	CDialog::OnOK();
}

Proxy_BufferAddr CProxyAddDlg::GetBufferAddr() 
{
	return m_buffer;
}

void CProxyAddDlg::GetSystemEtherInfo()
{	
	int CardNo = 0;	
	IP_ADAPTER_INFO AdapterInfo[MAX_DEVICE];
	
	// 장비 목록을 읽음
	DWORD dwBufLen = sizeof( AdapterInfo );	// Save memory size of buffer
	DWORD dwStatus = GetAdaptersInfo(		// Call GetAdapterInfo
		AdapterInfo,						// [out] buffer to receive data
		&dwBufLen );						// [in] size of receive data buffer
	
	if ( dwStatus != ERROR_SUCCESS )
	{	
		AfxMessageBox( "Error!" );
	}
	
	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	
	// 얻은 장비 목록을 정리함 
	do 
	{
		SystemEtherInfo[CardNo].InstallCardNo = CardNo;
		memcpy( SystemEtherInfo[CardNo].CardName, "rpcap://\\Device\\NPF_", 22 );
		strcat( ( char* )SystemEtherInfo[CardNo].CardName, ( char* )pAdapterInfo->AdapterName );
		memcpy( SystemEtherInfo[CardNo].CardDesc, pAdapterInfo->Description, MAX_ADAPTER_DESCRIPTION_LENGTH + 4 );
		memcpy( SystemEtherInfo[CardNo].MacAddr, ( unsigned char* )pAdapterInfo->Address, 6 );
		memcpy( SystemEtherInfo[CardNo].IPAddr, pAdapterInfo->IpAddressList.IpAddress.String, 16 );
		
		pAdapterInfo = pAdapterInfo->Next;
		CardNo++;
	}
	while( pAdapterInfo);
	
	if ( !CardNo ) 
	{
		AfxMessageBox( "No Network Device." );
		return;
	}
	
	SystemEtherInfo[CardNo].InstallCardNo = -1;  // 장비 리스트의 끝 표시
}

void CProxyAddDlg::OnSelchangeComboDevice() 
{
	// TODO: Add your control notification handler code here
	unsigned char *addr = SystemEtherInfo[cb_device.GetCurSel( )].MacAddr;	// 선택된 장비의 MAC주소를 가져와서 addr에 저장
	
	m_etherAddr.Format( "%02x-%02x-%02x-%02x-%02x-%02x", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5] );
	
	BYTE ip_Addr[4];
	char c_temp[4];
	
	for( int i = 0; i < 4; i++ ) 
	{
		memcpy( c_temp, SystemEtherInfo[cb_device.GetCurSel( )].IPAddr + i*4, 3 );	// 현재 IP주소를 가져와서 임시로 저장한 뒤
		c_temp[3] = '\0';
		ip_Addr[i] = atoi( c_temp );
	}
	
	m_IPAddr.SetAddress( ip_Addr[0], ip_Addr[1], ip_Addr[2], ip_Addr[3] ); // Source IP주소를 다이얼로그에 세팅함
	
	UpdateData( false );
}

BOOL CProxyAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//	네트워크 장치 정보 획득 및 출력
	GetSystemEtherInfo();	// 네트워크 장치 정보 획득
	
	int CardNo = 0;
	
	while( SystemEtherInfo[CardNo].InstallCardNo != -1 ) 
	{
		cb_device.AddString( ( LPCTSTR )SystemEtherInfo[CardNo].CardDesc );
		CardNo++;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
} 