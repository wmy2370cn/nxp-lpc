/****************************************Copyright (c)**************************************************
**                                              
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
#include <stdlib.h> 
#include "applib.h"
#include "board.h"
#include <rtthread.h>
#include <rthw.h>
#include "uart.h" 
#include <LPC24xx.H>


#ifndef OPERATE_SUCCESS
#define OPERATE_SUCCESS		1         /* 操作成功                     */
#endif

#ifndef OPERATE_FAIL
#define OPERATE_FAIL	   -1          /* 操作失败                     */
#endif

#define     UART0_BASE_ADDR		    0xE000C000
#define     UART1_BASE_ADDR		    0xE0010000
#define     UART2_BASE_ADDR		    0xE0078000

typedef enum RS485_STATE
{
	RS485_RECV  =0,	// 
	RS485_SEND	=1	// 
}RS485_STATE;
 
/*********************************************************************************************************
   定义操作UART器件的结构体，有多个器件就需要声明多个结构体
*********************************************************************************************************/
static UART_INFO    UART0Info;        /*  定义UART0的一个结构体       */
static UART_INFO    UART1Info;
static UART_INFO    UART2Info; 

/*********************************************************************************************************
   下面使用指针数组来保存结构体指针,方便对该驱动的扩展
*********************************************************************************************************/
UART_INFO *  UartInfoTab[UART_MAX_NUM] = {&UART0Info, &UART1Info,&UART2Info };

/*********************************************************************************************************
   下面定义了UART基地址值，如果想增加UART器件可以在该位置添加相应的基地址即可
*********************************************************************************************************/
const INT32U    UartBaseAddrTab[UART_MAX_NUM] = {UART0_BASE_ADDR, UART1_BASE_ADDR,UART2_BASE_ADDR };
                                                         
/*********************************************************************************************************
   下面定义了UART硬件FIFO值，如果想增加UART器件可以在该位置添加相应的值即可
*********************************************************************************************************/
const INT32U    UartFifoTab[UART_MAX_NUM] = {UART0FIFOLENFUN, UART1FIFOLENFUN,UART2FIFOLENFUN };
                                                     
/*********************************************************************************************************
   下面定义了UART软件接收FIFO大小值，如果有想增加UART器件可以在该位置添加相应的值即可
*********************************************************************************************************/
const INT32U    QueueReviceFifoTab[UART_MAX_NUM] = {QUEUE0REVICEFIFOLENFUN, QUEUE1REVICEFIFOLENFUN,QUEUE2REVICEFIFOLENFUN };  

/*********************************************************************************************************
   下面定义了UART软件发送FIFO值，如果有想增加UART器件可以在该位置添加相应的值即可
*********************************************************************************************************/
const INT32U    QueueSendFifoTab[UART_MAX_NUM] = {QUEUE0SENDFIFOLENFUN, QUEUE1SENDFIFOLENFUN, QUEUE2SENDFIFOLENFUN };  
                                                              
/*********************************************************************************************************
   下面定义了UART0的软件FIFO数组
*********************************************************************************************************/ 
static INT8U Uatr0RxBuf[QUEUE0REVICEFIFOLENFUN];
static INT8U Uatr0TxBuf[QUEUE0SENDFIFOLENFUN];    

/*********************************************************************************************************
   下面定义了UART1的软件FIFO数组
*********************************************************************************************************/ 
static INT8U Uatr1RxBuf[QUEUE1REVICEFIFOLENFUN];
static INT8U Uatr1TxBuf[QUEUE1SENDFIFOLENFUN];
/*********************************************************************************************************
   下面定义了UART2的软件FIFO数组
*********************************************************************************************************/ 
static INT8U Uatr2RxBuf[QUEUE2REVICEFIFOLENFUN];
static INT8U Uatr2TxBuf[QUEUE2SENDFIFOLENFUN];
/*********************************************************************************************************
   下面使用指针数组来保存UART软件FIFO数组的首地址
*********************************************************************************************************/ 
const INT8U *UartReviceTab[UART_MAX_NUM] = {Uatr0RxBuf, Uatr1RxBuf, Uatr2RxBuf};
const INT8U *UartSendTab[UART_MAX_NUM] = {Uatr0TxBuf, Uatr1TxBuf, Uatr2TxBuf}; 

