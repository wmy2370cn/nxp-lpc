// ClientRecvView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "ClientRecvView.h"


// CClientRecvView

IMPLEMENT_DYNCREATE(CClientRecvView, CBCGPFormView)

CClientRecvView::CClientRecvView()
	: CBCGPFormView(CClientRecvView::IDD)
{

}

CClientRecvView::~CClientRecvView()
{
}

void CClientRecvView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientRecvView, CBCGPFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CClientRecvView 诊断

#ifdef _DEBUG
void CClientRecvView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CClientRecvView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClientRecvView 消息处理程序

int CClientRecvView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	EnableVisualManagerStyle();

 	CRect rectGrid;
	rectGrid.SetRectEmpty();
	 
	m_wndGrid.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	
	m_wndGrid.SetReadOnly ();
	m_wndGrid.SetWholeRowSel ();
	m_wndGrid.EnableHeader (TRUE, 0);

	return 0;
}

void CClientRecvView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码	
	 if ( m_wndGrid.GetSafeHwnd())
	{
		CRect rectClient;
		GetClientRect (rectClient);
		rectClient.InflateRect (2, 2);

		 rectClient.top = rectClient.top + 40;
		m_wndGrid.MoveWindow(rectClient);
	}
}
