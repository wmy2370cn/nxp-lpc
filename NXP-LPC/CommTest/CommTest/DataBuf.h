
#pragma once




class CDataBuf
{
public:
	CDataBuf( );
	~CDataBuf( );
public:
	bool SetData( char * szData );

private:
	unsigned char *m_pData;
	unsigned int  m_nLen;

	
//	boost::memory::pool m_MemPool   ;
};