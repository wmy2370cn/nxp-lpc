/************************************************************************************/
/*     文件名:	DataType.h															*/
/*     功能:	包含TabView所必要的结构体和类										*/
/*     作者:    snowfly																*/
/*     时间:    2004.8.1															*/
/************************************************************************************/
#ifndef		_DATATYPE_H
#define     _DATATYPE_H
#include "XTFunctions.h"
#include "xtDefines.h"

//////////////////////////////////////////////////////////////////////
// Summary: XT_CLRFONT is a stand alone helper structure class.  It is used by CXTTreeCtrl
//			and CXTTreeView to store information for a specified tree item.
struct XT_CLRFONT
{
	LOGFONT  logfont; // A LOGFONT object that represents the tree item font.
	COLORREF color;   // An RGB value that represents the color for a tree item.
};
//////////////////////////////////////////////////////////////////////
// Summary: XT_LVITEMDATA is a stand alone helper structure class.  It holds list
//			item data for a shell list. It is used by CXTShellTreeCtrl, CXTShellTreeView,
//			CXTShellListCtrl and CXTShellListView.
struct XT_LVITEMDATA
{
	ULONG			ulAttribs;	// Shell item attributes.
	LPITEMIDLIST	lpi;		// Pointer to an item ID list.
	LPSHELLFOLDER	lpsfParent; // Points to the parent shell folder item.
};
//////////////////////////////////////////////////////////////////////
// Summary: XT_TVITEMDATA is a stand alone helper structure class.  It is used to
//			create a structure that holds tree item data for a shell tree.  It is
//			used by CXTShellTreeCtrl, CXTShellTreeView, CXTShellListCtrl and CXTShellListView.
struct XT_TVITEMDATA
{
	LPITEMIDLIST	lpi;		// Pointer to an item ID list.
	LPITEMIDLIST	lpifq;		// Pointer to an item ID list.
	LPSHELLFOLDER	lpsfParent; // Pointer to the parent shell folder item.
};

// CXTPColorManager类
// 用于管理XP风格的枚举变量
//---------------------------------------------------------------------------
// Summary: Enumeration used to determine the current Windows XP theme in use
//---------------------------------------------------------------------------
enum XTPCurrentSystemTheme
{
	xtpSystemThemeUnknown, // No known theme in use
	xtpSystemThemeBlue,    // Blue theme in use
	xtpSystemThemeOlive,   // Olive theme in use
	xtpSystemThemeSilver   // Silver theme in use
};

// XP base color.
const UINT XPCOLOR_BASE						= 30;
// XP toolbar background color.
const UINT XPCOLOR_TOOLBAR_FACE				= XPCOLOR_BASE;
// XP menu item selected color.
const UINT XPCOLOR_HIGHLIGHT				= 31;
// XP menu item selected border color.
const UINT XPCOLOR_HIGHLIGHT_BORDER			= 32;
// XP menu item pushed color.
const UINT XPCOLOR_HIGHLIGHT_PUSHED			= 33;
// XP menu item checked color.
const UINT XPCOLOR_HIGHLIGHT_CHECKED		= 36;
// XP menu item checked border color.
const UINT XPCOLOR_HIGHLIGHT_CHECKED_BORDER	= 37;
// XP menu item icon shadow.
const UINT XPCOLOR_ICONSHADDOW				= 34;
// XP menu item disabled text color.
const UINT XPCOLOR_GRAYTEXT					= 35;
// XP toolbar gripper color.
const UINT XPCOLOR_TOOLBAR_GRIPPER			= 38;
// XP toolbar separator color.
const UINT XPCOLOR_SEPARATOR				= 39;
// XP menu icon disabled color.
const UINT XPCOLOR_DISABLED					= 40;
// XP menu item text background color.
const UINT XPCOLOR_MENUBAR_FACE				= 41;
// XP hidden menu commands background color.
const UINT XPCOLOR_MENUBAR_EXPANDED			= 42;
// XP menu border color.
const UINT XPCOLOR_MENUBAR_BORDER			= 43;
// XP menu item text color.
const UINT XPCOLOR_MENUBAR_TEXT				= 44;
// XP menu item selected text color.
const UINT XPCOLOR_HIGHLIGHT_TEXT			= 45;
// XP toolbar text color.
const UINT XPCOLOR_TOOLBAR_TEXT				= 46;
// XP toolbar pushed text color.
const UINT XPCOLOR_PUSHED_TEXT				= 47;
// XP inactive tab background color.
const UINT XPCOLOR_TAB_INACTIVE_BACK		= 48;
// XP inactive tab text color.
const UINT XPCOLOR_TAB_INACTIVE_TEXT		= 49;
// XP border color for pushed in 3D elements.
const UINT XPCOLOR_HIGHLIGHT_PUSHED_BORDER	= 50;
// XP color for text displayed in a checked button.
const UINT XPCOLOR_CHECKED_TEXT				= XPCOLOR_HIGHLIGHT_TEXT;
// XP face color for three-dimensional display elements and for dialog box backgrounds.
const UINT XPCOLOR_3DFACE					= 51;
// XP shadow color for three-dimensional display elements (for edges facing away from the light source).
const UINT XPCOLOR_3DSHADOW					= 52;
// XP edit control border color.
const UINT XPCOLOR_EDITCTRLBORDER			= 53;
// XP frame color.
const UINT XPCOLOR_FRAME					= 54;
// XP splitter face color.
const UINT XPCOLOR_SPLITTER_FACE			= 55;
// XP last color.
const UINT XPCOLOR_LAST						= 55;


