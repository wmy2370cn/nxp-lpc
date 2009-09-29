
// CommTestView.cpp : CCommTestView 类的实现
//

#include "stdafx.h"
#include "CommTest.h"

#include "CommTestDoc.h"
#include "CommTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCommTestView

IMPLEMENT_DYNCREATE(CCommTestView, CView)

BEGIN_MESSAGE_MAP(CCommTestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCommTestView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCommTestView 构造/析构

CCommTestView::CCommTestView()
{
	// TODO: 在此处添加构造代码

}

CCommTestView::~CCommTestView()
{
}

BOOL CCommTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCommTestView 绘制

void CCommTestView::OnDraw(CDC* /*pDC*/)
{
	CCommTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCommTestView 打印


void CCommTestView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CCommTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCommTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCommTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CCommTestView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCommTestView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CCommTestView 诊断

#ifdef _DEBUG
void CCommTestView::AssertValid() const
{
	CView::AssertValid();
}

void CCommTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCommTestDoc* CCommTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCommTestDoc)));
	return (CCommTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CCommTestView 消息处理程序
