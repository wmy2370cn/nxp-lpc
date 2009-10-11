// NewClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "NewClientDlg.h"


// CNewClientDlg 对话框

IMPLEMENT_DYNAMIC(CNewClientDlg, CBCGPDialog)

CNewClientDlg::CNewClientDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CNewClientDlg::IDD, pParent)
{

}

CNewClientDlg::~CNewClientDlg()
{
}

void CNewClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewClientDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CNewClientDlg 消息处理程序

BOOL CNewClientDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	EnableVisualManagerStyle();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
