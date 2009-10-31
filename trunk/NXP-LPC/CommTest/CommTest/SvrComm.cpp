

#include "stdafx.h"
#include "SvrComm.h"

#include "Common.h"
#include "LogDataApi.h" 

#include  "boost/memory.hpp"

const int MAX_CLINENT_CNT = 128;

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

	ASSERT(this->m_nSocket == pContext->m_nSocket);
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

	//清除客户端列表信息
	FreeMem();
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

	int nSize = m_arrClientNode.size();
	if (nSize >= MAX_CLINENT_CNT)
		return;

	std::vector <CClientNode *>::iterator iter = m_arrClientNode.begin();
	bool bFind = false;

	//查找是否有重复
	for ( ;  iter != m_arrClientNode.end() ; ++ iter)
	{
		if ((*iter)->m_nSocket == pcontext->m_nSocket)
		{
			bFind = true;
			break;
		}
	}

	if (bFind)
	{//找到了，更新之
		ASSERT(FALSE);
		(*iter)->CopyData(pcontext);		
		return;
	}

	CClientNode *pClient = new CClientNode;
	pClient->CopyData(pcontext);

	m_arrClientNode.push_back(pClient);
}

void CSvrComm::NotifyDisconnectedClient(ClientContext *pContext)
{

}

void CSvrComm::FreeMem()
{

}
