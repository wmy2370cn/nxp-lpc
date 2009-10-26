// SvrOptionView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "SvrOptionView.h"
#include "SvrCommDoc.h"


// CSvrOptionView

IMPLEMENT_DYNCREATE(CSvrOptionView, CBCGPFormView)

CSvrOptionView::CSvrOptionView()
	: CBCGPFormView(CSvrOptionView::IDD)
{

}

CSvrOptionView::~CSvrOptionView()
{
}

void CSvrOptionView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSvrOptionView, CBCGPFormView)
	ON_BN_CLICKED(IDC_BUTTON_SVR_START, &CSvrOptionView::OnBnClickedButtonSvrStart)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CSvrOptionView 诊断

#ifdef _DEBUG
void CSvrOptionView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSvrOptionView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSvrOptionView 消息处理程序


int CSvrOptionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableVisualManagerStyle();

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CSvrOptionView::OnBnClickedButtonSvrStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CSvrCommDoc *pDoc = (CSvrCommDoc *) GetDocument();
	ASSERT(pDoc);
	if (pDoc == NULL)
		return;
	pDoc->m_SvrComm.Start();

}