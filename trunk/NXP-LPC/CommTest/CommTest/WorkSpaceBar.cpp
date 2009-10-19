// workspace.cpp : implementation of the CWorkSpaceBar class
//

#include "stdafx.h"
#include "CommTest.h"
#include "WorkSpaceBar.h" 
#include "NewClientDlg.h"
#include "ClientCommDoc.h"

#include "NewPingDlg.h"
#include "PingTestDoc.h"

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
	ON_COMMAND(ID_TREE_EXPAND, &CWorkSpaceBar::OnTreeExpand)
	ON_COMMAND(ID_TREE_COLLAPSE, &CWorkSpaceBar::OnTreeCollapse)
	ON_COMMAND(ID_WORKSPACE_NEW, &CWorkSpaceBar::OnNewTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar construction/destruction

CWorkSpaceBar::CWorkSpaceBar()
{
	// TODO: add one-time construction code here
	m_hEth = NULL;
	m_hCom = NULL;
	m_hClientMode = NULL;
	m_hSvrMode = NULL;
	m_hPingMode = NULL;
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
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	InitImages( );

	InitString( );

	// Setup trees content:
	m_hEth = m_wndTree.InsertItem (m_szEth, 0,1);
	m_hClientMode = m_wndTree.InsertItem (m_szClientMode, 0,1,m_hEth);
	m_hSvrMode = m_wndTree.InsertItem (m_szSvrMode,0,1, m_hEth);
	m_hPingMode = m_wndTree.InsertItem (m_szPingMode, 0,1,m_hEth);
	m_hCom = m_wndTree.InsertItem (m_szCom, 0,1);

	ExpandItem( m_wndTree.GetRootItem(), TVE_EXPAND);

	return 0;
}

BOOL  CWorkSpaceBar::InitString( )
{
	BOOL bValid = FALSE;
	bValid =  m_szEth.LoadString(ID_STRING_ETH);
	ASSERT(bValid);

	bValid =  m_szClientMode.LoadString(ID_STRING_CLIENT_MODE);
	ASSERT(bValid);

	bValid =  m_szSvrMode.LoadString(ID_STRING_SVR_MODE);
	ASSERT(bValid);
	
	bValid =  m_szPingMode.LoadString(ID_STRING_PING_MODE);
	ASSERT(bValid);

	bValid =  m_szCom.LoadString(ID_STRING_COM);
	ASSERT(bValid);
 

	return TRUE;
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
	pPopup = menu.GetSubMenu(0);

	HTREEITEM ItemSel = m_wndTree.HitTest(curPoint, &nFlags);
	CString szItem;
	if (ItemSel != NULL)
	{
		m_wndTree.SelectItem(ItemSel);
//		szItem = m_wndTree.GetItemText(ItemSel);
		if(ItemSel == m_hCom || ItemSel == m_hEth)
		{
			pPopup->DeleteMenu (ID_WORKSPACE_NEW,MF_BYCOMMAND );
			pPopup->DeleteMenu (ID_WORKSPACE_DELETE,MF_BYCOMMAND );
			pPopup->DeleteMenu (MF_SEPARATOR,MF_BYCOMMAND );
		}
		else if ( m_wndTree.GetParentItem(ItemSel) == m_hEth || m_wndTree.GetParentItem(ItemSel) == m_hCom)
		{
			pPopup->DeleteMenu (ID_WORKSPACE_DELETE,MF_BYCOMMAND );
		}
	}
	else
	{
		m_wndTree.SelectItem(NULL);
		pPopup->DeleteMenu (ID_WORKSPACE_NEW,MF_BYCOMMAND );
		pPopup->DeleteMenu (ID_WORKSPACE_DELETE,MF_BYCOMMAND );
 		pPopup->DeleteMenu (MF_SEPARATOR,MF_BYCOMMAND );
	}

	pWndTree->SetFocus();

	if (pPopup == NULL)
		return;	

	UINT nResult = 	g_pContextMenuManager->TrackPopupMenu(pPopup->GetSafeHmenu(),point.x,point.y,this);
	SendMessage(WM_COMMAND,nResult);
}
 

void CWorkSpaceBar::OnNewTest()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM ItemSel = m_wndTree.GetSelectedItem();

	CString szItem;

	if (ItemSel== NULL)
		return;
	  
	HTREEITEM hItem = NULL;
	CDocument *pDoc = NULL;
	CFrameWnd* pFrame = NULL;

	if ( m_wndTree.GetParentItem(ItemSel) == m_hClientMode || ItemSel == m_hClientMode)
	{
		CNewClientDlg dlg;
		INT_PTR nRet = dlg.DoModal();
		if (nRet != IDOK)
			return;

		in_addr stuIp;
		memset(&stuIp,0,sizeof(in_addr));
		stuIp.s_addr = htonl (dlg.m_dwDestIp);	
		char szIp[128] = {0};
		strcpy_s(szIp ,64, inet_ntoa(stuIp));
		CString tmp = CA2W (szIp);			 
		szItem.Format(_T("%s:%d"),tmp,dlg.m_nDestPort);

	    hItem = m_wndTree.InsertItem(szItem,103,103,m_hClientMode);
		m_wndTree.Expand(m_hClientMode,TVE_EXPAND);

		CClientCommDoc *pClientCommDoc = NULL;

		if (theApp.m_pClientDocTemplate)
		{
			pDoc = theApp.m_pClientDocTemplate->OpenDocumentFile(NULL);
			pDoc->SetTitle( szItem );

			pClientCommDoc = (CClientCommDoc *)pDoc;
			//保存值到doc里面
			pClientCommDoc->m_nDestPort = dlg.m_nDestPort;
			pClientCommDoc->m_dwDestIp = dlg.m_dwDestIp;
			pClientCommDoc->m_nProtocolType = dlg.m_nProtocolType;
			if (dlg.m_bRandomPort && dlg.m_nLocalPort )
			{
				pClientCommDoc->m_nLocalPort = dlg.m_nLocalPort;
			}
			else
				pClientCommDoc->m_nLocalPort = 0;

			pDoc->UpdateAllViews(NULL,1);
		}
	}
	else if ( m_wndTree.GetParentItem(ItemSel) == m_hPingMode || ItemSel == m_hPingMode)
	{
		CNewPingDlg dlg;
		INT_PTR nRet = dlg.DoModal();
		if (nRet != IDOK)
			return;

		in_addr stuIp;
		memset(&stuIp,0,sizeof(in_addr));
		stuIp.s_addr = htonl (dlg.m_dwDestIp);	
		char szIp[128] = {0};
		strcpy_s(szIp ,64, inet_ntoa(stuIp));
		CString tmp = CA2W (szIp);			 
		szItem.Format(_T("%s-[%d]"),tmp,dlg.m_nPingDataSize);

		hItem = m_wndTree.InsertItem(szItem,83,83,m_hPingMode);
		m_wndTree.Expand(m_hClientMode,TVE_EXPAND);

		CPingTestDoc *pPingDoc = NULL;

		if (theApp.m_pPingDocTemplate)
		{
			pDoc = theApp.m_pPingDocTemplate->OpenDocumentFile(NULL);
			pDoc->SetTitle( szItem );

			pPingDoc = (CPingTestDoc *)pDoc;
			//保存值到doc里面
// 			pPingDoc->m_nDestPort = dlg.m_nDestPort;
// 			pPingDoc->m_dwDestIp = dlg.m_dwDestIp;
// 			pClientCommDoc->m_nProtocolType = dlg.m_nProtocolType;
// 			if (dlg.m_bRandomPort && dlg.m_nLocalPort )
// 			{
// 				pClientCommDoc->m_nLocalPort = dlg.m_nLocalPort;
// 			}
// 			else
// 				pClientCommDoc->m_nLocalPort = 0;

			pDoc->UpdateAllViews(NULL,1);
		}

	}


	ASSERT(pDoc);	 	
	if (pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDoc->GetNextView(pos);
			ASSERT_VALID(pView);
			pFrame = pView->GetParentFrame();
			// assume frameless views are ok to close
			if (pFrame != NULL)
			{
				// assumes 1 document per frame
				ASSERT_VALID(pFrame);
				break;				 
			}
		}
	}
	 
	ASSERT_VALID(pFrame);
	if (pFrame && hItem)
	{
		m_wndTree.SetItemData(hItem,(DWORD_PTR)pFrame);
		m_TreeItemMap.insert( TreeItem_Pair ((DWORD_PTR)pFrame ,hItem) );
	}
	
	m_wndTree.RedrawWindow();  
}

void CWorkSpaceBar::CloseFrame( CWnd *pWnd)
{
	ASSERT_VALID(pWnd);
	if (pWnd == NULL)
		return;

	CTreeItemMap::iterator it =	m_TreeItemMap.find( (DWORD_PTR) pWnd );
	if (it != m_TreeItemMap.end())
	{
		m_wndTree.DeleteItem( (it)->second );

		m_TreeItemMap.erase(it);
		m_wndTree.RedrawWindow();  
	}
}