// CXTPPenDC类
//===========================================================================
// Summary: CXTPPenDC is a helper class used to create a pen using
//          the color specified by crColor.  The pen is then selected
//          into the device context specified by hDC.  When destroyed, the
//          class selects the previous pen back into the device context to 
//          handle GDI resource cleanup.
//===========================================================================
class CXTPPenDC
{
public:
	
	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPPenDC object and creates a pen using
	//          the color specified by crColor.  The pen is then selected
	//          into the device context specified by hDC.
	// Input:   hDC     - Handle to a valid device context.
	//          crColor - RGB value used to create pen.
	//-----------------------------------------------------------------------
	CXTPPenDC(HDC hDC, COLORREF crColor);
	
	//-----------------------------------------------------------------------
	// Summary: Destroys a CXTPPenDC object, handles cleanup and de-
	//          allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPenDC();
	
	//-----------------------------------------------------------------------
	// Summary: Call this member function to change the color used by the
	//          device context pen.
	// Input:   crColor - RGB value to change the pen color to.
	//-----------------------------------------------------------------------
	void Color(COLORREF crColor);
	
	//-----------------------------------------------------------------------
	// Summary: Call this member function to retrieve the color used by the
	//          device context pen.
	// Returns: An RGB value that represents the selected pen color.
	//-----------------------------------------------------------------------
	COLORREF Color();
	
protected:
	
	CPen m_pen;     // Pen selected into device context.
	HDC  m_hDC;     // Device context handle.
	HPEN m_hOldPen; // Handle to the previously selected pen.
};

// CXTPClientRect类
//===========================================================================
// Summary: CXTPClientRect is a helper class used to retrieve the client
//          area for a specified window.
//===========================================================================
class CXTPClientRect : public CRect
{
public:
	
	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPClientRect object used to retrieve the
	//          client area for the specified window.
	// Input:   hWnd - Handle to the window to retrieve the client area for.
	//-----------------------------------------------------------------------
	CXTPClientRect(HWND hWnd);
	
	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPClientRect object used to retrieve the
	//          client area for the specified window.
	// Input:   pWnd - Points to the window to retrieve the client area for.
	//-----------------------------------------------------------------------
	CXTPClientRect(const CWnd* pWnd);
};


// CXTPBufferDC类
//===========================================================================
// Summary: CXTPBufferDC is a helper class used to create a memory device
//          context used to draw to an offscreen bitmap.  When destroyed, the
//          class selects the previous bitmap back into the device context to 
//          handle GDI resource cleanup.
//===========================================================================
class CXTPBufferDC : public CDC
{
public:
	
	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPBufferDC object used to create a memory
	//          device context used to draw to an offscreen bitmap.
	// Input:   hDestDC - Handle to the destination device context the memory
	//                    device is BitBlt to.
	//          rcPaint - Size of the area to paint.
	//-----------------------------------------------------------------------
	CXTPBufferDC(HDC hDestDC, const CRect rcPaint);
	
	//-----------------------------------------------------------------------
	// Summary: Destroys a CXTPBufferDC object, handles cleanup and de-
	//          allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPBufferDC();
	
	//-----------------------------------------------------------------------
	// Summary: Call this method to retrieve a CDC pointer to the destination
	//          device context.
	//-----------------------------------------------------------------------
	CDC* GetDestDC();
	
protected:
	
	HDC     m_hDestDC;    // Handle to the destination device context.
	CBitmap m_bitmap;     // Bitmap in memory device context
	CRect   m_rect;       // Size of the area to paint.
	HGDIOBJ m_hOldBitmap; // Handle to the previously selected bitmap.
};




//===========================================================================
// Summary: Helper struct used by CXTPColorManager to display gradient color.
//===========================================================================
struct XTP_GRADIENT_COLOR
{
	
	COLORREF clrLight;	// Light color.
	COLORREF clrDark;	// Dark color.
	
	//-----------------------------------------------------------------------
	// Summary: Call this method to set the gradient colors for the
	//          structure.
	// Input:   _clrLight - RGB light color value
	//          _clrDark  - RGB dark color value
	//-----------------------------------------------------------------------
	void SetColors(COLORREF _clrLight, COLORREF _clrDark);
	
	//-----------------------------------------------------------------------
	// Summary: Overloaded assignment operator sets the value of the dark and
	//          light colors equal to the specified COLORREF value.
	// Input:   clr - RGB color value
	//-----------------------------------------------------------------------
	const XTP_GRADIENT_COLOR& operator= (const COLORREF& clr);
};

//===========================================================================
// Summary: CXTPColorManager is a stand alone class.  It is used to create a 
//			CXTPColorManager object. This object is used for manipulation of 
//          RGB values.
//===========================================================================
class CXTPColorManager
{
public:

