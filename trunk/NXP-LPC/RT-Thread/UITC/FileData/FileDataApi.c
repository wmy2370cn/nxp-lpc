/****************************************Copyright (c)**************************************************
**                                       FLASH文件操作接口的封装
**                                      文件的加载必须依照顺序加载，先加载FCB，然后是BIN SRC
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
#include "applib.h"
#include "IAP.h"

#include "FileDataDef.h"
#include "FileDataApi.h"
#include "Func.h"

//
 
#define  FCB_BLOCK_SIZE  4*1024  //0X16
#define  BIN_BLOCK_SIZE  4*1024  //0X17
#define  SRC_BLOCK_SIZE  4*1024  //0X18 

//FLASH的地址
 
#define	 FCB_BLOCK_DESTADDR 	0x00078000      //扇区0X16 
#define  BIN_BLOCK_DESTADDR 	0x00079000		//0X17
#define  SRC_BLOCK_DESTADDR 	0x0007A000		//0X18
 
//存储区管理指针
static INT32U FcbBuffPtr = 0;
static INT32U BinBuffPtr = 0;
static INT32U SrcBuffPtr = 0;

//用于存储FCB控制块的 内存临时区
__align(4) static INT8U FCB_BLOCK[FCB_BLOCK_SIZE];	
//BIN DATA
__align(4) 	static INT8U BIN_BLOCK[BIN_BLOCK_SIZE];	
//SRC DATA
__align(4) 	static INT8U SRC_BLOCK[SRC_BLOCK_SIZE];	

static FCB *g_spTmpFcb = NULL;

/*********************************************************************************************************
** 函数名称: WriteFcb
** 函数名称: WriteFcb
**
** 功能描述： 写配置信息控制块
**
** 输　入:  FCB * pFcb
**          
** 输　出:  INT16U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月6日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT16U WriteFcb( FCB *pFcb )
{

	return sizeof(FCB);
}

INT16U WriteBinData( BIN_DATA  *pBinDat, INT16U nLen)
{
	if (pBinDat == NULL || nLen == 0 || nLen >= BIN_BLOCK_SIZE)
		return 0;


	return 0;		
}

INT32U WriteSrcData( INT8U *pBuf,INT32U nLen)
{

	return 0;
}


INT8U  ErasureFlash( )
{
	return TRUE;
}

/*********************************************************************************************************
** 函数名称: ReadSrcData
** 函数名称: ReadSrcData
**
** 功能描述： 
**
** 输　入:  INT32U nLen
** 输　入:  INT16U wSrcCrc
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注: 注意，如果长度太长，可能得需要跨扇区读
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U ReadSrcData(INT32U nLen ,INT16U wSrcCrc)
{
	INT16U wCrc = 0;
	if (nLen>SRC_BLOCK_SIZE || wSrcCrc == 0)
		return FALSE;

	//从flash中读取数据到内存中
	FLASH_ReadByte(SRC_BLOCK_DESTADDR,SRC_BLOCK, nLen );
	//计算一下CRC值
	wCrc = GetCRC16(SRC_BLOCK,nLen);
	if (wCrc == wSrcCrc)	
		return TRUE;	

	return FALSE;
}
/*********************************************************************************************************
** 函数名称: ReadBinData
** 函数名称: ReadBinData
**
** 功能描述： 从flash中读取配置信息
**
** 输　入:  INT32U nLen 需要读取的长度
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注:  注意，如果长度太长，可能得需要跨扇区读
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U ReadBinData(INT32U nLen ,INT16U wBinCrc)
{
	INT16U wCrc = 0;
	if (nLen>BIN_BLOCK_SIZE || wBinCrc == 0)
		return FALSE;
	
	//从flash中读取数据到内存中
	FLASH_ReadByte(BIN_BLOCK_DESTADDR,BIN_BLOCK, nLen );
	//计算一下CRC值
	wCrc = GetCRC16(BIN_BLOCK,nLen);
	if (wCrc == wBinCrc)	
		return TRUE;	

	return FALSE;
}

extern INT8U  ReadSrcDataBuf(INT8U *pBuff, INT32U nFileOffset,INT8U nLen)
{
	if (pBuff == NULL)
		return FALSE;
	if (nFileOffset >= SRC_BLOCK_SIZE)
		return FALSE;
	if (nLen<=0)
		return FALSE;
	//从flash中读取数据到内存中
	return	FLASH_ReadByte(SRC_BLOCK_DESTADDR+nFileOffset,pBuff, nLen );
}


extern INT8U  ReadBinDataBuf(INT8U *pBuff, INT32U nFileOffset,INT8U nLen)
{
	if (pBuff == NULL)
		return FALSE;
	if (nFileOffset >= BIN_BLOCK_SIZE)
		return FALSE;
	if (nLen<=0)
		return FALSE;
	//从flash中读取数据到内存中
	return	FLASH_ReadByte(BIN_BLOCK_DESTADDR+nFileOffset,pBuff, nLen );
}

/*********************************************************************************************************
** 函数名称: ReadFcb
** 函数名称: ReadFcb
**
** 功能描述： 从flash中读取数据，
**
**          
** 输　出:  INT16U 如果数据有效则返回长度，如果无效，则返回0
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT16U CheckFcb(FCB * pTmpFcb, INT8U *err )
{
	if(pTmpFcb == NULL)
	{
		return FALSE;
	}
	//做有效性检查
	if (pTmpFcb->wHeadFlag != FCB_FLAG)
	{
		*err = FLASH_API_NO_FCB;
		return FALSE;
	}

	if ( pTmpFcb->nBinLen == 0  || pTmpFcb->nBinLen >BIN_BLOCK_SIZE )
	{
		*err = FLASH_API_NO_BINDATA;
		return FALSE;
	}

	if ( pTmpFcb->wBinCrc == 0)
	{
		*err = FLASH_API_BIN_CRC_ERR;
		return FALSE;
	}

	return sizeof(FCB);
}
/*********************************************************************************************************
** 函数名称: LoadFcb
** 函数名称: LoadFcb
**
** 功能描述：  从flash中读取数据，并应用配置信息
**
** 输　入:  FCB * pFcb
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注: 系统初始化时调用该函数
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern INT16U LoadFcb(FCB *pFcb,INT8U *err)
{
	INT16U nRet = 0;

	FLASH_ReadByte(FCB_BLOCK_DESTADDR,FCB_BLOCK,FCB_BLOCK_SIZE );
	g_spTmpFcb = (FCB *) FCB_BLOCK ;

	nRet = CheckFcb( g_spTmpFcb,err );

	if (nRet&& pFcb!= NULL)
	{
		//检查有效，则把相关的数据存放到RAM的对应结构中
		memcpy(pFcb,g_spTmpFcb,sizeof(FCB));
		return sizeof(FCB);
	}
	return FALSE;
}
/*********************************************************************************************************
** 函数名称: GetFcbBuff
** 函数名称: GetFcbBuff
**
** 功能描述： 在通信模块中调用该函数，用于上传数据
**
** 输　出:  INT8U * pBuff
** 输　出:  INT16U nLen
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern INT8U  GetFcbBuff(INT8U **ppBuff,INT8U *pLen )
{
	INT8U err = 0;
	INT16U nRet = 0;
	 
	//从flash中读取数据到内存中
//	FLASH_ReadByte(FCB_BLOCK_DESTADDR,FCB_BLOCK,sizeof(FCB) );
//	g_spTmpFcb = (FCB *) FCB_BLOCK ;

	nRet = CheckFcb( g_spTmpFcb,&err );

	*ppBuff = &FCB_BLOCK[0];
//	*ppBuff =  FCB_BLOCK ;

	*pLen = sizeof(FCB);
	return nRet;
	 
}
/*********************************************************************************************************
** 函数名称: GetBinDataBuff
** 函数名称: GetBinDataBuff
**
** 功能描述： 
**
** 输　出:  INT8U * pBuff
** 输　出:  INT32U nLen
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月18日
** 备  注:  BINDATA的文件长度，不能由ram中的fcb决定，必须由flash上面的来决定,
            在调用此函数前必须调用 LoadFcb 从flash加载控制信息
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern INT8U  GetBinDataBuff(INT8U **ppBuff, INT32U *pLen )
{
	INT8U bRet = FALSE;
	INT16U nRet = 0;
	INT8U err = 0;

	//读取BIN数据前，需要读取一下控制块
	nRet = CheckFcb( g_spTmpFcb,&err );
	if (nRet && g_spTmpFcb->nBinLen && g_spTmpFcb->nBinLen<= BIN_BLOCK_SIZE &&g_spTmpFcb->wBinCrc)
	{
		bRet = ReadBinData(g_spTmpFcb->nBinLen,g_spTmpFcb->wBinCrc);
		if (bRet)
		{
			*ppBuff = &BIN_BLOCK[0];
			*pLen = g_spTmpFcb->nBinLen;
			return TRUE;
		}
	}
	return FALSE;
}

extern INT8U  GetSrcDataBuff(INT8U **ppBuff, INT32U *pLen)
{
	INT8U bRet = FALSE;
	INT16U nRet = 0;
	INT8U err = 0;

	//读取BIN数据前，需要读取一下控制块
	nRet = CheckFcb( g_spTmpFcb,&err );
	if (nRet && g_spTmpFcb->nCfgFileLen && g_spTmpFcb->nCfgFileLen<= SRC_BLOCK_SIZE &&g_spTmpFcb->wConfCrc)
	{
		bRet = ReadSrcData(g_spTmpFcb->nCfgFileLen,g_spTmpFcb->wConfCrc);
		if (bRet)
		{
			*ppBuff = SRC_BLOCK;
			*pLen = g_spTmpFcb->nCfgFileLen;
			return TRUE;
		}
	}
	return FALSE;
}
/*********************************************************************************************************
** 函数名称: LoadFlashData
** 函数名称: LoadFlashData
**
** 功能描述： 从FLASH加载数据到ram中
**
**          
** 输　出:  extern INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年11月4日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U LoadFlashData( )
{
	//从flash中读取数据到内存中
	FLASH_ReadByte(FCB_BLOCK_DESTADDR,FCB_BLOCK,FCB_BLOCK_SIZE );
	FLASH_ReadByte(BIN_BLOCK_DESTADDR,BIN_BLOCK,BIN_BLOCK_SIZE );
	FLASH_ReadByte(SRC_BLOCK_DESTADDR,SRC_BLOCK,SRC_BLOCK_SIZE );

	g_spTmpFcb = (FCB *) FCB_BLOCK ;

	return TRUE;
}

/*********************************************************************************************************
** 函数名称: SelfCheckRam
** 函数名称: SelfCheckRam
**
** 功能描述： 内存自检
**
** 输　入:  void
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月24日
** 备  注:   内存自检 这个内存区域是片内？片外？
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:   
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U SelfCheckRam(void) 		 
{
	INT32U i = 0;
	INT8U chk = 0;

	//开始自检
	for (i=0;i<FCB_BLOCK_SIZE;i++) 
	{
		switch(i%4) 
		{
			case 0:
				chk=0xAA;
				break;
			case 1:
				chk=0x55;
				break;
			case 2:
				chk=0xFF;
				break;
			case 3:
				chk=0x00;
				break;
		}
		//写内存
		FCB_BLOCK[i] = chk;		
	}	
	for (i=0;i<BIN_BLOCK_SIZE;i++) 
	{
		switch(i%4) 
		{
		case 0:
			chk=0xAA;
			break;
		case 1:
			chk=0x55;
			break;
		case 2:
			chk=0xFF;
			break;
		case 3:
			chk=0x00;
			break;
		}
		//写内存
		BIN_BLOCK[i] = chk;		
	}
	for (i=0;i<SRC_BLOCK_SIZE;i++) 
	{
		switch(i%4) 
		{
		case 0:
			chk=0xAA;
			break;
		case 1:
			chk=0x55;
			break;
		case 2:
			chk=0xFF;
			break;
		case 3:
			chk=0x00;
			break;
		}
		//写内存
		SRC_BLOCK[i] = chk;		
	}
	for (i=0;i<FCB_BLOCK_SIZE;i++) 
	{
		switch(i%4) 
		{
		case 0:
			chk=0xAA;
			break;
		case 1:
			chk=0x55;
			break;
		case 2:
			chk=0xFF;
			break;
		case 3:
			chk=0x00;
			break;
		}
		//读内存，校验
		if( FCB_BLOCK[i] !=chk)
		{
			return FALSE;
		}
	}	
	for (i=0;i<BIN_BLOCK_SIZE;i++) 
	{
		switch(i%4) 
		{
		case 0:
			chk=0xAA;
			break;
		case 1:
			chk=0x55;
			break;
		case 2:
			chk=0xFF;
			break;
		case 3:
			chk=0x00;
			break;
		}
		//读内存，校验
		if( BIN_BLOCK[i] !=chk)
		{
			return FALSE;
		}
	}	
	for (i=0;i<SRC_BLOCK_SIZE;i++) 
	{
		switch(i%4) 
		{
		case 0:
			chk=0xAA;
			break;
		case 1:
			chk=0x55;
			break;
		case 2:
			chk=0xFF;
			break;
		case 3:
			chk=0x00;
			break;
		}
		//读内存，校验
		if( SRC_BLOCK[i] !=chk)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*********************************************************************************************************
** 函数名称: PutFcbData
** 函数名称: PutFcbData
**
** 功能描述： 往临时缓冲填入数据
**
** 输　入:  INT8U * pBuff
** 输　入:  INT8U nLen
** 输　入:  INT16U nPacketIdx 从1开始
**          
** 输　出:  extern INT8U
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
INT8U PutFcbData(INT8U *pBuff, INT8U nLen,INT16U nPacketIdx )
{
	if (nLen == 0 || pBuff == NULL || nPacketIdx == 0 || nPacketIdx > 256)
		return FALSE;

	if (nPacketIdx == 1)
	{//收到第一个包的时候，复归指针
		FcbBuffPtr = 0;	
		BinBuffPtr = 0;
		SrcBuffPtr = 0;	
	}
	if (FcbBuffPtr+nLen >= FCB_BLOCK_SIZE)
	{//缓冲溢出
		FcbBuffPtr = 0;	
		return FALSE;
	}

	memcpy( &FCB_BLOCK[FcbBuffPtr],pBuff,nLen);
	FcbBuffPtr += nLen;

	return TRUE;
}

INT8U PutBinData(INT8U *pBuff, INT8U nLen,INT16U nPacketIdx )
{
	if (nLen == 0 || pBuff == NULL || nPacketIdx == 0 || nPacketIdx > 512)
		return FALSE;

	if (BinBuffPtr+nLen >= BIN_BLOCK_SIZE)
	{//缓冲溢出
		BinBuffPtr = 0;	
		return FALSE;
	}

	if (nPacketIdx == 1)
	{//第一个包
		BinBuffPtr = 0;
	}

	memcpy( &BIN_BLOCK[BinBuffPtr],pBuff,nLen);
	BinBuffPtr += nLen;

	return TRUE;
}

INT8U PutSrcData(INT8U *pBuff, INT8U nLen,INT16U nPacketIdx )
{
	if (nLen == 0 || pBuff == NULL || nPacketIdx == 0 || nPacketIdx > 512)
		return FALSE;

	if (SrcBuffPtr+nLen >= SRC_BLOCK_SIZE)
	{//缓冲溢出
		SrcBuffPtr = 0;	
		return FALSE;
	}

	if (nPacketIdx == 1)
	{//第一个包
		SrcBuffPtr = 0;
	}

	memcpy( &SRC_BLOCK[SrcBuffPtr],pBuff,nLen);
	SrcBuffPtr += nLen;

	return TRUE;
}
/*********************************************************************************************************
** 函数名称: SaveData
** 函数名称: SaveData
**
** 功能描述： 将RAM中的信息存储到FLASH中
**
**          
** 输　出:  extern INT8U
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
INT8U SaveData( INT8U *err )
{                              
	OS_CPU_SR  cpu_sr;

	FCB *pTmpFcb = NULL;
	INT16U chkcrc = 0;
	INT32U i = 0;
	INT8U ByteValue = 0;
	INT8U nSector = 0;
	//0.检查RAM缓冲的数据长度与FCB是否一致
	pTmpFcb = (FCB *)FCB_BLOCK;
	if (pTmpFcb->wHeadFlag != FCB_FLAG)
	{
		*err = FLASH_API_NO_FCB;
		return FALSE;
	}
	if (pTmpFcb->nBinLen == 0  || pTmpFcb->nBinLen >BIN_BLOCK_SIZE )
	{//BIN 内容是必须下载的
		*err = FLASH_API_NO_BINDATA;
		return FALSE;
	}

	if (pTmpFcb->nCfgFileLen > SRC_BLOCK_SIZE)
	{
		*err = FLASH_API_SRC_LEN_ERR;
		return FALSE;
	}
	if (pTmpFcb->nBinLen != BinBuffPtr)
	{//如果长度不一致
		*err = FLASH_API_BIN_LEN_ERR;
		return FALSE;
	}
	if (pTmpFcb->nCfgFileLen && pTmpFcb->nCfgFileLen != SrcBuffPtr)
	{
		*err = FLASH_API_SRC_LEN_ERR;
		return FALSE;
	}

	//1.检查RAM中的数据CRC是否正确
	chkcrc = GetCRC16( BIN_BLOCK,BinBuffPtr);
	if (chkcrc != pTmpFcb->wBinCrc)
	{
		*err = FLASH_API_BIN_CRC_ERR;
		return FALSE;
	}

	if (pTmpFcb->nCfgFileLen)
	{
		chkcrc = GetCRC16( SRC_BLOCK,SrcBuffPtr);
		if (chkcrc != pTmpFcb->wConfCrc)
		{
			*err = FLASH_API_SRC_CRC_ERR;
			return FALSE;
		}
	}
	//2.检查分区情况，如果长度长，可能得要跨区操作。
	//3.开始写flash
	
	OS_ENTER_CRITICAL();	
	//2.0
	SelSector(0X16, 0X16);					// 选择扇区
	EraseSector(0X16, 0X16);
	SelSector(0X16, 0X16);					// 选择扇区
	RamToFlash(FCB_BLOCK_DESTADDR, (INT32U)FCB_BLOCK, FCB_BLOCK_SIZE);	// 写数据区到FLASH	

	//2.1 BIN 
	SelSector(0X17, 0X17);					// 选择扇区
	EraseSector(0X17, 0X17);
	SelSector(0X17, 0X17);					// 选择扇区
	RamToFlash(BIN_BLOCK_DESTADDR, (INT32U)BIN_BLOCK, BIN_BLOCK_SIZE);	// 写数据区到FLASH	

	if (pTmpFcb->nCfgFileLen)
	{
		//2.2 SRC 
		SelSector(0X18, 0X18);					// 选择扇区
		EraseSector(0X18, 0X18);
		SelSector(0X18, 0X18);					// 选择扇区
		RamToFlash(SRC_BLOCK_DESTADDR, (INT32U)SRC_BLOCK, SRC_BLOCK_SIZE);	// 写数据区到FLASH		 
	}
	OS_EXIT_CRITICAL();	

	//4.从flash返读回来，看是否检验正确，如果检验错误，点亮故障LED
	for (i = 0; i < FcbBuffPtr ; i++)
	{
		ByteValue = (*(volatile unsigned long *)(FCB_BLOCK_DESTADDR+i)); 
		if (ByteValue != FCB_BLOCK[i])
		{
			*err = FLASH_API_FCB_FLASH_ERR;
			return FALSE;
		}
	}

	for (i = 0; i < BinBuffPtr ; i++)
	{
		ByteValue = (*(volatile unsigned long *)(BIN_BLOCK_DESTADDR+i)); 
		if (ByteValue != BIN_BLOCK[i])
		{
			*err = FLASH_API_BIN_FLASH_ERR;
			return FALSE;
		}
	}

	if (pTmpFcb->nCfgFileLen)
	{
		for (i = 0; i < SrcBuffPtr ; i++)
		{
			ByteValue = (*(volatile unsigned long *)(SRC_BLOCK_DESTADDR+i)); 
			if (ByteValue != SRC_BLOCK[i])
			{
				*err = FLASH_API_SRC_FLASH_ERR;
				return FALSE;
			}
		}		 
	}
	//写成功,复归指针
	FcbBuffPtr = 0;
	BinBuffPtr = 0;
	SrcBuffPtr = 0;
	return TRUE;
}

INT32U CheckRam(INT32U nStart,INT32U nEnd)
{
	INT32U i = 0;
	INT8U Val = 0;

	INT8U *pRam = (INT8U *)nStart;

	for ( ; ; )
	{
		if((INT32U)pRam >= nEnd)
			break;

		Val = *pRam;

		*pRam = 0xEA;
		__asm{NOP}   ;
		if ( *pRam != 0xEA )
		{
			*pRam = Val;
			return (INT32U)pRam;
		}
		*pRam = Val;
		pRam++;
	}
	return 0;
}