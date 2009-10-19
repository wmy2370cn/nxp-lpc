#pragma once


// CNewPingDlg 对话框

class CNewPingDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CNewPingDlg)

public:
	CNewPingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewPingDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
