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
#ifndef GUI_DC_H
#define	GUI_DC_H 
#pragma  once

#include "GuiDef.h"
#include "LcdDriver.h"

typedef INT8U LCD_BUFF[LCD_WIDTH][LCD_WIDTH>>3];


typedef enum LCD_EN_FONT
{
	LCD_EN_FONT_8_8,     // 英文字符大小8×8
	LCD_EN_FONT_8_16     // 英文字符大小8×16
}LCD_EN_FONT;


//画笔颜色
typedef enum GUI_COLOR
{
	GUI_COLOR_BK	=0,		//背景色
	GUI_COLOR_FR	=1		//前景色
}GUI_COLOR;

// 画笔模式
typedef enum GUI_PEN
{
	PEN_COPY,		// 直接copy样子
	PEN_NOT,		// NOT
	PEN_OR,			// OR
	PEN_XOR,        // XOR
	PEN_AND			//AND
}GUI_PEN;

// 对齐方式
typedef enum GUI_ALIGN
{
	ALIGN_LEFT,     //左对齐
	ALIGN_CENTER,   //居中对齐
	ALIGN_RIGHT     //右对齐
}GUI_ALIGN;


/* DrawText 格式*/
#define DT_VMASK	    	0x0000000F
#define DT_TOP		    	0x00000001         ///< 顶对齐
#define DT_VCENTER	     	0x00000002         ///< 垂直居中对齐
#define DT_BOTTOM	     	0x00000004         ///< 底对齐
#define DT_HMASK	    	0x000000F0
#define DT_LEFT		    	0x00000010         ///< 左对齐
#define DT_CENTER	    	0x00000020         ///< 水平居中对齐
#define DT_RIGHT	    	0x00000040         ///< 右顶对齐


__inline INT16U GetLcdWidth( )
{
	return LCD_WIDTH;
}


//画线
void DrawLine(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr);
void DrawLineEx(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr/*=COLOR_FR*/, GUI_PEN pen/*=PEN_COPY*/);


void DrawBlock(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr/*=COLOR_FR*/);

void DrawBlockRect( CGuiRect *pRect, GUI_COLOR cr);

void DrawRect(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr/*=COLOR_FR*/);

void DrawStringByStr(LCD_POS x, LCD_POS y,const char * pString, GUI_PEN pen/*=PEN_COPY*/, GUI_ALIGN nAlign/*=ALIGN_LEFT*/,INT8U nEnFontType);
void DrawStringById(LCD_POS x, LCD_POS y, INT16U uStringResID, GUI_PEN pen/*=PEN_COPY*/, GUI_ALIGN nAlign/*=ALIGN_LEFT*/,INT8U nEnFontType);

void DrawIcon(LCD_POS x, LCD_POS y, INT16U iIconID, GUI_PEN pen/*=PEN_COPY*/);

//液晶重绘
void ClearScreen(void);
void ClearScreenEx(INT8U bClear/* = FALSE*/);


void ClrRectScreenBuf(CGuiRect* rc,GUI_COLOR color);

void InvalidateScreen( INT8U bErase );
void UpdateScreen(  INT8U bErase);

void ReverseClrRect(CGuiRect * rc);
#endif

