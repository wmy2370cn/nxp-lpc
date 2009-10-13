
#include "stdafx.h"
#include "DataBuf.h"
#include  "boost/memory.hpp"

boost::auto_alloc g_alloc; 


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

	return true;
}
