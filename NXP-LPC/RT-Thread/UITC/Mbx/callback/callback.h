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


MBErrorCode MBRegCoilsCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNCoils, MBRegisterMode eMode );
MBErrorCode MBRegDiscreteCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNDiscrete );
MBErrorCode MBRegInputCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNRegs );
MBErrorCode MBRegHoldingCB( INT8U * pucRegBuffer, INT16U usAddress, INT16U usNRegs, MBRegisterMode eMode );

