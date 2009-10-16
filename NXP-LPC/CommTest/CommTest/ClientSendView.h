#pragma once
#include "afxwin.h"



// CClientSendView 窗体视图

class CClientSendView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CClientSendView)

protected:
	CClientSendView();           // 动态创建所使用的受保护的构造函数
	virtual ~CClientSendView();

public:
	enum { IDD = IDD_DLG_SEND };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void OnConnected( );
	void OnDisconnected();
private:
	void BuildSendTxt( BOOL bUpdata = TRUE );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSend();
	CEdit m_wndSend;
	BOOL m_bHex;
	afx_msg void OnEnChangeEditSendTxt();
	afx_msg void OnBnClickedCheckAutoSend();
	BOOL m_bAutoSend;
	UINT m_nSendIntTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonClear();
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButtonStop();
};


