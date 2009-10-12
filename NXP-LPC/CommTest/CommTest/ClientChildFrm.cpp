
// ClientChildFrame.cpp : CClientChildFrame 类的实现
//

#include "stdafx.h"
#include "CommTest.h"

#include "MainFrm.h"
#include "ClientChildFrm.h"
#include "ClientSendView.h"
#include "ClientRecvView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientChildFrame

IMPLEMENT_DYNCREATE(CClientChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CClientChildFrame, CBCGPMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CClientChildFrame 构造/析构

CClientChildFrame::CClientChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CClientChildFrame::~CClientChildFrame()
{
}


BOOL CClientChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style &= ~WS_SYSMENU; 
	return TRUE;
}

// CClientChildFrame 诊断

#ifdef _DEBUG
void CClientChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CClientChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

void CClientChildFrame::ActivateFrame(int nCmdShow)
{
	CBCGPMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}

// CClientChildFrame 消息处理程序

int CClientChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
	

	return 0;
}

BOOL CClientChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!m_wndSplitter1.CreateStatic(this, 1, 2))
		return FALSE;
	
	if (!m_wndSplitter1.CreateView(0, 0, pContext->m_pNewViewClass, CSize(200, 100), pContext) )
	{
		m_wndSplitter1.DestroyWindow();
		return FALSE;
	}
	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter1,	// our parent window is the first splitter
		2, 1,											// the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,				// style, WS_BORDER is needed
		m_wndSplitter1.IdFromRowCol(0, 1)))				// new splitter is in the first row, 3rd column of first splitter
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}
//	m_wndSplitter1.ModifyStyle (WS_HSCROLL | WS_VSCROLL, 0);
	CRect rectClient;
	GetClientRect (rectClient);

	m_wndSplitter2.CreateView (0, 0, RUNTIME_CLASS (CClientSendView), CSize(0, 0), pContext);
	m_wndSplitter2.CreateView (1, 0, RUNTIME_CLASS (CClientRecvView), CSize(0, 0), pContext);

	m_wndSplitter1.SetColumnInfo(0, rectClient.Width() / 4 , 30);

	m_wndSplitter2.SetRowInfo(0, rectClient.Height () / 3 , 40);
	m_wndSplitter2.SetWindowPos (NULL, 0, 0, rectClient.Width (), rectClient.Height (), SWP_NOZORDER | SWP_NOREDRAW);
	m_wndSplitter2.RecalcLayout();

#if 0
	if (m_wndSplitter.GetSafeHwnd () != NULL) 
		return FALSE;
	if (!m_wndSplitter.CreateStatic (this, 2,1))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_wndSplitter.ModifyStyle (WS_HSCROLL | WS_VSCROLL, 0);
	CRect rectClient;
	GetClientRect (rectClient);
	m_wndSplitter.CreateView (0, 0, pContext->m_pNewViewClass, CSize(0, 0), pContext);
	m_wndSplitter.CreateView (1, 0, RUNTIME_CLASS (CClientRecvView), CSize(0, 0), pContext);

	m_wndSplitter.SetRowInfo(0, rectClient.Height () / 3 , 30);
	m_wndSplitter.SetWindowPos (NULL, 0, 0, rectClient.Width (), rectClient.Height (), SWP_NOZORDER | SWP_NOREDRAW);
	m_wndSplitter.RecalcLayout();
#endif
	return TRUE;

	return CBCGPMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CClientChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CBCGPMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: 在此处添加消息处理程序代码

}

void CClientChildFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CBCGPMDIChildWnd::OnClose();
}

void CClientChildFrame::OnDestroy()
{
	CMainFrame*pMain = (CMainFrame*) GetMDIFrame();
	ASSERT(pMain);
	if (pMain)
	{
		pMain->CloseChildFrm(this);
	}

	CBCGPMDIChildWnd::OnDestroy();
	

	// TODO: 在此处添加消息处理程序代码
}
