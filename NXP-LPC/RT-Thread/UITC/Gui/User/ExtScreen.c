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
#include <stdio.h>
#include "includes.h"
#include "ScreenBase.h"
#include "StringTable.h"
#include "Message.h"
#include "DC.h"
#include "ClockMgr.h"
#include "ExtScreen.h"

extern CScreenMgr  g_ScreeLib;

CRealTimeValScreen   RealTimeValScr;       //实时测值   

static OS_TMR  *pValScrTmr = NULL;

/*********************************************************************************************************
** 函数名称: RealTimeValScrTimer
** 函数名称: RealTimeValScrTimer
**
** 功能描述：  
**
** 输　入:  void * ptmr
** 输　入:  void * parg
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月24日
** 备  注:  该函数必须尽可能短，所以采用寄送消息的方式，不用发送
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void RealTimeValScrTimer (void *ptmr, void *parg)
{
	if (pValScrTmr && pValScrTmr == ptmr)
	{
		PostEvent(&RealTimeValScr.Base,LCD_WM_TIMER,(INT32U)parg);
	}
}
/*********************************************************************************************************
** 函数名称: DrawValueScrStatic
** 函数名称: DrawValueScrStatic
**
** 功能描述：  画窗口的静态部分
**
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月24日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void DrawValueScrStatic( )
{

}

static void DispRealTimeVal( )
{

}

static void OnMessageRealTimeValScreen(INT32U msg, INT32U param)  
{
	CRealTimeValScreen *pScreenExt = &RealTimeValScr;
	INT8U err = 0;
	switch ( msg )
	{
	case LCD_WM_INI:
	case LCD_WM_REFRASH: 
		{
			ClearScreen();
			//画标题
 			DrawStringById(LCD_WIDTH>>1,0,ID_STRING_LOGO,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_16);
			//画静态部分
			DrawValueScrStatic( );
 		//	DrawStringById(LCD_WIDTH>>1,16,ID_STRING_BRANDING,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);	
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
static void OnKeyDownRealTimeValScreen(INT16U keyCode)   
{	
	// 有任意键按下，退出开始画面
	if (keyCode!=0) 
	{
		// 退出等待状态
		SwitchScreen(&g_ScreeLib,RealTimeValScr.Base.ParentID);
	}
}

static void OnInitRealTimeValScreen(void)
{
//	PostMessage(LCD_WM_INI,0);
	PostEvent(&RealTimeValScr.Base,LCD_WM_INI,0);
}
 
//退出时处理
static void OnDestoryRealTimeValScreen(void)
{
	INT8U err = 0;
	// 关定时器
	if (pValScrTmr)
	{
		OSTmrDel(pValScrTmr,&err);
		pValScrTmr = NULL;		
	}	
}
 

extern void InitRealTimeValScreen(CRealTimeValScreen *pScreenExt )
{
 	INT8U err = 0;
	 
	if (pScreenExt)
	{
		InitScreenBase( & pScreenExt->Base );
	 
		SetScreenInfo(& pScreenExt->Base,ID_MAIN_SCREEN, ID_REALTIME_VAL, ID_STRING_VALUE_TAB);
		pScreenExt->Base.pfnMessage = OnMessageRealTimeValScreen;
		pScreenExt->Base.pfnKeyDown = OnKeyDownRealTimeValScreen;
		pScreenExt->Base.pfnDestory = OnDestoryRealTimeValScreen;
		pScreenExt->Base.pfnInit    = OnInitRealTimeValScreen;
		pScreenExt->Base.pfnOnNotify = NULL;

		//安装定时器
		pValScrTmr = OSTmrCreate( 50,	100,OS_TMR_OPT_PERIODIC,RealTimeValScrTimer,(void *)pScreenExt,(INT8U *)0,&err);
		if (err == OS_ERR_NONE) 
		{/* Timer was created but NOT started */
			OSTmrStart(pValScrTmr,&err);
		}	

	}
}
 