/****************************************Copyright (c)**************************************************
**            一个数据库文件允许多个模块同时写，每个模块注册一个表                               
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
#include "stdafx.h"
#include "LogData.h"
#include "LogEx.h"
#include   <process.h> 

#define LOG_EXT
#include "LogDataExtApi.h"


CLogDataMgr g_LogMsgMgr;

static CMutex g_Lock;



/*********************************************************************************************************
** 函数名称: BuildSQL
** 函数名称: CLogStr::BuildSQL
**
** 功能描述：  
**
** 输　入:  CString & szSQL
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年9月14日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: SELECT * FROM DDDD_LOG WHERE log_time >= datetime( '2009-09-14 14:58:33.000' ) ;
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void CLogStr::BuildSQL( CString & szSQL )
{
	CString szTime;
	szTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	
	int nLen = _tcslen (this->szLogStr);
	int i = 0;
	for (i = 0; i < nLen; i++)
	{
		if(szLogStr[i] == '%' )
		{
			szLogStr[i] = '#';
		}
	}
//	strftime('%Y.%m.%d %H:%M:%S','now','localtime');
 
	szSQL.Format(_T("INSERT INTO %s_LOG (log_time,type,log_str) values( '%s','%d','%s')"),szModuleName,szTime,nStrType,szLogStr);
}

 
void CLogDataMgr::FreeBuff( )
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

BOOL CLogDataMgr::Release (CLogStr *pBuff)
{
	ASSERT(pBuff);
	if(pBuff==NULL)
		return FALSE;
	std::deque<CLogStr *>::iterator iter;

	iter = std::find(m_arrBuffer.begin(), m_arrBuffer.end(),pBuff  );
	m_arrBuffer.erase( iter);

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
CLogStr* CLogDataMgr::Allocate( )
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
/*********************************************************************************************************
** 函数名称: LogDebugString
** 函数名称: CLogMsgMgr::LogDebugString
**
** 功能描述：  往缓冲区中登录调试内容
**
** 输　入:  LPCTSTR lpszModuleName
** 输　入:  SYSTEMTIME st
** 输　入:  LPCTSTR lpszTxt
** 输　入:  int nStrType
**          
** 输　出:   BOOL
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年9月14日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
BOOL CLogDataMgr::LogString(  TCHAR *  lpszModuleName, SYSTEMTIME st,  TCHAR *  lpszTxt ,int nStrType)
{
	CLogStr *pData = NULL;
	BOOL bRet = FALSE;

	int nSize = m_arrModuleList.GetCount();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		if (m_arrModuleList[i] == lpszModuleName)
		{
			bRet = TRUE;
			break;
		}
	}

	if(bRet == FALSE)
		return FALSE;
	bRet = FALSE;
	CSingleLock QueueLock(&g_Lock);
	QueueLock.Lock( );
	if (QueueLock.IsLocked())
	{
		if (m_arrBuffer.size() < MAX_LOG_STR_LINE )
		{		
			pData = Allocate();
			if(pData)
			{
				memcpy(&pData->st,&st,sizeof(st));
				//pData->szModuleName = lpszModuleName;
				//pData->szLogStr = lpszTxt;
				_tcsncpy_s(pData->szModuleName, lpszModuleName,min(_tcslen(lpszModuleName),MAX_NAME_LEN));
				_tcsncpy_s(pData->szLogStr, lpszTxt,min(_tcslen(lpszTxt),MAX_STR_LEN));
				pData->nStrType = nStrType;
				bRet = TRUE;
			}
		}		 
	}	 
	QueueLock.Unlock();	
	return bRet;
}

BOOL CLogDataMgr::LogString( TCHAR *  lpszModuleName,   TCHAR *  lpszTxt ,int nStrType)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CLogStr *pData = NULL;
	BOOL bRet = FALSE;

	int nSize = m_arrModuleList.GetCount();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		if (m_arrModuleList[i] == lpszModuleName)
		{
			bRet = TRUE;
			break;
		}
	}

	if(bRet == FALSE)
		return FALSE;

	bRet = FALSE;

	CSingleLock QueueLock(&g_Lock);
	QueueLock.Lock( );
	if (QueueLock.IsLocked())
	{
		if (m_arrBuffer.size() < MAX_LOG_STR_LINE )
		{		
			pData = Allocate();
			if(pData)
			{
				memcpy(&pData->st,&st,sizeof(st));
			//	pData->szModuleName = lpszModuleName;
			//	pData->szLogStr = lpszTxt;
				pData->nStrType = nStrType;
				_tcsncpy_s(pData->szModuleName, lpszModuleName,min(_tcslen(lpszModuleName),MAX_NAME_LEN));
				_tcsncpy_s(pData->szLogStr, lpszTxt,min(_tcslen(lpszTxt),MAX_STR_LEN));
				bRet = TRUE;
			}
		}		 
	}	 
	QueueLock.Unlock();		
	return bRet;
}

int CLogDataMgr::GetLogString(CLogStr *pBuff )
{
	ASSERT(pBuff);
	if (pBuff == NULL)
		return 0;
	CLogStr *pTmStr = NULL; 
	CSingleLock QueueLock(&g_Lock);
	QueueLock.Lock( );
	if (QueueLock.IsLocked())
	{
		if (!m_arrBuffer.empty() )
		{		
			pTmStr = m_arrBuffer.front();
			if(pTmStr)
			{
				pBuff->nStrType = pTmStr->nStrType ;
			//	pBuff->szLogStr = pTmStr->szLogStr;
			//	pBuff->szModuleName = pTmStr->szModuleName;
				memcpy(&pBuff->st,&pTmStr->st,sizeof(pTmStr->st));				 
				_tcsncpy_s(pBuff->szModuleName, pTmStr->szModuleName,min(_tcslen(pTmStr->szModuleName),MAX_NAME_LEN));
				_tcsncpy_s(pBuff->szLogStr, pTmStr->szLogStr,min(_tcslen(pTmStr->szLogStr),MAX_STR_LEN));
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
int  CLogDataMgr::GetLogStrCnt( )
{
	int nRet = 0;
	CSingleLock QueueLock(&g_Lock);
	QueueLock.Lock( );
	nRet = m_arrBuffer.size();
	QueueLock.Unlock();
	return nRet;
}


CLogDataMgr::CLogDataMgr( ):m_LogStrPool(sizeof(CLogStr))
{
	m_pWorkTsk = NULL;
	m_hWorkTsk = NULL;
	m_hStopWorkEvent = NULL;
	m_hDupWorkTaskHandle = NULL;
	m_hStopWorkEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
}

CLogDataMgr::~CLogDataMgr( )
{
	FreeMem( );
	FreeBuff();
}
bool CLogDataMgr::RegisterLog(TCHAR *szModuleName  )
{
	if(szModuleName == NULL)
		return false;
	int nLen =  _tcslen(szModuleName);
	if (nLen<=0 ||nLen>=MAX_NAME_LEN)
		return false;
	int i = 0;
	for (i = 0 ; i <nLen; i++)
	{
		if (szModuleName[i] ==  ('%'))
		{
			return false;
		}
	}

	if (m_db.IsOpen() == false)
	{
		ASSERT(false);
		return false;
	} 

	bool bFound = false;

	int nSize = m_arrModuleList.GetCount();
	for ( i = 0; i < nSize ; i++)
	{
		if (m_arrModuleList[i] == szModuleName)
		{
			bFound = true;
			break;
		}
	}

	if (bFound == false)
	{
		m_arrModuleList.Add(szModuleName);
	}

	int rc = 0;
	//查询看看以该模块名称为名的表是否存在
	CString szTabName ;
	szTabName.Format(_T("%s_LOG"),szModuleName);
//	szTabName.MakeUpper();
	CString szSQL;
	 
	szSQL.Format(_T("select count(*) from sqlite_master where  type='table' and name='%s'" ),szTabName);
	SQLite::Table Temp = m_db.QuerySQL( szSQL );
	//如果不存在，则创建表，格式如下：ID(关键字) TIME TYPE LOG_STRING
	if (Temp.GetRowCount() )
	{
		Temp.GoRow(1);
// 		CString szVal = Temp.GetValue(0);
		CString szVal = Temp.GetValue( _T("count(*)"));
		if (szVal == _T("0"))
		{
			szSQL.Format(_T("CREATE TABLE %s (id INTEGER PRIMARY KEY,log_time TIMESTAMP ,type INTEGER,log_str TEXT)"),szTabName);
			rc = m_db.ExecuteSQL(szSQL);
			if (rc != SQLITE_OK)
				return false;
		}
		else
		{//检查表格式是否完整

		}
	}
	else
		return false;
	CString szLog;
	szLog = _T("开始记录日志！");
	//先写入一个记录
	LogString(szModuleName,szLog.GetBuffer(szLog.GetLength()) ,0);
	szLog.ReleaseBuffer();

	return true;
}
void CLogDataMgr::UnregisterLog( TCHAR *szModuleName  )
{
	if (szModuleName == NULL || _tcslen(szModuleName) <= 0 || _tcslen(szModuleName) >= MAX_NAME_LEN)
		return;

	bool bFound = false;

	int nSize = m_arrModuleList.GetCount();
	int i = 0;
	for ( i = 0; i < nSize ; i++)
	{
		if (m_arrModuleList[i] == szModuleName)
		{
			m_arrModuleList.RemoveAt(i);
			return;
		}
	}
}

UINT __stdcall  MainTask (LPVOID lpParam)
{	
	CLogDataMgr *pMgr = reinterpret_cast <CLogDataMgr *> (lpParam);
	ASSERT(pMgr);
	if (pMgr == NULL)
		return 0x88;
	Sleep(2000);

	ResetEvent( pMgr->m_hStopWorkEvent  );

	while(TRUE)
	{
		DWORD dwRtn = WaitForSingleObject(pMgr->m_hStopWorkEvent, 0);
		if ( dwRtn == WAIT_OBJECT_0)
		{	//	szLog = "退出函数..................\n";
			pMgr->Work(true);
			OutputDebugString(_T("退出在线函数..................\n"));
			break;
		}	
		Sleep(800);
		pMgr->Work();	 
	} 
	return 0;
}
/*********************************************************************************************************
** 函数名称: InitMgr
** 函数名称: CLogMsgMgr::InitMgr
**
** 功能描述：  打开数据库，如果没有，则创建
** 
**          
** 输　出:   bool
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年9月10日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
bool CLogDataMgr::InitMgr( TCHAR*szFileName  )
{	  
	CString szPathName,szDirName,szExeName;
	GetExePathName(szPathName, szDirName,szExeName);

	CString szDbFileName ;
	if ( szFileName == NULL || _tcsclen(szFileName) <= 0)
	{
		szDbFileName =  szDirName + szPathName + _T("LogData.db");  
	}
	else
	{
		szDbFileName =  szDirName + szPathName + szFileName;
	}
	 
	ASSERT(szDbFileName.GetLength() > 0);

	bool bRet = false;

	if (m_db.IsOpen())
	{
		return true;
	}	

	int rc = m_db.Open(szDbFileName);

	if (rc != SQLITE_OK)
	{
		return false;
	}
	
	m_dwWriteSQL = GetTickCount();

	Sleep(100);

	//启动写线程
#if 0
	if (m_pWorkTsk == NULL)
	{
		m_pWorkTsk = AfxBeginThread( MainTask, (LPVOID)(this));
		m_pWorkTsk->m_bAutoDelete = FALSE;
		m_pWorkTsk->ResumeThread();

		HANDLE pProcess = ::GetCurrentProcess();
		ASSERT(pProcess);
		//将句柄复制
		BOOL bRet = ::DuplicateHandle(pProcess,m_pWorkTsk->m_hThread,pProcess,&m_hDupWorkTaskHandle,DUPLICATE_SAME_ACCESS,true,DUPLICATE_SAME_ACCESS);
		//保证所复制的句柄要有效
		ASSERT(bRet);
	}
#endif
	if (m_hWorkTsk == NULL)
	{
		unsigned uiThreadID = 0;
		m_hWorkTsk = (HANDLE)_beginthreadex( NULL, 0, &MainTask, (LPVOID)(this), 0, &uiThreadID );

		HANDLE pProcess = ::GetCurrentProcess();
		ASSERT(pProcess);
		//将句柄复制
		BOOL bRet = ::DuplicateHandle(pProcess,m_hWorkTsk,pProcess,&m_hDupWorkTaskHandle,DUPLICATE_SAME_ACCESS,true,DUPLICATE_SAME_ACCESS);
		//保证所复制的句柄要有效
		ASSERT(bRet);

	}

	return true;
}
void CLogDataMgr::ExitMgr(  )
{
	//停止写线程
	if(m_hStopWorkEvent)
		SetEvent( m_hStopWorkEvent );
#if 0
	if (m_pWorkTsk != NULL && m_hDupWorkTaskHandle)
	{
		::WaitForThreadToTerminate(m_hDupWorkTaskHandle);
		CloseHandle(m_hDupWorkTaskHandle);
		delete m_pWorkTsk;
		m_pWorkTsk = NULL;
	}
#endif
	if (m_hWorkTsk != NULL && m_hDupWorkTaskHandle)
	{
		::WaitForThreadToTerminate(m_hDupWorkTaskHandle);
		CloseHandle(m_hDupWorkTaskHandle);
		m_hWorkTsk = NULL;
	}

}

void CLogDataMgr:: FreeMem( )
{
	 
}


/*********************************************************************************************************
** 函数名称: Work
** 函数名称: CLogMsgMgr::Work
**
** 功能描述：  插入单条记录的方式，可能比较耗时，最好还是采用事务方式，待测试
**
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年9月10日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void CLogDataMgr::Work(bool bForceWrite /*= false*/ )
{
	CLogStr LogStr;
	BOOL bRet = FALSE;
	std::vector <CLogStr *> arrLogs;
	CLogStr *pLog = NULL; 

	DWORD dwTimeSpan = labs(GetTickCount() - m_dwWriteSQL);
 
	if (bForceWrite == true || GetLogStrCnt()>= WRITE_SQL_NUM || dwTimeSpan >= WRITE_SQL_TIME )
	{
		while( GetLogStrCnt() )
		{
			//	pLog = new CLogStr;
			pLog = (CLogStr *)m_LogStrPool.allocate();
			bRet = GetLogString(pLog);
			if (bRet == FALSE)
			{
				//	delete pLog;
				m_LogStrPool.deallocate(pLog);
				break;
			}
			arrLogs.push_back(pLog);
			m_dwWriteSQL = GetTickCount();
		} 
	}	

	//如果有数据，写入SQLite
	if (!arrLogs.empty())
	{
		ASSERT(m_db.IsOpen());
		CString szSQL;
		CString szTime;
		int rc = 0;
		std::vector <CLogStr  * >::iterator it = arrLogs.begin();
		try
		{
			szSQL = _T("begin transaction;");
			rc =m_db.ExecuteSQL(szSQL);
	
			for (;it != arrLogs.end(); ++it)
			{
				(*it)->BuildSQL(szSQL);
// 				szTime.Format(_T("%04d-%d-%d %d:%d:%d.%03d"),pLog->st.wYear,pLog->st.wMonth,pLog->st.wDay,pLog->st.wHour,pLog->st.wMinute,pLog->st.wSecond,pLog->st.wMilliseconds);
// 				szSQL.Format(_T("INSERT INTO %s (time,type,log_str) values('%s','%d','%s')"),pLog->szModuleName,szTime,pLog->nStrType,pLog->szLogStr);
				rc = m_db.ExecuteSQL(szSQL);
			}
			szSQL = _T("commit  transaction;");
			rc = m_db.ExecuteSQL(szSQL);		
		}
		catch (...)
		{
			
		} 
		//回收内存

		for (it = arrLogs.begin(); it != arrLogs.end(); ++it)
		{
		//	delete (*it);
			m_LogStrPool.deallocate(*it);

		}
		arrLogs.clear();
	}
}


