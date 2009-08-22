/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                             EXAMPLE CODE
*
*                                             NXP LPC2468
*                                                on the
*                                  Embedded Artists LPC2468 OEM Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

#include <includes.h>
#include <LPC23xx.H>
#include "smartarm2300.h"
#include "TimerExt.h"

/*
*********************************************************************************************************
*                                           #DEFINE CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK          AppTaskStartStk[APP_TASK_START_STK_SIZE];

/*
*********************************************************************************************************
*                                         LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void            AppTaskStart                (void *p_arg); 



/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary 68HC12 and C initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    CPU_INT08U  err;


    BSP_IntDisAll();                                            /* Disable all interrupts until we are ready to accept them */

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"              */

    OSTaskCreateExt(AppTaskStart,                               /* Create the start task                                    */
                    (void *)0,
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppTaskStartStk[0],
                    APP_TASK_START_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_SIZE > 13
    OSTaskNameSet(APP_TASK_START_PRIO, "Start Task", &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}

static  OS_EVENT *pLedMsgQ = NULL;
#define MAX_LEDMSG_CNT 10
void *LedMsgQeueTbl[MAX_LEDMSG_CNT];


void InitLedMsgMgr( )
{
	pLedMsgQ = OSQCreate(LedMsgQeueTbl,10 );
}

/*********************************************************************************************************
** 函数名称: SendLedMsg
** 函数名称: SendLedMsg
**
** 功能描述：  
**
** 输　入:  INT16U * pMsg
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年8月22日
** 备  注:  此处是临时做法,输入参数需要是全局变量
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U SendLedMsg( INT16U *pMsg )
{
	INT8U err = 0;
	err = OSQPost(pLedMsgQ,pMsg);
	if (err == OS_NO_ERR)
		return OS_TRUE;
	
	return OS_FALSE;
}

static void LedMsgHandler( )
{
	INT8U err = 0 ;
	INT16U *pMsg = NULL;
	if (pLedMsgQ == NULL)
		return;

	pMsg = OSQAccept(pLedMsgQ,&err);
	if (err == OS_NO_ERR)
	{
		if (pMsg)
		{
			if (*pMsg >= 2 && *pMsg <=8)
			{
				LED_On(*pMsg);
				OSTimeDlyHMSM(0,0,0,100);
				LED_Off(*pMsg);
			}
		}
	}
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    void           *msg;
    CPU_INT08U      err;
	INT8U   nLedState = 0;
	INT16U nTimerID = 0;
    

    (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                                 */

#if OS_TASK_STAT_EN > 0
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif
	eth_system_device_init();

	rt_hw_eth_init();
	lwip_sys_init( );
	InitTimerMgr(  );
	InitLedMsgMgr( );
	nTimerID = SetTimer(nTimerID,500);

  
    
	while (DEF_TRUE)
	{   
		LedMsgHandler( );
		if (IsTimeTo(nTimerID))
		{
			nLedState = !nLedState;
			if (nLedState)
			{
				LED_On(1);
			}
			else
			{
				LED_Off(1);
			}
		}
		OSTimeDly(10) ;
	//	OSTimeDlyHMSM(0, 0, 1, 0);
	//	OSTimeDlyHMSM(0,0,1,0);
	//	LED_On(1);
	//	OSTimeDlyHMSM(0,0,1,0);
	 //	LED_Off(1);
	}
}
 