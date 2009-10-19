#include "stdafx.h"
#include "xtDataType.h"
#include "xtDefines.h"
#include <math.h>
#include "XTColorRef.h"
#include "resource.h"
//===========================================================================
// CXTPPenDC class
//===========================================================================
CXTPPenDC::CXTPPenDC(HDC hDC, COLORREF crColor)
: m_hDC (hDC)
{
	VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
	m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

CXTPPenDC::~CXTPPenDC ()
{
	::SelectObject (m_hDC, m_hOldPen);
}

void CXTPPenDC::Color(COLORREF crColor)
{
	::SelectObject (m_hDC, m_hOldPen);
	VERIFY(m_pen.DeleteObject());
	VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
	m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

COLORREF CXTPPenDC::Color()
{
	LOGPEN logPen;
    m_pen.GetLogPen(&logPen);
    return logPen.lopnColor;
}


//===========================================================================
// CXTPClientRect class
//===========================================================================
CXTPClientRect::CXTPClientRect(HWND hWnd)
{
	::GetClientRect(hWnd, this);
}

CXTPClientRect::CXTPClientRect(const CWnd* pWnd)
{
	::GetClientRect(pWnd->GetSafeHwnd(), this);
}

//===========================================================================
// CXTPBufferDC class
//===========================================================================
CXTPBufferDC::CXTPBufferDC(HDC hDestDC, const CRect rcPaint) : m_hDestDC (hDestDC)
{
	m_rect = rcPaint;
	Attach (::CreateCompatibleDC (m_hDestDC));
	m_bitmap.Attach (::CreateCompatibleBitmap(
		m_hDestDC, m_rect.right, m_rect.bottom));
	m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);
}

CXTPBufferDC::~CXTPBufferDC()
{
	::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(),
		m_rect.Height(), m_hDC, m_rect.left, m_rect.top, SRCCOPY);
    ::SelectObject (m_hDC, m_hOldBitmap);
}

CDC* CXTPBufferDC::GetDestDC() 
{
	return CDC::FromHandle(m_hDestDC);
}


// XTPColorManager类
//===========================================================================
// XTP_GRADIENT_COLOR struct
//===========================================================================
void XTP_GRADIENT_COLOR::SetColors(COLORREF _clrLight, COLORREF _clrDark)
{
	clrLight = _clrLight;
	clrDark = _clrDark;
}

const XTP_GRADIENT_COLOR& XTP_GRADIENT_COLOR::operator= (const COLORREF& clr)
{
	clrLight = clrDark = clr;
	return *this;
}

//===========================================================================
// CXTPColorManager class
//===========================================================================
#pragma warning( disable  : 4244 ) 

CXTPColorManager CXTPColorManager::m_instance;

#define clrToolBar GetColor(XPCOLOR_TOOLBAR_FACE)
#define clrWindow GetColor(COLOR_WINDOW)
#define clrHighlight GetColor(COLOR_HIGHLIGHT)
#define pow2(x) pow(x, 2) 
#define NORMVALUE(a, max) a = (a < 0) ? 0 : (a > max) ? max : a
//#define UNIQUEVALUE _T("4718SY1ZUZCY2XS5T9ZKWS3QGQKD8QQ")

CXTPColorManager::CXTPColorManager()
{
	m_bDisableLunaColors = FALSE;
	m_bInit = FALSE;
	RefreshColors();
}

float CXTPColorManager::ColorWidth(int nLength, int nWidth)
{
	if (nLength > 220) nLength = 220;
	return (2.0 - (float)nLength / 220) * nWidth;
}


float CXTPColorManager::ColorDelta(COLORREF clrA, COLORREF clrB)
{	
	return pow2(GetRDelta(clrA) - GetRDelta(clrB)) + 
		pow2(GetGDelta(clrA) - GetGDelta(clrB)) +
		pow2(GetBDelta(clrA) - GetBDelta(clrB));
}

float CXTPColorManager::Length(COLORREF clrA, COLORREF clrB)
{
	return sqrt(ColorDelta(clrA, clrB));
}

BOOL CXTPColorManager::LongColor(COLORREF clrMain, COLORREF clrSub, BOOL bCalcLength, float fDistance)
{
	if (bCalcLength)
		fDistance =  ColorWidth(Length(clrMain, 0), fDistance);

	return pow2(fDistance) < ColorDelta(clrMain, clrSub);
}

COLORREF CXTPColorManager::MixColor(COLORREF clrMain, COLORREF clrSub, float fDistance)
{
	float fMainLength =  Length(clrMain, 0);

	float fGate = ColorWidth(fMainLength, fDistance);

	BOOL bReverse = TRUE;

	if (Length(clrSub, 0) > fMainLength)
	{
		if (fMainLength > 442 - fGate) 
			bReverse = FALSE;

	}
	else
	{
		if (fMainLength > fGate)  
			bReverse = FALSE;		
	}

	float fSubRed = GetRDelta(clrSub);
	float fSubGreen = GetGDelta(clrSub);
	float fSubBlue= GetBDelta(clrSub);

	float fMainRed = GetRDelta(clrMain);
	float fMainGreen = GetGDelta(clrMain);
	float fMainBlue= GetBDelta(clrMain);

	if (bReverse)
	{
		fSubRed = 195.0 - fSubRed;
		fSubGreen = 390.0 - fSubGreen;
		fSubBlue = 65.0 - fSubBlue;

		fMainRed = 195.0 - fMainRed;
		fMainGreen = 390.0 - fMainGreen;
		fMainBlue = 65.0 - fMainBlue;
	}		 

	float A = __max(0.001, sqrt(pow2(fSubRed) + pow2(fSubGreen) + pow2(fSubBlue)));
	float B = fMainRed * fSubRed + fMainGreen * fSubGreen + fMainBlue * fSubBlue;
	float X = B / pow2(A);

	float fR = X * fSubRed - fMainRed;
	float fG = X * fSubGreen - fMainGreen;
	float fB = X * fSubBlue - fMainBlue;

	float Y = sqrt(pow2(fGate) - (pow2(fR) + pow2(fG) + pow2(fB)));

	float Z =(B / A - Y);

	fR = Z * fSubRed / A;
	fG = Z * fSubGreen / A;
	fB = Z * fSubBlue / A;

	if (bReverse)
	{
		fR = 195.0 - fR;
		fG = 390.0 - fG;
		fB = 65.0  - fB;
	}

	NORMVALUE(fR, 195.0);
	NORMVALUE(fG, 390.0);
	NORMVALUE(fB, 65.0);

	int nR = .5 + fR * (255.0/195.0);
	int nG = .5 + fG * (255.0/390.0);
	int nB = .5 + fB * (255.0/65.0);

	return RGB(NORMVALUE(nR, 255), NORMVALUE(nG, 255), NORMVALUE(nB, 255));
}

COLORREF CXTPColorManager::AdjustColor(COLORREF clrMain, COLORREF clrSub, float fDistance)
{
	float Z = sqrt(
		pow2(GetRDelta(clrMain) - GetRDelta(clrSub)) + 
		pow2(GetGDelta(clrMain) - GetGDelta(clrSub)) +
		pow2(GetBDelta(clrMain) - GetBDelta(clrSub)));

	float Q = (Z - ColorWidth(Length(0, clrMain), fDistance)) / Z;

	float fR = Q * (GetRDelta(clrMain) - GetRDelta(clrSub)) + GetRDelta(clrSub);
	float fG = Q * (GetGDelta(clrMain) - GetGDelta(clrSub)) + GetGDelta(clrSub);
	float fB = Q * (GetBDelta(clrMain) - GetBDelta(clrSub)) + GetBDelta(clrSub);

	NORMVALUE(fR, 195.0);
	NORMVALUE(fG, 390.0);
	NORMVALUE(fB, 65.0);

	int nR = .5 + fR * (255.0/195.0);
	int nG = .5 + fG * (255.0/390.0);
	int nB = .5 + fB * (255.0/65.0);

	return RGB(NORMVALUE(nR, 255), NORMVALUE(nG, 255), NORMVALUE(nB, 255));
}

