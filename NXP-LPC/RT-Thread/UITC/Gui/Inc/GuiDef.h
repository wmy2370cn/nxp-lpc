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

typedef INT32U GUI_HANDLE;


#define GUI_RECT_WIDTH(r)	((r).right - (r).left)
#define GUI_RECT_HEIGHT(r)	((r).bottom - (r).top)


__inline void NormalizeRect(CGuiRect *pRect)
{
	LCD_POS nTemp;
	if (pRect)
	{
		if (pRect->left > pRect->right)
		{
			nTemp = pRect->left;
			pRect->left = pRect->right;
			pRect->right = nTemp;
		}
		if (pRect->top > pRect->bottom)
		{
			nTemp = pRect->top;
			pRect->top = pRect->bottom;
			pRect->bottom = nTemp;
		}
	}
}

__inline void SetGuiRect(CGuiRect *pRect, LCD_POS l, LCD_POS t, LCD_POS r, LCD_POS b)
{
	pRect->left = l; pRect->top = t; pRect->right = r; pRect->bottom = b;
}

__inline void SetGuiRectEmpty(CGuiRect * pRect)
{
	pRect->left=0;pRect->right=0;pRect->top=0;pRect->bottom=0;
}


//定义字符串最大长度
#define STRING_MAXLEN 256

//汉字库索引启示地址
#define CNZI_FIRST_INDEX 10000


