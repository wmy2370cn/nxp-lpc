///////////////////////////////////////////////////////////////
//	文 件 名 : hd_drive.h
//	文件功能 : 硬件驱动
//	作    者 :  
//	创建时间 : 
//	项目名称 : 
//	操作系统 : 
//	备    注 :
//	历史记录 : 
///////////////////////////////////////////////////////////////
#ifndef _HD_DRIVE_H
#define	_HD_DRIVE_H

#include "hd_init.h"
 								  
//#define SetTickVic(ignore) ReEnableVICIRQ(VIC_TIMER0); //开节拍中断
/*******************************************************************************************
** 函数功能 ： 延时
*******************************************************************************************/
void DelayNS (INT32U dly);
//void Delayus (INT32U dly);
#define hd_GetDigitalIn(ignore) (~(INT16U)FIO4PIN & 0x0FFF)

#define Reset 	Reset_Handler


/****************************************************************************
* 名称：GeAnalogIn()
* 功能：AD采集
* 入口参数：无
** 出口参数AD转换值
****************************************************************************/
extern void hd_GetAnalogIn(INT8U chNo);


/* 定义用于和I2C2中断传递信息的全局变量 */
extern volatile INT8U 	I2C_sla;			/* I2C器件从地址 					*/
extern volatile INT32U	I2C_suba;			/* I2C器件内部子地址 				*/
extern volatile INT8U 	I2C_suba_num;		/* I2C子地址字节数					*/
extern volatile INT8U 	*I2C_buf;        	/* 数据缓冲区指针  					*/
extern volatile INT32U 	I2C_num;			/* 要读取/写入的数据个数 			*/
extern volatile INT8U 	I2C_end;			/* I2C总线结束标志：结束总线是置1 	*/
extern volatile INT8U 	I2C_suba_en;		/* 	子地址控制。
											0--子地址已经处理或者不需要子地址
											1--读取操作
											2--写操作
											*/

/* 定义用于和I2C1中断传递信息的全局变量 */
extern volatile INT8U 	I2C1_sla;			/* I2C器件从地址 					*/
extern volatile INT32U	I2C1_suba;			/* I2C器件内部子地址 				*/
extern volatile INT8U 	I2C1_suba_num;		/* I2C子地址字节数					*/
extern volatile INT8U 	*I2C1_buf;        	/* 数据缓冲区指针  					*/
extern volatile INT32U 	I2C1_num;			/* 要读取/写入的数据个数 			*/
extern volatile INT8U 	I2C1_end;			/* I2C总线结束标志：结束总线是置1 	*/
extern volatile INT8U 	I2C1_suba_en;		/* 	子地址控制。
											0--子地址已经处理或者不需要子地址
											1--读取操作
											2--写操作
											*/
#endif

