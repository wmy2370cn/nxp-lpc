
#pragma once

const unsigned int MAX_DATA_LEN = 2048;



class CDataBuf
{
public:
	CDataBuf( );
	~CDataBuf( );
public:
	bool SetData( char * szData );
	bool SetData(unsigned char *pData, unsigned int nLen);
	unsigned int GetData( unsigned char *pData );

private:
	unsigned char *m_pData;
	unsigned int  m_nLen;
	CCriticalSection  m_Mutex;

	
//	boost::memory::pool m_MemPool   ;
};