// PacketTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PacketTreeView.h"


// CPacketTreeView

IMPLEMENT_DYNCREATE(CPacketTreeView, CView)

CPacketTreeView::CPacketTreeView()
{

}

CPacketTreeView::~CPacketTreeView()
{
}

BEGIN_MESSAGE_MAP(CPacketTreeView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CPacketTreeView 绘图

void CPacketTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CPacketTreeView 诊断

#ifdef _DEBUG
void CPacketTreeView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPacketTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPacketTreeView 消息处理程序

int CPacketTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}
