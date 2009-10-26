// NewSvrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "NewSvrDlg.h"
#include "UEServer.h"

// CNewSvrDlg 对话框

IMPLEMENT_DYNAMIC(CNewSvrDlg, CBCGPDialog)

CNewSvrDlg::CNewSvrDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CNewSvrDlg::IDD, pParent)
	, m_nSvrPortNum(0)
{

}

CNewSvrDlg::~CNewSvrDlg()
{
}

void CNewSvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SVR_PORTNUM, m_nSvrPortNum);
}


BEGIN_MESSAGE_MAP(CNewSvrDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CNewSvrDlg 消息处理程序

BOOL CNewSvrDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle();

	std::list<std::string> ip_list;
	bool bRet = false;

	bRet = CUEServer::GetIpList(ip_list);


	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
