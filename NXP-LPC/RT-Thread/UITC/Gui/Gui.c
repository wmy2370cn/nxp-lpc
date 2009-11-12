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
#include "applib.h"
#include "GuiList.h"
#include "keydrv.h"
 
#include "ScreenBase.h"  


#include "versionScr.h"
#include "devicecfgscreen.h"



//界面管理
struct SCREEN_MGR
{
	gui_list_t  ScrLib;  // 界面指针数组
	INT16U 	    ScreensCnt;	 // 界面总数
};

typedef struct SCREEN_MGR CScreenMgr;

static CScreenMgr g_ScreenLib;

// 切换到序号ScreenID屏幕
void SwitchScreen(INT8U nScreenID)
{
	CScreenBase * pScreen = NULL ;
	CScreenMgr *pMgr = &g_ScreenLib;

//	pScreen=GetCurrentScreenPtr(pMgr);

	//首先销毁原窗口
	if(pScreen && pScreen->pfnDestory)
	{
		pScreen->pfnDestory(pScreen);
	}

	//切换到下一窗口
//	pScreen=GetScreenPtr(pMgr,nScreenID);

	if(pScreen == NULL)
		return;

 	//如果切换到密码输入框保存密码输入框返回窗口
 //	if(nScreenID == ID_PASSWORD || nScreenID == ID_MODALDIALOG)
 //		SetFatherID(& pScreen->Base,GetActiveWinID());

//	SetActiveWinID(nScreenID);

 	//清除屏幕,模式对话框因为要保存上个界面的图形所有不进行清屏
 //	if(nScreenID != ID_MODALDIALOG)
 //		ClearScreen();
	if(pScreen != NULL && pScreen->pfnInit)
 		pScreen->pfnInit(pScreen);// 界面默认初始化代码，比如建立资源

}
/*********************************************************************************************************
** 函数名称: InitScreenLib
** 函数名称: InitScreenLib
**
** 功能描述：  初始化所有的屏幕
**
**
** 输　出:
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月11日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void InitScreenLib( )
{
	INT8U i = 0;
	CScreenBase *pScreen = NULL;

	//初始化链表先
	GuiListInit(&(g_ScreenLib.ScrLib));

	//创建LOGO SCREEN
	//pScreen = ( );

	pScreen = InitVerScreen();

	//添加到链表中
	GuiListAppend(&(g_ScreenLib.ScrLib),&(pScreen->List));
	g_ScreenLib.ScreensCnt ++;

	pScreen = InitDevCfgScreen();
	GuiListAppend(&(g_ScreenLib.ScrLib),&(pScreen->List));
	g_ScreenLib.ScreensCnt ++;


//	InitScreenMgr(&g_ScreeLib);
//	InitLogoScreen( &LogoScreen );
// 	InitDefaultScreen(&DefScreen);
//	InitMainMenuScreen(&MainMenuScreen);
//
//	InitRealTimeValScreen(&RealTimeValScr);
//
//	for ( i=0;i<sizeof(screens)/sizeof(CScreenBase * );i++)
//	{
//		AppendScreen(&g_ScreeLib, screens[i]);
//	}
//	//进入默认画面
//	SwitchScreen(&g_ScreeLib, ID_START_SCREEN);
}
/*********************************************************************************************************
** 函数名称: HandleTaskEvent
** 函数名称: HandleTaskEvent
**
** 功能描述：  处理任务级别的消息
**
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月12日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U HandleTaskEvent( )
{

	return TRUE;
}


static void GuiTask(void *pdata)
{
	INT16U  key = 0;
	pdata = pdata;

	while(TRUE)
	{
		//获取按键
		key = 0;
		if (KeyHit())
		{
			key = GetKey( 30 );
			if (key != KEY_NONE)
			{//有按键，点亮LCD的灯
			//	TurnOnLcd();
			//	nTimerID = SetTimer(nTimerID, 10000, NULL, NULL);

				//1.看按键是否是重启等功能键

				//2.按键消息
			//	PostEventN(LCD_WM_KEYDOWN, key);

			}
			else
			{
				//关闭灯

			}
		}
		//看看是否有其他任务发送过来的消息


	}

}

//定义各任务的堆栈长度
#define  GUI_TASK_STK_SIZE      512
#define  PRIO_GUI_TASK          30

static unsigned char  GuiTaskStk [GUI_TASK_STK_SIZE];
struct rt_thread Gui_Thread;

void InitGuiTask( void  )
{
	//初始化窗口消息队列
	InitScrEventList(  );
	// 界面初始化
	InitScreenLib();
	//启动任务
	rt_thread_init(&Gui_Thread,"Gui_Thread",GuiTask, RT_NULL, &GuiTaskStk[0], sizeof(GuiTaskStk),
			PRIO_GUI_TASK, 10);

	rt_thread_startup(&Gui_Thread);
}
