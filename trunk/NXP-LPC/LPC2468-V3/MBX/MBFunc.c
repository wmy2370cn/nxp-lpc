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
#include "MemData.h"
#include "MODBUS.h"
#include "MBFunc.h"
#include "FlashApiWrapper.h"
#include "ram.h"
#include "hd_drive.h"
#include "ClockMgr.h"
 

/*!
* Constants which defines the format of a modbus frame. The example is
* shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
* dependent on the underlying transport.
*
* <code>
* <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
*              <----------- MODBUS PDU (1') ---------------->
*  +-----------+---------------+----------------------------+-------------+
*  | Address   | Function Code | Data                       | CRC/LRC     |
*  +-----------+---------------+----------------------------+-------------+
*  |           |               |                                   |
* (2)        (3/2')           (3')                                (4)
*
* (1)  ... MB_SER_PDU_SIZE_MAX = 256
* (2)  ... MB_SER_PDU_ADDR_OFF = 0
* (3)  ... MB_SER_PDU_PDU_OFF  = 1
* (4)  ... MB_SER_PDU_SIZE_CRC = 2
*
* (1') ... MB_PDU_SIZE_MAX     = 253
* (2') ... MB_PDU_FUNC_OFF     = 0
* (3') ... MB_PDU_DATA_OFF     = 1
* </code>
*/

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_SIZE_MAX     253 /*!< Maximum size of a PDU. */
#define MB_PDU_SIZE_MIN     1   /*!< Function Code */
#define MB_PDU_FUNC_OFF     0   /*!< Offset of function code in PDU. */
#define MB_PDU_DATA_OFF     1   /*!< Offset for response data in PDU. */

