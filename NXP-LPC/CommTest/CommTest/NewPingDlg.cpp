// NewPingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "NewPingDlg.h"


// CNewPingDlg 对话框

IMPLEMENT_DYNAMIC(CNewPingDlg, CBCGPDialog)

CNewPingDlg::CNewPingDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CNewPingDlg::IDD, pParent)
{

}

CNewPingDlg::~CNewPingDlg()
{
}

void CNewPingDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewPingDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CNewPingDlg 消息处理程序

BOOL CNewPingDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	 
	// TODO:  在此添加额外的初始化
	EnableVisualManagerStyle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
