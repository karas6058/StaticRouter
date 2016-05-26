// DynamicRouterDlg.cpp : 구현 파일
//

#include <time.h>
#include "stdafx.h"
#include "pcap.h"
#include "DynamicRouter.h"
#include "DynamicRouterDlg.h"
#include "RoutingEntryDlg.h"
#include "process.h"
#include "packet32.h"
#include "NILayer.h"
#include "IPLayer.h"
#include "EthernetLayer.h"
#include "UDPLayer.h"
#include "RIPLayer.h"
#include "ARPLayer.h"

#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "packet.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
extern RoutingEntryDlg EntryDlg;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDynamicRouterDlg 대화 상자




CDynamicRouterDlg::CDynamicRouterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDynamicRouterDlg::IDD, pParent),CBaseLayer( "DynamicRoutingDlg" )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_LayerMgr.AddLayer( new CRIPLayer( "RIP" ) ) ;
	m_LayerMgr.AddLayer( new CUDPLayer( "UDP" ) ) ;
	m_LayerMgr.AddLayer( new CARPLayer( "ARP" ) ) ;
	m_LayerMgr.AddLayer( new CIPLayer( "IP" ) ) ;
	m_LayerMgr.AddLayer( new CEthernetLayer( "Ethernet" ) ) ;
	m_LayerMgr.AddLayer( new CNILayer( "NI" ) ) ;
	m_LayerMgr.AddLayer( this ) ;
	//      레이어를 연결한다.
	//		TCP레이어 위에는 file,chat레이어가 있고 그 위로 Dlg레이어로 이어진다.
	// + : 왼쪽레이어의 UpperLayer. - : 왼쪽레이어의 UnderLayer.
	m_LayerMgr.ConnectLayers("NI ( *Ethernet ( +IP ( -ARP ( -Ethernet ) *UDP ( *RIP ( *DynamicRoutingDlg ) ) ) ) )");
	CARPLayer* ARP = (CARPLayer*)mp_UnderLayer->GetUnderLayer()->GetUnderLayer()->GetUnderLayer();
	CIPLayer* IP = (CIPLayer*)mp_UnderLayer->GetUnderLayer()->GetUnderLayer();
	IP->setRouting_entry(routing_entry);
	IP->setRouting_top(&routing_top);
	setcache_entry(ARP->getcache_entry());
	cache_top = ARP->getCache_top();
}

void CDynamicRouterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_NIC1);
	DDX_Control(pDX, IDC_COMBO2, m_NIC2);
	DDX_Control(pDX, IDC_LIST1, routing_table);
	DDX_Control(pDX, IDC_LIST2, m_CacheTable);
}

BEGIN_MESSAGE_MAP(CDynamicRouterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	////////////////////////////////////////////////////////////////
	ON_WM_TIMER()				//이게 있어야 timer가 작동한다 !!!
	////////////////////////////////////////////////////////////////
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDynamicRouterDlg::OnPacketRecv)
	ON_BN_CLICKED(IDC_BUTTON2, &CDynamicRouterDlg::OnAddRoutingEntry)
	ON_BN_CLICKED(IDC_BUTTON5, &CDynamicRouterDlg::OnCleanCacheTable)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CDynamicRouterDlg::OnLvnItemchangedList3)
	ON_BN_CLICKED(IDC_BUTTON6, &CDynamicRouterDlg::OnBnClickedButton6)
	ON_LBN_SELCHANGE(IDC_LIST4, &CDynamicRouterDlg::OnLbnSelchangeList4)
	ON_BN_CLICKED(IDC_BUTTON7, &CDynamicRouterDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDynamicRouterDlg::OnBnClickedButton8)
	ON_LBN_SELCHANGE(IDC_LIST4, &CDynamicRouterDlg::OnLbnSelchangeList4)
END_MESSAGE_MAP()


// CDynamicRouterDlg 메시지 처리기

