#define IN_TARGET
#include "includes.h"
#include "hd_init.h"
#include "vicControl.h"
#include "TimerMgr.h"
#include "ClockMgr.h"


void FIQ_Exception(void)
{
    while(1);                   // change it to your code  这一句替换为自己的代码
}
 

void IRQCTimer0(void)
//void __irq  IRQCTimer0(void)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	OS_ENTER_CRITICAL();
	T0IR = 0x01;
	VICVectAddr = 0;
	OS_EXIT_CRITICAL();

	OSTimeTick();
// 	TimerMgrInIsr(10);
	ClockMgrSignal(10);
}
		 

void BSP_init(void)
{
	extern void IRQASMTimer0(void);
//	extern void IRQASMTimer3(void);
	VICIntEnClr 	= 0xFFFFFFFFL;

    T0IR = 0xff;
    T0TCR = 0x03;
    T0TC = 0;
    T0MCR = 0x03;
    T0MR0 = (Fpclk / OS_TICKS_PER_SEC);
    T0TCR = 0x01;
	vicIrqFuncSet(4,0,(INT32U)IRQASMTimer0);
 //	vicIrqFuncSet(4,1,(INT32U)IRQCTimer0);
 
	PCONP |= 3<<22;//使能定时器2，3
	PCONP |= 1<<25;//使能Uart3
	PCONP |= 1<<30;//使能Ethnet

	return;
#if 0
// 	VICIntEnClr	= (1 << 4);
// 	VICVectAddr4 	= (INT32U)IRQASMTimer0;
// 	VICVectPri4 	= (0x20 | 0x04);
// 	VICIntEnable 	= (1 << 4);


	PCONP |= 3<<22;//使能定时器2，3
	PCONP |= 1<<25;//使能Uart3
	PCONP |= 1<<30;//使能Ethnet

  	 
   
//	Timer3_Init();//毫秒中断
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
    return;


	T0IR  = 0xFF;
	T0TC  = 0x00;
	T0TCR = 0x01;
	T0MCR = 0x03;
	T0MR0 = (Fosc / OS_TICKS_PER_SEC );
	VICIntEnClr	= (1 << 4);
	VICVectAddr4 	= (INT32U)IRQASMTimer0;
	VICVectPri14 	= (0x20 | 0x04);
	VICIntEnable 	= (1 << 4);
#endif
}
