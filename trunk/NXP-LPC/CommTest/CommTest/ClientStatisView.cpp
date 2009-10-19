// ClientStatisView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "ClientStatisView.h"



IMPLEMENT_DYNAMIC(CStatisGridCtrlSplitter,CBCGPSplitterWnd)

/////////////////////////////////////////////////////////////////////////////
// CBCGPSplitterWnd

CStatisGridCtrlSplitter::CStatisGridCtrlSplitter()
{
}

CStatisGridCtrlSplitter::~CStatisGridCtrlSplitter()
{
}


BEGIN_MESSAGE_MAP(CStatisGridCtrlSplitter ,CBCGPSplitterWnd)
	//{{AFX_MSG_MAP(CStatisGridCtrlSplitter)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStatisGridCtrlSplitter message handlers 

void CStatisGridCtrlSplitter::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 限制切分条的运动范围。 
	if(point.y< 100 ||point.y> 120) 
	{ 
		CWnd::OnMouseMove(nFlags, point); 
	} 
	else 
	{ 
		CBCGPSplitterWnd::OnMouseMove(nFlags, point); 
	} 

	//	CBCGPSplitterWnd::OnMouseMove(nFlags, point);
}


static BCGP_GRID_COLOR_DATA light_blue_theme = 
{
	-1,	// Grid background color
	-1,	// Grid foreground color
	-1,	// Header foreground color

	{	// Even rows colors:
		RGB (246, 250, 253), -1, -1, 0, -1
	},

	{	// Odd rows colors:
		-1, -1, -1, 0, -1
	},

	{	// Group colors:
		RGB (210, 230, 249), RGB (16, 37, 127), RGB (228, 241, 251), 90, -1
	},

	{	// Selected group colors:
		RGB (250, 199, 97), RGB (0, 0, 0), -1, 0, RGB (192, 192, 192)
	},

	{	// Selected rows colors:
		RGB (255, 229, 153), -1, -1, 0, RGB (192, 192, 192)
	},

	{	// Header item colors:
//		RGB (154, 194, 237), RGB (16, 37, 127), RGB (189, 214, 242), 90, RGB (136, 176, 228)
		RGB (215, 218, 227), -1, RGB (244, 245, 248), 90, RGB (173, 183, 205)
	},

	{	// Selected header item colors:
		-1, -1, -1, 0, -1
	},

	{	// Left offset colors:
//		RGB (207, 221, 240), -1, RGB (227, 234, 244), 90, RGB (136, 176, 228)
		RGB (215, 218, 227), -1, RGB (244, 245, 248), 0, RGB (173, 183, 205)
	},

	-1,	// Grid horizontal line text
	-1,	// Grid vertical line text
	-1,	// Description text color
};
// CStatisGridCtrl

IMPLEMENT_DYNAMIC(CStatisGridCtrl, CBCGPGridCtrl)

CStatisGridCtrl::CStatisGridCtrl(void)
{
}

CStatisGridCtrl::~CStatisGridCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CStatisGridCtrl, CBCGPGridCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()
// CStatisGridCtrl 消息处理程序

int CStatisGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPGridColors theme;
	CBCGPVisualManager::GetInstance ()->OnSetGridColorTheme (this, theme);
	SetColorTheme(light_blue_theme);
	//	SetSingleSel(TRUE);

	SetWholeRowSel (FALSE);
	EnableMarkSortedColumn (FALSE);
	EnableMultipleSort(TRUE);
	EnableHeader (TRUE, BCGP_GRID_HEADER_SORT);
// 
// 	InsertColumn (TAB_COLUMN_IDX, _T("序号"), 60);
// 	InsertColumn (TAB_COLUMN_CODE, _T("编码值"), 120);
// 	//	InsertColumn (TAB_COLUMN_GATAGE, _T("闸门开度(单位：厘米)"), 160);
// 	InsertColumn (TAB_COLUMN_GATAGE, _T("闸门开度"), 160);

	return 0;
} 

CBCGPGridRow* CStatisGridCtrl::CreateNewRow ()
{
	CBCGPGridRow* pRow = CreateRow (GetColumnCount());

	for (int i = 0; i < GetColumnCount ();i++)
	{
		pRow->GetItem (i)->AllowEdit (FALSE);
	}

	return pRow;
}