/*********************************************************************************************************
** 函数名称: SetRS485Mode
** 函数名称: SetRS485Mode
**
** 功能描述： 设置485模式
**
** 输　入:  INT32U uiId
** 输　入:  INT8U Flag
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月8日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U SetRS485Mode(INT32U uiId, INT8U Flag )
{
	if (uiId == UART1)
	{
// 		if (Flag == RS485_SEND)
// 			FIO3CLR = RW_485;		
// 		else		
// 			FIO3SET = RW_485;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/*********************************************************************************************************
** Function name:			SetUartBandRate
** Descriptions:			初始化UART波特率参数
** input parameters:		pUartInfo:          指向uart信息结构体的指针
**                          uibps:              波特率值
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
*********************************************************************************************************/
static INT32S SetUartBandRate (UART_INFO *pUartInfo, INT32U uibps)
{    
    volatile INT32U  uiOffBase;
	volatile INT32U *puiAddrBase;
        
	puiAddrBase = pUartInfo->puiAddrBase;	                            /*  获取结构体参数              */
	uiOffBase   = pUartInfo->uiOffBase;
	
	if ((uibps <= 115200) && (uibps >= 9600))                           /*  参数过滤                    */
	{
	    puiAddrBase[__B_UART_LCR << uiOffBase] |= 0x80;        		    /*  允许访问分频因子寄存器      */
	    			                                                    /*  设置波特率                  */
	    puiAddrBase[__B_UART_DLM << uiOffBase] = ((Fpclk / uibps) / 16) / 256;   
		puiAddrBase[__B_UART_DLL << uiOffBase] = ((Fpclk / uibps) / 16) % 256;		
 		puiAddrBase[__B_UART_LCR << uiOffBase] &= 0x7f;             	/*  禁止访问分频因子寄存器      */
	    return UART_OK;
	}
    return UART_NOK;
}
 