//PDU的报文定义
/* -----------------------Coil Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_READ_COILCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_COILCNT_MAX        ( 0x07D0 )

#define MB_PDU_FUNC_WRITE_ADDR_OFF          ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_VALUE_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE              ( 4 )

#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF      ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF   ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF    ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN      ( 5 )
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX   ( 0x07B0 )

//#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF        ( MB_PDU_DATA_OFF + 5 )
//#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN          ( 5 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX        ( 0x0078 )

/* -----------------------InputReg Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_READ_REGCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_REGCNT_MAX         ( 0x007D )

#define MB_PDU_FUNC_READ_RSP_BYTECNT_OFF    ( MB_PDU_DATA_OFF )

/* ----------------------- DiscreteInputs Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_READ_DISCCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_DISCCNT_MAX        ( 0x07D0 )

#define MB_PDU_FUNC_READWRITE_READ_ADDR_OFF     ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  ( MB_PDU_DATA_OFF + 6 )
#define MB_PDU_FUNC_READWRITE_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 8 )
#define MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  ( MB_PDU_DATA_OFF + 9 )
#define MB_PDU_FUNC_READWRITE_SIZE_MIN          ( 9 )


#define MBX_PDU_FUNC_DN_FILE_SIZE_MIN          ( 8 )
#define MBX_PDU_FUNC_UP_FILE_SIZE_MIN          ( 9 )


#define MBX_PDU_FUNC_READ_FM_SIZE              ( 7 )
#define MBX_PDU_WRITE_READ_FM_SIZE              ( 6 )


/*********************************************************************************************************
** 函数名称: IsValidFuncCode
** 函数名称: IsValidFuncCode
**
** 功能描述： 是否为有效的功能码
**
** 输　入:  INT8U Code
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月16日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U IsValidFuncCode( INT8U Code  )
{
	if (Code == MB_FUNC_NONE || Code >=MB_FUNC_ERROR )
		return FALSE;

	switch ( Code )
	{
	case MB_FUNC_READ_COILS://01号，读开出
		return TRUE;
	case MB_FUNC_READ_DISCRETE_INPUTS://02号，读开入
		return TRUE;
	case MB_FUNC_READ_HOLDING_REGISTER://03号，读寄存器
		return TRUE;
	case MB_FUNC_READ_INPUT_REGISTER: // 读输入寄存器
		return TRUE;
	case MB_FUNC_WRITE_SINGLE_COIL:   // 写单个线圈
		return TRUE;
	case MB_FUNC_WRITE_REGISTER:   // 写单个寄存器
		return TRUE;
	case MB_FUNC_DIAG_READ_EXCEPTION:   // 读自检异常
		return TRUE;
	case MB_FUNC_DIAG_DIAGNOSTIC:   // 0X8
		return TRUE;
	case MB_FUNC_DIAG_GET_COM_EVENT_CNT:   // 11
		return TRUE;
	case MB_FUNC_DIAG_GET_COM_EVENT_LOG:   // 12
		return TRUE;
	case MB_FUNC_WRITE_MULTIPLE_COILS:   // 写多个线圈
		return TRUE;
	case MB_FUNC_WRITE_MULTIPLE_REGISTERS:   // 写多个寄存器
		return TRUE;
	case MB_FUNC_OTHER_REPORT_SLAVEID:   // 17
		return TRUE;
	case MB_FUNC_READWRITE_MULTIPLE_REGISTERS:   // 23
		return TRUE;
	default:
		return FALSE; 	 
	}

	return FALSE;
}   
     
static MBException MBError2Exception( MBErrorCode eErrorCode )
{
	MBException    eStatus;

	switch ( eErrorCode )
	{
	case MB_ENOERR:
		eStatus = MB_EX_NONE;
		break;
	case MB_ENOREG:
		eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
		break;
	case MB_ETIMEDOUT:
		eStatus = MB_EX_SLAVE_BUSY;
		break;
	default:
		eStatus = MB_EX_SLAVE_DEVICE_FAILURE;
		break;
	}

	return eStatus;
}

/*********************************************************************************************************
** 函数名称: MBRegCoilsCB
** 函数名称: MBRegCoilsCB
**
** 功能描述： 读取DO，MB等类型的寄存器
**
** 输　入:  INT8U * pucRegBuffer
** 输　入:  INT16U usAddress
** 输　入:  INT16U usNCoils
** 输　入:  MBRegisterMode eMode
**          
** 输　出:  MBErrorCode
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
static MBErrorCode MBRegCoilsCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNCoils, MbxRWMode eMode )
{
	MBErrorCode  Status = MB_ENOERR;
	INT8U offset;
	INT16U i=0 ;
	INT8U  *pByte = pucRegBuffer;
	INT16U         RegIndex = 0;

	if ( (usAddress >= Q_START_ADDR) && (usAddress + usNCoils <= Q_START_ADDR+MAX_Q_CNT ) )
	{
		if (eMode == MBX_READ)
		{
			while(i<usNCoils)
			{
				*pByte = 0;
				for(offset=0;offset<8;offset++,i++)		// 将Q值写入一个字节
				{
					if(i<usNCoils)
					{
						RegIndex = usAddress - Q_START_ADDR + i;
						*pByte |= GetQVal(RegIndex+i+1)<<offset;
					}
				}
				pByte++;
			}
		}
		else if ( eMode == MBX_WRITE )
		{//写其实也只是单个线圈的
			while(i<usNCoils)
			{
				*pByte = 0;
				for(offset=0;offset<8;offset++,i++)		// 将Q值写入一个字节
				{
					if(i<usNCoils)
					{
						RegIndex = usAddress - Q_START_ADDR + i;
						*pByte |= GetQVal(RegIndex+i+1)<<offset;
					}
				}
				pByte++;
			}
		}
	}
	else
	{
		Status = MB_ENOREG;
	}
	return Status;
}
/*********************************************************************************************************
** 函数名称: MBRegDiscreteCB
** 函数名称: MBRegDiscreteCB
**
** 功能描述： 读取Di
**
** 输　入:  INT8U * pucRegBuffer
** 输　入:  INT16U usAddress
** 输　入:  INT16U usNDiscrete
**          
** 输　出:  MBErrorCode
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
static MBErrorCode MBRegDiscreteCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNDiscrete )
{
	MBErrorCode  Status = MB_ENOERR;
	INT8U offset;
	INT16U i=0 ;
	INT8U  *pByte = pucRegBuffer;
	INT16U  RegIndex = 0;
 
	if ( (usAddress >= I_START_ADDR) && (usAddress + usNDiscrete <= I_START_ADDR+MAX_I_CNT ) )
	{
		while(i<usNDiscrete)
		{
			*pByte = 0;
			for(offset=0;offset<8;offset++,i++)		// 将DI值写入一个字节
			{
				if(i<usNDiscrete)
				{
					RegIndex = usAddress - I_START_ADDR + i;
					*pByte |= GetIVal(RegIndex+1)<<offset;
				}
			}
			pByte++;
		}
	}
	return Status;
}
/*********************************************************************************************************
** 函数名称: MBRegInputCB
** 函数名称: MBRegInputCB
**
** 功能描述： 从内存中读取输入寄存器AI的测值
**
** 输　入:  INT8U * pucRegBuffer
** 输　入:  INT16U usAddress
** 输　入:  INT16U usNRegs
**          
** 输　出:  MBErrorCode
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月16日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static MBErrorCode MBRegInputCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNRegs )
{
	MBErrorCode    eStatus = MB_ENOERR;
	INT16U         RegIndex = 0;
	INT16U         Value = 0 ;

	if ( (usAddress >= AI_START_ADDR) && (usAddress + usNRegs <= AI_START_ADDR+MAX_AI_CNT ) )
	{
		//iRegIndex从0开始
		RegIndex = ( INT16U )( usAddress - AI_START_ADDR );
		while( usNRegs > 0 )
		{
			Value = GetAIVal(RegIndex+1);

			*pucRegBuffer++ = ( INT8U )( Value >> 8 );
			*pucRegBuffer++ = ( INT8U )( Value & 0xFF );
			RegIndex++;
			usNRegs--;
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBRegHoldingCB
** 函数名称: MBRegHoldingCB
**
** 功能描述： 保持型寄存器MW AQ的数据读写操作接口
**
** 输　入:  INT8U * pucRegBuffer
** 输　入:  INT16U usAddress
** 输　入:  INT16U usNRegs
** 输　入:  MBRegisterMode eMode
**          
** 输　出:  MBErrorCode
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
static MBErrorCode MBRegHoldingCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNRegs, MbxRWMode eMode )
{
	MBErrorCode    eStatus = MB_ENOERR;
	INT32U         iRegIndex = 0;
	INT16U         Value = 0;

	if ( (usAddress >= MW_START_ADDR) && (usAddress + usNRegs <= MW_START_ADDR + MAX_MW_CNT) )
	{
		iRegIndex = ( INT32U )( usAddress - MW_START_ADDR );
		if (eMode == MBX_READ)
		{
			while( usNRegs > 0 )
			{
				Value = GetMWVal(iRegIndex+1);
				*pucRegBuffer++ = ( INT8U ) ( Value >> 8 );
				*pucRegBuffer++ = ( INT8U ) ( Value & 0xFF );
				iRegIndex++;
				usNRegs--;
			}
		}
		else if ( eMode == MBX_WRITE )
		{
			while( usNRegs > 0 )
			{
				SetMWVal(iRegIndex+1,Value);
				Value = GetMWVal(iRegIndex+1);
				*pucRegBuffer++ = ( INT8U ) ( Value >> 8 );
				*pucRegBuffer++ = ( INT8U ) ( Value & 0xFF );
				iRegIndex++;
				usNRegs--;
			}
		}
	}
	else if ( (usAddress >= AQ_START_ADDR) && (usAddress + usNRegs <= AQ_START_ADDR + MAX_AQ_CNT) )
	{
		iRegIndex = ( INT32U )( usAddress - AQ_START_ADDR );
		if (eMode == MBX_READ)
		{
			while( usNRegs > 0 )
			{
				Value = GetAQVal(iRegIndex+1);
				*pucRegBuffer++ = ( INT8U ) ( Value >> 8 );
				*pucRegBuffer++ = ( INT8U ) ( Value & 0xFF );
				iRegIndex++;
				usNRegs--;
			}
		}
		else if ( eMode == MBX_WRITE )
		{
			while( usNRegs > 0 )
			{
				SetAQVal(iRegIndex+1,Value);
				Value = GetAQVal(iRegIndex+1);
				*pucRegBuffer++ = ( INT8U ) ( Value >> 8 );
				*pucRegBuffer++ = ( INT8U ) ( Value & 0xFF );
				iRegIndex++;
				usNRegs--;
			}
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBXReadForceMarkCB
** 函数名称: MBXReadForceMarkCB
**
** 功能描述： 读通道的强制标志
**
** 输　入:  INT8U * pucRegBuffer
** 输　入:  INT8U RegType
** 输　入:  INT16U usStart
** 输　入:  INT16U usCnt
**          
** 输　出:  MBErrorCode
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年11月24日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static MBErrorCode MBXReadForceMarkCB( INT8U * pucRegBuffer,INT8U RegType, INT16U usStart, INT16U usCnt )
{
	MBErrorCode  Status = MB_ENOERR;
	INT8U offset;
 	INT8U  *pByte = pucRegBuffer;
	INT16U  RegIndex = 0;
	INT16U  i = 0;

	if (RegType == I_TYPE)
	{
		if (usStart + usCnt <= (MAX_I_CNT ))
		{
			i = 0;
			while(i<usCnt)
			{
				*pByte = 0;
				for(offset=0;offset<8;offset++,i++)		// 将 值写入一个字节
				{
					RegIndex = usStart + i;
					*pByte |= GetIForceMark(RegIndex+1)<<offset;
				}
				pByte++;
			}		
		}
		else
		{
			Status = MB_ENOREG;
		}
		return Status;
	}
	else if (RegType == Q_TYPE )
	{
		if (usStart + usCnt <= (MAX_Q_CNT ))
		{
			i = 0;
			while(i<usCnt)
			{
				*pByte = 0;
				for(offset=0;offset<8;offset++,i++)		// 将 值写入一个字节
				{				 
					RegIndex = usStart + i;
					*pByte |= GetQForceMark(RegIndex+1)<<offset;
				}
				pByte++;
			}		
		}
		else
		{
			Status = MB_ENOREG;
		}
		return Status;
	}
	else if (RegType == AQ_TYPE )
	{
		if (usStart + usCnt <= (MAX_AQ_CNT ))
		{
			i = 0;
			while(i<usCnt)
			{
				*pByte = 0;
				for(offset=0;offset<8;offset++,i++)		// 将 值写入一个字节
				{ 
					RegIndex = usStart + i;
					*pByte |= GetAQForceMark(RegIndex+1)<<offset;					 
				}
				pByte++;
			}		
		}
		else
		{
			Status = MB_ENOREG;
		}
		return Status;
	}
	else if (RegType == AI_TYPE )
	{
		if (usStart + usCnt <= (MAX_AI_CNT ))
		{
			i = 0;
			while(i<usCnt)
			{
				*pByte = 0;
				for(offset=0;offset<8;offset++,i++)		// 将 值写入一个字节
				{
					RegIndex = usStart + i;
					*pByte |= GetAIForceMark(RegIndex+1)<<offset;					 
				}
				pByte++;
			}		
		}
		else
		{
			Status = MB_ENOREG;
		}
		return Status;
	}
	return Status;
}

static MBErrorCode MBXWriteForceMarkCB( INT8U RegType, INT16U usAddr, INT16U Val )
{
	MBErrorCode  Status = MB_ENOERR;
	if (RegType == I_TYPE)
	{
		if (usAddr <= (MAX_I_CNT ))
		{
			SetIForceMark(usAddr+1,Val);
		}
		else
		{
			Status = MB_ENOREG;
		}
	}
	else if ( RegType == Q_TYPE )
	{
		if (usAddr <= (MAX_Q_CNT ))
		{
			SetQForceMark(usAddr+1,Val);
		}
		else
		{
			Status = MB_ENOREG;
		}
	}
	else if (RegType == AI_TYPE)
	{
		if (usAddr <= (MAX_AI_CNT ))
		{
			SetAIForceMark(usAddr+1,Val);
		}
		else
		{
			Status = MB_ENOREG;
		}
	}
	else if (RegType == AQ_TYPE)
	{
		if (usAddr <= (MAX_AQ_CNT ))
		{
			SetAQForceMark(usAddr+1,Val);
		}
		else
		{
			Status = MB_ENOREG;
		}
	}
	return Status;
}
/*********************************************************************************************************
** 函数名称: MBXWriteFileCB
** 函数名称: MBXWriteFileCB
**
** 功能描述： 写文件
**
** 输　入:  INT8U * pFileData
** 输　入:  INT8U * pLen
** 输　入:  INT16U nPacketIdx
** 输　入:  INT8U FileType
**          
** 输　出:  MBErrorCode
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月24日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static MBException MBXWriteFileCB( INT8U * pFileData, INT8U nLen,INT16U nPacketIdx, INT8U FileType )
{
	MBException  eStatus = MB_ENOERR;
	INT8U bRet = FALSE;

	if ( FileType == FCB_DATA_TYPE )
	{
		bRet =  PutFcbData(pFileData,nLen,nPacketIdx);
		if (bRet == FALSE)
			eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
	}
	else if ( FileType == BIN_DATA_TYPE )
	{
		bRet =  PutBinData(pFileData,nLen,nPacketIdx);
		if (bRet == FALSE)
			eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
	}
	else if ( FileType == SRC_DATA_TYPE )
	{
		bRet =  PutSrcData(pFileData,nLen,nPacketIdx);
		if (bRet == FALSE)
			eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
	}
	else
		eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;

	return eStatus;
}

static MBException MBXReadFileCB( INT8U * pFileData, INT8U nLen,INT32U nFileOffset, INT8U FileType )
{
	MBException  eStatus = MB_ENOERR;
	INT8U bRet = FALSE;

	if (FileType == FCB_DATA_TYPE)
	{
	}
	else if (FileType == BIN_DATA_TYPE)
	{		 
		bRet = ReadSrcDataBuf(pFileData, nFileOffset,nLen);
	}
	else if (FileType == SRC_DATA_TYPE)
	{		
		bRet = ReadSrcDataBuf(pFileData, nFileOffset,nLen);
	}
	else
		eStatus = MB_EX_ILLEGAL_FUNCTION;

	if (bRet != TRUE)
	{
		eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
	}

	return eStatus;
}

static MBErrorCode MBGetDeviceVer( INT8U * pucBuffer)
{
	MBErrorCode    eStatus = MB_ENOERR;

	return eStatus;
}


/*********************************************************************************************************
** 函数名称: MBGetFlashRamFcb
** 函数名称: MBGetFlashRamFcb
**
** 功能描述： 获取FLASH和RAM中的FCB
**
** 输　入:  INT8U * pFcbBuf
**          
** 输　出:  MBErrorCode
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年11月14日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static MBErrorCode MBGetFlashRamFcb( INT8U * pFcbBuf)
{
	MBErrorCode eStatus = MB_ENOERR;
	INT8U err = 0;
	INT8U nPos = 0;
	INT8U nFcbLen = sizeof(FCB);
	INT8U *pFlashFcb = NULL;

	if (pFcbBuf == NULL)
		return MB_EINVAL;
	//拷贝FLASH的FCB
	GetFcbBuff( &pFlashFcb,&err);
	memcpy(& pFcbBuf[nPos],pFlashFcb,nFcbLen);
	nPos += nFcbLen;
	memcpy(& pFcbBuf[nPos] , & Ram.Fcb ,nFcbLen);

	return eStatus;
}

/*********************************************************************************************************
** 函数名称: MBFuncReadCoils
** 函数名称: MBFuncReadCoils
**
** 功能描述： 解析报文，并执行命令，然后组织成报文返回
**
** 输　入:  INT8U * pucFrame
** 输　入:  INT16U * usLen
**          
** 输　出:  MBException
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
MBException MBFuncReadCoils( INT8U *  pAduBuf, INT16U * usADULen )
{
	MBException   Status = MB_EX_NONE;
	INT16U      usRegAddress;
	INT16U      usCoilCount;
	INT8U       ucNBytes;
	INT8U       *pucFrameCur = NULL;
	MBErrorCode    RegStatus;
	INT8U * pPduFrame  = & pAduBuf[1];

	if( *usADULen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN+ MB_SER_PDU_PDU_OFF) )
	{
		usRegAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1] );
//		usRegAddress++;

		usCoilCount = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_COILCNT_OFF] << 8 );
		usCoilCount |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_COILCNT_OFF + 1] );
		/* Check if the number of registers to read is valid. If not
		* return Modbus illegal data value exception. 
		*/
		if( ( usCoilCount >= 1 ) && ( usCoilCount < MB_PDU_FUNC_READ_COILCNT_MAX ) )
		{
			/* Set the current PDU data pointer to the beginning. */
			pucFrameCur = &pPduFrame[MB_PDU_FUNC_OFF];
			*usADULen = MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_OFF;

			/* First byte contains the function code. */
			*pucFrameCur++ = MB_FUNC_READ_COILS;
			*usADULen += 1;

			/* Test if the quantity of coils is a multiple of 8. If not last
			* byte is only partially field with unused coils set to zero. */
			if( ( usCoilCount & 0x0007 ) != 0 )
			{
				ucNBytes = ( INT8U )( usCoilCount / 8 + 1 );
			}
			else
			{
				ucNBytes = ( INT8U )( usCoilCount / 8 );
			}
			*pucFrameCur++ = ucNBytes;
			*usADULen += 1;

			RegStatus = MBRegCoilsCB( pucFrameCur, usRegAddress, usCoilCount,MBX_READ );

			/* If an error occured convert it into a Modbus exception. */
			if( RegStatus != MB_ENOERR )
			{
				Status = MBError2Exception( RegStatus );
			}
			else
			{
				/* The response contains the function code, the starting address
				* and the quantity of registers. We reuse the old values in the 
				* buffer because they are still valid. */
				*usADULen += ucNBytes;
			}
		}
		else
		{
			Status = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid read coil register request because the length
		* is incorrect. */
		Status = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return  Status;
}
/*********************************************************************************************************
** 函数名称: MBFuncWriteCoil
** 函数名称: MBFuncWriteCoil
**
** 功能描述：写单个线圈 
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usLen
**          
** 输　出:  MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBFuncWriteCoil( INT8U * pAduBuf, INT16U * usLen )
{
	INT16U          usRegAddress;
	INT8U           ucBuf[2];

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;
	INT8U * pPduFrame  = & pAduBuf[1];


	if( *usLen == ( MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN + MB_SER_PDU_PDU_OFF) )
	{
		usRegAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1] );
//		usRegAddress++;

		if( ( pPduFrame[MB_PDU_FUNC_WRITE_VALUE_OFF + 1] == 0x00 ) &&
			( ( pPduFrame[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0xFF ) ||
			( pPduFrame[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0x00 ) ) )
		{
			ucBuf[1] = 0;
			if( pPduFrame[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0xFF )
			{
				ucBuf[0] = 1;
			}
			else
			{
				ucBuf[0] = 0;
			}
			eRegStatus = MBRegCoilsCB( &ucBuf[0], usRegAddress, 1, MBX_WRITE );

			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid write coil register request because the length
		* is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncWriteMultipleCoils
** 函数名称: MBFuncWriteMultipleCoils
**
** 功能描述： 写多个线圈
**
** 输　入:  UCHAR * pAduBuf
** 输　入:  USHORT * usLen
**          
** 输　出:  MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBFuncWriteMultipleCoils( INT8U * pAduBuf, INT16U * usLen )
{
	INT16U          usRegAddress;
	INT16U          usCoilCnt;
	INT8U           ucByteCount;
	INT8U           ucByteCountVerify;
	INT8U * pPduFrame  = & pAduBuf[1];

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if( *usLen > ( MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN + MB_SER_PDU_PDU_OFF) )
	{
		usRegAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1] );
//		usRegAddress++;

		usCoilCnt = ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF] << 8 );
		usCoilCnt |= ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF + 1] );

		ucByteCount = pPduFrame[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

		/* Compute the number of expected bytes in the request. */
		if( ( usCoilCnt & 0x0007 ) != 0 )
		{
			ucByteCountVerify = ( INT8U )( usCoilCnt / 8 + 1 );
		}
		else
		{
			ucByteCountVerify = ( INT8U )( usCoilCnt / 8 );
		}

		if( ( usCoilCnt >= 1 ) &&
			( usCoilCnt <= MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX ) &&
			( ucByteCountVerify == ucByteCount ) )
		{
			eRegStatus = MBRegCoilsCB( &pPduFrame[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF],usRegAddress, usCoilCnt, MBX_WRITE );

			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
			}
			else
			{
				/* The response contains the function code, the starting address
				* and the quantity of registers. We reuse the old values in the 
				* buffer because they are still valid. */
				*usLen = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF + MB_SER_PDU_PDU_OFF;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid write coil register request because the length
		* is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncReadDiscreteInputs
** 函数名称: MBFuncReadDiscreteInputs
**
** 功能描述： 读开入
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usADULen
**          
** 输　出:  MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月28日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
MBException MBFuncReadDiscreteInputs( INT8U * pAduBuf, INT16U * usADULen )
{
	INT16U          usRegAddress;
	INT16U          usDiscreteCnt;
	INT8U           ucNBytes;
	INT8U          *pucFrameCur;
	INT8U * pPduFrame = & pAduBuf[1];

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if( *usADULen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN + MB_SER_PDU_PDU_OFF) )
	{
		usRegAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1] );
