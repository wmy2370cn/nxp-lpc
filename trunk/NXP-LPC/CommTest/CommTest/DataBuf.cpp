
#include "stdafx.h"
#include "DataBuf.h"
#include  "boost/memory.hpp"

boost::auto_alloc g_databuf_alloc; 


CDataBuf::CDataBuf()
{
	m_pData = NULL;
	m_nLen = 0;
}

CDataBuf::~CDataBuf( )
{


}

bool CDataBuf::SetData( char * szData )
{
	if (szData == NULL)
		return false;
	unsigned int nLen = strlen(szData);
	if (nLen <= 0)
		return false;

	CSingleLock lock(&m_Mutex);

	if (nLen <= m_nLen && m_nLen <= MAX_DATA_LEN)
	{	
	}	 
	else
	{//重新分配内存
		m_pData = BOOST_MEMORY_ALLOC_ARRAY(g_databuf_alloc, unsigned char, nLen); 
		ASSERT(m_pData);	
	}
	m_nLen = nLen;
	memcpy_s(m_pData,m_nLen,szData,nLen);

	lock.Unlock();
	return true;
}

bool  CDataBuf::SetData(unsigned char *pData, unsigned int nLen)
{
	if (pData == NULL || nLen <= 0 || nLen > MAX_DATA_LEN)
		return false;

	CSingleLock lock(&m_Mutex);

	if (nLen <= m_nLen && m_nLen <= MAX_DATA_LEN)
	{
	}
	else
	{//重新分配内存
		m_pData = BOOST_MEMORY_ALLOC_ARRAY(g_databuf_alloc, unsigned char, nLen); 
		ASSERT(m_pData);
		m_nLen = nLen;
	}
	memcpy_s(m_pData,m_nLen,pData,nLen);
	lock.Unlock(); 
	return true;
}


unsigned int  CDataBuf::GetData( unsigned char *pData )
{
	if (pData == NULL)
		return 0;
	if (m_nLen == 0 || m_nLen > MAX_DATA_LEN)
		return 0 ;
	
	unsigned int nRet = m_nLen;
	CSingleLock lock(&m_Mutex);
	memcpy_s(pData,m_nLen,m_pData,m_nLen);
	nRet = m_nLen;
	lock.Unlock();
	return nRet;
}
