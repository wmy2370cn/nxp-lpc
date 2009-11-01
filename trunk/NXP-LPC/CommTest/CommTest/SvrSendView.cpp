// SvrSendView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "SvrSendView.h"


// CSvrSendView

IMPLEMENT_DYNCREATE(CSvrSendView, CBCGPFormView)

CSvrSendView::CSvrSendView()
	: CBCGPFormView(CSvrSendView::IDD)
{

}

CSvrSendView::~CSvrSendView()
{
}

void CSvrSendView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSvrSendView, CBCGPFormView)
END_MESSAGE_MAP()


// CSvrSendView 诊断

#ifdef _DEBUG
void CSvrSendView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSvrSendView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSvrSendView 消息处理程序