//		usRegAddress++;

		usDiscreteCnt = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_DISCCNT_OFF] << 8 );
		usDiscreteCnt |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_DISCCNT_OFF + 1] );

		/* Check if the number of registers to read is valid. If not
		* return Modbus illegal data value exception. 
		*/
		if( ( usDiscreteCnt >= 1 ) &&
			( usDiscreteCnt < MB_PDU_FUNC_READ_DISCCNT_MAX ) )
		{
			/* Set the current PDU data pointer to the beginning. */
			pucFrameCur = &pPduFrame[MB_PDU_FUNC_OFF];
			*usADULen = MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_OFF;

			/* First byte contains the function code. */
			*pucFrameCur++ = MB_FUNC_READ_DISCRETE_INPUTS;
			*usADULen += 1;

			/* Test if the quantity of coils is a multiple of 8. If not last
			* byte is only partially field with unused coils set to zero. */
			if( ( usDiscreteCnt & 0x0007 ) != 0 )
			{
				ucNBytes = ( INT8U ) ( usDiscreteCnt / 8 + 1 );
			}
			else
			{
				ucNBytes = ( INT8U ) ( usDiscreteCnt / 8 );
			}
			*pucFrameCur++ = ucNBytes;
			*usADULen += 1;

			eRegStatus = MBRegDiscreteCB( pucFrameCur, usRegAddress, usDiscreteCnt );

			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
			}
			else
			{
				/* The response contains the function code, the starting address
				* and the quantity of registers. We reuse the old values in the 
				* buffer because they are still valid. */
				*usADULen += ucNBytes;;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid read coil register request because the length
		* is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncReadInputRegister
** 函数名称: MBFuncReadInputRegister
**
** 功能描述： 读模入
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usLen
**          
** 输　出:  MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBFuncReadInputRegister( INT8U * pAduBuf , INT16U * usLen )
{ 
	INT16U          usRegAddress;
	INT16U          usRegCount;
	INT8U          *pucFrameCur;
	INT8U * pPduFrame = & pAduBuf[1];

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if( *usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN + MB_SER_PDU_PDU_OFF) )
	{
		usRegAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1] );
