#pragma once


#include "OScopeCtrl.h"
#include "SplitterWndExt.h"

class CStatisGridCtrl: public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CStatisGridCtrl)
public:
	CStatisGridCtrl(void);
	virtual ~CStatisGridCtrl(void);

	virtual void SetRowHeight ()
	{
		CBCGPGridCtrl::SetRowHeight ();
		m_nRowHeight += 8;
		m_nLargeRowHeight += 8;
	}

	CBCGPGridRow* CreateNewRow ();
public:
	void AddEmptyRow ();
 
	//	virtual BOOL EndEditItem (BOOL bUpdateData = TRUE);	 
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

class CGridChartWnd : public CWnd
{
	DECLARE_DYNAMIC(CGridChartWnd)

public:
	CGridChartWnd();
	virtual ~CGridChartWnd();

	const CBCGPSplitterWnd* GetSplitter () const;
	const CStatisGridCtrl* GetGrid () const;
	const COScopeCtrl* GetChart () const;

	CStatisGridCtrl*         GetGrid ();
	COScopeCtrl*        GetChart ();
	CBCGPSplitterWnd*       GetSplitter ();

	virtual int  CreateControls ();

	virtual CStatisGridCtrl* CreateGrid ();
	virtual COScopeCtrl* CreateChart ();

protected:
	CWnd*	        m_pParentWnd;
//	CStatisGridCtrlSplitter    m_wndSplitter;
	CSplitterWndExt    m_wndSplitter;

	CStatisGridCtrl   *m_pGrid;
	COScopeCtrl      *m_pChart;
	CFont			m_Font;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create (DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnOscopePositionMsg(WPARAM wParam, LPARAM lParam);
};
 
// CClientStatisView 视图

class CClientStatisView : public CView
{
	DECLARE_DYNCREATE(CClientStatisView)

protected:
	CClientStatisView();           // 动态创建所使用的受保护的构造函数
	virtual ~CClientStatisView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	CGridChartWnd  m_wndGridChart;
	void	RecalLayout();
	CBCGPToolTipCtrl  m_TimeToolTips;
//	CToolTipCtrl  m_TimeToolTips;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg LRESULT OnOscopePositionMsg(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


