/****************************************Copyright (c)**************************************************
**                                 
**            软时钟的处理，系统时间处理                          
**            时钟这样做，在修改时间时，存在问题                          
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
#include "Ram.h"
#include "ClockMgr.h"
#include "TaskDef.h"
 

static OS_EVENT   *ClkSem = NULL;            /* Semaphore used to access the time of day clock               */
static OS_EVENT   *ClkSemSec = NULL;         /* Counting semaphore used to keep track of seconds             */

static  OS_STK      ClkTaskStk[CLK_TASK_STK_SIZE];
 

// One-based array of days in year at month start
const INT16U _MonthDays[13] =
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

//判断是否是闰年
/*
闰年有两种情况，设年份为year，则：

（1）当year是400的整倍数时为闰年，条件表示为：

year%400= =0

（2）当year是4的整倍数，但不是100的整倍数时为闰年，条件为：

    year%4= =0 && year%100 != 0
*/

#define IsLeapYear(nYear) ((((nYear) & 3) == 0) && (((nYear) % 100) != 0 || ((nYear) % 400) == 0))

//计算某年某月有多少天
#define GetDaysInMonth(nYear, nMon) (_MonthDays[nMon] - _MonthDays[(nMon)-1] + (((nMon) == 2 && IsLeapYear(nYear)) ? 1 : 0))

//时间增加一秒
static void Time_AddSecond(CDateTime * pNow)
{
	INT16U nDaysInMonth;
	pNow->s++;
	if(pNow->s >= 60)
	{
		pNow->s = 0;
		pNow->min++;
		if(pNow->min >= 60)
		{
			pNow->min = 0;
			pNow->h++;
			if(pNow->h >= 24)
			{
				pNow->h = 0;
				pNow->d++;
				
				//计算当前月有多少天
				nDaysInMonth =GetDaysInMonth(pNow->y, pNow->mon);
				
				if(pNow->d > nDaysInMonth)	//超过一个月
				{
					if(pNow->mon==12)
					{
						//下一年
						pNow->d = 1;
						pNow->mon = 1;
						pNow->y++;
					}
					else
					{
						pNow->d = 1;
						pNow->mon++;
					}
				}
			}
		}
	}		
}
/*********************************************************************************************************
** 函数名称: ClockMgrSignal
** 函数名称: ClockMgrSignal
**
** 功能描述： 毫秒中断中，对时钟进行计时处理
**
** 输　入:  void
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
extern void ClockMgrSignal(INT16U nMsCnt)
{
	Ram.now.ms += nMsCnt; //1毫秒一个TICK
	if(Ram.now.ms>=1000)
	{
		Ram.now.ms=0;
		//增加一秒
		OSSemPost(ClkSemSec);               /* Signal that one second elapsed                          */
//		Time_AddSecond((CDateTime *)&Ram.now);
	}
}

static void  ClockMgrTask (void *data)
{
	INT8U err = 0;

	for (;;) 
	{
		OSSemPend(ClkSemSec, 0, &err);

		OSSemPend(ClkSem, 0, &err);         /* Gain exclusive access to time-of-day clock              */
		
		Time_AddSecond((CDateTime *)&Ram.now);
		
		OSSemPost(ClkSem);                  /* Release access to time-of-day clock                     */
	}
}

extern void InitClockMgr( void )
{
	ClkSem     = OSSemCreate(1);       /* Create time of day clock semaphore                           */
	ClkSemSec  = OSSemCreate(0);       /* Create counting semaphore to signal the occurrence of 1 sec. */
 
	OSTaskCreate(ClockMgrTask, (void *)0, &ClkTaskStk[CLK_TASK_STK_SIZE-1], CLK_TASK_PRIO);
}

static INT8U CheckTime(CDateTime * pTime)
{
	if (pTime->y > 99 || pTime->mon < 1 || pTime->mon > 12)
		return FALSE;

	if (pTime->d < 1 || pTime->h > 23 || pTime->min > 59 || pTime->s > 59 || pTime->ms>999)
	{
		return FALSE;
	}

	return TRUE;
}