BOOL CDynamicRouterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	EntryDlg.setDlg(this);
	routing_top = 0;
	exitFlag = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t* devs;
	pcap_findalldevs(&alldevs, errbuf);
	for(devs = alldevs; devs; devs = devs->next)
	{
		m_NIC1.AddString(devs->name);
		m_NIC2.AddString(devs->name);
	}
	LV_COLUMN lvColumn;
	char *Routing_table[6] = {"Destination", "NetMask" ,"Gateway","Flag","Device","Metric"};
	int nWidth_routing[6] = {100,100,120,55,55,55};
	for(int i=0; i<6; i++)
	{
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_CENTER; //중앙정렬
		lvColumn.pszText = Routing_table[i];
		lvColumn.iSubItem = i;
		lvColumn.cx = nWidth_routing[i];
		routing_table.InsertColumn(i, &lvColumn);
	}
	char *list[3] = {"IP", "MAC address" ,"State"};
	int nWidth[3] = {110,120,80};
	for(int i=0; i<3; i++)
	{
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_CENTER; //중앙정렬
		lvColumn.pszText = list[i];
		lvColumn.iSubItem = i;
		lvColumn.cx = nWidth[i];
		m_CacheTable.InsertColumn(i, &lvColumn);
	}
	routing_table.SetExtendedStyle(routing_table.GetExtendedStyle() | LVS_EX_GRIDLINES);
	hTimer = (HANDLE)_beginthread(TimerThread,0,this);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDynamicRouterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDynamicRouterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDynamicRouterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CDynamicRouterDlg::OnPacketRecv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	hRecv_1 = (HANDLE)_beginthread(Receive_1_Thread,0,this);
	hRecv_2 = (HANDLE)_beginthread(Receive_2_Thread,0,this);
}

