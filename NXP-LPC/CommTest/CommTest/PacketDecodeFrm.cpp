// PacketDecodeFrm.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PacketDecodeFrm.h"


// CPacketDecodeFrm

IMPLEMENT_DYNCREATE(CPacketDecodeFrm, CBCGPMDIChildWnd)

CPacketDecodeFrm::CPacketDecodeFrm()
{

}

CPacketDecodeFrm::~CPacketDecodeFrm()
{
}


BEGIN_MESSAGE_MAP(CPacketDecodeFrm, CBCGPMDIChildWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CPacketDecodeFrm 消息处理程序

BOOL CPacketDecodeFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style &= ~WS_SYSMENU;
//	cs.style  &= ~FWS_ADDTOTITLE ;

	return CBCGPMDIChildWnd::PreCreateWindow(cs);
}

int CPacketDecodeFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

BOOL CPacketDecodeFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CBCGPMDIChildWnd::OnCreateClient(lpcs, pContext);
}
