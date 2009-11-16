 /****************************************Copyright (c)**************************************************
**							     
**     所有窗口的消息队列 窗口消息队列由统一的消息池，其操作在一个线程内，              
**     任务间的消息队列，采用IPC进行管理                           
**     TODO:看看是否采用静态内存池来实现？
**                           
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

struct GUI_EVENT_MGR
{
	GuiEvent *pEvent;
	struct GuiListNode FreeListHead;
	struct GuiListNode UsedListHead;
};

typedef struct GUI_EVENT_MGR GuiEventMgr;
 //窗口的消息队列，属于同一个进程内，
GuiEventMgr g_ScrEvent;

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
INT8U InitGuiEventMgr( void )
{
	INT16U i  = 0;
	GuiEvent *pEvent = NULL;
	g_ScrEvent.pEvent = rt_malloc( sizeof(GuiEvent) * GUI_EVENT_CNT );
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
	GuiEvent *pEvent = NULL;
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
			pEvent = CONTAINING_RECORD(pNode, GuiEvent,NextNode);
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
			pEvent = CONTAINING_RECORD( pHead,GuiEvent,NextNode );
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
static INT8U EventHandler( GuiEvent *pEvent )
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
	GuiEvent *pEvent = NULL;
	struct GuiListNode *pNode = NULL;
	INT8U bRet = FALSE;
	if(pScr == NULL)
		return FALSE;
	//循环遍历消息列表，如果有属于本窗口的，则执行，并删除
	GuiListForEach( pNode, & g_ScrEvent.UsedListHead )
	{
		pEvent = CONTAINING_RECORD(pNode, GuiEvent,NextNode);
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

static rt_mailbox_t g_mbTask ;

INT8U InitTaskEvent( void )
{
// 	g_mbTask = rt_mb_create("TSK_ENT",sizeof(GuiEvent)*GUI_EVENT_CNT/4,RT_IPC_FLAG_FIFO);
// 	if (g_mbTask == RT_NULL)
// 		return FALSE;

//TODO: 用消息队列来实现比较合理
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
INT8U GuiTaskEventRecv( )
{

	return TRUE;
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
INT8U PostEventToGuiTask( )
{

	return TRUE;
}
