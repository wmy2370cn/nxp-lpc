// outputbar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ListCtrlEx.h"


class COutputListCtrl : public CListCtrlEx
{	
public:
	COutputListCtrl();
	virtual ~COutputListCtrl();
public:
protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

class CLogStr;
class COutputBar : public CBCGPDockingControlBar
{
public:
	COutputBar();

// Attributes
protected:

	COutputListCtrl	m_wndList;
	CFont m_Font;
	void InitListControl();
	bool m_bAutoScroll;
	CImageList  m_ilSmall;
	int DispDebugStr(CLogStr *pLogData );
	CString GetItemTxt(int nSelItem) ;
	void LoadDebugStr( );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputBar();

// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////
