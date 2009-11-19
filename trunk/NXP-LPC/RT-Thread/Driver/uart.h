/****************************************Copyright (c)**************************************************
**                                       UART       
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 
**描        述:  
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本:  
** 日　期:  
** 描　述: 
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef __UART_H
#define __UART_H
#include "DataQueue.h"



/*********************************************************************************************************
   UART器件编号,用户不可修改
*********************************************************************************************************/
#define UART0   0
#define UART1   1
#define UART2   2


/*********************************************************************************************************
  定义使用到的UART寄存器偏移地址,用户不可修改
*********************************************************************************************************/
#define __B_UART_RBR    0  //接收器缓存寄存器 只读
#define __B_UART_THR    0  //发送保持寄存器 只写
#define __B_UART_DLL    0
#define __B_UART_DLM    1
#define __B_UART_IER    1
#define __B_UART_IIR    2
#define __B_UART_FCR    2
#define __B_UART_LCR    3
#define __B_UART_LSR    5  //线状态寄存器，只读
#define __B_UART_SCR    7

/*********************************************************************************************************
   标识是接收队列还是发送队列,用户不可修改
*********************************************************************************************************/
#define RX_FIFO     0
#define TX_FIFO     1

/*********************************************************************************************************
   操作参数返回,用户不可修改
*********************************************************************************************************/
#define UART_OK             1                                           /*  串口操作成功                */                
#define UART_NOK            0                                           /*  串口操作失败                */

/*********************************************************************************************************
   定义Uart器件数目
*********************************************************************************************************/
#define UART_MAX_NUM    3                                             /*  LPC22294有2个UART            */

/*********************************************************************************************************
   UART器件信息结构结构体,用户不可修改
*********************************************************************************************************/
struct UART_INFO 
{
                INT32U      uiOffBase;                                  /*  uart寄存器间隔= 1 << OffBase*/                         
    volatile    INT32U     *puiAddrBase;                                /*  uart寄存器基地址            */  
                 
                INT32U      uiQueueReviceFifoLen;                       /*  软件接收FIFO长度            */
                INT32U      uiQueueSendFifoLen;                         /*  软件发送FIFO长度            */
                
                DataQueue  *pDQSendBuf;                                 /*  发送队列                    */
                DataQueue  *pDQReviceBuf;                               /*  接收队列                    */
                
                INT32U      uiUartFlag;                                 /*  7位置位表示不用队列缓冲
                                                                            
                                                                            4位置位用于队列使能         */                                                                                                        
                
                INT32U      uiUartId;                                   /*  用于记录自己的UART通道号    */
                INT32U      uiUartState;                                /*  记录收发状态1表示发,0表示收 */
				struct rt_semaphore RcvDataSem;
}; 
typedef struct UART_INFO     UART_INFO;                    

typedef  struct  UART_PARAM
{  
	INT32U bps;           //波特率
	INT16U datab;         // 字长度，5/6/7/8
	INT16U stopb;         // 停止位，1/2
	INT16U parity;    	  // 奇偶校验位，0为无校验，1奇数校验，2为偶数校验
	INT16U rtsctl;        
}UART_PARAM;

/*********************************************************************************************************
   配置UART硬件FIFO长度,用户可修改,但没必要时不要修改,修改前先了解硬件FIFO
*********************************************************************************************************/ 
#define UARTFIFOLEN1     0     
#define UARTFIFOLEN4     1
#define UARTFIFOLEN8     2
#define UARTFIFOLEN14    3

//#define UART0FIFOLENFUN  UARTFIFOLEN8                                /*  配置UART0的接收FIFO触发深度   */
#define UART0FIFOLENFUN  UARTFIFOLEN14                                /*  配置UART0的接收FIFO触发深度   */

#define UART1FIFOLENFUN  UARTFIFOLEN8                                /*  配置UART1的接收FIFO触发深度   */
#define UART2FIFOLENFUN  UARTFIFOLEN8                                /*  配置UART1的接收FIFO触发深度   */

/*********************************************************************************************************
   配置UART软件FIFO长度,用户可修改
*********************************************************************************************************/  
#define QUEUE0REVICEFIFOLEN      256                                    /*  用户可在此修改接收软FIFO大小*/ 
#define QUEUE0SENDFIFOLEN        256                                    /*  用户可在此修改发送软FIFO大小*/ 
#define QUEUE0REVICEFIFOLENFUN   QUEUE0REVICEFIFOLEN+28                 /*  用户不要修改                */
#define QUEUE0SENDFIFOLENFUN     QUEUE0SENDFIFOLEN+28                   /*  用户不要修改                */
                                                
#define QUEUE1REVICEFIFOLEN      256                                    /*  用户可在此修改接收软FIFO大小*/ 
#define QUEUE1SENDFIFOLEN        256                                    /*  用户可在此修改发送软FIFO大小*/ 
#define QUEUE1REVICEFIFOLENFUN   QUEUE1REVICEFIFOLEN+28                 /*  用户不要修改                */
#define QUEUE1SENDFIFOLENFUN     QUEUE1SENDFIFOLEN+28                   /*  用户不要修改                */
 
