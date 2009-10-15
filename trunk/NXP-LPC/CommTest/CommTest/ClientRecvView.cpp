// ClientRecvView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "ClientRecvView.h"
#include "ClientCommDoc.h"
#include "MainFrm.h"

#include "ChildFrm.h"

static BCGP_GRID_COLOR_DATA sliver_theme = 
{
	-1,	// Grid background color
	RGB (50, 50, 50),	// Grid foreground color
	-1,	// Header foreground color

	{	// Even rows colors:
		RGB (238, 238, 238), -1, -1, 0, -1
	},

	{	// Odd rows colors:
		-1, -1, -1, 0, -1
	},

	{	// Group colors:
		RGB (222, 230, 240), RGB (72, 105, 148), RGB (255, 255, 255), 90, -1
	},

	{	// Selected group colors:
		RGB (117, 150, 188), RGB (249, 250, 252), RGB (183, 200, 220), 90, -1
	},

	{	// Selected colors:
		RGB (175, 194, 217), RGB (52, 77, 108), RGB (222, 230, 240), 90, RGB (175, 194, 217)
	},

	{	// Header item colors:
		RGB (215, 218, 227), -1, RGB (244, 245, 248), 90, RGB (173, 183, 205)
	},

	{	// Selected header item colors:
		RGB (251, 157, 105), -1, RGB (254, 204, 153), 90, RGB (173, 183, 205)
	},

	{	// Left offset colors:
		RGB (215, 218, 227), -1, RGB (244, 245, 248), 0, RGB (173, 183, 205)
	},

	-1,	// Grid horizontal line
	-1,	// Grid vertical line
	-1,	// Description text color
};

typedef enum  PacketGridColumn
{
	PACKET_GRID_COLUMN_ID,  // 序号
	PACKET_GRID_COLUMN_DATE,   // 日期 
	PACKET_GRID_COLUMN_TIME , //时间
	PACKET_GRID_COLUMN_LEN , //长度
	PACKET_GRID_COLUMN_PACKET,    //报文
	PACKET_GRID_COLUMN_COMMENT     //备注,可编辑
};

IMPLEMENT_DYNAMIC(CPacketGridCtrl, CBCGPGridCtrl)
CPacketGridCtrl::CPacketGridCtrl( )
{
}

CPacketGridCtrl::~CPacketGridCtrl( )
{
}

BEGIN_MESSAGE_MAP(CPacketGridCtrl, CBCGPGridCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()
// CRegDefGridCtrl 消息处理程序

int CPacketGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetSingleSel(TRUE);
	SetWholeRowSel (TRUE);
	EnableMarkSortedColumn (FALSE);
	EnableMultipleSort(FALSE);
	EnableHeader (TRUE,BCGP_GRID_HEADER_MOVE_ITEMS); 		 

	CBCGPGridColors theme;
	CBCGPVisualManager::GetInstance ()->OnSetGridColorTheme (this, theme);
	SetColorTheme (sliver_theme);

	InsertColumn (PACKET_GRID_COLUMN_ID, _T("序号"), 70);
	InsertColumn (PACKET_GRID_COLUMN_DATE, _T("日期"), 90);
	InsertColumn (PACKET_GRID_COLUMN_TIME, _T("时间"), 100);
	InsertColumn (PACKET_GRID_COLUMN_LEN, _T("长度"),40);
	InsertColumn (PACKET_GRID_COLUMN_PACKET, _T("报文"),180);
	InsertColumn (PACKET_GRID_COLUMN_COMMENT, _T("备注"),80);
 
	return 0;
} 


CBCGPGridRow* CPacketGridCtrl::CreateNewRow ()
{
	CBCGPGridRow* pRow = CreateRow (GetColumnCount());

	for (int i = 0; i < GetColumnCount ();i++)
	{
		pRow->GetItem (i)->AllowEdit (FALSE);
	}

	return pRow;
}

CBCGPGridRow* CPacketGridCtrl::GetHeadRow ()
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
CBCGPGridRow* CPacketGridCtrl::GetTailRow ()
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


#ifdef _DEBUG
class CBCGPGridRowEx : public CBCGPGridRow
{
	friend CPacketGridCtrl;
};

int CPacketGridCtrl::AddRowEx (CBCGPGridRow* pItem )
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

CBCGPGridRow *CPacketGridCtrl::AddPacketRow ( CPacket *pPacket  )
{
	ASSERT(pPacket);
	if (pPacket == NULL)
		return NULL;
	CBCGPGridRow *pRow = CreateNewRow ();

	pRow->GetItem(PACKET_GRID_COLUMN_ID)->SetValue( pPacket->m_nId );
	CString szTxt;

	szTxt.Format(_T("%d-%02d-%02d"),pPacket->m_stBuild.wYear,pPacket->m_stBuild.wMonth,pPacket->m_stBuild.wDay);
	pRow->GetItem(PACKET_GRID_COLUMN_DATE)->SetValue( (_variant_t)szTxt );

	szTxt.Format(_T("%02d:%02d:%02d.%03d"),pPacket->m_stBuild.wHour,pPacket->m_stBuild.wMinute,pPacket->m_stBuild.wSecond,
		pPacket->m_stBuild.wMilliseconds);
	pRow->GetItem(PACKET_GRID_COLUMN_TIME)->SetValue( (_variant_t)szTxt );

	unsigned int nLen = pPacket->GetPacket(szTxt);
	pRow->GetItem(PACKET_GRID_COLUMN_LEN)->SetValue(  nLen );

	pRow->GetItem(PACKET_GRID_COLUMN_PACKET)->SetValue(  (_variant_t)szTxt );

#ifdef _DEBUG
	AddRowEx(pRow);
#else
	AddRow (pRow, FALSE);
#endif // _DEBUG

	pRow->SetData( (DWORD_PTR) pPacket );

// 	CRect rectRow = pRow->GetRect();
// 
// 	if (rectRow.top < m_rectList.top - 1 || rectRow.bottom > m_rectList.bottom)
// 	{ 
// 		int nNewVertOffset = (rectRow.bottom > m_rectList.bottom) ?
// 			// scroll down
// 			m_nVertScrollOffset + (rectRow.bottom - m_rectList.bottom) :
// 		// scroll up
// 		m_nVertScrollOffset + (rectRow.top - m_rectList.top);
// 
// 		SetScrollPos (SB_VERT, nNewVertOffset*2, FALSE);
// 		OnVScroll (SB_THUMBPOSITION, nNewVertOffset*2, NULL);
// 	}

	return pRow; 
}
// CClientRecvView

