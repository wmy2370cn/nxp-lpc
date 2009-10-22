// TimerEx.h: interface for the CTimerEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_TIMER_EX_H_)
#define _TIMER_EX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 

const int MAX_TIMER_NUM = 32; //最多同时支持32个定时器

typedef  struct tagTimeEvent
{
	BOOL bEnable;
	DWORD dwLastTime;
	DWORD dwCurTime; 
	DWORD dwTimerOut;
} TimerEvent ;

class CTimerEx
{
public:
	CTimerEx();
	~CTimerEx();
public:
	BOOL InitTimer(WORD wTimerID,DWORD dwTimeOut,BOOL bEnable = TRUE  );
	BOOL IsTimeTo(WORD wTimerID =1 );
	void KillTimer(WORD wTimerID);
	void EnableTimer( WORD wTimerID =1 ,BOOL bEnable = TRUE  );
private:
	TimerEvent m_te[MAX_TIMER_NUM];
};

#endif // 