	//-----------------------------------------------------------------------
	// Summary: This function will refresh the display element colors defined
	//          in the CXTPColorManager object.
	// Input:   bInit - TRUE when called for initialization.
	//-----------------------------------------------------------------------
	void RefreshColors(BOOL bInit = FALSE);

	// BULLETED LIST:

	//-----------------------------------------------------------------------
	// Summary: This function retrieves the current color of the specified XP
	//          display element. Display elements are the parts of a window 
	//          and the display that appear on the system display screen.  
	//          The function returns the red, green, blue (RGB) color value 
	//          of the given element.
	// Input:	nIndex - Specifies the display element whose color is to be 
    //                   retrieved.  This parameter can be one of the 
    //                   following values:
    //                   [ul]
    //                   [li]<b>XPCOLOR_BASE</b> - XP base color.[/li]
    //                   [li]<b>XPCOLOR_TOOLBAR_FACE</b> - XP toolbar
    //                       background color.[/li]
    //                   [li]<b>XPCOLOR_HIGHLIGHT</b> - XP menu item selected
	//                       color.[/li]
    //                   [li]<b>XPCOLOR_HIGHLIGHT_BORDER</b> - XP menu item 
	//                       selected border color.[/li]
    //                   [li]<b>XPCOLOR_HIGHLIGHT_PUSHED</b> - XP menu item
	//                       pushed color.[/li]
    //                   [li]<b>XPCOLOR_ICONSHADDOW</b> - XP menu item icon
	//                       shaddow.[/li]
    //                   [li]<b>XPCOLOR_GRAYTEXT</b> - XP menu item disabled
	//                       text color.[/li]
    //                   [li]<b>XPCOLOR_HIGHLIGHT_CHECKED</b> - XP menu item
	//                       checked color.[/li]
    //                   [li]<b>XPCOLOR_HIGHLIGHT_CHECKED_BORDER</b> - An RGB
	//                       value that represents the XP menu item checked
	//                       border color.[/li]
    //                   [li]<b>XPCOLOR_TOOLBAR_GRIPPER</b> - XP toolbar
    //                       gripper color.[/li]
    //                   [li]<b>XPCOLOR_SEPARATOR</b> - XP toolbar separator
	//                       color.[/li]
    //                   [li]<b>XPCOLOR_DISABLED</b> - XP menu icon disabled
	//                       color.[/li]
    //                   [li]<b>XPCOLOR_MENUBAR_FACE</b> - XP menu item text
	//                       background color.[/li]
    //                   [li]<b>XPCOLOR_MENUBAR_EXPANDED</b> - XP hidden menu
	//                       commands background color.[/li]
    //                   [li]<b>XPCOLOR_MENUBAR_BORDER</b> - XP menu border
	//                       color.[/li]
    //                   [li]<b>XPCOLOR_MENUBAR_TEXT</b> - XP menu item text
	//                       color.[/li]
    //                   [li]<b>XPCOLOR_HIGHLIGHT_TEXT</b> - XP menu item
	//                       selected text color.[/li]
    //                   [li]<b>XPCOLOR_TOOLBAR_TEXT</b> - XP toolbar text
	//                       color.[/li]
    //                   [li]<b>XPCOLOR_PUSHED_TEXT</b> - XP toolbar pushed
	//                       text color.[/li]
    //                   [li]<b>XPCOLOR_TAB_INACTIVE_BACK</b> - XP inactive
	//                       tab background color.[/li]
    //                   [li]<b>XPCOLOR_TAB_INACTIVE_TEXT</b> - XP inactive
	//                       tab text color.[/li]
    //                   [li]<b>XPCOLOR_HIGHLIGHT_PUSHED_BORDER</b> - An RGB
	//                       value that represents the XP border color for
	//                       pushed in 3D elements.[/li]
    //                   [li]<b>XPCOLOR_CHECKED_TEXT</b> - XP color for text
	//                       displayed in a checked button.[/li]
    //                   [li]<b>XPCOLOR_3DFACE</b> - XP face color for three-
	//                       dimensional display elements and for dialog box 
	//                       backgrounds.[/li]
    //                   [li]<b>XPCOLOR_3DSHADOW</b> - XP shadow color for 
	//                       three-dimensional display elements (for edges 
	//                       facing away from the light source).[/li]
    //                   [/ul]
	//-----------------------------------------------------------------------
	COLORREF GetColor(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to blend the light and dark colors
	//          by the amount specified in nDelta.
	// Input:   clrLight - RGB light color
	//          clrDark  - RGB dark color
	//          nDelta   - Amount saturation to blend colors.
	// Returns: An RGB color value representing the two blended colors.
	//-----------------------------------------------------------------------
	COLORREF LightColor(COLORREF clrLight, COLORREF clrDark, int nDelta);
	

	//-----------------------------------------------------------------------
	// Summary: Call this member function to disable the luna (Blue, Olive 
	//          and Green) colors for Office 2003 theme for Windows XP and 
	//          later operating systems.
	// Input:   bDisable - TRUE to disable luna colors, otherwise FALSE.
	//-----------------------------------------------------------------------
	void DisableLunaColors(BOOL bDisable = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Determines if Luna colors for Office 2003 style is disabled.
	// Returns: TRUE if the luna colors are disabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLunaColorsDisabled();

	//////////////////////////////////////////////////////////////////////
	// XP Color Functions
	//////////////////////////////////////////////////////////////////////  

	XTP_GRADIENT_COLOR grcCaption;				// Caption gradient color.
	XTP_GRADIENT_COLOR grcDockBar;				// Dockbar gradient color.
	XTP_GRADIENT_COLOR grcShortcutBarGripper;	// Shortcut bar gripper.

	//-----------------------------------------------------------------------
	// Summary:	Call this member to set the color for the specified index.  
	//          See GetColor for a complete list of index values.
	// Input:   nIndex - Color index.
	//          RGB color value to set.
	//-----------------------------------------------------------------------
	void SetColor(int nIndex, COLORREF clrValue);
	
	//-----------------------------------------------------------------------
	// Summary:	Call this member to set the colors for each element to the
	//          color in the specified color array.
	// Input:	cElements    - Number of elements in array.
	//			lpaElements  - Array of elements.
	//			lpaRgbValues - Array of RGB values.
	//-----------------------------------------------------------------------
	void SetColors(int cElements, CONST INT *lpaElements, CONST COLORREF* lpaRgbValues);

private:

	BOOL     m_bInit;
	BOOL     m_bDisableLunaColors;
	COLORREF m_arrColor[XPCOLOR_LAST + 1];
	
	CXTPColorManager();
	void RefreshSysColors();
	void RefreshXPColors();
	void RefreshGradientColors();
	float ColorWidth(int nLength, int nWidth);
	float ColorDelta(COLORREF clrA, COLORREF clrB);
	float Length(COLORREF clrA, COLORREF clrB);
	BOOL LongColor(COLORREF clrMain, COLORREF clrSub, BOOL bCalcLength, float fDistance);
	COLORREF MixColor(COLORREF clrMain, COLORREF clrSub, float fDistance);
	COLORREF AdjustColor(COLORREF clrMain, COLORREF clrSub, float fDistance);
	double GetRDelta(COLORREF clr); 
	double GetGDelta(COLORREF clr); 
	double GetBDelta(COLORREF clr); 

	// singleton.
	static CXTPColorManager m_instance;;
	friend CXTPColorManager* XTPColorManager();
};


//---------------------------------------------------------------------------
// Summary:	Call this member function to access CXTPColorManager members. 
//          Since this class is designed as a single instance object you can 
//          only access version info thru this method. You <b>cannot</b> 
//          directly instantiate an object of type CXTPColorManager.
// Example: <pre>COLORREF cr = CXTPColorManager()->GetColor(nIndex);</pre>
//---------------------------------------------------------------------------
AFX_INLINE CXTPColorManager* XTPColorManager() 
{ 
	return &CXTPColorManager::m_instance;; 
}
//---------------------------------------------------------------------------
// Summary:	Call this member as a convenient way to call
//          XTPColorManager()->GetColor, see CXTPColorManager::GetColor for
//          more details.
// Input:	nIndex - Specifies the display element whose color is to be 
//                   retrieved.
//---------------------------------------------------------------------------
AFX_INLINE COLORREF GetXtremeColor(UINT nIndex) 
{
	return XTPColorManager()->GetColor(nIndex);
}


// CXTPEmptyRect类
//===========================================================================
// Summary: CXTPEmptyRect is a helper class used to instantiate an empty
//          CRect object.
//===========================================================================
class CXTPEmptyRect : public CRect
{
public:
	
	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPEmptyRect object used to instantiate an
	//          empty CRect object.
	//-----------------------------------------------------------------------
	CXTPEmptyRect();
};


// CXTMemDC类
//===========================================================================
// Summary: CXTMemDC is a CDC derived class. CXTMemDC is an extension of CDC 
//          that helps eliminate screen flicker when windows are resized, by
//          painting to an off screen bitmap. The class then uses CDC::BitBlt
//          to copy the bitmap back into the current device context after all
//          items have been painted.
//===========================================================================
class   CXTMemDC : public CDC  
{
public:
    DECLARE_DYNAMIC(CXTMemDC);
    
	//-----------------------------------------------------------------------
	// Summary:	Constructs a CXTMemDC object used for flicker free drawing.
	// Input:	pDC      - A Pointer to the current device context. 
	//			rect     - Represents the size of the area to paint.
	//			clrColor - An RGB value that represents the current system 
	//                     face color of three dimensional display elements.
	//-----------------------------------------------------------------------
    CXTMemDC(CDC* pDC,const CRect& rect,COLORREF clrColor=GetSysColor(COLOR_3DFACE));
	
	//-----------------------------------------------------------------------
    // Summary: Destroys a CXTMemDC object, handles cleanup and de-allocation
	//-----------------------------------------------------------------------
    virtual ~CXTMemDC();
	
	//-----------------------------------------------------------------------
	// Summary: This member function is called to set the valid flag to false
	//          so that the offscreen device context will not be drawn.
	//-----------------------------------------------------------------------
	void Discard();
	
	//-----------------------------------------------------------------------
    // Summary: This member function gets content from the given DC
	//-----------------------------------------------------------------------
	void FromDC();
	
	//-----------------------------------------------------------------------
    // Summary:	This member function retrieves a reference to the CBitmap
	//          object associated with the memory device context. 
	// Returns: A reference to the CBitmap object associated with the memory
	//          device context.
	//-----------------------------------------------------------------------
	CBitmap& GetBitmap();
	
protected:
    
	CDC*	m_pDC;			// Saves the CDC passed in constructor.
	BOOL	m_bValid;		// Flag used for autodraw in destructor.
	CRect	m_rc;			// Rectangle of the drawing area.
	CBitmap m_bitmap;		// Offscreen bitmap.
	HBITMAP m_hOldBitmap;	// Original GDI object.
};

// XT_LOGFONT结构体
//////////////////////////////////////////////////////////////////////
// Summary: XT_LOGFONT is a self initializing LOGFONT derived structure class. It
//			will allow you to create or copy a LOGFONT object, and defines the attributes
//			of a font.
struct XT_LOGFONT : public LOGFONT
{
    // Summary: Constructs an XT_LOGFONT object.
    XT_LOGFONT();
	
	// Input:	logfont - Valid address of a LOGFONT structure.
	// Returns:	An XT_LOGFONT object.
    // Summary:	Copy constructor will construct an XT_LOGFONT object and copy the
	//			data specified by 'logfont' into the structure's data members.
    XT_LOGFONT(LOGFONT& logfont);
    
	// Input:	logfont - Valid address of a LOGFONT structure.
    // Summary:	This overloaded operator will copy the data specified by 'logfont'
    //			into the structure's data members.
    void operator = (LOGFONT& logfont);
	
	DWORD dwType;  // Used to hold the font type, i.e. TT_FONT, DEVICE_FONT.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE XT_LOGFONT::XT_LOGFONT() {
	::ZeroMemory(this, sizeof(XT_LOGFONT)); dwType = 0;
}    
AFX_INLINE XT_LOGFONT::XT_LOGFONT( LOGFONT& logfont ) {
	::CopyMemory((void*)&*this, (const void*) &logfont, (DWORD)sizeof(LOGFONT)); dwType = 0;
}
AFX_INLINE void XT_LOGFONT::operator = (LOGFONT& logfont) {
	::CopyMemory((void*)&*this, (const void*)&logfont, (DWORD)sizeof(LOGFONT));
}

//////////////////////////////////////////////////////////////////////
// CXTOSVersionInfo类
//===========================================================================
// Summary: CXTOSVersionInfo is a OSVERSIONINFO derived class. This class 
//          wraps the Win32 API GetVersionEx(...), used to get the current 
//          Windows OS version. CXTOSVersionInfo is a single instance, or 
//          "singleton" object, that is accessed with the Get() method.
//===========================================================================
class   CXTOSVersionInfo : public OSVERSIONINFO  
{
public:

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the 
	//          operating system is Windows 3.1.  
	// Returns:	true if the OS is Windows 3.1, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin31() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the 
	//          operating system is Windows 95.  
	// Returns: true if the OS is Windows 95, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin95() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the 
	//          operating system is Windows 98.  
	// Returns: true if the OS is Windows 98, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin98() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is Windows ME.  
	// Returns: true if the OS is Windows ME, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinME() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is Windows NT 4.  
	// Returns: true if the OS is Windows NT 4, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinNT4() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is Windows 2000.  
	// Returns: true if the OS is Windows 2000, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin2K() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is Windows XP.  
	// Returns: true if the OS is Windows XP, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinXP() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is greater than or equal to Windows 3.1. 
	// Returns: true if the OS is greater than or equal to Windows 3.1,
	//          otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin31OrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is of the Windows 9x family, and if it is
	//          Windows 95 or a later version. 
	// Returns: true if the OS is of the Windows 9x family, and is Windows 95
	///         or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin95OrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is of the Windows 9x family, and if it is
	//          Windows 98 or a later version. 
	// Returns: true if the OS is of the Windows 9x family, and is Windows 98
	//          or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin98OrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is of the Windows 9x family, and if it is
	//          Windows ME or a later version.
	// Returns: true if the OS is of the Windows 9x family, and is Windows ME
	//          or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinMEOrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is of the Windows NT family, and if it is
	//          Windows NT 4 or a later version.
	// Returns: true if the OS is of the Windows NT family, and is Windows NT
	//          4 or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinNT4OrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is of the Windows NT family, and if it is
	//          Windows 2000 or a later version. 
	// Returns: true if the OS is of the Windows NT family, and is Windows
	//          2000 or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin2KOrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:	Call this member function to check whether or not the
	//          operating system is of the Windows NT family, and if it is
	//          Windows XP or a later version. 
	// Returns: true if the OS is of the Windows NT family, and is Windows XP
	//          or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinXPOrGreater() const;

	//:Ignore - for compatibility with older versions use XTOSVersionInfo().
	static CXTOSVersionInfo& Get() { return m_instance; }
	//:End Ignore

private:

	//-----------------------------------------------------------------------
	// Summary: Checks to see if the OS is greater or equal to the specified
	//          version number
	// Returns: true if the OS is greater than or equal to the specified
	//          version.
	//-----------------------------------------------------------------------
	bool GreaterThanEqualTo(const DWORD maj, const DWORD min) const;

	//-----------------------------------------------------------------------
	// Summary: Checks to see if the OS is equal to the specified version
	//          number
	// Returns: true if the OS is equal to the specified version.
	//-----------------------------------------------------------------------
	bool EqualTo(const DWORD maj, const DWORD min) const;

	//-----------------------------------------------------------------------
    // Constructs a CXTOSVersionInfo object.
	//-----------------------------------------------------------------------
	CXTOSVersionInfo();

	// singleton
	static CXTOSVersionInfo  m_instance;
	friend CXTOSVersionInfo* XTOSVersionInfo();
};

//---------------------------------------------------------------------------
// Summary:	Call this member function to access CXTOSVersionInfo members. 
//          Since this class is designed as a single instance object you can 
//          only access version info thru this method. You <b>cannot</b> 
//          directly instantiate an object of type CXTOSVersionInfo.
// Example: <pre>bool bIsWinNT = XTOSVersionInfo()->IsWinNT4();</pre>
//---------------------------------------------------------------------------
AFX_INLINE CXTOSVersionInfo* XTOSVersionInfo() {
	return &CXTOSVersionInfo::m_instance;
}

//////////////////////////////////////////////////////////////////////
// Summary: XT_NONCLIENTMETRICS is a self initializing NONCLIENTMETRICS derived
//			structure class. It contains the scalable metrics associated with the
//			non-client area of a non-minimized window.  This structure is used by
//			the SPI_GETNONCLIENTMETRICS and SPI_SETNONCLIENTMETRICS actions of 
//			SystemParametersInfo.
struct XT_NONCLIENTMETRICS : public NONCLIENTMETRICS
{
    // Summary: Constructs an XT_NONCLIENTMETRICS object.
    XT_NONCLIENTMETRICS();
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE XT_NONCLIENTMETRICS::XT_NONCLIENTMETRICS() { 
	memset(this, 0, sizeof(NONCLIENTMETRICS)); cbSize = sizeof(NONCLIENTMETRICS); /*// Retrieves the value of the specified system-wide parameter.*/VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), this, 0));
}

















