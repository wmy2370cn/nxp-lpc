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
#include <stdlib.h>
#include <string.h>
#include "mb.h"
#include "MemCNST.h"
#include "MemData.h"
 

#pragma diag_suppress   186
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
MBErrorCode MBRegCoilsCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNCoils, MBRegisterMode eMode )
{
	MBErrorCode  Status = MB_ENOERR;
	INT8U offset;
	INT16U i=0 ;
	INT8U  *pByte = pucRegBuffer;
	INT16U         RegIndex = 0;

	if ( (usAddress >= Q_START_ADDR) && (usAddress + usNCoils <= Q_START_ADDR+MAX_Q_CNT ) )
	{
		if (eMode == MB_REG_READ)
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
		else if ( eMode == MB_REG_WRITE )
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
MBErrorCode MBRegDiscreteCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNDiscrete )
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
MBErrorCode MBRegInputCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNRegs )
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
MBErrorCode MBRegHoldingCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNRegs, MBRegisterMode eMode )
{
	MBErrorCode    eStatus = MB_ENOERR;
	INT32U         iRegIndex = 0;
	INT16U         Value = 0;

	if ( (usAddress >= MW_START_ADDR) && (usAddress + usNRegs <= MW_START_ADDR + MAX_MW_CNT) )
	{
		iRegIndex = ( INT32U )( usAddress - MW_START_ADDR );
		if (eMode == MB_REG_READ)
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
		else if ( eMode == MB_REG_WRITE )
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
		if (eMode == MB_REG_READ)
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
		else if ( eMode == MB_REG_WRITE )
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