extern CLogExApp theApp;
void CLogDataMgr::GetExePathName(CString & strPathName,CString &szDirName,CString &strPlugInName)
{
	//	char szFullPath[MAX_PATH];
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szPlugName[MAX_PATH];

	//	USES_CONVERSION;
	// Get application's full path.
	::GetModuleFileName(theApp.m_hInstance,szFullPath, MAX_PATH);

	// Break full path into seperate components.
	//	_splitpath(  szFullPath, szDrive, szDir, szPlugName, NULL);
	//	_tsplitpath( (const wchar_t *) szFullPath, szDrive, szDir, szPlugName, NULL);
	_tsplitpath_s( (const TCHAR *) szFullPath, szDrive,_MAX_DRIVE ,szDir,_MAX_DIR, szPlugName,_MAX_PATH, NULL,0);

	// Store application's drive and path
	//	strAppDir.Format(_T("%s%s"), szDrive, szDir);
	strPlugInName.Format(_T("%s"),szPlugName);
	strPathName.Format( _T("%s"),szDir);
	szDirName.Format( _T("%s"),szDrive);
}

void WaitForThreadToTerminate(HANDLE hThread)
{
	CString szLog = _T("线程超时退出\n");

	DWORD dwRet = ::WaitForSingleObject( hThread,  3000);
	switch (dwRet)
	{
	case WAIT_OBJECT_0:
		//  The thread has terminated - do something
		break;

	case WAIT_TIMEOUT:
		//  The timeout has elapsed but the thread is still running
		//  do something appropriate for a timeout
		OutputDebugString(szLog);
		TerminateThread(hThread,0x88);
		break;
	default:
		ASSERT(false);
		break;
	}		 
}
 
