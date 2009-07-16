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
**描        述:  串口通信任务，232调试口与485口的任务可能得需要分成2个任务来完成
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
#include "includes.h"
#include "MODBUS.h"
#include "TaskDef.h"
//#include "hd_drive.h"
#include "uart.h"
#include "MBCommon.h"

static OS_STK	DbgMbxTaskStk [DBGMBX_TASK_STK_SIZE];
static OS_STK	CommMbxTaskStk [COMMMBX_TASK_STK_SIZE];

static  MBX  DbgMbx; 
static  MBX  CommMbx;

void MBX_Task(void *pdata)
{	
	MBX * pMBX = NULL;
	pMBX = (MBX *) pdata;

	while (TRUE)
	{
		ModbusMainCycle( pMBX);
		OSTimeDlyHMSM(0,0,0,200);
	}
}    
/*********************************************************************************************************
** 函数名称: SetDeviceAddr
** 函数名称: SetDeviceAddr
**
** 功能描述： 设置装置地址
**
** 输　入:  INT8U Addr
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月22日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SetDeviceAddr( INT8U Addr )
{
	DbgMbx.Address = Addr;
	CommMbx.Address = Addr;
}
 
/*********************************************************************************************************
** 函数名称: InitMBXTsk
** 函数名称: InitMBXTsk
**
** 功能描述： 
**
** 输　入:  INT8U Addr
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月22日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern INT8U InitMBXTsk( void )
{
	InitMBX(&DbgMbx);
//	DbgMbx.pUartRecvSem = OSSemCreate(0);
	poeUart0ReviceSem  =  DbgMbx.pUartRecvSem ;
	
	DbgMbx.UartId = UART0;

	InitMBX(&CommMbx);
//	CommMbx.pUartRecvSem = OSSemCreate(0);
 	poeUart3ReviceSem =  CommMbx.pUartRecvSem  ;
  	CommMbx.UartId = UART3;

	//启动调试口通信任务
//   OSTaskCreate (DbgMBX_Task, 0, 	&DbgMbxTaskStk[DBGMBX_TASK_STK_SIZE-1], DBGMBX_TASK_PRIO);

	//启动通信口口通信任务
//	OSTaskCreate (CommMBX_Task, 0, 	&CommMbxTaskStk[COMMMBX_TASK_STK_SIZE-1], COMMMBX_TASK_PRIO);

	OSTaskCreateExt (MBX_Task, (void *)&CommMbx, &CommMbxTaskStk[COMMMBX_TASK_STK_SIZE-1],
		COMMMBX_TASK_PRIO,
		COMMMBX_TASK_PRIO,
		&CommMbxTaskStk [0],
		COMMMBX_TASK_STK_SIZE,
		(void *)&CommMbx,
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); 

	OSTaskCreateExt (MBX_Task, (void *)&DbgMbx, &DbgMbxTaskStk[DBGMBX_TASK_STK_SIZE-1],
		DBGMBX_TASK_PRIO,
		DBGMBX_TASK_PRIO,
		&DbgMbxTaskStk [0],
		DBGMBX_TASK_STK_SIZE,
		(void *)&DbgMbx,
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); 

	return TRUE;
}

extern void CommTimerHandle( )
{
	DbgMbx.RxTimer++;
	DbgMbx.TxTimer++;
	 
	CommMbx.RxTimer++;
	CommMbx.TxTimer++;
	 	
	if(DbgMbx.RxCount>0 && DbgMbx.RxTimer>RX_OVERTIME)	//在接受态
	{
		DbgMbx.RxCount=0;
		DbgMbx.RxTimer=0;
	}
	if(CommMbx.RxCount>0 && CommMbx.RxTimer>RX_OVERTIME)	//在接受态
	{
		CommMbx.RxCount=0;
		CommMbx.RxTimer=0;
	}	
}
