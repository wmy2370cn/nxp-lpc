

#include "stdafx.h"
#include "LogData.h"
#include "LogDataApi.h"
 
#include "LogDataExtApi.h"
#pragma comment(lib,"..\\lib\\LogEx.lib")



CLogStrMgr g_LogStrMgr;


CLogStrMgr::CLogStrMgr()
{
	m_bWriteDB = true;
}

CLogStrMgr::~CLogStrMgr()
{
	FreeMem( );
}
void CLogStrMgr::FreeMem( )
{
	std::deque<CLogStr *>::iterator iter = m_arrBuffer.begin();
	for (; iter != m_arrBuffer.end(); ++iter)
	{
		delete *iter;
	}
	m_arrBuffer.clear();

	for (iter = m_arrFreeBuffer.begin(); iter != m_arrFreeBuffer.end(); ++iter)
	{
		delete *iter;
	}
	m_arrFreeBuffer.clear();
}

BOOL CLogStrMgr::ReleaseBuffer(CLogStr *pBuff)
{
	ASSERT(pBuff);
	if(pBuff==NULL)
		return FALSE;
	std::deque<CLogStr *>::iterator iter;

	iter = std::find(m_arrBuffer.begin(), m_arrBuffer.end(),pBuff  );
	if (iter != m_arrBuffer.end())
	{
		m_arrBuffer.erase( iter);
	}

	//添加到空闲列表中
	if( m_arrFreeBuffer.size() <MAX_LOG_STR_LINE)
	{
		m_arrFreeBuffer.push_back( pBuff );
	}
	else
	{
		// Delete the buffer. 
		if ( pBuff!=NULL )
			delete pBuff;
	}
	pBuff=NULL;
	return TRUE;
}
/*********************************************************************************************************
** 函数名称: AllocateBuffer
** 函数名称: CLogStrMgr::AllocateBuffer
**
** 功能描述：  
**
** 输　入:   
**          
** 输　出:   CLogStr*
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年7月20日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CLogStr* CLogStrMgr::AllocateBuffer( )
{
	CLogStr *pMsg = NULL;

	if(!m_arrFreeBuffer.empty())
	{
		pMsg= m_arrFreeBuffer.front();
		m_arrFreeBuffer.pop_front();
	}

	if (pMsg == NULL)
	{//创建一个新的
		pMsg= new CLogStr();
	}
	ASSERT(pMsg);
	if (pMsg == NULL)
		return NULL;

	m_arrBuffer.push_back(pMsg);

	return pMsg;
}
void CLogStrMgr:: LogString(TCHAR * szModuleName,  SYSTEMTIME st, TCHAR * lpszTxt ,int nStrType)
{
	CLogStr *pData = NULL;

	CSingleLock QueueLock(&m_Lock);
	QueueLock.Lock( );
	if (QueueLock.IsLocked())
	{
		if (m_arrBuffer.size() < MAX_LOG_STR_LINE )
		{		
			pData = AllocateBuffer();
			if(pData)
			{
			//	memcpy(&pData->ST,&st,sizeof(st));
				memcpy_s(&pData->ST,sizeof(pData->ST),&st,sizeof(st));
			//	pData->LogStr = lpszTxt;
				_tcsncpy_s(pData->LogStr,LOG_STR_LEN,lpszTxt,min(LOG_STR_LEN,_tcslen(lpszTxt)));
				_tcsncpy_s(pData->ModuleName,MODULE_NAME_LEN,szModuleName,min(MODULE_NAME_LEN,_tcslen(szModuleName)));			

				pData->StrType = nStrType;
			}
		}		 
	}	 
	QueueLock.Unlock();		
}

void CLogStrMgr::LogString(TCHAR * szModuleName, TCHAR * lpszTxt ,int nStrType)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CLogStr *pData = NULL;

	CSingleLock QueueLock(&m_Lock);
	QueueLock.Lock( );
	if (QueueLock.IsLocked())
	{
		if (m_arrBuffer.size() < MAX_LOG_STR_LINE )
		{		
			pData = AllocateBuffer();
			if(pData)
			{
			//	memcpy(&pData->ST,&st,sizeof(st));
				memcpy_s(&pData->ST,sizeof(pData->ST),&st,sizeof(st));
			//	pData->LogStr = lpszTxt;
				_tcsncpy_s(pData->LogStr,LOG_STR_LEN,lpszTxt,min(LOG_STR_LEN,_tcslen(lpszTxt)));	
				_tcsncpy_s(pData->ModuleName,MODULE_NAME_LEN,szModuleName,min(MODULE_NAME_LEN,_tcslen(szModuleName)));			
				pData->StrType = nStrType;
			}
		}		 
	}	 
	QueueLock.Unlock();			
}

int CLogStrMgr::GetLogStr(CLogStr *pBuff )
{
	ASSERT(pBuff);
	if (pBuff == NULL)
		return 0;
	CLogStr *pTmStr = NULL; 
	CSingleLock QueueLock(&m_Lock);
	QueueLock.Lock( );
	if (QueueLock.IsLocked())
	{
		if (!m_arrBuffer.empty() )
		{		
			pTmStr = m_arrBuffer.front();
			if(pTmStr)
			{
				pBuff->StrType = pTmStr->StrType ;
				//pBuff->LogStr = pTmStr->LogStr;
				memcpy(&pBuff->ST,&pTmStr->ST,sizeof(pTmStr->ST));		
				_tcsncpy_s(pBuff->LogStr,LOG_STR_LEN,pTmStr->LogStr,min(LOG_STR_LEN,_tcslen(pTmStr->LogStr)));

			}
			delete pTmStr;
			m_arrBuffer.pop_front();
		}
		else
		{
			QueueLock.Unlock();		
			return FALSE;
		}
	}	 
	QueueLock.Unlock();		
	return TRUE;
}

void EnableWriteDB(bool bFlag)
{
	g_LogStrMgr.m_bWriteDB = bFlag;
}


void LogString( TCHAR* lpszTxt ,int nStrType)
{
	TCHAR szModuleName [] = _T("主程序"); 
	g_LogStrMgr.LogString(szModuleName, lpszTxt,nStrType);
	if (g_LogStrMgr.m_bWriteDB)
	{
		LogStringEx(szModuleName,lpszTxt,nStrType);
	}
}
void LogStringTm( SYSTEMTIME st, TCHAR* lpszTxt ,int nStrType)
{
	TCHAR szModuleName [] = _T("主程序"); 

	g_LogStrMgr.LogString(szModuleName,st,lpszTxt,nStrType);
	if (g_LogStrMgr.m_bWriteDB)
	{
		LogStringTmEx(szModuleName,st,lpszTxt,nStrType);
	}
}

bool RegisterLog(TCHAR *szModuleName  )
{
	return ::RegisterLogEx(szModuleName);
}
void UnregisterLog( TCHAR *szModuleName  )
{
	::UnregisterLogEx(szModuleName);
}


void AppLogString( TCHAR *szModuleName, TCHAR *lpszTxt, int nStrType )
{
	 	g_LogStrMgr.LogString(szModuleName, lpszTxt,nStrType);
	if (g_LogStrMgr.m_bWriteDB)
	{
		LogStringEx(szModuleName,lpszTxt,nStrType);
	}
}
void AppLogStringTm (TCHAR *szModuleName,SYSTEMTIME st, TCHAR* lpszTxt ,int nStrType)
{
	g_LogStrMgr.LogString(szModuleName,st,lpszTxt,nStrType);
	if (g_LogStrMgr.m_bWriteDB)
	{
		LogStringTmEx(szModuleName,st,lpszTxt,nStrType);
	}
}
