// PingOptionView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PingOptionView.h"


// CPingOptionView

IMPLEMENT_DYNCREATE(CPingOptionView, CBCGPFormView)

CPingOptionView::CPingOptionView()
	: CBCGPFormView(CPingOptionView::IDD)
{

}

CPingOptionView::~CPingOptionView()
{
}

void CPingOptionView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPingOptionView, CBCGPFormView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CPingOptionView 诊断

#ifdef _DEBUG
void CPingOptionView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPingOptionView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPingOptionView 消息处理程序

int CPingOptionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	EnableVisualManagerStyle();
	return 0;
}
