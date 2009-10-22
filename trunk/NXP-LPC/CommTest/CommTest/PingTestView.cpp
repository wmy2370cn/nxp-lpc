// PingTestView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PingTestView.h"
#include "PingTestDoc.h"

typedef enum  PING_GRID_COL
{
	PING_GRID_COLUMN_ID,  // 序号
	PING_GRID_COLUMN_DATE,  // 日期 
	PING_GRID_COLUMN_TIME , //时间
	PING_GRID_COLUMN_TXT,
	PING_GRID_COLUMN_RTT,  //往返时间  --超时或者 ms
	PING_GRID_COLUMN_REPLY_TM   //返回时间

// 	PACKET_GRID_COLUMN_LEN , //长度
// 	PACKET_GRID_COLUMN_PACKET   //报文
	//	PACKET_GRID_COLUMN_COMMENT     //备注,可编辑
};
#ifdef _DEBUG
class CBCGPGridRowEx : public CBCGPGridRow
{
	friend CPingMsgGridCtrl;
};

int CPingMsgGridCtrl::AddRowEx (CBCGPGridRow* pItem )
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem); 

	((CBCGPGridRowEx*)pItem)->SetOwnerList (this);

	m_lstItems.AddTail (pItem);
	int nIndex = (int) m_lstItems.GetCount () - 1;
	((CBCGPGridRowEx*)pItem)->m_nIdRow = nIndex;

	SetRebuildTerminalItems ();
	return nIndex;
}
#endif // _DEBUG

CBCGPGridRow *CPingMsgGridCtrl::AddMsgRow( CPingMsg *pMsg )
{
	ASSERT(pMsg);
	if (pMsg == NULL)
		return NULL;
	CBCGPGridRow *pRow = CreateNewRow ();

	pRow->GetItem(PING_GRID_COLUMN_ID)->SetValue( pMsg->m_nIdx );
	CString szTxt;

	szTxt.Format(_T("%d-%02d-%02d"),pMsg->m_stRequest.wYear,pMsg->m_stRequest.wMonth,pMsg->m_stRequest.wDay);
	pRow->GetItem(PING_GRID_COLUMN_DATE)->SetValue( (_variant_t)szTxt );
  
	szTxt.Format(_T("%02d:%02d:%02d.%03d"),pMsg->m_stRequest.wHour,pMsg->m_stRequest.wMinute,pMsg->m_stRequest.wSecond,
		pMsg->m_stRequest.wMilliseconds);
	pRow->GetItem(PING_GRID_COLUMN_TIME)->SetValue( (_variant_t)szTxt );

	if(pMsg->m_nStatus)
	{
		szTxt.Format(_T("线程[%d] Ping %d.%d.%d.%d 超时"),pMsg->m_nTaskId,pMsg->m_Address.s_net, pMsg->m_Address.s_host,pMsg->m_Address.s_lh,
			pMsg->m_Address.s_impno  );
		pRow->GetItem(PING_GRID_COLUMN_TXT)->SetValue( (_variant_t)szTxt );

		szTxt = _T("超时");
		pRow->GetItem(PING_GRID_COLUMN_RTT)->SetValue( (_variant_t)szTxt );
	}
	else
	{
		szTxt.Format(_T("线程[%d] Ping %d.%d.%d.%d 返回"),pMsg->m_nTaskId,pMsg->m_Address.s_net, pMsg->m_Address.s_host,pMsg->m_Address.s_lh,
			pMsg->m_Address.s_impno  );
		pRow->GetItem(PING_GRID_COLUMN_TXT)->SetValue( (_variant_t)szTxt );

		pRow->GetItem(PING_GRID_COLUMN_RTT)->SetValue( pMsg->m_dwRTT );
	}

	szTxt.Format(_T("%02d:%02d:%02d.%03d"),pMsg->m_stReply.wHour,pMsg->m_stReply.wMinute,pMsg->m_stReply.wSecond,
		pMsg->m_stReply.wMilliseconds);
	pRow->GetItem(PING_GRID_COLUMN_REPLY_TM)->SetValue( (_variant_t)szTxt );

#ifdef _DEBUG
	AddRowEx(pRow);
#else
	AddRow (pRow, FALSE);
#endif // _DEBUG 
	return pRow; 
}

CBCGPGridRow* CPingMsgGridCtrl::CreateNewRow ()
{
	CBCGPGridRow* pRow = CreateRow (GetColumnCount());

	for (int i = 0; i < GetColumnCount ();i++)
	{
		pRow->GetItem (i)->AllowEdit (FALSE);
	}

	return pRow;
}

