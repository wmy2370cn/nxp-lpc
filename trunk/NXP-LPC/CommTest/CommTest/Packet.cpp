/****************************************Copyright (c)**************************************************
**                                 
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 
**描        述:    报文
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
#include "Packet.h"
#include <algorithm>
#include  "boost/memory.hpp"

boost::auto_alloc g_packet_alloc; 

CPacket::CPacket()
{
	m_pPacketData = NULL;
	m_nLen = 0;;
	m_nId = 0;
}

CPacket::~CPacket()
{

}

CPacket::CPacket( unsigned char *pData,unsigned int nLen )
{
	m_pPacketData = NULL;
	m_nLen = 0;;
	m_nId = 0;

	ASSERT( pData );
	ASSERT(nLen >0  && nLen <= MAX_PACKET_LEN);
	if (pData == NULL || nLen == 0 || nLen > MAX_PACKET_LEN)
		return;
	GetLocalTime(&m_stBuild);

	m_pPacketData = BOOST_MEMORY_ALLOC_ARRAY(g_packet_alloc, unsigned char, nLen); 
	m_nLen = nLen;
	memcpy_s(m_pPacketData,m_nLen,pData,nLen);
}

void CPacket::SetPacket( unsigned char *pData,unsigned int nLen)
{
	ASSERT( pData );
	ASSERT(nLen >0  && nLen <= MAX_PACKET_LEN);
	if (pData == NULL || nLen == 0 || nLen > MAX_PACKET_LEN)
		return;
	GetLocalTime(&m_stBuild);

	m_pPacketData = BOOST_MEMORY_ALLOC_ARRAY(g_packet_alloc, unsigned char, nLen); 
	m_nLen = nLen;
	memcpy_s(m_pPacketData,m_nLen,pData,nLen);
}

unsigned int CPacket::GetPacket(CString &szPacket)
{
	if (m_nLen >0 && m_nLen <MAX_PACKET_LEN)
	{
//		char szMsgB[512] = {0};
//		sprintf(szMsgB,"%02X ",pMsg->pData[0]);
		unsigned int nLen =  m_nLen;
		unsigned int i = 1;
		TCHAR temp[10] = {0};
		
	//	_stprintf_s(temp,"%02X ",m_pPacketData[0]);
		szPacket.Format(_T("%02X "),m_pPacketData[0]);
		if (nLen >= 32)
		{
			nLen = 32;
			for (i = 1; i<nLen; i++)
			{
				_stprintf_s(temp,6,_T("%02X "), m_pPacketData[i] );
				szPacket += temp;
			}
			szPacket.Delete(szPacket.GetLength()-1);
			szPacket += _T("...");
		}
		else
		{
			if (nLen>1)
			{
				for (i = 1; i<nLen; i++)
				{
					_stprintf_s(temp,6,_T("%02X "), m_pPacketData[i] );
					szPacket += temp;
				}
			}			
			szPacket.Delete(szPacket.GetLength()-1);			  
		}
	}
	return m_nLen;
}

unsigned char *CPacket::GetPacket(unsigned int &nLen)
{
	nLen = m_nLen;
	return m_pPacketData;
}

CPacketContainer::CPacketContainer()
{
	m_nCurId = 0;
}

CPacketContainer::~CPacketContainer()
{
	FreeMem( );
}
/*********************************************************************************************************
** 函数名称: PutData
** 函数名称: CPacketContainer::PutData
**
** 功能描述：  
**
** 输　入:  unsigned char * pData
** 输　入:  unsigned int nLen
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年10月15日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void CPacketContainer::PutData( unsigned char *pData,unsigned int nLen  )
{
	ASSERT( pData );
	ASSERT(nLen >0  && nLen <= MAX_PACKET_LEN);
	if (pData == NULL || nLen == 0 || nLen > MAX_PACKET_LEN)
		return;

	CSingleLock lock( & m_Mutex);

	if (m_arrPingBuf.size() >= MAX_PACKET_CNT )
	{
		lock.Unlock();
		return;
	}

	CPacket *pPacket = BOOST_NEW(g_packet_alloc,CPacket);
	ASSERT(pPacket);
	if (pPacket)
	{
		pPacket->SetPacket(pData,nLen);
		pPacket->m_nId = ++m_nCurId;
		m_arrPingBuf.push_back(pPacket);
	}
	lock.Unlock();
	return;
}
/*********************************************************************************************************
** 函数名称: GetData
** 函数名称: CPacketContainer::GetData
**
** 功能描述：  从PING队列中获取数据，并送到pong队列中
**
** 输　入:  CPacket * * ppPacket
**          
** 输　出:   bool
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年10月15日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
bool  CPacketContainer::GetData( CPacket **ppPacket )
{
	if (ppPacket == NULL)
		return false;

	CSingleLock lock( & m_Mutex);
	if (m_arrPingBuf.empty() )
	{
		lock.Unlock();
		return false;
	}

	*ppPacket = m_arrPingBuf.front();
	m_arrPingBuf.pop_front();
	m_arrPongBuf.push_back(*ppPacket);

	return true;
}
/*********************************************************************************************************
** 函数名称: RemoveData
** 函数名称: CPacketContainer::RemoveData
**
** 功能描述：  
**
** 输　入:  CPacket * pPacket
**          
** 输　出:   bool
**         
** 全局变量:  
** 调用模块: 由视图来调用
**
** 作　者:  LiJin
** 日　期:  2009年10月15日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
bool  CPacketContainer::RemoveData( CPacket *pPacket )
{
	ASSERT(pPacket);
	if (pPacket == NULL)
		return false;
	
	std::deque<CPacket *>::iterator iter;

	iter = std::find(m_arrPongBuf.begin(), m_arrPongBuf.end(),pPacket );
	if (iter != m_arrPongBuf.end())
	{
		ASSERT( (*iter)->m_nId == pPacket->m_nId );
		m_arrPongBuf.erase( iter);
		return true;
	}

//	CSingleLock lock( & m_Mutex);

	return false;
}

void CPacketContainer::FreeMem( )
{
	m_arrPongBuf.clear();
	m_arrPingBuf.clear();
}
