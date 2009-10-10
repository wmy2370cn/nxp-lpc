
// PingChildFrm.cpp : CPingChildFrame 类的实现
//

#include "stdafx.h"
#include "CommTest.h"

#include "PingChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPingChildFrame

IMPLEMENT_DYNCREATE(CPingChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CPingChildFrame, CBCGPMDIChildWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CPingChildFrame 构造/析构

CPingChildFrame::CPingChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CPingChildFrame::~CPingChildFrame()
{
}


BOOL CPingChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style &= ~WS_SYSMENU;
//	cs.style  &= ~FWS_ADDTOTITLE ;
	return TRUE;
}

// CPingChildFrame 诊断

#ifdef _DEBUG
void CPingChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CPingChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

void CPingChildFrame::ActivateFrame(int nCmdShow)
{
	CBCGPMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}

// CPingChildFrame 消息处理程序

int CPingChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

//	SetTitle(_T("xxxxxx"));
// 	SetWindowText(_T("xxxxxx"));
// 	CDocument *pDoc = GetActiveDocument();
	

	return 0;
}
