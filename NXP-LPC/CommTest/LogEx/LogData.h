

#include <queue>
#include "BDBWrapper.h"
#include "SQLite.h"


#include  "boost/memory.hpp"

const int MAX_LOG_STR_LINE = 1024;

const int MAX_NAME_LEN = 128;
const int MAX_STR_LEN = 256;

const int WRITE_SQL_NUM = 100;
const int WRITE_SQL_TIME = 8000;

 

class CLogStr
{
public:
	CLogStr() 
	{
		memset(&st,0,sizeof(SYSTEMTIME));
	//	szLogStr.Empty();
	//	szModuleName.Empty();
		memset(szModuleName,0,sizeof(TCHAR)*MAX_NAME_LEN);
		memset(szLogStr,0,sizeof(TCHAR)*MAX_STR_LEN);
		nStrType = 0;
	}
	SYSTEMTIME st;
	int  nStrType;
	TCHAR szModuleName[MAX_NAME_LEN];
	TCHAR szLogStr[MAX_STR_LEN]; 
	void BuildSQL( CString & szSQL );
}; 
 

class CLogDataMgr
{
	friend UINT __stdcall MainTask (LPVOID lpParam);
public:
	CLogDataMgr( );
	~CLogDataMgr( );
public:
	bool RegisterLog(TCHAR *szModuleName  );
	void UnregisterLog( TCHAR *szModuleName  );

	bool InitMgr( TCHAR*szFileName = NULL);
	void ExitMgr();
private:
	void GetExePathName(CString & strPathName,CString &szDirName,CString &strPlugInName);
	void Work( bool bForceWrite = false);

protected:
	SQLite::Database  m_db;
	//注册过的应该在此列表中定义
	CStringArray      m_arrModuleList;
	
	void FreeMem( );

	CWinThread *m_pWorkTsk;
	HANDLE      m_hWorkTsk;
	HANDLE      m_hStopWorkEvent;
	HANDLE      m_hDupWorkTaskHandle;

	DWORD    m_dwWriteSQL;

	boost::memory::pool m_LogStrPool   ;
public:  
	BOOL LogString( TCHAR *  lpszModuleName,  TCHAR *  lpszTxt ,int nStrType);
	BOOL LogString(  TCHAR *  lpszModuleName,SYSTEMTIME st,  TCHAR *  lpszTxt ,int nStrType);
	int  GetLogString(CLogStr *pBuff );
	int  GetLogStrCnt( );
private:
	CLogStr* Allocate();
	BOOL Release (CLogStr *pBuff);
	void FreeBuff( );

	std::deque <CLogStr *> m_arrBuffer;
	std::deque <CLogStr *> m_arrFreeBuffer;
};


void WaitForThreadToTerminate(HANDLE hThread);