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
#include <string.h>
#include "includes.h" 
#include "Message.h"
#include "ScreenBase.h"
#include "Gui.h"
#include "DC.h"
#include "TaskDef.h"
#include "Key.h"
#include "TimerMgr.h"


#include "LogoScreen.h"
#include "MainMenuScreen.h"
#include "ExtScreen.h"

CScreenMgr  g_ScreeLib;

CScreen * const screens[]=
{
	(CScreen*)&LogoScreen,
  	(CScreen*)&DefScreen,
	(CScreen*)&MainMenuScreen ,

	(CScreen*)&RealTimeValScr ,

};
 


// 建立窗口库
static void InitScreenLib( )
{
	INT8U i = 0;

	InitScreenMgr(&g_ScreeLib);
	InitLogoScreen( &LogoScreen );
 	InitDefaultScreen(&DefScreen); 	
	InitMainMenuScreen(&MainMenuScreen);

	InitRealTimeValScreen(&RealTimeValScr);

	for ( i=0;i<sizeof(screens)/sizeof(CScreenBase * );i++) 
	{
		AppendScreen(&g_ScreeLib, screens[i]);
	}
	//进入默认画面
	SwitchScreen(&g_ScreeLib, ID_START_SCREEN);
}

static void HandleScreenEvent(void)
{
	CScreen *pScreen = NULL;
	CScreenMgr *pMgr = &g_ScreeLib;

	pScreen = GetCurrentScreenPtr(pMgr);

	if (pScreen == NULL)
		return;

	HandleEvent( & pScreen->Base  )	;
}
//匿名寄送，送给当前活动的窗口
static INT8U PostEventN(INT32U msg,INT32U param)
{
	CScreen *pScreen = NULL;
	CScreenMgr *pMgr = &g_ScreeLib;

	pScreen = GetCurrentScreenPtr(pMgr);

	if (pScreen == NULL)
		return FALSE;

	return PostEvent( & pScreen->Base,msg,param );
}
//任务自身的事件
static UEVENT  g_GuiTaskEvent[MAX_ENENT_CNT];
/*********************************************************************************************************
** 函数名称: PostGuiTaskEvent
** 函数名称: PostGuiTaskEvent
**
** 功能描述：  给GUI任务寄送消息
**
** 输　入:  INT32U msg
** 输　入:  INT32U WParam
** 输　入:  INT32U LParam
**          
** 输　出:   extern INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月30日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern INT8U PostGuiTaskEvent( INT32U msg,INT32U WParam,INT32U LParam)
{ 
	INT8U  i = 0;
	INT8U Flag = FALSE;
	
	//查看是否有重复的事件
	for(i = 0;i < MAX_ENENT_CNT;i++)
	{
		if (g_GuiTaskEvent[i].Flag && g_GuiTaskEvent[i].Msg == msg)
		{
			g_GuiTaskEvent[i].Flag++;
			Flag = TRUE;
			return Flag;;
		}
	}
	for(i = 0;i < MAX_ENENT_CNT;i++)
	{
		if (g_GuiTaskEvent[i].Flag  == FALSE )
		{
			g_GuiTaskEvent[i].Msg = msg;
			g_GuiTaskEvent[i].WParam = WParam;
			g_GuiTaskEvent[i].LParam = LParam;
			g_GuiTaskEvent[i].Flag  = TRUE;
			Flag = TRUE;
			break;
		}
	}
	return Flag;
}
/*********************************************************************************************************
** 函数名称: HandleTaskEvent
** 函数名称: HandleTaskEvent
**
** 功能描述： 处理任务自身的事件 
**
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月30日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U HandleTaskEvent( )
{
	INT8U Flag = FALSE;
	INT8U i = MAX_ENENT_CNT;
	UEVENT Event;

	memset(&Event,0,sizeof(UEVENT));

	for (;;)
	{
		if (g_GuiTaskEvent[i-1].Flag)
		{//
			//复制出来
			Event.Msg = g_GuiTaskEvent[i-1].Msg;
			Event.WParam = g_GuiTaskEvent[i-1].WParam;
			Event.LParam = g_GuiTaskEvent[i-1].LParam;
			Event.Flag = TRUE;

			g_GuiTaskEvent[i-1].Flag = 0;
			g_GuiTaskEvent[i-1].Msg = 0;
			g_GuiTaskEvent[i-1].WParam = 0;
			g_GuiTaskEvent[i-1].LParam = 0;
			Flag = TRUE;

			break;
		}
		if(i == 0)
			break;
		i--;	
	}
	if(Flag)
	{//处理事件
		if (Event.Msg == LCD_WM_PAINT)
		{
			UpdateScreen((INT8U)Event.WParam);
			return TRUE;
		}	
	}
	return FALSE;
}
static void GuiTask(void *pdata)
{
	INT16U  key = 0;
	INT16U  nTimerID = 0;
	pdata = pdata;
	
	//点亮液晶
	TurnOnLcd();
	//5秒后关闭液晶
	nTimerID = SetTimer(nTimerID,5000,NULL,NULL);

	while (TRUE)
	{
		//获取按键
		key = 0;
		if (KeyHit())
		{
			key = GetKey( 0 );
			if (key !=KEY_NONE )
			{//有按键，点亮LCD的灯
				TurnOnLcd();
				nTimerID = SetTimer(nTimerID,10000,NULL,NULL);
				 


				//1.看按键是否是重启等功能键

				//2.按键消息
				PostEventN(LCD_WM_KEYDOWN,key );


			}
			else
			{
				//关闭灯

			}
		}
		else
		{//没有按键
			if (nTimerID)
			{
				if (IsTimeTo(nTimerID))
				{//到时间了 
					TurnOffLcd();
					KillTimer(nTimerID);
					nTimerID = 0;
				}
			}
		}
	

		//消息处理
		HandleMessage();
		HandleScreenEvent( );
		HandleTaskEvent( );
		OSTimeDlyHMSM(0,0,0,200);
	//	OSTimeDly(OS_TICKS_PER_SEC/4);
	}
}



static OS_STK  GUITaskStk [GUI_TASK_STK_SIZE]; 

extern void InitGuiTask( )
{
	//LCD初始化
	InitLcd( );
	// 界面初始化
	InitScreenLib();
	//启动任务
	OSTaskCreate (GuiTask, (void *)0, 	&GUITaskStk[GUI_TASK_STK_SIZE-1], PRIO_GUI_TASK);

	//启动键盘扫描模块
	InitKeyDriver ( );
}
