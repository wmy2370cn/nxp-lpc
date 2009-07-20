///////////////////////////////////////////////////////////////
//	文 件 名 : hd_Init.c
//	文件功能 : 硬件初始化
//	作    者 : 
//	创建时间 : 2007年8月日
//	项目名称 : 
//	操作系统 : 
//	备    注 : 
//	历史记录 : 
///////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "includes.h"
#include "hd_Init.h"
#include "hd_const.h"
#include "lpc2400.h" 
 
#include "VICControl.h" 

 
/****************************************************************************
* 名	称：CPU_PIN_Init()
* 功	能：I/O模式初始化
* 入口参数：无
* 出口参数：无
****************************************************************************/


void CPU_PIN_Init(void)
{	
	SCS |= 0x01;				
    PINSEL0 = 0x000aa05f;		//PIN设置
    PINSEL1 = 0x01400000;
    PINSEL2 = 0x50150105;
    PINSEL3 = 0x00000005;
    PINSEL4 = 0x05500000;
    PINSEL5 = 0x00c0f000;
    PINSEL9 = 0x0f000a00;
    PINSEL10 = 0x00000000;	    //要保留
   
    PINMODE0=0x00000000;
    PINMODE1=0x00000000;
    PINMODE2=0x00000000;
    PINMODE3=0x00000000;
    PINMODE4=0x00000000;
    PINMODE5=0x00000000;
    PINMODE6=0x00000000;
    PINMODE7=0x00000000;
    PINMODE8=0x00000000;
    PINMODE9=0x00000000;
     
    FIO0DIR = 0xe0019c00;
    FIO1DIR = 0x1c000000;
    FIO2DIR = 0x130c8380;    					 
    FIO3DIR = 0x07270080;
    FIO4DIR = 0x08857000;
} 
  

/*-----------------------------------------------------------------------
* 名	称：Timer3_Init()
* 功	能：Timer3初始化,
* 入口参数：
* 出口参数：无
-----------------------------------------------------------------------*/
void  Timer3_Init(void)
{   
	//OS_ENTER_CRITICAL();
	T3IR = 0xff;
	T3TCR = 0x01;
	T3TC = 0;
	T3MCR = 0x03;
	T3MR0 = MS_TIMER;
	//OS_EXIT_CRITICAL();
}
/****************************************************************************
* 名	称：
* 功	能：硬件初始化，如果硬件板没有改变，各单元的初始化工作无需改变
* 入口参数：无
* 出口参数：无
****************************************************************************/
void SetupHardware(void)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif 
	OS_ENTER_CRITICAL();

 
//    SetVICIRQ(VIC_TIMER3, 5, (INT32U)Timer3_Exception);
//   DisableVICIRQ(VIC_TIMER3);
	

	//定义各中断优先级，用周立功的中断管理接口函数
	//VICInit();
	//开IRQ中断
	//IRQEnable();

	OS_EXIT_CRITICAL();

// 	VICIntSelect = 0;
// 	VICIntEnClr = 0xffffffff;
// 
//  	Timer3_Init();//毫秒中断
//   	vicIrqFuncSet(VIC_TIMER3,5,(INT32U)Timer3_Exception);
//  	vicIrqDisable(VIC_TIMER3);

// 	T3IR  = 0xFF;
// 	T3TC  = 0x00;
// 	T3TCR = 0x01;
// 	T3MCR = 0x03;
// 	T3MR0 = MS_TIMER;
// 	VICIntEnClr	= (1 << 0X1B);
// 	VICVectAddr5 	= (INT32U)Timer3_Exception;
// 	VICVectPri27 	= (0x20 | 0x05);
// 	VICIntEnable 	= (1 << 4);

}
 
