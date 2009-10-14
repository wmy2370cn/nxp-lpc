

#include "stdafx.h"
#include "ClientComm.h"
#include "ClientCommDoc.h"
#include "Common.h"
#include "LogDataApi.h"

CClientComm::CClientComm( )
{
	m_pDocument = NULL;

	m_dwDestIp = 0;
	m_nDestPortNum = 0;
	m_nLocalPortNum = 0;

	m_nSocket = INVALID_SOCKET;

	m_pCommTsk = NULL;
	m_hTaskHandle = NULL;
	m_hStopEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_pRawBuf = new unsigned char [MAX_DATA_LEN];
}

CClientComm::~CClientComm( )
{
	if(m_pRawBuf)
	{
		delete [] m_pRawBuf;
		m_pRawBuf = NULL;
	}
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
BOOL CClientComm::Connect ( DWORD dwTimeout )
{
	ASSERT(m_pDocument);
	if (m_pDocument == NULL)
		return FALSE;

	StopTask();

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
	//	return FALSE;
	}

	m_nSocket = CreateSocket( );

	CString szLog;

	if(m_nSocket != INVALID_SOCKET)
	{
		if( m_nLocalPortNum )
		{
			if (  BindSocketEx(m_nSocket,m_nLocalPortNum) != SOCKET_SUCCESS)
			{
				CloseSocket(m_nSocket);
				m_nSocket = INVALID_SOCKET;

				szLog.Format(_T("绑定本地端口%d失败，可能是端口已被占用！"),m_nLocalPortNum );
				LogString(szLog.GetBuffer(szLog.GetLength()),ERR_STR );
				szLog.ReleaseBuffer();

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
			szLog.Format(_T("连接%d.%d.%d.%d:%d成功！"),dst_addr.sin_addr.s_net ,dst_addr.sin_addr.s_host ,
				dst_addr.sin_addr.s_lh ,dst_addr.sin_addr.s_impno,m_nDestPortNum);
			LogString(szLog.GetBuffer(szLog.GetLength()),NORMAL_STR );
			szLog.ReleaseBuffer();

			StartTask();
			return TRUE;
		}
		else
		{//连接不成功，强制关闭
			szLog.Format(_T("连接%d.%d.%d.%d:%d失败！"),dst_addr.sin_addr.s_net ,dst_addr.sin_addr.s_host ,
				dst_addr.sin_addr.s_lh ,dst_addr.sin_addr.s_impno,m_nDestPortNum );
			LogString(szLog.GetBuffer(szLog.GetLength()),ERR_STR );
			szLog.ReleaseBuffer();

			CloseSocket(m_nSocket,TRUE);
			m_nSocket = INVALID_SOCKET;
			return FALSE;
		}
	}
	return FALSE;
} 
/*********************************************************************************************************
** 函数名称: StartTask
** 函数名称: CClientComm::StartTask
**
** 功能描述： 启动线程，建立连接时启动线程 
**
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年10月14日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void CClientComm::StartTask( )
{
	StopTask();
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
void CClientComm::StopTask( )
{
	if( m_hStopEvent )
		SetEvent( m_hStopEvent );

	//等待线程退出
	if ( m_pCommTsk != NULL && m_hTaskHandle)
	{
		Common::WaitForThreadToTerminate(m_hTaskHandle);
		CloseHandle(m_hTaskHandle);
		delete m_pCommTsk;
		m_pCommTsk = NULL;

		OutputDebugString(_T("文件传输线程退出 \n"));
	}
}

void CClientComm::ExecMsgCmd( CCommMsg & msg  )
{
	if (msg.m_nMsgType == MSG_SEND_DATA)
	{
		if (m_nSocket == 0 || m_nSocket == INVALID_SOCKET )	
		{
			return;
		}

		unsigned int nLen = m_SendBuf.GetData(m_pRawBuf);
		int nRetLen = 0;

		if (nLen>0)
		{
			nRetLen = SendData_Event(m_nSocket,(const char *)m_pRawBuf,nLen);
			if ( nRetLen == nLen )
			{//发送成功

			}
			else 
			{//
				if (m_nSocket != 0 && m_nSocket != INVALID_SOCKET)
				{
					if (nRetLen == SOCKET_ERROR)
					{//硬关闭
						CloseSocket(m_nSocket,TRUE);
						m_nSocket= INVALID_SOCKET;
					}
					else
					{//从容关闭
						CloseSocket(m_nSocket);
						m_nSocket= INVALID_SOCKET;

					}
					// 通知界面
				}
			}				 
		}
	}
}
/*********************************************************************************************************
** 函数名称: Engine
** 函数名称: CClientComm::Engine
**
** 功能描述： 收发引擎 
**
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年10月14日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 	//3.如果收发过程中发现socket已被对端关闭掉,则通知界面
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void CClientComm::Engine( )
{
	//1.先看看是否需要有发送，如果有发送，则发送数据
	CCommMsg msg;
	if(m_CommMsg.GetCommMsg(msg))
	{
		ExecMsgCmd(msg);
		Sleep(50);
	}
	//2.接收数据，并推送到界面
	int nRet = RecvData_Event(m_nSocket,(char*)m_pRawBuf,MAX_DATA_LEN);

	if (nRet == SOCKET_ERROR)
	{

	}
	else
	{
	
	}


}



UINT  EthCommTask (LPVOID lpParam)
{	
	CClientComm *pComm = (CClientComm *)lpParam;
	ASSERT(pComm);
	if (pComm == NULL)
		return 0x88;

	CString szLog;
	struct sockaddr_in dst_addr;		
	memset((void*)&dst_addr,0,sizeof(dst_addr));
 	dst_addr.sin_addr.s_addr=htonl( pComm->m_dwDestIp ); 
	szLog.Format(_T("%d.%d.%d.%d:%d收发线程启动。"),dst_addr.sin_addr.s_net ,dst_addr.sin_addr.s_host ,
		dst_addr.sin_addr.s_lh ,dst_addr.sin_addr.s_impno,pComm->m_nDestPortNum );

 	LogString(szLog.GetBuffer(szLog.GetLength()),ERR_STR );
	szLog.ReleaseBuffer();

	Sleep(1000);

	ResetEvent(pComm->m_hStopEvent);
 
	
	while(1)
	{
		DWORD dwRtn = WaitForSingleObject(pComm->m_hStopEvent, 0);
		if ( dwRtn == WAIT_OBJECT_0)
		{   //	szLog = "退出函数..................\n";
			OutputDebugString(_T("退出在线函数..................\n"));
			return 0x88;
		}	

		pComm->Engine();
	}

	return 0;
}