LOG_DATA_EXT void LogStringEx(TCHAR * szModuleName,  TCHAR *  lpszTxt ,int nStrType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(lpszTxt == NULL)
		return;
	int nLen = _tcslen(lpszTxt);
	if (nLen <= 0)
		return ;
	if (szModuleName == NULL || _tcslen( szModuleName ) <= 0 || _tcslen( szModuleName ) >= MAX_NAME_LEN)
		return;

	if (nLen < MAX_STR_LEN)
	{
		g_LogMsgMgr.LogString(szModuleName, lpszTxt,nStrType);
	}
	else
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		TCHAR szTmpStr[MAX_STR_LEN] = {0};
		int nRemainLen = nLen;
		int nOffset = 0;
		int nCpLen = 0;
		while(nRemainLen)
		{
			nCpLen = min(nRemainLen,MAX_STR_LEN);
			_tcsncpy_s(szTmpStr,MAX_STR_LEN,& lpszTxt[nOffset],nCpLen);
			nOffset+=nCpLen;
			nRemainLen -= nCpLen;
			g_LogMsgMgr.LogString(szModuleName, st,szTmpStr,nStrType);
			if (nOffset >= nLen || nRemainLen <= 0)
			{
				break;
			}
		}
	}	 
}
 
LOG_DATA_EXT void LogStringTmEx(TCHAR * szModuleName, SYSTEMTIME st,  TCHAR *  lpszTxt ,int nStrType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (szModuleName == NULL || _tcslen( szModuleName ) <= 0 || _tcslen( szModuleName ) >= MAX_NAME_LEN)
		return;

	g_LogMsgMgr.LogString(szModuleName,st,lpszTxt,nStrType);
}
 

LOG_DATA_EXT  bool RegisterLogEx(TCHAR *szModuleName  )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 

	return g_LogMsgMgr.RegisterLog(szModuleName);
}
LOG_DATA_EXT  void UnregisterLogEx( TCHAR *szModuleName  )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_LogMsgMgr.UnregisterLog(szModuleName);
}
