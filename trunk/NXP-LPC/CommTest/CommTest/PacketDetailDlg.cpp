// PacketDetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PacketDetailDlg.h"
#include "Packet.h"


typedef enum  PACKET_COL
{
	PACKET_COLUMN_IDX,  //名称
	PACKET_COLUMN_HEX,  // 
	PACKET_COLUMN_DEC ,
	PACKET_COLUMN_ASC
};
// CPacketDetailDlg 对话框

IMPLEMENT_DYNAMIC(CPacketDetailDlg, CBCGPDialog)

CPacketDetailDlg::CPacketDetailDlg(CPacket *pPacket,CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CPacketDetailDlg::IDD, pParent)
	,m_pPacket(pPacket)
	, m_nId(0)
{
	ASSERT(pPacket);
	ASSERT(	m_pPacket);

}

CPacketDetailDlg::~CPacketDetailDlg()
{
}

void CPacketDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PACKET_ID, m_nId);
}


BEGIN_MESSAGE_MAP(CPacketDetailDlg, CBCGPDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPacketDetailDlg 消息处理程序

const int GRID_TOP = 24;
BOOL CPacketDetailDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle( );

	// TODO:  在此添加额外的初始化

	m_nId = m_pPacket->m_nId;

	CRect rectGrid;
 	GetClientRect (&rectGrid);
 	rectGrid.top += GRID_TOP;
	m_wndGridTree.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);

	m_wndGridTree.EnableColumnAutoSize (TRUE);
	m_wndGridTree.SetSingleSel (FALSE);
	m_wndGridTree.EnableGroupByBox (FALSE);
	m_wndGridTree.SetReadOnly ();
	m_wndGridTree.SetWholeRowSel ();
	m_wndGridTree.EnableHeader (TRUE, 0);

	CBitmap bmp;
	bmp.LoadBitmap (IDB_BITMAP_LIST);

	m_Images.Create (16, 16, ILC_COLOR32 | ILC_MASK, 0, 0);
	m_Images.Add (&bmp, RGB (255, 0, 255));
	m_wndGridTree.SetImageList (&m_Images);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_wndGridTree.SetColorTheme (colors);

	m_wndGridTree.InsertColumn (PACKET_COLUMN_IDX, _T(" "), 120);
	m_wndGridTree.InsertColumn (PACKET_COLUMN_HEX, _T("十六进制"), 130);
	m_wndGridTree.InsertColumn (PACKET_COLUMN_DEC, _T("十进制"), 130);
	m_wndGridTree.InsertColumn (PACKET_COLUMN_ASC, _T("ASC"), 130);

	UpdateTree();

	CWnd *pWnd = GetDlgItem(IDC_STATIC_PACKET_TIME);
	ASSERT(pWnd);
	if(pWnd)
	{
		CString szTxt;
		szTxt.Format( _T("%d-%02d-%02d %02d:%02d:%02d.%03d"),m_pPacket->m_stBuild.wYear,
			m_pPacket->m_stBuild.wMonth,m_pPacket->m_stBuild.wDay,m_pPacket->m_stBuild.wHour,
			m_pPacket->m_stBuild.wMinute,m_pPacket->m_stBuild.wSecond,m_pPacket->m_stBuild.wMilliseconds);
		pWnd->SetWindowText(szTxt);		
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CPacketDetailDlg::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_wndGridTree.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);
		rc.top += GRID_TOP;
		rc.InflateRect(1,1);
		m_wndGridTree.MoveWindow(rc);
	}
}

CBCGPGridRow* CPacketDetailDlg::CreateNewRow ()
{
	CBCGPGridRow* pRow = m_wndGridTree. CreateRow (m_wndGridTree.GetColumnCount ());

	for (int i = 0; i < m_wndGridTree.GetColumnCount ();i++)
	{
		pRow->GetItem (i)->AllowEdit (FALSE);
	}

	return pRow;
}

void  CPacketDetailDlg:: UpdateTree( )
{
	if (m_wndGridTree.GetSafeHwnd() == NULL)
		return;
	ASSERT(m_pPacket);
	if(m_pPacket == NULL)
		return;

	CBCGPGridRow* pRootRow = CreateNewRow ( );
	ASSERT_VALID (pRootRow);
	pRootRow->AllowSubItems ();

	unsigned char *pData = NULL;

	unsigned int nLen = 0;
	pData = m_pPacket->GetPacket(nLen);

	if (pData && nLen && nLen <= MAX_PACKET_LEN)
	{
		CString szTxt;
		szTxt.Format(_T("长度：%d"),nLen);
		pRootRow->GetItem (PACKET_COLUMN_IDX)->SetValue ( _variant_t (szTxt) ,FALSE);
		pRootRow->GetItem (PACKET_COLUMN_IDX)->SetImage (32,FALSE);

		m_wndGridTree.AddRow (pRootRow, FALSE);

		unsigned int i  = 0;
		for (i = 0 ;i <nLen; i++)
		{
			//版本信息
			CBCGPGridRow* pDataRow = CreateNewRow ( );
			ASSERT_VALID(pDataRow);
			pDataRow->GetItem (PACKET_COLUMN_IDX)->SetImage (35,FALSE);

			szTxt.Format(_T("%d"),i+1);
			pDataRow->GetItem (PACKET_COLUMN_IDX)->SetValue ( _variant_t (szTxt),FALSE);

	 		szTxt.Format(_T("%02X"),pData[i]);
			pDataRow->GetItem (PACKET_COLUMN_HEX)->SetValue ( _variant_t (szTxt),FALSE);
		 		
			szTxt.Format(_T("%02d"),pData[i]);
			pDataRow->GetItem (PACKET_COLUMN_DEC)->SetValue ( _variant_t (szTxt),FALSE);

			szTxt.Format(_T("%C"),pData[i]);
			pDataRow->GetItem (PACKET_COLUMN_ASC)->SetValue ( _variant_t (szTxt),FALSE);

		 	pRootRow->AddSubItem (pDataRow, FALSE);
		}
		m_wndGridTree.AdjustLayout();
	}
}