IMPLEMENT_DYNCREATE(CClientRecvView, CBCGPFormView)

CClientRecvView::CClientRecvView()
	: CBCGPFormView(CClientRecvView::IDD)
{

}

CClientRecvView::~CClientRecvView()
{
}

void CClientRecvView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientRecvView, CBCGPFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RECV, &CClientRecvView::OnBnClickedButtonClearRecv)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientRecvView 诊断

#ifdef _DEBUG
void CClientRecvView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CClientRecvView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClientRecvView 消息处理程序

const UINT_PTR ID_LOG_PACKET = 2009;

int CClientRecvView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	EnableVisualManagerStyle();

 	CRect rectGrid;
	rectGrid.SetRectEmpty();
	 
	m_wndGrid.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
 
	SetTimer(ID_LOG_PACKET,1000,NULL);


	return 0;
}

void CClientRecvView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码	
	 if ( m_wndGrid.GetSafeHwnd())
	{
		CRect rectClient;
		GetClientRect (rectClient);
		rectClient.InflateRect (2, 2);

		rectClient.top = rectClient.top + 40;
		m_wndGrid.MoveWindow(rectClient);
	}
}

void CClientRecvView::OnBnClickedButtonClearRecv()
{
	// TODO: 在此添加控件通知处理程序代码
	//TEST
//	GetParentFrame()
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	if (pFrame == NULL)
		return;	

	CClientCommDoc *pDoc = (CClientCommDoc *) GetDocument();
	ASSERT(pDoc);
	if (pDoc == NULL)
		return;

	CMDIChildWnd *pChildWnd = NULL;
 
	CCreateContext context;
	context.m_pCurrentDoc = pDoc;
	context.m_pCurrentFrame = pFrame;
	context.m_pNewDocTemplate = (CDocTemplate*) theApp.m_pDocTemplate;

	CRuntimeClass* pFrameClass = RUNTIME_CLASS(CChildFrame);   		 

// 	HMENU hMenu = NULL;       // default menu resource for this frame
// 	HACCEL hAccelTable = NULL;       // accelerator table
// 	hAccelTable = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_FBD_FRAME));
// 	//	ASSERT( hAccelTable );
// 	hMenu = ::LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_FBD_FRAME));
// 	ASSERT(hMenu);
	pChildWnd = pFrame->CreateNewChild( pFrameClass, IDR_MAINFRAME );
//	pChildWnd->SetHandles(hMenu,hAccelTable);

	pFrame->MDIActivate( pChildWnd );

}

void CClientRecvView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ID_LOG_PACKET)
	{
		KillTimer(nIDEvent);
		LoadPacket( );
		SetTimer(ID_LOG_PACKET,1000,NULL);		
	}

	CBCGPFormView::OnTimer(nIDEvent);
}

void CClientRecvView::LoadPacket( )
{
	CClientCommDoc *pDoc = (CClientCommDoc *) GetDocument();
	ASSERT(pDoc);
	if (pDoc == NULL)
		return;
	if(m_wndGrid.GetSafeHwnd() == NULL)
		return;

	int nRowCnt = m_wndGrid.GetRowCount();

	CPacket *pPacket = NULL;
	bool bRet = false;
	CBCGPGridRow *pRow = NULL;

	while(nRowCnt > MAX_PACKET_CNT)
	{
		pRow = m_wndGrid.GetHeadRow();
		ASSERT(pRow);
		if (pRow)
		{
			pPacket = (CPacket*)pRow->GetData();
			ASSERT(pPacket);
			m_wndGrid.RemoveRow(pRow->GetRowId(),FALSE);
			if (pPacket)
			{
				bRet = pDoc->m_PacketCtnr.RemoveData(pPacket);
				ASSERT(bRet);
			}
			m_wndGrid.RebuildIndexes();		
		}

		nRowCnt = m_wndGrid.GetRowCount();
	}

	bRet = pDoc->m_PacketCtnr.GetData(&pPacket);
	 
	while(bRet)
	{
		ASSERT(pPacket);
		//显示出来
		pRow = m_wndGrid.AddPacketRow(pPacket);
		bRet = pDoc->m_PacketCtnr.GetData(&pPacket);
	}

 //	pRow = m_wndGrid.GetTailRow();

	if (pRow)
	{
//		m_wndGrid.SetCurSel(pRow);
		m_wndGrid.AdjustLayout();
		m_wndGrid.EnsureVisible(pRow);
	}	 
}
