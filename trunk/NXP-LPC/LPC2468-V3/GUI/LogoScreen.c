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
#include "includes.h"
#include "ScreenBase.h"
#include "LogoScreen.h"
#include "StringTable.h"
#include "Message.h"
#include "DC.h"

extern CScreenMgr  g_ScreeLib;

CLogoScreen      LogoScreen;       // 版本声明画面

static OS_TMR *BrandingTmr;


CScreenDefault       defscreen;

static void OnMessageLogoScreen(INT32U msg, INT32U param)  
{
	CLogoScreen *pScreenExt = NULL;
	INT8U err = 0;
	switch ( msg )
	{
	case LCD_WM_INI:
	case LCD_WM_REFRASH: 
		{
			ClearScreen();
			DrawStringById(LCD_WIDTH>>1,0,ID_STRING_WISCOM,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);
			DrawStringById(LCD_WIDTH>>1,16,ID_STRING_BRANDING,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);	
		}
		break;
	case LCD_WM_TIMER:
		{
			// 2秒等待定时器消息到退出本画面进入默认显示画面
	//		if(param&TimerMask(TIMER_0))
			if(OSTmrStop(BrandingTmr,OS_TMR_OPT_NONE,(void *)0,&err))
			{
				// 退出等待状态
				 SwitchScreen(&g_ScreeLib,pScreenExt->Base.ParentID);
			}
		}
		break;
	default:
		break;
	}
}


//按键处理虚函数 
static void OnKeyDownLogoScreen(INT16U keyCode)   
{	
	// 有任意键按下，退出开始画面
	if (keyCode!=0) 
	{
		// 退出等待状态
		SwitchScreen(&g_ScreeLib,LogoScreen.Base.ParentID);
	}
}

static void OnInitLogoScreen(void)
{
	PostMessage(LCD_WM_INI,0);
}
 
//退出时处理
static void OnDestoryLogoScreen(void)
{
	INT8U err = 0;
	// 关定时器
	if (BrandingTmr)
	{
		OSTmrDel(BrandingTmr,&err);
	}	
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
void LogoScreenTimer (void *ptmr, void *parg)
{
	if (BrandingTmr && BrandingTmr == ptmr)
	{
		PostMessage(LCD_WM_TIMER,parg);
	}
}

extern void InitLogoScreen(CLogoScreen *pScreenExt )
{
	INT8U i = 0;
	INT8U err = 0;

	BrandingTmr = NULL;

	if (pScreenExt)
	{
		//安装定时器
		BrandingTmr = OSTmrCreate( 100,	2000,OS_TMR_OPT_PERIODIC,LogoScreenTimer,(void *)pScreenExt,(INT8U *)0,&err);
		if (err == OS_ERR_NONE) 
		{/* Timer was created but NOT started */
			OSTmrStart(BrandingTmr,&err);
		}	

		SetScreenInfo(& pScreenExt->Base,ID_DEFAULTSCREEN, ID_START_WINDOW, ID_STRING_NONE);
		pScreenExt->Base.pfnMessage = OnMessageLogoScreen;
		pScreenExt->Base.pfnKeyDown = OnKeyDownLogoScreen;
		pScreenExt->Base.pfnDestory = OnDestoryLogoScreen;
		pScreenExt->Base.pfnInit    = OnInitLogoScreen;
		pScreenExt->Base.pfnOnNotify = NULL;

		if (pScreenExt->Base.pfnInit)
		{
			pScreenExt->Base.pfnInit();
		}

		//该窗口光标
		for(i=0; i<STATE_COUNT; i++)
		{
			pScreenExt->Cursor[i].Row = 0;
			pScreenExt->Cursor[i].Col = 0;
		}

		//当前显示状态
		pScreenExt->CurrentState = STATE_NORMAL;  
	}
}




// 根据循环显示数据类型初始化显示数据个数
static void InitDftScreenDispNum(CScreenDefault *pScreenExt,INT16U nTypeIndex)
{
	INT16U nTypeString=0;

	if (pScreenExt == NULL)
		return;

	pScreenExt->CurrentTypeIndex=nTypeIndex;
	pScreenExt->CurrentMeasIndex=0;

// 	if(nTypeIndex<MeasDispType.MeasTypeCount)
// 	{
// 		nTypeString=MeasDispType.MeasDisp[nTypeIndex].nMeasType;
// 	}
// 	else
// 	{
// 		nTypeString=ID_STRING_MEASTYPE_ALL;
// 	}

 	 ClearScreen();
//     DrawString(60,8,nTypeString,PEN_COPY,ALIGN_CENTER);
}

extern void InitDefaultScreen(CScreenDefault *pScreenExt )
{
	if (pScreenExt)
	{

	}
}

