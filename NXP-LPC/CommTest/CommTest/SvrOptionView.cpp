// SvrOptionView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "SvrOptionView.h"


// CSvrOptionView

IMPLEMENT_DYNCREATE(CSvrOptionView, CFormView)

CSvrOptionView::CSvrOptionView()
	: CFormView(CSvrOptionView::IDD)
{

}

CSvrOptionView::~CSvrOptionView()
{
}

void CSvrOptionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSvrOptionView, CFormView)
END_MESSAGE_MAP()


// CSvrOptionView 诊断

#ifdef _DEBUG
void CSvrOptionView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSvrOptionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSvrOptionView 消息处理程序