COLORREF CXTPColorManager::LightColor(COLORREF clrLight, COLORREF clrDark, int nDelta)
{
	int nParam = (nDelta < 100 ? 100 : 1000);

	int nR = (GetRValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetRValue(clrLight) * nDelta) / nParam;
	int nG = (GetGValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetGValue(clrLight) * nDelta) / nParam;
	int nB = (GetBValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetBValue(clrLight) * nDelta) / nParam;

	return RGB(nR, nG, nB);
}

COLORREF CXTPColorManager::GetColor(int nIndex)
{
	if (nIndex > XPCOLOR_LAST) 
		return nIndex;
	return m_arrColor[nIndex];
}

void CXTPColorManager::RefreshSysColors()
{
	m_arrColor[0] = 0;
	int i;
	for (i = 1; i < 28; i++) m_arrColor[i] = GetSysColor(i);
}

void CXTPColorManager::RefreshXPColors()
{
	m_arrColor[XPCOLOR_TOOLBAR_FACE]             = GetColor(COLOR_3DFACE);             
	m_arrColor[XPCOLOR_HIGHLIGHT]                = GetColor(COLOR_WINDOW);             
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED]         = GetColor(COLOR_HIGHLIGHT);             
	m_arrColor[XPCOLOR_MENUBAR_FACE]             = GetColor(COLOR_WINDOW);             
	m_arrColor[XPCOLOR_GRAYTEXT]                 = GetColor(COLOR_GRAYTEXT);             
	m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]          = GetColor(COLOR_3DSHADOW);             
	m_arrColor[XPCOLOR_SEPARATOR]                = GetColor(COLOR_3DSHADOW);             
	m_arrColor[XPCOLOR_MENUBAR_BORDER]           = GetColor(COLOR_BTNSHADOW);             
	m_arrColor[XPCOLOR_DISABLED]                 = GetColor(COLOR_3DSHADOW);             
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED]        = GetColor(COLOR_WINDOW);             
	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER]         = GetColor(COLOR_HIGHLIGHT);
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED_BORDER] = GetColor(COLOR_HIGHLIGHT);
	m_arrColor[XPCOLOR_MENUBAR_TEXT]             = GetColor(COLOR_WINDOWTEXT);
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT]           = GetColor(COLOR_MENUTEXT); 
	m_arrColor[XPCOLOR_TOOLBAR_TEXT]             = GetColor(COLOR_BTNTEXT);        
	m_arrColor[XPCOLOR_PUSHED_TEXT]              = GetColor(COLOR_HIGHLIGHTTEXT);  
	m_arrColor[XPCOLOR_TAB_INACTIVE_BACK]        = GetColor(COLOR_3DFACE);
	m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT]        = GetColor(COLOR_BTNTEXT);
	m_arrColor[XPCOLOR_MENUBAR_EXPANDED]         = GetColor(COLOR_3DSHADOW);

	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER]  = GetColor(COLOR_HIGHLIGHT);
	m_arrColor[XPCOLOR_TAB_INACTIVE_BACK]        = GetColor(COLOR_3DFACE);
	m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT]        = GetColor(COLOR_BTNTEXT);
	m_arrColor[XPCOLOR_ICONSHADDOW]              = GetColor(COLOR_3DSHADOW);
	m_arrColor[XPCOLOR_3DFACE]                   = GetColor(COLOR_3DFACE);
	m_arrColor[XPCOLOR_3DSHADOW]                 = GetColor(COLOR_3DSHADOW);
	m_arrColor[XPCOLOR_EDITCTRLBORDER]			 = GetColor(COLOR_3DFACE);  
	m_arrColor[XPCOLOR_FRAME]					 = GetColor(COLOR_3DSHADOW);  
	m_arrColor[XPCOLOR_SPLITTER_FACE]			 = GetColor(COLOR_3DFACE);  
}

void CXTPColorManager::RefreshGradientColors()
{
	// 因为TabView中使用的TabControl不具有2003的风格,所以这里我随便设置一个风格
//	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();
	XTPCurrentSystemTheme systemTheme = xtpSystemThemeBlue;
	grcCaption.SetColors(m_arrColor[COLOR_3DSHADOW], m_arrColor[COLOR_3DSHADOW]);
	grcDockBar.SetColors(m_arrColor[COLOR_3DFACE], XTPColorManager()->LightColor(GetColor(COLOR_3DFACE), GetColor(COLOR_WINDOW), 0xcd));
	grcShortcutBarGripper.SetColors(m_arrColor[COLOR_3DFACE], m_arrColor[COLOR_3DSHADOW]);
	
	if (!m_bDisableLunaColors)
	{
		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			{			
				m_arrColor[XPCOLOR_FRAME] = RGB(0, 45, 150);
				grcCaption.SetColors(RGB(89, 135, 214), RGB(0, 45, 150));
				grcDockBar.SetColors(RGB(158, 190, 245), RGB(196, 218, 250));
				grcShortcutBarGripper.SetColors(RGB(89, 135, 214), RGB(0, 45, 150));
			}
			break;
			
		case xtpSystemThemeOlive:
			{
				m_arrColor[XPCOLOR_FRAME] = RGB(96, 128, 88);;
				grcCaption.SetColors(RGB(175, 192, 130), RGB(99, 122, 68));
				grcDockBar.SetColors(RGB(217, 217, 167), RGB(242, 241, 228));
				grcShortcutBarGripper.SetColors(RGB(120, 142, 111), RGB(73,91, 67));
			}
			break;
			
		case xtpSystemThemeSilver:
			{
				m_arrColor[XPCOLOR_FRAME] = RGB(124, 124, 148);
				grcCaption.SetColors(RGB(168, 167, 191), RGB(112, 111, 145));
				grcDockBar.SetColors(RGB(215, 215, 229), RGB(243, 243, 247));
				grcShortcutBarGripper.SetColors(RGB(168, 167, 191),RGB(119, 118, 151));
			}
			break;
		}
	}
}

