#pragma once

#include "CommMsg.h"

// CPingTestView 视图
class CPingMsgGridCtrl : public CBCGPGridCtrl
{
public:
	CBCGPGridRow * AddMsgRow ( CPingMsg *pMsg  );
	CBCGPGridRow* CreateNewRow ();
	CBCGPGridRow* GetHeadRow ();
	CBCGPGridRow* GetTailRow ();
protected:
#ifdef _DEBUG
	int AddRowEx (CBCGPGridRow* pItem );
#endif
};

class CPingTestView : public CView
{
	DECLARE_DYNCREATE(CPingTestView)

protected:
	CPingTestView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPingTestView();

	CPingMsgGridCtrl  m_wndGrid;

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void LoadPingEvent( );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


