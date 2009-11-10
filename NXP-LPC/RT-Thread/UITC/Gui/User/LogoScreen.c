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
#include "LogoScreen.h"
#include "StringTable.h"
#include "Message.h"
#include "DC.h"
#include "ClockMgr.h"

extern CScreenMgr  g_ScreeLib;

CLogoScreen      LogoScreen;       // 版本声明画面

static OS_TMR *BrandingTmr;


CDefaultScreen       DefScreen;

static OS_TMR  *DefScrTmr;

static void OnMessageLogoScreen(INT32U msg, INT32U param)  
{
	CLogoScreen *pScreenExt = &LogoScreen;
	INT8U err = 0;
	switch ( msg )
	{
	case LCD_WM_INI:
		{
			ClearScreen();
			DrawStringById(LCD_WIDTH>>1,0,ID_STRING_LOGO,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_16);
			InvalidateScreen(TRUE); 
		}
		break;
	case LCD_WM_REFRASH: 
		{
			UpdateScreen(TRUE);
		}
		break;
	case LCD_WM_TIMER:
		{
			// 5秒等待定时器消息到退出本画面进入默认显示画面
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
//	PostMessage(LCD_WM_INI,0);
	PostEvent(&LogoScreen.Base,LCD_WM_INI,0);
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
static void LogoScreenTimer (void *ptmr, void *parg)
{
	if (BrandingTmr && BrandingTmr == ptmr)
	{
	//	PostMessage(LCD_WM_TIMER,(INT32U)parg);
		PostEvent(&LogoScreen.Base,LCD_WM_TIMER,(INT32U)parg);
	}
}

extern void InitLogoScreen(CLogoScreen *pScreenExt )
{
	INT8U i = 0;
	INT8U err = 0;

	BrandingTmr = NULL;

	if (pScreenExt)
	{
		InitScreenBase( & pScreenExt->Base );
		//安装定时器
		BrandingTmr = OSTmrCreate( 50,	100,OS_TMR_OPT_PERIODIC,LogoScreenTimer,(void *)pScreenExt,(INT8U *)0,&err);
		if (err == OS_ERR_NONE) 
		{/* Timer was created but NOT started */
			OSTmrStart(BrandingTmr,&err);
		}	

		SetScreenInfo(& pScreenExt->Base,ID_MAIN_SCREEN, ID_START_SCREEN, ID_STRING_NONE);
		pScreenExt->Base.pfnMessage = OnMessageLogoScreen;
		pScreenExt->Base.pfnKeyDown = OnKeyDownLogoScreen;
		pScreenExt->Base.pfnDestory = OnDestoryLogoScreen;
		pScreenExt->Base.pfnInit    = OnInitLogoScreen;
		pScreenExt->Base.pfnOnNotify = NULL;

// 		if (pScreenExt->Base.pfnInit)
// 		{
// 			pScreenExt->Base.pfnInit();
// 		}

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
static void InitDftScreenDispNum(CDefaultScreen *pScreenExt,INT16U nTypeIndex)
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
/*********************************************************************************************************
** 函数名称: DefScreenTimer
** 函数名称: DefScreenTimer
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
static void DefScreenTimer (void *ptmr, void *parg)
{
	if (DefScrTmr && DefScrTmr == ptmr)
	{
		//	PostMessage(LCD_WM_TIMER,(INT32U)parg);
	 	PostEvent(&DefScreen.Base,LCD_WM_TIMER,(INT32U)parg);
	}
}
/*********************************************************************************************************
** 函数名称: DispTime
** 函数名称: DispTime
**
** 功能描述：  
**
** 输　入:  CDefaultScreen * pScreen
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年4月27日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void DispTime(CDefaultScreen *pScreen )
{
	char szTxt[16] = {0};
	CDateTime DataTime;
	
	if (pScreen == NULL)
		return;

	ClockGetDataTime(&DataTime);
#define _OFFSET (40) //菜单文字与左边间距

 
	DrawStringById(LCD_WIDTH>>1,0,ID_STRING_LOGO,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_16);	

//	DrawStringById(16+_OFFSET,0,ID_STRING_MAIN_MENU,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_16);	
	//	theDC.DrawString(0,0,isprintf("%02d-%02d-%02d",dt.y,dt.mon,dt.d));
	//	theDC.DrawString(0,8,isprintf("%02d:%02d:%02d",dt.h,dt.min,dt.s));
 	sprintf(szTxt,"%02d-%02d-%02d[%d]",DataTime.wYear,DataTime.wMonth,DataTime.wDay,OSCPUUsage );
 //	DrawStringByStr( 0,32,szTxt,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);

 	sprintf(szTxt,"%02d:%02d:%02d:%03d", DataTime.wHour,DataTime.wMinute,DataTime.wSecond,DataTime.wMilliseconds);
 //	DrawStringByStr( 0,40,szTxt,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_8);
 
	 
}

/*********************************************************************************************************
** 函数名称: DispValue
** 函数名称: DispValue
**
** 功能描述：  显示测值，今后如果需要增加显示的量，则此函数需要改造
**
** 输　入:  CDefaultScreen * pScreen
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年4月27日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void DispValue( CDefaultScreen *pScreen )
{
	if (pScreen == NULL)
		return;


}

// 循环显示模式时根据循环显示数据类型循环显示
static 	void LoopDisp (CDefaultScreen *pScreen)
{
	if (pScreen == NULL)
		return;
//	ClearScreenEx(TRUE);

	DispTime(pScreen);

//	ClearScreenEx(TRUE);
}


static void OnMessageDefaultScreen(INT32U msg, INT32U param)  
{
	CDefaultScreen *pScreenExt = &DefScreen;
	switch ( msg )
	{
	case LCD_WM_INI:	
	case LCD_WM_REFRASH: 
		{
			// 刷新界面测量量显示
			LoopDisp(pScreenExt);
	 	}
		break;
	case LCD_WM_TIMER:
		{
			// 刷新界面测量量显示
			LoopDisp(pScreenExt);
		}
		break;
	default:
		break;
	}
}


//按键处理虚函数 
static void OnKeyDownDefaultScreen(INT16U keyCode)   
{	
	switch(keyCode)
	{
	case KEY_1:
		{	// 切换暂停循环显示
			DefScreen.Toggle = !DefScreen.Toggle;
		}
		break;
	case KEY_2:
		{	// 退出该窗口，进入父窗口
			SwitchScreen(&g_ScreeLib,DefScreen.Base.ParentID);
		}
		break;
	case KEY_3:
		{
			// 上移一个测量量显示
	//		m_nCurrentMeasIndex--;
	//		if(m_nCurrentMeasIndex<0)
	//		{
	//			m_nCurrentMeasIndex=nLoopDispNum-1;	//为nLoopDispNum时显示所有测值
	//		}
	//		OnMessage(LCD_WM_REFRASH,NULL);
			PostEvent(&DefScreen.Base,LCD_WM_REFRASH,0);
		}
		break;
	case KEY_4:
		{
			// 下移一个测量量显示
	//		m_nCurrentMeasIndex++;
	//		if(m_nCurrentMeasIndex>=nLoopDispNum)
	//		{
	//			m_nCurrentMeasIndex=0;
	//		}
	//		OnMessage(LCD_WM_REFRASH,NULL);
			PostEvent(&DefScreen.Base,LCD_WM_REFRASH,0);
		}
		break;
	case KEY_5:
		{
			// 选择循环显示的模式
	//		m_nCurrentTypeIndex--;
	//		if(m_nCurrentTypeIndex<0)
	//		{
	//			m_nCurrentTypeIndex = MeasDispType.MeasTypeCount;
	//		}
	//		InitDispNum(m_nCurrentTypeIndex);
			PostEvent(&DefScreen.Base,LCD_WM_REFRASH,0);
		}
		break;
	case KEY_6:
		{
			// 选择循环显示的模式
// 		 	m_nCurrentTypeIndex++;
// 			if(m_nCurrentTypeIndex>MeasDispType.MeasTypeCount)
// 			{
// 				m_nCurrentTypeIndex = 0;
// 			}
// 			InitDispNum(m_nCurrentTypeIndex);
			PostEvent(&DefScreen.Base,LCD_WM_REFRASH,0);
		}
		break;
	default:
		break;
	}
}

static void OnInitDefaultScreen(void)
{
	INT8U err = 0;
	DefScrTmr = OSTmrCreate( 0,10,OS_TMR_OPT_PERIODIC,DefScreenTimer,(void *)(&DefScreen),(INT8U *)0,&err);
	if (err == OS_ERR_NONE) 
	{/* Timer was created but NOT started */
		OSTmrStart(DefScrTmr,&err);
	}	
	PostEvent(&DefScreen.Base,LCD_WM_INI,0);
}

//退出时处理
static void OnDestoryDefaultScreen(void)
{
	INT8U err = 0;
	// 关定时器
	if (DefScrTmr)
	{
		OSTmrDel(DefScrTmr,&err);
	}	
}

/*********************************************************************************************************
** 函数名称: InitDefaultScreen
** 函数名称: InitDefaultScreen
**
** 功能描述： 创建窗口时的初始化
**
** 输　入:  CScreenDefault * pScreenExt
**          
** 输　出:   extern void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年4月27日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern void InitDefaultScreen(CDefaultScreen *pScreenExt )
{
	INT8U err = 0;

	if (pScreenExt == NULL)
		return;

	InitScreenBase( & pScreenExt->Base );
	SetScreenInfo(& pScreenExt->Base,ID_MAIN_SCREEN, ID_DEFAULT_SCREEN, ID_STRING_NONE);
	pScreenExt->Base.pfnMessage = OnMessageDefaultScreen;
	pScreenExt->Base.pfnKeyDown = OnKeyDownDefaultScreen;
	pScreenExt->Base.pfnDestory = OnDestoryDefaultScreen;
	pScreenExt->Base.pfnInit    = OnInitDefaultScreen;
	pScreenExt->Base.pfnOnNotify = NULL;

	pScreenExt->Toggle       = TRUE;     
	pScreenExt->CurrentTypeIndex=0;
	pScreenExt->CurrentMeasIndex=0;
	
	//默认显示测量量
// 	for (i=0;i<MeasDispType.MeasTypeCount;i++) 
// 	{
// 		if(MeasDispType.MeasDisp[i].nMeasType==ID_STRING_MEASTYPE_MEAS)
// 		{
// 			m_nCurrentTypeIndex=i;
// 			break;
// 		}
// 	}
 }
