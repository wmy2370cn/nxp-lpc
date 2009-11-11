/******************************************************************************
模块名      :  
文件名      :   
相关文件    :
实现功能    : 寄存器的读写操作接口
修改记录    : 目前只是做了一部分概念上的定义
              需要增加状态字中的位的获取与修改接口实现
------------------------------------------------------------------------------
日期        修改人      修改内容
2007/9/16
******************************************************************************/

#include <string.h>       // Keil: Line added	 
#include "MemData.h" 
#include "Func.h"

static 	MEMDB g_MemData; 

/*********************************************************************************************************
** 函数名称: SetPIBit
** 函数名称: SetPIBit
**
** 功能描述： 输入开关量 以bit方式输入 nIdx 从0开始
**
** 输　入:  INT8U Val
** 输　入:  INT16U nIdx
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年6月13日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SetPIBit(INT8U Val, INT16U nIdx )
{
	INT16U  no, bias; 

	if (nIdx < MAX_I_CNT)
	{
		no= nIdx/8; 
		bias=nIdx%8;

		g_MemData.IData.RawData[no] = ( ( g_MemData.IData.RawData[no] & (ClrBit(bias)) ) | ((!!Val)<<bias)  ); 	
 	}
}

/*********************************************************************************************************
** 函数名称: SetPIW
** 函数名称: SetPIW
**
** 功能描述： 
**
** 输　入:  INT16U Val
** 输　入:  INT16U nIdx
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月4日
** 备  注:  在中断里面，由硬件驱动层调用
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SetPIW( INT16U Val,INT16U nIdx)
{
	if (nIdx<MAX_AI_CNT)
	{
		g_MemData.AIData.RawData[nIdx] = Val;
	}
}

INT8U GetIVal(INT16U OccNo)
{
	return	( !!( g_MemData.IData.State[(OccNo-1)/8] & OrBit((OccNo-1)%8) ) );
}

INT8U GetIForceMark(INT16U OccNo )
{
	return	( !!( g_MemData.IData.ForceMark[(OccNo-1)/8] & OrBit((OccNo-1)%8) ) );
}


//强制I寄存器
INT8U ForceI( INT16U OccNo, INT16U value ) 
{ 
	INT16U  no, bias; 
	if( OccNo<=MAX_I_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8; 
		bias=(OccNo-1)%8;
		g_MemData.IData.State[no]=( ( g_MemData.IData.State[no] & (ClrBit(bias)) ) | ((!!value)<<bias) ); 
		g_MemData.IData.ForceMark[no] = g_MemData.IData.ForceMark[no] | OrBit(bias); 
//		Ram.CurrentMeasure.Stu.udigital_in = Ram.CurrentMeasure.Stu.udigital_in | ((!!value)<<(OccNo-1));
		return TRUE;
	}  
	return FALSE;
}

//解除强制
INT8U UnForceI( INT16U OccNo )
{ 
	INT16U  no, bias; 
	if( OccNo<=MAX_I_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8; 
		bias=(OccNo-1)%8;
		g_MemData.IData.State[no]=(  ( g_MemData.IData.State[no] & (ClrBit(bias)) ) | ( g_MemData.IData.RawData[no] & OrBit(bias) ) ); 
		g_MemData.IData.ForceMark[no] = g_MemData.IData.ForceMark[no] & (ClrBit(bias)); 
		return TRUE;
	} 
	return FALSE;
}

//置强制标志
INT8U SetIForceMark( INT16U OccNo, INT16U value )
{ 
	INT16U  no, bias; 
	if( OccNo<=MAX_I_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8; 
		bias=(OccNo-1)%8;
		g_MemData.IData.ForceMark[no]=( (g_MemData.IData.ForceMark[no] & (ClrBit(bias)) ) | ((!!value)<<bias) );
		return TRUE;
	} 
	return FALSE;
}

/* -------------------------------  Q  -----------------------------------------
///////////////////////////////////////////////////////////////
Input :
occno : 1 -- MAX_Q
///////////////////////////////////////////////////////////////
----------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
** 函数名称:  GetQVal
** 功能描述:  取开出值，由逻辑扫描任务、通讯任务调用
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年7月26日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------*/
INT8U GetQVal(INT16U OccNo )
{
	return	( !!( g_MemData.QData.State[(OccNo-1)/8] & OrBit((OccNo-1)%8) ) );	 
}

