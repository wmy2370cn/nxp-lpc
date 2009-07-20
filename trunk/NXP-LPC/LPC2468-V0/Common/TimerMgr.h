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

#if !defined(TIMER_MGR_H)
#define TIMER_MGR_H


#define  MAX_TIMER_CNT  16

typedef void (*fnTimerProcess)( void *pParam  );

typedef struct TIMER_MSG
{
	fnTimerProcess pFunc;
	void         *pParam;

}TIMER_MSG;

typedef struct TIMER
{
	INT32U Count;		//时间计数器
	INT32U TimeOut;		//触发点
	INT8U  Enable; 	    //使能标志
	INT8U  Flag ;       //是否已经超时标志
 	fnTimerProcess pFunc;
	void   *pFuncParam;
}TIMER;

extern void InitTimerMgr( void );

//使用接口  nTimerID 从1开始，必须 <= 32

//设置定时器
extern  INT16U  SetTimer  (INT16U nTimerID, INT32U nTime,fnTimerProcess fnProc,void *pFuncParam );
// 关定时器
extern  INT8U   KillTimer (INT16U nTimerID);

extern  INT8U  ResetTimer(INT16U nTimerID);

//
extern INT8U IsTimeTo(INT16U TimerID );



//在毫秒中断里面调用
extern void TimerMgrInIsr(INT16U nCnt );


#endif 
