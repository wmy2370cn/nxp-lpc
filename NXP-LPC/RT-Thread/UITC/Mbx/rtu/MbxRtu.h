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
**   File: $Id: ,v 1.0 2009/11/15 15:27:16   $
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
#pragma once


#define	MAX_PDU_DATA_LENGTH		 253
#define	MAX_PDU_LENGTH		 	(MAX_PDU_DATA_LENGTH+1)
#define MAX_ADU_LENGTH  		(MAX_PDU_LENGTH+2)

struct MBX_RTU
{
	INT8U  Address;
	INT8U  FuncCode;			//报文特征码	

	INT8U  SubFuncCode;        //子功能码
	INT8U  UartId;              //串口号

	INT8U Broadcast;           //广播报文  
	INT8U Pad;

	//接受
	INT16U ModbusRxCRC;			//ModbusCRC校验

	INT8U  TempBuf[MAX_ADU_LENGTH];
	
	/**接受**/
	INT8U RxBuff[MAX_ADU_LENGTH];	//接受缓冲区
	INT16U RxLen;					//接受长度(PDU+地址的长度)，不含CRC段
	INT16U RxCount;					//接受计数

	/**发送**/
	INT8U TxBuff[MAX_ADU_LENGTH];	//发送缓冲区
	INT16U TxLen;					//发送长度(PDU+地址的长度)，不含CRC段
	INT16U TxCount;					//发送计数

	INT32U RxTotalLen;             //接收到的报文的总长度
	INT32U RxTotoalCnt;            //接收到报文的帧数量

	INT32U TxTotalLen;
	INT32U TxTotoalCnt;

	INT32U RxCrcErr;      //CRC错误的帧数量

	
	INT32U RxTimer;					//接受时间计数器
	INT32U TxTimer;					//发送超时计数器
  
	/** 通讯硬件接口函数 **/
//	RxEnableFunc pfnRxEnable;
//	TxEnableFunc pfnTxEnable;
//	IsTxEnableFunc pfnIsTxEnable;
//	CommSend pfnCommSend;
} ;

typedef struct MBX_RTU CMBExt;

void ModbusMainCycle(CMBExt * pMBX);

void InitMbxRtu( CMBExt *pMBX );