/*********************************************************************************************************
** Function name:			SetUartMode
** Descriptions:			初始化UART的模式参数
** input parameters:		pUartInfo:          指向uart信息结构体的指针
**                          uiParaData:         参数指针
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
*********************************************************************************************************/
static INT32S SetUartParam (UART_INFO *pUartInfo,  UART_PARAM *pUartParam)
{
    volatile INT32U uiOffBase;
	volatile INT32U *puiAddrBase;
	
    INT32U uiBak;
             
	puiAddrBase = pUartInfo->puiAddrBase;	                            /*  获取结构体参数              */
	uiOffBase   = pUartInfo->uiOffBase;

	/*  参数过滤                    */ 
	if ( pUartParam	)
	{
		if ((pUartParam->datab  <5) || (pUartParam->datab  > 8) )
		{
			 return UART_NOK;
		}
		if (( pUartParam->stopb  == 0) || (pUartParam->stopb  > 2))
		{ 	 
			return UART_NOK;
		}
		if ( pUartParam->parity > 4) 
		{
			return UART_NOK;
		}

		uiBak = pUartParam->datab - 5;
		if (pUartParam->stopb  == 2)
		{                                    /*  判断是否为2位停止位         */
			uiBak |= 0x04;			
		}
		if (pUartParam->parity != 0) 
		{
			pUartParam->parity = pUartParam->parity - 1;
			uiBak |= 0x08;                    					            /*  使能奇偶校验	            */   
		}
		uiBak |= pUartParam->parity << 4;							        /*  设置奇偶校验                */						 
		puiAddrBase[__B_UART_LCR << uiOffBase] = uiBak;           		    /*  禁止访问分频因子寄存器      */
	}
	return UART_OK;
}                                                 
/*********************************************************************************************************
** Function name:			UartCfgInit
** Descriptions:			初始化UART,此函数供UartInit调用,不提供给用户
** input parameters:		pUartInfo:          指向uart信息结构体的指针
**                          uiParaData:         参数指针
**                          uiReBufSize:        uart的接收缓冲区大小
**                          uiSeBufSize:        uart的发送缓冲区大小
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
*********************************************************************************************************/
static INT32U UartCfgInit (UART_INFO *pUartInfo,  UART_PARAM *pUartParam, INT32U uiReBufSize,  INT32U uiSeBufSize)
{
//#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
//    OS_CPU_SR  cpu_sr = 0;
//#endif

    volatile INT32U *puiAddrBase;
	volatile INT32U uiOffBase;
	INT32U uiId;
	
	puiAddrBase = pUartInfo->puiAddrBase;
	uiOffBase   = pUartInfo->uiOffBase;
	uiId = pUartInfo->uiUartId;

// 	if (uiId == UART3)
// 	{//允许485芯片接受
// 		FIO3SET =  1 << 21;	
// 	}
	
//    OS_ENTER_CRITICAL();                                                /*  关中断                     */     	
    if (QueueCreate(pUartInfo->pDQReviceBuf, uiReBufSize, NULL, NULL) == NOT_OK)
	{  /*  创建接收缓冲区             */	                                                                      
    	return UART_NOK;  
    } 			
    if (QueueCreate(pUartInfo->pDQSendBuf, uiSeBufSize, NULL, NULL) == NOT_OK)
	{    /*  创建发送缓冲区             */ 	                                                                    
        return UART_NOK;
    }                                                               
	pUartInfo->uiUartFlag = 0x80;	    /*  7位置位表示不用发送队列缓冲  也就是禁止发送中断         */
										
	  /*  参数过滤         */
	if (pUartParam)
	{
		if (pUartParam->bps < 9600 || pUartParam->bps > 115200)
		{
			pUartParam->bps = 9600;
		}

		if ((pUartParam->datab < 5) || (pUartParam->datab > 8)) 
		{	
			pUartParam->datab  = 8;
		}
		if (( pUartParam->stopb == 0) || ( pUartParam->stopb > 2))
		{ 	
			 pUartParam->stopb  = 1;
		}
		if (  pUartParam->parity  > 4) 
		{
			 pUartParam->parity  = 0;
		}
		if ( pUartParam->rtsctl != 0)
		{	
			pUartParam->rtsctl = 0;
		}

		if (OPERATE_FAIL == (SetUartBandRate(pUartInfo,pUartParam->bps))) 
		{
			return UART_NOK;
		}
		if(OPERATE_FAIL == (SetUartParam(pUartInfo,pUartParam)))
		{
			return UART_NOK;
		}    
		puiAddrBase[__B_UART_FCR << uiOffBase] = ((UartFifoTab[uiId] << 6) | 0x07);       
		/*  初始化FIFO为N个字节触发,WO	发送和接收的FIFO都使能      */                                                                        
		puiAddrBase[__B_UART_IER << uiOffBase] = 0x05;                      /*  使能接收中断,不使能发送中断 */

		pUartInfo->uiUartFlag |= 0x10;                                      /*  4位置位表示发送队列使能     */                       
	}

//    OS_EXIT_CRITICAL();
    
    return UART_OK;
}
void Uart0Isr (int vector);
void Uart1Isr (int vector);
void Uart2Isr (int vector);
/*********************************************************************************************************
** Function name:           InitUart
** Descriptions:            串口初始化及中断设置
** input parameters:        uiId:       子设备号
**                          pcArg:      字符串指针
**                          pRsv:       保留参数
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
** Example:                 char pUart[]="BaudRate=9600,DataBits=8,StopBits=1,Parity=NONE,RtsControl=NONE"; 
**                          InitUart(0,pUart,0); 
*********************************************************************************************************/
INT32S InitUart (INT32U uiId,  UART_PARAM *pUartParam, void *pRsv)
{ 
    if ((uiId<UART_MAX_NUM) )
	{        
        UartInfoTab[uiId]->puiAddrBase = (INT32U *)UartBaseAddrTab[uiId];   /*  定基址                      */
                                                                        
        UartInfoTab[uiId]->uiOffBase = 0;                         /*  地址偏移量                  */
        
        UartInfoTab[uiId]->uiUartFlag = 0;                        /*  清0标志                     */
        
        UartInfoTab[uiId]->uiQueueReviceFifoLen = QueueReviceFifoTab[uiId];    /*  软件FIFO接收长度            */
                                                                        
        UartInfoTab[uiId]->uiQueueSendFifoLen = QueueSendFifoTab[uiId];      /*  软件FIFO发送长度            */
                                                                           
        UartInfoTab[uiId]->pDQReviceBuf = (DataQueue *)UartReviceTab[uiId];         /*  记录接收缓存                */
                                                                        
        UartInfoTab[uiId]->pDQSendBuf = (DataQueue *)UartSendTab[uiId];	       /*  记录发送缓存                */
                                                                        
        UartInfoTab[uiId]->uiUartId =  uiId;                      /*  记录下自己的ID号            */
     
        if (UartCfgInit(UartInfoTab[uiId], pUartParam, QueueReviceFifoTab[uiId], QueueSendFifoTab[uiId]) == OPERATE_FAIL)
		{
            return(UART_NOK);
        }
        else 
		{
			rt_sem_init( & (UartInfoTab[uiId]->RcvDataSem),"UART",0,RT_IPC_FLAG_FIFO);

			if (uiId == 0)
			{
				rt_hw_interrupt_install(UART0_INT, Uart0Isr, RT_NULL);	
				rt_hw_interrupt_umask(UART0_INT);
			}
			else if ( uiId == 1)
			{
				rt_hw_interrupt_install(UART1_INT, Uart1Isr, RT_NULL);	
				rt_hw_interrupt_umask(UART1_INT);
			}
			else if ( uiId == 2)
			{
				rt_hw_interrupt_install(UART2_INT, Uart2Isr, RT_NULL);	
				rt_hw_interrupt_umask(UART2_INT);
			} 
            return(UART_OK);
        }                                                                                                                               
    }
    else
	{
        return(UART_NOK);
    } 
}
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
**                          WdtSetMode( 0, BAUDRATE, pUartBps);  
** Example2:                char pUart[]="DataBits=8 StopBits=1 Parity=NONE RtsControl=NONE";
**                          WdtSetMode( 0, CTMODE, pUart);  
*********************************************************************************************************/
#define BAUDRATE    0   
#define CTMODE      1
INT32S SetUartMode (INT32U uiId, INT32U uiCmd, UART_PARAM *pParam)
{
	if ((uiId<UART_MAX_NUM))
	{
		switch(uiCmd) 
		{
		case BAUDRATE:
			return(SetUartBandRate(UartInfoTab[uiId],pParam->bps)); 
		case CTMODE:
			return(SetUartParam(UartInfoTab[uiId],pParam));					
		default:
			break;	
		}
	}		
	return UART_NOK;     
}
/*********************************************************************************************************
** Function name:			GetUartState
** Descriptions:			从底层串口寄存器获取当前串口的参数
** input parameters:		uiId:               子设备号
**							uiParaData:         字符串指针
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
*********************************************************************************************************/
INT32S GetUartState (INT32U uiId, UART_PARAM *pParam)
{
    volatile INT32U *puiAddrBase;
	volatile INT32U  uiOffBase;

	INT32U uiBak = 0;

	if (uiId < UART_MAX_NUM && pParam)
	{
		puiAddrBase = UartInfoTab[uiId]->puiAddrBase;
		uiOffBase   = UartInfoTab[uiId]->uiOffBase;

		uiBak = puiAddrBase[__B_UART_LCR << uiOffBase];
		puiAddrBase[__B_UART_LCR << uiOffBase] |= 0x80;                     /*  允许访问分频因子寄存器      */

		pParam->bps  = ((Fpclk /(puiAddrBase[__B_UART_DLL << uiOffBase] | (puiAddrBase[__B_UART_DLM << uiOffBase] << 8)) / 16));
		pParam->datab  = (uiBak & 0x03) + 5;
		pParam->stopb  = ((uiBak >> 2) & 0x01) + 1;
		if (( (uiBak >> 3) & 0x01) == 0)
			pParam->parity = 0;
		else
		{
			pParam->parity  = (((uiBak >> 4) & 0x03) + 1);   
		}

		puiAddrBase[__B_UART_LCR << uiOffBase] &= 0x7F;                     /*  禁止访问分频因子寄存器      */
	}
	else
		return UART_NOK;
	return UART_OK;	 					  
}
/*********************************************************************************************************
** Function name:           GetUartFifoStatus
** Descriptions:            获取Queue队列的状态
** input parameters:        uiId:           子设备号
**                          uiWhichFifo:    TX_FIFO-发送队列;RX_FIFO-接收队列
** Returned value:          发送队列中可操作的空间大小，或接收队列中可用数据个数
*********************************************************************************************************/
INT32S GetUartFifoStatus (INT32U uiId,INT32U uiWhichFifo)
{
	UART_INFO *pUartInfo  =NULL;

	if (uiId < UART_MAX_NUM)
	{
		pUartInfo = UartInfoTab[uiId];
		if ((uiWhichFifo == TX_FIFO) && ((pUartInfo->uiUartFlag) < 0x80))
		{
			return (pUartInfo->pDQSendBuf->MaxDataNum - pUartInfo->pDQSendBuf->DataCnt);
		}
		if (uiWhichFifo == RX_FIFO)
		{
			return (pUartInfo->pDQReviceBuf->DataCnt);
		}
		return 0;

	//	return _uartQueueStatus(UartInfoTab[uiId], uiWhichFifo);
	}
	else
		return 0;
}

