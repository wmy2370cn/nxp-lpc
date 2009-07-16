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
#include "GuiConst.h" 
#include "global.h"
#include "DC.h"
#include "MsgBox.h"
#include "Icon.h"
 

static LcdBuffer m_LcdSave;



extern void ShowMsgBoxStr(const char * szMsg, INT8U mode)
{
	ClearScreen();

	// 信息
	DrawStringByStr(1, 0,szMsg, PEN_COPY, ALIGN_LEFT,LCD_EN_FONT_8_8);

	// 图标及按钮
	if(mode & LCD_MB_YESNO)  // 有YES NO按键
	{
		DrawRect(32,20,48,30,COLOR_FR);
	    DrawIcon(36,21,ID_ICON_FALSE,PEN_COPY);
		DrawRect(64,20,80,30,COLOR_FR);
		DrawIcon(68,21,ID_ICON_TRUE,PEN_COPY);
	}
	else if(mode & LCD_MB_OK) // 有确认按键
	{
		DrawRect(48,20,80,30,COLOR_FR);
		DrawStringByStr(52,21,"ok" , PEN_COPY, ALIGN_LEFT,LCD_EN_FONT_8_8);
	}

	if(mode & LCD_MB_INFO) // 有信息图标
	{
		DrawIcon(13<<3,0,ID_ICON_INFO,PEN_COPY);
	}
	else if(mode & LCD_MB_WARN) // 有告警图标
	{
		DrawIcon(13<<3,0,ID_ICON_WARN,PEN_COPY);
	}

	// 显示边框
	DrawRect(0,0,121,31,COLOR_FR);	
}

void ShowMsgBoxID(INT16U  nID, INT8U mode)
{
	ClearScreen();

	DrawStringById(1,0,nID, PEN_COPY, ALIGN_LEFT,LCD_EN_FONT_8_8);// 信息

	// 图标及按钮
	if(mode & LCD_MB_YESNO)  // 有YES NO按键
	{
		DrawRect(32,20,48,30,COLOR_FR);
		DrawIcon(36,21,ID_ICON_FALSE,PEN_COPY);
		DrawRect(64,20,80,30,COLOR_FR);
		DrawIcon(68,21,ID_ICON_TRUE,PEN_COPY);

	}
	else if(mode & LCD_MB_OK) // 有确认按键
	{
		DrawRect(48,20,80,30,COLOR_FR);
		DrawStringByStr(52,21,"ok", PEN_COPY, ALIGN_LEFT,LCD_EN_FONT_8_8);
	}

	if(mode & LCD_MB_INFO) // 有信息图标
	{
		DrawIcon(13<<3,0,ID_ICON_INFO,PEN_COPY);
	}
	else if(mode & LCD_MB_WARN) // 有告警图标
	{
		DrawIcon(13<<3,0,ID_ICON_WARN,PEN_COPY);
	}

	// 显示边框
	DrawRect(0,0,121,31,COLOR_FR); 		
}
