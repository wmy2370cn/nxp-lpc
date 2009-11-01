

#include "stdafx.h"
#include "SvrComm.h"

#include "Common.h"
#include "LogDataApi.h" 
#include <utility>

#include  "boost/memory.hpp"

const int MAX_CLINENT_CNT = 1024;

/*********************************************************************************************************
** 函数名称: CopyData
** 函数名称: CClientNode::CopyData
**
** 功能描述：  从父类拷贝数据出来
**
** 输　入:  ClientContext * pContext
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年10月31日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void CClientNode::CopyData(ClientContext *pContext)
{
	ASSERT(pContext);
	if(pContext == NULL)
		return;

//	ASSERT(this->m_nSocket == pContext->m_nSocket);
	this->m_nSocket = pContext->m_nSocket;
	memcpy_s( & (this->m_addr) ,sizeof(m_addr),& (pContext->m_addr),sizeof(m_addr));
	
	this->m_nSendCnt = pContext->m_nSendCnt; //次数
	this->m_nRecvCnt = pContext->m_nRecvCnt;

	this->m_nTotalSendLen = pContext->m_nTotalSendLen; //字节长度
	this->m_nTotalRecvLen = pContext->m_nTotalRecvLen;

}

CSvrComm::CSvrComm( )
{
	m_bStart = FALSE;
}

CSvrComm::~CSvrComm( )
{
	FreeMem();
}

BOOL CSvrComm::StartTask( )
{
	return Start();
}

void CSvrComm::StopTask( )
{
	ShutDown();
	CSingleLock lock(&m_Lock,TRUE);

	//清除客户端列表信息
	FreeMem();
	lock.Unlock();
}
/*********************************************************************************************************
** 函数名称: NotifyNewConnection
** 函数名称: CSvrComm::NotifyNewConnection
**
** 功能描述：  
**
** 输　入:  ClientContext * pcontext
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年10月30日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void CSvrComm::NotifyNewConnection(ClientContext *pcontext)
{
	ASSERT(pcontext);
	if (pcontext == NULL)
		return;

	ASSERT(pcontext->m_nSocket != 0 && pcontext->m_nSocket != INVALID_SOCKET);
	ASSERT(pcontext->m_nID != 0 && pcontext->m_nID != INVALID_SOCKET);
	bool bFind = false;
	std::vector <CClientNode *>::iterator iter ;
	stdext::hash_map <ClientContext *, CClientNode *>::iterator it_map;

	CSingleLock lock(&m_Lock,TRUE);
	int nSize = m_arrClientNode.size();
	if (nSize >= MAX_CLINENT_CNT)
	{
		lock.Unlock();
		return;
	}

	//查找是否有重复 以IP地址和端口号来比较
	for (iter = m_arrClientNode.begin();  iter != m_arrClientNode.end() ; ++ iter)
	{
// 		if ((*iter)->m_nSocket == pcontext->m_nSocket)
// 		{
// 			bFind = true;
// 			break;
// 		}
		if (0 == memcmp( &((*iter)->m_addr ),&(pcontext->m_addr),sizeof(pcontext->m_addr)))
		{
			bFind = true;
			break;
		}
	}

	if (bFind)
	{//找到了，更新之 同一个来源的断开重新联机
		(*iter)->CopyData(pcontext);	
		(*iter)->m_bOnline = TRUE;

		it_map = m_mapClients.find( pcontext );
		if (it_map != m_mapClients.end())
		{
			ASSERT(FALSE);
			it_map->second = (*iter);
		}
		else
		{
			m_mapClients.insert(std::make_pair(pcontext,(*iter)));
		} 

		lock.Unlock();
		return;
	}

	CClientNode *pClient = new CClientNode;
	pClient->CopyData(pcontext);
	pClient->m_bOnline = TRUE;

	m_arrClientNode.push_back(pClient);

	it_map = m_mapClients.find( pcontext );
	if (it_map != m_mapClients.end())
	{
		ASSERT(FALSE);
		it_map->second = pClient;
	}
	else
	{
		m_mapClients.insert(std::make_pair(pcontext,pClient));
	}

	lock.Unlock();
}

void CSvrComm::NotifyDisconnectedClient(ClientContext *pContext)
{
	CClientNode *pClient = NULL;
	stdext::hash_map <ClientContext *, CClientNode *>::iterator it_map;

	CSingleLock lock(&m_Lock,TRUE);

	it_map = m_mapClients.find( pContext );
	if (it_map != m_mapClients.end())
	{
		it_map->second->m_bOnline = FALSE;
		m_mapClients.erase(it_map);
	}
	else
	{
	//	ASSERT(FALSE);
		OutputDebugString(_T("xxxxxxx\r\n"));
	}
	lock.Unlock();
}

/*********************************************************************************************************
** 函数名称: NotifyReceivedPackage
** 函数名称: CSvrComm::NotifyReceivedPackage
**
** 功能描述：  
**
** 输　入:  CSvrCommPacket * pOverlapBuff
** 输　入:  int nSize
** 输　入:  ClientContext * pContext
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月1日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void CSvrComm::NotifyReceivedPackage(CSvrCommPacket *pOverlapBuff,int nSize,ClientContext *pContext)
{
	ASSERT(pContext);
	ASSERT(nSize >= 0 && nSize <= MAX_PACKAGE_SIZE);
	ASSERT(pOverlapBuff);

	if(pOverlapBuff == NULL || pContext == NULL || nSize <= 0 || nSize >= MAX_PACKAGE_SIZE)
		return;

	CClientNode *pClient = NULL;
	stdext::hash_map <ClientContext *, CClientNode *>::iterator it_map;

	CSingleLock lock(&m_Lock,TRUE);

	it_map = m_mapClients.find( pContext );
	if (it_map != m_mapClients.end())
	{
		pClient = it_map->second ;
	}
	else
	{
		ASSERT(FALSE);
	}
	ASSERT(pClient);
	if (pClient==NULL)
	{
		lock.Unlock();
		return;
	}
	pClient->m_nRecvCnt ++;
	pClient->m_nTotalRecvLen += nSize;
	lock.Unlock();
	return;
}

void CSvrComm::FreeMem()
{
	std::vector <CClientNode *>::iterator iter ;

	for (iter = m_arrClientNode.begin();  iter != m_arrClientNode.end() ; ++ iter)
	{
		delete (*iter);		 
	}
	m_arrClientNode.clear();
	m_mapClients.clear();
}
