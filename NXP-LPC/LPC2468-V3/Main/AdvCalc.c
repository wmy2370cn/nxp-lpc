/****************************************Copyright (c)**************************************************
**                                       高级计算任务
**                                      
**          通过采集当前的编码器的输出值，根据闸门的类型，计算出闸门的对应开度                            
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
#include "FileData.h"
#include "AdvCalc.h"
#include "MemData.h"
#include "TimerMgr.h"
 
#include "IoAdapter.h"
#include "FileData.h"
#include "Ram.h"
#include "IoAdapter.h"
#include "TaskDef.h"


//闸门开度计算相关
typedef struct GATAGE_CALC_PARA
{
	FP64	k;				//平板门(直拉门)斜率
	FP64	b;				//修正值
	GATAGE_PARA  	*pArcPoint;
	INT32U  slopcnt;
	FP64	slop[MAX_GATAGE_CNT-1];		//弧形门分段计算各段斜率
}GATAGE_CALC_PARA;


static GATAGE_CALC_PARA  GatagePara;
static OS_STK	AdvCalcTaskStk  [ADVCALC_TASK_STK_SIZE];

/***********************************************************************************
**函数原型：INT16U GraytoBinary(INT16U x)
**入口参数:	INT16U	x:16位格雷码值
**出口参数:	INT16U	y:16位二进制值
**返 回 值：		
**说    明：格雷码转换为二进制码
************************************************************************************/
static INT32U GraytoBinary(INT32U x_INT32U)
{
    INT32U y = 0;
    y = x_INT32U;
    while(x_INT32U>>=1)
         y ^= x_INT32U;
    return y;
}   
/*********************************************************************************************************
** 函数名称: GetParameter
** 函数名称: GetParameter 不对外开放，只在本模块使用
**
** 功能描述： 计算闸门开度相关参数：斜率等
**
** 输　入:  CGatagePara * pPara
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月1日
** 备  注:  系统初始化时，计算出来所有相关的参数，在运行时，不做参数计算。
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U InitGatagePara (GATAGE_CALC_PARA *pPara,SEC_GATAGE_CFG *pGatage)
{
	INT8U	i = 0;
	INT32U	calcdata = 0;	
	INT8U  CalcMethod = 0;
	
	if (pPara == NULL)
		return FALSE;
	if (pGatage == NULL)
		return FALSE;

	CalcMethod = GetGatageCalcMethod( ) ;
	
	if ( CalcMethod == CALC_BY_LINE)
	{//如果是线性计算
		calcdata = pGatage->MaxEncoderVal - pGatage->MinEncoderVal;
		if (calcdata == 0)
			return FALSE;

		pPara->k = (pGatage->MaxGatage - pGatage->MinGatage)/calcdata;
		pPara->b = (pGatage->MinGatage * pGatage->MaxEncoderVal - pGatage->MaxGatage* pGatage->MinEncoderVal ) /calcdata;
		return TRUE;
	}
	else if ( CalcMethod == CALC_BY_TAB )
	{
		if (pGatage->ParaCnt<=3 || pGatage->ParaCnt > MAX_GATAGE_CNT || pGatage->pGatageTab == NULL)
		{
			return FALSE;
		}
		pPara->pArcPoint = pGatage->pGatageTab;
		pPara->slopcnt = pGatage->ParaCnt-1;

		for ( i = 0; i < pGatage->ParaCnt-1; i++)
		{
			calcdata = pGatage->pGatageTab[i+1].EncoderVal - pGatage->pGatageTab[i].EncoderVal;
			if(calcdata == 0)
				return FALSE;
			pPara->slop[i] = (FP32)(pPara->pArcPoint[i+1].Gatage - pPara->pArcPoint[i].Gatage) / calcdata;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
} 
/*********************************************************************************************************
** 函数名称: CalcGatage
** 函数名称: 不对外开放，只在本模块使用
**
** 功能描述： 计算闸门开度值
**
** 输　入:  INT32U EncoderVal
** 输　入:  GATAGE_CALC_PARA * pPara
** 输入输出:  fp32 * pfGatageVal 计算的闸门开度
**          
** 输　出:  INT8U 计算是否成功
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月2日
** 备  注:  假定认为此时输入的编码值有效，编码值的合法性在其他的地方做校验
            计算出来的开度值是否正确，也不在此处校验
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U CalcGatage(INT32U EncoderVal,GATAGE_CALC_PARA *pPara,FP32 *pfGatageVal)
{
	INT8U  CalcMethod = 0;
	INT8U  i = 0;
 	INT8U  Direction ;

	if (pPara == NULL)
		return FALSE;
	if (pfGatageVal == NULL)
		return FALSE;

	CalcMethod = GetGatageCalcMethod() ;

	if ( CalcMethod == CALC_BY_LINE)
	{//如果是线性计算
		*pfGatageVal = (FP32)(pPara->k * EncoderVal) + pPara->b;
		return TRUE;
	}
	else if ( CalcMethod == CALC_BY_TAB)
	{
		if (pPara->pArcPoint == NULL)
			return FALSE;

		Direction = GetEncoderDirection();

		if (Direction == DIRE_CLOCKWISE)  //顺时针方向
		{
			if (EncoderVal >=pPara->pArcPoint[0].EncoderVal && EncoderVal < pPara->pArcPoint[pPara->slopcnt].EncoderVal )
			{//在范围内
				for (i = 0; i < pPara->slopcnt-1; i++)
				{//找出所属的区段
					if (EncoderVal >=pPara->pArcPoint[i].EncoderVal && EncoderVal < pPara->pArcPoint[i+1].EncoderVal)
					{
						*pfGatageVal = pPara->pArcPoint[i].Gatage + pPara->slop[i]*(EncoderVal-pPara->pArcPoint[i].EncoderVal);
						return TRUE;
					}
				}
			}
			else if (EncoderVal < pPara->pArcPoint[0].EncoderVal )
			{//在零点以下，为负值
				*pfGatageVal = pPara->pArcPoint[0].Gatage + pPara->slop[0]*(EncoderVal-pPara->pArcPoint[0].EncoderVal);
				return TRUE;
			}
			else if (EncoderVal >= pPara->pArcPoint[pPara->slopcnt].EncoderVal )
			{//超过上限范围
				*pfGatageVal = pPara->pArcPoint[pPara->slopcnt].Gatage;
				return TRUE;
			}			 		
		}
		else if ( Direction == DIRE_ANTI_CLOCKWISE ) //逆时针方向
		{//零点值是最大的值
			if (EncoderVal <= pPara->pArcPoint[0].EncoderVal && EncoderVal >pPara->pArcPoint[pPara->slopcnt].EncoderVal )
			{//在范围内
				for (i = 0; i < pPara->slopcnt-1; i++)
				{//找出所属的区段
					if (EncoderVal <=pPara->pArcPoint[i].EncoderVal && EncoderVal > pPara->pArcPoint[i+1].EncoderVal)
					{
						*pfGatageVal = pPara->pArcPoint[i+1].Gatage + pPara->slop[i]*(EncoderVal-pPara->pArcPoint[i+1].EncoderVal);
						return TRUE;
					}
				}
			}
			else if (EncoderVal > pPara->pArcPoint[0].EncoderVal )
			{//在零点以下，为负值
				*pfGatageVal = pPara->pArcPoint[0].Gatage + pPara->slop[0]*(EncoderVal-pPara->pArcPoint[0].EncoderVal);
				return TRUE;
			}
			else if (EncoderVal <= pPara->pArcPoint[pPara->slopcnt].EncoderVal )
			{//超过上限范围
				*pfGatageVal = pPara->pArcPoint[pPara->slopcnt].Gatage;
				return TRUE;
			}		
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
} 
/*********************************************************************************************************
** 函数名称: AdvCalcTask
** 函数名称: AdvCalcTask
**
** 功能描述： 高级计算任务，目前只计算开度，今后可以把流量计算也合并在内
**
** 输　入:  void * pdata
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月22日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 目前暂时不支持从模拟量输入，暂时只做从MD采集编码器的信号
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void AdvCalcTask(void *pdata)
{
	FP32 fValue = 0.0f;
	INT16U nTimerId = 3;
	INT8U bRet = FALSE;
	INT32U EncoderVal = 0;
	INT32U LastEncoderVal = 0xff;
	INT8U bTimeTo = FALSE;

	pdata = pdata;

	//1000ms计算一次
	SetTimer(nTimerId,1000,NULL,NULL);
 
	while(1)
	{
		bTimeTo = IsTimeTo(nTimerId);
		if (bTimeTo)
		{
			//编码值固定存放在%MD0001
			EncoderVal = GetMDVal(0);
			if (LastEncoderVal != EncoderVal)
			{
				LastEncoderVal = EncoderVal;
				bRet = CalcGatage(EncoderVal,&GatagePara ,&fValue);
				if (bRet == FALSE)
				{//置系统错误位					

				}
				else
				{//清系统错误位

				}
			}
		}	 
		OSTimeDlyHMSM(0,0,0,300);
	}
}

/*********************************************************************************************************
** 函数名称: InitGatageTsk
** 函数名称: InitGatageTsk
**
** 功能描述： 
**
** 输　入:  void
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月24日
** 备  注: 今后考虑合并到主任务里面
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U InitGatageTsk( void )
{
	INT8U bRet = FALSE;

	bRet = InitGatagePara(&GatagePara,&Ram.BinData.GatageCfg);
	if (bRet == FALSE)
	{
		SetLed(LED_FAULT,TRUE);
		return FALSE;
	}

	//启动任务
	OSTaskCreate (AdvCalcTask, (void *)0, 	&AdvCalcTaskStk[ADVCALC_TASK_STK_SIZE-1], GATAGECALC_TASK_PRIO);
	return bRet;
}
