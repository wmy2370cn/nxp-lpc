 

#include "stdafx.h"
#include "EcBusSocket.h"



//注册WINSOCK2.2DLL
int  InitLibrary(void)
{
	WSADATA         WSD;
	WORD wVersionRequired = MAKEWORD( SOCKET_MAJOR_VERSION, SOCKET_MINOR_VERSION );
	ZeroMemory(&WSD,sizeof(WSADATA));
	int nErrorNo = WSAStartup(wVersionRequired, &WSD);
	if ( SOCKET_SUCCESS != nErrorNo )
	{
		//SetLastError( nErrorNo );
		return ( SOCKET_ERROR );
	}
	if ( LOBYTE( WSD.wVersion ) != SOCKET_MINOR_VERSION || HIBYTE( WSD.wVersion ) != SOCKET_MAJOR_VERSION ) 
	{
		WSACleanup( );
		//	SetLastError( WSAVERNOTSUPPORTED );
		return (SOCKET_ERROR); 
	}
	//成功初始化
	return (SOCKET_SUCCESS);
}
//释放Winsock2动态连接库
void  CleanupLibrary(void)
{
	WSACleanup();
}

CEcBusSocket::CEcBusSocket()
{
	::InitializeCriticalSection(&m_csErrorNo);
}

CEcBusSocket::~CEcBusSocket()
{
	::DeleteCriticalSection(&m_csErrorNo);
}

// 设置套接字是否为阻塞的
//入口:套接字,是否需要阻塞的
//出口:如果正确那么返回0，错误返回-1
int CEcBusSocket::BlockSocket(SOCKET hSocket, BOOL bBlock/*FALSE*/)
{
	u_long IoctlLong = (bBlock) ? 0 : 1;
	if (ioctlsocket(hSocket, FIONBIO, &IoctlLong) == SOCKET_ERROR)
	{
		SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
    }
	return (SOCKET_SUCCESS);
}

//设置套接字属性
//入口:套接字
//出口:如果正确那么返回0，错误返回-1
int CEcBusSocket::SetSocketOption(SOCKET hSocket)
{
    int nActivate = 1;
	//允许地址重用
    if (setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &nActivate,sizeof(nActivate)) == SOCKET_ERROR )
    {
        SetLastError( WSAGetLastError() );
        return (SOCKET_ERROR);//return (-1)
    }
	return (SOCKET_SUCCESS);
}

//接收所有数据，注意在这个函数调用之前必须确认是否有接收消息到来
//入口:套接字,数据缓冲区,缓冲区大小
//出口:如果正确那么返回接收的字节数量，错误返回错误代码
int CEcBusSocket::RecvLL(SOCKET hSocket, char *pszBuffer, int nBufferSize)
{
	DWORD		dwRtxBytes = 0,
				dwRtxFlags = 0;
	WSABUF		WSABuff;

	//清空缓冲
	ZeroMemory(&WSABuff,sizeof(WSABUF));

	WSABuff.len = nBufferSize;
	WSABuff.buf = pszBuffer;
	//如果正确就返回本次接收的字节个数，如果错误返回错误号码(负数)
	return ((WSARecv(hSocket, &WSABuff, 1, &dwRtxBytes, &dwRtxFlags,NULL, NULL) == SOCKET_SUCCESS) ? (int) dwRtxBytes : -WSAGetLastError());
}


int CEcBusSocket::RecvData_Block(SOCKET hSocket, char *pszBuffer, int nBufferSize, 	DWORD dwTimeout)
{
	ASSERT(hSocket != NULL);
	if(hSocket==NULL)
		return ( SOCKET_ERROR );
	FD_SET fd = {1, hSocket};
	TIMEVAL tv = {dwTimeout, 0};
	int nBytesReceived=0;
	if(select(0, &fd, NULL, NULL, &tv) == 0) 
		goto CLEAR;
	if((nBytesReceived = recv(hSocket, pszBuffer, nBufferSize, 0)) == SOCKET_ERROR)
		goto CLEAR;
	return nBytesReceived;

CLEAR:
	SetLastError(WSAGetLastError());//超时
	return(SOCKET_ERROR);
}

