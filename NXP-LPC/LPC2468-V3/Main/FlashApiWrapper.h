/****************************************Copyright (c)**************************************************
**                                       FLASH文件操作接口的封装
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
#ifndef FLASH_API_H
#define	FLASH_API_H 
#include "FileData.h"

typedef enum FAErrCode
{
	FLASH_API_NO_ERR = 0,   //没有错
	FLASH_API_NO_FCB = 1,   //没有FCB控制块
	FLASH_API_NO_BINDATA = 2, //没有bin文件
	FLASH_API_BIN_CRC_ERR = 3,
	FLASH_API_SRC_CRC_ERR = 4,
	FLASH_API_BIN_LEN_ERR = 5 ,//BIN 文件长度错误 
	FLASH_API_SRC_LEN_ERR = 6 ,//SRC 文件长度错误，超过限制,或者与下载的长度不一致
	FLASH_API_FCB_FLASH_ERR = 7 ,//从FLASH返读错误
	FLASH_API_BIN_FLASH_ERR = 8 ,//从FLASH返读错误
	FLASH_API_SRC_FLASH_ERR = 9  //从FLASH返读错误
}FAErrCode; 

typedef enum FLASH_DATA_TYPE
{
	FCB_DATA_TYPE = 1,
	BIN_DATA_TYPE = 2,
	SRC_DATA_TYPE = 3
}FLASH_DATA_TYPE;

//通常这些写操作都是由通信模块调用的，或者在液晶界面里面操作
//写控制块
//INT16U WriteFcb(  FCB *pFcb  );
//写BIN
//INT16U WriteBinData( BIN_DATA  *pBinDat, INT16U nLen);
//写源文件
INT32U WriteSrcData( INT8U *pBuf,INT32U nLen);

//擦除
extern INT8U  ErasureFlash( void ); 

extern INT8U LoadFlashData( );

//读操作，从flash中读取相应的配置
extern INT16U LoadFcb(FCB *pFcb,INT8U *err);
//由通信模块调用或者在系统启动时初始化调用
extern INT8U  ReadSrcDataBuf(INT8U *pBuff, INT32U nFileOffset,INT8U nLen);
extern INT8U  ReadBinDataBuf(INT8U *pBuff, INT32U nFileOffset,INT8U nLen);

extern INT8U  GetFcbBuff(INT8U **pBuff,INT8U *pLen );
extern INT8U  GetBinDataBuff(INT8U **ppBuff, INT32U *pLen);
extern INT8U  GetSrcDataBuff(INT8U **ppBuff, INT32U *pLen);

//由通信模块调用，往临时缓冲里面写数据
extern INT8U PutFcbData(INT8U *pBuff, INT8U nLen,INT16U nPacketIdx );
extern INT8U PutBinData(INT8U *pBuff, INT8U nLen,INT16U nPacketIdx );
extern INT8U PutSrcData(INT8U *pBuff, INT8U nLen,INT16U nPacketIdx );

extern INT8U SaveData(INT8U *err );


//内存自检
extern INT8U SelfCheckRam(void);		

 
#endif