/*-------------------------------------------------------------------------
** 函数名称: SetPulseQValue
** 功能描述:  开出输出  提供给通讯、逻辑等模块用的
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年7月26日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------*/
INT8U SetPulseQValue( INT16U OccNo, INT8U Val,INT16U PulseTime )
{
	INT16U  no, bias; 

	if( OccNo<=MAX_Q_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8;
		bias=(OccNo-1)%8;
		if(!(g_MemData.QData.ForceMark[no]&OrBit(bias))) 
		{ //set val
			if(Val==0 && PulseTime!=HOLD_TYPE && PulseTime!=HOLD_TYPE1 ) 
				return FALSE;
			g_MemData.QData.Output[no]=(  ( g_MemData.QData.Output[no] & (ClrBit(bias)) ) | ((!!Val)<<bias)  ); 
			/*overwrite State Right now*/
			//g_MemReg.QData.State[no]=(  ( g_MemReg.QData.State[no] & (ClrBit(bias)) ) | ((!!Val)<<bias)  ); 
			if( PulseTime==HOLD_TYPE || PulseTime==HOLD_TYPE1 )
				g_MemData.QData.PulseTime[OccNo-1]=0; 
			else
			{
				g_MemData.QData.PulseTime[OccNo-1]=PulseTime; 
			}

			//g_MemReg.QData.OutputEna[no] =g_MemReg.QData.OutputEna[no] | OrBit(bias); 
			//if(PulseTime==HOLD_TYPE)
			//	g_MemReg.QData.CheckEna [no] =  g_MemReg.QData.CheckEna[no] | OrBit(bias); 
			//else
			//    g_MemReg.QData.CheckEna [no] = g_MemReg.QData.CheckEna [no] & (ClrBit(bias)); 
			//return TRUE;
		} 
		return TRUE;
	} 
	return FALSE;
}
/*-------------------------------------------------------------------------
** 函数名称: ForceQValue
** 功能描述:  开出强制，由调试软件使用
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年7月26日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------*/
INT8U ForceQ(INT16U OccNo, INT16U Val)
{
	INT16U  no, bit; 
	if( OccNo<=MAX_Q_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8; 
		bit=(OccNo-1)%8;

		g_MemData.QData.ForceMark[no] = g_MemData.QData.ForceMark[no] | OrBit(bit); 
		g_MemData.QData.Output[no]=(  ( g_MemData.QData.Output[no] & (ClrBit(bit)) ) | ((!!Val)<<bit)  ); 
		/*overwrite State Right now*/
		//g_MemReg.QData.State[no]= (  ( g_MemReg.QData.State[no] & (ClrBit(bit)) ) | ((!!Val)<<bit)  ); 
		g_MemData.QData.PulseTime[OccNo-1]=HOLD_TYPE; 
		//  g_MemReg.QData.OutputEna[no] = g_MemReg.QData.OutputEna[no] | OrBit(bit); 
		//  g_MemReg.QData.CheckEna [no] = g_MemReg.QData.CheckEna [no] | OrBit(bit); 
		return TRUE;
	}
	return FALSE;
}
/*-------------------------------------------------------------------------
** 函数名称: UnForceQ
** 功能描述:  取消开出强制，由调试软件使用
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年7月26日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------*/
INT8U UnForceQ( INT16U OccNo )
{ 
	INT16U  no, bit; 
	if( OccNo<=MAX_Q_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8; 
		bit=(OccNo-1)%8;
		if(g_MemData.QData.ForceMark[no] & OrBit(bit) )
		{
			//	g_MemReg.QData.OutputEna[no] = g_MemReg.QData.OutputEna[no] & (ClrBit(bit)); 
			//	g_MemReg.QData.CheckEna [no] = g_MemReg.QData.CheckEna [no] & (ClrBit(bit)); 
			g_MemData.QData.PulseTime[OccNo-1]=0; 
			g_MemData.QData.ForceMark[no] = g_MemData.QData.ForceMark[no] & (ClrBit(bit)); 
		}
		return TRUE;
	} 
	return FALSE;
}
/*-------------------------------------------------------------------------
** 函数名称: SetForceMarkQ
** 功能描述:  设置开出强制标志，由调试软件使用
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年7月26日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------*/
INT8U SetQForceMark( INT16U OccNo, INT16U value )
{ 
	INT16U  no, bias; 
	if( OccNo<=MAX_Q_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8; 
		bias=(OccNo-1)%8;
		g_MemData.QData.ForceMark[no]=( ( g_MemData.QData.ForceMark[no] & (ClrBit(bias)) ) | ((!!value)<<bias) );
		return TRUE;
	} 
	return FALSE;
}
/*-------------------------------------------------------------------------
** 函数名称:  GetQForceMark
** 功能描述:  取开出强制标志，由调试软件使用
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年7月26日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------*/
INT8U GetQForceMark( INT16U OccNo)
{
	return ( !!( g_MemData.QData.ForceMark[(OccNo-1)/8] & OrBit((OccNo-1)%8) ) ) ;
}