/*********************************************************************************************************
** Function name:           FlushUartFifo
** Descriptions:            清空接收或发送Queue队列
** input parameters:        uiId:        UART子设备号
**                          uiWhichFifo: TX_FIFO-发送队列;RX_FIFO-接收队列
** Output parameters:       NONE
** Returned value:			OPERATE_SUCCESS:    操作成功
**                          OPERATE_FAIL:       操作失败
*********************************************************************************************************/
INT32S FlushUartFifo (INT32U uiId, INT32U uiWhichFifo)
{
	if (uiId >UART_MAX_NUM) 
	{
		return UART_NOK;
	}
	else if (uiWhichFifo == RX_FIFO) 
	{
		QueueFlush((void *)UartInfoTab[uiId]->pDQReviceBuf);  /*  清空接收队列                */
	}
	else if (uiWhichFifo == TX_FIFO) 
	{	 
		QueueFlush((void *)UartInfoTab[uiId]->pDQSendBuf);/*  清空发送队列                */
	}
	else 
	{ 
		return UART_NOK;
	}
	return UART_OK;
}
/*********************************************************************************************************
** Function name:			GetUartCh
** Descriptions:			无等待接收一个字节
** input parameters:		pUartInfo      : 指向uart信息结构体的指针
**                          uiRet       : 存储接收到的数据
** Returned value:			UART_OK   : 成功
**                          UART_EMPTY: 无数据
** Created by:				Chenmingji
** Created Date:			2006-09-08
*********************************************************************************************************/
static INT8U GetUartCh ( UART_INFO *pUartInfo, INT8U *uiRet)
{
    OS_CPU_SR  cpu_sr = 0;

    INT32U uiErr;

//    OS_ENTER_CRITICAL();
    
    uiErr = QueueRead(uiRet, pUartInfo->pDQReviceBuf);
    
//    OS_EXIT_CRITICAL();
    return uiErr;
} 
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
extern INT32S   ReadUart (INT32U uiId,   INT8U *puiBuf,  INT32U uiNum, void *pRsv)
{
    OS_CPU_SR  cpu_sr = 0;

	INT32U i;
	INT32U uiReviceNum = 0;	
	if (uiId < UART_MAX_NUM) 
	{
//		OS_ENTER_CRITICAL(); 
		
		for (i=0;i<uiNum;i++) 
		{			
			if (QUEUE_OK == ( GetUartCh(UartInfoTab[uiId],puiBuf++))) 
			{
				uiReviceNum ++;
			}
			else
				break;
		}
		
//	    OS_EXIT_CRITICAL();
	} 
	return uiReviceNum;	
}
/*********************************************************************************************************
** Function name:			__uartWrite
** Descriptions:			发送多个字节数据
** input parameters:		pUartInfo:      指向uart信息结构体的指针
**                          puiData:        要发送的数据的首地址
**                          uiNumByte:      发送数据的个数
** Output parameters:       NONE
** Returned value:			成功发送数据的个数
*********************************************************************************************************/
static INT16U __uartWrite (UART_INFO *pUartInfo, INT8U *puiData, INT32U uiNumByte)
{
    OS_CPU_SR  cpu_sr = 0;

    volatile INT32U *puiAddrBase = NULL;
	volatile INT32U  uiOffBase;
	
    INT16U uiSendNumbyte;	    
    INT8U uiTemp;
    
    uiSendNumbyte = uiNumByte;
    pUartInfo->uiUartFlag &= (~0x80);                                   /*  使用队列做为缓冲            */
    if((pUartInfo->uiUartFlag) < 0x80) 
	{ 	
// 		while (uiSendNumbyte > 0)
// 		{
// 			if (QueueWrite((void *)pUartInfo->pDQSendBuf, *puiData++) == QUEUE_FULL)
// 			{
// 				break;
// 			}
// 			uiSendNumbyte--;
// 		}
		if (QueueWriteEx( pUartInfo->pDQSendBuf,  puiData,uiNumByte,&uiSendNumbyte ) == QUEUE_FULL)
		{//队列满,没有发送成功
			return (uiNumByte - uiSendNumbyte);
		//	break;
		}
	//	uiSendNumbyte--;

		OS_ENTER_CRITICAL(); 

		pUartInfo->uiUartFlag |= 0x10;                                  /*  使能发送队列                */
		if (pUartInfo->uiUartFlag > 0x0f) 
		{
			puiAddrBase = pUartInfo->puiAddrBase;
			uiOffBase = pUartInfo->uiOffBase;

			if ((puiAddrBase[__B_UART_IER << uiOffBase] & 0x02) == 0)
			{				
				if (QUEUE_OK == QueueRead(&uiTemp, pUartInfo->pDQSendBuf))
				{	
					 /*  开发送中断                  */
					puiAddrBase[__B_UART_IER << uiOffBase] |= 0x02;    
					SetRS485Mode(pUartInfo->uiUartId,RS485_SEND);	
					 
					puiAddrBase[__B_UART_THR << uiOffBase] = uiTemp; 
				}		 	
			}
		}	
		OS_EXIT_CRITICAL();	 
	}	
    return (uiNumByte - uiSendNumbyte);
} 
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
INT32S  WriteUart (INT32U uiId,   INT8U *puiBuf, INT32U uiNum,  void *pRsv)
{
	if (uiId < UART_MAX_NUM)
	{
		return (__uartWrite(UartInfoTab[uiId], puiBuf, uiNum));
	}		
	else
		return 0;
}
/*********************************************************************************************************
** Function name:           DirectWriteUart
** Descriptions:            向串口设备直接发送数据
** input parameters:        uiId      : 子设备号
**                          uiData    : 输出的数据                            
** Output parameters:       NONE
** Returned value:          实际发送的数据个数(1或0);
*********************************************************************************************************/
INT32S   DirectWriteUart (INT32U uiId, INT8U uiData)
{
    OS_CPU_SR  cpu_sr = 0;

	volatile INT32U *puiAddrBase;
	volatile INT32U  uiOffBase;
	UART_INFO  *pUartInfo = NULL;

	if (uiId >=UART_MAX_NUM) 
		return 0;

	pUartInfo = UartInfoTab[uiId];

	puiAddrBase = pUartInfo->puiAddrBase;
	uiOffBase   = pUartInfo->uiOffBase;
	
	OS_ENTER_CRITICAL(); 

	puiAddrBase[__B_UART_LCR << uiOffBase] &= (~0x80);                  /* DLAB = 0                     */
	
	puiAddrBase[__B_UART_THR << uiOffBase] = uiData;
	//  等待发送完
	//如果是232只需要检查发送FIFO为空
	while ((puiAddrBase[__B_UART_LSR << uiOffBase] & 0x20) == 0);     
	//如果是485 需要检查发送器为空
//	while ((puiAddrBase[__B_UART_LSR << uiOffBase] & 0x40) == 0);     

	OS_EXIT_CRITICAL();
	return UART_OK;
}

