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