void CStatisGridCtrl::AddEmptyRow ()
{
	AddRow (CreateNewRow (), TRUE);
}
 


// CGridChartWnd

IMPLEMENT_DYNAMIC(CGridChartWnd, CWnd)

CGridChartWnd::CGridChartWnd():
m_pGrid (NULL)
, m_pChart (NULL)
, m_pParentWnd (NULL)
{

}

CGridChartWnd::~CGridChartWnd()
{
	if (m_pGrid != NULL)
	{
		delete m_pGrid;
	}

	if (m_pChart != NULL)
	{
		delete m_pChart;
	}
}

const CBCGPSplitterWnd* CGridChartWnd::GetSplitter () const
{
	return &m_wndSplitter;
}
//*****************************************************************************
const CStatisGridCtrl* CGridChartWnd::GetGrid () const
{
	return m_pGrid;
}
//*****************************************************************************
const COScopeCtrl* CGridChartWnd::GetChart () const
{
	return m_pChart;
}
//*****************************************************************************
CBCGPSplitterWnd* CGridChartWnd::GetSplitter ()
{
	return &m_wndSplitter;
}
//*****************************************************************************
CStatisGridCtrl* CGridChartWnd::GetGrid ()
{
	return m_pGrid;
}
//*****************************************************************************
COScopeCtrl* CGridChartWnd::GetChart ()
{
	return m_pChart;
}

COScopeCtrl* CGridChartWnd::CreateChart ()
{
	COScopeCtrl* pNewChart = NULL;

	if (m_pParentWnd->GetSafeHwnd () != NULL)
	{
	//	pNewChart = (COScopeCtrl*)m_pParentWnd->SendMessage (BCGM_GANTT_CONTROL_CREATE_CHART, 0, 0);
	}

	if (pNewChart != NULL)
	{
		ASSERT_VALID(pNewChart);
		return pNewChart;
	}
	else
	{
		return new COScopeCtrl;
	}
}
CStatisGridCtrl* CGridChartWnd::CreateGrid ()
{
	CStatisGridCtrl* pNewGrid = NULL;

	if (m_pParentWnd->GetSafeHwnd () != NULL)
	{
	//	pNewGrid = (CBCGPGridCtrl*)m_pParentWnd->SendMessage (BCGM_GANTT_CONTROL_CREATE_GRID, 0, 0);
	}

	if (pNewGrid != NULL)
	{
		ASSERT_VALID(pNewGrid);
		return pNewGrid;
	}
	else
	{
		return new CStatisGridCtrl;
	}
}
BEGIN_MESSAGE_MAP(CGridChartWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_OSCOPEPOSITION, OnOscopePositionMsg)
END_MESSAGE_MAP()



// CCtrlContainer 消息处理程序
LRESULT CGridChartWnd::OnOscopePositionMsg(WPARAM wParam, LPARAM lParam)
{
	CWnd* pwndParent = GetParent();
	if (pwndParent)
		pwndParent->SendMessage(UM_OSCOPEPOSITION, wParam,lParam);
	return TRUE;
}

void CGridChartWnd::OnDestroy()
{
	//	CWnd::OnDestroy();
	if (m_wndSplitter.GetSafeHwnd () != NULL)
	{
		m_wndSplitter.DestroyWindow ();
	}
	// TODO: 在此处添加消息处理程序代码
}