// 接收数据(阻塞直至收到数据为止)
int CEcBusSocket::RecvData_Event(SOCKET hSocket, char *pszBuffer, int nBufferSize, DWORD dwTimeout)
{
	HANDLE hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hReadEvent == NULL)
	{
		SetLastError( (int)GetLastError() );
		return ( SOCKET_ERROR );
	}

	int		nRecvBytes = 0;
	DWORD	dwWaitResult ;
	for (;;)
	{
		// 注册FD_READ | FD_CLOSE 事件 
		// (因为可能在等待FD_READ事件中，对方关闭套接字，所以要关注FD_CLOSE)
		if( WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, FD_READ | FD_CLOSE) == SOCKET_ERROR)
		{
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
	
		// 等等FD_READ | FD_CLOSE事件的发生
		dwWaitResult = WSAWaitForMultipleEvents(1, &hReadEvent, TRUE,dwTimeout, TRUE);

		if (dwWaitResult != WSA_WAIT_EVENT_0)
		{
			// 清除事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		
		////////////////////////////////////////////////////////////// 
		///	注意：即使 dwWaitResult == WSA_WAIT_EVENT0 ，也应该 
		///			进一步检查网络是否发生错误
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hReadEvent,&NetEvent) == SOCKET_ERROR)
		{
			// 清除事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		//判断发生了什么事件 FD_READ 或 FD_CLOSE	
		if( ( NetEvent.lNetworkEvents == FD_CLOSE ) ||
			 ( NetEvent.lNetworkEvents	== FD_READ && 
			   NetEvent.iErrorCode[FD_READ_BIT] !=0 ) )	// 发生错误
		{
			// 清除事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError(WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		////////////////////////////////////////////////////////////////
		// 清除事件
		WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
	
		// 接收数据
		if ((nRecvBytes = RecvLL(hSocket, pszBuffer, nBufferSize)) >= 0)
			break;	// 跳出循环

		//Recv返回的是错误代码的负数，所以需要调转过来
		int nErrorCode = -nRecvBytes;

		if ( nErrorCode != WSAEWOULDBLOCK )	//太多的未完成重叠操作
		{
			CloseHandle(hReadEvent);
			SetLastError( nErrorCode );
			return (SOCKET_ERROR);
		}
		//阻塞住了
		Sleep(BLOCKED_SNDRCV_SLEEP);
    }
    CloseHandle(hReadEvent);
    return (nRecvBytes);
}


//发送数据，阻塞
//入口:套接字，发送的字串,字串长度，超时值
int CEcBusSocket::Send_Block(SOCKET hSocket,char const * pszBuffer, int nBufferSize, DWORD dwTimeout)
{
	ASSERT(hSocket!=NULL);
	if(hSocket==NULL||pszBuffer==NULL)
		return (SOCKET_ERROR);
	FD_SET fd = {1, hSocket};
	TIMEVAL tv = {dwTimeout, 0};
	int nBytesSent=0;
	if(select(0, NULL, &fd, NULL, &tv) == 0)
		goto CLEAR;//选择发送超时
	if((nBytesSent = send(hSocket, pszBuffer, nBufferSize, 0)) == SOCKET_ERROR) 
		goto CLEAR;//发送出错误
	return nBytesSent;

CLEAR:
	SetLastError(WSAGetLastError());//超时
	return(SOCKET_ERROR);
}

//发送全部缓冲区中数据，阻塞
//入口:套接字，发送的字串,字串长度，超时值
//出口:正确返回发送的字节数量，错误返回SOCKET_ERROR
int CEcBusSocket::SendData_Block(SOCKET hSocket,char const * pszBuffer, int nBufferSize, DWORD dwTimeout)
{
	if(hSocket==NULL)
		return(SOCKET_ERROR);
	int nBytesSent = 0;
	int nBytesThisTime;
	const char* pszTemp = pszBuffer;
	do 
	{
		nBytesThisTime = Send_Block(hSocket,pszTemp, nBufferSize-nBytesSent, dwTimeout);
		if(nBytesThisTime<0)
			return(SOCKET_ERROR);
		//如果一次没有发送成功
		nBytesSent += nBytesThisTime;
		//改变当前字符指针
		pszTemp += nBytesThisTime;
	} while(nBytesSent < nBufferSize);
	return nBytesSent;
}

