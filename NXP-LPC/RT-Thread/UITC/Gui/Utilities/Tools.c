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
**
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
#include "includes.h"  
#include "global.h"
#include "ScreenBase.h"
#include "Gui.h"
 


/*******************************************************************/
//	10的倍数计算
/*******************************************************************/
const INT32U TenMultiple[]=
{
	1,
	10,
	100,
	1000,
	10000,
	100000,
	1000000,
	10000000,
	100000000
};

extern void NormalizeRect(LPLCDRECT pRect)
{
	LcdPos nTemp;
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
 
extern void SetLcdRect(LCDRECT *pRect, LcdPos l, LcdPos t, LcdPos r, LcdPos b)
{
	pRect->left = l; pRect->top = t; pRect->right = r; pRect->bottom = b; 
}

extern void SetLcdRectEmpty(LCDRECT * pRect)
{
	pRect->left=0;pRect->right=0;pRect->top=0;pRect->bottom=0; 
}

/************************************************************************/
//  几个元件位置定义
/************************************************************************/
const LCDRECT LCD_RECT_MENU={0,16,GetLCDWidth()-1,GetLCDHeight()-1};	//菜单位置
const LCDRECT LCD_RECT_VARBAR={GetLCDWidth()-16,0,GetLCDWidth()-1,GetLCDHeight()-1};//滚动条位置
const LCDRECT LCD_RECT_NUMEDIT={0,24,88,31};		//数字编辑框位置
const LCDRECT LCD_RECT_ENUMEDIT={0,16,120,31};		//枚举编辑框位置

const LCDPOINT LCD_POINT_EDIT_TOOLTIP={0,16};		//定义编辑框提示信息位置
const LCDPOINT LCD_POINT_EDIT_UNIT={GetLCDWidth()-32,GetLCDHeight()-8};			//定义编辑框单位信息位置
const LCDPOINT LCD_POINT_EDIT_CAPTION={0,0};		//定义编辑框标题信息位置
