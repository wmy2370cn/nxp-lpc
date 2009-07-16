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
#include "filedata.h"
#include "hd_isr.h"
#include "VICControl.h"

static INT8U s_nEncoderType = INTERFACE_NONE;


OS_EVENT *g_pEvtI2C1Sem;
OS_EVENT *g_pEvtI2C1Mbox;
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
* 名	称：EINT12_Init()
* 功	能：EINT1、2初始化,脉冲捕获计数器
* 入口参数：
* 出口参数：无
-----------------------------------------------------------------------*/
void  EINT12_Init(void)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif 
	OS_ENTER_CRITICAL();
	EXTMODE = 0x07;
	EXTPOLAR = 0x06;					//下降沿触发
	EXTINT = 1<<2 | 1<<3 | 1<<4;		// 清外部中断中断标志
	OS_EXIT_CRITICAL();
}

/*-----------------------------------------------------------------------
* 名	称：I2C1_Init()
* 功	能：主模式I2C初始化，包括初始化其中断为向量IRQ中断。
* 入口参数：fi2c		初始化I2C总线速率，最大值为400K
* 出口参数：无
-----------------------------------------------------------------------*/
void  I2C1_Init(INT32U fi2c)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif 
	OS_ENTER_CRITICAL();

	if(fi2c>400000) 
		fi2c = 400000;

	I21SCLH = (Fpclk/fi2c + 1) / 2;	// 设置I2C时钟为fi2c
	I21SCLL = (Fpclk/fi2c) / 2;
	I21CONCLR = 0x2C;
	I21CONSET = 0x40;				// 使能主I2C
	//变量初始化
	g_pEvtI2C1Sem = OSSemCreate(1);
	g_pEvtI2C1Mbox = OSMboxCreate(NULL);
	if (g_pEvtI2C1Mbox == NULL)
	{
		return ;
	}
	if (g_pEvtI2C1Sem == NULL)
	{
		return ;
	}
	OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** 函数名称: Encoder_Init
** 函数名称: 本模块内部使用，不对外开放
**
** 功能描述： 初始化编码器，读IO判断出编码器类型
**
** 输　入:  void
**          
** 输　出:  INT16U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月5日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U  Encoder_Init( )
{	
	INT8U nType = INTERFACE_NONE;
	switch(FIO2PIN&0x9<<14)	//编码器识别，固定的硬件引脚，不能改变
	{
	case 0:			//增量型编码器
		nType = INTERFACE_INCREMENTAL;
		EINT12_Init(); 
		break;
	case 0x1<<17:	//绝对型SSI编码器
		nType = INTERFACE_SSI;
		break;			
	case 0x1<<14:	//绝对型并口编码器
		nType = INTERFACE_PARALLEL;
		I2C1_Init(100000);//初始化总线
	//	SetVICIRQ(VIC_I2C1, 10, (INT32U)I2C1_Exception);//定义中断优先级并开中断
		break;
	default:
		nType = INTERFACE_NONE;
		break;
	}	
	return nType;
}
/****************************************************************************
* 名	称：SPI_Init()
* 功	能：初始化SPI接口，SPI0:DA输出，SPI1:AD输入 
* 入口参数：无
* 出口参数：无
****************************************************************************/
/****************************************************************************
** 功能：
**************************************************************************/
void  SPI0_Init( void )				//DA
{	
	//OS_ENTER_CRITICAL();
	SSP0CR0=0x0000000F;
	SSP0CR1=0x02;
	SSP0CPSR=0x32;
	//OS_EXIT_CRITICAL();	
}

void  SPI1_Init( void )				//AD
{	
	//OS_ENTER_CRITICAL();
	SSP1CR0=0x0087;
	SSP1CR1=0x02;
	SSP1CPSR=0x32;

	SetVICIRQ(VIC_EINT3, 11, (INT32U)EINT3_Exception);
	DisableVICIRQ(VIC_EINT3);
	EXTINT = 0x08;					// 清外部中断3中断标志		

	hd_ADSoftReset();//复位AD
	hd_ADSoftReset();

	OPCODE_AD(0x28);
	OPCODE_AD(0x03);
	DelayNS(1);
	OPCODE_AD(0x2a);
	OPCODE_AD(0x03);
	//OS_EXIT_CRITICAL();
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

#if 0
	//判断编码器模块类型，决定是否初始化外部中断
	s_nEncoderType = Encoder_Init();
	//定时器初始化
	Timer3_Init();//毫秒中断
	//SPI初始化，for AD DA 
	EINT12_Init();
	SPI0_Init();
	SPI1_Init();
#endif
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

/****************************************************************************
* 名	称：
* 功	能：硬软件初始化完之后，打开被关闭的中断
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Enable_Interupt(void)
{
	EXTINT = 1<<2 | 1<<3;
	ReEnableVICIRQ(VIC_EINT1);
	ReEnableVICIRQ(VIC_EINT2);
	ReEnableVICIRQ(VIC_TIMER3);
	ReEnableVICIRQ(VIC_UART0);
	ReEnableVICIRQ(VIC_UART3);

	ReEnableVICIRQ(VIC_EINT3);
}