void CXTPColorManager::RefreshColors(BOOL bInit)
{

	if (bInit && m_bInit)
		return;

	m_bInit = TRUE;
	
	struct MUTECOLOR
	{
		int nIndex;
		int clrMain;
		int clrSub;
		int nDistance;
	} ;

	BOOL bSimpleColors = FALSE;

	CDC *pDC = CWnd::GetDesktopWindow()->GetDC();
	if (pDC)
	{
		int nColors = pDC->GetDeviceCaps(BITSPIXEL);
		CWnd::GetDesktopWindow()->ReleaseDC(pDC);
		
		bSimpleColors = (nColors > 0 && nColors <= 8);
	}

	RefreshSysColors();
	RefreshXPColors();

	if (bSimpleColors)
	{
		RefreshGradientColors();
		return;
	}

	const MUTECOLOR IndexTable[] =
	{ 
        { XPCOLOR_TOOLBAR_FACE,     COLOR_WINDOW,         COLOR_3DFACE,    165  }, 
        { XPCOLOR_HIGHLIGHT,        COLOR_HIGHLIGHT,      COLOR_WINDOW,    0x1E },
        { XPCOLOR_HIGHLIGHT_PUSHED, COLOR_HIGHLIGHT,      COLOR_WINDOW,    0x32 }, 
        { XPCOLOR_MENUBAR_FACE,     COLOR_3DFACE,         COLOR_WINDOW,    0x8f },
        { XPCOLOR_GRAYTEXT,         COLOR_GRAYTEXT,       COLOR_WINDOW,    0x46 },
        { XPCOLOR_TOOLBAR_GRIPPER,  COLOR_3DSHADOW,       COLOR_WINDOW,    0x4b },
        { XPCOLOR_SEPARATOR,        COLOR_3DSHADOW,       COLOR_WINDOW,    0x46 },
        { XPCOLOR_MENUBAR_BORDER,   XPCOLOR_TOOLBAR_TEXT, COLOR_BTNSHADOW, 0x14 },
        { XPCOLOR_DISABLED,         COLOR_3DSHADOW,       COLOR_WINDOW,    0x5A },
		{ XPCOLOR_MENUBAR_EXPANDED, COLOR_3DFACE,         COLOR_3DSHADOW,  90 } 
	};

	int i;
	for (i = 0; i < sizeof(IndexTable) / sizeof(IndexTable[0]); i++)
	{
		m_arrColor[IndexTable[i].nIndex] = LightColor(GetColor(IndexTable[i].clrMain), 
			GetColor(IndexTable[i].clrSub), IndexTable[i].nDistance);

	}

	const MUTECOLOR LongTable[] =
	{
        { XPCOLOR_HIGHLIGHT,                XPCOLOR_TOOLBAR_FACE,          XPCOLOR_HIGHLIGHT,                50  },
        { XPCOLOR_HIGHLIGHT_BORDER,         XPCOLOR_TOOLBAR_FACE,          COLOR_HIGHLIGHT,                  100 },
        { XPCOLOR_HIGHLIGHT_PUSHED,         XPCOLOR_TOOLBAR_FACE,          XPCOLOR_HIGHLIGHT_PUSHED,         30  },
        { XPCOLOR_GRAYTEXT,                 XPCOLOR_MENUBAR_FACE,          XPCOLOR_GRAYTEXT,                 80  },
        { XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED,      XPCOLOR_HIGHLIGHT_CHECKED_BORDER, 100 },
        { XPCOLOR_TOOLBAR_GRIPPER,          XPCOLOR_TOOLBAR_FACE,          XPCOLOR_TOOLBAR_GRIPPER,          85  },
        { XPCOLOR_SEPARATOR,                XPCOLOR_TOOLBAR_FACE,          XPCOLOR_SEPARATOR,                50  },
        { XPCOLOR_MENUBAR_BORDER,           XPCOLOR_PUSHED_TEXT,           XPCOLOR_MENUBAR_BORDER,           100 },
        { XPCOLOR_DISABLED,                 XPCOLOR_TOOLBAR_FACE,          XPCOLOR_DISABLED,                 80  },
        { XPCOLOR_MENUBAR_TEXT,             XPCOLOR_MENUBAR_FACE,          XPCOLOR_MENUBAR_TEXT,             180 },
        { XPCOLOR_HIGHLIGHT_TEXT,           XPCOLOR_HIGHLIGHT,             XPCOLOR_HIGHLIGHT_TEXT,           180 },
        { XPCOLOR_TOOLBAR_TEXT,             XPCOLOR_TOOLBAR_FACE,          XPCOLOR_TOOLBAR_TEXT,             180 },
        { XPCOLOR_PUSHED_TEXT,              XPCOLOR_HIGHLIGHT_PUSHED,      XPCOLOR_PUSHED_TEXT,              180 }
	};
	
	if (LongColor(GetColor(COLOR_3DFACE), GetColor(XPCOLOR_TOOLBAR_FACE), 1, 35))
	{
		m_arrColor[XPCOLOR_TOOLBAR_FACE] = AdjustColor(GetColor(COLOR_3DFACE), GetColor(XPCOLOR_TOOLBAR_FACE), 35);
	}

	for (i = 0; i < sizeof(LongTable) / sizeof(LongTable[0]); i++)
	{
		if (! LongColor(GetColor(LongTable[i].clrMain),GetColor(LongTable[i].clrSub) , 1, LongTable[i].nDistance))
		{
			m_arrColor[LongTable[i].nIndex] = MixColor(GetColor(LongTable[i].clrMain),GetColor(LongTable[i].clrSub), LongTable[i].nDistance);
		}		
	}
	
	if (LongColor(GetColor(XPCOLOR_MENUBAR_FACE), GetColor(XPCOLOR_GRAYTEXT), 1, 145))
	{

		m_arrColor[XPCOLOR_GRAYTEXT] = AdjustColor(GetColor(XPCOLOR_MENUBAR_FACE), GetColor(XPCOLOR_GRAYTEXT), 145);
	}

	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED] = 
		RGB( GetRValue(clrWindow) * 40 / 100 + GetRValue(clrHighlight) * 10 / 100 + GetRValue(clrToolBar) * 50 / 100, 
		GetGValue(clrWindow) * 40 / 100 + GetGValue(clrHighlight) * 10 / 100 + GetGValue(clrToolBar) * 50 / 100,
		GetBValue(clrWindow) * 40 / 100 + GetBValue(clrHighlight) * 10 / 100 + GetBValue(clrToolBar) * 50 / 100);


	COLORREF clrBtn = GetColor(COLOR_3DFACE);
	int r =  GetRValue(clrBtn), g = GetGValue(clrBtn), b = GetBValue(clrBtn);
	int nMax = __max(__max(r, g),  b);	
	if (nMax == 0)
		m_arrColor[XPCOLOR_TAB_INACTIVE_BACK] = RGB(35, 35, 35); else
	{
		int nDelta = __min(35, 255 - nMax) + nMax;
		m_arrColor[XPCOLOR_TAB_INACTIVE_BACK] = RGB(r * nDelta / nMax, g * nDelta / nMax, b * nDelta / nMax);
	}

	COLORREF clrShadow = GetColor(COLOR_3DSHADOW);
	r =  GetRValue(clrShadow), g = GetGValue(clrShadow), b = GetBValue(clrShadow );
	nMax = __max(__max(r, g),  b);	
	
	if (clrBtn == 0) m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(0x7f, 0x7f, 0x7f);
	else if (nMax == 0)	m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(0, 0, 0); 
	else
	{
		int nDelta = __max(0, nMax - 43);
		m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(r * nDelta / nMax, g * nDelta / nMax, b * nDelta / nMax);		
	}		

	COLORREF clrBackground = m_arrColor[XPCOLOR_HIGHLIGHT];
	m_arrColor[XPCOLOR_ICONSHADDOW] = RGB( GetRValue(clrBackground) * .75, GetGValue(clrBackground ) * .75, GetBValue(clrBackground) * .75);
	
	m_arrColor[COLOR_3DLIGHT] = RGB(min(GetRValue(clrBtn) + 15, 255), min(GetGValue(clrBtn) + 15, 255), min(GetBValue(clrBtn) + 15, 255));
	m_arrColor[XPCOLOR_EDITCTRLBORDER] = m_arrColor[XPCOLOR_TOOLBAR_FACE];

	RefreshGradientColors();
}

