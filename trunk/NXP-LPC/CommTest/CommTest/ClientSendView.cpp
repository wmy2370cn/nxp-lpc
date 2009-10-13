// ClientSendView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "ClientSendView.h"
#include "ClientCommDoc.h"


// CClientSendView

IMPLEMENT_DYNCREATE(CClientSendView, CBCGPFormView)

CClientSendView::CClientSendView()
	: CBCGPFormView(CClientSendView::IDD)
	, m_bHex(FALSE)
{

}

CClientSendView::~CClientSendView()
{
}

void CClientSendView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SEND_TXT, m_wndSend);
	DDX_Check(pDX, IDC_CHECK_HEX, m_bHex);
}

BEGIN_MESSAGE_MAP(CClientSendView, CBCGPFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClientSendView::OnBnClickedButtonSend)
	ON_EN_CHANGE(IDC_EDIT_SEND_TXT, &CClientSendView::OnEnChangeEditSendTxt)
END_MESSAGE_MAP()


// CClientSendView 诊断

#ifdef _DEBUG
void CClientSendView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CClientSendView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClientSendView 消息处理程序

int CClientSendView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	EnableVisualManagerStyle();

	return 0;
}

void CClientSendView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd *pEdit = GetDlgItem(IDC_EDIT_SEND_TXT);
	if (pEdit && pEdit->GetSafeHwnd())
	{
		CRect rcEdit;
		CRect rectClient;

		GetClientRect (rectClient);
		pEdit->GetWindowRect(rcEdit);
		ScreenToClient (rcEdit);

		rcEdit.left = rectClient.left + 2;
		rcEdit.right = rectClient.right - 3;
		rcEdit.bottom = rectClient.bottom - 3;
		pEdit->MoveWindow(rcEdit);
	}
}

void CClientSendView::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CClientSendView::OnEnChangeEditSendTxt()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CClientCommDoc *pDoc = (CClientCommDoc *) GetDocument();
	ASSERT(pDoc);
	if (pDoc == NULL)
		return;

	m_wndSend.GetWindowText( pDoc->m_szRawSendTxt );

	pDoc->m_szRawSendTxt.MakeUpper();

	int nLen = pDoc->m_szRawSendTxt.GetLength();
	int i = 0;

	if (m_bHex)
	{//如果是16进制，那么只能是  0 - F
		for ( i = 0; i < nLen; i++)
		{

		}
	}
	else
	{//输入什么就发什么
		if ( nLen > MAX_SEND_LEN  )
		{
			pDoc->m_szRawSendTxt.Left(MAX_SEND_LEN);
			m_wndSend.SetWindowText( pDoc->m_szRawSendTxt );
		}

		CStringA szTxt; 
		szTxt = CT2CA( pDoc->m_szRawSendTxt );

 

	}

//	pDoc->m_szRawSendTxt.Trim()


	CSingleLock lock(& (pDoc->m_nSendTxtMutex) );




	


	lock.Unlock();
}
