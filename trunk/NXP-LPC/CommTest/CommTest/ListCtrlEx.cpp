// ListCtrlEx.cpp : 实现文件
//

#include "stdafx.h"
#include "ListCtrlEx.h"


// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CBCGPListCtrl)

CListCtrlEx::CListCtrlEx()
{
	m_bColor = TRUE;
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CBCGPListCtrl)
END_MESSAGE_MAP()



// CListCtrlEx 消息处理程序



bool CListCtrlEx::BuildColumns(int nCols, int* nWidth, int* nColString)
{
	int i;
	for (i = 0; i < nCols; ++i)
	{
		CString strColumn;
		if (!strColumn.LoadString(nColString[i]))
		{
		}

		if (InsertColumn(i, strColumn, LVCFMT_LEFT, nWidth[i], i) == -1)
		{
			return false;
		}
	}

	return true;
}

bool CListCtrlEx::BuildColumns(int nCols, int* nWidth, CString* strColString)
{
	int i;
	for (i = 0; i < nCols; ++i)
	{
		if (InsertColumn(i, strColString[i], LVCFMT_LEFT, nWidth[i], i) == -1)
		{
			return false;
		}
	}

	return true;
}


COLORREF CListCtrlEx::OnGetCellTextColor (int nRow, int nColum)
{
	if (!m_bColor)
	{
		return CBCGPListCtrl::OnGetCellTextColor (nRow, nColum);
	}
	return CBCGPListCtrl::OnGetCellTextColor (nRow, nColum);
	return (nRow % 2) == 0 ? RGB (128, 37, 0) : RGB (0, 0, 0);
}

COLORREF CListCtrlEx::OnGetCellBkColor (int nRow, int nColum)
{
	if (!m_bColor)
	{
		return CBCGPListCtrl::OnGetCellBkColor (nRow, nColum);
	}

	if (m_bMarkSortedColumn && nColum == m_iSortedColumn)
	{
		return (nRow % 2) == 0 ? RGB (233, 221, 229) : RGB(235, 235, 255);
	}

	return (nRow % 2) == 0 ? RGB (242, 248, 255) : RGB (255, 254, 254);
}

HFONT CListCtrlEx::OnGetCellFont (int nRow, int nColum, DWORD /*dwData = 0*/)
{
	if (!m_bModifyFont)
	{
		return NULL;
	}

// 	if (nColum == 2 && (nRow >= 4 && nRow <= 8))
// 	{
// 		return globalData.fontDefaultGUIBold;
// 	}

	return NULL;
}


bool CListCtrlEx::SetBackgroundImage( UINT nID, int width /* = 128 */, int height /* = 128 */)
{
	HWND hListCtrl = this->GetSafeHwnd();
	ListView_SetTextBkColor(hListCtrl, CLR_NONE);
	COLORREF bkColor = ListView_GetBkColor(hListCtrl);
	// create a bitmap from the icon
	HICON hIcon = (HICON)LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nID), IMAGE_ICON, width, height, LR_DEFAULTCOLOR);
	if (!hIcon)
		return false;

	RECT rect = {0};
	rect.right = width;
	rect.bottom = height;
	HBITMAP bmp = NULL;

	HWND desktop = ::GetDesktopWindow();
	if (desktop)
	{
		HDC screen_dev = ::GetDC(desktop);
		if (screen_dev)
		{
			// Create a compatible DC
			HDC dst_hdc = ::CreateCompatibleDC(screen_dev);
			if (dst_hdc)
			{
				// Create a new bitmap of icon size
				bmp = ::CreateCompatibleBitmap(screen_dev, rect.right, rect.bottom);
				if (bmp)
				{
					// Select it into the compatible DC
					HBITMAP old_dst_bmp = (HBITMAP)::SelectObject(dst_hdc, bmp);
					// Fill the background of the compatible DC with the given color
					::SetBkColor(dst_hdc, bkColor);
					::ExtTextOut(dst_hdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);

					// Draw the icon into the compatible DC
					::DrawIconEx(dst_hdc, 0, 0, hIcon, rect.right, rect.bottom, 0, NULL, DI_NORMAL);
					::SelectObject(dst_hdc, old_dst_bmp);
				}
				::DeleteDC(dst_hdc);
			}
		}
		::ReleaseDC(desktop, screen_dev); 
	}

	// Restore settings
	DestroyIcon(hIcon);

	if (bmp == NULL)
		return false;

	LVBKIMAGE lv;
	lv.ulFlags = LVBKIF_TYPE_WATERMARK;
	lv.hbm = bmp;
	lv.xOffsetPercent = 100;
	lv.yOffsetPercent = 100;
	ListView_SetBkImage(hListCtrl, &lv);
	return true;
}