CBCGPGridRow* CPingMsgGridCtrl::GetHeadRow ()
{
	POSITION pos = m_lstItems.GetHeadPosition ();
	if (pos)
	{
		CBCGPGridRow* pItem = m_lstItems.GetNext (pos);
		ASSERT_VALID (pItem);
		return pItem;
	}
	return NULL;
}

CBCGPGridRow* CPingMsgGridCtrl::GetTailRow ()
{
	POSITION pos = m_lstItems.GetTailPosition ();
	if (pos)
	{
		CBCGPGridRow* pItem = m_lstItems.GetPrev (pos);
		ASSERT_VALID (pItem);
		return pItem;
	}
	return NULL;
}
// CPingTestView

IMPLEMENT_DYNCREATE(CPingTestView, CView)

CPingTestView::CPingTestView()
{

}

CPingTestView::~CPingTestView()
{
}

BEGIN_MESSAGE_MAP(CPingTestView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPingTestView 绘图

void CPingTestView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CPingTestView 诊断

#ifdef _DEBUG
void CPingTestView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPingTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

const UINT_PTR ID_LOAD_PING_EVENT = 200910;
// CPingTestView 消息处理程序

int CPingTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectGrid;
	rectGrid.SetRectEmpty();

	m_wndGrid.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	m_wndGrid.SetSingleSel(TRUE);
	m_wndGrid.SetWholeRowSel (TRUE);
	m_wndGrid.EnableMarkSortedColumn (FALSE);
	m_wndGrid.EnableMultipleSort(FALSE);
	m_wndGrid.EnableHeader (TRUE,BCGP_GRID_HEADER_MOVE_ITEMS); 		 
	m_wndGrid.EnableLineNumbers(TRUE);

	m_wndGrid.InsertColumn (PING_GRID_COLUMN_ID, _T("序号"), 70);
	m_wndGrid.InsertColumn (PING_GRID_COLUMN_DATE, _T("日期"), 90);
	m_wndGrid.InsertColumn (PING_GRID_COLUMN_TIME, _T("时间"), 100);
	m_wndGrid.InsertColumn (PING_GRID_COLUMN_TXT, _T("  "),240);
	m_wndGrid.InsertColumn (PING_GRID_COLUMN_RTT, _T("往返耗时"),100);
	m_wndGrid.InsertColumn (PING_GRID_COLUMN_REPLY_TM, _T("返回时间"),100);
 	
 	SetTimer(ID_LOAD_PING_EVENT,1000,NULL);

	return 0;
}

void CPingTestView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (m_wndGrid.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);
		m_wndGrid.MoveWindow(rc);
	}
	// TODO: 在此处添加消息处理程序代码
}


void CPingTestView::LoadPingEvent( )
{
	CPingTestDoc *pDoc = (CPingTestDoc *) GetDocument();
	ASSERT(pDoc);
	if (pDoc == NULL)
		return;

	if(m_wndGrid.GetSafeHwnd() == NULL)
		return;

	int nRowCnt = m_wndGrid.GetRowCount();

 	bool bRet = false;
	CBCGPGridRow *pRow = NULL;
	CPingMsg  Msg  ;

	while(nRowCnt > MAX_COMM_MSG_CNT)
	{
		pRow = m_wndGrid.GetHeadRow();
		ASSERT(pRow);
		if (pRow)
		{
			m_wndGrid.RemoveRow(pRow->GetRowId(),FALSE);
			 
			m_wndGrid.RebuildIndexes();		
		}

		nRowCnt = m_wndGrid.GetRowCount();
	}

	bRet = pDoc->m_PingMsgCntnr.GetPingMsg(Msg);

	while(bRet)
	{
	 	//显示出来
		pRow = m_wndGrid.AddMsgRow(&Msg);
		bRet = pDoc->m_PingMsgCntnr.GetPingMsg(Msg);
	}
	if (pRow)
	{
//		m_wndGrid.SetCurSel(pRow);
		m_wndGrid.AdjustLayout();
		m_wndGrid.EnsureVisible(pRow);
	}	 
}

void CPingTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ID_LOAD_PING_EVENT)
	{
		KillTimer(ID_LOAD_PING_EVENT);
		LoadPingEvent( );
		SetTimer(ID_LOAD_PING_EVENT,1000,NULL);
	}

	CView::OnTimer(nIDEvent);
}
