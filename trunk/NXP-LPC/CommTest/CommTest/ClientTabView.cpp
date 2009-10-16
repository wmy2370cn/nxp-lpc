// ClientTabView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "ClientTabView.h"
#include "ClientRecvView.h"
#include "ClientCommDoc.h"
#include "ClientStatisView.h"


// CClientTabView

IMPLEMENT_DYNCREATE(CClientTabView, CBCGPTabView)

CClientTabView::CClientTabView()
{

}

CClientTabView::~CClientTabView()
{
}

BEGIN_MESSAGE_MAP(CClientTabView, CBCGPTabView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CClientTabView 绘图

void CClientTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CClientTabView 诊断

#ifdef _DEBUG
void CClientTabView::AssertValid() const
{
	CBCGPTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CClientTabView::Dump(CDumpContext& dc) const
{
	CBCGPTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClientTabView 消息处理程序

int CClientTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	 
//	CFrameWnd *pFrame = (CFrameWnd*)theApp.m_pMainWnd;
	//	ASSERT(pFrame);
	CClientCommDoc* pDoc = (CClientCommDoc*)GetDocument ();
	ASSERT(pDoc);

	// TODO:  在此添加您专用的创建代码
  	CCreateContext context;
 	memset(&context,0,sizeof(CCreateContext));	
  	context.m_pCurrentDoc = pDoc;	
//	AddView(_T("数据信息"),RUNTIME_CLASS(CClientRecvView),pDoc);
//	AddView(_T("统计信息"),RUNTIME_CLASS(CClientStatisView),pDoc);
	AddView(RUNTIME_CLASS(CClientRecvView),_T("数据信息"),-1,&context);
	AddView(RUNTIME_CLASS(CClientStatisView),_T("统计信息"),-1,&context);

	return 0;
}

void CClientTabView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPTabView::OnSize(nType, cx, cy);

	 
	// TODO: 在此处添加消息处理程序代码
}