//		usRegAddress++;

		usRegCount = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_REGCNT_OFF] << 8 );
		usRegCount |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_REGCNT_OFF + 1] );

		/* Check if the number of registers to read is valid. If not
		* return Modbus illegal data value exception. 
		*/
		if( ( usRegCount >= 1 )	&& ( usRegCount < MB_PDU_FUNC_READ_REGCNT_MAX ) )
		{
			/* Set the current PDU data pointer to the beginning. */
			pucFrameCur = &pPduFrame[MB_PDU_FUNC_OFF];
			*usLen = MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_OFF;

			/* First byte contains the function code. */
			*pucFrameCur++ = MB_FUNC_READ_INPUT_REGISTER;
			*usLen += 1;

			/* Second byte in the response contain the number of bytes. */
			*pucFrameCur++ = ( INT8U )( usRegCount * 2 );
			*usLen += 1;

			eRegStatus = MBRegInputCB( pucFrameCur, usRegAddress, usRegCount );

			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
			}
			else
			{
				*usLen += usRegCount * 2;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid read input register request because the length
		* is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncReadHoldingRegister
** 函数名称: MBFuncReadHoldingRegister
**
** 功能描述： 读保持型寄存器
**
** 输　入:  INT8U * pucFrame  ADU
** 输　入:  INT16U * usLen
**          
** 输　出:  MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月10日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBFuncReadHoldingRegister( INT8U * pAduBuf, INT16U * usADULen )
{
	INT16U          usRegAddress;
	INT16U          usRegCount;
	INT8U          *pucFrameCur;
	INT8U * pPduFrame = & pAduBuf[1];

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if( *usADULen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN + MB_SER_PDU_PDU_OFF) )
	{
		usRegAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1] );
