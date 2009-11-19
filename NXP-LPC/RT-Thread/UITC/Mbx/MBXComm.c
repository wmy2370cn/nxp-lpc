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
**   File: $Id: ,v 1.0 2009/11/15 15:27:16   $
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
#include <rtthread.h>
#include "applib.h"
#include "MbxRtu.h"


#define  DBGMBX_TASK_STK_SIZE    128 
#define  DBGMBX_TASK_PRIO        26               

#define  COMMMBX_TASK_STK_SIZE    128 
#define  COMMMBX_TASK_PRIO        21   


static INT32U	DbgMbxTaskStk [DBGMBX_TASK_STK_SIZE];
static INT32U	Comm1MbxTaskStk [COMMMBX_TASK_STK_SIZE];
static INT32U	Comm2MbxTaskStk [COMMMBX_TASK_STK_SIZE];

static  CMBExt  DbgMbx; 
static  CMBExt  Comm1Mbx;
static  CMBExt  Comm2Mbx;

static struct rt_thread DbgMbxTsk;
static struct rt_thread Comm1MbxTsk;
static struct rt_thread Comm2MbxTsk;


void MBX_Task(void *pdata)
{	
	CMBExt * pMBX = NULL;
	pMBX = (CMBExt *) pdata;

	while (TRUE)
	{
		ModbusMainCycle( pMBX);
 	}
}   

INT8U InitMBXTask( void )
{
	rt_thread_init(&DbgMbxTsk,"dbg_thread",MBX_Task, (void *)&DbgMbx,	&DbgMbxTaskStk[0], sizeof(DbgMbxTaskStk),
		DBGMBX_TASK_PRIO, 10);
	rt_thread_startup(&DbgMbxTsk);

	rt_thread_init(&Comm1MbxTsk,"comm1_thread",MBX_Task, (void *)&Comm1Mbx,	&Comm1MbxTaskStk[0], sizeof(Comm1MbxTaskStk),
		COMMMBX_TASK_PRIO, 10);
	rt_thread_startup(&Comm1MbxTsk);
	
	rt_thread_init(&Comm2MbxTsk,"comm2_thread",MBX_Task, (void *)&Comm2Mbx,	&Comm2MbxTaskStk[0], sizeof(Comm2MbxTaskStk),
		COMMMBX_TASK_PRIO, 10);
	rt_thread_startup(&Comm1MbxTsk);
	return TRUE;
}

