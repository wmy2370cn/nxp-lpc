// PingTestView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PingTestView.h"


// CPingTestView

IMPLEMENT_DYNCREATE(CPingTestView, CView)

CPingTestView::CPingTestView()
{

}

CPingTestView::~CPingTestView()
{
}

BEGIN_MESSAGE_MAP(CPingTestView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPingTestView 绘图

void CPingTestView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CPingTestView 诊断

#ifdef _DEBUG
void CPingTestView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPingTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPingTestView 消息处理程序

int CPingTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectGrid;
	rectGrid.SetRectEmpty();

	m_wndGrid.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);


	return 0;
}

void CPingTestView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (m_wndGrid.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);
		m_wndGrid.MoveWindow(rc);
	}
	// TODO: 在此处添加消息处理程序代码
}
