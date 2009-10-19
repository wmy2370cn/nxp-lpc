
// ClientChildFrame.h : CClientChildFrame 类的接口
//

#pragma once

class CSplitterWndExt: public CBCGPSplitterWnd
{
	DECLARE_DYNAMIC(CSplitterWndExt)

	// Construction
public:
	CSplitterWndExt();

private:
	int m_nX1,m_nX2,m_nY1,m_nY2;

	// Attributes
public:
	void SetXMoveLimit(int x1 ,int x2 )
	{
		ASSERT(x2>x1);
		m_nX1 = x1;
		m_nX2 = x2;
	}

	void SetYMoveLimit( int y1,int y2 )
	{
		ASSERT(y1<y2);
		m_nY1 = y1;
		m_nY2 = y2;
	}

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterWndExt)
	//}}AFX_VIRTUAL

 
	// Implementation
public:
	virtual ~CSplitterWndExt();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplitterWndExt)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
 