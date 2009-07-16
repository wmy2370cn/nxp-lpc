///////////////////////////////////////////////////////////////
//	文 件 名 : hd_drive.c
//	文件功能 : 硬件驱动
//	作    者 :  
//	创建时间 : 
//	项目名称 : 
//	操作系统 : 
//	备    注 :
//	历史记录 : 
///////////////////////////////////////////////////////////////
#include "includes.h"
#include "hd_drive.h" 
#include "hd_Init.h"
#include "hd_Const.h"
//#include "VIC_Control.h" 

/* 定义用于和I2C2中断传递信息的全局变量 */
volatile INT8U 	I2C_sla;			/* I2C器件从地址 					*/
volatile INT32U	I2C_suba;			/* I2C器件内部子地址 				*/
volatile INT8U 	I2C_suba_num;		/* I2C子地址字节数					*/
volatile INT8U 	*I2C_buf;        	/* 数据缓冲区指针  					*/
volatile INT32U I2C_num;			/* 要读取/写入的数据个数 			*/
volatile INT8U 	I2C_end;			/* I2C总线结束标志：结束总线是置1 	*/
volatile INT8U 	I2C_suba_en;		/* 	子地址控制。
									0--子地址已经处理或者不需要子地址
									1--读取操作
									2--写操作
									*/

/* 定义用于和I2C1中断传递信息的全局变量 */
volatile INT8U 	I2C1_sla;			/* I2C器件从地址 					*/
volatile INT32U	I2C1_suba;			/* I2C器件内部子地址 				*/
volatile INT8U 	I2C1_suba_num;		/* I2C子地址字节数					*/
volatile INT8U 	*I2C1_buf;        	/* 数据缓冲区指针  					*/
volatile INT32U I2C1_num;			/* 要读取/写入的数据个数 			*/
volatile INT8U 	I2C1_end;			/* I2C总线结束标志：结束总线是置1 	*/
volatile INT8U 	I2C1_suba_en;		/* 	子地址控制。
									0--子地址已经处理或者不需要子地址
									1--读取操作
									2--写操作
									*/
extern OS_EVENT *g_pEvtI2C1Sem;


extern OS_EVENT *g_pEvtI2C1Mbox;



extern OS_EVENT *g_pEvtI2C1Sem;
extern OS_EVENT *g_pEvtI2C1Mbox;

/*******************************************************************************************
** 函数名称 ： DelayNS
*******************************************************************************************/
void DelayNS (INT32U dly)
{
	INT32U i;
	for ( ; dly>0; dly--)
		for (i=0; i<5000; i++);
}

void hd_SetOneLED(INT8U led_num)
{	
	switch(led_num)
	{
		case 0:
		FIO2CLR =LED1;	break;
		case 1:
		FIO0CLR =LED2;	break;			
		case 2:
		FIO2CLR =LED3;	break;	
		case 3:
		FIO3CLR =LED4;	break;
		case 4:
		FIO3CLR =LED5;	break;
		case 5:
		FIO0CLR =LED6;	break;			
		case 6:
		FIO3CLR =LED7;	break;	
		case 7:
		FIO2CLR =LED8;	break;
		case 8:
		FIO2CLR =LED9;	break;
		case 9:
		FIO0CLR =LED10;	break;			
		case 10:
		FIO0CLR =LED11;	break;	
		case 11:
		FIO2CLR =LED12;	break;
	}
}

void hd_ClearOneLED(INT8U led_num)
{	
	switch(led_num)
	{
		case 0:
		FIO2SET =LED1;	break;
		case 1:
		FIO0SET =LED2;	break;			
		case 2:
		FIO2SET =LED3;	break;	
		case 3:
		FIO3SET =LED4;	break;
		case 4:
		FIO3SET =LED5;	break;
		case 5:
		FIO0SET =LED6;	break;			
		case 6:
		FIO3SET =LED7;	break;	
		case 7:
		FIO2SET =LED8;	break;
		case 8:
		FIO2SET =LED9;	break;
		case 9:
		FIO0SET =LED10;	break;			
		case 10:
		FIO0SET =LED11;	break;	
		case 11:
		FIO2SET =LED12;	break;
	}	
}

/****************************************************************************
* 名称：GeAnalogIn()
* 功能：AD采集
* 入口参数：无
** 出口参数AD转换值
****************************************************************************/
void hd_GetAnalogIn(INT8U chNo)
{
	if(chNo == 1)
	{
		OPCODE_AD(0x38);
		OPCODE_AD(0x40);
	}
	else if(chNo == 2)
	{
		OPCODE_AD(0x3a);
		OPCODE_AD(0x40);
	}
	DelayNS(10);

	//	while(FIO2PIN&1<<13)
	//	EINT3_Exception();	
}

/****************************************************************************
* 名称：hd_ADSoftReset()
* 功能：AD复位
* 入口参数：无
** 出口参数
****************************************************************************/
void hd_ADSoftReset(void)
{
	OPCODE_AD(0x00);
	OPCODE_AD(0xFF);
	OPCODE_AD(0xFF);
	OPCODE_AD(0xFF);
	OPCODE_AD(0xFF);
	DelayNS(10);
}