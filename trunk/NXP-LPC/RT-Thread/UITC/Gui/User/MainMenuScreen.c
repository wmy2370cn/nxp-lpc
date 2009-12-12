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
#include <stdlib.h> 
#include "ScreenBase.h"
#include "MainMenuScreen.h"
#include "StringTable.h" 
#include "ExtScreenDef.h"
#include "DC.h"
 
 
/************************************************************************
* 用到的全局变量
************************************************************************/
 
const CListMenuItem MainMenu[]=
{
 	{ID_REALTIME_VAL,	ID_STRING_VALUE_TAB},  //实测值一览表
  	{ID_GATE_CTRL,		ID_STRING_GATE_CTRL }, //闸门操作
	{ ID_DEVICE_CFG, ID_STRING_DEVICE_CFG  },   //装置参数
	{ ID_RUN_STAT, ID_STRING_RUN_STAT }, //闸门运行统计
	{ ID_VER_INFO, ID_STRING_VER_INFO } //版本信息
};


static void OnMessage(INT32U msg, INT32U param)  
{
#if 0
	switch ( msg )
	{
	case LCD_WM_INI:
		{
			ClearScreen();
			DrawScreenCaption((CScreen*)&MainMenuScreen,0,0  );
			//	DrawStringById(LCD_WIDTH>>1,0,ID_STRING_DEVICE_CFG,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_16);
			// 	DrawStringById(LCD_WIDTH>>1,16,ID_STRING_REALTIME_VAL,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);	

			DrawScreenStatusBar( );
			InvalidateScreen( FALSE );
		}
		break;
	case LCD_WM_REFRASH: 
		{ 
			UpdateScreen(TRUE);		
		}
		break;
	case LCD_WM_TIMER:
		{
			
		}
		break;
	default:
		break;
	}
	OnCtrlMsg((CCtrl *) &MainMenuScreen.Menu,msg,param );

#endif

}


//按键处理虚函数 
static void OnKeyDown (INT16U keyCode)   
{	
#if 0
	if(keyCode == KEY_3)
		// 退出该窗口，进入下一级窗口
		SwitchScreen(&g_ScreeLib,  GetCurrentMenuID(& MainMenuScreen.Menu) );
 	else if(keyCode == KEY_4)	
		// 退出该窗口，进入父窗口
		SwitchScreen(&g_ScreeLib,MainMenuScreen.Base.ParentID);

	// 菜单键盘响应函数
// 	if (MainMenuScreen.Menu.Base.pfnKeyDown)
// 	{
// 		MainMenuScreen.Menu.Base.pfnKeyDown( &MainMenuScreen.Menu.Base,keyCode );
// 	}
	OnCtrlKeyDown((CCtrl *) &MainMenuScreen.Menu ,keyCode );

#endif

}

static void OnInitScreen(void)
{
//	PostMessage(LCD_WM_INI,0);
//	PostEvent( & MainMenuScreen.Base,LCD_WM_INI,0 );
}

//退出时处理
static void OnDestory(void)
{
//	ClearEvent( &(MainMenuScreen.Base) );	 
}
 


static void SetupResource( CMainMenuScreen *pScreenExt )
{
	if (pScreenExt)
	{
//		 CreateLcdMenu( & pScreenExt->Menu,(LCDRECT*)  & LCD_RECT_MENU,(LCDRECT*)  & LCD_RECT_VARBAR )	 ;
		// 菜单链接
//		pScreenExt->Menu.AddItem( MainMenu, sizeof(MainMenu)/sizeof(CMenuData));
//		 AddMenuItems( & pScreenExt->Menu,MainMenu, sizeof(MainMenu)/sizeof(CMenuData));
		// 聚焦当前选择的菜单
//		pScreenExt->Menu.SetFocus();	
	}

}
#if 0
 void InitMainMenuScreen(CMainMenuScreen *pScreenExt )  
{ 
	if (pScreenExt)
	{	 
		SetScreenInfo(& pScreenExt->Base,ID_MAIN_SCREEN,ID_MAIN_SCREEN,ID_STRING_MAIN_MENU);
		pScreenExt->Base.pfnMessage = OnMessage;
		pScreenExt->Base.pfnKeyDown = OnKeyDown;
		pScreenExt->Base.pfnDestory = OnDestory;
		pScreenExt->Base.pfnInit    = OnInitScreen;
		pScreenExt->Base.pfnOnNotify = NULL;

// 		if (pScreenExt->Base.pfnInit)
// 		{
// 			pScreenExt->Base.pfnInit();
// 		}	
		SetupResource ( pScreenExt );
	}
}

#endif

 