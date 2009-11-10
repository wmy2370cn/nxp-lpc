/***********************************************Copyright (c)*********************************************
**                                
**                                     
**                                      
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			    queue.c
** Last modified Date:      2007-09-20
** Last Version:		    1.0
** Descriptions:		    中间件函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:			    chengmingji
** Created date:		    2007-07-15
** Version:				    1.0
** Descriptions:		    
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			    lixiaocheng
** Modified Date:		    2007-09-20
** Version:				    1.0
** Descriptions:		    对排版格式进行了调整
**
*********************************************************************************************************/
#include <stdlib.h>
#include <string.h> 

#include "applib.h"
#include "DataQueue.h"

/*********************************************************************************************************
** Function name:           QueueCreate
** Descriptions:            初始化数据队列
** Input parameters:        pDataQ      ：为队列分配的存储空间地址
**                          SizeOfBuf：为队列分配的存储空间大小（字节）
**                          ReadEmpty：为队列读空时处理程序
**                          WriteFull：为队列写满时处理程序
** Output parameters:       NONE
** Returned value:          NOT_OK  :参数错误
**                          QUEUE_OK:成功
*********************************************************************************************************/
INT8U QueueCreate (DataQueue *pDataQ, INT32U  SizeOfBuf,  INT8U   (* ReadEmpty)(), INT8U   (* WriteFull)() )
{	
	OS_CPU_SR  cpu_sr = 0; 

	DataQueue *pQueue = NULL;
	if (pDataQ != NULL && SizeOfBuf >= (sizeof(DataQueue))) 
	{ /* 判断参数是否有效             */
        pQueue = (DataQueue *)pDataQ;

        OS_ENTER_CRITICAL(); 
        /* 初始化结构体数据             */
        pQueue->MaxDataNum = (INT16U)( ( SizeOfBuf - (INT32U)( ((DataQueue *)0)->Buf ) ) /sizeof(QUEUE_DATA_TYPE) );                      /* 计算队列可以存储的数据数目   */
        pQueue->pEnd = pQueue->Buf + pQueue->MaxDataNum;    /* 计算数据缓冲的结束地址       */
        pQueue->pOut = pQueue->Buf;
        pQueue->pIn = pQueue->Buf;
        pQueue->DataCnt = 0;
        pQueue->ReadEmpty = ReadEmpty;
        pQueue->WriteFull = WriteFull;

        OS_EXIT_CRITICAL();
        return QUEUE_OK;
    } 
	else
	{
        return NOT_OK;
    }
}
/*********************************************************************************************************
** Function name:           QueueRead
** Descriptions:            获取队列中的数据
** Input parameters:        pRet:存储返回的消息的地址
**                          Buf:指向队列的指针
** Output parameters:       NONE
** Returned value:          NOT_OK     ：参数错误
**                          QUEUE_OK   ：收到消息
**                          QUEUE_EMPTY：队列空
*********************************************************************************************************/
INT8U QueueRead (QUEUE_DATA_TYPE *pRet, DataQueue *Buf)
{
    OS_CPU_SR  cpu_sr = 0;

    INT8U err = NOT_OK;
    DataQueue *Queue = NULL;
  
    if (Buf != NULL)
	{                                                  /* 队列是否有效                 */
        Queue = (DataQueue *)Buf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->DataCnt > 0) 
		{                                         /* 队列是否为空                 */                                                               
            *pRet = Queue->pOut[0];                 /* 数据出队                     */
            Queue->pOut++;                                               
            
            if (Queue->pOut >= Queue->pEnd)
			{  /* 调整出队指针                 */
                Queue->pOut = Queue->Buf;
            }
            
            Queue->DataCnt--;                                             /* 数据减少                     */
            err = QUEUE_OK;
        } 
		else 
		{                                                               
            err = QUEUE_EMPTY;
            
            if (Queue->ReadEmpty != NULL)
			{                             /* 空，调用用户处理函数         */
                err = Queue->ReadEmpty(pRet, Queue);
            }
        }
        OS_EXIT_CRITICAL();
    }
    return err;
}

