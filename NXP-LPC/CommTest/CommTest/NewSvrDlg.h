#pragma once


// CNewSvrDlg 对话框

class CNewSvrDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CNewSvrDlg)

public:
	CNewSvrDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewSvrDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_SVR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	UINT m_nSvrPortNum;
protected:
	virtual void OnOK();
};
