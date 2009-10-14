

#pragma once

#include <queue>


const int MAX_LOG_STR_LINE = 1024;

const int MODULE_NAME_LEN = 128;
const int LOG_STR_LEN = 512;

class CLogStr
{
public:
	CLogStr() 
	{
		memset(&ST,0,sizeof(SYSTEMTIME));
 		StrType = 0;

		memset(ModuleName,0,sizeof(TCHAR)*MODULE_NAME_LEN);
		memset(LogStr,0,sizeof(TCHAR)*LOG_STR_LEN);
	}
	SYSTEMTIME ST;
 	int  StrType;
	TCHAR ModuleName[MODULE_NAME_LEN];
	TCHAR LogStr[LOG_STR_LEN]; 
};


class CLogStrMgr
{
public:
	CLogStrMgr( );
	~CLogStrMgr( );
public:
	void LogString(TCHAR * szModuleName, TCHAR * lpszTxt ,int nStrType);
	void LogString( TCHAR * szModuleName, SYSTEMTIME st, TCHAR * lpszTxt ,int nStrType);
	int GetLogStr(CLogStr *pBuff );
	bool m_bWriteDB;
private:
	CLogStr* AllocateBuffer();
	BOOL ReleaseBuffer(CLogStr *pBuff);
	void FreeMem( );

	CCriticalSection m_Lock;
	std::deque <CLogStr *> m_arrBuffer;
	std::deque <CLogStr *> m_arrFreeBuffer;
};

extern CLogStrMgr g_LogStrMgr;