INT32S  DirectWriteUartApi (INT32U uiId, INT8U *pBuf, INT16U nLen)
{
	INT16U k;
	volatile INT32U *puiAddrBase;
	volatile INT32U  uiOffBase;
	UART_INFO  *pUartInfo = NULL;

	if (uiId >=UART_MAX_NUM) 
		return 0;

	pUartInfo = UartInfoTab[uiId];

	puiAddrBase = pUartInfo->puiAddrBase;
	uiOffBase   = pUartInfo->uiOffBase;
	//禁止接收中断
	puiAddrBase[__B_UART_IER << uiOffBase] = 0;    

	SetRS485Mode(uiId,RS485_SEND);	
	 
	// DLAB = 0                    
	puiAddrBase[__B_UART_LCR << uiOffBase] &= (~0x80);    
	
	for(k=0;k<nLen;k++)
	{
		puiAddrBase[__B_UART_THR << uiOffBase] = pBuf[k];
		// 等待发送保持THR为空
		while ((puiAddrBase[__B_UART_LSR << uiOffBase] & 0x20) == 0);    
	}
	
	while ((puiAddrBase[__B_UART_LSR << uiOffBase] & 0x40) == 0);     
	
	SetRS485Mode(uiId,RS485_RECV);	

	// 接收使能
	puiAddrBase[__B_UART_IER << uiOffBase] = 1;    
 	return nLen;
}
/*********************************************************************************************************
** Function name:			__uartIsr
** Descriptions:			UART中断服务程序
** input parameters:		pUartInfo:      指向uart信息结构体的指针
** Output parameters:       NONE
** Returned value:			无
*********************************************************************************************************/
static void __uartNone (UART_INFO *pUartInfo, volatile INT32U *puiAddrBase, INT32U  uiOffBase)
{
    pUartInfo = pUartInfo;
    puiAddrBase = puiAddrBase;
    uiOffBase = uiOffBase;
}