BOOL CGridChartWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}
BOOL CGridChartWnd::Create (DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pParentWnd = pParentWnd;
	m_pGrid = CreateGrid ();
	m_pChart = CreateChart ();

	BOOL bResult = CWnd::Create (globalData.RegisterWindowClass (_T("GridChartCntnr")), _T(""), dwStyle, rect, pParentWnd, nID);
	return bResult && m_pGrid != NULL && m_pChart != NULL;

	//	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CGridChartWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CWnd* pParent = NULL;
	if(lpCreateStruct)
	{
		pParent = CWnd::FromHandle (lpCreateStruct->hwndParent);
	}

	if (pParent != NULL && pParent->IsKindOf (RUNTIME_CLASS (CView)) == NULL)
	{
		return CreateControls ();
	}

	return 0;
}
int CGridChartWnd::CreateControls ()
{
	if (m_wndSplitter.GetSafeHwnd () != NULL)
	{
		return -1;
	}

	if (!m_wndSplitter.CreateStatic (this,2,1))
	{
		ASSERT(FALSE);
		return -1;
	}

	m_wndSplitter.ModifyStyle (WS_HSCROLL | WS_VSCROLL, 0);

	CRect rectDummy (0, 0, 1, 1);
	DWORD dwStyle =  (WS_CHILD | WS_VISIBLE );
	if (!m_pGrid->Create (dwStyle, rectDummy, &m_wndSplitter, m_wndSplitter.IdFromRowCol (0, 0)))
	{
		TRACE0("Failed to create grid control\n");
		ASSERT(FALSE);
		return -1;
	}

	if (!m_pChart->Create (dwStyle, rectDummy,&m_wndSplitter,  m_wndSplitter.IdFromRowCol (1, 0)))
	{
		TRACE0("Failed to create chart control\n");
		ASSERT(FALSE);
		return -1;
	}
 
	CRect rect;
	GetClientRect (rect);

	m_wndSplitter.SetRowInfo (0, rect.Height() / 3, 30);
//	m_wndSplitter.SetWindowPos (NULL, 0, 0, rect.Width (), rect.Height (), SWP_NOZORDER | SWP_NOREDRAW);
 
	return 0;
}
void CGridChartWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (m_wndSplitter.GetSafeHwnd () != NULL)
	{
		m_wndSplitter.SetWindowPos (this, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	}
	// TODO: 在此处添加消息处理程序代码
}
// CClientStatisView

IMPLEMENT_DYNCREATE(CClientStatisView, CView)

CClientStatisView::CClientStatisView()
{

}

CClientStatisView::~CClientStatisView()
{
}

BEGIN_MESSAGE_MAP(CClientStatisView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE(UM_OSCOPEPOSITION, OnOscopePositionMsg)
END_MESSAGE_MAP()


// CClientStatisView 绘图

void CClientStatisView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CClientStatisView 诊断

#ifdef _DEBUG
void CClientStatisView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CClientStatisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClientStatisView 消息处理程序

typedef enum  STAT_COL
{
	STAT_COL_NAME,  // 

	STAT_COL_CUR_SPEED,  // 当前速度
	STAT_COL_AVR_SPEED,   // 平均速度
	STAT_COL_DATA_CNT , //累计长度
	STAT_COL_PACKET_CNT  //累计数据包
};

int CClientStatisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_wndGridChart.Create (WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, 1))
	{
		TRACE0("CBCGPGanttControl::OnCreate: cannot create gantt control\n");
		return -1;
	}
	CStatisGridCtrl*   pGrid =  m_wndGridChart.GetGrid();
	ASSERT(pGrid);
	if (pGrid)
	{
		pGrid->InsertColumn (STAT_COL_NAME, _T("当前速度"), 80);
		pGrid->InsertColumn (STAT_COL_CUR_SPEED, _T("当前速度"), 80);
		pGrid->InsertColumn (STAT_COL_AVR_SPEED, _T("平均速度"), 90);
		pGrid->InsertColumn (STAT_COL_DATA_CNT, _T("累计长度"), 100);
		pGrid->InsertColumn (STAT_COL_PACKET_CNT, _T("累计数据包"),100);	 
		pGrid->AddEmptyRow();
		pGrid->AddEmptyRow();

	}



	return 0;
}

BOOL CClientStatisView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}

void CClientStatisView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (m_wndGridChart.GetSafeHwnd () != NULL)
	{
		m_wndGridChart.SetWindowPos (this, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
	}	
	// TODO: 在此处添加消息处理程序代码

}


