#pragma once
#include "afxcmn.h"


// CNewClientDlg 对话框

class CNewClientDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CNewClientDlg)

public:
	CNewClientDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewClientDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
 	CIPAddressCtrl m_ctlDestIp;
 	UINT m_nDestPort;
	UINT m_nLocalPort;

	DWORD m_dwDestIp;
protected:
	virtual void OnOK();
public: 
	int m_nProtocolType;
	BOOL m_bRandomPort;
	afx_msg void OnBnClickedRadioLocalPort1();
	afx_msg void OnBnClickedRadioLocalPort2();
};
