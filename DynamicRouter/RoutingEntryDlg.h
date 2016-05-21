#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DynamicRouterDlg.h"

// RoutingEntryDlg 대화 상자입니다.
class RoutingEntryDlg : public CDialog
{
	DECLARE_DYNAMIC(RoutingEntryDlg)

public:
	RoutingEntryDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~RoutingEntryDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ROUTINGENTRYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_dstip;
	CIPAddressCtrl m_netmask;
	CIPAddressCtrl m_gateway;
	CButton m_G;
	CButton m_H;
	CButton m_U;
	CComboBox m_interface;
	afx_msg void OnBnClickedOk();

private:
	CDynamicRouterDlg*	RouterDlg;
public:
	void	setDlg(CDynamicRouterDlg* dlg);
	int		m_metric;
	virtual BOOL OnInitDialog();
};
