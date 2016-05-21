// RoutingEntryDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DynamicRouter.h"
#include "RoutingEntryDlg.h"

RoutingEntryDlg EntryDlg;
// RoutingEntryDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(RoutingEntryDlg, CDialog)

RoutingEntryDlg::RoutingEntryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RoutingEntryDlg::IDD, pParent)
	, m_metric(0)
{

}

RoutingEntryDlg::~RoutingEntryDlg()
{
}

void RoutingEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_dstip);
	DDX_Control(pDX, IDC_IPADDRESS2, m_netmask);
	DDX_Control(pDX, IDC_IPADDRESS3, m_gateway);
	DDX_Control(pDX, IDC_CHECK2, m_G);
	DDX_Control(pDX, IDC_CHECK3, m_H);
	DDX_Control(pDX, IDC_CHECK1, m_U);
	DDX_Control(pDX, IDC_COMBO1, m_interface);
	DDX_Text(pDX, IDC_EDIT1, m_metric);
}


BEGIN_MESSAGE_MAP(RoutingEntryDlg, CDialog)
	ON_BN_CLICKED(IDOK, &RoutingEntryDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL RoutingEntryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	pcap_if_t *alldevs;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_findalldevs(&alldevs,errbuf);
	for(;alldevs;alldevs = alldevs->next)
		m_interface.AddString(alldevs->name);
	m_netmask.SetWindowTextA("255.255.255.0");
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
// RoutingEntryDlg 메시지 처리기입니다.

void RoutingEntryDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	unsigned char dstip[4];
	unsigned char Netmask[4];
	unsigned char Gateway[4];
	unsigned char dev_name[100];
	int flag = 0;
	if(m_U.GetCheck())
		flag += 1;
	if(m_G.GetCheck())
		flag += 2;
	if(m_H.GetCheck())
		flag += 4;
	m_dstip.GetAddress(dstip[0],dstip[1],dstip[2],dstip[3]);
	m_netmask.GetAddress(Netmask[0],Netmask[1],Netmask[2],Netmask[3]);
	m_gateway.GetAddress(Gateway[0],Gateway[1],Gateway[2],Gateway[3]);
	m_interface.GetLBText(m_interface.GetCurSel(),(LPTSTR)(LPCTSTR)dev_name);

	RouterDlg->addrouting_entry(dstip,Netmask,Gateway,flag,dev_name,m_metric);
	OnOK();
}
void RoutingEntryDlg::setDlg(CDynamicRouterDlg* dlg)
{
	RouterDlg = dlg;
}