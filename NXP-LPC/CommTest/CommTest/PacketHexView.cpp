// PacketHexView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PacketHexView.h"


// CPacketHexView

IMPLEMENT_DYNCREATE(CPacketHexView, CView)

CPacketHexView::CPacketHexView()
{

}

CPacketHexView::~CPacketHexView()
{
}

BEGIN_MESSAGE_MAP(CPacketHexView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CPacketHexView 绘图

void CPacketHexView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CPacketHexView 诊断

#ifdef _DEBUG
void CPacketHexView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPacketHexView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPacketHexView 消息处理程序

int CPacketHexView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}
