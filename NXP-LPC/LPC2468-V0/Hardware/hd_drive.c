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