/*********************************************************************************************************
** 函数名称:  CheckPulseQ
** 功能描述:   检查脉冲型开出，等到脉冲时间到后，自动给output清0 该函数在10ms调用一次，检查
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年7月26日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#define  MS_1TICK 20
void CheckPulseQ( INT16U OccNo, INT32U TickSum )
{
	if (OccNo>0 && OccNo<=MAX_Q_CNT)
	{
		if( g_MemData.QData.PulseTime[OccNo] && (g_MemData.QData.Output[OccNo/8] & OrBit(OccNo%8)) ) 
		{
			if(g_MemData.QData.PulseTime[OccNo] >= (MS_1TICK*TickSum))
				g_MemData.QData.PulseTime[OccNo] -= (MS_1TICK*TickSum); 
//			if(g_MemReg.QData.PulseTime[OccNo] >= TickSum)
//				g_MemReg.QData.PulseTime[OccNo] -= TickSum; 
			else
				g_MemData.QData.PulseTime[OccNo] = 0;

			if(g_MemData.QData.PulseTime[OccNo]==0)
			{
				g_MemData.QData.Output[OccNo/8] = (g_MemData.QData.Output[OccNo/8] & ClrBit(OccNo%8) ); 
			} 
		}	
	}
	return ;
}
 
/*-------------------------------------------------------------------------
** 函数名称:  
** 功能描述:   
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: Ram
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年8月7日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------*/
INT8U SetMWVal( INT16U OccNo, INT16U Val ) 
{
	if( OccNo<=MAX_MW_CNT && OccNo>0 ) 
	{
		g_MemData.MData.m_pMW[OccNo-1] = Val; 
		return TRUE;
	}
	return FALSE;
}

INT16U GetMWVal( INT16U occno ) 
{ 
	if( occno<=MAX_MW_CNT && occno>0)
		return ( (g_MemData.MData.m_pMW[occno-1]) ); 
	return 0;
}
/*********************************************************************************************************
** 函数名称: GetMWBitVal
** 函数名称: GetMWBitVal
**
** 功能描述： 
**
** 输　入:  INT16U occno
** 输　入:  INT16U bit 从1开始
**          
** 输　出:  INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月7日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U GetMWBitVal( INT16U occno, INT16U bit ) 
{ 
	if( occno<=MAX_MW_CNT && occno>0 && bit>0 && bit<=16 ) 
		return ( (g_MemData.MData.m_pMW[occno-1] >>(bit-1)) & 0x01 ); 
	return 0;
}

INT8U SetMWBitVal(INT16U OccNo,INT16U bit ,INT8U Val)
{
	if( OccNo<=MAX_MW_CNT && OccNo>0 && bit>0 && bit<=16 ) 
	{
//		g_MemData.MData.m_pMW[OccNo-1] = g_MemData.MData.m_pMW[OccNo-1]   ; 
		return TRUE;
	}
	return FALSE;
}


INT8U SetMDVal( INT16U OccNo, INT32U Val ) 
{
	if( OccNo<=MAX_MD_CNT && OccNo>0 ) 
	{
		g_MemData.MData.m_pMD[OccNo-1] =  Val; 
		return TRUE;
	}
	return FALSE;
}

INT32U GetMDVal( INT16U occno ) 
{ 
	if( occno<=MAX_MD_CNT && occno>0)
		return ( (g_MemData.MData.m_pMD[occno-1]) ); 
	return 0;
}

INT8U GetMDBitVal( INT16U occno, INT16U bit ) 
{ 
	if( occno<=MAX_MD_CNT && occno>0 && bit>0 && bit<=32 ) 
		return ( (g_MemData.MData.m_pMD[occno-1] >>(bit-1)) & 0x01 ); 
	return 0;
}

/*-------------------------------------------------------------------------
** 函数名称:  GetAIVal
** 功能描述:  取模拟量的测值
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: g_MemReg
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期: 2007年8月7日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------*/
INT16U GetAIVal( INT16U OccNo )
{
	return ( g_MemData.AIData.Value[OccNo - 1] );
}