//		usRegAddress++;

		usRegCount = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_REGCNT_OFF] << 8 );
		usRegCount = ( INT16U )( pPduFrame[MB_PDU_FUNC_READ_REGCNT_OFF + 1] );

		/* Check if the number of registers to read is valid. If not
		* return Modbus illegal data value exception. 
		*/
		if( ( usRegCount >= 1 ) && ( usRegCount <= MB_PDU_FUNC_READ_REGCNT_MAX ) )
		{
			/* Set the current PDU data pointer to the beginning. */
			pucFrameCur = &pPduFrame[MB_PDU_FUNC_OFF];
			*usADULen = MB_SER_PDU_PDU_OFF + MB_PDU_FUNC_OFF;

			/* First byte contains the function code. */
			*pucFrameCur++ = MB_FUNC_READ_HOLDING_REGISTER;
			*usADULen += 1;

			/* Second byte in the response contain the number of bytes. */
			*pucFrameCur++ = ( INT8U ) ( usRegCount * 2 );
			*usADULen += 1;

			/* Make callback to fill the buffer. */
			eRegStatus = MBRegHoldingCB( pucFrameCur, usRegAddress, usRegCount, MBX_READ );
			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
			}
			else
			{
				*usADULen += usRegCount * 2;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncWriteHoldingRegister
** 函数名称: MBFuncWriteHoldingRegister
**
** 功能描述： 写单个寄存器
**
** 输　入:  INT8U * pucFrame
** 输　入:  INT16U * usLen
**          
** 输　出:  MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBFuncWriteHoldingRegister( INT8U * pAduBuf, INT16U * usLen )
{
	INT16U          usRegAddress;
	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;
	INT8U * pPduFrame = & pAduBuf[1];

	if( *usLen == ( MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN + MB_SER_PDU_PDU_OFF ) )
	{
		usRegAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1] );
//		usRegAddress++;

		/* Make callback to update the value. */
		eRegStatus = MBRegHoldingCB( &pPduFrame[MB_PDU_FUNC_WRITE_VALUE_OFF],usRegAddress, 1, MBX_WRITE );

		/* If an error occured convert it into a Modbus exception. */
		if( eRegStatus != MB_ENOERR )
		{
			eStatus = MBError2Exception( eRegStatus );
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncWriteMultipleHoldingRegister
** 函数名称: MBFuncWriteMultipleHoldingRegister
**
** 功能描述： 写多个寄存器
**
** 输　入:  INT8U * pucFrame
** 输　入:  INT16U * usLen
**          
** 输　出:  MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBFuncWriteMultipleHoldingRegister( INT8U * pAduBuf, INT16U * usLen )
{
	INT16U          usRegAddress;
	INT16U          usRegCount;
	INT8U           ucRegByteCount;
	INT8U * pPduFrame = & pAduBuf[1];

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if( *usLen >= ( MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + MB_PDU_SIZE_MIN + MB_SER_PDU_PDU_OFF) )
	{
		usRegAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1] );
//		usRegAddress++;

		usRegCount = ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF] << 8 );
		usRegCount |= ( INT16U )( pPduFrame[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1] );

		ucRegByteCount = pPduFrame[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

		if( ( usRegCount >= 1 ) &&
			( usRegCount <= MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX ) &&
			( ucRegByteCount == ( INT8U ) ( 2 * usRegCount ) ) )
		{
			/* Make callback to update the register values. */
			eRegStatus = MBRegHoldingCB( &pPduFrame[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF],usRegAddress, usRegCount, MBX_WRITE );

			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
			}
			else
			{
				/* The response contains the function code, the starting
				* address and the quantity of registers. We reuse the
				* old values in the buffer because they are still valid.
				*/
				*usLen = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF + MB_SER_PDU_PDU_OFF;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncReadWriteMultipleHoldingRegister
** 函数名称: MBFuncReadWriteMultipleHoldingRegister
**
** 功能描述： 读/写多个寄存器
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usLen
**          
** 输　出:  extern MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBFuncReadWriteMultipleHoldingRegister( INT8U * pAduBuf, INT16U * usLen )
{
	INT16U          usRegReadAddress;
	INT16U          usRegReadCount;
	INT16U          usRegWriteAddress;
	INT16U          usRegWriteCount;
	INT8U           ucRegWriteByteCount;
	INT8U          *pucFrameCur;
	INT8U * pPduFrame = & pAduBuf[1];

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if( *usLen >= ( MB_PDU_FUNC_READWRITE_SIZE_MIN + MB_PDU_SIZE_MIN ) )
	{
		usRegReadAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF] << 8U );
		usRegReadAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF + 1] );
