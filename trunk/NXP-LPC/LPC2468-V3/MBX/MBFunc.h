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

#ifndef MB_FUNC_H
#define MB_FUNC_H 

//MBX
#define  MBX_PDU_FUNC_DEVICE_CTRL_LEN    (12)           //对时命令PDU的长度

#define  MB_PDU_FUNC_MEI_LEN      (4)  //MEI的PDU长度固定为4


/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */


//读开出以及MB寄存器
extern MBException MBFuncReadCoils( INT8U * pucFrame, INT16U * usLen );
//写单个线圈
extern MBException MBFuncWriteCoil( INT8U * pAduBuf, INT16U * usLen );
//写多个线圈
extern MBException MBFuncWriteMultipleCoils( INT8U * pucFrame, INT16U * usLen );
//读开入
extern MBException MBFuncReadDiscreteInputs( INT8U * pucFrame, INT16U * usLen );
//读保持型寄存器 AQ MW
extern MBException MBFuncReadHoldingRegister( INT8U * pucFrame, INT16U * usLen );
//读AI
extern MBException MBFuncReadInputRegister( INT8U * pucFrame, INT16U * usLen );

//写单个寄存器
extern MBException MBFuncWriteHoldingRegister( INT8U * pAduBuf, INT16U * usLen );
//写多个寄存器
extern MBException MBFuncWriteMultipleHoldingRegister( INT8U * pucFrame, INT16U * usLen );
//读、写多个寄存器
extern MBException MBFuncReadWriteMultipleHoldingRegister( INT8U * pAduBuf, INT16U * usLen );
//批量读强制标志
extern MBException MBXFuncReadForceMark( INT8U * pAduBuf, INT16U * usLen );
//写单个寄存器的强制标志
extern MBException MBXFuncWriteForceMark( INT8U * pAduBuf, INT16U * usLen );


extern INT8U IsValidFuncCode( INT8U Code  );

//////////////////////////////////////////////////////////////////////////

extern MBException MBXFuncSetTime( INT8U * pAduBuf, INT16U * usAduLen );

extern MBException MBFuncMEI( INT8U * pAduBuf, INT16U * usAduLen );
//写文件
extern MBException MBXWriteFileRecord( INT8U * pAduBuf, INT16U * usAduLen );
//读文件
extern MBException MBXReadFileRecord( INT8U * pAduBuf, INT16U * usAduLen );
//保存数据到FLASH
extern MBException MBXWriteFileToFlash( INT8U * pAduBuf, INT16U * usAduLen );

#endif