// XT_AUX_DATA结构体
//////////////////////////////////////////////////////////////////////
// Summary: XT_AUX_DATA is a stand alone global item data structure class. It is
//			used by the toolkit to initialize and store resource and item data shared
//			by all objects.  Items include system colors, icons, fonts and OS flags.
struct XT_AUX_DATA
{
private:
	
    // Summary: Constructs an XT_AUX_DATA object.
    XT_AUX_DATA();

public:

    // Summary: Destroys an XT_AUX_DATA object, handles cleanup and de-allocation.
    virtual ~XT_AUX_DATA();

	////////////////////////////////////////////////////////////////////
    // (RGB) System colors used by Xtreme Toolkit
	////////////////////////////////////////////////////////////////////

	COLORREF clr3DFace;						// An RGB value that represents the face color for three dimensional display elements.
	COLORREF clr3DShadow;					// An RGB value that represents the shadow color for three dimensional display elements.
	COLORREF clr3DDkShadow;					// An RGB value that represents the dark shadow for three dimensional display elements.
	COLORREF clr3DHilight;					// An RGB value that represents the highlight color for three dimensional display elements.
	COLORREF clr3DLight;					// An RGB value that represents the light color for three dimensional display elements.
	COLORREF clrBtnText;					// An RGB value that represents the text color on push buttons.
	COLORREF clrGrayText;					// An RGB value that represents the grayed (disabled) text.
	COLORREF clrHighlight;					// An RGB value that represents the item(s) selected in a control.
	COLORREF clrHighlightText;				// An RGB value that represents the text color of item(s) selected in a control.
	COLORREF clrMenu;						// An RGB value that represents the menu background.
	COLORREF clrMenuText;					// An RGB value that represents the text color in menus.
	COLORREF clrWindow;						// An RGB value that represents the window background.
	COLORREF clrWindowFrame;				// An RGB value that represents the window frame.
	COLORREF clrWindowText;					// An RGB value that represents the text color in windows.
	COLORREF clrActiveCaption;				// An RGB value that represents the active window title bar.
	COLORREF clrInActiveCaption;			// An RGB value that represents the inactive window title bar.
	COLORREF clrGradActiveCapt;				// An RGB value that represents the gradient active title bar.
	COLORREF clrGradInActiveCapt;			// An RGB value that represents the gradient inactive title bar.
	COLORREF clrActiveCaptText;				// An RGB value that represents the active caption text.
	COLORREF clrInactiveCaptText;			// An RGB value that represents the inactive caption text.
	COLORREF clrXPBarFace;					// An RGB value that represents the XP toolbar background color.
	COLORREF clrXPHighlight;				// An RGB value that represents the XP menu item selected color.
	COLORREF clrXPHighlightBorder;			// An RGB value that represents the XP menu item selected border color.
	COLORREF clrXPHighlightPushed;			// An RGB value that represents the XP menu item pushed color.
	COLORREF clrXPIconShadow;				// An RGB value that represents the XP menu item icon shadow.
	COLORREF clrXPGrayText;					// An RGB value that represents the XP menu item disabled text color.
	COLORREF clrXPHighlightChecked;			// An RGB value that represents the XP menu item checked color.
	COLORREF clrXPHighlightCheckedBorder;	// An RGB value that represents the XP menu item checked border color.
	COLORREF clrXPGripper;					// An RGB value that represents the XP toolbar gripper color.
	COLORREF clrXPSeparator;				// An RGB value that represents the XP toolbar separator color.
	COLORREF clrXPDisabled;					// An RGB value that represents the XP menu icon disabled color.
	COLORREF clrXPMenuTextBack;				// An RGB value that represents the XP menu item text background color.
	COLORREF clrXPMenuExpanded;				// An RGB value that represents the XP hidden menu commands background color.
	COLORREF clrXPMenuBorder;				// An RGB value that represents the XP menu border color.
	COLORREF clrXPMenuText;					// An RGB value that represents the XP menu item text color.
	COLORREF clrXPHighlightText;			// An RGB value that represents the XP menu item selected text color.
	COLORREF clrXPBarText;					// An RGB value that represents the XP toolbar text color.
	COLORREF clrXPBarTextPushed;			// An RGB value that represents the XP toolbar pushed text color.
	COLORREF clrXPTabInactiveBack;			// An RGB value that represents the XP inactive tab background color.
	COLORREF clrXPTabInactiveText;			// An RGB value that represents the XP inactive tab text color.
	
