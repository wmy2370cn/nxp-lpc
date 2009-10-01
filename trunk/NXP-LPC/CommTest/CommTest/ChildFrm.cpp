
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "CommTest.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CBCGPMDIChildWnd)
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style &= ~WS_SYSMENU;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

void CChildFrame::ActivateFrame(int nCmdShow)
{
	CBCGPMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}

// CChildFrame 消息处理程序