INT8U GetAIForceMark(INT16U OccNo )
{
	return	( !!( g_MemData.AIData.ForceMark[(OccNo-1)/8] & OrBit((OccNo-1)%8) ) );
}

INT8U ForceAI(INT16U OccNo, INT16U Val)
{
	INT16U bias; 

	if(Val != 0 && Val < 4000)
		Val = 4000;
	else if(Val > 20000)
		Val = 20000;

	if( OccNo<=MAX_AI_CNT && OccNo>0 ) 
	{ 
		bias=(OccNo-1)%8;
		g_MemData.AIData.Value[OccNo-1] = Val; 
		g_MemData.AIData.ForceMark[(OccNo-1)/8] = g_MemData.AIData.ForceMark[(OccNo-1)/8] | OrBit(bias);
		return TRUE;
	}  
	return FALSE;
}

INT8U UnForceAI(INT16U OccNo)
{
	INT16U bit; 
	if( OccNo<=MAX_AI_CNT && OccNo>0 ) 
	{ 
		bit=(OccNo-1)%8;
		if((g_MemData.AIData.ForceMark[(OccNo-1)/8] & OrBit(bit)))
		{
			g_MemData.AIData.ForceMark[(OccNo-1)/8] = g_MemData.AIData.ForceMark[(OccNo-1)/8] & (ClrBit(bit)); 
			g_MemData.AIData.Value[OccNo-1] = g_MemData.AIData.RawData[OccNo-1];
		}
		return TRUE;
	} 
	return FALSE;
}

INT8U SetAIForceMark( INT16U OccNo, INT16U value )
{ 
	INT16U  bias; 
	if( OccNo<=MAX_AI_CNT && OccNo>0 ) 
	{ 
		bias=(OccNo-1)%8;
		g_MemData.AIData.ForceMark[(OccNo-1)/8] =( ( g_MemData.AIData.ForceMark[(OccNo-1)/8] & (ClrBit(bias)) ) | ((!!value)<<bias) );
		return TRUE;
	} 
	return FALSE;
}
/* -------------------------------  AQ  --------------------------------------
///////////////////////////////////////////////////////////////
Input :
occno : 1 -- MAX_AQ
///////////////////////////////////////////////////////////////
----------------------------------------------------------------------------*/
INT16U GetAQVal( INT16U OccNo )
{
	//	return	g_MemReg.AQData.Value[OccNo - 1];
	return	g_MemData.AQData.Output[OccNo - 1];
}

INT8U GetAQForceMark(INT16U OccNo )
{
	return	( !!( g_MemData.AQData.ForceMark[(OccNo-1)/8] & OrBit((OccNo-1)%8) ) );
}

INT8U SetAQVal( INT16U OccNo, INT16U Val )
{
	if(Val < 4000)
		Val = 4000;
	else if(Val > 20000)
		Val = 20000;

	if( OccNo<=MAX_AQ_CNT && OccNo>0 )
	{
		//		g_MemReg.AQData.Output[OccNo - 1] = Val;
		g_MemData.AQData.Value[OccNo - 1] = Val;
		return TRUE;
	}
	return 0;
}

INT8U ForceAQ(INT16U OccNo, INT16U Val)
{
	INT16U no, bias;

	if(Val < 4000)
		Val = 4000;
	else if(Val > 20000)
		Val = 20000;

	if( OccNo<=MAX_AQ_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8;
		bias=(OccNo-1)%8;
		g_MemData.AQData.Value[OccNo-1] = Val;
		g_MemData.AQData.Output[OccNo-1] = Val;
		g_MemData.AQData.ForceMark[(OccNo-1)/8] = g_MemData.AQData.ForceMark[(OccNo-1)/8] | OrBit(bias);

		return TRUE;
	}  
	return FALSE;
}