//入口:套接字，发送的字串,字串长度，超时值
//出口:正确返回发送的字节数量，错误返回SOCKET_ERROR
int CEcBusSocket::SendData_Event(SOCKET hSocket,char const * pszBuffer, int nBufferSize, DWORD dwTimeout)
{
	if(hSocket==NULL)
		return(SOCKET_ERROR);
	int nBytesSent = 0;
	int nBytesThisTime;
	const char* pszTemp = pszBuffer;
	do 
	{
		nBytesThisTime = Send_Event(hSocket,pszTemp, nBufferSize-nBytesSent, dwTimeout);
		if(nBytesThisTime<0)
			return(SOCKET_ERROR);
		//如果一次没有发送成功
		nBytesSent += nBytesThisTime;
		//改变当前字符指针
		pszTemp += nBytesThisTime;
	} while(nBytesSent < nBufferSize);
	return nBytesSent;
}
//一次发送数据，但不一定全部都发送
//入口:套接字，接收方地址信息,地址结构，结构长度，缓冲区，缓冲区长度，超时
//出口:正确返回发送字节数量，错误返回SOCKET_ERROR
int CEcBusSocket::SendTo_Block(SOCKET hSocket, const struct sockaddr * pTo,	int nAddrLen,char const * pszBuffer, int nBufferSize, DWORD dwTimeout)
{
	if(hSocket==NULL||pszBuffer==NULL)
		return SOCKET_ERROR;
	FD_SET fd = {1, hSocket};
	TIMEVAL tv = {dwTimeout, 0};
	int nBytesSent=0;
	if(select(0, NULL, &fd, NULL, &tv) == 0) 
		goto CLEAR;
	nBytesSent = sendto(hSocket, pszBuffer, nBufferSize, 0, pTo, nAddrLen);
	if(nBytesSent == SOCKET_ERROR)
		goto CLEAR;
	return nBytesSent;

CLEAR:
	SetLastError(WSAGetLastError());//超时
	return(SOCKET_ERROR);
}

// 数据报发送数据报
int CEcBusSocket::SendTo_Event(SOCKET hSocket, const struct sockaddr * pTo,
								 int nAddrLen,char const * pszBuffer, int nBufferSize, DWORD dwTimeout)
{
	HANDLE hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL)
	{
		SetLastError( (int)GetLastError() );
		return (SOCKET_ERROR);
	}

	DWORD	dwRtxBytes = 0,
            dwRtxFlags = 0;
	WSABUF	WSABuff;

	ZeroMemory(&WSABuff,sizeof(WSABUF));
	WSABuff.len = nBufferSize;
	WSABuff.buf = (char *) pszBuffer;

	for (;;)
	{
		if (WSASendTo( hSocket, &WSABuff, 1, &dwRtxBytes, dwRtxFlags,
			pTo, nAddrLen, NULL, NULL) == SOCKET_SUCCESS)
			break;

		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		//////////////////////////////////////////////////////////////////////////
		//	睡眠一段时间
		/////////////////////////////////////////////////////////////////////////
		Sleep(BLOCKED_SNDRCV_SLEEP);

		// 注册FD_WRITE事件  
		if( WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, FD_WRITE) 
			== SOCKET_ERROR)
		{
			CloseHandle(hWriteEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		DWORD dwWaitResult = WSAWaitForMultipleEvents
			(1, &hWriteEvent, TRUE,dwTimeout, TRUE);
		
		if( dwWaitResult != WSA_WAIT_EVENT_0 )
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hWriteEvent,&NetEvent) 
			== SOCKET_ERROR)
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		if(NetEvent.iErrorCode[FD_WRITE_BIT] !=0 )	// 发生错误
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(NetEvent.iErrorCode[FD_WRITE_BIT]);
			return (SOCKET_ERROR);
		}
		////////////////////////////////////////////////////////////////
		// 注销事件
		WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
	}

	CloseHandle(hWriteEvent);
	return ((int) dwRtxBytes);
}