void CXTPColorManager::SetColors(int cElements, CONST INT *lpaElements, CONST COLORREF *lpaRgbValues)
{
	for (int i = 0; i < cElements; i++)
		m_arrColor[lpaElements[i]] = lpaRgbValues[i];
}

void CXTPColorManager::SetColor(int nIndex, COLORREF clrValue)
{
	m_arrColor[nIndex] = clrValue;
}

double CXTPColorManager::GetRDelta(COLORREF clr)
{
	return GetRValue(clr) * (195.0/255.0);
} 

double CXTPColorManager::GetGDelta(COLORREF clr)
{
	return GetGValue(clr) * (390.0/255.0);
} 

double CXTPColorManager::GetBDelta(COLORREF clr)
{
	return GetBValue(clr) * (65.0/255.0);
}

void CXTPColorManager::DisableLunaColors(BOOL bDisable)
{
	m_bDisableLunaColors = bDisable;
}

BOOL CXTPColorManager::IsLunaColorsDisabled()
{
	return m_bDisableLunaColors;
}

// XTPEmptyRect类
//===========================================================================
// CXTPEmptyRect class
//===========================================================================
CXTPEmptyRect::CXTPEmptyRect()
{
	SetRectEmpty();
}

// XTMemDC类

IMPLEMENT_DYNAMIC(CXTMemDC, CDC);

//===========================================================================
// CXTMemDC class
//===========================================================================
CXTMemDC::CXTMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor/*=GetSysColor(COLOR_3DFACE)*/)
{
	ASSERT(pDC != NULL);
	m_pDC = pDC;
	
	// If rect is NULL, use the device context's clip box.
	if (rect.IsRectEmpty())
		m_pDC->GetClipBox(&m_rc);
	else
		m_rc.CopyRect(&rect);
	
    // Create the memory DC, set Map Mode
	if (CreateCompatibleDC(m_pDC))
	{
		SetMapMode(m_pDC->GetMapMode());
		
		// Create a bitmap big enough to hold the window's image
		m_bitmap.CreateCompatibleBitmap(m_pDC, m_rc.Width(), m_rc.Height());
		
		// Select the bitmap into the memory DC
		m_hOldBitmap = (HBITMAP)SelectObject(&m_bitmap)->GetSafeHandle();
		
		// Repaint the background, this takes the place of WM_ERASEBKGND.
		CRect rc( m_rc ); //P.Baranov
		
		rc.left    = 0;
		rc.right  -= m_rc.left;
		rc.top     = 0;
		rc.bottom -= m_rc.top;
		
		FillSolidRect(rc, clrColor);
		
		m_bValid = TRUE;
	}
	
	// Something bad happened, could be GDI leak, didn't create device context.
	else
	{
		m_bValid = FALSE;
		m_hOldBitmap = NULL;
	}
}

CXTMemDC::~CXTMemDC()
{
	if (m_bValid)
	{
		// Blt it
		m_pDC->BitBlt(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(),
			this, 0, 0, SRCCOPY);            
	}
	
	// Select the original bitmap back in
	if (m_hOldBitmap != NULL)
	{
		SelectObject(m_hOldBitmap);
		m_bitmap.DeleteObject();
	}
	
	DeleteDC();
}

CBitmap& CXTMemDC::GetBitmap()
{
	return m_bitmap;
}

void CXTMemDC::Discard()
{
	m_bValid = FALSE;
}

void CXTMemDC::FromDC()
{
	BitBlt(0, 0, m_rc.Width(), m_rc.Height(), m_pDC, m_rc.left, m_rc.top, SRCCOPY);            
}




// CXTOSVersionInfo类
CXTOSVersionInfo CXTOSVersionInfo::m_instance;

//===========================================================================
// CXTOSVersionInfo class
//===========================================================================

CXTOSVersionInfo::CXTOSVersionInfo()
{
	// zero memory and set struct size.
	::ZeroMemory((OSVERSIONINFO*)this, sizeof(OSVERSIONINFO));
	dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	// get the Windows OS version information.
	::GetVersionEx(this);
}

bool CXTOSVersionInfo::IsWin31() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32s);
}

bool CXTOSVersionInfo::IsWin95() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		(dwMajorVersion == 4) && (dwMinorVersion < 10);
}

bool CXTOSVersionInfo::IsWin98() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		EqualTo(4, 10);
}

bool CXTOSVersionInfo::IsWinME() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		EqualTo(4, 90);
}

bool CXTOSVersionInfo::IsWinNT4() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_NT) &&
		EqualTo(4, 0);
}

bool CXTOSVersionInfo::IsWin2K() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_NT) &&
		EqualTo(5, 0);
}

bool CXTOSVersionInfo::IsWinXP() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_NT) &&
		EqualTo(5, 1);
}

bool CXTOSVersionInfo::IsWin95OrGreater() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		(dwMajorVersion >= 4);
}

bool CXTOSVersionInfo::IsWin98OrGreater() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		GreaterThanEqualTo(4, 10);
}

bool CXTOSVersionInfo::IsWinMEOrGreater() const
{
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		GreaterThanEqualTo(4, 90);
}

bool CXTOSVersionInfo::IsWinNT4OrGreater() const
{
	return (dwPlatformId >= VER_PLATFORM_WIN32_NT) &&
		(dwMajorVersion >= 4);
}

bool CXTOSVersionInfo::IsWin2KOrGreater() const
{
	return (dwPlatformId >= VER_PLATFORM_WIN32_NT) &&
		(dwMajorVersion >= 5);
}

bool CXTOSVersionInfo::IsWinXPOrGreater() const
{
	return (dwPlatformId >= VER_PLATFORM_WIN32_NT) &&
		GreaterThanEqualTo(5, 1);
}

bool CXTOSVersionInfo::GreaterThanEqualTo(const DWORD maj, const DWORD min) const
{
	return (dwMajorVersion > maj) || (dwMajorVersion == maj  &&
		dwMinorVersion >= min);
}

bool CXTOSVersionInfo::EqualTo(const DWORD maj, const DWORD min) const
{
	return (dwMajorVersion == maj) && (dwMinorVersion == min);
}

// XT_AUX_DATA结构体
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XT_AUX_DATA::XT_AUX_DATA()
{
	strINIFileName = _T("");
	iComCtlVersion = -1;

	CXTOSVersionInfo* pInfo = XTOSVersionInfo();
	bWin95 = pInfo->IsWin95();
	bWin98 = pInfo->IsWin98();
	bWinNT = pInfo->IsWinNT4();
	bWin2K = pInfo->IsWin2K();
	bWinXP = pInfo->IsWinXP();

	bUseOfficeFont      = FALSE;
	bMenuShadows        = TRUE;
	nMenuAnimation      = 0;		// no animation
	bXPMode             = FALSE;
	bCoolMenuCompatMode = FALSE;
	bControlBarMenus    = TRUE;
	bDockBarMenus       = TRUE;
	bToolBarVisualize   = TRUE;
	bMenuRecentCommands = TRUE;
	bShowFullAfterDelay = TRUE;
	bToolBarScreenTips  = TRUE;
	bToolBarAccelTips   = FALSE;
	bUseSolidShadows    = FALSE;
	bDisableFadedIcons  = FALSE;

#if defined (_XTP_STATICLINK) || defined (_XTP_DYN_STATICLIB)
#if defined (_XTP_USES_RESOURCE_DLL)
#pragma message( "XTGlobal.cpp: ### WARNING: You must call xtAfxData.InitResources(HINSTANCE hInst) to initialize Xtreme Toolkit resources!" )
#else
	InitResources(NULL);
#endif // defined (_XT_USES_RESOURCE_DLL)
#endif // defined (_XT_STATICLINK) || defined (_XT_DYN_STATICLIB)
}

