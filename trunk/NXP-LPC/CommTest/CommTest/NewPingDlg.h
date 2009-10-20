#pragma once
#include "afxcmn.h"


// CNewPingDlg 对话框

class CNewPingDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CNewPingDlg)

public:
	CNewPingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewPingDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_PING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIPAddressCtrl m_ctlDestIp;
	UINT m_nPingDataSize;
	DWORD m_dwDestIp;
protected:
	virtual void OnOK();
public:
	UINT m_nIntTime;
	UINT m_nTaskCnt;
	BOOL m_bAutoDelay;
};
