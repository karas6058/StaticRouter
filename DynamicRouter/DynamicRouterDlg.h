// DynamicRouterDlg.h : 헤더 파일
//

#pragma once
#include "LayerManager.h"
#include "pcap.h"
#include "afxwin.h"
#include "afxcmn.h"

enum : BYTE {
	eDev_1,
	eDev_2
};

// CDynamicRouterDlg 대화 상자
class CDynamicRouterDlg : public CDialog, public CBaseLayer
{
// 생성입니다.
public:
	CDynamicRouterDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DYNAMICROUTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_NIC1;
	CComboBox m_NIC2;
	char* getDevMac(char *dev_name);
	pcap_t* getDev_1();
	pcap_t* getDev_2();
	afx_msg void OnPacketRecv();
	void OnTimer(UINT nIDEvent);

public :
	static void Receive_1_Thread(void* param);
	static void Receive_2_Thread(void* param);
	static void TimerThread(void* param);
	void		setcache_entry(cache_entry* _cache_entry);
	void		addrouting_entry(unsigned char* dstip, unsigned char* Netmask, unsigned char* Gateway, int flag, unsigned char* dev_name,int m_metric);
private :
	CLayerManager		m_LayerMgr;
	pcap_if_t*			alldevs;
	pcap_t*				Dev_1;
	pcap_t*				Dev_2;
	cache_entry*		pcache_entry;
	int*				cache_top;
	Routing_table		routing_entry[100];
	int					routing_top;
	HANDLE				hRecv_1;
	HANDLE				hRecv_2;
	HANDLE				hTimer;
	int					exitFlag;
public:
	CListCtrl routing_table;
	CListCtrl m_CacheTable;
	afx_msg void OnAddRoutingEntry();
	afx_msg void OnCleanCacheTable();
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnLbnSelchangeListProxyTable();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnLbnSelchangeList4();
};