/*********************************************************************************************************
** Function name:           QueueWrite
** Descriptions:            FIFO方式发送数据
** Input parameters:        Buf :指向队列的指针
**                          Data:发送的数据
** Output parameters:       NONE
** Returned value:          NOT_OK    : 参数错误
**                          QUEUE_FULL: 队列满
**                          QUEUE_OK  : 发送成功
*********************************************************************************************************/
INT8U QueueWrite (DataQueue *Buf, QUEUE_DATA_TYPE Data)
{
	OS_CPU_SR  cpu_sr = 0;
	INT8U err = NOT_OK;
	DataQueue *Queue = NULL;
		  
    if (Buf != NULL)
	{                                                  /* 队列是否有效                 */
        Queue = (DataQueue *)Buf;
        
        OS_ENTER_CRITICAL(); 
        
        if (Queue->DataCnt < Queue->MaxDataNum) 
		{                            /* 队列是否满                   */
            Queue->pIn[0] = Data;                                        /* 数据入队                     */
            Queue->pIn++;
                                                           
            if (Queue->pIn >= Queue->pEnd)
			{                              /* 调整入队指针                 */
                Queue->pIn = Queue->Buf;
            }
            
            Queue->DataCnt++;                                             /* 数据增加                     */
            err = QUEUE_OK;
        } 
		else 
		{                                                        /* 满                           */
            err = QUEUE_FULL;
            if (Queue->WriteFull != NULL) 
			{                             /* 调用用户处理函数             */
                err = Queue->WriteFull(Queue, Data, Q_WRITE_MODE);
            }
        }
        OS_EXIT_CRITICAL();
    }
    return err;
}

//批量
extern INT8U QueueWriteEx (DataQueue *Buf, QUEUE_DATA_TYPE *pData,INT16U nLen,INT16U *pnRetLen)
{
 	OS_CPU_SR  cpu_sr = 0;
 
	INT8U err = NOT_OK;
	DataQueue *Queue = (DataQueue *)Buf;
	INT16U nQLen = 0;
 
	  /* 队列是否有效                 */
	if (Queue != NULL && pData != NULL && nLen > 0 && nLen <= Queue->MaxDataNum )
	{   
		OS_ENTER_CRITICAL(); 

		if (Queue->DataCnt + nLen < Queue->MaxDataNum) 
		{      /* 队列是否满  */
		 //	Queue->pIn[0] = Data;                                        /* 数据入队                     */
		//	Queue->pIn++;
			memcpy(& Queue->pIn[0] ,pData, nLen );
			Queue->pIn += nLen;

			if (Queue->pIn >= Queue->pEnd)
			{                              /* 调整入队指针                 */
				Queue->pIn = Queue->Buf;
			}

			  /* 数据增加              */
			Queue->DataCnt += nLen;
			*pnRetLen = nLen;
			err = QUEUE_OK;
		} 
		else if ( Queue->DataCnt + nLen >= Queue->MaxDataNum )
		{/* 缓存队列只能存放部分  */
			nQLen = Queue->MaxDataNum - Queue->DataCnt;
			memcpy(& Queue->pIn[0] ,pData, nQLen );
			Queue->pIn += nQLen;

			if (Queue->pIn >= Queue->pEnd)
			{                              /* 调整入队指针                 */
				Queue->pIn = Queue->Buf;
			}

			/* 数据增加              */
			Queue->DataCnt += nQLen;
			*pnRetLen = nQLen;
			err = QUEUE_OK;
		}
		else if (Queue->DataCnt == Queue->MaxDataNum)
	 	{ /* 已经满  */
			*pnRetLen = 0;
			err = QUEUE_FULL;
			if (Queue->WriteFull != NULL) 
			{                             /* 调用用户处理函数             */
				err = Queue->WriteFull(Queue, pData[0], Q_WRITE_MODE);
			}
		}
		OS_EXIT_CRITICAL();
	}
	return err;
}

