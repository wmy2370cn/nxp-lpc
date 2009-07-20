//任务相关的设置

#ifndef _TASK_DEFINE_H
#define _TASK_DEFINE_H

//定义各任务的堆栈长度
#define  GUI_TASK_STK_SIZE      512
#define  PRIO_GUI_TASK          30

#define  KEY_SCAN_TASK_STK_SIZE  128
#define  PRIO_KEY_SCAN_TASK       32


#define	 TIMERMR_TASK_STK_SIZE  	256   //软定时器任务
#define  PRIO_TMR_MGR               22

#define	 FRAMWR_TASK_STK_SIZE       512   //铁电定时保存任务
#define  PRIO_FRAM_SAVE   	        7

#define	 ADVCALC_TASK_STK_SIZE  	512   //闸门计算
#define  GATAGECALC_TASK_PRIO       25        

#define  DBGMBX_TASK_STK_SIZE    512 
#define  DBGMBX_TASK_PRIO        26               

#define  COMMMBX_TASK_STK_SIZE    512 
#define  COMMMBX_TASK_PRIO        21   

#define  CLK_TASK_PRIO        50                 /* This defines the priority of ClkTask()             */
#define  CLK_TASK_STK_SIZE   256                 /* Stack size in BYTEs for ClkTask()                  */


#define	MainTaskStkLen			256
#define ModbusRtuTaskStkLen		512
#define	FRamProcTaskStkLen		256
#define DOutProcTaskStkLen		256
#define CalcMeasTaskStkLen		512
#define SelfCheckTaskStkLen		128
#define AssistantTaskStkLen		128
#define DispTaskStkLen			512
#define FLoadTaskStkLen			128

//定义各任务优先级
#define	PRIO_MAIN_TASK      	14
#define PRIO_SELF_CHECK		4

#define PRIO_CALC_MEAS		6
#define PRIO_DIGITAL_Out	5	
#define PRIO_DISP			15
#define PRIO_LOAD			13

#define	PRIO_MODBUSRtu_DBG 	16
#define	PRIO_MODBUSRtu_COM 	9

#define PRIO_ASSISTANT		17
 	
// #define	PRIO_ModbusTCP   	8
// #define PRIO_TCPPROC		10
// #define	PRIO_TCPLISTEN		12
// #define PRIO_TCPNODE		11

#endif