//		usRegReadAddress++;

		usRegReadCount = ( INT16U )( pPduFrame[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF] << 8U );
		usRegReadCount |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF + 1] );

		usRegWriteAddress = ( INT16U )( pPduFrame[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF] << 8U );
		usRegWriteAddress |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF + 1] );
//		usRegWriteAddress++;

		usRegWriteCount = ( INT16U )( pPduFrame[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF] << 8U );
		usRegWriteCount |= ( INT16U )( pPduFrame[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF + 1] );

		ucRegWriteByteCount = pPduFrame[MB_PDU_FUNC_READWRITE_BYTECNT_OFF];

		if( ( usRegReadCount >= 1 ) && ( usRegReadCount <= 0x7D ) &&
			( usRegWriteCount >= 1 ) && ( usRegWriteCount <= 0x79 ) &&
			( ( 2 * usRegWriteCount ) == ucRegWriteByteCount ) )
		{
			/* Make callback to update the register values. */
			eRegStatus = MBRegHoldingCB( &pPduFrame[MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF],usRegWriteAddress, usRegWriteCount, MBX_WRITE );

			if( eRegStatus == MB_ENOERR )
			{
				/* Set the current PDU data pointer to the beginning. */
				pucFrameCur = &pPduFrame[MB_PDU_FUNC_OFF];
				*usLen = MB_SER_PDU_PDU_OFF + MB_PDU_FUNC_OFF;

				/* First byte contains the function code. */
				*pucFrameCur++ = MB_FUNC_READWRITE_MULTIPLE_REGISTERS;
				*usLen += 1;

				/* Second byte in the response contain the number of bytes. */
				*pucFrameCur++ = ( INT8U ) ( usRegReadCount * 2 );
				*usLen += 1;

				/* Make the read callback. */
				eRegStatus = MBRegHoldingCB( pucFrameCur, usRegReadAddress, usRegReadCount, MBX_READ );
				if( eRegStatus == MB_ENOERR )
				{
					*usLen += 2 * usRegReadCount;
				}
			}
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBXFuncReadForceMark
** 函数名称: MBXFuncReadForceMark
**
** 功能描述： 批量读强制标志
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usLen
**          
** 输　出:  extern MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年11月24日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBXFuncReadForceMark( INT8U * pAduBuf, INT16U * usLen )
{
	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	INT8U * pPduFrame  = & pAduBuf[1];
	INT8U Len = pPduFrame[1]; //报文长度
	
	INT16U ICnt = 0;
	INT16U QCnt = 0;
	INT16U AICnt = 0;
	INT16U AQCnt = 0;

	INT16U IAddr = 0;
	INT16U QAddr = 0;
	INT16U AIAddr = 0;
	INT16U AQAddr = 0;

// 	INT16U  usRegAddress = 0;
// 	INT16U  usCount = 0;

	INT8U ucNBytes;
	INT8U RegType = 0;
	INT8U nIdx = 0;

	if( *usLen > ( MBX_PDU_FUNC_READ_FM_SIZE + MB_SER_PDU_PDU_OFF) )
	{
		//解析收到的报文
		while (1)
		{
			RegType = pPduFrame[2+nIdx];
			if (RegType == I_TYPE)
			{
				IAddr = (INT16U) (pPduFrame[3+nIdx] << 8);
				IAddr |= (INT16U) (pPduFrame[4+nIdx]);

				ICnt = (INT16U) (pPduFrame[5+nIdx] << 8);
				ICnt |= (INT16U) (pPduFrame[6+nIdx]);
			}
			else if ( RegType == Q_TYPE )
			{
				QAddr = (INT16U) (pPduFrame[3+nIdx] << 8);
				QAddr |= (INT16U) (pPduFrame[4+nIdx]);

				QCnt = (INT16U) (pPduFrame[5+nIdx] << 8);
				QCnt |= (INT16U) (pPduFrame[6+nIdx]);
			}
			else if ( RegType == AI_TYPE )
			{
				AIAddr = (INT16U) (pPduFrame[3+nIdx] << 8);
				AIAddr |= (INT16U) (pPduFrame[4+nIdx]);

				AICnt = (INT16U) (pPduFrame[5+nIdx] << 8);
				AICnt |= (INT16U) (pPduFrame[6+nIdx]);
			}
			else if ( RegType == AQ_TYPE )
			{
				AQAddr = (INT16U) (pPduFrame[3+nIdx] << 8);
				AQAddr |= (INT16U) (pPduFrame[4+nIdx]);

				AQCnt = (INT16U) (pPduFrame[5+nIdx] << 8);
				AQCnt |= (INT16U) (pPduFrame[6+nIdx]);
			}
			else
			{
				break;
			}

			nIdx += 5;
			if (nIdx>=Len)
			{
				break;
			}
		}
		//组织回复的报文
		nIdx  = 0; //总的字节长度
		if (ICnt >=1 && ICnt <= MAX_I_CNT )
		{
			if( ( ICnt & 0x0007 ) != 0 )
			{
				ucNBytes = ( INT8U ) ( ICnt / 8 + 1 );
			}
			else
			{
				ucNBytes = ( INT8U ) ( ICnt / 8 );
			}
			pPduFrame[nIdx+2] = I_TYPE;
			pPduFrame[nIdx+3] = (INT8U) (ICnt >> 8);//寄存器数量
			pPduFrame[nIdx+4] = (INT8U) (ICnt);//寄存器数量

			eRegStatus = MBXReadForceMarkCB(& pPduFrame[nIdx+5] ,I_TYPE,IAddr,ICnt);
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
				return eStatus;
			}
			else
			{
				nIdx += 3;
				nIdx += ucNBytes;
			}
		}
		if (AICnt >=1 && AICnt <= MAX_AI_CNT )
		{
			if( ( AICnt & 0x0007 ) != 0 )
			{
				ucNBytes = ( INT8U ) ( AICnt / 8 + 1 );
			}
			else
			{
				ucNBytes = ( INT8U ) ( AICnt / 8 );
			}
			pPduFrame[nIdx+2] = AI_TYPE;
			pPduFrame[nIdx+3] = (INT8U) (AICnt >> 8);//寄存器数量
			pPduFrame[nIdx+4] = (INT8U) (AICnt);//寄存器数量

			eRegStatus = MBXReadForceMarkCB(& pPduFrame[nIdx+5] ,AI_TYPE,AIAddr,AICnt);
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
				return eStatus;
			}
			else
			{
				nIdx += 3;
				nIdx += ucNBytes;
			}
		}
		if (QCnt >=1 && QCnt <= MAX_Q_CNT )
		{
			if( ( QCnt & 0x0007 ) != 0 )
			{
				ucNBytes = ( INT8U ) ( QCnt / 8 + 1 );
			}
			else
			{
				ucNBytes = ( INT8U ) ( QCnt / 8 );
			}
			pPduFrame[nIdx+2] = Q_TYPE;
			pPduFrame[nIdx+3] = (INT8U) (QCnt >> 8);//寄存器数量
			pPduFrame[nIdx+4] = (INT8U) (QCnt);//寄存器数量

			eRegStatus = MBXReadForceMarkCB(& pPduFrame[nIdx+5] ,Q_TYPE,QAddr,QCnt);
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
				return eStatus;
			}
			else
			{
				nIdx += 3;
				nIdx += ucNBytes;
			}
		}
		if (AQCnt >=1 && AQCnt <= MAX_AQ_CNT )
		{
			if( ( AQCnt & 0x0007 ) != 0 )
			{
				ucNBytes = ( INT8U ) ( AQCnt / 8 + 1 );
			}
			else
			{
				ucNBytes = ( INT8U ) ( AQCnt / 8 );
			}
			pPduFrame[nIdx+2] = AQ_TYPE;
			pPduFrame[nIdx+3] = (INT8U) (AQCnt >> 8);//寄存器数量
			pPduFrame[nIdx+4] = (INT8U) (AQCnt);//寄存器数量

			eRegStatus = MBXReadForceMarkCB(& pPduFrame[nIdx+5] ,AQ_TYPE,AQAddr,AQCnt);
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
				return eStatus;
			}
			else
			{
				nIdx += 3;
				nIdx += ucNBytes;
			}
		}
		if (nIdx > 3)
		{
			pPduFrame[1] = nIdx;
			*usLen = nIdx + 2 + MB_SER_PDU_PDU_OFF;
		}
		else
			return MB_EX_ILLEGAL_DATA_VALUE;
	}
	else
	{
		/* Can't be a valid write coil register request because the length
		* is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
//写单个寄存器的强制标志
extern MBException MBXFuncWriteForceMark( INT8U * pAduBuf, INT16U * usLen )
{
	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	INT8U * pPduFrame  = & pAduBuf[1];
	INT8U RegType = pPduFrame[1]; // 

	INT16U          usRegAddress;
	 
	if( *usLen == ( MBX_PDU_WRITE_READ_FM_SIZE + MB_SER_PDU_PDU_OFF) )
	{
		usRegAddress = ( INT16U )( pPduFrame[2] << 8 );
		usRegAddress |= ( INT16U )( pPduFrame[2 + 1] );
		 
		if( ( pPduFrame[4 + 1] == 0x00 ) &&
			( ( pPduFrame[4] == 0xFF ) ||
			( pPduFrame[4] == 0x00 ) ) )
		{
		 	if( pPduFrame[4] == 0xFF )
			{
				eRegStatus = MBXWriteForceMarkCB(RegType,usRegAddress,TRUE);  
		 	}
			else
			{
				eRegStatus = MBXWriteForceMarkCB(RegType,usRegAddress,FALSE);		
			}
			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR )
			{
				eStatus = MBError2Exception( eRegStatus );
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid write coil register request because the length
		* is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBXFuncSetTime
** 函数名称: MBXFuncSetTime
**
** 功能描述：对时命令
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usAduLen
**          
** 输　出:  extern MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBXFuncSetTime( INT8U * pAduBuf, INT16U * usAduLen )
{
	MBException    eStatus = MB_EX_NONE;
	INT8U * pPduFrame = & pAduBuf[1];
	INT8U nPduIdx = 2;

	INT16U year = 0;
	INT8U month = 0;
	INT8U day = 0;
	INT8U hr = 0;
	INT8U min = 0;
	INT8U sec = 0;
	INT8U bRespFlag = FALSE;

	INT8U bRet = FALSE;

	if( *usAduLen == MBX_PDU_FUNC_DEVICE_CTRL_LEN + MB_SER_PDU_PDU_OFF)
	{
		if (pAduBuf[0] != BROADCAST_ADDRESS && pPduFrame[nPduIdx] == 0XFF)
		{
			bRespFlag = TRUE;
		}
		else
		{
			bRespFlag = FALSE;
		}
		nPduIdx++;

		year = ( INT16U )( pPduFrame[nPduIdx] << 8U );
		nPduIdx++;

		year |= ( INT16U )( pPduFrame[nPduIdx] );
		nPduIdx++;

		month = pPduFrame[nPduIdx]  ;
		nPduIdx++;

		day =   pPduFrame[nPduIdx]  ;
		nPduIdx++;

		hr =   pPduFrame[nPduIdx]  ;
		nPduIdx++;

		min =   pPduFrame[nPduIdx]  ;
		nPduIdx++;

		sec =   pPduFrame[nPduIdx]  ;
		nPduIdx++;
		//毫秒丢弃不用
		bRet =  ClockSetDateTime(  month,   day,   year,   hr,   min,   sec);
		if (bRet == FALSE)
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		if (bRespFlag == TRUE)
		{//回复报文
			*usAduLen = MB_SER_PDU_PDU_OFF+6;
			pPduFrame[2] = bRet == TRUE ? 0xff: 0x00; 
			pPduFrame[3] = 0;
			pPduFrame[4] = 0;
			pPduFrame[5] = 0;
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBXFuncReSetDevice
** 函数名称: MBXFuncReSetDevice
**
** 功能描述： 复位装置
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usAduLen
**          
** 输　出:  extern MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBXFuncReSetDevice( INT8U * pAduBuf, INT16U * usAduLen )
{
	MBException    eStatus = MB_EX_NONE;
 	INT8U * pPduFrame = & pAduBuf[1];


	Reset();


	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncMEI
** 函数名称: MBFuncMEI
**
** 功能描述： 
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usAduLen
**          
** 输　出:  extern MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月14日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBFuncMEI( INT8U * pAduBuf, INT16U * usAduLen )
{
	MBException    eStatus = MB_EX_NONE;
	INT8U * pPduFrame = & pAduBuf[1];
	INT8U SubFuncCode = 0;
	INT8U ReadDevId = 0;
	INT8U ObjId = 0;
	INT8U nPduIdx = 0;
	 	
	if( *usAduLen == MB_PDU_FUNC_MEI_LEN + MB_SER_PDU_PDU_OFF)
	{
		SubFuncCode = pPduFrame[1];
		if (SubFuncCode == 0x0e)
		{
			ReadDevId = pPduFrame[2];
			if (ReadDevId == 0x01)
			{

			}			
			else if (ReadDevId == 0x02)
			{

			}
			else if (ReadDevId == 0x03)
			{


			}
			else if (ReadDevId == 0x04)
			{//专有数据对象
				ObjId = pPduFrame[3];
				nPduIdx = 3;
				switch ( ObjId )
				{
				case MB_MEI_READ_DEV_VER:
					break;
				case MB_MEI_READ_DN_TM:
					break;
				case MB_MEI_READ_VER_TM:
					break;
				case MB_MEI_READ_CFG_VER:
					break;
				case MB_MEI_READ_FLASH_FCB:
					break;
				case MB_MEI_READ_RAM_FCB:
					break;
				case MB_MEI_READ_FLASHRAM_FCB:
					pPduFrame[nPduIdx] = 0; //一致性
					nPduIdx++;
					pPduFrame[nPduIdx] = 0; //更多
					nPduIdx++;
					pPduFrame[nPduIdx] = 01; //对象数量
					nPduIdx++;
					pPduFrame[nPduIdx] = ObjId; //对象ID
					nPduIdx++;
					pPduFrame[nPduIdx] = sizeof(FCB) *2; //对象长度

					nPduIdx++;
					MBGetFlashRamFcb(&pPduFrame[nPduIdx]);
					
					*usAduLen = sizeof(FCB) *2 + 9;

					break;
				default:
					break;
				}				 
			}
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}

	return eStatus;
} 

/*********************************************************************************************************
** 函数名称: MBXWriteFileRecord
** 函数名称: MBXWriteFileRecord
**
** 功能描述： 
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usAduLen
**          
** 输　出:  extern MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月23日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBXWriteFileRecord( INT8U * pAduBuf, INT16U * usAduLen )
{
	MBException  eStatus = MB_EX_NONE;
	INT8U * pPduFrame = & pAduBuf[1];
	INT8U FileType = 0; //文件类型：BIN/SRC/
	INT16U PacketIdx = 0;
	INT8U PacketLen = 0;
 
	if( *usAduLen >= ( MBX_PDU_FUNC_DN_FILE_SIZE_MIN + MB_PDU_SIZE_MIN ) )
	{
		FileType= pPduFrame[3];
		PacketLen = pPduFrame[4];

		PacketIdx = pPduFrame[5] << 8 & 0xff00 | pPduFrame[6]; //报文的顺序号
	 
		//写缓冲
		eStatus = MBXWriteFileCB( & pPduFrame[7] ,PacketLen,PacketIdx,FileType);

		pPduFrame[1] = 6; //回复报文长度

		//如果写正常，准备回复报文
		if (eStatus == MB_EX_NONE)
		{
			pPduFrame[7] = 0xff;
			*usAduLen = MBX_PDU_FUNC_DN_FILE_SIZE_MIN + MB_PDU_SIZE_MIN;
		}
		else
		{
			pPduFrame[7] = 0;
			*usAduLen = MBX_PDU_FUNC_DN_FILE_SIZE_MIN + MB_PDU_SIZE_MIN;
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
 	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBXWriteFileToFlash
** 函数名称: MBXWriteFileToFlash
**
** 功能描述： 文件下载控制命令 文件下载结束后，下发此报文，通知装置文件下载结束 装置从ram写入flash
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usAduLen
**          
** 输　出:  extern MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月27日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBXWriteFileToFlash( INT8U * pAduBuf, INT16U * usAduLen )
{
	MBException  eStatus = MB_EX_NONE;
	INT8U * pPduFrame = & pAduBuf[1];
	INT8U  Ret = FALSE;
	INT8U  err = 0;

	if( *usAduLen >= ( 5 + MB_PDU_SIZE_MIN ) )
	{
		Ret = SaveData(&err);
		pPduFrame[1] = 0x03;//报文长度
		//组织回复报文
		if (Ret)
		{//成功
			pPduFrame[3] = 0xff;
			pPduFrame[4] = 0xff;
			//ADU的长度不变
		}
		else
		{
			pPduFrame[3] = 0x00; //失败的标志
			pPduFrame[4] = err;  //错误号
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBXReadFileRecord
** 函数名称: MBXReadFileRecord
**
** 功能描述： 上传文件信息
**
** 输　入:  INT8U * pAduBuf
** 输　入:  INT16U * usAduLen
**          
** 输　出:  extern MBException
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月27日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern MBException MBXReadFileRecord( INT8U * pAduBuf, INT16U * usAduLen )
{
	MBException  eStatus = MB_EX_NONE;
	INT8U * pPduFrame = & pAduBuf[1];
	INT8U FileType = 0; //文件类型：BIN/SRC/
	INT8U PacketLen = 0;
	INT32U BuffOffset = 0;


	if( *usAduLen >= ( MBX_PDU_FUNC_UP_FILE_SIZE_MIN + MB_PDU_SIZE_MIN ) )
	{
		FileType= pPduFrame[3];
		PacketLen = pPduFrame[4];

		BuffOffset =  (pPduFrame[5] << 24 & 0xff000000) | 
			(pPduFrame[6]  << 16 & 0x00ff0000 )|
			(pPduFrame[7] << 8 & 0xff00 )| pPduFrame[8]; //数据的偏移地址
		//读缓冲
		eStatus = MBXReadFileCB(&pPduFrame[3],PacketLen,BuffOffset,FileType);

		//如果写正常，准备回复报文
		if (eStatus == MB_EX_NONE)
		{
			pPduFrame[1] = PacketLen + 1;//报文长度
			*usAduLen = pPduFrame[1] + 2 + MB_PDU_SIZE_MIN;
		}	
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