	////////////////////////////////////////////////////////////////////
    // Cursors used by Xtreme Toolkit
	////////////////////////////////////////////////////////////////////

	HCURSOR hcurDragCopy;	// Drag copy.
	HCURSOR hcurDragMove;	// Drag move.
	HCURSOR hcurDragNone;	// Drag none.
	HCURSOR hcurHand;		// Hand.
	HCURSOR hcurHandNone;	// No Hand.
	HCURSOR hcurHSplitBar;	// Horizontal Splitter.
	HCURSOR hcurVSplitBar;	// Vertical Splitter.
	HCURSOR hcurMove;		// 4 way move.
	
	////////////////////////////////////////////////////////////////////
    // System metrics for small icons.
	////////////////////////////////////////////////////////////////////

	int cxSmIcon;		// cx small icon size (width).
	int cySmIcon;		// cy small icon size (height).
	int cxSize;			// Width, in pixels, of a button in a window's caption or title bar.
	int cySize;			// Height, in pixels, of a button in a window's caption or title bar.
	int cxHThumb;		// Width, in pixels, of the thumb box in a horizontal scroll bar.
	int cyVThumb;		// Height, in pixels, of the thumb box in a vertical scroll bar.
	int cyMenuItem;		// Height, in pixels, of single-line menu bar. 
	int nMenuAnimation; // Type of menu animation.
	
