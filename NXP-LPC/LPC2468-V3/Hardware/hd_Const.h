///////////////////////////////////////////////////////////////
//	文 件 名 : hdConst.h
//	文件功能 : 硬件配置常量
//	作    者 :  
//	创建时间 : 
//	项目名称 : 
//	操作系统 : 
//	备    注 :
//	历史记录 : 
///////////////////////////////////////////////////////////////
#ifndef _HDCONST_H
#define _HDCONST_H

//定义10s喂狗时间
#define WD_FEED_TIME 10000

#define     ASM_NOP    __asm{NOP}     //空指令

//定义LCD的IO控制脚
#define LCD_RESET   1<<27		//P1.27
#define LCD_RW  	1<<15		//P2.15
#define LCD_CL  	1<<28		//P1.28
#define LCD_E2  	1<<10		//P0.10     
#define LCD_E1 		1<<11		//P0.11
#define LCD_A0  	1<<16		//P4.16
#define LCD_DIR 	1<<26		//P1.26
#define LCD_DB  	0xff<<18	//P1.18..25

//定义LED的IO控制脚
#define	LED1		1 << 19		//P2.19  
#define	LED2		1 << 30		//P0.30
#define	LED3		1 << 18		//P2.18
#define	LED4		1 << 24		//P3.24
#define	LED5		1 << 25		//P3.25
#define	LED6		1 << 29		//P0.29
#define	LED7		1 << 26		//P3.26
#define	LED8		1 << 25		//P2.25
#define	LED9		1 << 24 	//P2.24
#define	LED10		1 << 12		//P0.12
#define	LED11		0x80000000	//P0.31
#define	LED12		1 << 28		//P2.28

#define	DO1A		1 << 23		//P4.23
#define	DO1B		1 << 16		//P0.16
#define	DO2A		1 << 9		//P2.09
#define	DO2B		1 << 8		//P2.08
#define	DO3A		1 << 7		//P2.07
#define	DO3B		1 << 16		//P3.16
#define	DO4A		1 << 27		//P4.27
#define	DO4B		1 << 17		//P3.17
#define	DO5A		1 << 12		//P4.12
#define	DO5B		1 << 18		//P3.18
#define	DO6A		1 << 13		//P4.13
#define	DO6B		1 << 14		//P4.14
#define DO_LATCH	1 << 15		//P0.15

#define DA_LATCH	1 << 7		//P3.07

#define	SSI_DATA	1 << 17		//P4.17
#define	SSI_CLOCK	1 << 18		//P4.18

#define RW_485  	1 << 21  //P3.21
 
//定义液晶长宽
#define LCD_WIDTH    122
#define LCD_HEIGHT   32


#endif