int  CEcBusSocket::Send_Event(SOCKET hSocket, char const * pszBuffer, int nBufferSize, DWORD dwTimeout/*=SEND_TIMEOUT*/)
{
	HANDLE hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL)
	{
		SetLastError( (int)GetLastError() );
		return (SOCKET_ERROR);
	}

	DWORD	dwRtxBytes = 0, dwRtxFlags = 0;
	WSABUF	WSABuff;

	ZeroMemory(&WSABuff,sizeof(WSABUF));
	WSABuff.len = nBufferSize;
	WSABuff.buf = (char *) pszBuffer;

	for (;;)
	{
		if (WSASend( hSocket, &WSABuff, 1, &dwRtxBytes, dwRtxFlags, NULL, NULL) == SOCKET_SUCCESS)
			break;

		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		Sleep(BLOCKED_SNDRCV_SLEEP);
		// 注册FD_WRITE事件  
		if( WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, FD_WRITE) == SOCKET_ERROR)
		{
			CloseHandle(hWriteEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hWriteEvent, TRUE,dwTimeout, TRUE);

		if( dwWaitResult != WSA_WAIT_EVENT_0 )
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hWriteEvent,&NetEvent) == SOCKET_ERROR)
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		if(NetEvent.iErrorCode[FD_WRITE_BIT] !=0 )	// 发生错误
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(NetEvent.iErrorCode[FD_WRITE_BIT]);
			return (SOCKET_ERROR);
		}
		////////////////////////////////////////////////////////////////
		// 注销事件
		WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
	}

	CloseHandle(hWriteEvent);
	return ((int) dwRtxBytes);
}
 
//关闭套接字
//入口:套接字,是否强行关闭(如果bHardClose==FALSE,那么接收剩余的数据后关闭连接)
void CEcBusSocket::CloseSocket(SOCKET hSocket, BOOL bHardClose)
{
	// 不需要捕获错误
	if (bHardClose == FALSE) // 优雅关闭 Graceful close
	{
		// 不再发送数据，对于TCP套接字，在所有的数据都发送完毕之后，
		// 将发送一个 FIN ，通知接收方所有数据已经发送完毕。
		shutdown(hSocket, SD_SEND);

		// 接收缓冲区有可能还有未接收的数据，在关闭套接字之前应该先
		// 读取残留的数据。
		int		nRecvResult;
		HANDLE	hSocketEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		//为残留数据提供的缓冲区
		char	szBuffer[256];
		do
		{
			if (hSocketEvent != NULL)
			{
				//注册网络事件
				WSAEventSelect(hSocket,(WSAEVENT) hSocketEvent, FD_READ | FD_CLOSE);
				WSAWaitForMultipleEvents(1,&hSocketEvent, TRUE,SHUTDOWN_RECV_TIMEOUT, TRUE);
				//清除网络事件
				WSAEventSelect(hSocket, (WSAEVENT) hSocketEvent, 0);
			}
			ZeroMemory(szBuffer,256);
			//接收残留数据
			nRecvResult = RecvLL(hSocket, szBuffer, sizeof(szBuffer));
		} while (nRecvResult > 0);

		if (hSocketEvent != NULL)
			CloseHandle(hSocketEvent);
		//不再允许接收和发送
		shutdown(hSocket, SD_BOTH);
	}
	else
		shutdown(hSocket, SD_BOTH);
	// 关闭套接字
	closesocket(hSocket);
}

//阻塞ACCEPT，没有响应不返回
//入口:套接字，主机地址，长度
//出口:正确返回端口号，否则返回INVALID_SOCKET
SOCKET CEcBusSocket::Accept_Block(SOCKET hSocket, struct sockaddr * pSocketAddress,int *nAddrLen)
{
	ASSERT(hSocket!=NULL);
	//int nLengthAddr = sizeof(SOCKADDR);
	SOCKET hAccept = accept(hSocket, pSocketAddress, nAddrLen);
	//如果该端口错误
	if(hAccept == INVALID_SOCKET)
	{
		SetLastError(WSAGetLastError());
	}
	return hAccept;
}

// 绑定套接字
//入口:套接字,绑定的地址信息,长度
//出口:正确0，错误-1
int CEcBusSocket::BindSocket(SOCKET hSocket, struct sockaddr * pSocketAddress, int nAddrLen)
{
	if (bind(hSocket, pSocketAddress, nAddrLen) == SOCKET_ERROR)
	{
		SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
	}
	return (SOCKET_SUCCESS);
}

