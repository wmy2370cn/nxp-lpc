 /****************************************Copyright (c)**************************************************
**							     
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:
**创   建   人:
**最后修改日期:
**描        述:   
**备        注 :
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本:  
** 日　期:  
** 描　述: 
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#pragma once

#include "applib.h"

typedef INT32U GUI_HWND;

typedef INT32U GUI_HDC;

#define HWND_NULL        0
#define HWND_INVALID       (INT32U)(-1)

#define HDC_INVALID      0

#define TYPE_SCREEN     0x11
#define TYPE_CONTROL    0x12

//坐标位置
#ifndef LCD_POS
#	define LCD_POS INT16U
#endif

struct GUI_POINT
{
	INT16U x, y;
};
typedef struct GUI_POINT CGuiPoint;

struct GUI_RECT
{
	INT16U left, top, right, bottom;
};
typedef struct GUI_RECT CGuiRect;



#define GUI_RECT_WIDTH(r)	((r).right - (r).left)
#define GUI_RECT_HEIGHT(r)	((r).bottom - (r).top)

void NormalizeGuiRect(CGuiRect *pRect);
void SetGuiRect(CGuiRect *pRect, LCD_POS l, LCD_POS t, LCD_POS r, LCD_POS b);
void SetGuiRectEmpty(CGuiRect * pRect);
void CopyGuiRect(CGuiRect* pdrc, const CGuiRect* psrc);
INT8U IsGuiRectEmpty(const CGuiRect* prc);
INT8U EqualGuiRect(const CGuiRect* prc1, const CGuiRect* prc2);
INT8U IsGuiRectCovered(const CGuiRect* prc1, const CGuiRect* prc2);
INT8U IntersectGuiRect(CGuiRect* pdrc, const CGuiRect* psrc1, const CGuiRect* psrc2);
INT8U DoesIntersect (const CGuiRect* psrc1, const CGuiRect* psrc2);
INT8U UnionGuiRect(CGuiRect* pdrc, const CGuiRect* psrc1, const CGuiRect* psrc2);
void GetBoundGuiRect(CGuiRect * pdrc,  const CGuiRect* psrc1, const CGuiRect* psrc2);
INT16U SubtractGuiRect(CGuiRect* rc, const CGuiRect* psrc1, const CGuiRect* psrc2);
void  OffsetGuiRect(CGuiRect* prc, INT16U x, INT16U y);
void InflateGuiRect(CGuiRect* prc, INT16U cx, INT16U cy);
void InflateGuiRectToPt (CGuiRect* prc, INT16U x, INT16U y);
INT8U PtInGuiRect(const CGuiRect* prc, INT16U x, INT16U y);


struct GUI_SIZE
{
    /**
     * the extent in x coordinate of a 2D object.
     */
	INT16U cx;
    /**
     * the extent in y coordinate of a 2D object.
     */
	INT16U cy;
} ;

typedef struct GUI_SIZE CGuiSize;

//定义字符串最大长度
#define STRING_MAXLEN 256

//汉字库索引启示地址
#define CNZI_FIRST_INDEX 10000

/**
 * \def TABLESIZE(table)
 * \brief A macro returns the number of elements in a \a table.
 */
#define TABLESIZE(table)    (sizeof(table)/sizeof(table[0]))

/* MAX/MIN/ABS macors */
/**
 * \def MAX(x, y)
 * \brief A macro returns the maximum of \a x and \a y.
 */
#ifndef MAX
#define MAX(x, y)           ((x > y)?x:y)
#endif
/**
 * \def MIN(x, y)
 * \brief A macro returns the minimum of \a x and \a y.
 */
#ifndef MIN
#define MIN(x, y)           ((x < y)?x:y)
#endif
/**
 * \def ABS(x)
 * \brief A macro returns the absolute value of \a x.
 */
#ifndef ABS
#define ABS(x)              (((x)<0) ? -(x) : (x))
#endif

/**
 * \var typedef unsigned int WPARAM
 * \brief A type definition for the first message paramter.
 */
typedef INT32U    GUI_WPARAM;

/**
 * \var typedef unsigned long LPARAM
 * \brief A type definition for the second message paramter.
 */
typedef INT32U   GUI_LPARAM;



//画笔颜色
enum GUI_COLOR_DEF
{
	GUI_COLOR_BK	=0,		//背景色
	GUI_COLOR_FR	=1		//前景色
};

typedef enum GUI_COLOR_DEF GUI_COLOR;
