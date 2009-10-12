#pragma once



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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