// 绑定套接字
//入口:套接字,端口号
//出口:正确0,错误-1
int CEcBusSocket::BindSocketEx(SOCKET hSocket,int nPort)
{
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr,sizeof(sockAddr));

	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	sockAddr.sin_port			= htons((u_short)nPort);

	return BindSocket(hSocket,(SOCKADDR *)&sockAddr, sizeof(sockAddr));
}

// 建立连接
//入口:套接字,地址结构,结构长度,超时
//出口:SOCKET_SUCCESS/SOCKET_ERROR
int CEcBusSocket::Connect_Event(SOCKET hSocket, const struct sockaddr * pSocketAddress, int nAddrLen,DWORD dwTimeout)
{
	HANDLE hConnectEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hConnectEvent == NULL)
	{
		SetLastError( (int)GetLastError() );
		return (SOCKET_ERROR);
	}
	// 注册FD_CONNECT事件
	if( WSAEventSelect(hSocket, (WSAEVENT) hConnectEvent, FD_CONNECT) == SOCKET_ERROR)
	{
		CloseHandle(hConnectEvent);
		SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
	}
	int	nConnectResult = WSAConnect(hSocket, pSocketAddress, nAddrLen, NULL, NULL, NULL, NULL);
	int	nConnectError  = WSAGetLastError();
	
	if((nConnectResult==SOCKET_ERROR) && (nConnectError == WSAEWOULDBLOCK))
	{
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hConnectEvent, TRUE,dwTimeout, TRUE);
		if (dwWaitResult != WSA_WAIT_EVENT_0)
		{
			SetLastError( WSAGetLastError() );
			nConnectResult = SOCKET_ERROR;
		}
		else
		{
			WSANETWORKEVENTS NetEvent;
			if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hConnectEvent,&NetEvent) == SOCKET_ERROR)
			{
				SetLastError( WSAGetLastError() );
				nConnectResult = SOCKET_ERROR;
			}
			else if(NetEvent.iErrorCode[FD_CONNECT_BIT] !=0 )	// 发生错误
			{
				SetLastError( NetEvent.iErrorCode[FD_CONNECT_BIT] );
				nConnectResult = SOCKET_ERROR;
			}
			else
				nConnectResult = SOCKET_SUCCESS;
			////////////////////////////////////////////////////////////////
		}
	}

	// 注销网络事件
	WSAEventSelect(hSocket, (WSAEVENT) hConnectEvent, 0);
	CloseHandle(hConnectEvent);
	return (nConnectResult);
}

int CEcBusSocket::Connect_Block(SOCKET hSocket, const struct sockaddr * pSocketAddress, int nAddrLen)
{
	ASSERT(hSocket!=NULL);
	if(hSocket==NULL)
		return SOCKET_ERROR;
	if(connect(hSocket, pSocketAddress, nAddrLen) == SOCKET_ERROR) 
		return SOCKET_ERROR;
	return SOCKET_SUCCESS;
}

//创建具有重叠IO能力的套接字
//入口:协议，协议类型(TCP/U),协议
//出口:返回创建的重叠IO SOCKET
//注意:使用SOCKET()函数创建的套接字默认具有重叠IO能力
SOCKET CEcBusSocket::CreateSocket(int nAddressFamily /*= AF_INET*/, int nType/*= SOCK_STREAM*/, int nProtocol/*= 0*/)
{
	SOCKET hSocket = WSASocket(nAddressFamily, nType, nProtocol, NULL,0,WSA_FLAG_OVERLAPPED);	
	if ( hSocket == INVALID_SOCKET )
	{
		SetLastError( WSAGetLastError() );
		return (INVALID_SOCKET);
	}

	//设置套接字选项
	if ( SOCKET_ERROR == SetSocketOption(hSocket) )	//设置属性失败
	{
		CloseSocket(hSocket, TRUE);
		return (INVALID_SOCKET);
	}
	return (hSocket);
}
// 监听套接字
//入口:套接字,接入的等待队列长度
//出口:SOCKET_ERROR/SOCKET_SUCCESS
int CEcBusSocket::ListenSocket(SOCKET hSocket, int nConnections)
{
	if(listen(hSocket, nConnections) == SOCKET_ERROR)
	{
		SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
	}
	return (SOCKET_SUCCESS);
}