void CDynamicRouterDlg::Receive_1_Thread(void* param)
{
	struct pcap_pkthdr info;
	const unsigned char* Data;
	CDynamicRouterDlg* dlg = (CDynamicRouterDlg*)param;
	CIPLayer *m_ip = (CIPLayer*)dlg->mp_UnderLayer->GetUnderLayer()->GetUnderLayer();
	char errbuf[PCAP_ERRBUF_SIZE];
	char dev_name[100];

	dlg->m_NIC1.GetLBText(dlg->m_NIC1.GetCurSel(),(LPTSTR)(LPCTSTR)dev_name);
	m_ip->setDev_1_name(dev_name);
	unsigned char *mac = (unsigned char*)dlg->getDevMac(dev_name);

	CIPLayer* IP = (CIPLayer*)dlg->mp_UnderLayer->GetUnderLayer()->GetUnderLayer();
	pcap_if_t* devs;
	for(devs = dlg->alldevs; devs; devs = devs->next)
	{
		if(!memcmp(devs->name,dev_name,strlen(dev_name)))
		{
			IP->setDev_1_IP((unsigned char*)(devs->addresses->addr->sa_data+2));
			break;
		}
	}
	CEthernetLayer* Ether = (CEthernetLayer*)dlg->mp_UnderLayer->GetUnderLayer()->GetUnderLayer()->GetUnderLayer()->GetUnderLayer();
	Ether->SetMac_Dev_1(mac);
	dlg->Dev_1 = pcap_open_live(dev_name,1500,1,1,errbuf);
	pcap_t* Dev_1 = dlg->getDev_1();

	CNILayer* NI = (CNILayer*)dlg->mp_UnderLayer->GetUnderLayer()->GetUnderLayer()->GetUnderLayer()->GetUnderLayer()->GetUnderLayer();
	NI->setDev_1(Dev_1);
	while(true)
	{
		//해당 패킷이 있을때까지 loop
		do
		{
			Data = pcap_next(Dev_1,&info);
		}while(!Data);
		// 해당 패킷이 있다면 수신하기 위해서 NILayer부터 Receive하기 시작한다.
		NI->Receive((unsigned char*)(LPTSTR)(LPCTSTR)Data,eDev_1);
	}
}
void CDynamicRouterDlg::Receive_2_Thread(void* param)
{
	struct pcap_pkthdr info;
	const unsigned char* Data;
	CDynamicRouterDlg* dlg = (CDynamicRouterDlg*)param;
	char errbuf[PCAP_ERRBUF_SIZE];
	char dev_name[100];
	CIPLayer *m_ip = (CIPLayer*)dlg->mp_UnderLayer->GetUnderLayer()->GetUnderLayer();

	dlg->m_NIC2.GetLBText(dlg->m_NIC2.GetCurSel(),(LPTSTR)(LPCTSTR)dev_name);
	m_ip->setDev_2_name(dev_name);
	unsigned char *mac = (unsigned char*)dlg->getDevMac(dev_name);

	CIPLayer* IP = (CIPLayer*)dlg->mp_UnderLayer->GetUnderLayer()->GetUnderLayer();
	pcap_if_t* devs;
	for(devs = dlg->alldevs; devs; devs = devs->next)
	{
		if(!memcmp(devs->name,dev_name,strlen(dev_name)))
		{
			IP->setDev_2_IP((unsigned char*)(devs->addresses->addr->sa_data+2));
			break;
		}
	}
	CEthernetLayer* Ether = (CEthernetLayer*)dlg->mp_UnderLayer->GetUnderLayer()->GetUnderLayer()->GetUnderLayer()->GetUnderLayer();
	Ether->SetMac_Dev_2(mac);

	dlg->Dev_2 = pcap_open_live(dev_name,1500,1,1,errbuf);
	pcap_t* Dev_2 = dlg->getDev_2();
	CNILayer* NI = (CNILayer*)dlg->mp_UnderLayer->GetUnderLayer()->GetUnderLayer()->GetUnderLayer()->GetUnderLayer()->GetUnderLayer();
	NI->setDev_2(Dev_2);
	while(true)
	{
		//해당 패킷이 있을때까지 loop
		do
		{
			Data = pcap_next(Dev_2,&info);
		}while(!Data);
		// 해당 패킷이 있다면 수신하기 위해서 NILayer부터 Receive하기 시작한다.
		NI->Receive((unsigned char*)(LPTSTR)(LPCTSTR)Data,eDev_2);		//인자를 하나 더 넣어서 어느 device로 오는지 명시
	}
}
void CDynamicRouterDlg::TimerThread(void* param)
{
	CDynamicRouterDlg* dlg = (CDynamicRouterDlg*)param;
	dlg->SetTimer(0,1000,NULL);
}
void CDynamicRouterDlg::OnTimer(UINT nIDEvent)
{
	if(exitFlag == 1)
		return;
	m_CacheTable.DeleteAllItems();
	char formatIP[16];
	char formatMAC[18];
	char buf[11];
	for(int i = 0; i<*cache_top; i++)
	{
		if(pcache_entry[i].cache_ttl != 0)
		{
			sprintf_s(formatIP,16,"%d.%d.%d.%d",pcache_entry[i].cache_ipaddr.S_un.S_un_b.s_b1,pcache_entry[i].cache_ipaddr.S_un.S_un_b.s_b2,pcache_entry[i].cache_ipaddr.S_un.S_un_b.s_b3,pcache_entry[i].cache_ipaddr.S_un.S_un_b.s_b4);
			sprintf_s(formatMAC,18,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",pcache_entry[i].cache_enetaddr.addr0,pcache_entry[i].cache_enetaddr.addr1,pcache_entry[i].cache_enetaddr.addr2,pcache_entry[i].cache_enetaddr.addr3,pcache_entry[i].cache_enetaddr.addr4,pcache_entry[i].cache_enetaddr.addr5);
			int nItemNum = m_CacheTable.GetItemCount();
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT; /* 첫 번째 */
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 0;
			lvItem.pszText = (LPTSTR)(LPCSTR)formatIP;
			m_CacheTable.InsertItem(&lvItem);

			lvItem.mask = LVIF_TEXT; /* 두 번째 */
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 1;
			lvItem.pszText = (LPTSTR)(LPCSTR)formatMAC;
			m_CacheTable.SetItem(&lvItem);

			_itoa_s(pcache_entry[i].cache_type,buf,2,10);
			if(!memcmp(buf,"0",2))
				memcpy(buf,"Incomplete",11);
			else
				memcpy(buf,"Complete",9);
			lvItem.mask = LVIF_TEXT; /* 세 번째 */
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 2;
			lvItem.pszText = (LPTSTR)(LPCSTR)buf;
			m_CacheTable.SetItem(&lvItem);
		}
	}
	routing_table.DeleteAllItems();
	char formatDstIP[16];
	char formatNetmask[16];
	char formatGateway[16];
	char parseflag[4];
	char formatMetric[2];
	for(int i = 0; i<routing_top; i++)
	{
		if(routing_entry[i].routing_ttl != 0)
		{
			sprintf_s(formatDstIP,16,"%d.%d.%d.%d",routing_entry[i].DesIp.S_un.S_un_b.s_b1,routing_entry[i].DesIp.S_un.S_un_b.s_b2,routing_entry[i].DesIp.S_un.S_un_b.s_b3,routing_entry[i].DesIp.S_un.S_un_b.s_b4);
			sprintf_s(formatNetmask,16,"%d.%d.%d.%d",routing_entry[i].Netmask.S_un.S_un_b.s_b1,routing_entry[i].Netmask.S_un.S_un_b.s_b2,routing_entry[i].Netmask.S_un.S_un_b.s_b3,routing_entry[i].Netmask.S_un.S_un_b.s_b4);
			sprintf_s(formatGateway,16,"%d.%d.%d.%d",routing_entry[i].Gateway.S_un.S_un_b.s_b1,routing_entry[i].Gateway.S_un.S_un_b.s_b2,routing_entry[i].Gateway.S_un.S_un_b.s_b3,routing_entry[i].Gateway.S_un.S_un_b.s_b4);
			sprintf_s(formatMetric,2,"%d",routing_entry[i].metric);
			if(routing_entry[i].flag == 1)
				memcpy(parseflag,"U",2);
			else if(routing_entry[i].flag == 2)
				memcpy(parseflag,"G",2);
			else if(routing_entry[i].flag == 3)
				memcpy(parseflag,"UG",3);
			else if(routing_entry[i].flag == 4)
				memcpy(parseflag,"H",2);
			else if(routing_entry[i].flag == 5)
				memcpy(parseflag,"UH",3);
			else if(routing_entry[i].flag == 6)
				memcpy(parseflag,"GH",3);
			else if(routing_entry[i].flag == 7)
				memcpy(parseflag,"UGH",4);
			

			int nItemNum = routing_table.GetItemCount();
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 0;
			lvItem.pszText = (LPTSTR)(LPCSTR)formatDstIP;
			routing_table.InsertItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 1;
			lvItem.pszText = (LPTSTR)(LPCSTR)formatNetmask;
			routing_table.SetItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 2;
			lvItem.pszText = (LPTSTR)(LPCSTR)formatGateway;
			routing_table.SetItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 3;
			lvItem.pszText = (LPTSTR)(LPCSTR)parseflag;
			routing_table.SetItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 4;
			lvItem.pszText = (LPTSTR)(LPCSTR)routing_entry[i].div_name;
			routing_table.SetItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItemNum;
			lvItem.iSubItem = 5;
			lvItem.pszText = (LPTSTR)(LPCSTR)formatMetric;
			routing_table.SetItem(&lvItem);
		}
	}
	memcpy(formatDstIP,"0.0.0.0",8);
	memcpy(formatNetmask,"0.0.0.0",8);
	memcpy(formatGateway,"0.0.0.0",8);
	memcpy(parseflag,"-",2);
	memcpy(formatMetric,"-",2);
	int nItemNum = routing_table.GetItemCount();
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItemNum;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPTSTR)(LPCSTR)formatDstIP;
	routing_table.InsertItem(&lvItem);

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItemNum;
	lvItem.iSubItem = 1;
	lvItem.pszText = (LPTSTR)(LPCSTR)formatNetmask;
	routing_table.SetItem(&lvItem);

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItemNum;
	lvItem.iSubItem = 2;
	lvItem.pszText = (LPTSTR)(LPCSTR)formatGateway;
	routing_table.SetItem(&lvItem);

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItemNum;
	lvItem.iSubItem = 3;
	lvItem.pszText = (LPTSTR)(LPCSTR)parseflag;
	routing_table.SetItem(&lvItem);

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItemNum;
	lvItem.iSubItem = 4;
	lvItem.pszText = (LPTSTR)(LPCSTR)"-";
	routing_table.SetItem(&lvItem);

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItemNum;
	lvItem.iSubItem = 5;
	lvItem.pszText = (LPTSTR)(LPCSTR)formatMetric;
	routing_table.SetItem(&lvItem);
}
pcap_t* CDynamicRouterDlg::getDev_1()
{
	return Dev_1;
}
pcap_t* CDynamicRouterDlg::getDev_2()
{
	return Dev_2;
}
char AdapterList[Max_Num_Adapter][1024];
char* CDynamicRouterDlg::getDevMac(char *dev_name)
{
	LPADAPTER	lpAdapter = 0;
	int			i;
	char		AdapterName[8192];
	char		*temp,*temp1;
	int			AdapterNum=0;
	ULONG		AdapterLength;
	PPACKET_OID_DATA  OidData;
	BOOLEAN		Status;
	char *src;

	i=0;	

	AdapterLength = sizeof(AdapterName);

	PacketGetAdapterNames(AdapterName,&AdapterLength);
	temp=AdapterName;
	temp1=AdapterName;

	while ((*temp!='\0')||(*(temp-1)!='\0'))
	{
		if (*temp=='\0') 
		{
			memcpy(AdapterList[i],temp1,temp-temp1);
			if(!strcmp(AdapterList[i],dev_name))
				break;				
			temp1=temp+1;
			i++;
		}
		temp++;
	}

	AdapterNum=i;

	lpAdapter =   PacketOpenAdapter(AdapterList[i]);
	OidData = (PPACKET_OID_DATA)malloc(0x12);

	OidData->Oid = 0x01010102;

	OidData->Length = 6;
	ZeroMemory(OidData->Data, 6);

	Status = PacketRequest(lpAdapter, FALSE, OidData);

	PacketCloseAdapter(lpAdapter);
	OidData->Data[6] = '\0';
	src = (char*)OidData->Data;
	return src;
}
void CDynamicRouterDlg::setcache_entry(cache_entry* _cache_entry)
{
	pcache_entry = _cache_entry;
}
void CDynamicRouterDlg::OnAddRoutingEntry()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(EntryDlg.DoModal() == IDOK)
		return;
}
void CDynamicRouterDlg::addrouting_entry(unsigned char* dstip, unsigned char* Netmask, unsigned char* Gateway, int flag, unsigned char* dev_name,int m_metric)
{
	for(int i = 0; i<routing_top; i++)
	{
		if(!memcmp(routing_entry[i].DesIp.S_un.S_ip_addr,dstip,4))
		{
			AfxMessageBox("이미 존재하는 entry입니다");
			return;
		}
	}
	for(int i = 0; i<routing_top; i++)
	{
		if(routing_entry[i].routing_ttl == 0)
		{
			memcpy(routing_entry[i].DesIp.S_un.S_ip_addr,dstip,4);
			memcpy(routing_entry[i].Netmask.S_un.S_ip_addr,Netmask,4);
			memcpy(routing_entry[i].Gateway.S_un.S_ip_addr,Gateway,4);
			routing_entry[i].flag = flag;
			memcpy(routing_entry[i].div_name,dev_name,strlen((char*)dev_name));
			routing_entry[i].metric = m_metric;
			routing_entry[i].routing_ttl = 300;
			return;
		}
	}
	memcpy(routing_entry[routing_top].DesIp.S_un.S_ip_addr,dstip,4);
	memcpy(routing_entry[routing_top].Netmask.S_un.S_ip_addr,Netmask,4);
	memcpy(routing_entry[routing_top].Gateway.S_un.S_ip_addr,Gateway,4);
	routing_entry[routing_top].flag = flag;
	memcpy(routing_entry[routing_top].div_name,dev_name,strlen((char*)dev_name)+1);
	routing_entry[routing_top].metric = m_metric;
	routing_entry[routing_top].routing_ttl = 300;
	routing_top++;
}
void CDynamicRouterDlg::OnCleanCacheTable()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_CacheTable.DeleteAllItems();
	for(int i = 0; i<*cache_top; i++)
		pcache_entry[i].cache_ttl = 0;
	*cache_top = 0;
}