INT8U UnForceAQ(INT16U OccNo)
{
	INT16U  bit; 
	if( OccNo<=MAX_AQ_CNT && OccNo>0 ) 
	{  
		bit=(OccNo-1)%8;
		if((g_MemData.AQData.ForceMark[(OccNo-1)/8] & OrBit(bit)) )
		{
			g_MemData.AQData.ForceMark[(OccNo-1)/8] = g_MemData.AQData.ForceMark[(OccNo-1)/8] & (ClrBit(bit)); 
		}
		return TRUE;
	} 
	return FALSE;
}

INT8U SetAQForceMark( INT16U OccNo, INT16U value )
{ 
	INT16U  bias; 
	if( OccNo<=MAX_AQ_CNT && OccNo>0 ) 
	{ 
		bias=(OccNo-1)%8;
		g_MemData.AQData.ForceMark[(OccNo-1)/8] =( ( g_MemData.AQData.ForceMark[(OccNo-1)/8] & (ClrBit(bias)) ) | ((!!value)<<bias) );
		return TRUE;
	} 
	return FALSE;
}

/* -------------------------------  M  -----------------------------------------
///////////////////////////////////////////////////////////////
Input :
occno : 1 -- MAX_M
///////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------*/
// INT8U GetMVal( INT16U OccNo)
// {
// 	return	( !!( g_MemData.MData.State[(OccNo-1)/8] & OrBit((OccNo-1)%8) ) );
// }
// 
// INT8U SetMVal( INT16U occno, INT16U value, INT32U pulseTime )
// { 
// 	INT16U  no, bias; 
// 	if( occno<=MAX_MB_CNT && occno>0 ) 
// 	{ 
// 		no= (occno-1)/8; 
// 		bias=(occno-1)%8;
// 
// 		g_MemData.MData.State[no]=(  ( g_MemReg.MData.State[no] & (ClrBit(bias)) ) | ((!!value)<<bias)  ); 
// 		if((pulseTime!=HOLD_TYPE) && (pulseTime!=HOLD_TYPE1) ) 
// 		{ 
// 			if(value)
// 			{
// 				g_MemReg.MData.PulseTime[occno-1]=pulseTime; 
// 				g_MemReg.MData.OutputEna[no] = g_MemReg.MData.OutputEna[no] | OrBit(bias); 
// 				g_MemReg.MData.State[no]=(  ( g_MemReg.MData.State[no] & (ClrBit(bias)) ) | ((!!value)<<bias)  ); 
// 			}
// 			//else
// 			//{
// 			//	return FALSE;
// 			//}
// 		} 
// 		else 
// 		{ 
// 			g_MemReg.MData.PulseTime[occno-1]=0; 
// 			g_MemReg.MData.OutputEna[no] = g_MemReg.MData.OutputEna[no] & (ClrBit(bias)); 
// 			g_MemReg.MData.State[no]=(  ( g_MemReg.MData.State[no] & (ClrBit(bias)) ) | ((!!value)<<bias)  ); 
// 		} 
// 		return TRUE;
// 	} 
// 	return FALSE;
// }
 
INT8U GetSVal(INT16U OccNo )
{
	return	( !!( g_MemData.SData.m_pS[(OccNo-1)/8] & OrBit((OccNo-1)%8) ) );
}

INT8U SetSVal( INT16U OccNo, INT8U Val )
{
	INT16U  no, bias; 
	if( OccNo<=MAX_S_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8;
		bias=(OccNo-1)%8;

		g_MemData.SData.m_pS[no]=( ( g_MemData.SData.m_pS[no] & (ClrBit(bias)) ) | ((!!Val)<<bias) );

		return TRUE;
	}

	return FALSE;
}
 
INT16U GetSWVal( INT16U OccNo )
{
	return	g_MemData.SData.m_pSW[OccNo - 1];
}

INT8U SetSWValue( INT16U OccNo, INT16U state )
{
	if( OccNo<=MAX_SW_CNT && OccNo>0 ) 
	{
		g_MemData.SData.m_pSW[OccNo-1] = (INT16U)state; 
		return TRUE;
	}
	return FALSE;
} 

void ScanAQ( INT16U OccNo, INT16U num )
{
	INT16U  bias, i; 

	for(i=OccNo;i<(OccNo+num);i++)
	{
		if( i<=MAX_AQ_CNT && i>0 )
		{ 
			bias=(i-1)%8;
			if(g_MemData.AQData.ForceMark[(OccNo-1)/8] & OrBit(bias)) 
				continue;
			g_MemData.AQData.Output[i-1] = g_MemData.AQData.Value[i-1]; 
		} 
		else
			return;
	}
	return;
}


