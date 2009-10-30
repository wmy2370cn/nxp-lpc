

#include "stdafx.h"
#include "SvrComm.h"

#include "Common.h"
#include "LogDataApi.h" 

#include  "boost/memory.hpp"

const int MAX_CLINENT_CNT = 128;

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

		
		return;
	}

	CClientNode *pClient = new CClientNode;




	m_arrClientNode.push_back(pClient);
}

void CSvrComm::NotifyDisconnectedClient(ClientContext *pContext)
{

}

void CSvrComm::FreeMem()
{

}
