// PingOptionView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PingOptionView.h"
#include "PingTestDoc.h"

// CPingOptionView

IMPLEMENT_DYNCREATE(CPingOptionView, CBCGPFormView)

CPingOptionView::CPingOptionView()
	: CBCGPFormView(CPingOptionView::IDD)
	, m_nTaskCnt(1)
	, m_nPingDataSize(32)
	, m_nIntTime(300)
	, m_bAutoDelay(TRUE)
{

}

CPingOptionView::~CPingOptionView()
{
}

void CPingOptionView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_PING_DEST_OPT, m_wndDestIp);
	DDX_Text(pDX, IDC_EDIT_PING_THREAD_CNT_OPT, m_nTaskCnt);
	DDV_MinMaxUInt(pDX, m_nTaskCnt, 1, 16);
	DDX_Text(pDX, IDC_EDIT_PING_DATA_SIZE, m_nPingDataSize);
	DDV_MinMaxUInt(pDX, m_nPingDataSize, 32, 64000);
	DDX_Text(pDX, IDC_EDIT_PING_INT_TIME, m_nIntTime);
	DDV_MinMaxUInt(pDX, m_nIntTime, 32, 50000);
	DDX_Check(pDX, IDC_CHECK_TIMEOUT_DELAY, m_bAutoDelay);
}

BEGIN_MESSAGE_MAP(CPingOptionView, CBCGPFormView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_PING_START, &CPingOptionView::OnBnClickedButtonPingStart)
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

void CPingOptionView::OnUpdate(CView*  pSender , LPARAM lHint, CObject*  pHint )
{
	// TODO: 在此添加专用代码和/或调用基类
	if(lHint == UPDATA_VIEW_PING_NORMAL)
	{
		CPingTestDoc *pDoc = (CPingTestDoc *) GetDocument();
		ASSERT(pDoc);
		if (pDoc == NULL)
			return;

	 	m_wndDestIp.SetAddress(pDoc->m_dwDestIp);

		m_nTaskCnt = pDoc->m_nTaskCnt;
		m_nPingDataSize = pDoc->m_nPingDataSize;
		m_bAutoDelay = pDoc->m_bAutoDelay;
		m_nIntTime = pDoc->m_nIntTime;		

		UpdateData(TRUE);
	}
}

void CPingOptionView::OnBnClickedButtonPingStart()
{
	// TODO: 在此添加控件通知处理程序代码
}