XT_AUX_DATA::~XT_AUX_DATA()
{
	FreeSysFonts();
	FreeSysCursors();
}

void XT_AUX_DATA::InitResources(HINSTANCE hInst)
{
	// Set the resource handle for the toolkit.
	if (hInst == NULL)
	{
		hInst = ::GetModuleHandle(0);
	}

	ASSERT(hInst != NULL);
	hInstance = hInst;

	UpdateSysColors();
	UpdateSysMetrics();

	LoadSysFonts();
	LoadSysCursors();

	// Verify correct version of Comctl32.dll.
	GetComCtlVersion();

	/*if (iComCtlVersion < VERSION_IE401)
	{
		TCHAR szTitle[]   = _T("Codejock Software?- Wrong Version Comctl32.dll");
		TCHAR szMessage[] = _T("This application uses the Xtreme Toolkit which requires Comctl32.dll version 4.72 or higher.\nPlease contact Codejock Software for more details...application will now abort!\n\nXtreme Toolkit ?998-2004 Codejock Software, All Rights Reserved.\n\nemail support@codejock.com, URL: http://www.codejock.com");

		::MessageBox(NULL, szMessage, szTitle, MB_ICONSTOP | MB_OK);
		::PostQuitMessage(0);
	}*/
}

void XT_AUX_DATA::LoadSysCursors()
{
	FreeSysCursors();

	// Load the cursors used by the Xtreme Toolkit.
	hcurDragNone = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGNONE));

	// If your static linked application asserts
	// here you most likely need to include the
	// following line in your .rc2 file:
	// #include "XTControlsResource.rc"
	ASSERT(hcurDragNone != NULL);

	hcurDragCopy = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGCOPY));
	ASSERT(hcurDragCopy != NULL);

	hcurDragMove = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGMOVE));
	ASSERT(hcurDragMove != NULL);

	hcurHand = ::LoadCursor(0, MAKEINTRESOURCE(32649));	
	if (hcurHand == 0)
	hcurHand = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_HAND));
	ASSERT(hcurHand != NULL);

	hcurHandNone = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_HANDNONE));
	ASSERT(hcurHandNone != NULL);

	hcurHSplitBar = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XTP_IDC_HSPLITBAR));
	ASSERT(hcurHSplitBar != NULL);

	hcurVSplitBar = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XTP_IDC_VSPLITBAR));
	ASSERT(hcurVSplitBar != NULL);

	hcurMove = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL));
	ASSERT(hcurMove != NULL);
}

void XT_AUX_DATA::FreeSysCursors()
{
	// Free the cursors used by the Xtreme Toolkit.
	if (hcurDragNone != NULL) {
		::DestroyCursor(hcurDragNone);
	}
	if (hcurDragCopy != NULL) {
		::DestroyCursor(hcurDragCopy);
	}
	if (hcurDragMove != NULL) {
		::DestroyCursor(hcurDragMove);
	}
	if (hcurHand != NULL) {
		::DestroyCursor(hcurHand);
	}
	if (hcurHandNone != NULL) {
		::DestroyCursor(hcurHandNone);
	}
	if (hcurHSplitBar != NULL) {
		::DestroyCursor(hcurHSplitBar);
	}
	if (hcurVSplitBar != NULL) {
		::DestroyCursor(hcurVSplitBar);
	}
}

static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX* lpntm, int FontType, LPVOID pThis)
{
	UNREFERENCED_PARAMETER( lpntm );
	UNREFERENCED_PARAMETER( FontType );

	CString strFontName = *( CString* )pThis;
	CString strFaceName = pelf->elfLogFont.lfFaceName;

	if ( strFaceName.CompareNoCase( strFontName ) == 0 )
	{
		return 0;
	}

	return 1;
}

void XT_AUX_DATA::SetGlobalFont(LPCTSTR lpszFaceName, LPCTSTR lpszVertFaceName/*=NULL*/)
{
    XT_NONCLIENTMETRICS ncm;

    if ( lpszFaceName != NULL )
        _tcscpy( ncm.lfMenuFont.lfFaceName, lpszFaceName );

    CFont fontHorz;
    fontHorz.CreateFontIndirect(&ncm.lfMenuFont);

    if ( lpszVertFaceName != NULL )
        _tcscpy( ncm.lfMenuFont.lfFaceName, lpszVertFaceName );

    CFont fontVert;
    fontVert.CreateFontIndirect(&ncm.lfMenuFont);

    SetGlobalFont(&fontHorz, &fontVert);
}

void XT_AUX_DATA::SetGlobalFont(CFont* pFont, CFont* pVertFont/*=NULL*/)
{
    if ( pFont != NULL && pFont->m_hObject != NULL )
    {
        XT_LOGFONT lf;
        pFont->GetLogFont( &lf );
        
        // Create the toolkit horizontal fonts.
        CreateSysFont( lf, fontBold, FW_BOLD );
        CreateSysFont( lf, font );
        CreateSysFont( lf, fontULine, FW_NORMAL, 1 );
        CreateSysFont( lf, fontHCapt );
    }

    if ( pVertFont != NULL && pVertFont->m_hObject != NULL )
    {
        XT_LOGFONT lf;
        pVertFont->GetLogFont( &lf );
        
        // Create the toolkit vertical fonts.
        CreateSysFont( lf, fontVCapt, FW_NORMAL, 0, 900, 900 );
        CreateSysFont( lf, fontVCaptBold, FW_BOLD, 0, 900, 900 );
        CreateSysFont( lf, fontVCaptR, FW_NORMAL, 0, 2700, 2700 );
        CreateSysFont( lf, fontVCaptRBold, FW_BOLD, 0, 2700, 2700 );
    }
}

bool XT_AUX_DATA::FontExists(CString& strFaceName)
{
	// Enumerate all styles and charsets of all fonts:
	XT_LOGFONT lfEnum;
	lfEnum.lfFaceName[ 0 ] = 0;
	lfEnum.lfCharSet = DEFAULT_CHARSET;

	// See if lpszFaceName font exists, if so, then use it...
	CWindowDC dc( NULL );
	if ( ::EnumFontFamiliesEx(
		dc.m_hDC, &lfEnum, ( FONTENUMPROC )EnumFontFamExProc, ( LPARAM )&strFaceName, 0 ) == 0 )
	{
		return true;
	}

	return false;
}

bool XT_AUX_DATA::CreateSysFont(XT_LOGFONT& lf, CFont& font, long lfWeight/*=FW_NORMAL*/, char lfUnderline/*=0*/, long lfOrientation/*=0*/, long lfEscapement/*=0*/)
{
	lf.lfWeight      = lfWeight;
	lf.lfOrientation = lfOrientation;
	lf.lfEscapement  = lfEscapement;
	lf.lfUnderline   = lfUnderline;

	if ( !font.CreateFontIndirect( &lf ) )
	{
		return false;
	}

	return true;
}

void XT_AUX_DATA::UseOfficeFont()
{
    bUseOfficeFont = true;
    LoadSysFonts();
}

