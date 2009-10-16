
#pragma once


namespace Common
{
	unsigned short  GetCrc16(unsigned char *pData,int nDataLen)  ;

	void WaitForThreadToTerminate(HANDLE hThread);
	unsigned char HexAscToInt( TCHAR data );
}