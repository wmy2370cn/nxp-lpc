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
**描        述:   软件定时器,软时钟的处理，系统时间处理
**备        注 :  定时器的回调可以有两种执行方法，一种是在毫秒中断里面直接执行，另外一种以队列存储，然后
                  在任务里面执行，各有优缺点，本模块采用后者实现
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
#include "TaskDef.h"
#include "TimerMgr.h"

static OS_STK	TimerMgrTaskStk [TIMERMR_TASK_STK_SIZE];

static TIMER  g_Timer[MAX_TIMER_CNT];

static OS_EVENT *s_pTmrQeue = NULL;
void *TmrFuncQeueTbl[MAX_TIMER_CNT];

static OS_MEM  *s_pParamMem = NULL;
// INT8U  Param[MAX_TIMER_CNT][4];
static TIMER_MSG TimerMsgPartition[MAX_TIMER_CNT];

/*********************************************************************************************************
** 函数名称: TimerFuncHandle_Task
** 函数名称: TimerFuncHandle_Task
**
** 功能描述： 处理定时器里面的回调
**
** 输　入:  void * pdata
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月17日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void TimerFuncHandle_Task(void *pdata)
{	
	TIMER_MSG  *pTimer = NULL;
	INT8U err = 0;

	pdata = pdata;
	while (TRUE)
	{
		pTimer = (TIMER_MSG *)OSQPend(s_pTmrQeue,0,&err);
		if (err == OS_NO_ERR) 
		{
			if (pTimer && pTimer->pFunc)
			{
				pTimer->pFunc(  pTimer->pParam );
				//释放消息所占用的内存
				OSMemPut(s_pParamMem,pTimer);
			}
		}
		OSTimeDlyHMSM(0,0,0,100);
	}
}    
/*********************************************************************************************************
** 函数名称: InitTimerMgr
** 函数名称: InitTimerMgr
**
** 功能描述： 
**
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月17日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void InitTimerMgr(void )
{
	INT8U err = 0;
	INT8U i = 0;
	//创建队列
	s_pTmrQeue = OSQCreate(& TmrFuncQeueTbl[0] ,MAX_TIMER_CNT);

// 	//创建内存分区，用于保存回调函数参数
  	s_pParamMem = OSMemCreate(TimerMsgPartition,MAX_TIMER_CNT,sizeof(TIMER_MSG ),&err );

	for ( i = 0; i < MAX_TIMER_CNT; i++)
	{
		g_Timer[i].Enable = FALSE;
		g_Timer[i].Count = 0;
		g_Timer[i].TimeOut = 0;
		g_Timer[i].Flag = FALSE;
		g_Timer[i].pFunc = NULL;
		g_Timer[i].pFuncParam = NULL;
	}

	//启动任务
	OSTaskCreate (TimerFuncHandle_Task, (void *)0, 	&TimerMgrTaskStk[TIMERMR_TASK_STK_SIZE-1], PRIO_TMR_MGR);
}
/*********************************************************************************************************
** 函数名称: TimerMgrInMsInt
** 函数名称: TimerMgrInMsInt
**
** 功能描述： 在毫秒中断里面调用
**
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月17日
** 备  注:  这里面存在一个资源冲突的情况，不过无所谓的。
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: ISR 里面已经做了保护了
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern void TimerMgrInIsr( INT16U nCnt )
{
	INT8U err = 0;
	INT16U  i = 0;
	TIMER_MSG *pTimerMsg = NULL;

	if(nCnt == 0)
   		nCnt = 10;

//	OS_ENTER_CRITICAL();	

	for ( i = 0; i < MAX_TIMER_CNT; i++)
	{
		if (g_Timer[i].Enable && g_Timer[i].TimeOut  )
		{
			g_Timer[i].Count += nCnt; //10毫秒一个TICK
			if (g_Timer[i].Count >= g_Timer[i].TimeOut )
			{
				g_Timer[i].Flag = TRUE;
				g_Timer[i].Count = 0;

				if (g_Timer[i].pFunc)
				{	
					//申请一个内存
					pTimerMsg = OSMemGet( s_pParamMem , &err);
					if (pTimerMsg)
					{
						pTimerMsg->pFunc = g_Timer[i].pFunc;
						pTimerMsg->pParam = g_Timer[i].pFuncParam;

						//将消息的回调及参数post到队列里面
						err = OSQPost(s_pTmrQeue,(void *)pTimerMsg );
						if (err != OS_NO_ERR)
						{//如果post不成功的话，则回收内存，防止内存用完
							OSMemPut(s_pParamMem,pTimerMsg);
						}
					}
				}		
			}
		}	
	}
//	OS_EXIT_CRITICAL();
}
 
/*********************************************************************************************************
** 函数名称: SetTimer
** 函数名称: SetTimer
**
** 功能描述： 安装一个定时器
**
** 输　入:  INT16U TimerID
** 输　入:  INT32U nTime
** 输　入:  fnTimerProcess fnProc 定时器的回调函数
** 输　入:  void * pFuncParam     回调函数的参数
**          
** 输　出:  INT16U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT16U  SetTimer(INT16U TimerID, INT32U nTime,fnTimerProcess fnProc,void *pFuncParam )
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	if (TimerID>0 && TimerID <= MAX_TIMER_CNT && nTime)
	{
		OS_ENTER_CRITICAL();	

		g_Timer[TimerID-1].Enable = TRUE;
		g_Timer[TimerID-1].TimeOut = nTime;
		
		g_Timer[TimerID-1].pFunc = fnProc;
		g_Timer[TimerID-1].pFuncParam = pFuncParam;

		OS_EXIT_CRITICAL();
		return TimerID;
	}
	return FALSE;
}

/*********************************************************************************************************
** 函数名称: KillTimer
** 函数名称: KillTimer
**
** 功能描述： 删除定时器
**
** 输　入:  INT16U TimerID
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U   KillTimer (INT16U  TimerID)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	if (TimerID>0 && TimerID <= MAX_TIMER_CNT )
	{
		
		g_Timer[TimerID-1].TimeOut = 0;
		g_Timer[TimerID-1].Count = 0;
		g_Timer[TimerID-1].Flag = FALSE;
		if (g_Timer[TimerID-1].Enable)
		{
			OS_ENTER_CRITICAL();	
			g_Timer[TimerID-1].Enable = FALSE;
			g_Timer[TimerID-1].pFunc = NULL; //可能会有问题 
			g_Timer[TimerID-1].pFuncParam = NULL;
			OS_EXIT_CRITICAL();
			return TRUE;
		}		
	}
	return FALSE;
}

/*********************************************************************************************************
** 函数名称: ResetTimer
** 函数名称: ResetTimer
**
** 功能描述： 定时重新计时
**
** 输　入:  INT16U TimerID
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U  ResetTimer(INT16U TimerID)
{
	if (TimerID>0 && TimerID <= MAX_TIMER_CNT )
	{
		if (g_Timer[TimerID-1].Enable)
		{
		 	g_Timer[TimerID-1].Count = 0;
			return TRUE;
		}		
	}
	return FALSE;
}

/*********************************************************************************************************
** 函数名称: IsTimeTo
** 函数名称: IsTimeTo
**
** 功能描述： 判断定时是否到。
**
** 输　入:  INT16U TimerID
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月17日
** 备  注:  此接口被调用后，定时到标志就被翻转了
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U IsTimeTo(INT16U TimerID )
{
	if (TimerID>0 && TimerID <= MAX_TIMER_CNT)
	{
		if (g_Timer[TimerID-1].Enable && g_Timer[TimerID-1].Flag == TRUE)
		{
			g_Timer[TimerID-1].Flag = FALSE;
			return TRUE;
		}
	}
	return FALSE;
}

/*
//测试用的定时器回调函数
void TimerTestFunc( void * pPara )
{
	INT16U *pCnt =  pPara ;

	INT16U aa = *pCnt;

	aa++;

}

void TestTimer( )
{
	INT16U cnt = 0;
	INT8U bRet = FALSE;
	INT32U nTest = 0;

	SetTimer(1,100,TimerTestFunc,&cnt);
	SetTimer(2,1000,NULL,NULL);

	while (1)
	{
		OSTimeDly(2);
		bRet = IsTimeTo(2);
		if (bRet)
		{
			 nTest ++;
		}
	}
}*/
