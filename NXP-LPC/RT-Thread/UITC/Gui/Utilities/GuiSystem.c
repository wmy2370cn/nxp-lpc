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
#include <stddef.h>
#include "applib.h"
#include "ScreenBase.h"
#include "guisystem.h"


/*********************************************************************************************************
** 函数名称: GuiTaskEventRecv
** 函数名称: GuiTaskEventRecv
**
** 功能描述：  用于GUI任务 接收其他任务发来的消息
**
**          
** 输　出:   INT8U
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
INT8U GuiTaskEventRecv( )
{

	return TRUE;
}

/*********************************************************************************************************
** 函数名称: PostEventToGuiTask
** 函数名称: PostEventToGuiTask
**
** 功能描述：  用于其他任务发消息给GUI  通常，定时器任务
**
**          
** 输　出:   INT8U
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
INT8U PostEventToGuiTask( )
{

	return TRUE;
}

static void TimerCB( void *pData)
{//定时器时间到了，通知GUI任务，

	PostEventToGuiTask();

}


struct GUI_TIMER
{
	CScreenBase *pScreen;
	INT32U       nIDEvent;
	rt_timer_t   pSysTimer;
	gui_list_t   list;
};
typedef struct GUI_TIMER CGuiTimer;

static CGuiTimer g_lstGuiTimerHead;

void InitGuiTimerMgr( void )
{
	GuiListInit(&g_lstGuiTimerHead.list);
	g_lstGuiTimerHead.pScreen = NULL;
	g_lstGuiTimerHead.nIDEvent = 0;
	g_lstGuiTimerHead.pSysTimer = NULL;
}
/*********************************************************************************************************
** 函数名称: SetGuiTimer
** 函数名称: SetGuiTimer
**
** 功能描述：  
**
** 输　入:  CScreenBase * pScr
** 输　入:  INT32U nIdEvent
** 输　入:  INT32U nMsTime
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月13日
** 备  注:  TODO: 定时器的单位是TICK？待确认
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SetGuiTimer(CScreenBase *pScr,INT32U nIdEvent,INT32U nMsTime)
{
	//根据窗口的名称和ID，查看是否已有重复的定时器，如果没有，则动态分配一个定时器GUI_TIMER，并挂到链表上
	struct GUI_TIMER *pGuiTimer = NULL;
 	gui_list_t *pNode = NULL;
	INT8U bFindFlag = FALSE;

	//
	GuiListForEach(pNode, &(g_lstGuiTimerHead.list))
	{
		pGuiTimer = CONTAINING_RECORD(pNode, CGuiTimer,list);
		ASSERT(pGuiTimer);
 		if (pGuiTimer && pGuiTimer->pScreen == pScr && pGuiTimer->nIDEvent == nIdEvent)
		{
 			bFindFlag = TRUE;
 			break;
		}
	}

	if (bFindFlag)
	{//如果有重复的，则重新启动一下定时器
		if(pGuiTimer && pGuiTimer->pSysTimer)
		{
			rt_timer_control(pGuiTimer->pSysTimer,RT_TIMER_CTRL_SET_TIME,&nMsTime);
			//启动定时器
			rt_timer_start(pGuiTimer->pSysTimer);
		}
	}
	else
	{
		pGuiTimer = rt_malloc(sizeof(CGuiTimer));
		ASSERT(pGuiTimer);
		if (pGuiTimer)
		{
			pGuiTimer->pScreen = NULL;
			pGuiTimer->nIDEvent = 0;
			pGuiTimer->pSysTimer = NULL;

			//挂接到链表上面
			GuiListAppend(&	(g_lstGuiTimerHead.list),&(pGuiTimer->list));

			//建立系统定时器
			pGuiTimer->pSysTimer = rt_timer_create("GuiTm", TimerCB, (void *) pGuiTimer, nMsTime,
					RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
			ASSERT(pGuiTimer->pSysTimer);
			//启动定时器
			rt_timer_start(pGuiTimer->pSysTimer);
		}
	}
}
/*********************************************************************************************************
** 函数名称: KillGuiTimer
** 函数名称: KillGuiTimer
**
** 功能描述：  
**
** 输　入:  CScreenBase * pScr
** 输　入:  INT32U nIdEvent
**          
** 输　出:   INT8U
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
INT8U KillGuiTimer( CScreenBase *pScr,INT32U nIdEvent )
{
	struct GUI_TIMER *pGuiTimer = NULL;
	gui_list_t *pNode = NULL;

	if(pScr == NULL)
		return FALSE;

	GuiListForEach(pNode, &(g_lstGuiTimerHead.list))
	{
		pGuiTimer = CONTAINING_RECORD(pNode, CGuiTimer,list);
		ASSERT(pGuiTimer);
		if (pGuiTimer && pGuiTimer->pScreen == pScr && pGuiTimer->nIDEvent
				== nIdEvent)
		{
			GuiListRemove( &(g_lstGuiTimerHead.list), & (pGuiTimer->list));
			rt_free( pGuiTimer );

			return TRUE;
		}
	}
	return FALSE;
}
