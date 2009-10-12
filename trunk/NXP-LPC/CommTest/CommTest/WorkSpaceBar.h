// workspacebar.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <hash_map>
 

typedef stdext::hash_map <DWORD_PTR ,HTREEITEM> CTreeItemMap ;
typedef std::pair <DWORD_PTR,HTREEITEM> TreeItem_Pair;

class CWorkSpaceBar : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar();

// Attributes
protected:
	CTreeCtrl	m_wndTree;
	int  InitImages( );
	CImageList	m_Images;
	void  ExpandItem(HTREEITEM hItem,UINT nCode);

	CString  m_szEth;
	CString  m_szClientMode;
	CString  m_szSvrMode;
	CString  m_szPingMode;
	CString  m_szCom;
	BOOL  InitString( );

	HTREEITEM m_hEth;
	HTREEITEM m_hCom;
	HTREEITEM m_hClientMode;
	HTREEITEM m_hSvrMode;
	HTREEITEM m_hPingMode;

	CTreeItemMap m_TreeItemMap;


// Operations
public:
	void CloseFrame( CWnd *pWnd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkSpaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTreeExpand();
	afx_msg void OnTreeCollapse();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnNewTest();
};

/////////////////////////////////////////////////////////////////////////////
