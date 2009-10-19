// PingStatisView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PingStatisView.h"


// CPingStatisView

IMPLEMENT_DYNCREATE(CPingStatisView, CView)

CPingStatisView::CPingStatisView()
{

}

CPingStatisView::~CPingStatisView()
{
}

BEGIN_MESSAGE_MAP(CPingStatisView, CView)
END_MESSAGE_MAP()


// CPingStatisView 绘图

void CPingStatisView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CPingStatisView 诊断

#ifdef _DEBUG
void CPingStatisView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPingStatisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPingStatisView 消息处理程序
