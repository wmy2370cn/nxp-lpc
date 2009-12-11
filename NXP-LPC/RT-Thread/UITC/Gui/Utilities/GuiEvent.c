 /****************************************Copyright (c)**************************************************
**							     
**     所有窗口的消息队列 窗口消息队列由统一的消息池，其操作在一个线程内，              
**     任务间的消息队列，采用IPC进行管理                           
**     TODO:看看是否采用静态内存池来实现？消息的处理是否需要再仔细考虑下？目前只考虑了Screen部分消息，没有
**     控件的消息处理
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:   GuiEvent.c
**创   建   人:   LiJin
**最后修改日期: 2009-11-12
**描        述:   
**备        注 :
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
#include <rtthread.h>
#include "applib.h"
#include "GuiConfig.h"
#include "GuiList.h"
#include "GuiEvent.h"
#include "ScreenBase.h"
#include "list.h" 

struct GUI_EVENT_MGR
{
	GuiEventNode *pEvent;
	struct GuiListNode FreeListHead;
	struct GuiListNode UsedListHead;
};

typedef struct GUI_EVENT_MGR GuiEventMgr;
 //窗口的消息队列，属于同一个进程内，
GuiEventMgr g_ScrEvent;


static rt_mq_t g_mqTask ;

static INT8U InitTaskEvent( void )
{
	//TODO: 用消息队列来实现比较合理
	g_mqTask = rt_mq_create("GT_EVNT",sizeof(GuiEvent)*GUI_EVENT_CNT,GUI_EVENT_CNT,RT_IPC_FLAG_FIFO);
	if (g_mqTask == RT_NULL)
		return FALSE;

	return TRUE;
}
/*********************************************************************************************************
** 函数名称: InitGuiEventMgr
** 函数名称: InitGuiEventMgr
**
** 功能描述：  
**
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月12日
** 备  注:  所有窗口的消息队列
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U InitGuiEventQueue( void )
{
	INT16U i  = 0;
	GuiEventNode *pEvent = NULL;
	InitTaskEvent();
	g_ScrEvent.pEvent = rt_malloc( sizeof(GuiEventNode) * GUI_EVENT_CNT );
	ASSERT(g_ScrEvent.pEvent );
	if (g_ScrEvent.pEvent == NULL)
		return FALSE;

	pEvent =g_ScrEvent.pEvent ;
	GuiListInit( &g_ScrEvent.FreeListHead );
	GuiListInit( &g_ScrEvent.UsedListHead );

	//生成链表
	for (i = 0; i < GUI_EVENT_CNT; i++)
	{	
		GuiListAppend( & g_ScrEvent.FreeListHead, & (pEvent[i].NextNode) );
	}


	return TRUE;
}
/*********************************************************************************************************
** 函数名称: SendEvnent
**
** 功能描述： 发送消息，立刻执行
**
** 输　入:
**
** 输　出:
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
INT8U SendScreenEvnent( struct SCREEN_BASE *pScr,INT32U Msg,INT32U wParam,INT32U lParam )
{
	if(pScr == NULL)
		return FALSE;

	switch (Msg)
	{
	case GUI_EVENT_KEYDOWN:
		ASSERT(pScr->pfnKeyDown);
		if (pScr->pfnKeyDown)
		{
			pScr->pfnKeyDown(pScr,wParam);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
 



	return TRUE;
}
/*********************************************************************************************************
** 函数名称: PostScreenEvnent
** 函数名称: PostScreenEvnent
**
** 功能描述：  寄送消息，暂存于任务队列中，然后在任务合适的时候执行
**
** 输　入:  CScreenBase * pScr
** 输　入:  INT32U Msg
** 输　入:  INT32U wParam
** 输　入:  INT32U lParam
** 输　入:  BOOL bNoCheck  是否检查有重复的消息
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
INT8U PostScreenEvnent( struct SCREEN_BASE *pScr,INT32U Msg,INT32U wParam,INT32U lParam ,INT8U bNoCheck)
{
	GuiEventNode *pEvent = NULL;
	struct GuiListNode *pNode = NULL;
	struct GuiListNode* pHead = NULL;
	if(pScr == NULL)
		return FALSE;

	//检查 信息是否合法

	//检查该消息是否有重复了
	if (bNoCheck == FALSE)
	{
		GuiListForEach( pNode, & g_ScrEvent.UsedListHead )
		{
			pEvent = CONTAINING_RECORD(pNode, GuiEventNode,NextNode);
			ASSERT(pEvent);
			if(pEvent && pEvent->pScreen == pScr && pEvent->Msg == Msg && pEvent->WParam == wParam 
				&& pEvent->LParam == lParam )
			{
				pEvent->Flag ++;
				return TRUE;
			}
		}
	}

	//从free 里面申请一个消息
	if ( !GuiListIsEmpty(& g_ScrEvent.FreeListHead ) )
	{
		pHead = GuiListRemoveHead( & g_ScrEvent.FreeListHead );
		if (pHead)
		{
			GuiListAppend(  & g_ScrEvent.UsedListHead, pHead);
			pEvent = CONTAINING_RECORD( pHead,GuiEventNode,NextNode );
			ASSERT(pEvent);
			if (pEvent)
			{
				pEvent->pScreen = pScr;
				pEvent->Msg = Msg;
				pEvent->WParam = wParam;
				pEvent->LParam = lParam;
				//
				return TRUE;
			}
		}
	}
	return FALSE;
}
/*********************************************************************************************************
** 函数名称: GetScrEvent
** 函数名称: GetScrEvent
**
** 功能描述：  
**
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月11日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U GetScrEvent( GuiEvent *pEvent )
{

	return TRUE;
}
/*********************************************************************************************************
** 函数名称: EventHandler
** 函数名称: EventHandler
**
** 功能描述：  
**
** 输　入:  GuiEvent * pEvent
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
static INT8U EventHandler( GuiEventNode *pEvent )
{
	ASSERT(pEvent && pEvent->pScreen);
	if (pEvent == NULL || pEvent->pScreen == NULL)
		return FALSE;


	return TRUE;
}
/*********************************************************************************************************
** 函数名称: HandleScreenEvent
** 函数名称: HandleScreenEvent
**
** 功能描述：  
**
** 输　入:  CScreenBase * pScr
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
INT8U HandleScreenEvent(struct SCREEN_BASE *pScr)
{
	GuiEventNode *pEvent = NULL;
	struct GuiListNode *pNode = NULL;
	INT8U bRet = FALSE;
	if(pScr == NULL)
		return FALSE;
	//循环遍历消息列表，如果有属于本窗口的，则执行，并删除
	GuiListForEach( pNode, & g_ScrEvent.UsedListHead )
	{
		pEvent = CONTAINING_RECORD(pNode, GuiEventNode,NextNode);
		ASSERT(pEvent);
		if(pEvent && pEvent->pScreen == pScr)
		{
			//响应消息
			bRet = EventHandler(pEvent);
			//回收
			GuiListRemove( &g_ScrEvent.UsedListHead , pNode );
			GuiListAppend(  &g_ScrEvent.FreeListHead , pNode  );
		}
	}
	return bRet;
}
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
INT8U GuiTaskEventRecv( GuiEvent * pEvent  )
{
	rt_err_t result;
	 
	if (g_mqTask == NULL || pEvent == NULL)
		return FALSE;
	result = rt_mq_recv(g_mqTask, (void*)pEvent, sizeof(GuiEvent), 0);
	if (result == RT_EOK)
	{


		return TRUE;
	}
	return FALSE;
}
/*********************************************************************************************************
** 函数名称: PostEventToGuiTask
** 函数名称: PostEventToGuiTask
**
** 功能描述：  用于其他任务发消息给GUI  
**
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月16日
** 备  注:  通常，定时器任务发送定时到，其他任务发送过来的MessageBox
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U PostEventToGuiTask(  GuiEvent * pEvent )
{
	rt_mq_send(g_mqTask, (void*)pEvent, sizeof(GuiEvent));

	return TRUE;
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
extern INT8U OnTaskTimer(INT32U nEventID,INT32U Param);
INT8U HandleTaskEvent(  GuiEvent * pEvent )
{
	ASSERT(pEvent);
	if(pEvent == NULL)
		return FALSE;
	switch(pEvent->Msg)
	{
		case GUI_EVENT_TIMER:
			if (pEvent->pScreen == NULL)
			{//任务级的定时消息
				return OnTaskTimer(pEvent->WParam,pEvent->LParam);				
			}
			break;
		default:
			break;
	}




	if (pEvent->pScreen == NULL)
	{//
	}

	return TRUE;
}
