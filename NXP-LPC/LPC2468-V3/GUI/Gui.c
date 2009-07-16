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
#include "ScreenBase.h"
#include "LogoScreen.h"
#include "Gui.h"
#include "DC.h"
#include "TaskDef.h"
#include "Key.h"

CScreenMgr  g_ScreeLib;

CScreenBase * const screens[]=
{
	(CScreenBase*)&LogoScreen,
//	(CScreenBase*)&defscreen

};
 


// 建立窗口库
void InitScreenLib( )
{
	INT8U i = 0;
	INT8U nScreenCnt = 0;

	InitLogoScreen( &LogoScreen );

//	InitDefaultScreen(&defscreen);

	for ( i=0;i<sizeof(screens)/sizeof(CScreenBase * );i++) 
	{
		AppendScreen(&g_ScreeLib, screens[i]);
	}


	//进入默认画面
	SwitchScreen(&g_ScreeLib, ID_START_WINDOW);
}

static void Gui_Task(void *pdata)
{
	INT8U	status;
	INT16U key = 0;
	pdata = pdata;

	while (TRUE)
	{
		//获取按键
		key = 0;
		if (key)
		{//有按键，点亮LCD的灯

			//1.看按键是否是重启等功能键

			//2.按键消息

		}
		else
		{
			//关闭灯

		}

		//消息处理
		HandleMessage();
		OSTimeDlyHMSM(0,0,0,200);
	//	OSTimeDly(OS_TICKS_PER_SEC/4);
	}
}



static OS_STK  GUITaskStk [GUI_TASK_STK_SIZE];


extern void InitGuiTask( )
{
	//LCD初始化
	GXM12232_SED1520_initlcd();

	//按键初始化
//	KeyPress_Initial();

	//测值显示初始化
//	MeasDisp_Initial();

	// 界面初始化
	InitScreenLib();
	//点亮液晶
	//点亮液晶
	TurnOnLed();



	//启动任务
	OSTaskCreate (Gui_Task, (void *)0, 	&GUITaskStk[GUI_TASK_STK_SIZE-1], PRIO_GUI_TASK);

	//启动键盘扫描模块
	InitKeyDriver ( );
}
 

 