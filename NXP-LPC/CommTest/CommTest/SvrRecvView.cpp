// SvrRecvView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "SvrRecvView.h"


// CSvrRecvView

IMPLEMENT_DYNCREATE(CSvrRecvView, CView)

CSvrRecvView::CSvrRecvView()
{

}

CSvrRecvView::~CSvrRecvView()
{
}

BEGIN_MESSAGE_MAP(CSvrRecvView, CView)
END_MESSAGE_MAP()


// CSvrRecvView 绘图

void CSvrRecvView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CSvrRecvView 诊断

#ifdef _DEBUG
void CSvrRecvView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSvrRecvView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSvrRecvView 消息处理程序