void XT_AUX_DATA::LoadSysFonts()
{
    CWindowDC dc(NULL);

	// Free up any resources previously allocated.
	FreeSysFonts();

    XT_LOGFONT lf;
    lf.lfCharSet = (BYTE)::GetTextCharsetInfo(dc, NULL, 0);

	// Set the character set to default.
	XT_NONCLIENTMETRICS ncm;
    lf.lfHeight = ncm.lfMenuFont.lfHeight < 0 ? __min(-11, ncm.lfMenuFont.lfHeight) : ncm.lfMenuFont.lfHeight;
	lf.lfWeight = ncm.lfMenuFont.lfWeight;	
	lf.lfItalic = ncm.lfMenuFont.lfItalic;

	_tcscpy (lf.lfFaceName, ncm.lfMenuFont.lfFaceName);

    // see if the Tahoma font was found.
	CString strOfficeFont(_T("Tahoma"));
	bool bTahomaExists = FontExists( strOfficeFont );

    // see if the Arial font was found.
	CString strArialFont(_T("Fixedsys"));
    bool bArialExists = FontExists(strArialFont);

    // see if the system font is set to the Tahoma font, if so set the office font flag to true.
    bool bTahomaLF = strOfficeFont.CompareNoCase(ncm.lfMenuFont.lfFaceName) == 0;
    if (bTahomaLF) {
        bUseOfficeFont = true;
    }
	
    // set the office font if applicable.
	BOOL bUseSystemFont = (ncm.lfMenuFont.lfCharSet > SYMBOL_CHARSET);
	if (!bUseSystemFont && !bTahomaLF)
    {
        if (bUseOfficeFont && bTahomaExists) {
	        _tcscpy( lf.lfFaceName, strOfficeFont );
        }
    }

    // Create the default GUI bold font.
	XT_LOGFONT lfHorz( lf );
	CreateSysFont( lfHorz, fontBold, FW_BOLD );

	// Create the default GUI font.
	CreateSysFont( lfHorz, font );

	// Create the default GUI underlined font.
	CreateSysFont( lfHorz, fontULine, FW_NORMAL, 1 );

	// Create the horizontal caption font.
	CreateSysFont( lfHorz, fontHCapt );

    // If Tahoma font is not used, use Arial for vertical fonts, it displays better on older
    // operating systems than MS Sans Serif.
    if (!bUseSystemFont && !bTahomaLF)
    {
        if (!bUseOfficeFont && bArialExists) {
            _tcscpy( lf.lfFaceName, strArialFont );
        }
    }

	// Create the vertical caption font.
	XT_LOGFONT lfVert( lf );
	CreateSysFont( lfVert, fontVCapt, FW_NORMAL, 0, 900, 900 );

	// Create the bold vertical caption font.
	CreateSysFont( lfVert, fontVCaptBold, FW_BOLD, 0, 900, 900 );

	// Create the right aligned vertical caption font.
	CreateSysFont( lfVert, fontVCaptR, FW_NORMAL, 0, 2700, 2700 );

	// Create the bold right aligned vertical caption font.
	CreateSysFont( lfVert, fontVCaptRBold, FW_BOLD, 0, 2700, 2700 );
}

void XT_AUX_DATA::FreeSysFonts()
{
	font.DeleteObject();
	fontBold.DeleteObject();
	fontULine.DeleteObject();
	fontHCapt.DeleteObject();
	fontVCapt.DeleteObject();
	fontVCaptBold.DeleteObject();
	fontVCaptR.DeleteObject();
	fontVCaptRBold.DeleteObject();
}

void XT_AUX_DATA::SetINIFileName(LPCTSTR lpszINIFullPath)
{
	strINIFileName = lpszINIFullPath;
}

LPCTSTR XT_AUX_DATA::GetINIFileName()
{
	return strINIFileName;
}

XT_AUX_DATA& XT_AUX_DATA::Get()
{
	static XT_AUX_DATA xtauxdata;
	return xtauxdata;
}

void XT_AUX_DATA::UpdateSysColors()	
{
	XTPColorManager()->RefreshColors(TRUE);
	// Initialize standard color for windows components.
	clr3DFace                   = GetXtremeColor( COLOR_3DFACE );
	clr3DShadow					= GetXtremeColor( COLOR_3DSHADOW );
	clr3DDkShadow				= GetXtremeColor( COLOR_3DDKSHADOW );
	clr3DHilight				= GetXtremeColor( COLOR_3DHILIGHT );
	clrBtnText					= GetXtremeColor( COLOR_BTNTEXT );
	clrGrayText					= GetXtremeColor( COLOR_GRAYTEXT );
//	clrHighlight				= GetXtremeColor( COLOR_HIGHLIGHT ); //  snowfly
	clrHighlightText			= GetXtremeColor( COLOR_HIGHLIGHTTEXT );
	clrMenu						= GetXtremeColor( COLOR_MENU );
	clrMenuText					= GetXtremeColor( COLOR_MENUTEXT );
//	clrWindow					= GetXtremeColor( COLOR_WINDOW );   // snowfly
	clrWindowFrame				= GetXtremeColor( COLOR_WINDOWFRAME );
	clrWindowText				= GetXtremeColor( COLOR_WINDOWTEXT );
	clrActiveCaption		    = GetXtremeColor( COLOR_ACTIVECAPTION );
	clrInActiveCaption          = GetXtremeColor( COLOR_INACTIVECAPTION );
	clrGradActiveCapt           = GetXtremeColor( COLOR_GRADIENTACTIVECAPTION );
//	clrGradInActiveCapt         = GetXtremeColor( COLOR_GRADIENTINACTIVECAPTION ); //snowfly
	clrActiveCaptText           = GetXtremeColor( COLOR_CAPTIONTEXT );
	clrInactiveCaptText         = GetXtremeColor( COLOR_INACTIVECAPTIONTEXT );

	// For some strange reason Windows 2000/NT does not return
	// the expected RGB value for COLOR_3DLIGHT...
	clr3DLight					=  CXTColorRef(clr3DFace) + 15;  //snowfly

	// Initialize special colors for XP style interfaces.
	clrXPBarFace				= GetXtremeColor( XPCOLOR_TOOLBAR_FACE );
	clrXPHighlight				= GetXtremeColor( XPCOLOR_HIGHLIGHT );
	clrXPHighlightBorder		= GetXtremeColor( XPCOLOR_HIGHLIGHT_BORDER );
	clrXPHighlightPushed		= GetXtremeColor( XPCOLOR_HIGHLIGHT_PUSHED );
	clrXPIconShadow				= GetXtremeColor( XPCOLOR_ICONSHADDOW );
	clrXPGrayText				= GetXtremeColor( XPCOLOR_GRAYTEXT );
	clrXPHighlightChecked		= GetXtremeColor( XPCOLOR_HIGHLIGHT_CHECKED );
	clrXPHighlightCheckedBorder	= GetXtremeColor( XPCOLOR_HIGHLIGHT_CHECKED_BORDER );
	clrXPGripper				= GetXtremeColor( XPCOLOR_TOOLBAR_GRIPPER );
	clrXPSeparator				= GetXtremeColor( XPCOLOR_SEPARATOR );
	clrXPDisabled				= GetXtremeColor( XPCOLOR_DISABLED );
	clrXPMenuTextBack			= GetXtremeColor( XPCOLOR_MENUBAR_FACE );
	clrXPMenuExpanded           = GetXtremeColor( XPCOLOR_MENUBAR_EXPANDED );
	clrXPMenuBorder				= GetXtremeColor( XPCOLOR_MENUBAR_BORDER );
	clrXPMenuText				= GetXtremeColor( XPCOLOR_MENUBAR_TEXT );
	clrXPHighlightText			= GetXtremeColor( XPCOLOR_HIGHLIGHT_TEXT );
	clrXPBarText				= GetXtremeColor( XPCOLOR_TOOLBAR_TEXT );
	clrXPBarTextPushed			= GetXtremeColor( XPCOLOR_PUSHED_TEXT );
	clrXPTabInactiveBack		= GetXtremeColor( XPCOLOR_TAB_INACTIVE_BACK );
	clrXPTabInactiveText		= GetXtremeColor( XPCOLOR_TAB_INACTIVE_TEXT );
}

