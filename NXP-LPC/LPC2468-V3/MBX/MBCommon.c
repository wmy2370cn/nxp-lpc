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
#include <string.h>
#include "includes.h"
#include "MODBUS.h"
#include "Func.h"
#include "IoAdapter.h"
#include "uart.h"
#include "MBFunc.h"


typedef enum RECV_FRAME_STATE
{
	RECV_FRAME_ERR = 0,
	RECV_FRAME_BYTE_OK = 1,
	RECV_FRAME_OK = 2
}RECV_FRAME_STATE;
/*********************************************************************************************************
** 函数名称: GetOneFrame
** 函数名称: GetOneFrame
**
** 功能描述： 
**
** 输　入:  MBX * pMbx
** 输　入:  INT8U c
**          
** 输　出:  INT8U  0错误，1继续收，2接收正确并完成
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月12日
** 备  注:   
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U GetOneFrame(MBX * pMbx, INT8U c)
{
	INT16U chkcrc = 0;
 
	if (pMbx == NULL)
		return 0;
	
	//不在发送态，并且当前没有新数据
	if(pMbx->RxCount<MAX_ADU_LENGTH)	
	{	
		//清超时计数
		pMbx->RxTimer=0;
		pMbx->RxBuff[pMbx->RxCount++]=c;
		
		/** 0. 从机地址 **/
		if(pMbx->RxCount==1)
		{
			if(c==pMbx->Address )
			{
				pMbx->ModbusRxCRC=0xFFFF;
				crc16_ex(&c,1,& pMbx->ModbusRxCRC);
				pMbx->Broadcast = FALSE;
			}
			else if (c == BROADCAST_ADDRESS )
			{
				pMbx->Broadcast = TRUE;
				pMbx->ModbusRxCRC=0xFFFF;
				crc16_ex(&c,1,& pMbx->ModbusRxCRC);
			}
			else
			{
				pMbx->RxCount=0;
				pMbx->Broadcast = FALSE;
				pMbx->RxLen = 0;
				return RECV_FRAME_ERR;
			}
		}
		/** 1. 功能码 **/
		else if(pMbx->RxCount==2)
		{
			//支持的功能码检查
 		 	if(c==MB_FUNC_READ_COILS 
				|| c==MB_FUNC_READ_DISCRETE_INPUTS 
				|| c == MB_FUNC_READ_HOLDING_REGISTER 
				|| c == MB_FUNC_READ_INPUT_REGISTER
				|| c == MB_FUNC_WRITE_REGISTER
				|| c == MB_FUNC_WRITE_SINGLE_COIL
				)
			{
				if (pMbx->Broadcast == FALSE)
				{//以上报文不支持广播类型
					pMbx->FuncCode=c;
					/** 查询Query类的报文(含写单个寄存器/线圈)，报文长度一定为6(不包括CRC段) **/
					pMbx->RxLen=6;
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
				}
				else
				{
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}		
  			}
			else if ( c == MBX_FUNC_DEVICE_CTRL )//属于设备控制
			{
				pMbx->FuncCode=c;
				pMbx->RxLen = MBX_PDU_FUNC_DEVICE_CTRL_LEN;
				crc16_ex(&c,1,& pMbx->ModbusRxCRC);
			}
			//其他类型的功能码，在这个时候还无法断定此封报文有多长
			else if ( c == MB_FUNC_WRITE_MULTIPLE_COILS 
				|| c == MB_FUNC_WRITE_MULTIPLE_REGISTERS
				|| c == MB_FUNC_READ_FILE_RECORD    //文件读写
				|| c == MB_FUNC_WRITE_FILE_RECORD
				)
			{
				if (pMbx->Broadcast == FALSE)
				{
					pMbx->FuncCode=c;
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
					pMbx->RxLen = 0;
				}
				else
				{
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}		
			}
			else if ( c== MBX_FUNC_READ_FORCEMARK )
			{//读强制标志
				if (pMbx->Broadcast == FALSE)
				{
					pMbx->FuncCode=c;
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
					pMbx->RxLen = 0;
				}
				else
				{
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}	
			}
			else if ( c== MBX_FUNC_WRITE_FORCEMARK )
			{//写强制标志
				if (pMbx->Broadcast == FALSE)
				{//以上报文不支持广播类型
					pMbx->FuncCode=c;
					/**  报文长度一定为7(不包括CRC段) **/
					pMbx->RxLen=7;
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
				}
				else
				{
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}		
			}
			else if (c == MB_FUNC_READWRITE_MULTIPLE_REGISTERS)
			{
				if (pMbx->Broadcast == FALSE)
				{
					pMbx->FuncCode=c;
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
					pMbx->RxLen = 0;
				}
				else
				{
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}					
			}
			else if ( c == MB_FUNC_MEI )
			{//读设备信息				
				if (pMbx->Broadcast == FALSE)
				{
					pMbx->FuncCode = c;
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
					pMbx->RxLen = 0;
				}
				else
				{
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}		
			}
			else
			{
				pMbx->RxCount=0;
				pMbx->Broadcast = FALSE;
				pMbx->RxLen = 0;
				return RECV_FRAME_ERR;
			} 
		}		 
		else if (pMbx->RxCount>=3  )
		{
			/** CRC校验段 **/
			if( pMbx->RxLen && pMbx->RxCount==(pMbx->RxLen+2))
			{
				chkcrc=MAKEWORD(pMbx->RxBuff[pMbx->RxCount-2], c);
				//	pMbx->ModbusRxCRC == GetCRC16( pMbx->RxBuff,pMbx->RxLen);
				if(pMbx->ModbusRxCRC==chkcrc)
				{//接收到一封正确的通信报文

					pMbx->RxTotalLen += pMbx->RxCount;
					pMbx->RxTotoalCnt ++;

					return RECV_FRAME_OK;
				}
				else
				{
					pMbx->RxCrcErr ++;
					pMbx->RxCount=0;
					return RECV_FRAME_ERR;
				}
			}

			if (pMbx->RxCount <= (pMbx->RxLen))
			{
				crc16_ex(&c,1,& pMbx->ModbusRxCRC);
			}
			
			 //报文长度还是未知
			if ( pMbx->FuncCode == MB_FUNC_WRITE_MULTIPLE_COILS 
				|| pMbx->FuncCode == MB_FUNC_WRITE_MULTIPLE_REGISTERS
				)
			{
				if (pMbx->RxCount==7 && pMbx->Broadcast == FALSE)
				{
					pMbx->RxLen = c +  6;
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
				}
				else
				{
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}	
			}
			else if ( pMbx->FuncCode == MBX_FUNC_READ_FORCEMARK )
			{//读取强制标志
				if (pMbx->RxCount==3 && pMbx->Broadcast == FALSE)
				{
					pMbx->RxLen = c + 3; //不含CRC段
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
				}	
				else if ( pMbx->Broadcast == TRUE)
				{//不支持广播
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}
			}
			else if ( pMbx->FuncCode == MB_FUNC_READ_FILE_RECORD
				|| pMbx->FuncCode == MB_FUNC_WRITE_FILE_RECORD
				)
			{//文件读写，不支持广播类型
				if (pMbx->RxCount==3 && pMbx->Broadcast == FALSE)
				{
					pMbx->RxLen = c + 3; //不含CRC段
					crc16_ex(&c,1,& pMbx->ModbusRxCRC);
				}
				else if ( pMbx->RxCount==4 && pMbx->Broadcast == FALSE )
				{
					if (c == 0x06 || c == 0x07)
					{//目前只有这两个子功能码
						pMbx->SubFuncCode = c;
					//	crc16_ex(&c,1,& pMbx->ModbusRxCRC);
					}
					else
					{
						pMbx->RxCount=0;
						pMbx->Broadcast = FALSE;
						pMbx->RxLen = 0;
						return RECV_FRAME_ERR;
					}				
				}				 
				else if ( pMbx->Broadcast == TRUE)
				{//不支持广播
					pMbx->RxCount=0;
					pMbx->Broadcast = FALSE;
					pMbx->RxLen = 0;
					return RECV_FRAME_ERR;
				}					
			}
			else if ( pMbx->FuncCode == MBX_FUNC_DEVICE_CTRL )
			{//支持广播报文
				if (pMbx->RxCount == 3)
				{					
					if (c == MBX_CTRL_SET_TIME
						|| MBX_CTRL_RESET
						|| MBX_CTRL_CLEAR_FLASH
						|| MBX_CTRL_CLEAR_FRAM)
					{//设备控制类的报文长度固定为13字节
						pMbx->SubFuncCode = c;
						pMbx->RxLen = MBX_PDU_FUNC_DEVICE_CTRL_LEN + MB_SER_PDU_PDU_OFF ;
					}
					else
					{
						pMbx->RxCount=0;
						pMbx->Broadcast = FALSE;
						pMbx->RxLen = 0;
						return RECV_FRAME_ERR;
					}
				}
			}
			else if (  pMbx->FuncCode == MB_FUNC_MEI  )
			{
				if (pMbx->RxCount == 3)
				{
					pMbx->SubFuncCode = c;
					if ( c== 0x0E  )
					{
						pMbx->RxLen = MB_PDU_FUNC_MEI_LEN+ MB_SER_PDU_PDU_OFF;
						crc16_ex(&c,1,& pMbx->ModbusRxCRC);
					}
					else
					{
						pMbx->FuncCode=0;
						pMbx->RxCount=0;
						pMbx->RxLen = 0;
						return RECV_FRAME_ERR;
					}
				}
			}
			else if ( pMbx->FuncCode  == MB_FUNC_READWRITE_MULTIPLE_REGISTERS &&  pMbx->RxCount==10 )
			{
				pMbx->RxLen = c +  10;
			}			 		
		}		
	}
	return RECV_FRAME_BYTE_OK;
}
/*********************************************************************************************************
** 函数名称: RecvData
** 函数名称: RecvData
**
** 功能描述： 
**
** 输　入:  MBX * pMbx
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月9日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U RecvData( MBX * pMbx )
{
	INT8U  Err = 0;
	INT16U nRecvNum = 0;
	INT16U i = 0;

	if (pMbx == NULL)
		return	FALSE; 
	
	OSSemPend(pMbx->pUartRecvSem, 0, &Err);

	//出错了
	if (Err != OS_NO_ERR)
		return FALSE;	

    // 查看收到多少数据  
	nRecvNum = GetUartFifoStatus(pMbx->UartId, RX_FIFO);      
	if (nRecvNum==0 || nRecvNum > MAX_ADU_LENGTH)
	{
		FlushUartFifo(pMbx->UartId, RX_FIFO);
		return FALSE;
	}
	 
	nRecvNum = ReadUart (pMbx->UartId,pMbx->TempBuf , min(nRecvNum,MAX_ADU_LENGTH), NULL);	    
	if (nRecvNum==0 || nRecvNum > MAX_ADU_LENGTH)
		return FALSE;

	while (TRUE)
	{
		Err= GetOneFrame(pMbx, pMbx->TempBuf[i] );
		if (Err == 0)
		{//报文不合要求,继续下一个字节
			i++;
			if (i>=nRecvNum || i>=MAX_ADU_LENGTH)
				return FALSE;
		}
		else if ( Err == 1 )
		{//报文符合要求，但是没有收完，继续收
			i++;
			if (i>=nRecvNum || i>=MAX_ADU_LENGTH)
				return FALSE;
		}
		else if ( Err == 2 )
		{//报文接收完全正确
			pMbx->RxCount=0;

			return TRUE;
		}
		else 
			return FALSE;
	} 
	return FALSE;
}
/*********************************************************************************************************
** 函数名称: PasreMsg
** 函数名称: PasreMsg
**
** 功能描述： 解析报文，并进行相应的处理，最终是在任务里面的循环调用的
**
** 输　入:  MBX * pBuff
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月15日
** 备  注:  此时收到的报文是一封正确的报文
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U PasreMsg(MBX * pMbx)
{
	MBException eException = MB_EX_NONE;
	INT16U chkcrc = 0;

#ifdef _DEBUG
	memset(pMbx->TxBuff,0,sizeof(INT8U)*MAX_ADU_LENGTH);	
#endif // _DEBUG

	//从接收缓冲区复制到发送
	memcpy( pMbx->TxBuff,pMbx->RxBuff,pMbx->RxLen );
	pMbx->TxLen = pMbx->RxLen;	
	pMbx->RxLen = 0;

	switch ( pMbx->FuncCode)
	{
		//线圈部分 
	case MB_FUNC_READ_COILS:
		eException = MBFuncReadCoils( pMbx->TxBuff,&  pMbx->TxLen );
		break;
	case MB_FUNC_WRITE_SINGLE_COIL:
		eException = MBFuncWriteCoil( pMbx->TxBuff,&  pMbx->TxLen );
		break;
	case MB_FUNC_WRITE_MULTIPLE_COILS:
		eException = MBFuncWriteMultipleCoils( pMbx->TxBuff,&  pMbx->TxLen );
		break;
		//开入部分
	case MB_FUNC_READ_DISCRETE_INPUTS:
		eException = MBFuncReadDiscreteInputs (pMbx->TxBuff,&  pMbx->TxLen );
		break;
		//读保持型寄存器 AQ MW
	case MB_FUNC_READ_HOLDING_REGISTER:
		eException = MBFuncReadHoldingRegister(pMbx->TxBuff,& pMbx->TxLen );
		break;
	case MB_FUNC_WRITE_REGISTER:
		eException = MBFuncWriteHoldingRegister(pMbx->TxBuff,& pMbx->TxLen );
		break;
	case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
		eException = MBFuncWriteMultipleHoldingRegister(pMbx->TxBuff,& pMbx->TxLen );
		break;
	case MB_FUNC_READWRITE_MULTIPLE_REGISTERS:
		eException = MBFuncReadWriteMultipleHoldingRegister(pMbx->TxBuff,& pMbx->TxLen );
		break;
		//输入寄存器 AI
	case MB_FUNC_READ_INPUT_REGISTER:
		eException = MBFuncReadInputRegister( pMbx->TxBuff,& pMbx->TxLen );
		break;
	case MB_FUNC_READ_FILE_RECORD:
		eException = MBXReadFileRecord( pMbx->TxBuff,& pMbx->TxLen );
		break;
	case MB_FUNC_WRITE_FILE_RECORD:
		{
			if (pMbx->SubFuncCode == 0x06)
			{
				eException = MBXWriteFileRecord( pMbx->TxBuff,& pMbx->TxLen );
				break;
			}
			else if ( pMbx->SubFuncCode == 0x07 )
			{
				eException = MBXWriteFileToFlash( pMbx->TxBuff,& pMbx->TxLen );
				break;
			}
			break;
		}
	case MBX_FUNC_READ_FORCEMARK:
		eException = MBXFuncReadForceMark( pMbx->TxBuff,& pMbx->TxLen );
		break;
	case MBX_FUNC_WRITE_FORCEMARK:
		eException = MBXFuncWriteForceMark( pMbx->TxBuff,& pMbx->TxLen );
		break;
	case MBX_FUNC_DEVICE_CTRL:
		{
			if (pMbx->SubFuncCode == MBX_CTRL_SET_TIME )
			{
				eException = MBXFuncSetTime( pMbx->TxBuff,& pMbx->TxLen );
				
			}
			else if ( pMbx->SubFuncCode == MBX_CTRL_RESET )
			{

			}
			else
			{

			}

			if (pMbx->TxLen == 0)
			{//如果是无需应答的，则直接返回
				return FALSE;
			}
		}
	case MB_FUNC_MEI:
		{
			if (pMbx->SubFuncCode == 0x0E)
			{
				eException = MBFuncMEI( pMbx->TxBuff,& pMbx->TxLen );
			}
		}


		break;


	default:
		return FALSE;	 
	}

	if( eException != MB_EX_NONE )
	{//如果请求报文有问题
		/* An exception occured. Build an error frame. */
		pMbx->TxLen = 1;
		pMbx->TxBuff[pMbx->TxLen++] = ( INT8U )( pMbx->FuncCode | MB_FUNC_ERROR );
		pMbx->TxBuff[pMbx->TxLen++] = eException;
	}

	//校验CRC
	chkcrc = GetCRC16( pMbx->TxBuff,pMbx->TxLen);
	pMbx->TxBuff[pMbx->TxLen++]=LOBYTE(chkcrc);			
	pMbx->TxBuff[pMbx->TxLen++]=HIBYTE(chkcrc);

	//报文组织完成
	pMbx->TxCount=0;

 	return TRUE; 
}

