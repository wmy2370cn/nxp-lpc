#pragma once



// CClientOptionView 窗体视图

class CClientOptionView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CClientOptionView)

protected:
	CClientOptionView();           // 动态创建所使用的受保护的构造函数
	virtual ~CClientOptionView();

public:
	enum { IDD = IDD_DLG_CLIENT_OPTIONS };
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