void XT_AUX_DATA::UpdateSysMetrics()
{
	cxSmIcon   = ::GetSystemMetrics(SM_CXSMICON);
	cySmIcon   = ::GetSystemMetrics(SM_CYSMICON);
	cxSize	   = 18/*::GetSystemMetrics(SM_CXSIZE)*/;
	cySize     = 18/*::GetSystemMetrics(SM_CYSIZE)*/;
	cxHThumb   = ::GetSystemMetrics(SM_CXHTHUMB);
	cyVThumb   = ::GetSystemMetrics(SM_CYVTHUMB);
	cyMenuItem = ::GetSystemMetrics(SM_CYMENU);
}

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxDrawEmbossed
/////////////////////////////////////////////////////////////////////////////
 void AFXAPI _xtAfxDrawEmbossed(CDC* pDC, CImageList& imageList, int nIndex, CPoint point, BOOL bInColor)
{
	IMAGEINFO imgInfo;
	VERIFY(imageList.GetImageInfo(0, &imgInfo));
	CRect rcImage = imgInfo.rcImage;

	// create memory dc
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// create mono or color bitmap
	CBitmap bitmap;
	if (bInColor) {
		bitmap.CreateCompatibleBitmap(pDC, rcImage.Width(), rcImage.Height());
	} else {
		bitmap.CreateBitmap(rcImage.Width(), rcImage.Height(), 1, 1, NULL);
	}

	// draw image into memory DC--fill BG white first
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rcImage.Width(), rcImage.Height(), WHITENESS);
	imageList.Draw(&memDC, nIndex, CPoint(0,0), ILD_TRANSPARENT);

	// Draw using highlight offset by (1,1), then shadow
	CBrush brShadow(xtAfxData.clr3DShadow);
	CBrush brHilite(xtAfxData.clr3DHilight);

	CBrush* pOldBrush = pDC->SelectObject(&brHilite);
	pDC->BitBlt(point.x+1, point.y+1, rcImage.Width(), rcImage.Height(), &memDC, 0, 0, 0xB8074A);
	
	pDC->SelectObject(&brShadow);
	pDC->BitBlt(point.x, point.y, rcImage.Width(), rcImage.Height(), &memDC, 0, 0, 0xB8074A);

	pDC->SelectObject(pOldBrush);
	memDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
}
/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxDrawShadedRect
/////////////////////////////////////////////////////////////////////////////
  void AFXAPI _xtAfxDrawShadedRect(CDC *pDC, CRect& rect)
{
	// Bit pattern for a monochrome brush with every
	// other pixel turned off
	WORD bits[] = {
		0x0055, 0x00AA, 0x0055, 0x00AA,
		0x0055, 0x00AA, 0x0055, 0x00AA
	};

	// Need a monochrome pattern bitmap
	CBitmap bitmap;
	bitmap.CreateBitmap( 8, 8, 1, 1, &bits );

	// Create the pattern brush
	CBrush brush;
	brush.CreatePatternBrush( &bitmap );
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	COLORREF crBack	= pDC->SetBkColor(xtAfxData.clr3DFace);
	COLORREF crText	= pDC->SetTextColor(xtAfxData.clr3DHilight);

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	// Restore the device context
	pDC->SelectObject(pOldBrush);
	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crText);

	brush.DeleteObject();
}


//typedef HRESULT (CALLBACK* XT_AFX_DLLGETVERSIONPROC)(XT_DLLVERSIONINFO*);

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: GetComCtlVersion()
//
DWORD XT_AUX_DATA::GetComCtlVersion()
{
	// return cached version if already determined...
	if (iComCtlVersion != -1)
		return iComCtlVersion;

/*snowfly
	// otherwise determine comctl32.dll version via DllGetVersion
	HINSTANCE hInst = ::LoadLibraryA("COMCTL32.DLL");
	ASSERT(hInst != NULL);

	XT_AFX_DLLGETVERSIONPROC pfn = (XT_AFX_DLLGETVERSIONPROC)
		::GetProcAddress(hInst, "DllGetVersion");

	if (pfn != NULL)
	{
		XT_DLLVERSIONINFO dvi;
		memset(&dvi, 0, sizeof(dvi));
		dvi.cbSize = sizeof(dvi);

		if (SUCCEEDED((*pfn)(&dvi)))
		{
			ASSERT(dvi.dwMajorVersion <= 0xFFFF);
			ASSERT(dvi.dwMinorVersion <= 0xFFFF);
			DWORD dwVersion = MAKELONG(dvi.dwMinorVersion, dvi.dwMajorVersion);

			iComCtlVersion = dwVersion;
		}
	}

	FreeLibrary(hInst);*/


	return iComCtlVersion != -1? iComCtlVersion : 0;
}

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: GetXTVersion()
//
CString XT_AUX_DATA::GetXTVersion(bool bVerNumOnly/*=false*/) const
{
	UNREFERENCED_PARAMETER(bVerNumOnly);
	CString strVersion;
/*
	if ( bVerNumOnly )
	{
		strVersion.Format( _T( "%d.%02d" ),
			_XTPLIB_VERSION_MAJOR, _XTPLIB_VERSION_MINOR );
	}
	else
	{
		strVersion.Format( _T( "Xtreme Toolkit Pro v%d.%02d" ),
			_XTPLIB_VERSION_MAJOR, _XTPLIB_VERSION_MINOR );
	}
*/
	return strVersion;
}

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxChangeWindowFont()
/////////////////////////////////////////////////////////////////////////////

