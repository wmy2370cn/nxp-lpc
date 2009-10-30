// PacketDecodeFrm.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h" 
#include "SvrCommFrm.h"
#include "SvrRecvView.h"
 


// CSvrCommFrm

IMPLEMENT_DYNCREATE(CSvrCommFrm, CBCGPMDIChildWnd)

CSvrCommFrm::CSvrCommFrm()
{

}

CSvrCommFrm::~CSvrCommFrm()
{
}


BEGIN_MESSAGE_MAP(CSvrCommFrm, CBCGPMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSvrCommFrm 消息处理程序

BOOL CSvrCommFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style &= ~WS_SYSMENU;
 	cs.style  &= ~FWS_ADDTOTITLE ;

	return CBCGPMDIChildWnd::PreCreateWindow(cs);
}

int CSvrCommFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	 
	return 0;
}

BOOL CSvrCommFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (!m_wndSplitter.CreateStatic (this, 2,1))
	{
		ASSERT(FALSE);
		return -1;
	}

	m_wndSplitter.ModifyStyle (WS_HSCROLL | WS_VSCROLL, 0);

	CRect rectClient;
	GetClientRect (rectClient);

	m_wndSplitter.CreateView (0, 0, RUNTIME_CLASS (CSvrRecvView), CSize (0, rectClient.Width() / 2), pContext);
	m_wndSplitter.CreateView (1,0, RUNTIME_CLASS (CSvrRecvView), CSize (0, rectClient.Width () / 2), pContext);

	return TRUE;
//	return CBCGPMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CSvrCommFrm::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	CDocument *pDoc = GetActiveDocument();
// 	if (pDoc->IsKindOf(RUNTIME_CLASS( CClientCommDoc )))
// 	{
// 		((CClientCommDoc*)pDoc)->m_pDecodeFrm = NULL;
// 		DestroyWindow();
// 		return;
// 	}
//	else


	CBCGPMDIChildWnd::OnClose();
}