/*  发送中断                   */
static void __uartThre (UART_INFO *pUartInfo, volatile INT32U *puiAddrBase, INT32U  uiOffBase)
{
    INT16U uiNum;
    DataQueue *dqSend;
    INT8U *pucOut, *pucEnd;
    
    pUartInfo-> uiUartState = 1;                                        /*  发状态                     */

    if (pUartInfo->uiUartFlag < 0x10)
	{
        puiAddrBase[__B_UART_IER << uiOffBase]  &= ~0x02;
        return;
    }
    
    dqSend = pUartInfo->pDQSendBuf;
    uiNum = pUartInfo->uiQueueSendFifoLen;
    
    if ( uiNum > dqSend->DataCnt) 
	{
         uiNum = dqSend->DataCnt;
    }
    
    if (uiNum == 0) 
	{
        puiAddrBase[__B_UART_IER << uiOffBase]  &= ~0x02;
        return;
    }

    dqSend->DataCnt -= uiNum;

    pucOut = dqSend->pOut;
    pucEnd = dqSend->pEnd;
    do 
	{
        puiAddrBase[__B_UART_THR << uiOffBase] = *pucOut++;

        while((puiAddrBase[__B_UART_LSR << uiOffBase]&0x40)==0)
			;        /*  数据是否发出去              */
        if (pucOut >= pucEnd) 
		{
            pucOut = dqSend->Buf;
        }
    } while (--uiNum != 0);
    dqSend->pOut = pucOut;  
	 
	SetRS485Mode( pUartInfo->uiUartId,RS485_RECV );
//	UART3_RxEnable();			// 接收使能
}
/*  接收中断                    */
static void __uartRevice(UART_INFO *pUartInfo, volatile INT32U *puiAddrBase, INT32U  uiOffBase)
{
    INT8U ucData; 
    DataQueue *pdqRevice;
    INT16U uiNum, uiMaxData;
    INT8U *pucIn, *pucEnd;

    pUartInfo-> uiUartState = 0;                                         /*  收状态                     */
    
    pdqRevice = pUartInfo->pDQReviceBuf;
    
    pucIn = pdqRevice->pIn;
    pucEnd = pdqRevice->pEnd;
    uiNum = pdqRevice->DataCnt;
    uiMaxData = pdqRevice->MaxDataNum;
    do
    {
        ucData = puiAddrBase[__B_UART_RBR << uiOffBase];
        if (uiNum < uiMaxData) 
		{
            *pucIn++ = ucData;
            if (pucIn >= pucEnd) 
			{
                pucIn = pdqRevice->Buf;
            }
            uiNum++;
        }
    } while ((puiAddrBase[__B_UART_LSR << uiOffBase] & 0x00000001) != 0);

    pdqRevice->pIn = pucIn;
    pdqRevice->DataCnt = uiNum; 
}

