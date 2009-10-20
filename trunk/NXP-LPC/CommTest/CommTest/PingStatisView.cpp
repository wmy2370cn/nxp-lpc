// PingStatisView.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PingStatisView.h"

static int GRID_WIDTH = 320;
static int PIE_OFFSET = 1;
// CPingStatisView

IMPLEMENT_DYNCREATE(CPingStatisView, CView)

CPingStatisView::CPingStatisView()
{

}

CPingStatisView::~CPingStatisView()
{
}

BEGIN_MESSAGE_MAP(CPingStatisView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPingStatisView 绘图

void CPingStatisView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CPingStatisView 诊断

#ifdef _DEBUG
void CPingStatisView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPingStatisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPingStatisView 消息处理程序

int CPingStatisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rect;
	GetClientRect(rect);

	CRect rcPie = rect;
	rcPie.left = rect.left + GRID_WIDTH + PIE_OFFSET; 

	m_wndPieChart.Create(_T("统计"), rcPie, this, (UINT)-1);

	CRect rcGrid = rect;
	rcGrid.right = rcGrid.left + GRID_WIDTH;

 	//DWORD dwStyle =  (WS_CHILD | WS_VISIBLE );
	m_wndGridCtrl.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rcGrid, this, (UINT)-1);

//	if (m_wndGridCtrl.Create (dwStyle, rcGrid, this, 1))
// 	{
// 		TRACE0("Failed to create grid control\n");
// 		ASSERT(FALSE);
// 		return -1;
// 	}

	m_wndPieChart.SetParentWindowColor(::GetSysColor(COLOR_3DFACE));
	
	m_wndPieChart.SetBackgrndColor(Gdiplus::Color(180,180,180));
	m_wndPieChart.SetLabelBoxColor(Gdiplus::Color(220, 220, 200));

	double dVal = 90;
	Gdiplus::Color Clr[] = {Gdiplus::Color::BlueViolet, Gdiplus::Color::DarkRed};

// 	CString szVal;
// 	CString szName = _T("Item");
// 	for (int i = 0; i < 2; ++i)
// 	{
// 		szVal.Format(_T("%d"), (i + 1));
// 		szName.Append(szVal);
// 
// 		if (dVal > 100)
// 			dVal = dVal / 100;
// 		Gdiplus::Color itemColor(i + 100, i * 120, 20);
// 		m_wndPieChart.InsertItem(szName, szName, dVal,Clr[i % 2]);	 
// 	}
	m_wndPieChart.SetPieChartStyle(CPieChartWnd::ThreeDStyle);
	CString szElem,szLable ;

	szElem.Format(_T("返回 %d"),0);
	dVal = 1;
//	m_wndPieChart.InsertItem(szTxt, szTxt, dVal,Gdiplus::Color::BlueViolet);	 
	return 0;
}

void CPingStatisView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);
	CRect rcPie = rect;
	rcPie.left = rect.left + GRID_WIDTH + PIE_OFFSET; 
	rcPie.InflateRect(1,1);

	if(m_wndPieChart.GetSafeHwnd())
		m_wndPieChart.MoveWindow(rcPie);
	// TODO: 在此处添加消息处理程序代码
	CRect rcGrid = rect;
	rcGrid.right = rcGrid.left + GRID_WIDTH;
	if(m_wndGridCtrl.GetSafeHwnd())
		m_wndGridCtrl.MoveWindow(rcGrid);
}