	////////////////////////////////////////////////////////////////////
    // Fonts used by Xtreme Toolkit
	////////////////////////////////////////////////////////////////////

	CFont font;				// Default GUI font.
	CFont fontBold;			// Default bold GUI font.
	CFont fontULine;		// Default underlined GUI font.
	CFont fontHCapt;		// Default horizontal caption font.
	CFont fontVCapt;		// Default vertical caption font.
	CFont fontVCaptBold;	// Default vertical caption bold font.
	CFont fontVCaptR;		// Default vertical caption font, right aligned.
	CFont fontVCaptRBold;	// Default vertical caption bold font, right aligned.

	////////////////////////////////////////////////////////////////////
    // OS flags used by Xtreme Toolkit
	////////////////////////////////////////////////////////////////////

	int			iComCtlVersion;			// Common control dll (comctl32.dll) version information.
	BOOL		bWin95;					// TRUE if Win 95.
	BOOL		bWin98;					// TRUE if Win 98.
	BOOL		bWinNT;					// TRUE if Win NT.
	BOOL		bWin2K;					// TRUE if Win 2000.
	BOOL		bWinXP;					// TRUE if Win XP.
	BOOL		bCoolMenuCompatMode;	// TRUE to enable cool menu compatibility mode.
	BOOL		bXPMode;				// TRUE to use XP style menus.
	BOOL		bMenuShadows;			// TRUE to use shadows under the menus.
	BOOL		bToolBarVisualize;		// TRUE to render the toolbar while dragging or resizing.
	BOOL		bControlBarMenus;		// TRUE to display right click control bar menus.
	BOOL		bDockBarMenus;			// TRUE to display right click dockbar menus.
	BOOL		bMenuRecentCommands;	// TRUE to use intelligent menus to hide selected menu commands.
	BOOL		bShowFullAfterDelay;	// TRUE to display hidden menu commands after a short delay.
	BOOL		bToolBarScreenTips;		// TRUE to show tooltips on toolbar commands.
	BOOL		bToolBarAccelTips;		// TRUE to add accelerator keys to toolbar tips.
	BOOL		bDisableFadedIcons;		// TRUE to disable icons fading .
	BOOL		bUseSolidShadows;		// TRUE to use solid shadows in Operation Systems that are not supported layered windows.
	BOOL        bUseOfficeFont;			// TRUE to use the Tahoma font if found, good for Office style applications by default.
	CString		strINIFileName;			// Name of an ini file for registry settings.
	HINSTANCE	hInstance;				// Instance handle for resources.

