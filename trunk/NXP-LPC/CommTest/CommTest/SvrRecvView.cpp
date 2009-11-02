// SvrRecvView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "SvrRecvView.h"


// CSvrRecvView

 

IMPLEMENT_DYNCREATE(CSvrRecvView, CBCGPFormView)

CSvrRecvView::CSvrRecvView()
: CBCGPFormView(CSvrRecvView::IDD)
{

}

CSvrRecvView::~CSvrRecvView()
{
}

void CSvrRecvView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSvrRecvView, CBCGPFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSvrRecvView 诊断

#ifdef _DEBUG
void CSvrRecvView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSvrRecvView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSvrRecvView 消息处理程序

int CSvrRecvView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	EnableVisualManagerStyle();

	return 0;
}

void CSvrRecvView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

void CSvrRecvView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBCGPFormView::OnTimer(nIDEvent);
}
