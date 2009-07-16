///////////////////////////////////////////////////////////////
//	文 件 名 : hd_ISR.c
//	文件功能 : 硬件中断服务程序
//	作    者 : 
//	创建时间 : 2007年月日
//	项目名称 : 
//	操作系统 : 
//	备    注 : 
//	历史记录 : 
///////////////////////////////////////////////////////////////
#include "includes.h" 
#include "hd_ISR.h"
#include "hd_Init.h"
#include "hd_drive.h"
#include "IoAdapter.h"
 

extern OS_EVENT *g_pEvtI2C1Mbox;
extern OS_EVENT *g_pEvtI2C2Mbox;
 

/*********************************************************************************************************
** 函数名称 ：IRQ_EINT1/2
** 函数功能 ：中断服务程序，计数脉冲增减
*********************************************************************************************************/
void EINT1_Exception (void)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	OS_ENTER_CRITICAL();	
//	Ram.Abz_Pulse++;
	EXTINT = 2;						// 清外部中断1中断标志		
	VICVectAddr = 0x00;				// 通知VIC中断处理结束
	OS_EXIT_CRITICAL();
}

void EINT2_Exception (void)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	OS_ENTER_CRITICAL();
//	Ram.Abz_Pulse--;
	EXTINT = 4;						// 清外部中断2中断标志		
	VICVectAddr = 0x00;				// 通知VIC中断处理结束
	OS_EXIT_CRITICAL();
} 
INT16U  OPCODE_AD(INT16U ssp1)
{
	INT16U	cnt = 0;
	INT16U	ADcode = 0;
   SSP1DR = ssp1;
//   while((SSP1SR&0x01)==0);		// 等待SPIF置位，即等待数据发送完毕
   while((SSP1SR&0x01)==0)
   {	
   		cnt++;
   		if((SSP1SR&0x01) || cnt > 1000)
   			break;
   }
   ADcode = SSP1DR; 
   return ADcode;
}


//void __irq EINT3_Exception (void)
void EINT3_Exception (void)
{
	CpuGetAinVal(   );
	 
	EXTINT = 0x08;						// 清外部中断2中断标志		
	VICVectAddr = 0x00;					// 通知VIC中断处理结束
 
} 
/****************************************************************************
* 名	称：Timer3_Exception()
* 功	能：定时器3中断服务
* 入口参数：无
* 出口参数：无
****************************************************************************/
#if   0
void Timer3_Exception(void)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	OS_ENTER_CRITICAL();

	T3IR = 0x01;
    VICVectAddr = 0;

 
	OS_EXIT_CRITICAL();//为尽量避免滞后其他中断相应时间，只将比较重要的处理保护起来

	CommTimerHandle();

	//显示部分按键处理窗口定时器
//	DispProc_InMsInt();
	//系统时钟
	ClockMgrSignal(); 
}
#endif // 0
 