void CClientStatisView::OnInitialUpdate()
{
	CView::OnInitialUpdate(); 
 	m_wndGridChart.CreateControls();
	
	m_wndGridChart.GetChart()->SetRange(0, 600, 0) ;
	m_wndGridChart.GetChart()->SetRange(0, 600, 1) ;
	m_wndGridChart.GetChart()->m_nYDecimals = 0;

	m_wndGridChart.GetChart()->SetYUnits(_T("单位(byte)"),_T("0"),_T("1000")) ;
	m_wndGridChart.GetChart()->SetXUnits(_T("数据传输速度 (计算间隔2秒)")) ;
	m_wndGridChart.GetChart()->SetLegendLabel(_T("数据接收"),0);
 	m_wndGridChart.GetChart()->SetLegendLabel(_T("数据发送"),1);

//	m_wndGridChart.GetChart()->SetBackgroundColor(RGB(0, 0, 164)) ;
//	m_wndGridChart.GetChart()->SetGridColor(RGB(192, 192, 255)) ;

	m_wndGridChart.GetChart()->SetGridColor(RGB(0, 0, 0));		// Grid
	m_wndGridChart.GetChart()->SetPlotColor(RGB(0, 255, 0), 0);	//  
	m_wndGridChart.GetChart()->SetPlotColor(RGB(255, 0, 0), 1);	//  
	m_wndGridChart.GetChart()->SetBackgroundColor(RGB(0, 64, 128)) ; 

	m_wndGridChart.GetChart()->SetBarsPlot(true, 1);
	m_wndGridChart.GetChart()->InvalidateCtrl();
// 
 	m_TimeToolTips.Create(this);
	m_TimeToolTips.AddTool(m_wndGridChart.GetChart(),	_T(""),NULL,0);
	m_TimeToolTips.SetDelayTime(TTDT_INITIAL,	30000);
	m_TimeToolTips.SetDelayTime(TTDT_RESHOW,	30000);
	m_TimeToolTips.SetDelayTime(TTDT_AUTOPOP,	300);

	// TODO: 在此添加专用代码和/或调用基类
}
#define ARRSIZE(x)	(sizeof(x)/sizeof(x[0]))
LRESULT CClientStatisView::OnOscopePositionMsg(WPARAM /*wParam*/, LPARAM lParam)
{
	time_t m_tNow= time(NULL)-lParam;

	TCHAR szDate[128] = {0};
	TCHAR szAgo[64] = {0};
	_tcsftime(szDate, ARRSIZE(szDate), _T("  %X  "),  localtime(&m_tNow));

	CString buffer;
	if (lParam<60)
	{
		buffer.Format(_T("%d %s"), lParam, _T("秒"));
	}
	else if (( lParam >60 ) && ( lParam <3600 ))
	{
		//	buffer.Format(_T("%d %s"), lParam, "秒");
		buffer.Format(_T("%d:%d %s"), lParam/60, ((UINT)(lParam - (lParam/60)*60)), _T("分钟"));
	}
	else if (lParam < 86400)
	{
		buffer.Format(_T("%d:%d %s"), lParam/3600, ((UINT)((lParam - (lParam/3600)*3600)/60)), _T("小时"));
	}
	else
	{
		ULONGLONG cntDays = lParam/86400;
		ULONGLONG cntHrs = (lParam - (lParam/86400)*86400)/3600;
		if (cntHrs)
			buffer.Format(_T("%d %s %d:%d %s"), cntDays, _T("天"), cntHrs, ((UINT32)(lParam - (cntDays*86400) - (cntHrs*3600))/60), _T("小时"));
		else
			buffer.Format(_T("%d %s %u %s"), cntDays, _T("天"), (UINT32)(lParam - (cntDays*86400) - (cntHrs*3600))/60,_T("分钟"));
	}

	//	wsprintf(szAgo,_T(" ") + GetResString(IDS_TIMEBEFORE),CastSecondsToLngHM(lParam));
	wsprintf(szAgo,_T("%s %s "),  _T("前"),buffer);
	_tcscat(szDate,szAgo);

	m_TimeToolTips.UpdateTipText(szDate, m_wndGridChart.GetChart());

	m_TimeToolTips.Update();
	return 0;
}
BOOL CClientStatisView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_TimeToolTips.RelayEvent(pMsg);
	return CView::PreTranslateMessage(pMsg);
}
