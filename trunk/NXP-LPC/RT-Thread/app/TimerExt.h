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
**描        述:   软件定时器,软时钟的处理，系统时间处理
**备        注 : 软件定时器最好不要用在循环中
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


#if !defined(TIMER_MGR_H)
#define TIMER_MGR_H

#define  EXT_TIMER_CNT  8 
#define  EXT_TMR_CFG_TICKS_PER_SEC 10  

#define  EXT_TIMER_CB_EN   0
#define  EXT_TASK_TMR_STK_SIZE    256
#define  PRIO_TMR_MGR              32

#if  EXT_TIMER_CB_EN > 0
typedef void (*fnTimerProcess)( void *pParam  );
typedef struct TIMER_MSG
{
	fnTimerProcess pFunc;
	void         *pParam;
}TIMER_MSG;
#endif //  EXT_TIMER_CB_EN


typedef struct EXT_TIMER
{
	rt_uint32_t Count;		//时间计数器
	rt_uint32_t TimeOut;		//触发点
	rt_uint8_t  Enable; 	    //使能标志
	rt_uint8_t  Flag ;       //是否已经超时标志
#if  EXT_TIMER_CB_EN > 0
 	fnTimerProcess pFunc;
	void   *pFuncParam;
#endif //  EXT_TIMER_CB_EN
}EXT_TIMER;

void InitTimerMgr( void );

//使用接口  nTimerID 从1开始，必须 <= 32

//设置定时器
#if  EXT_TIMER_CB_EN > 0
rt_uint16_t  SetTimer  (rt_uint16_t nTimerID, rt_uint32_t nTime,fnTimerProcess fnProc,void *pFuncParam );
#else
rt_uint16_t  SetTimer  (rt_uint16_t nTimerID, rt_uint32_t nTime);
#endif
// 关定时器
rt_uint8_t   KillTimer (rt_uint16_t nTimerID);

rt_uint8_t  ResetTimer(rt_uint16_t nTimerID);

//
rt_uint8_t IsTimeTo(rt_uint16_t TimerID ); 
//在TICK中断里面调用
void   ExtTimerTick( void );


#endif 
