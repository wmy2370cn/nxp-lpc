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

	return (nRow % 2) == 0 ? RGB (253, 241, 249) : RGB (196, 238, 254);
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