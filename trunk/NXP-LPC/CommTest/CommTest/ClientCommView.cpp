
// CommTestView.cpp : CClientCommView 类的实现
//

#include "stdafx.h"
#include "CommTest.h"

#include "ClientCommDoc.h"
#include "ClientCommView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientCommView

IMPLEMENT_DYNCREATE(CClientCommView, CView)

BEGIN_MESSAGE_MAP(CClientCommView, CView) 
END_MESSAGE_MAP()

// CClientCommView 构造/析构

CClientCommView::CClientCommView()
{
	// TODO: 在此处添加构造代码

}

CClientCommView::~CClientCommView()
{
}

BOOL CClientCommView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CClientCommView 绘制

void CClientCommView::OnDraw(CDC* /*pDC*/)
{
	CClientCommDoc* pDoc = (CClientCommDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CClientCommView 打印

 
void CClientCommView::OnContextMenu(CWnd* pWnd, CPoint point)
{
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CClientCommView 诊断

#ifdef _DEBUG
void CClientCommView::AssertValid() const
{
	CView::AssertValid();
}

void CClientCommView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCClientCommDoc* CClientCommView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCClientCommDoc)));
	return (CCClientCommDoc*)m_pDocument;
}
#endif //_DEBUG


// CClientCommView 消息处理程序
