// outputbar.cpp : implementation of the COutputBar class
//

#include "stdafx.h"
#include "CommTest.h"
#include "outputbar.h"
#include "Clipboard.h"
#include <locale.h>

#include "LogData.h"
#include "LogDataApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COutputListCtrl::COutputListCtrl()
{
}

COutputListCtrl::~COutputListCtrl()
{
}

BEGIN_MESSAGE_MAP(COutputListCtrl, CListCtrlEx)
//	ON_WM_CONTEXTMENU()
//	ON_COMMAND(ID_OUTPUT_COPY, OnOutputCopy)
	//	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	//	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	//	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
#if 0
void COutputListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CBCGPMDIFrameWnd)))
	{
		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CBCGPFrameWnd*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

#endif

const int nBorderSize = 1;
const int ID_LOG_STR = 1978;

typedef enum  OUTPUT_COL
{
	W_COLUMN_TYPE, //
	W_COLUMN_INDEX, //序号
	W_COLUMN_DATE,
	W_COLUMN_TIME, //时间
	W_COLUMN_STR   // this MUST be the last item in the list
};

static int arColWidths[] = {
	20,
	48,
	98,
	100,
	600
	//	120
};

static CString arColLabels[] = {
	_T("!"), //类型
	_T(" "), //序号
	_T("日期"),
	_T("时间"),
	_T("内容")
};
/////////////////////////////////////////////////////////////////////////////
// COutputBar

BEGIN_MESSAGE_MAP(COutputBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OUTPUT_CLEAR, OnListCtrlClear)
	ON_COMMAND(ID_OUTPUT_COPY, OnListCtrlCopy)
	ON_COMMAND(ID_OUTPUT_CUT, OnListCtrlCut)
	ON_COMMAND(ID_OUTPUT_SAVEAS, OnListCtrlSaveas)
	ON_COMMAND(ID_OUTPUT_AUTOSCROLL,  OnListCtrlAutoscroll)
	ON_UPDATE_COMMAND_UI(ID_OUTPUT_AUTOSCROLL,  OnUpdateListCtrlAutoScroll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar construction/destruction

COutputBar::COutputBar()
{
	// TODO: add one-time construction code here
}

COutputBar::~COutputBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create list window.
	// TODO: create your own window here:
	const DWORD dwViewStyle =	LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	DWORD dwStyle = LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS |	WS_CHILD | WS_VISIBLE|WS_VSCROLL  | WS_HSCROLL | WS_TABSTOP;

	if (!m_wndList.Create (dwStyle, rectDummy,this, 2) )
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}

	m_Font.CreateStockObject(DEFAULT_GUI_FONT);
	m_wndList.SetFont(&m_Font);

	InitListControl();
	SetTimer(ID_LOG_STR,1000,NULL);

	return 0;
}

void COutputBar::InitListControl()
{
	CBitmap bmp;
	bmp.LoadBitmap (IDB_BMP_FLAG);

	m_ilSmall.Create (16, 16, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_ilSmall.Add (&bmp, RGB (255, 0, 255));	

	m_wndList.SetImageList(&m_ilSmall,LVSIL_SMALL);

	// Insert the columns.
	m_wndList.BuildColumns(_countof(arColWidths), arColWidths, arColLabels);

	//ListView_SetExtendedListViewStyle(m_wndList.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyle(m_wndList.m_hWnd, LVS_EX_FULLROWSELECT);

	m_wndList.SetBackgroundImage( IDR_MAINFRAME,48,48);

	// 	m_listCtrl.InsertItem(0, NULL, 0);
	// 	m_listCtrl.SetItemText(0, 3, _T("Click here to add a new task"));

	//	m_wndList.EnableGroupView(TRUE);
}
void COutputBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	m_wndList.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectList;
	m_wndList.GetWindowRect (rectList);
	ScreenToClient (rectList);

	rectList.InflateRect (1, 1);
	dc.Draw3dRect (rectList,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}

void COutputBar::OnDestroy()
{
	KillTimer(ID_LOG_STR);	

	CBCGPDockingControlBar::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void COutputBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ID_LOG_STR)
	{
		KillTimer(ID_LOG_STR);
		LoadDebugStr();		
		SetTimer(ID_LOG_STR,1000,NULL);
	}	

	CBCGPDockingControlBar::OnTimer(nIDEvent);
}
const int DEL_LINE_NUM = 200;
void COutputBar::LoadDebugStr( )
{
	if (m_wndList.GetSafeHwnd() == NULL)
		return;

	int nItemCnt = m_wndList.GetItemCount();
	if (nItemCnt>=MAX_LOG_STR_LINE)
	{
		::SendMessage(m_wndList.m_hWnd, WM_SETREDRAW, FALSE, 0);
		while (nItemCnt >= MAX_LOG_STR_LINE-DEL_LINE_NUM )
		{
			m_wndList.DeleteItem(0);
			nItemCnt = m_wndList.GetItemCount();
		}
		::SendMessage(m_wndList.m_hWnd, WM_SETREDRAW, TRUE, 0);
	}

	CLogStr Data ;

	while(g_LogStrMgr.GetLogStr(&Data))
	{
		DispDebugStr(&Data);		
	} 
} 

