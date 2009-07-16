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
#include "includes.h"

#include "TaskDef.h"
#include "hd_drive.h"	  
#include "IoAdapter.h"
#include "hd_Init.h"
#include "ram.h"
#include "UEInit.h"
#include "viccontrol.h"
#include "TimerMgr.h"
#include "ClockMgr.h"
#include "MODBUS.h"
#include "AdvCalc.h"
#include "Gui.h"
#include "FRamMgr.h"

 
 

void FlickerLed( )
{
	INT32U i = 0;
	for (i = 0; i < 12; i++)
	{
		SetLed(i,TRUE);	 
	}
	OSTimeDlyHMSM(0,0,0,500);   	 
	for (i = 0; i < 12; i++)
	{
		SetLed(i,FALSE);	 
	}

   	OSTimeDlyHMSM(0,0,0,300);  
	for (i = 0; i < 12; i++)
	{
		SetLed(i,TRUE);	 
	}
	OSTimeDlyHMSM(0,0,0,300);   	 
	for (i = 0; i < 12; i++)
	{
		SetLed(i,FALSE);	 
	}
	OSTimeDlyHMSM(0,0,0,200);  
	for (i = 0; i < 12; i++)
	{
		SetLed(i,TRUE);	 
	}
	for (i = 0; i < 12; i++)
	{
		SetLed(i,TRUE);
	 	OSTimeDlyHMSM(0,0,0,300);
		SetLed(i,FALSE);
	}
	OSTimeDlyHMSM(0,0,0,200);  
	 	 
	for (i = 0; i < 12; i++)
	{
		SetLed(i,FALSE);	 
	}
	for (i = 11; ;i--)
	{
		SetLed(i,TRUE);
	 	OSTimeDlyHMSM(0,0,0,300);
		SetLed(i,FALSE);
		if (i == 0)
			break;
	}
}

extern void IRQASMTimer3(void);

void IRQCTimer3(void)
{
 
	T3IR = 0x01; //清除中断标志
	VICVectAddr = 0;  //通知VIC中断处理结束
 	TimerMgrInIsr(1); 
}
 

static void DelayS (INT32U dly)
{
	INT32U i;
	for ( ; dly>0; dly--)
		for (i=0; i<5000; i++);
}
 
void MainTask(void *pdata)
{
	INT8U bRet = FALSE;

	INT8U count = 0;
	INT16U nTimerId = 1;
 	INT8U nLedState = FALSE; 

	pdata = pdata;
	BSP_init();
   	CPU_PIN_Init(); 
 
	//硬件初始化
	SetupHardware();
	//清狗
	// 	hd_ClearWatchDog();
	//延时一段时间，等待硬件各部分稳定
	DelayS(300);
	//软件初始化
	bRet = InitConfData( &Ram ); 
	if (bRet == FALSE)
	{
		SetLed(LED_FAULT,TRUE);
	}
	//初始化装置软件版本号
	InitDeviceVer ( &Ram );
	//定时器启动
	InitTimerMgr();
	//初始化FRAM,并从FRAM里面加载出装置时间
	InitFRamMgrTsk();
	//初始化时钟模块
	InitClockMgr();

	InitGatageTsk( );
	//通信任务
	InitMBXTsk();

	InitGuiTask();

	//开中断
//	Enable_Interupt();

 
	T3IR  = 0xFF;	  //清除全部中断标志
	T3TC  = 0x00;	  //定时器计数器
	T3TCR = 0x01;     //启动定时器
	T3MCR = 0x03;     //匹配寄存器,当定时器的TC值与MR0匹配时,定时器复位,并产生中断
	T3MR0 = MS_TIMER;

//  VICIntEnClr	= (1 << 0X1B);	 //中断使能清零 寄存器
// 	VICVectAddr27 	= (INT32U)IRQASMTimer3;
// 	VICVectPri27 	= (0x20 | 0x05);
// 	VICIntEnable 	= (1 <<  0X1B);		 //中断使能 寄存器


   	vicIrqFuncSet(VIC_TIMER3,2,(INT32U)IRQASMTimer3);
 

	 FlickerLed( );
    //vicIrqEnable(4);
	SetTimer(nTimerId,1000,NULL,NULL);
	 while(1)
	 {
 	//	OSTimeDlyHMSM(0,0,0,500);
 	//	FlickerLed( );
		 
		 bRet = IsTimeTo(nTimerId);
		 if (bRet)
		 {
			 nLedState = !nLedState;
			 SetLed(LED_RUN,nLedState);

			 //测试
			 TestMemData();				
		 }

// 		 nLedState = !nLedState;
// 		 SetLed(LED_RUN+1,nLedState);
		 OSTimeDlyHMSM(0,0,0,200);
	//    FlickerLed( );
	//	 OSTimeDly(OS_TICKS_PER_SEC/20 + 1);	//50ms
	 } 	
}

