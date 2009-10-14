//内部用

#pragma once


#ifndef LOG_DATA_DEF
#define LOG_DATA_DEF
typedef enum LOG_STR_TYPE
{
	NORMAL_STR = 1,ALARM_STR =2,ERR_STR =3
}STRING_TYPE ;
#endif


bool RegisterLog(TCHAR *szModuleName  );
void UnregisterLog( TCHAR *szModuleName  );
//是否需要写库
void EnableWriteDB(bool bFlag);

//主程序用
void LogString( TCHAR* lpszTxt ,int nStrType);
void LogStringTm( SYSTEMTIME st, TCHAR* lpszTxt ,int nStrType);

//其他模块使用
void AppLogString( TCHAR *szModuleName, TCHAR *szTxt, int nStrType );
void AppLogStringTm (TCHAR *szModuleName,SYSTEMTIME st, TCHAR* lpszTxt ,int nStrType);
