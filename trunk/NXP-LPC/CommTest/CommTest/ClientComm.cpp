

#include "stdafx.h"
#include "ClientComm.h"
#include "ClientCommDoc.h"

CClientSocket::CClientSocket( )
{
	m_pDocument = NULL;

	m_dwDestIp = 0;
	m_nDestPortNum = 0;
	m_nLocalPortNum = 0;

	m_pCommTsk = NULL;
	m_hTaskHandle = NULL;
	m_hStopEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
}

CClientSocket::~CClientSocket( )
{

}


/*********************************************************************************************************
** 函数名称: Connect
** 函数名称: CClientSocket::Connect
**
** 功能描述：  tcp 连接
**
** 输　入:  DWORD dwTimeout
**          
** 输　出:   BOOL
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年10月13日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
BOOL CClientSocket::Connect ( DWORD dwTimeout )
{
	if (m_pDocument == NULL)
		return FALSE;

	m_dwDestIp = m_pDocument->m_dwDestIp;
	ASSERT(m_dwDestIp);
	if (m_dwDestIp == 0)
		return FALSE;	

	m_nDestPortNum = m_pDocument->m_nDestPort;
	if (m_nDestPortNum == 0)
		return FALSE;

	m_nLocalPortNum = m_pDocument->m_nLocalPort;

	if (m_nSocket != 0 && m_nSocket != INVALID_SOCKET)
	{
		CloseSocket(m_nSocket);
		m_nSocket = INVALID_SOCKET;
		return FALSE;
	}

	m_nSocket = CreateSocket( );

	if(m_nSocket != INVALID_SOCKET)
	{
		if( m_nLocalPortNum )
		{
			if (  BindSocketEx(m_nSocket,m_nLocalPortNum) != SOCKET_SUCCESS)
			{
				CloseSocket(m_nSocket);
				m_nSocket = INVALID_SOCKET;
				return FALSE;
			}			 
		}

		struct sockaddr_in dst_addr;		
		memset((void*)&dst_addr,0,sizeof(dst_addr));

		dst_addr.sin_family=AF_INET;
		dst_addr.sin_port=htons(m_nDestPortNum);
		dst_addr.sin_addr.s_addr=htonl( m_dwDestIp ); 

		if (dwTimeout <= 10)
			dwTimeout = CONNECT_TIMEOUT;
		int nRet = Connect_Event(m_nSocket,(struct sockaddr *)&dst_addr,sizeof(dst_addr),dwTimeout);
		if (nRet == SOCKET_SUCCESS)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//开始收发
void CClientSocket::StartEngine( )
{
	StopEngine();
	if (m_pCommTsk == NULL)
	{
		m_pCommTsk = AfxBeginThread( EthCommTask, (LPVOID)(this));
		m_pCommTsk->m_bAutoDelete = FALSE;
		m_pCommTsk->ResumeThread();

		HANDLE pProcess = ::GetCurrentProcess();
		ASSERT(pProcess);
		//将句柄复制
		BOOL bRet = ::DuplicateHandle(pProcess,m_pCommTsk->m_hThread,pProcess,&m_hTaskHandle,DUPLICATE_SAME_ACCESS,true,DUPLICATE_SAME_ACCESS);
		//保证所复制的句柄要有效
		ASSERT(bRet);
	}
	return ;
}
//停止收发
void CClientSocket::StopEngine( )
{

}
UINT  EthCommTask (LPVOID lpParam)
{

	return 0;
}