/*********************************************************************************************************
** 函数名称: InitMemData
** 函数名称: InitMemData
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
** 日　期:  2008年9月19日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void InitMemData( )
{
	//清空
	memset( &g_MemData,0,sizeof(MEMDB));
	//系统寄存器初始化

}


//设置开入值，仅用于逻辑设置虚拟开入
INT8U SetIVal( INT16U OccNo, INT16U value ) 
{ 
	INT16U  no, bias; 
	if( OccNo<=MAX_I_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8; 
		bias=(OccNo-1)%8;
		if(!(g_MemData.IData.ForceMark[no]&OrBit(bias))) 
		{ 
			g_MemData.IData.State[no]=( ( g_MemData.IData.State[no] & (ClrBit(bias)) ) | ((!!value)<<bias)  ); 
		}
		return TRUE;
	}  
	return FALSE;
}

INT8U SetQValue( INT16U OccNo, INT8U Val)
{
	INT16U  no, bias; 

	if( OccNo<=MAX_Q_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8;
		bias=(OccNo-1)%8;
		if(!(g_MemData.QData.ForceMark[no]&OrBit(bias))) 
		{ //set val
		 	g_MemData.QData.Output[no]=(  ( g_MemData.QData.Output[no] & (ClrBit(bias)) ) | ((!!Val)<<bias)  ); 
			/*overwrite State Right now*/
			//g_MemReg.QData.State[no]=(  ( g_MemReg.QData.State[no] & (ClrBit(bias)) ) | ((!!Val)<<bias)  ); 
				return TRUE;
		 }
	}	 
	return FALSE;
}

extern INT8U SetMBValue( INT16U OccNo, INT16U Value )
{
	INT16U  no, bias; 
	if( OccNo<=MAX_MB_CNT && OccNo>0 ) 
	{ 
		no= (OccNo-1)/8; 
		bias=(OccNo-1)%8;
	 	  
		g_MemData.MData.m_pMB[no]=( ( g_MemData.MData.m_pMB[no] & (ClrBit(bias)) ) | ((!!Value)<<bias)  ); 
		 
		return TRUE;
	}  
	return FALSE;
}

/*********************************************************************************************************
** 函数名称: TestMemData
** 函数名称: TestMemData
**
** 功能描述： 测试接口
**
**          
** 输　出:  extern void
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
extern void TestMemData()
{
	static INT16U IOccNo = 1;
	static INT16U QOccNo = 1;

	static INT16U AIOccNo = 1;
	static INT16U AQOccNo = 1;
	
	static INT16U MBOccNo = 1;
	static INT16U MWOccNo = 1;

	INT8U  Val = 0;
 	 
	INT16U WVal = 0;

 
	SetIVal(IOccNo,0);
	IOccNo ++;

	if (IOccNo <= MAX_I_CNT)
	{
		Val = GetIVal(IOccNo);
		if (Val==0)
		{
			SetIVal(IOccNo,1);
		}	
	} 
	else
	{
		IOccNo = 1;
	}

	SetQValue(QOccNo,0);
	QOccNo ++;

	if (QOccNo <= MAX_Q_CNT)
	{
		Val = GetQVal(QOccNo);
		if (Val==0)
		{
			SetQValue(QOccNo,1);
		}	
	} 
	else
	{
		QOccNo = 1;
	}

	SetMBValue(MBOccNo,0);
	MBOccNo ++;
	if (MBOccNo <= MAX_MB_CNT)
	{
		Val = GetQVal(MBOccNo);
		if (Val==0)
		{
			SetQValue(MBOccNo,1);
		}	
	} 
	else
	{
		MBOccNo = 1;
	} 


	if (AQOccNo <= MAX_AQ_CNT)
	{
		WVal = GetAQVal(AQOccNo);		 
		SetAQVal(AQOccNo,++WVal);	
	} 
	else
	{
		AQOccNo = 1;
	}
	AQOccNo ++;
 
	if (MWOccNo <= MAX_MW_CNT)
	{
		WVal = GetMWVal(MWOccNo);		 
		SetMWVal(MWOccNo,++WVal);	
	} 
	else
	{
		MWOccNo = 1;
	}
	MWOccNo ++;
}

extern  M_DEF * GetMDataPtr( void )
{
	return &g_MemData.MData;
}
