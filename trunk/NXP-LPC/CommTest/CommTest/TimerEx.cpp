// TimerEx.cpp: implementation of the CTimerEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimerEx.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTimerEx::CTimerEx()
{
	memset(&m_te,0,sizeof(TimerEvent)*MAX_TIMER_NUM);
//	ZeroMemory(m_te,sizeof(m_te)*MAX_TIMER_NUM);
}

CTimerEx::~CTimerEx()
{
//	ZeroMemory(m_te,sizeof(m_te)*MAX_TIMER_NUM);
}

BOOL CTimerEx::InitTimer(WORD wTimerID,DWORD dwTimeOut,BOOL bEnable /* = TRUE  */)
{
	ASSERT(dwTimeOut>=100);
	ASSERT(wTimerID>0);
	ASSERT(wTimerID<=MAX_TIMER_NUM);

	m_te[wTimerID-1].dwCurTime = GetTickCount();
	m_te[wTimerID-1].dwLastTime = GetTickCount();
	m_te[wTimerID-1].bEnable = bEnable;
	m_te[wTimerID-1].dwTimerOut = dwTimeOut;
	return TRUE;
}

BOOL CTimerEx::IsTimeTo(WORD wTimerID /* =1  */)
{
	if ((m_te[wTimerID-1].bEnable)&&(m_te[wTimerID-1].dwTimerOut))
	{
		m_te[wTimerID-1].dwCurTime = GetTickCount();

		DWORD dwInterVal = labs( m_te[wTimerID-1].dwCurTime - m_te[wTimerID-1].dwLastTime );
		if (dwInterVal>=m_te[wTimerID-1].dwTimerOut)
		{
			m_te[wTimerID-1].dwCurTime = m_te[wTimerID-1].dwLastTime = GetTickCount();
			return TRUE;
		}
		else
		{
			m_te[wTimerID-1].dwCurTime =  GetTickCount();
			return FALSE;
		}
	}
	return FALSE;
}

void CTimerEx::KillTimer(WORD wTimerID)
{
	m_te[wTimerID-1].bEnable = FALSE;
}

void CTimerEx::EnableTimer(WORD wTimerID , BOOL bEnable    )
{
	ASSERT(wTimerID<=MAX_TIMER_NUM);

	m_te[wTimerID-1].bEnable = bEnable;
}
