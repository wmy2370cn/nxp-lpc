// ClientSendView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "ClientSendView.h"
#include "ClientCommDoc.h"
#include "Common.h"


// CClientSendView

IMPLEMENT_DYNCREATE(CClientSendView, CBCGPFormView)

CClientSendView::CClientSendView()
	: CBCGPFormView(CClientSendView::IDD)
	, m_bHex(FALSE)
	, m_bAutoSend(FALSE)
	, m_nSendIntTime(200)
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
	DDX_Check(pDX, IDC_CHECK_AUTO_SEND, m_bAutoSend);
	DDX_Text(pDX, IDC_EDIT_INT_TIME, m_nSendIntTime);
	DDV_MinMaxUInt(pDX, m_nSendIntTime, 100, 1000000);
}

BEGIN_MESSAGE_MAP(CClientSendView, CBCGPFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClientSendView::OnBnClickedButtonSend)
	ON_EN_CHANGE(IDC_EDIT_SEND_TXT, &CClientSendView::OnEnChangeEditSendTxt)
	ON_BN_CLICKED(IDC_CHECK_AUTO_SEND, &CClientSendView::OnBnClickedCheckAutoSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CClientSendView::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CClientSendView::OnBnClickedButtonStop)
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
	CClientCommDoc *pDoc = (CClientCommDoc *) GetDocument();
	ASSERT(pDoc);
	if (pDoc == NULL)
		return;

	CCommMsg msg(MSG_SEND_DATA);
	pDoc->m_ClientComm.m_CommMsg.AddCommMsg(msg);
}
const UINT_PTR ID_UPDATE_SEND_TXT = 200804;
void CClientSendView::BuildSendTxt( BOOL bUpdata /*= TRUE*/ )
{
	CClientCommDoc *pDoc = (CClientCommDoc *) GetDocument();
	ASSERT(pDoc);
	if (pDoc == NULL)
		return;

	m_wndSend.GetWindowText( pDoc->m_szRawSendTxt );

	pDoc->m_szRawSendTxt.MakeUpper();

	int nLen = pDoc->m_szRawSendTxt.GetLength();
	int i = 0;

	UpdateData(TRUE);

	if (m_bHex)
	{//如果是16进制，那么只能是  0 - F
		CString szTxt;
		TCHAR tc;
		for ( i = 0; i < nLen; i++)
		{
			tc = pDoc->m_szRawSendTxt.GetAt(i);
			if (( tc >= L'0' &&  tc <= L'9' )||  ( tc >= L'A' &&  tc <= L'F' ))
			{
				szTxt.AppendChar( tc );
			}
		}
		nLen = szTxt.GetLength();
	    pDoc->m_szRawSendTxt.Empty();
		if (nLen)
		{
			int nBufLen =(int)((nLen+1)/2);
			unsigned char *pData = new unsigned char [nBufLen+10];
			unsigned char low =0 ,high = 0;
			unsigned char data  = 0;
			unsigned int idx = 0;
			for ( i = 0; i < (int)(nLen/2); i++)
			{
				tc = szTxt.GetAt(2*i);
				high  =  Common::HexAscToInt(tc);
				tc = szTxt.GetAt(2*i+1);
				low  =  Common::HexAscToInt(tc);

				pData[idx] = static_cast <unsigned char> (high << 4 | low);
				idx++;
			}
			if (nLen%2)
			{
				tc = szTxt.GetAt( szTxt.GetLength()-1 );
				pData[idx] = Common::HexAscToInt(tc);
			}
			pDoc->m_ClientComm.m_SendBuf.SetData( pData,nBufLen );
			delete [] pData;

			for ( i = 0; i < (int)(nLen/2); i++)
			{
				tc = szTxt.GetAt(2*i);
				pDoc->m_szRawSendTxt.AppendChar(tc);
				tc = szTxt.GetAt(2*i+1);
				pDoc->m_szRawSendTxt.AppendChar(tc);
				tc = L' ';
				pDoc->m_szRawSendTxt.AppendChar(tc);
			}

			if (nLen%2)
			{
				tc = L'0';
				pDoc->m_szRawSendTxt.AppendChar(tc);
				tc = szTxt.GetAt(szTxt.GetLength()-1);
				pDoc->m_szRawSendTxt.AppendChar(tc);
			}
			else
			{
				pDoc->m_szRawSendTxt.Delete(pDoc->m_szRawSendTxt.GetLength()-1 );
			}
		}
		if (bUpdata)
		{
			SetTimer(ID_UPDATE_SEND_TXT,8000,NULL);
		}
		//	m_wndSend.SetWindowText( pDoc->m_szRawSendTxt );	
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
		if (!szTxt.IsEmpty())
		{
			pDoc->m_ClientComm.m_SendBuf.SetData(szTxt.GetBuffer(szTxt.GetLength()) );
			szTxt.ReleaseBuffer();
		}
	} 
}