/* 接收线状态                  */
static void __uartLine(UART_INFO *pUartInfo, volatile INT32U *puiAddrBase, INT32U  uiOffBase)
{
    INT8U uiState; 

    uiState = puiAddrBase[__B_UART_LSR << uiOffBase];
}

static void  (* const arrpfnUartTiom[16])(UART_INFO *pUartInfo,  volatile INT32U *puiAddrBase, 
                                         INT32U  uiOffBase) =
      {__uartNone,   __uartNone, __uartThre, __uartNone,  
       __uartRevice, __uartNone, __uartLine, __uartNone,
       __uartNone,   __uartNone, __uartNone, __uartNone,
       __uartRevice, __uartNone, __uartNone, __uartNone};
 
/*********************************************************************************************************
** Function name:           UartISR
** Descriptions:            UART中断服务程序
** input parameters:        uiId      : 子设备号
** Output parameters:       NONE
** Returned value:          无
*********************************************************************************************************/
static __inline void UartISR (INT32U uiId)
{
	volatile INT32U *puiAddrBase;
	volatile INT32U  uiOffBase;

	INT8U uiState;
	UART_INFO *pUartInfo = NULL;

	if (uiId < UART_MAX_NUM) 
	{
		pUartInfo = UartInfoTab[uiId];
		puiAddrBase = pUartInfo->puiAddrBase;
		uiOffBase   = pUartInfo->uiOffBase;

		uiState = puiAddrBase[__B_UART_IIR << uiOffBase] & 0x0f;
		arrpfnUartTiom[uiState](pUartInfo, puiAddrBase, uiOffBase);   
    }
}