static void SendResponse(MBX *pMbx)
{
	INT32U temp = 0;

	if (pMbx == NULL)
		return; 

	if( pMbx->TxCount<pMbx->TxLen)
	{
		temp = WriteUart (pMbx->UartId, &pMbx->TxBuff[pMbx->TxCount], pMbx->TxLen-pMbx->TxCount,  NULL);

		pMbx->TxTotalLen += pMbx->TxCount;
		pMbx->TxTotoalCnt ++;

		pMbx->TxCount+=temp;
	}
}
/*********************************************************************************************************
** 函数名称: ModbusMainCycle
** 函数名称: ModbusMainCycle
**
** 功能描述： 由任务调用的主循环
**
** 输　入:  MBX * pBuff
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月15日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern void ModbusMainCycle(MBX * pMBX)
{
	INT8U bRet = FALSE;

// 	if(pMBX->bSend)	//在发送态
// 	{
// 		//检查发送超时
// 		if(pMBX->TxTimer>TX_OVERTIME)
// 		{
// 			//发送已经超时，置接受态
// 			pMBX->bSend=FALSE;
// 			pMBX->TxLen=pMBX->TxCount=0;
// 			pMBX->TxTimer=0;
// 
// 			//允许接受
// 			pMBX->pfnTxEnable();
// 		}
// 	}
// 	else if(pMBX->RxCount>0 && pMBX->RxTimer>RX_OVERTIME)	//在接受态
	
// 	if(pMBX->RxCount>0 && pMBX->RxTimer>RX_OVERTIME)	//在接受态
// 	{//接收超时处理
// 		OS_ENTER_CRITICAL();
// 		pMBX->RxCount=0;
// 		pMBX->RxTimer=0;
// 		OS_EXIT_CRITICAL();
// 	}

	bRet = RecvData(pMBX);
	if (bRet == FALSE)
		return;

	//点LED灯
	SetLed(LED_COMM,TRUE);

	//有新数据要处理
	//数据处理
	bRet = PasreMsg(pMBX);

	OSTimeDlyHMSM(0,0,0,200);

	 //检查是否有数据需要发送
	if(bRet)
	{
		SendResponse(pMBX);
	}
	SetLed(LED_COMM,FALSE); 
}
/*********************************************************************************************************
** 函数名称: InitMBX
** 函数名称: InitMBX
**
** 功能描述： 初始化默认值
**
** 输　入:  MBX * pMBX
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月17日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern void InitMBX( MBX *pMBX )
{
	if (pMBX == NULL)
		return;
//	pMBX->Address = 0;

	memset(pMBX->RxBuff,0,sizeof(INT8U)*MAX_ADU_LENGTH);
	pMBX->RxLen = 0;					//接受长度
	pMBX->RxCount = 0;					//接受计数

	memset(pMBX->TxBuff,0,sizeof(INT8U)*MAX_ADU_LENGTH);

	pMBX->TxLen = 0;					//发送长度
	pMBX->TxCount = 0;					//发送计数
 	
	pMBX->RxTimer = 0;					//接受时间计数器
	pMBX->TxTimer = 0;					//发送超时计数器
 
	pMBX->pUartRecvSem = OSSemCreate(0); //创建信号量                     


 	pMBX->ModbusRxCRC = 0;				//ModbusCRC校验
	pMBX->FuncCode = 0;					//报文特征码	
	pMBX->SubFuncCode = 0;

	pMBX->RxTotalLen = 0;
	pMBX->RxTotoalCnt =0;

	pMBX->TxTotalLen = 0;
	pMBX->TxTotoalCnt = 0;

	pMBX->RxCrcErr = 0;
}