int COutputBar::DispDebugStr(CLogStr *pLogData )
{
	ASSERT(pLogData);
	if (pLogData == NULL)
		return -1;
	int nItemCnt = m_wndList.GetItemCount();
	
	nItemCnt = m_wndList.GetItemCount();

	if (nItemCnt<0)
		nItemCnt = 0;

	CString szEntry ;
	if (pLogData->StrType == NORMAL_STR)// 
	{
		m_wndList.InsertItem(nItemCnt,szEntry,1);
	}
	else if ( pLogData->StrType == ALARM_STR ) // 
	{
		m_wndList.InsertItem(nItemCnt,szEntry,4);
		//	m_wndList.SetRowColor(nItemCnt,RGB(255,255,0),RGB(0,255,0));
	}
	else//ERR_STR
	{
		m_wndList.InsertItem(nItemCnt,szEntry,2);
	}

	static INT_PTR nIdx = 0;
	nIdx++;
	if (nIdx>MAX_LOG_STR_LINE)
	{
		nIdx = 1;
	}

	szEntry.Format(_T("%d"),nIdx);
	m_wndList.SetItemText(nItemCnt,W_COLUMN_INDEX,szEntry);

	CString szDate ;
	szDate.Format(_T("%04u-%02u-%02u"),pLogData->ST.wYear,pLogData->ST.wMonth,pLogData->ST.wDay);
	m_wndList.SetItemText(nItemCnt,W_COLUMN_DATE,szDate);

	CString szTime ;
	szTime.Format(_T("%02u:%02u:%02u.%03u"),pLogData->ST.wHour,pLogData->ST.wMinute,pLogData->ST.wSecond ,pLogData->ST.wMilliseconds);
	m_wndList.SetItemText(nItemCnt,W_COLUMN_TIME,szTime);

	m_wndList.SetItemText(nItemCnt,W_COLUMN_STR, pLogData->LogStr );

	if (m_bAutoScroll)
	{
		ListView_EnsureVisible( m_wndList.m_hWnd, ListView_GetItemCount(m_wndList.m_hWnd)-1, FALSE ); 
	}
	return nItemCnt;
} 


void COutputBar::OnContextMenu(CWnd*  pWnd , CPoint point )
{
	// TODO: 在此处添加消息处理程序代码
	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_OUTPUT));
	CMenu* pPopup = NULL;//menu.GetSubMenu(0);

	m_wndList.SetFocus();
	UINT nFlags = 0; 
	CPoint curPoint;
	GetCursorPos(&curPoint);
	m_wndList.ScreenToClient(&curPoint);
	//	HTREEITEM ItemSel = m_wndGrid.HitTest(curPoint, &nFlags);
	pPopup =  menu.GetSubMenu(0);

	ASSERT(pPopup != NULL);

	if (pPopup == NULL)
		return;		

	UINT nResult = 	g_pContextMenuManager->TrackPopupMenu(pPopup->GetSafeHmenu(),point.x,point.y,this);
	SendMessage(WM_COMMAND,nResult);
}

void COutputBar::OnListCtrlClear() 
{
	if (m_wndList.GetSafeHwnd() == NULL)
		return;
	m_wndList.DeleteAllItems( );
}

void COutputBar::OnListCtrlCopy() 
{
	if (m_wndList.GetSafeHwnd() == NULL)
		return;

	//	int nSelItem =m_wndList.GetSelectionMark();//用istat存放当前选定的是第几项
	POSITION pos  = m_wndList.GetFirstSelectedItemPosition( );
	int nSelItem = 0;
	CString szTxt,szTmp;

	if (pos == NULL)
	{
		TRACE0("No items were selected!\n");
		return;
	}
	else
	{
		while (pos)
		{
			nSelItem = m_wndList.GetNextSelectedItem(pos);
			TRACE1("Item %d was selected!\n", nSelItem);
			szTmp = GetItemTxt( nSelItem) ;
			if (!szTmp.IsEmpty())
			{
				if (szTxt.IsEmpty())
				{
					szTxt = szTmp;			
				}
				else
				{
					szTxt = szTxt + "\n" + szTmp;
				}		
			}		
		}
	}

	if (!szTxt.IsEmpty())
		CClipboard::SetText(szTxt);		
}