/*********************************************************************************************************
** Function name:           QueueWriteFront
** Descriptions:            LIFO方式发送数据
** Input parameters:        Buf :指向队列的指针
**                          Data:发送的数据
** Output parameters:       NONE
** Returned value:          NOT_OK    : 参数错误
**                          QUEUE_FULL: 队列满
**                          QUEUE_OK  : 发送成功
*********************************************************************************************************/
INT8U QueueWriteFront (DataQueue *Buf, QUEUE_DATA_TYPE Data)
{
	OS_CPU_SR  cpu_sr = 0;
	INT8U err = NOT_OK;
	DataQueue *Queue = NULL;
	    
    if (Buf != NULL) 
	{                                                  /* 队列是否有效                 */
        Queue = (DataQueue *)Buf;
        
        OS_ENTER_CRITICAL();
        if (Queue->DataCnt < Queue->MaxDataNum)
		{                            /* 队列是否满                   */
            Queue->pOut--; 

			if (Queue->pOut < Queue->Buf) 
			{ /* 调整出队指针                 */
				Queue->pOut = Queue->pEnd - 1;
			}
            
            Queue->pOut[0] = Data;                                       /* 数据入队                     */
            Queue->DataCnt++;                                             /* 数据数目增加                 */
            err = QUEUE_OK;
        }
		else
		{                                                        /* 满                           */
            err = QUEUE_FULL;
            if (Queue->WriteFull != NULL) 
			{                             /* 调用用户处理函数             */
                err = Queue->WriteFull(Queue, Data, Q_WRITE_FRONT_MODE);
            }
        }
        OS_EXIT_CRITICAL();
    }
    return err;
}
/*********************************************************************************************************
** Function name:           GetQueueDataCnt
** Descriptions:            取得队列中数据数
** Input parameters:        Buf :指向队列的指针
** Output parameters:       NONE
** Returned value:          队列包含数据数
*********************************************************************************************************/
INT16U GetQueueDataCnt (DataQueue *Buf)
{
	OS_CPU_SR  cpu_sr = 0;

    INT16U temp = 0;                                                           /* 队列无效返回0                */
    
    if (Buf != NULL)
	{
        OS_ENTER_CRITICAL(); 
        temp = ((DataQueue *)Buf)->DataCnt;
        OS_EXIT_CRITICAL();
    }
    return temp;
} 
/*********************************************************************************************************
** Function name:           GetQueueSize
** Descriptions:            取得队列总容量
** Input parameters:        Buf :指向队列的指针
** Output parameters:       NONE
** Returned value:          队列总容量
*********************************************************************************************************/
INT16U GetQueueSize (DataQueue *Buf)
{
    OS_CPU_SR  cpu_sr = 0;
 
    INT16U temp = 0;        /* 队列无效返回0                */
    
    if (Buf != NULL)
	{
        OS_ENTER_CRITICAL();
        temp = ((DataQueue *)Buf)->MaxDataNum;
        OS_EXIT_CRITICAL();
    }
    return temp;
} 
/*********************************************************************************************************
** Function name:           OSQFlush
** Descriptions:            清空队列
** Input parameters:        Buf :指向队列的指针
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void QueueFlush (DataQueue *Buf)
{
     OS_CPU_SR  cpu_sr = 0;
 
    DataQueue *Queue = NULL;
    
    if (Buf != NULL) 
	{                                                  /* 队列是否有效                 */
        Queue = (DataQueue *)Buf;
        OS_ENTER_CRITICAL();
        Queue->pOut = Queue->Buf;
        Queue->pIn = Queue->Buf;
        Queue->DataCnt = 0;                                               /* 数据数目为0                  */
        OS_EXIT_CRITICAL();
    }
}
                                                    

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
