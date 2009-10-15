#pragma once
#include "afxwin.h"



// CClientRecvView 窗体视图

class CClientRecvView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CClientRecvView)

protected:
	CClientRecvView();           // 动态创建所使用的受保护的构造函数
	virtual ~CClientRecvView();

public:
	enum { IDD = IDD_DLG_RECV };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	CBCGPGridCtrl		m_wndGrid;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonClearRecv();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