void CClientSendView::OnEnChangeEditSendTxt()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	BuildSendTxt();

}

void CClientSendView::OnConnected( )
{
	CWnd *pSndBtn = GetDlgItem(IDC_BUTTON_SEND);
	ASSERT(pSndBtn);
	if (pSndBtn)
	{
		pSndBtn->EnableWindow(TRUE);
	}

// 	CWnd *pStopBnt = GetDlgItem(IDC_BUTTON_STOP);
// 	ASSERT(pStopBnt);
// 	if (pStopBnt)
// 	{
// 		pStopBnt->EnableWindow(TRUE);
// 	}
	
	CWnd *pCheckWnd = GetDlgItem(IDC_CHECK_AUTO_SEND);
	ASSERT(pCheckWnd);
	if (pCheckWnd)
	{
		pCheckWnd->EnableWindow(TRUE);
	}
}

void CClientSendView::OnDisconnected()
{
	CWnd *pSndBtn = GetDlgItem(IDC_BUTTON_SEND);
	ASSERT(pSndBtn);
	if (pSndBtn)
	{
		pSndBtn->EnableWindow(FALSE);
	}

	CWnd *pStopBnt = GetDlgItem(IDC_BUTTON_STOP);
	ASSERT(pStopBnt);
	if (pStopBnt)
	{
		pStopBnt->EnableWindow(FALSE);
	}

	CWnd *pTimeWnd = GetDlgItem(IDC_EDIT_INT_TIME);
	ASSERT(pTimeWnd);
	if (pTimeWnd)
	{
		pTimeWnd->EnableWindow(FALSE);
	}
	CWnd *pCheckWnd = GetDlgItem(IDC_CHECK_AUTO_SEND);
	ASSERT(pCheckWnd);
	if (pCheckWnd)
	{
		pCheckWnd->EnableWindow(FALSE);
	}
}

const UINT_PTR ID_AUTO_SEND = 2010;

void CClientSendView::OnBnClickedCheckAutoSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CWnd *pWnd = GetDlgItem(IDC_EDIT_INT_TIME);
	ASSERT(pWnd);

	if (m_bAutoSend)
	{
		pWnd->EnableWindow(TRUE);
		if (m_nSendIntTime<100)
		{
			m_nSendIntTime = 100;
		}
		CWnd *pStopBnt = GetDlgItem(IDC_BUTTON_STOP);
		ASSERT(pStopBnt);
		if (pStopBnt)
		{
			pStopBnt->EnableWindow(TRUE);
		}
		KillTimer(ID_AUTO_SEND);
		SetTimer(ID_AUTO_SEND,m_nSendIntTime,NULL);
	}
	else
	{
		pWnd->EnableWindow(FALSE);
	}
}
void CClientSendView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ID_AUTO_SEND)
	{
		KillTimer(ID_AUTO_SEND);

		CClientCommDoc *pDoc = (CClientCommDoc *) GetDocument();
		ASSERT(pDoc);
		if (pDoc != NULL)
		{
			CCommMsg msg(MSG_SEND_DATA);
			pDoc->m_ClientComm.m_CommMsg.AddCommMsg(msg);
			SetTimer(ID_AUTO_SEND,m_nSendIntTime,NULL);
		}	
	}
	if ( nIDEvent == ID_UPDATE_SEND_TXT )
	{
		KillTimer(nIDEvent);
		CClientCommDoc *pDoc = (CClientCommDoc *) GetDocument();
		ASSERT(pDoc);
		if (pDoc != NULL)
		{
			m_wndSend.SetWindowText( pDoc->m_szRawSendTxt );	
			int nNum=m_wndSend.GetWindowTextLength();
			m_wndSend.SetSel(nNum, nNum);
		}
	}

	CBCGPFormView::OnTimer(nIDEvent);
}

void CClientSendView::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_wndSend.SetReadOnly(FALSE);
	m_wndSend.SetSel(0, -1);	
	m_wndSend.Clear();
//	m_wndSend.SetReadOnly(TRUE);	
}

void CClientSendView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
}

void CClientSendView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	OnDisconnected();
	// TODO: 在此添加专用代码和/或调用基类
}

void CClientSendView::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
}
