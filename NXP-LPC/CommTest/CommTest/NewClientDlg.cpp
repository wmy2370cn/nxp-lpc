// NewClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "NewClientDlg.h"


// CNewClientDlg 对话框

IMPLEMENT_DYNAMIC(CNewClientDlg, CBCGPDialog)

CNewClientDlg::CNewClientDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CNewClientDlg::IDD, pParent)
	, m_nDestPort(0)
	, m_nLocalPort(0)
{
	m_dwDestIp = 0;
}

CNewClientDlg::~CNewClientDlg()
{
}

void CNewClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_DEST, m_ctlDestIp);
	DDX_Text(pDX, IDC_EDIT_DEST_PORT, m_nDestPort);
	DDX_Text(pDX, IDC_EDIT_LOCAL_PORT, m_nLocalPort); 
}


BEGIN_MESSAGE_MAP(CNewClientDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CNewClientDlg 消息处理程序

BOOL CNewClientDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	EnableVisualManagerStyle();

	DWORD dwIp = htonl( inet_addr("192.9.200.101"));
	m_ctlDestIp.SetAddress(dwIp);
	m_nDestPort = 505;


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNewClientDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);

 	m_ctlDestIp.GetAddress(m_dwDestIp);

	CBCGPDialog::OnOK();
}
