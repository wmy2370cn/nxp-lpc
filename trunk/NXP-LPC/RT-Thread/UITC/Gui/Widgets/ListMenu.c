/****************************************Copyright (c)**************************************************
**                                     LIST MENU 菜单的实现 
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  
#include "ListMenu.h" 
#include "DC.h"
#include "icon.h"


#define MENU_OFFSET (40) //菜单文字与左边间距

/*********************************************************************************************************
** 函数名称: OnPaintCtrl
**
** 功能描述： 显示菜单 内部私有接口
**
** 输　入:  CLcdMenu * pMenu
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月22日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void OnPaintCtrl(CCtrlBase *pCtrl)
{
	INT8U i,start,end;
	INT8U row = 0;
	CGuiListMenu *pListBox = (CGuiListMenu *)pCtrl;
	CGuiRect rcBlock;

	char szTxt[16] = {0};

	if (pListBox == NULL)
		return;

	if(pListBox->ItemCount == 0)
	{
		DrawStringByStr( pListBox->Block.left,pListBox->Block.top,"NO MENU ITEM",PEN_COPY,ALIGN_LEFT,LCD_EN_FONT_8_8);
		return;
	}
	// 显示菜单
	start =pListBox->CurItem/pListBox->MaxShowLines* (pListBox->MaxShowLines);
	end   = start + pListBox->MaxShowLines-1;
	if(end>=pListBox->ItemCount)
		end = pListBox->ItemCount-1;

	for( i = start; i <= end; i++)
	{
		row=i-start;
		//DrawStringByStr(pMenu->Base.Block.left,pMenu->Base.Block.top+row*16,"." ,PEN_COPY,ALIGN_LEFT,LCD_EN_FONT_8_8);
		snprintf(szTxt,sizeof(szTxt),"%d",row+start+1);
	//	DrawStringByStr( LCD_WIDTH>>1,32,szTxt,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);
		DrawStringByStr( pListBox->Block.left+MENU_OFFSET+16,pListBox->Block.top+row*16,szTxt,PEN_COPY,ALIGN_LEFT,LCD_EN_FONT_8_16);

		// 反白显示菜单选择
		if( i== pListBox->CurItem)
		{
			//显示小手
			DrawIcon(pListBox->Block.left+MENU_OFFSET ,pListBox->Block.top+row*16,ID_ICON_HAND, PEN_COPY);
			// 显示菜单名称
		//	DrawStringById(pListBox->Block.left+MENU_OFFSET+32,pListBox->Block.top+row*16,pMenu->MenuLink[i].nResourceID, PEN_COPY, ALIGN_LEFT,LCD_EN_FONT_8_16);
			SetGuiRect(&rcBlock,pListBox->Block.left+MENU_OFFSET+24,pListBox->Block.top+row*16,pListBox->Block.right-MENU_OFFSET,pListBox->Block.top+(row+1)*16-1);
			ReverseClrRect(&rcBlock);// 显示黑块	
	 	}
		else
		{
			//小手位置
		//	SetGuiRect(&rcBlock,pMenu->Base.Block.left+MENU_OFFSET,pMenu->Base.Block.top+row*16,pMenu->Base.Block.left+MENU_OFFSET+16 ,pMenu->Base.Block.top+(row+1)*16-1);
		//	ClrRectScreenBuf(&rcBlock,COLOR_BK);
			// 显示菜单名称
		//	SetGuiRect(&rcBlock,pMenu->Base.Block.left+MENU_OFFSET+24,pMenu->Base.Block.top+row*16,pMenu->Base.Block.right-MENU_OFFSET,pMenu->Base.Block.top+(row+1)*16-1);
		//	ClrRectScreenBuf(&rcBlock,COLOR_BK);
		//	DrawStringById(pMenu->Base.Block.left+MENU_OFFSET+32,pMenu->Base.Block.top+row*16,pMenu->MenuLink[i].nResourceID, PEN_COPY, ALIGN_LEFT,LCD_EN_FONT_8_16);
		}
	} 
// 	if(pMenu->ShowScrollBar)
// 	{
// 		DispScrollBar(pMenu);
// 	}
	InvalidateScreen(FALSE);
}
static void ListMenuCtor( CCtrlBase *pCtrl )
{

}

static void ListMenuDtor( CCtrlBase *pCtrl )
{
	ASSERT(pCtrl);

}
/*********************************************************************************************************
** 函数名称: ListMenuCtrlProc
** 函数名称: ListMenuCtrlProc
**
** 功能描述：  
**
** 输　入:  GUI_HWND hWnd
** 输　入:  INT32U uMsg
** 输　入:  GUI_WPARAM wParam
** 输　入:  GUI_LPARAM lParam
**          
** 输　出:   INT32U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月13日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT32U ListMenuCtrlProc (GUI_HWND hWnd, INT32U uMsg, GUI_WPARAM wParam, GUI_LPARAM lParam)
{

	return 0;
}
/*********************************************************************************************************
** 函数名称: InitListMenu
** 函数名称: InitListMenu
**
** 功能描述：  
**
** 输　入:  CGuiListMenu * pMenu
** 输　入:  INT32U ResID
** 输　入:  CGuiRect * pRc
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月13日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U InitListMenu(CGuiListMenu *pMenu, INT32U ResID, CGuiRect *pRc )
{
	ASSERT(pMenu);
	if(pMenu == NULL)
		return FALSE;

	InitControl((CCtrlBase*)pMenu,ListMenuCtor,ListMenuDtor);
	pMenu->ControlProc = ListMenuCtrlProc;
	pMenu->ID = ResID;
	CopyGuiRect(& (pMenu->Block), pRc);






	return TRUE;
}


