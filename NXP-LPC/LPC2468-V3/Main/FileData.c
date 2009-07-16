/****************************************Copyright (c)**************************************************
**                                文件解析模块 [目前加载配置过程中，未对数据有效性进行检查]
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
**备        注: 
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
#include "FileData.h"
#include "modbus.h"



/*********************************************************************************************************
** 函数名称: LoadHdCfg
** 函数名称: LoadHdCfg
**
** 功能描述：加载硬件配置信息
**
** 输　入:  INT8U * pBuf
** 输　入:  SEC_HD_CFG * pCfg
**          
** 输　出:  INT32U
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
static INT32U LoadHdCfg(INT8U *pBuf,SEC_HD_CFG *pCfg)
{
	INT32U nBufPos = 0;
	INT32U i = 0;

	if (pBuf == NULL)
		return nBufPos;
	if (pCfg == NULL)
		return nBufPos;

	memcpy(& pCfg->CpuCfg, &pBuf[nBufPos],sizeof(CPU)); 
	nBufPos += sizeof(CPU);

	memcpy(& pCfg->ModuleCnt, &pBuf[nBufPos],sizeof(INT32U)); 
	nBufPos += sizeof(INT32U);

	memset(& pCfg->pIoCfg,0,sizeof(IO)*MAX_MODULE_CNT);
	if (pCfg->ModuleCnt >0 && pCfg->ModuleCnt <= MAX_MODULE_CNT)
	{
		for (i = 0; i < pCfg->ModuleCnt; i++)
		{
			memcpy(& pCfg->pIoCfg[i], &pBuf[nBufPos],sizeof(IO)); 
			nBufPos += sizeof(IO);
		}
	}

	return nBufPos;
}
/*********************************************************************************************************
** 函数名称: LoadGatageCfg
** 函数名称: LoadGatageCfg
**
** 功能描述： 加载闸门分段参数等
**
** 输　入:  INT8U * pBuf
** 输　入:  SEC_GATAGE_CFG * pCfg
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月1日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT32U LoadGatageCfg(INT8U *pBuf,SEC_GATAGE_CFG *pCfg)
{
	INT32U nBufPos = 0;
	INT32U i = 0;

 	if (pBuf == NULL)
		return nBufPos;
	if (pCfg == NULL)
		return nBufPos;

	memcpy(& pCfg->MaxGatage, &pBuf[nBufPos],sizeof(FP64)); 
	nBufPos += sizeof(FP64);

	memcpy(& pCfg->MinGatage, &pBuf[nBufPos],sizeof(FP64)); 
	nBufPos += sizeof(FP64);

	memcpy(& pCfg->MaxEncoderVal, &pBuf[nBufPos],sizeof(INT32U)); 
	nBufPos += sizeof(INT32U);

	memcpy(& pCfg->MinEncoderVal, &pBuf[nBufPos],sizeof(INT32U)); 
	nBufPos += sizeof(INT32U);

	memcpy(& pCfg->UnitType, &pBuf[nBufPos],sizeof(INT8U)); 
	nBufPos += sizeof(INT8U);

	memcpy(& pCfg->CalcMethod, &pBuf[nBufPos],sizeof(INT8U)); 
	nBufPos += sizeof(INT8U);

	memcpy(& pCfg->Direction, &pBuf[nBufPos],sizeof(INT8U)); 
	nBufPos += sizeof(INT8U);

	memcpy(& pCfg->EncodeType, &pBuf[nBufPos],sizeof(INT8U)); 
	nBufPos += sizeof(INT8U);

	memcpy(& pCfg->ParaCnt, &pBuf[nBufPos],sizeof(INT32U)); 
	nBufPos += sizeof(INT32U);

	memset(  pCfg->pGatageTab,0,sizeof(GATAGE_PARA) *MAX_GATAGE_CNT );

	if (pCfg->ParaCnt >0 && pCfg->ParaCnt <= MAX_GATAGE_CNT)
	{
		for (i = 0; i < pCfg->ParaCnt; i++)
		{
			memcpy(& pCfg->pGatageTab[i], &pBuf[nBufPos],sizeof(GATAGE_PARA)); 
			nBufPos += sizeof(GATAGE_PARA);
		}
	}

	return nBufPos;
}
/*********************************************************************************************************
** 函数名称: LoadBinData
** 函数名称: LoadBinData
**
** 功能描述：从配置文件中提取配置信息
**
** 输　入: INT8U 内存中
**          
** 输　出: INT8U 错误返回FALSE，正确返回TURE
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  
** 日　期:  
** 备  注: 先从flash中加载数据到ram，然后解析ram的数据
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT32U LoadBinData(INT8U *pBuf,BIN_DATA *pBinData)
{
	INT32U nBufPos = 0;
	INT32U nLen = 0;

	//1.加载硬件配置信息
	nLen = LoadHdCfg(&pBuf[nBufPos], &pBinData->HdCfg);
	if (nLen == FALSE)
		return FALSE;
	nBufPos += nLen;

	//2.加载闸门配置信息
	nLen = LoadGatageCfg(&pBuf[nBufPos], &pBinData->GatageCfg);
	if (nLen == FALSE)
		return FALSE;
	nBufPos += nLen;

	return nBufPos;
}

/*********************************************************************************************************
** 函数名称: InitDefaultHdCfg
** 函数名称: InitDefaultHdCfg
**
** 功能描述： 默认的配置信息
**
** 输　入:  SEC_HD_CFG * pHdCfg
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月19日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void InitDefaultHdCfg(SEC_HD_CFG *pHdCfg)
{
	if (pHdCfg == NULL)
		return;

	pHdCfg->CpuCfg.MDCnt = 0;
	pHdCfg->CpuCfg.MWCnt = 0;

	pHdCfg->CpuCfg.Port.Addr = MAX_MB_ADDR;
	pHdCfg->CpuCfg.Port.Baud = 9600;

	pHdCfg->CpuCfg.Port.StopBit = ONE_STOPBIT;
	pHdCfg->CpuCfg.Port.DataBit = 8;
 	pHdCfg->CpuCfg.Port.ParityBit = NONE_PARITY;

	pHdCfg->ModuleCnt = 0;
	memset(pHdCfg->pIoCfg,0,sizeof(IO)*MAX_MODULE_CNT);	
}
static void InitDefaultGatageCfg(SEC_GATAGE_CFG *pGatageCfg)
{
	if (pGatageCfg == NULL)
		return;

	pGatageCfg->MaxGatage = 10*100;    //最大闸门开度 10米
	pGatageCfg->MinGatage = 0;    //最小闸门开度 0米

	pGatageCfg->MaxEncoderVal = 4096;    //最大闸门开度对应的编码器值
	pGatageCfg->MinEncoderVal = 96;    //最小闸门开度

	pGatageCfg->UnitType  = UNIT_CM;      //单位
	pGatageCfg->CalcMethod = CALC_BY_LINE;    //计算方式
	pGatageCfg->Direction = DIRE_CLOCKWISE;     //旋转方向
	pGatageCfg->EncodeType = CODE_GRAY;    //编码类型，只对绝对编码器有效

	pGatageCfg->ParaCnt = 0;  //分段参数个数
	memset( pGatageCfg->pGatageTab,0,sizeof(GATAGE_PARA)*MAX_GATAGE_CNT );
}

void InitDefaultBinData(BIN_DATA *pBinData)
{
	InitDefaultHdCfg(& pBinData->HdCfg);
	InitDefaultGatageCfg( & pBinData->GatageCfg );
}
/*********************************************************************************************************
** 函数名称: InitDefaultFcb
** 函数名称: InitDefaultFcb
**
** 功能描述： 
**
**          
** 输　出:  void
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
void InitDefaultFcb( FCB *pFcb)
{
	if (pFcb == NULL)
		return;

	pFcb->wHeadFlag = FCB_FLAG;

	pFcb->wMajorVer = 1; //主版本号  如：1.00 :wMajorVer = 1 wMinorVer= 0，2.01 = wMajorVer = 2 wMinorVer= 1 主版本号和应用程序有关 
	pFcb->wMinorVer = 0; //从版本号，
	pFcb->wBuild = 0;    //由文件保存时产生，每保存一次，版本号增加1	

	pFcb->wBinBuild = 0;    //如果键盘修改配置，每修改一次，此版本号追加一次 	

	pFcb->DownLoadTm.wYear = 2008;
	pFcb->DownLoadTm.ucMonth = 8;
	pFcb->DownLoadTm.ucDay = 8;
	pFcb->DownLoadTm.ucHour = 8;
	pFcb->DownLoadTm.ucMinute = 8;
	pFcb->DownLoadTm.wSecond = 8;
	pFcb->DownLoadTm.wMilliseconds = 8;

	pFcb->SaveTm.wYear = 2008; 
	pFcb->SaveTm.ucMonth = 8; 		
	pFcb->SaveTm.ucDay = 8; 		
	pFcb->SaveTm.ucHour = 8; 		
	pFcb->SaveTm.ucMinute = 8; 		
	pFcb->SaveTm.wSecond = 8; 	
	pFcb->SaveTm.wMilliseconds = 8 ;		

	//源文件的长度
	pFcb->nCfgFileLen = 0;
	//源文件的crc校验
	pFcb->wConfCrc = 0xffff;

	//编译后文件长度
	pFcb->nBinLen = 0;     
	pFcb->wBinCrc = 0xffff;

	pFcb->nCtrlBit = 0;       // bit 0  
}
