#pragma once


// CListCtrlEx

class CListCtrlEx : public CBCGPListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

public:
	CListCtrlEx();
	virtual ~CListCtrlEx();
public:
	virtual bool BuildColumns(int nCols, int* nWidth, int* nColString);
	virtual bool BuildColumns(int nCols, int* nWidth, CString* strColString); 

	virtual COLORREF OnGetCellTextColor (int nRow, int nColum);
	virtual COLORREF OnGetCellBkColor (int nRow, int nColum);
	virtual HFONT OnGetCellFont (int nRow, int nColum, DWORD dwData = 0);
	bool SetBackgroundImage( UINT nID, int width = 128, int height = 128);

public:
	BOOL	m_bColor;
	BOOL	m_bModifyFont;

protected:
	DECLARE_MESSAGE_MAP()
};