    // Summary: This member function loads the cursors used by Xtreme Toolkit.
    void LoadSysCursors();

    // Summary: This member function frees cursor resources.
    void FreeSysCursors();

    // Summary: This member function creates the fonts used by Xtreme Toolkit.
    void LoadSysFonts();

    // Summary: This member function frees font resources.
    void FreeSysFonts();

    // Summary: This member function updates system colors used by Xtreme Toolkit.
    void UpdateSysColors(); 

    // Summary: This member function updates system metrics used by Xtreme Toolkit.
    void UpdateSysMetrics();

	// Returns: A DWORD value.
    // Summary:	This member function is called to retrieve the version information
	//			for the common control dll (comctl32.dll). 
    DWORD GetComCtlVersion();

	// Input:	hInst - Instance handle for resources.
    // Summary:	This member function is called by the library to initialize resources.
    void InitResources(HINSTANCE hInst);

	// Input:	pFont - Points to a valid CFont object that is used to define the fonts
	//			the toolkit will use.
	//			pVertFont - Points to a valid CFont object that is used to define the vertical
	//			fonts used by the toolkit.  If NULL, 'pFont' will be used.
	// Example: <pre>
	//			CGUI_VisualStudioApp::CGUI_VisualStudioApp()
    //			{
    //			    // TODO: add construction code here,
    //			    m_bFirstTime = true;
    //			
    //			    // Place all significant initialization in InitInstance
    //			    xtAfxData.SetGlobalFont( _T( "Verdana" ) );
    //			}</pre>
    // Summary:	Call this member function to set the font the toolkit will use.
	//			This member function should be called from the constructor of your
	//			CWinApp derived class for the fonts to be properly initialized.
    void SetGlobalFont(CFont* pFont,CFont* pVertFont=NULL);

