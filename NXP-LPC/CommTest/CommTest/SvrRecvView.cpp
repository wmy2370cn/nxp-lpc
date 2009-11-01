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
