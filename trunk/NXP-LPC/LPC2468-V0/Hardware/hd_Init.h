///////////////////////////////////////////////////////////////
//	文 件 名 : hd_Init.h
//	文件功能 : 硬件初始化
//	作    者 : 
//	创建时间 : 2007年月日
//	项目名称 : 
//	操作系统 : 
//	备    注 : 
//	历史记录 : 
///////////////////////////////////////////////////////////////

#ifndef HD_INIT_H
#define HD_INIT_H


/* 定义串口模式设置数据结构 */
typedef  struct  UART_MODE
{  
	INT16U datab;         // 字长度，5/6/7/8
	INT16U stopb;         // 停止位，1/2
	INT16U parity;    	  // 奇偶校验位，0为无校验，1奇数校验，2为偶数校验
}UART_MODE;

typedef struct tagTI2C
{
	INT8U 	I2C_sla;			//I2C器件从地址
	INT8U 	I2C_suba_num;		//I2C子地址字节数
	INT8U 	*I2C_buf;			//读写数据缓冲区指针
	INT8U 	I2C_end;			//I2C总线结束标志：结束总线是置1
	INT8U 	I2C_suba_en;		/* 	子地址控制。
								0--子地址已经处理或者不需要子地址
								1--读取操作
								2--写操作
								*/
	INT16U	I2C_num;			//要读取/写入的数据个数
	INT16U	I2C_suba;			//I2C器件内部子地址
}TI2C;

//定义毫秒时钟时，T1MR3的比较值
#define MS_TIMER Fpclk/1000

#define COMM_485_TIMER	20000

//定义中断源
#define		VIC_EINT1		0x0F
#define		VIC_EINT2		0x10
#define		VIC_EINT3		0x11
#define		VIC_TIMER0		0x04
#define		VIC_TIMER1		0x05
#define		VIC_TIMER2		0x1A
#define		VIC_TIMER3		0x1B
#define		VIC_UART0		0x06
#define		VIC_UART3		0x1D
#define		VIC_PWM0		0x08
#define		VIC_I2C1		0x13
#define		VIC_I2C2		0x1E


#define Fosc                11059200			//	OSC = 11.0592MHz
#define Fcclk	(Fosc * 4)							// 主频 Fcclk = 44.2368MHz
#define Fcco    (Fcclk* 6)
	
#define Fpclk	(Fcclk / 4)
	
#define PLL_NValue			1	
#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
#define CCLKDivValue		(Fcco/Fcclk-1)
#define USBCLKDivValue		254		

/* 定义串口模式设置数据结构 */
//typedef  struct  UART_MODE
//{  
//	INT16U datab;         // 字长度，5/6/7/8
//	INT16U stopb;         // 停止位，1/2
//	INT16U parity;    	  // 奇偶校验位，0为无校验，1奇数校验，2为偶数校验
//}UART_MODE;

extern void CPU_PIN_Init(void);
extern void SetupHardware(void); 
#endif

