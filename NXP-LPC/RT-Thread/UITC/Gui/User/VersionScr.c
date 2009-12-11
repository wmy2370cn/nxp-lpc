/****************************************Copyright (c)**************************************************
**                                      版本声明画面
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
#include "ScreenBase.h" 
#include "VersionScr.h"
#include "StringTable.h"
#include "extscreendef.h"
#include "GuiEvent.h"

#include "dc.h"

#if 0
CVersionScreen      VersionScreen;
 

static void OnMessageVersionScreen(INT32U msg, INT32U param)  
{
	CVersionScreen *pScreenExt = &VersionScreen;
	INT8U err = 0;
	switch ( msg )
	{
	case LCD_WM_INI:
	case LCD_WM_REFRASH: 
		{
			ClearScreen();
	//		DrawStringById(LCD_WIDTH>>1,0,ID_STRING_WISCOM,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);
	//		DrawStringById(LCD_WIDTH>>1,16,ID_STRING_BRANDING,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);	
		}
		break;
	case LCD_WM_TIMER:
		{
		 
		}
		break;
	default:
		break;
	}
}


//按键处理虚函数 
static void OnKeyDownVersionScreen(INT16U keyCode)   
{	
	// 有任意键按下，退出开始画面
	if (keyCode!=0) 
	{
		// 退出等待状态
		SwitchScreen(&g_ScreeLib,VersionScreen.Base.ParentID);
	}
}

static void OnInitVersionScreen(void)
{
//	PostMessage(LCD_WM_INI,0);
	PostEvent(&VersionScreen.Base,LCD_WM_INI,0);
}
 
//退出时处理
static void OnDestoryVersionScreen(void)
{
	 
}
/*********************************************************************************************************
** 函数名称: LogoScreenTimer
** 函数名称: LogoScreenTimer
**
** 功能描述： 
**
** 输　入:  void * ptmr
** 输　入:  void * parg
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月22日
** 备  注:  该函数必须尽可能短，所以采用寄送消息的方式，不用发送
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void VersionScreenTimer (void *ptmr, void *parg)
{
	 
}

extern void InitVersionScreen(CVersionScreen *pScreenExt )
{
	INT8U i = 0;
	INT8U err = 0;

 
	if (pScreenExt)
	{
		InitScreenBase( & pScreenExt->Base );
	 
		SetScreenInfo(& pScreenExt->Base,ID_DEFAULT_SCREEN, ID_VER_SCREEN, ID_STRING_NONE);
		pScreenExt->Base.pfnMessage = OnMessageVersionScreen;
		pScreenExt->Base.pfnKeyDown = OnKeyDownVersionScreen;
		pScreenExt->Base.pfnDestory = OnDestoryVersionScreen;
		pScreenExt->Base.pfnInit    = OnInitVersionScreen;
		pScreenExt->Base.pfnOnNotify = NULL;

// 		if (pScreenExt->Base.pfnInit)
// 		{
// 			pScreenExt->Base.pfnInit();
// 		}

	 
	}
}
#endif
 

/*********************************************************************************************************
** 函数名称: VerScrCtor
** 函数名称: VerScrCtor
**
** 功能描述：  构造函数
**
** 输　入:  CScreenBase * pScr
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void VerScrCtor( CScreenBase *pScr )
{

}
/*********************************************************************************************************
** 函数名称: VerScrDtor
** 函数名称: VerScrDtor
**
** 功能描述： 析构函数 
**
** 输　入:  CScreenBase * pScr
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void VerScrDtor(CScreenBase *pScr)
{
	ASSERT(pScr);

}

//创建窗口时处理
static void OnCreateVersionScreen( CScreenBase *pScr )
{
	ClearScreen();
 	DrawStringById(LCD_WIDTH>>1,0,ID_STRING_LOGO,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);
 	DrawStringById(LCD_WIDTH>>1,16,ID_STRING_LOGO,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);

}
//退出时处理
static void OnDestoryVersionScreen( CScreenBase *pScr )
{

}

static void OnInitVersionScreen( CScreenBase *pScr,INT32U wParam,INT32U lParam )
{

}

static INT32U VersionScreenProc (GUI_HWND hWnd, INT32U message, GUI_WPARAM wParam, GUI_LPARAM lParam)
{
	switch (message) 
	{
	case GUI_EVENT_CREATE:
		{
// 				int timeout = (int)GetWindowAdditionalData (hWnd);
// 				if (timeout >= 10)
// 					SetTimer (hWnd, _ID_TIMER, timeout / 10);
				break;
		}

	case GUI_EVENT_TIMER:
		//	KillTimer (hWnd, _ID_TIMER);
		//	ShowWindow (hWnd, SW_HIDE);
		break;

	case GUI_EVENT_PAINT:
		{
// 				HDC hdc;
// 				const char* text;
// 
// 				hdc = BeginPaint (hWnd);
// 
// 				text = GetWindowCaption (hWnd);
// 				SetBkMode (hdc, BM_TRANSPARENT);
// 				SelectFont (hdc, GetSystemFont (SYSLOGFONT_WCHAR_DEF));
// 				TabbedTextOut (hdc, _MARGIN, _MARGIN, text);
// 
// 				EndPaint (hWnd, hdc);
			return 0;
		}

	case GUI_EVENT_DESTROY:
		{
		//	KillTimer (hWnd, _ID_TIMER);
			return 0;
		}	 
	}

	return DefaultScreenProc(hWnd, message, wParam, lParam);
}


CScreenBase * InitVerScreen(void)
{
	INT32U nSize = sizeof(CVersionScreen);
	CScreenBase *pScr = NULL;

	pScr = CreateScreen(nSize ,VerScrCtor,VerScrDtor );
	ASSERT(pScr);
	SetScreenInfo(pScr,ID_DEFAULT_SCREEN, ID_VER_SCREEN, ID_STRING_NONE);
	pScr->ScreenProc = VersionScreenProc;
	return pScr;
}