	// Input:	lpszFaceName - Points to a NULL terminated string that is used to define the fonts
	//			the toolkit will use.
	//			lpszVertFaceName - Points to a NULL terminated string that is used to define the vertical
	//			fonts used by the toolkit.  If NULL, the name defined in 'lpszFaceName'
	//			will be used.
    // Example: <pre>
	//			CGUI_VisualStudioApp::CGUI_VisualStudioApp()
    //			{
    //			    // TODO: add construction code here,
    //			    m_bFirstTime = true;
    //			
    //			    // Place all significant initialization in InitInstance
    //			    xtAfxData.SetGlobalFont( _T( "Verdana" ) );
    //			}</pre>
    // Summary:	Call this member function to set the font the toolkit will use.
	//			This member function should be called from the constructor of your
	//			CWinApp derived class for the fonts to be properly initialized.
    void SetGlobalFont(LPCTSTR lpszFaceName,LPCTSTR lpszVertFaceName=NULL);

	// Input:	lpszINIFullPath - Full path to the ini file.
    // Summary:	Call this member function to store registry settings in an ini file
    //			instead of the system's registry.
    void SetINIFileName(LPCTSTR lpszINIFullPath);
	
	// Returns: An LPCTSTR data type.
    // Summary:	This member function will get the full path to the ini file
    //			used to store registry settings. 
    LPCTSTR GetINIFileName();

	// Example: <pre>XT_AUX_DATA::Get().InitResources( m_hInstance );</pre>
	// Returns: A reference to the one and only XT_AUX_DATA object.
	// Summary:	This static member function will retrieve a reference to the one
	//			and only XT_AUX_DATA object.  You can use this function to access
	//			data members for the XT_AUX_DATA structure.  You can also use the
	//			macro xtAfxData.
	static XT_AUX_DATA& AFX_CDECL Get();

	// Input:	bVerNumOnly - true to return the version number only, minus "Xtreme Toolkit v".
	// Returns: A NULL terminated string that indicates the version of the Xtreme Toolkit.
	// Summary:	Call this member function to retrieve a CString object that represents
	//			the current version of the Xtreme Toolkit.  The string returned is
	//			formatted like so: "Xtreme Toolkit v1.94". 
	CString GetXTVersion(bool bVerNumOnly=false) const;

    // Summary: Call this member function to use the Office font (Tahoma) if it exists.
    void UseOfficeFont();

private:
	bool FontExists(CString& strFaceName);
	bool CreateSysFont(XT_LOGFONT& lf, CFont& font, long lfWeight=FW_NORMAL, char lfUnderline=0, long lfOrientation=0, long lfEscapement=0);
};

//:Associate with "Global Data"

//////////////////////////////////////////////////////////////////////
// Summary: XT_AUX_DATA is a singleton object and can only be instantiated
//			one time.  The macro xtAfxData is used to access the members of this
//			structure.
#define xtAfxData XT_AUX_DATA::Get()





#endif