#define QUEUE2REVICEFIFOLEN      256                                    /*  用户可在此修改接收软FIFO大小*/ 
#define QUEUE2SENDFIFOLEN        256                                    /*  用户可在此修改发送软FIFO大小*/ 
#define QUEUE2REVICEFIFOLENFUN   QUEUE1REVICEFIFOLEN+28                 /*  用户不要修改                */
#define QUEUE2SENDFIFOLENFUN     QUEUE1SENDFIFOLEN+28                   /*  用户不要修改                */

/*********************************************************************************************************
** Function name:           InitUart
** Descriptions:            串口初始化
** input parameters:        uiId:       子设备号
**                          pcArg:      字符串指针
**                          pRsv:       保留参数
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
** Example:                 char pUart[]="BaudRate=9600,DataBits=8,StopBits=1,Parity=NONE,RtsControl=NONE"; 
**                          InitUart(0,pUart,0); 
*********************************************************************************************************/
INT32S InitUart (INT32U uiId, UART_PARAM *pUartParam, void *pRsv);


/*********************************************************************************************************
** Function name:           SetUartMode
** Descriptions:            设置串口的参数
** input parameters:        uiId      : 子设备号
**                          uiCmd     :设置参数宏:CTMODE--串口模式;BAUDRATE--串口波特率
**                          pcArg     :字符串指针
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
** Example1:                char pUartBps[] = "BaudRate=9600";
**                          SetUartMode( 0, BAUDRATE, pUartBps);  
** Example2:                char pUart[]="DataBits=8,StopBits=1,Parity=NONE,RtsControl=NONE";
**                          SetUartMode( 0, CTMODE, pUart);  
*********************************************************************************************************/
INT32S SetUartMode (INT32U uiId,  INT32U uiCmd,   UART_PARAM *pParam);

/*********************************************************************************************************
** Function name:           uartGetState
** Descriptions:            获取当前串口的参数
** input parameters:        uiId:               子设备号
**                          pcArg:              字符串指针
** Output parameters:       pcArg:              输出字符串参数
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
*********************************************************************************************************/
INT32S GetUartState (INT32U uiId, UART_PARAM *pParam);

/*********************************************************************************************************
** Function name:           GetUartFifoStatus
** Descriptions:            获取Queue队列的状态
** input parameters:        uiId:           子设备号
**                          uiWhichFifo:    TX_FIFO-发送队列;RX_FIFO-接收队列
** Returned value:          发送队列中可操作的空间大小，或接收队列中可用数据个数
*********************************************************************************************************/
INT32S GetUartFifoStatus (INT32U uiId, INT32U uiWhichFifo);

/*********************************************************************************************************
** Function name:           FlushUartFifo
** Descriptions:            清空接收或发送Queue队列
** input parameters:        uiId:        UART子设备号
**                          uiWhichFifo: TX_FIFO-发送队列;RX_FIFO-接收队列
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
*********************************************************************************************************/
INT32S FlushUartFifo (INT32U uiId, INT32U uiWhichFifo);

/*********************************************************************************************************
** Function name:           ReadUart
** Descriptions:            从串口设备读取数据
** input parameters:        uiId:        子设备号
**                          puiBuf:      保存返回数据的字符串指针地址
**                          uiNum:       读取的数据个数
**                          pRsv:        保留参数
** Output parameters:       puiBuf:      读到的数据首地址      
** Returned value:          实际读取的数据个数
*********************************************************************************************************/
INT32S   ReadUart (INT32U uiId, INT8U *puiBuf, INT32U uiNum, void *pRsv);

/*********************************************************************************************************
** Function name:           WriteUart
** Descriptions:            向串口设备写数据
** input parameters:        uiId:       子设备号
**                          puiBuf:     输出数据的字符串指针地址
**                          uiNum:      一次写入的数据个数
**                          pRsv:       保留参数
** Output parameters:       NONE
** Returned value:          实际发送的数据个数
*********************************************************************************************************/
INT32S    WriteUart (INT32U uiId, INT8U *puiBuf, INT32U uiNum,  void *pRsv);

/*********************************************************************************************************
** Function name:           DirectWriteUart
** Descriptions:            向串口设备直接发送数据
** input parameters:        uiId      : 子设备号
**                          uiData    : 输出的数据                            
**                          Rsv     : 保留参数(NULL)
** Output parameters:       NONE
** Returned value:          实际发送的数据个数(1或0);
*********************************************************************************************************/
INT32S  DirectWriteUart (INT32U uiId, INT8U uiData);
INT32S  DirectWriteUartApi (INT32U uiId, INT8U *pBuf, INT16U nLen);

/*********************************************************************************************************
** Function name:           uart0Isr
** Descriptions:            UART0中断入口函数
** input parameters:        NONE
** Output parameters:       NONE
** Returned value:          无
********************************************************************************************************/
 
 
//extern OS_EVENT *poeUart0ReviceSem;                                    /*  接收UART0数据标志          */  
void uart0Isr (void);

/*********************************************************************************************************
** Function name:           uart1Isr
** Descriptions:            UART1中断入口函数
** input parameters:        NONE
** Output parameters:       NONE
** Returned value:          无
*********************************************************************************************************/
//extern OS_EVENT *poeUart1ReviceSem;                                    /*  接收UART1数据标志           */
void uart1Isr (void);
 
#endif                                                                  /*  __UART_H                    */
/*********************************************************************************************************
   END FILE
*********************************************************************************************************/
