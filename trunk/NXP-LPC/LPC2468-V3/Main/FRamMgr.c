/****************************************Copyright (c)**************************************************
**                                掉电自保持模块
**     铁电存储前部需要有一个控制块结构，用于保存标志位、掉电寄存器的个数，此外MD和MW的保存区分开                          
**     如果铁电容量够大，可以预留部分空间用于存储录波数据                                  
**
**     时间保存处理？？？？      如果时间容许，可以在2468平台测试。2468，I2C需要增加控制字。                
**     今后还需要预留出接口用于录波保存
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
 
#include "MemData.h" 
#include "FRamMgr.h"
#include "TaskDef.h"
#include "hd_Init.h"
#include "I2c.h" 
#include "ram.h"
#include "TimerMgr.h"
#include "vicControl.h"


#define  FRAM3164   0xA0     //定义器件地址

#define  FRAMCB_FLAG   0x4C49


// 控制块
typedef struct FRAMCB 
{
	INT16U  wHeadFlag; //文件头标志 0x4c 0x49
	//上一次保存的时间
	DEVICE_TIME   SaveTm;			 
	INT16U     nMWCount;	 
	INT16U     nMDCount; 
}FRAMCB;


#define MW_FRAM_ADDR sizeof(FRAMCB)+10
#define MD_FRAM_ADDR MW_FRAM_ADDR+2*MAX_MW_CNT +10


static FRAMCB  FRamCB;
static OS_STK  FRamWRTaskStk [FRAMWR_TASK_STK_SIZE];
/*-------------------------------------------------------*/
// 函数名称：FRam_SaveValue
// 输入参数：INT8U uID:要设置的ID号，以内存中存储结构数据ID为准;
//				Val:设置的值
// 输出参数：无
// 功能描述：修改FRAM中的配置信息
/*-------------------------------------------------------*/
static void FRamSaveValue(INT16U nMWCnt,INT16U nMDCnt,M_DEF *pMemData )
{
	INT16U i = 0;
	INT16U Addr = 0;

	Addr = MW_FRAM_ADDR;

	for (i = 0; i < nMWCnt; i++)
	{
		if (pMemData->m_pMW[i] != pMemData->m_pMWLast[i])
		{//写入铁电
			Addr += i*2;
			i2cWrite(1, FRAM3164, (INT8U *) & pMemData->m_pMW[i] ,  Addr,  1, sizeof(INT16U));
			while (i2cGetFlag(1) == I2C_BUSY);  
		}
	}

	Addr = MD_FRAM_ADDR;

	for (i = 0; i < nMDCnt; i++)
	{
		if (pMemData->m_pMD[i] != pMemData->m_pMDLast[i])
		{//写入铁电
			Addr += i*4;
			i2cWrite(1, FRAM3164, (INT8U *) & pMemData->m_pMW[i] ,  Addr,  1, sizeof(INT32U));
			while (i2cGetFlag(1) == I2C_BUSY);  
		}
	}
	//更新FRAMCB的值

}
/*********************************************************************************************************
** 函数名称: FRamSave_Task
** 函数名称: FRamSave_Task
**
** 功能描述： 定时存储任务
**
** 输　入:  void * pdata
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
void FRamSave_Task(void *pdata)
{
	INT16U nTimerId = 2;
	INT8U bRet = FALSE;

	pdata = pdata;

	//1.从FRAM里面读取FRAMCB
	i2cRead( 1,	FRAM3164,(INT8U *) & FRamCB,0, 1,sizeof(FRAMCB));    
    // 等待I2C总线操作结束        
 	while (i2cGetFlag(1) == I2C_BUSY);                                

	//2.判断掉电前的保存的MW MD的数量是否与现在的一致
	if (FRamCB.wHeadFlag == FRAMCB_FLAG)
	{//如果铁电上面的控制块有效
		if (FRamCB.nMWCount && FRamCB.nMDCount && FRamCB.nMWCount <= MAX_MW_CNT && FRamCB.nMDCount <= MAX_MD_CNT)
		{//个数是合法的
			if (GetFRamMWCnt() && GetFRamMDCnt() && GetFRamMWCnt() <= MAX_MW_CNT && GetFRamMDCnt() <= MAX_MD_CNT)
			{ 
				//防止铁电上面的垃圾数据
				if ( GetFRamMWCnt()<= FRamCB.nMWCount )
				{//，

				}
				else
				{//如果掉电存储的个数超过预期，则只加载有用部分，剩下的部分则不管，等待第一次的写入

				}
				if (GetFRamMDCnt() <= FRamCB.nMDCount)
				{

				}
			}
		}
	}
	else
	{//写入控制块
		FRamCB.wHeadFlag = FRAMCB_FLAG;
		FRamCB.nMDCount = GetFRamMDCnt();
		FRamCB.nMWCount = GetFRamMWCnt( );
		//时间


		i2cWrite(1, FRAM3164, (INT8U *) & FRamCB,  0,  1, sizeof(FRAMCB));
		while (i2cGetFlag(1) == I2C_BUSY);                              
	}

	if (GetFRamMWCnt() && GetFRamMDCnt() && GetFRamMWCnt() <= MAX_MW_CNT && GetFRamMDCnt() <= MAX_MD_CNT)
	{
		//500ms保存一次
		SetTimer(nTimerId,500,NULL,NULL);
		while(1)
		{
			if (GetFRamMWCnt() && GetFRamMDCnt() && GetFRamMWCnt() <= MAX_MW_CNT && GetFRamMDCnt() <= MAX_MD_CNT)
			{
				bRet = IsTimeTo(nTimerId);
				if (bRet)
				{
					FRamSaveValue( GetFRamMWCnt(),GetFRamMDCnt() , GetMDataPtr()  );
				}
			}
		    OSTimeDlyHMSM(0,0,0,100);		 
		}
	}
	else
	{
	    OSTimeDlyHMSM(0,0,0,200);		
	}	
}
/*********************************************************************************************************
** 函数名称: InitFRamWR
** 函数名称: InitFRamWR
**
** 功能描述： 系统初始化时，需要看看FRAM里面是否有存储过往的数据，如果有，而且匹配，则加载
**
**          
** 输　出:  INT8U
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
extern INT8U  InitFRamMgrTsk( void )
{
	//0.启动硬件部分
	//I2C中断设置                
	SetVICIRQ(VIC_I2C2, 6, (int)i2c1IRQ);                                      
    //初始化I2C0      
	I2CInit(1, 100000,NULL);                                  

	 //启动定时写任务		 
	OSTaskCreate (FRamSave_Task, (void *)0, &FRamWRTaskStk[FRAMWR_TASK_STK_SIZE-1], PRIO_FRAM_SAVE);
	 
	return TRUE;
}
