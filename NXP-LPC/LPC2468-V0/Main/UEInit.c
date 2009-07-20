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
#include "includes.h" 
#include "hd_Init.h"
#include "uart.h" 
#include "ram.h"
#include "hd_Init.h"
#include "Viccontrol.h"


/*********************************************************************************************************
** 函数名称: InitCom
** 函数名称: InitCom
**
** 功能描述： 根据配置信息初始化硬件
**
** 输　入:  MB_PORT * pPort  如果此参数为空[没有配置信息]，则采用默认配置进行初始化
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
extern void IRQASMUart0(void);
extern void IRQASMUart3(void);

static void InitCom( MB_PORT *pPort )
{
	INT32U nBaud = 0;
	UART_MODE uart;	
	UART_PARAM UartParam;
	
	if (pPort == NULL)
		return;	
	else
	{
		poeUart0ReviceSem = NULL;  
		poeUart3ReviceSem = NULL;  

		UartParam.bps=9600;        //波特率
		UartParam.datab=8;         // 字长度，5/6/7/8
		UartParam.stopb=1;         // 停止位，1/2
		UartParam.parity=0;    	  // 奇偶校验位，0为无校验，1奇数校验，2为偶数校验
 
		//串口初始化
	//	UART0_Init(9600,&uart);
		InitUart( UART0, &UartParam,NULL);
		SetVICIRQ(VIC_UART0, 9, (INT32U)IRQASMUart0);
//		DisableVICIRQ(VIC_UART0);

		UartParam.bps = pPort->Baud;        //波特率
		UartParam.datab = pPort->DataBit;         // 字长度，5/6/7/8
		UartParam.stopb = 1;//pPort->StopBit;         // 停止位，1/2
		UartParam.parity = pPort->ParityBit;      // 奇偶校验位，0为无校验，1奇数校验，2为偶数校验
		InitUart( UART3, &UartParam,NULL);
		SetVICIRQ(VIC_UART3, 8, (INT32U)IRQASMUart3);

//	 	DisableVICIRQ(VIC_UART3); 
// 		nBaud = pPort->Baud;
// 		uart.datab = pPort->DataBit;
// 		uart.stopb = pPort->StopBit + 1;
// 		uart.parity = pPort->ParityBit;
// 		UART3_Init(nBaud ,&uart);
		return;
	}
}
/*********************************************************************************************************
** 函数名称: LoadConfData
** 函数名称: LoadConfData
**
** 功能描述： 从flash加载配置信息
**
** 输　入:  CRamMain * pRam
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
static INT8U LoadConfData(CRamMain *pRam)
{
	INT8U err = 0;
	INT16U nRet = 0;
	INT8U bRet = FALSE;
	INT8U *pBuff = NULL;
	INT32U nDataLen = 0; 
	INT32U nRetLen = 0;

	if (pRam == NULL)
		return FALSE;

 //	LoadFlashData();
	
	//加载FCB
	nRet = LoadFcb( & pRam->Fcb,&err);
	if (nRet < sizeof(FCB) || nRet == 0)
		return FALSE;

	//加载BIN文件到内存中
	bRet = GetBinDataBuff(&pBuff,&nDataLen );
	if (bRet == FALSE)
		return FALSE;
	//解析BIN
	nRetLen = LoadBinData(pBuff, &pRam->BinData );
	if (nRetLen != nDataLen)
		return FALSE;

	return TRUE;
}
/*********************************************************************************************************
** 函数名称: InitConfData
** 函数名称: InitConfData
**
** 功能描述： 初始化配置信息，先从flash加载，如果加载失败，则以默认数据填入
**
** 输　入:  CRamMain * pRam
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
extern INT8U InitConfData(CRamMain *pRam)
{
	INT8U bRet = FALSE;

//	if (pRam == NULL)
//		return FALSE;

	bRet = LoadConfData(pRam);
	if (bRet == FALSE)
	{//加载失败  启用默认配置
		InitDefaultBinData(& pRam->BinData);
	}
	else
	{//成功

	}

	//初始化内存
	InitMemData();

	//设置装置
	SetDeviceAddr( pRam->BinData.HdCfg.CpuCfg.Port.Addr );
	
	//根据配置启动相关的硬件

	//启动通信口
 	InitCom( & pRam->BinData.HdCfg.CpuCfg.Port );

	return bRet;
}