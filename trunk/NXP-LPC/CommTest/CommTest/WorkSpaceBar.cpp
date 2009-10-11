// workspace.cpp : implementation of the CWorkSpaceBar class
//

#include "stdafx.h"
#include "CommTest.h"
#include "WorkSpaceBar.h" 
#include "NewClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;
/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar

BEGIN_MESSAGE_MAP(CWorkSpaceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkSpaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WORKSPACE_NEW, &CWorkSpaceBar::OnNewTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar construction/destruction

CWorkSpaceBar::CWorkSpaceBar()
{
	// TODO: add one-time construction code here

}

CWorkSpaceBar::~CWorkSpaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar message handlers

int CWorkSpaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}
	InitImages( );

	// Setup trees content:
	HTREEITEM hNet = m_wndTree.InsertItem (_T("以太网"), 0,1);
	m_wndTree.InsertItem (_T("客户端模式"), 0,1,hNet);
	m_wndTree.InsertItem (_T("服务器模式"),0,1, hNet);

	HTREEITEM hPing = m_wndTree.InsertItem (_T("PING"), 0,1);

	HTREEITEM hCom = m_wndTree.InsertItem (_T("串口"), 0,1);
	 

	return 0;
}
// 加载视图图像:
int CWorkSpaceBar::InitImages( )
{
	m_Images.DeleteImageList ();

	UINT uiBmpId = IDB_BITMAP_LIST;

	CBitmap bmp;
	if (!bmp.LoadBitmap (uiBmpId))
	{
		TRACE(_T ("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT (FALSE);
		return -1;
	}

	BITMAP bmpObj;
	bmp.GetBitmap (&bmpObj);	
	UINT nFlags = ILC_MASK;	
	nFlags |= ILC_COLOR32 ;

	m_Images.Create (16, bmpObj.bmHeight, nFlags, 0, 0);
	m_Images.Add (&bmp,RGB (255, 0, 255) );

	m_wndTree.SetImageList (&m_Images, TVSIL_NORMAL);
	return 0;
} 

void CWorkSpaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkSpaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}


void CWorkSpaceBar::OnTreeExpand()
{
	HTREEITEM hItemSel = m_wndTree.GetSelectedItem();	
	if (hItemSel!=NULL)
	{
		ExpandItem(hItemSel,TVE_EXPAND);
	}
}

void CWorkSpaceBar::OnTreeCollapse()
{
	HTREEITEM hItemSel = m_wndTree.GetSelectedItem();	
	if (hItemSel!=NULL)
	{
		ExpandItem(hItemSel,TVE_COLLAPSE);
	}
} 
void  CWorkSpaceBar::ExpandItem(HTREEITEM hItem,UINT nCode)
{
	m_wndTree.Expand(hItem,nCode);
	if (m_wndTree.ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_wndTree.GetChildItem(hItem);

		while (hChildItem != NULL)
		{
			hNextItem = m_wndTree.GetNextItem(hChildItem,  TVGN_NEXT);				
			ExpandItem(hChildItem,nCode);		
			hChildItem = hNextItem;
		}
	}
}
void CWorkSpaceBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CBCGPDockingControlBar::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	UINT nFlags = 0;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	m_wndTree.ScreenToClient(&curPoint);

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_WORKSPACE));
	CMenu* pPopup = NULL;

	HTREEITEM ItemSel = m_wndTree.HitTest(curPoint, &nFlags);
	CString szItem;
	if (ItemSel != NULL)
	{
		m_wndTree.SelectItem(ItemSel);
		szItem = m_wndTree.GetItemText(ItemSel);
	}
	else
	{
		m_wndTree.SelectItem(NULL);
	}

	pPopup = menu.GetSubMenu(0);

// 	if (szItem == m_szDeviceInfo )
// 	{
// 		pPopup = menu.GetSubMenu(DEVICE_MENU);
// 		ASSERT(pPopup);
// 		//		pPopup->DeleteMenu (ID_PROJECT_EXPORT,MF_BYCOMMAND );
// 	}
// 	else if( szItem == m_szRTData )
// 	{
// 		pPopup = menu.GetSubMenu(RT_DATA_MENU);
// 		ASSERT(pPopup);
// 	}

	pWndTree->SetFocus();

	if (pPopup == NULL)
		return;	

	UINT nResult = 	g_pContextMenuManager->TrackPopupMenu(pPopup->GetSafeHmenu(),point.x,point.y,this);
	SendMessage(WM_COMMAND,nResult);
}
 

void CWorkSpaceBar::OnNewTest()
{
	// TODO: 在此添加命令处理程序代码
	CPoint curPoint;
	UINT nFlags = 0;
	HTREEITEM ItemSel = m_wndTree.HitTest(curPoint, &nFlags);
	CString szItem;


	CNewClientDlg dlg;
	INT_PTR nRet = dlg.DoModal();

	if (nRet != IDOK)
		return;

	if (theApp.m_pClientDocTemplate)
	{
		CDocument *pDoc = theApp.m_pClientDocTemplate->OpenDocumentFile(NULL);
		if (pDoc)
		{
			POSITION pos = pDoc->GetFirstViewPosition();
			while (pos != NULL)
			{
				CView* pView = pDoc->GetNextView(pos);
				ASSERT_VALID(pView);
				CFrameWnd* pFrame = pView->GetParentFrame();
				// assume frameless views are ok to close
				if (pFrame != NULL)
				{
					// assumes 1 document per frame
					ASSERT_VALID(pFrame);
					break;				 
				}
			}
		}
	}
}