void COutputBar::OnListCtrlCut() 
{
	if (m_wndList.GetSafeHwnd() == NULL)
		return;

	int nSelItem = 0;
	CString szTxt,szTmp;
	POSITION pos  = m_wndList.GetFirstSelectedItemPosition( );

	CArray <int ,int> arrSelItem;

	if (pos == NULL)
	{
		TRACE0("No items were selected!\n");
		return;
	}
	else
	{
		while (pos)
		{
			nSelItem = m_wndList.GetNextSelectedItem(pos);
			TRACE1("Item %d was selected!\n", nSelItem);
			szTmp = GetItemTxt( nSelItem) ;
			if (szTxt.IsEmpty())
			{
				szTxt = szTmp;			
			}
			else
			{
				szTxt = szTxt + "\n" + szTmp;
			}			
			arrSelItem.Add( nSelItem );
		}
	}

	INT_PTR i = 0;
	INT_PTR nItemCnt =  arrSelItem.GetSize();
	for (i = nItemCnt; i > 0; i--)
	{
		m_wndList.DeleteItem(arrSelItem.GetAt(i-1));
	}

	if (!szTxt.IsEmpty())
		CClipboard::SetText(szTxt);	
}

void COutputBar::OnListCtrlSaveas() 
{
	if (m_wndList.GetSafeHwnd() == NULL)
		return;

	CString  szFileName =_T("c:\\日志信息.txt");
	CFileDialog FileDlg(FALSE, NULL, szFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT , _T("日志文件 (*.txt)|*.txt||"), theApp.m_pMainWnd);
	FileDlg.m_ofn.lpstrTitle = _T("日志信息另存为");

	if (szFileName.GetLength() == 0)
		FileDlg.m_ofn.lpstrInitialDir = _T("C:");

	CString szLogFile;
	if (FileDlg.DoModal() == IDOK)
	{
		szLogFile = FileDlg.GetFileName() ;

		CStdioFile stdFile;
		try
		{
			setlocale( LC_CTYPE,  ("chs"));

			if(!stdFile.Open(szLogFile, CFile::modeWrite|CFile::typeText,NULL))
			{
				if(!stdFile.Open(szLogFile,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyRead|CFile::typeText,NULL))
					return;
			}

			stdFile.SeekToEnd();
			int nCnt = m_wndList.GetItemCount();
			int i = 0;
			CString szTxt;
			CString szTmp;
			for (i = 0; i < nCnt; i++)
			{
				szTmp = m_wndList.GetItemText(i,W_COLUMN_INDEX );
				szTxt = _T("序号：") + szTmp;

				szTmp = m_wndList.GetItemText(i,W_COLUMN_DATE );
				szTxt += _T(" 日期：") + szTmp;

				szTmp = m_wndList.GetItemText(i,W_COLUMN_TIME);
				szTxt += _T(" 时间：") + szTmp;
			 
				szTmp = m_wndList.GetItemText(i,W_COLUMN_STR);
				szTxt += _T(" 内容：") + szTmp;

			 	// 	stdFile.Write(szTxt,szTxt.GetLength());
			 	stdFile.WriteString( szTxt );
			 	szTxt = _T("\n");
			// 	stdFile.Write(szTxt,szTxt.GetLength());
			 	stdFile.WriteString( szTxt );
			}	
	 		stdFile.Close();
		}
		catch(CFileException * pEx)
		{
			LPTSTR  szInfo = _T("Write Err") ;
			pEx->GetErrorMessage(szInfo,128,NULL);
		}
	}			
}

CString COutputBar::GetItemTxt(int nSelItem) 
{
	CString	szTxt ;
	if (nSelItem<0||nSelItem>m_wndList.GetItemCount())
	{
		return szTxt;
	}
	CString szTmp  ;

	szTmp = m_wndList.GetItemText(nSelItem,W_COLUMN_INDEX );
	szTxt = _T("序号：") + szTmp;

	szTmp = m_wndList.GetItemText(nSelItem,W_COLUMN_DATE);
	szTxt += _T(" 日期：") + szTmp;

	szTmp = m_wndList.GetItemText(nSelItem,W_COLUMN_TIME);
	szTxt += _T(" 时间：") + szTmp;

	szTmp = m_wndList.GetItemText(nSelItem,W_COLUMN_STR );
	szTxt += _T(" 内容：") + szTmp;

	return szTxt;
}

void COutputBar::OnListCtrlAutoscroll()
{
	// TODO: 在此添加命令处理程序代码
	m_bAutoScroll  = !m_bAutoScroll;
}

void COutputBar::OnUpdateListCtrlAutoScroll(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_bAutoScroll);
}