// helper function which sets the font for a window and all its children
// and also resizes everything according to the new font

  void  AFXAPI _xtAfxChangeWindowFont(CWnd* pWnd, CFont* pFont)
{
	CRect windowRect;
	
	// grab old and new text metrics
	TEXTMETRIC tmOld, tmNew;
	CDC * pDC = pWnd->GetDC();
	CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pSavedFont);
	pWnd->ReleaseDC(pDC);
	
	long oldHeight = tmOld.tmHeight+tmOld.tmExternalLeading;
	long newHeight = tmNew.tmHeight+tmNew.tmExternalLeading;
	
	// calculate new dialog window rectangle
	CRect clientRect, newClientRect, newWindowRect;
	
	pWnd->GetWindowRect(windowRect);
	pWnd->GetClientRect(clientRect);
	long xDiff = windowRect.Width() - clientRect.Width();
	long yDiff = windowRect.Height() - clientRect.Height();
	
	newClientRect.left = newClientRect.top = 0;
	newClientRect.right = clientRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
	newClientRect.bottom = clientRect.bottom * newHeight / oldHeight;
	
	newWindowRect.left = windowRect.left - 
		(newClientRect.right - clientRect.right)/2;
	newWindowRect.top = windowRect.top -
		(newClientRect.bottom - clientRect.bottom)/2;
	newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
	newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
	
	pWnd->MoveWindow(newWindowRect);
	
	pWnd->SetFont(pFont);
	
	// iterate through and move all child windows and change their font.
	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);
	
	while (pChildWnd)
	{
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(windowRect);
		
		CString strClass;
		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
		strClass.MakeUpper();
		if(strClass==_T("COMBOBOX"))
		{
			CRect rect;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
			windowRect.right = rect.right;
			windowRect.bottom = rect.bottom;
		}
		
		pWnd->ScreenToClient(windowRect);
		windowRect.left = windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		windowRect.right = windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		windowRect.top = windowRect.top * newHeight / oldHeight;
		windowRect.bottom = windowRect.bottom * newHeight / oldHeight;
		pChildWnd->MoveWindow(windowRect);
		
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}
/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxRelease()
/////////////////////////////////////////////////////////////////////////////
  DWORD AFXAPI _xtAfxRelease(LPUNKNOWN* lplpUnknown)
{
	ASSERT(lplpUnknown != NULL);
	if (*lplpUnknown != NULL)
	{
		DWORD dwRef = (*lplpUnknown)->Release();
		*lplpUnknown = NULL;
		return dwRef;
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxMakeOleVariant()
/////////////////////////////////////////////////////////////////////////////
  void AFXAPI _xtAfxMakeOleVariant(COleVariant &ov, LPCITEMIDLIST pidl)
{
	AfxVariantInit(&ov);

	if (pidl != NULL)
	{
		// walk through entries in the list and accumulate their size

		UINT cbTotal = 0;
		SAFEARRAY *psa = NULL;
		LPCITEMIDLIST pidlWalker = pidl;

		while (pidlWalker->mkid.cb)
		{
			cbTotal += pidlWalker->mkid.cb;
			pidlWalker = (LPCITEMIDLIST)
				(((LPBYTE)pidlWalker) + pidlWalker->mkid.cb);
		}

		// add the base structure size
		cbTotal += sizeof(ITEMIDLIST);

		// get a safe array for them
		psa = SafeArrayCreateVector(VT_UI1, 0, cbTotal);

		// copy it and set members
		if (psa != NULL)
		{
			memcpy(psa->pvData, (LPBYTE) pidl, cbTotal);
			ov.vt = VT_ARRAY | VT_UI1;
			ov.parray = psa;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxChildWindowFromPoint()
/////////////////////////////////////////////////////////////////////////////
  HWND AFXAPI _xtAfxChildWindowFromPoint(HWND hWnd, POINT pt)
{
  ASSERT(hWnd != NULL);

  // check child windows
  ::ClientToScreen(hWnd, &pt);
  HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
  for (; hWndChild != NULL; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT))
  {
	if ((UINT)(WORD)::GetDlgCtrlID(hWndChild) != (WORD)-1 &&
	  (::GetWindowLong(hWndChild, GWL_STYLE) & WS_VISIBLE))
	{
	  // see if point hits the child window
	  CRect rect;
	  ::GetWindowRect(hWndChild, rect);
	  if (rect.PtInRect(pt))
		return hWndChild;
	}
  }

  return NULL;    // not found
}

/////////////////////////////////////////////////////////////////////////////
// _xtAfxTreeViewCompareProc(LPARAM, LPARAM, LPARAM)
// Callback routine for sorting tree 

int CALLBACK _xtAfxTreeViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM /*lparamSort*/)
{
	XT_TVITEMDATA* lptvid1 = (XT_TVITEMDATA*)lparam1;
	XT_TVITEMDATA* lptvid2 = (XT_TVITEMDATA*)lparam2;
	
	HRESULT hr = lptvid1->lpsfParent->CompareIDs(0, lptvid1->lpi, lptvid2->lpi);
	if (FAILED(hr)) {
		return 0;
	}
	
	return (short)SCODE_CODE(GetScode(hr));
}

/////////////////////////////////////////////////////////////////////////////
// _xtAfxListViewCompareProc(LPARAM, LPARAM, LPARAM)
// Callback routine for sorting list 

int CALLBACK _xtAfxListViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM /*lparamSort*/)
{
	XT_LVITEMDATA* lplvid1 = (XT_LVITEMDATA*)lparam1;
	XT_LVITEMDATA* lplvid2 = (XT_LVITEMDATA*)lparam2;

	HRESULT hr = lplvid1->lpsfParent->CompareIDs(0,lplvid1->lpi,lplvid2->lpi);
	if (FAILED(hr)) {
		return 0;
	}

	return (short)SCODE_CODE(GetScode(hr));
}
 void AFXAPI _xtAfxStripMnemonics(CString& strIn)
{
#if _MSC_VER < 1200 // MFC 5.0
	CXTString strText(strIn);
#else
	CString  strText(strIn);
#endif

	// Check to see if '&&' was used for format an ampersand.
	int nIndex = strText.Find(_T("&&"), 0);

	// If not then remove mnemonic formatting.
	if (nIndex == -1) {
		strText.Remove(_T('&'));
	}

	// Otherwise swap out the '&&' temporarily and remove
	// the mnemonic character...
	else
	{
		strText.Replace(_T("&&"), _T("^^"));
		strText.Remove(_T('&'));
		strText.Replace(_T("^^"), _T("&&"));
	}

	strIn = strText;
}

  BOOL AFXAPI _xtAfxExist(LPCTSTR lpszFileName)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(lpszFileName, &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		::FindClose(hFind);
	}

	return (hFind != INVALID_HANDLE_VALUE);
}

  LPTSTR AFXAPI _xtAfxPathFindNextComponent(LPCTSTR pszPath)
{
	// We will return static memory
	static TCHAR tNextComponent[MAX_PATH + 1];
	
	// Make sure we were given a valid path
	if ( pszPath == NULL || _tcslen( pszPath ) == 0 ) {
		return NULL;
	}

	// Find the path delimiter
	TCHAR *cp =(TCHAR *)  _tcschr( pszPath, _T('\\') );
	if ( cp == NULL ) {
		// If we didn't find it, return an empty string
		tNextComponent[0] = 0;
		return tNextComponent;
	}

	// OK, we have the delimiter. Copy the rest of
	// the path to our internal buffer and return it
	_tcscpy( tNextComponent, cp + 1 );
	return tNextComponent;
}

  CWnd* AFXAPI _xtAfxCreateView(CWnd* pParentWnd, CRuntimeClass *pViewClass, CDocument *pDocument/*=NULL*/, CCreateContext *pContext/*=NULL*/, DWORD dwStyle/*=AFX_WS_DEFAULT_VIEW*/, CWnd* pOwnerWnd/*=NULL*/, UINT nID/*=AFX_IDW_PANE_FIRST*/)
{
#ifdef _DEBUG
	ASSERT(pContext != NULL || pViewClass != NULL);
	ASSERT(pContext != NULL || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext != NULL || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	if (pOwnerWnd == NULL)
		pOwnerWnd = pParentWnd;

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the 
		// currently selected client if possible.
		contextT.m_pLastView	   = NULL;
		contextT.m_pCurrentFrame   = NULL;
		contextT.m_pNewDocTemplate = NULL;
		contextT.m_pCurrentDoc	   = pDocument;
		contextT.m_pNewViewClass   = pViewClass;

		if (pDocument != NULL)
			contextT.m_pNewDocTemplate = pDocument->GetDocTemplate();

		pContext = &contextT;
	}

	CWnd* pWnd = NULL;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" ); // Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL); // not yet created.

	// Create with the right size (wrong position)
	CRect rect(0,0,0,0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, pParentWnd, nID, pContext))
	{
		TRACE0( "Failed to create docking window view.\n" );
		return NULL;
	}
	
	pWnd->SetOwner(pOwnerWnd);
	return pWnd;
}