//设置系统时间
INT8U SetDeviceSystemTime(CDateTime * pTime)
{
	if(CheckTime(pTime))
	{
		Ram.now=*pTime;

	//	ModeifyRTCTime(pTime);

		return TRUE;
	}
	else
		return FALSE;
}

static INT8U CheckClockTime( INT8U hr, INT8U min, INT8U sec)
{ 
	if (  hr > 23 || min > 59 || sec > 59)
	{
		return FALSE;
	}

	return	TRUE;
}

static INT8U CheckClockDate( INT8U month, INT8U day, INT16U year)
{
	if (year > 99 ||month < 1 || month > 12 || day<1 || day >31)
		return FALSE;
	return	TRUE;
}

extern INT8U  ClockSetDateTime(INT8U month, INT8U day, INT16U year, INT8U hr, INT8U min, INT8U sec)
{
	INT8U err = 0;
	INT8U ret = 0;

	ret = CheckClockDate( month,day,year );
	if (ret == FALSE)
		return FALSE;

	ret =CheckClockTime( hr,min,sec);
	if (ret == FALSE)
		return FALSE;

	OSSemPend(ClkSem, 0, &err);                  /* Gain exclusive access to time-of-day clock         */

	Ram.now.y = year;
	Ram.now.mon = month;
	Ram.now.d = day;
	Ram.now.h = hr;
	Ram.now.min = min;
	Ram.now.s = sec;

	OSSemPost(ClkSem);                           /* Release access to time-of-day clock                */
	return	TRUE;
}

extern INT8U   ClockSetTime(INT8U hr, INT8U min, INT8U sec)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	INT8U ret = 0;

	ret =CheckClockTime( hr,min,sec);
	if (ret == FALSE)
		return FALSE;

	OS_ENTER_CRITICAL();                         /* Gain exclusive access to time-of-day clock         */
 
	Ram.now.h = hr;
	Ram.now.min = min;
	Ram.now.s = sec;

	OS_EXIT_CRITICAL();                          /* Release access to time-of-day clock                */

	return	TRUE;
}

extern INT8U   ClockSetDate(INT8U month, INT8U day, INT16U year)
{
	INT8U err = 0;
	INT8U ret = 0;

	ret = CheckClockDate( month,day,year );
	if (ret == FALSE)
		return FALSE;

	OSSemPend(ClkSem, 0, &err);                  /* Gain exclusive access to time-of-day clock         */
	Ram.now.y = year;
	Ram.now.mon = month;
	Ram.now.d = day;
	OSSemPost(ClkSem);                           /* Release access to time-of-day clock                */

	return	TRUE;
}

// void RtcTime2DateTime(const TRtcTime * pRtcTime, CDateTime * pTime)
// {
// 	//秒
// 	pTime->s=BCD2HEX(pRtcTime->sec);
// 	//分
// 	pTime->min=BCD2HEX(pRtcTime->min);
// 	//时
// 	pTime->h=BCD2HEX(pRtcTime->hour);	//转换为24小时形式
// 	//日
// 	pTime->d=BCD2HEX(pRtcTime->date);
// 	//月
// 	pTime->mon=BCD2HEX(pRtcTime->mon);
// 	//年
// 	pTime->y=BCD2HEX(pRtcTime->year);
// 
// 	pTime->ms=0;
// }

// void DateTime2RtcTime(const TDateTime * pTime, TRtcTime * pRtcTime)
// {
// 	//秒
// 	pRtcTime->sec=HEX2BCD(pTime->s);
// 	//分
// 	pRtcTime->min=HEX2BCD(pTime->min);
// 	//时
// 	pRtcTime->hour=HEX2BCD(pTime->h);
// 	//日
// 	pRtcTime->date=HEX2BCD(pTime->d);
// 	//月
// 	pRtcTime->mon=HEX2BCD(pTime->mon);
// 	//年
// 	pRtcTime->year=HEX2BCD(pTime->y);
// }

 

 