static __inline void Uart0Isr (int vector)
{
	UartISR(UART0);                                                     /*  UART中断管理函数           */                                                               

	if (UartInfoTab[UART0]->uiUartState == 0)
	{   /*  处于接收状态               */
		rt_sem_release( & (UartInfoTab[UART0]->RcvDataSem) );
	}                                                              

	VICVectAddr = 0x00;                                                 /*  中断处理结束               */
} 
static __inline void Uart1Isr (int vector)
{
	UartISR(UART1); 

	if (UartInfoTab[UART1]->uiUartState == 0)
	{ /*  处于接收状态                */
		rt_sem_release( & (UartInfoTab[UART1]->RcvDataSem) );
 	}    

	VICVectAddr = 0x00;                                                 /*  中断处理结束                */
}
static __inline void Uart2Isr (int vector)
{
	UartISR(UART2); 

	if (UartInfoTab[UART2]->uiUartState == 0)
	{                   /*  处于接收状态                */
		rt_sem_release( & (UartInfoTab[UART2]->RcvDataSem) );
	}
	VICVectAddr = 0x00;                                                 /*  中断处理结束                */
}
 
INT8U UartRecvDataPend(INT32U nUartId,INT32U nTick)
{
	if (  rt_sem_take(& (UartInfoTab[UART1]->RcvDataSem),nTick ) == RT_EOK )
	{
		return TRUE;
	}
	return FALSE;
}


/*********************************************************************************************************
   END FILE
*********************************************************************************************************/


