#pragma once


// CPacketDecodeFrm 框架

class CPacketDecodeFrm : public CBCGPMDIChildWnd
{
	DECLARE_DYNCREATE(CPacketDecodeFrm)
protected:
	CPacketDecodeFrm();           // 动态创建所使用的受保护的构造函数
	virtual ~CPacketDecodeFrm();

	CBCGPSplitterWnd m_wndSplitter;
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnClose();
};