void CDynamicRouterDlg::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

// 프록시 테이블 창의 ADD 버튼 누를시
void CDynamicRouterDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CProxyAddDlg dlg;

	// dlg에서 가져온 content를 통해 주소목록 추가
	if(dlg.DoModal() == IDOK) 
	{
		LPPROXY_LIST proxy_element = (LPPROXY_LIST)malloc(sizeof(PROXY_LIST));
		
		Proxy_BufferAddr temp_BufferAddr = dlg.GetBufferAddr();
		
		proxy_element->proxy_entry.proxy_device = new CString(temp_BufferAddr.device);
		memcpy(proxy_element->proxy_entry.proxy_enetaddr, temp_BufferAddr.buffer_EtherAddr, 6);
		memcpy(proxy_element->proxy_entry.proxy_ipaddr, temp_BufferAddr.buffer_IPAddr, 4);
		proxy_element->next = NULL;
		
		m_ARPLayer->AddProxy(proxy_element);
		
		Invalidate();
	}
}

//비어있다.
void CDynamicRouterDlg::OnLbnSelchangeListProxyTable()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 프록시 테이블 창의 DELETE 버튼 누를시
void CDynamicRouterDlg::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_ProxyList.GetCurSel() == LB_ERR)
		return;

	CString temp_IPAddr;
	unsigned char IPAddr[10];
	m_ProxyList.GetText(m_ProxyList.GetCurSel(), temp_IPAddr);
	
	int offset = temp_IPAddr.Find("IP:");
	temp_IPAddr = temp_IPAddr.Right(temp_IPAddr.GetLength() - offset -4);
	offset = temp_IPAddr.Find("[MAC:");
	temp_IPAddr = temp_IPAddr.Left(offset);
	offset = temp_IPAddr.Find("]");
	temp_IPAddr = temp_IPAddr.Left(offset);
	
	for(int i = 0; i < 10; i++) 
	{
		CString temp;
		unsigned char char_temp[10];
		
		AfxExtractSubString(temp, temp_IPAddr, i, '.');
		sscanf(temp,"%d", &char_temp[i]);
		IPAddr[i] = char_temp[i];
	}
	
	m_ARPLayer->DeleteProxy(IPAddr);
	
	Invalidate();
}

// 프록시 테이블 창의 DELETE ALL 버튼 누를시
void CDynamicRouterDlg::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ARPLayer->AllDeleteProxy();// 모든 element 제거
	Invalidate();
}


void CDynamicRouterDlg::OnLbnSelchangeList4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
