

#include "stdafx.h" 
#include "PingTest.h"
#include "PingTestDoc.h"
#include  <ws2tcpip.h>
#include "Common.h"
#include "LogDataApi.h"

#define MIN_ICMP_PACKET_SIZE 8    //minimum 8 byte icmp packet (just header)
#define MAX_ICMP_PACKET_SIZE 65500 //Maximum icmp packet size

// #define ICMP_ECHO    0
// #define ICMP_ECHOREPLY    8

#define ICMP_ECHOREPLY	0
#define ICMP_ECHOREQ	8

void CPingTask::InitDefault()
{
	m_pPingTsk = NULL;
	m_hTaskHandle = NULL;
	m_hStopEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_nID = 0;
	m_nSeq = 0;

	m_bDontFragment = FALSE;
	m_hSocketRaw = INVALID_SOCKET;
	m_nPacketSize = MIN_ICMP_PACKET_SIZE;

	m_nTTL = 255;
	m_nTOS = 0;
	m_dwTimeout = 2000;
	m_pSendBuf = NULL;
	m_pRecvBuf = NULL;
}


CPingTask::CPingTask( )
{
	InitDefault();
}

CPingTask::CPingTask( CPingTestDoc *pDoc )
{
	ASSERT(pDoc);
	m_pDocument = pDoc;
	InitDefault();

	m_dwDestIp = pDoc->m_dwDestIp;
	m_nPacketSize = pDoc->m_nPingDataSize ;
}


CPingTask::~CPingTask( )
{

}
void CPingTask::StopPing( )
{
	if (m_hSocketRaw != 0  && m_hSocketRaw != INVALID_SOCKET)
	{
		closesocket( m_hSocketRaw );
		m_hSocketRaw = INVALID_SOCKET;
	}
 	if (m_pRecvBuf)
	{
		delete [] m_pRecvBuf;
		m_pRecvBuf = NULL;
	}
	if(m_pSendBuf)
	{
		delete [] m_pSendBuf;
		m_pSendBuf = NULL;
	}
}

BOOL  CPingTask::PingTest( )
{
	ASSERT(m_hSocketRaw != INVALID_SOCKET && m_hSocketRaw != 0);
	if ( m_hSocketRaw == 0 || m_hSocketRaw == INVALID_SOCKET )
		return FALSE;
	ASSERT(m_dwDestIp != INADDR_NONE && m_dwDestIp != 0);
	if(m_dwDestIp == INADDR_NONE || m_dwDestIp == 0)
		return FALSE;

	int nBufSize = m_nPacketSize + sizeof(ICMP_HEADER);

	sockaddr_in dest;
	memset(&dest,0,sizeof(dest));
	dest.sin_addr.s_addr = htonl( m_dwDestIp );
	dest.sin_family = AF_INET;

	FillIcmpData((LPICMP_HEADER) m_pSendBuf, nBufSize);

	int nWrote = sendto(m_hSocketRaw,(const char*) m_pSendBuf, nBufSize, 0, (sockaddr*)&dest, sizeof(dest));
	if (nWrote == SOCKET_ERROR)
	{//发送是不能出错的,如果发送都出问题的话,那么....
		TRACE(_T("CPing::PingUsingWinsock, sendto failed\n"));

		closesocket(m_hSocketRaw);
		m_hSocketRaw = INVALID_SOCKET;
		return FALSE;
	}
	BOOL bReadable;
	sockaddr_in from;
	int nFromlen = sizeof(from);
	int nRead = 0;

	WSABUF		WSABuff;
	//清空缓冲
	ZeroMemory(&WSABuff,sizeof(WSABUF));

	WSABuff.len = nBufSize;
	WSABuff.buf = (char *) m_pRecvBuf;

	DWORD		dwRtxBytes = 0,
		dwRtxFlags = 0;
	//Allow the specified timeout
	if (IsSocketReadible(m_hSocketRaw, m_dwTimeout, bReadable))
	{
		if (bReadable)
		{
			//Receive the response
		//	nRead = recvfrom(m_hSocketRaw,(char *) m_pRecvBuf, MAX_ICMP_PACKET_SIZE, 0, (sockaddr*)&from, &nFromlen);
			int nRet = WSARecvFrom(m_hSocketRaw, &WSABuff,1,&dwRtxBytes,&dwRtxFlags,(sockaddr*)&from, &nFromlen,NULL,NULL);
		}
		else
		{
			TRACE(_T(" PingUsingWinsock, timeout occured while awaiting recvfrom\n"));
		//	closesocket(m_hSocketRaw);
		//	超时,暂时不关闭socket

		 
			return FALSE;
		}

		if(dwRtxBytes == 0)
		{
			//	closesocket(m_hSocketRaw);
			//	超时,暂时不关闭socket

			return FALSE;
		}

		//Decode the response we got back
		m_PingReply.nError = DecodeResponse(m_pRecvBuf, dwRtxBytes, &from);
		if ( m_PingReply.nError == 11 )
		{//printf("  (TTL Expired)
		}
		else if ( m_PingReply.nError == 3 )
		{//DESTINATION_UNREACHABLE

		}
	
		if(m_PingReply.nError == 0)
			return TRUE;	

	}
	return FALSE;
}


BOOL CPingTask::InitPing( )
{
	//Create the raw socket
	 m_hSocketRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, 0);
	if (m_hSocketRaw == INVALID_SOCKET) 
	{
		TRACE(_T("PingUsingWinsock, Failed to create a raw socket\n"));
		return FALSE;
	}
 
// 	SOCKADDR_IN sockLocalAddress;
// 	ZeroMemory(&sockLocalAddress, sizeof(sockLocalAddress));
// 	sockLocalAddress.sin_family = AF_INET;
// 	sockLocalAddress.sin_port = htons((u_short)0);
// 	sockLocalAddress.sin_addr.s_addr = htonl(INADDR_ANY);

// 	LPHOSTENT lphost;
// 	lphost = gethostbyname(NULL);
// 	if (lphost != NULL)
// 		sockLocalAddress.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
// 	else
// 	{
// 		return FALSE; 
// 	}

// 	if (bind(m_hSocketRaw, (sockaddr*) &sockLocalAddress, sizeof(sockLocalAddress)) == SOCKET_ERROR)
// 	{
// 		TRACE(_T("PingUsingWinsock, Failed to bind to specified address\n"));
// 		return FALSE;
// 	}
 
	//Set the TTL on the socket  
	int nTempTTL = m_nTTL;
	if (setsockopt(m_hSocketRaw, IPPROTO_IP, IP_TTL, (char*) &nTempTTL, sizeof(nTempTTL)) == SOCKET_ERROR)
	{
		TRACE(_T("CPing::PingUsingWinsock, Failed to set the TTL value on the socket\n"));
		return FALSE;
	}

	//Set the TOS on the socket  
	int nTempTos = m_nTOS;
	if (setsockopt(m_hSocketRaw, IPPROTO_IP, IP_TOS, (char*) &nTempTos, sizeof(nTempTos)) == SOCKET_ERROR)
	{
		TRACE(_T("CPing::PingUsingWinsock, Failed to set the Tos value on the socket\n"));
		return FALSE;
	}

	//Set the Don't Fragment flag on the socket  
	if (m_bDontFragment)
	{
		if (setsockopt(m_hSocketRaw, IPPROTO_IP, IP_DONTFRAGMENT, (char*) &m_bDontFragment, sizeof(m_bDontFragment)) == SOCKET_ERROR)
		{
			TRACE(_T("CPing::PingUsingWinsock, Failed to set the Don't Fragment value on the socket\n"));
			return FALSE;
		}
	}

	ASSERT(m_pSendBuf == NULL);
	int nBufSize = m_nPacketSize + sizeof(ICMP_HEADER) +100;
	m_pSendBuf = new unsigned char[nBufSize];
	ASSERT(m_pSendBuf);

	ASSERT(m_pRecvBuf == NULL);
	m_pRecvBuf = new unsigned char[nBufSize];
	ASSERT(m_pRecvBuf);

 

	return TRUE;
}

BOOL CPingTask::IsSocketReadible(SOCKET socket, DWORD dwTimeout, BOOL& bReadible)
{
	timeval timeout = {dwTimeout/1000, dwTimeout % 1000};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	int nStatus = select(0, &fds, NULL, NULL, &timeout);
	if (nStatus == SOCKET_ERROR)
	{
		return FALSE;
	}
	else
	{
		bReadible = !(nStatus == 0);
		return TRUE;
	}
}

//Decode the raw IP packet we get back
unsigned char CPingTask::DecodeResponse( unsigned char* pBuf, int nBytes, sockaddr_in* from )
{
	//Get the current tick count
	LARGE_INTEGER TimerTick;
	VERIFY(QueryPerformanceCounter(&TimerTick));


	LPIP_HEADER pIpHdr = (LPIP_HEADER) pBuf;
	int nIpHdrlen = pIpHdr->h_len * 4; //Number of 32-bit words*4 = bytes

	//Not enough data recieved
	if (nBytes < nIpHdrlen + MIN_ICMP_PACKET_SIZE)
	{
		TRACE(_T("Received too few bytes from %sin ICMP packet\n"), inet_ntoa(from->sin_addr));
		SetLastError(ERROR_UNEXP_NET_ERR);
		return -1;
	}

	//Check it is an ICMP_ECHOREPLY packet
	LPICMP_HEADER pIcmpHdr = (LPICMP_HEADER) (pBuf + nIpHdrlen);
	if (pIcmpHdr->i_type != 0) //type ICMP_ECHOREPLY is 0
	{
		return ((int)pIcmpHdr->i_type);
	}
 
	//Check it is the same id as we sent
	if (pIcmpHdr->i_id != (USHORT)m_nID) 
	{
		TRACE(_T("Received someone else's ICMP packet!\n"));
		SetLastError(ERROR_UNEXP_NET_ERR);
		return -1;
	}
	return pIcmpHdr->i_type;
}

//generate an IP checksum based on a given data buffer
USHORT CPingTask::GenerateIPChecksum(USHORT* pBuffer, int nSize) 
{
	unsigned long cksum = 0;

	while (nSize > 1) 
	{
		cksum += *pBuffer++;
		nSize -= sizeof(USHORT);
	}

	if (nSize) 
		cksum += *(UCHAR*)pBuffer;

	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);
	return (USHORT)(~cksum);
}

//Fill up the ICMP packet with defined values
void CPingTask::FillIcmpData(LPICMP_HEADER pIcmp, int nData)
{
	pIcmp->i_type    = ICMP_ECHOREQ; //ICMP_ECHO type
	pIcmp->i_code    = 0;
	pIcmp->i_id      =  (USHORT) m_nID;
	pIcmp->i_seq     = ++m_nSeq;
	pIcmp->i_cksum   = 0;
	pIcmp->timestamp = GetTickCount();

	//Set up the data which will be sent
	int nHdrSize = sizeof(ICMP_HEADER);
	char* pData = ((char*)pIcmp) + nHdrSize;
	memset(pData, 'E', nData - nHdrSize);

	//Generate the checksum
	pIcmp->i_cksum = GenerateIPChecksum((USHORT*)pIcmp, nData);
}

UINT  PingTestTask (LPVOID lpParam)
{
	CPingTask *pTask = (CPingTask *)lpParam;
	if (pTask == NULL)
		return 0x88;

	Sleep(500);
	CString szLog;
	struct sockaddr_in dst_addr;		
	memset((void*)&dst_addr,0,sizeof(dst_addr));
	dst_addr.sin_addr.s_addr=htonl( pTask->m_dwDestIp ); 
	szLog.Format(_T("PING %d.%d.%d.%d 线程[%d]启动。"),dst_addr.sin_addr.s_net ,dst_addr.sin_addr.s_host ,
		dst_addr.sin_addr.s_lh ,dst_addr.sin_addr.s_impno,pTask->m_nID );

	LogString(szLog.GetBuffer(szLog.GetLength()),NORMAL_STR );
	szLog.ReleaseBuffer();
	BOOL bRet =	pTask->InitPing();
	if (bRet == FALSE)
	{
		szLog.Format(_T("PING %d.%d.%d.%d 线程[%d] 初始化网络错误。"),dst_addr.sin_addr.s_net ,dst_addr.sin_addr.s_host ,
			dst_addr.sin_addr.s_lh ,dst_addr.sin_addr.s_impno,pTask->m_nID );
		LogString(szLog.GetBuffer(szLog.GetLength()),ERR_STR );
		szLog.ReleaseBuffer();

		return 0x88;
	}

	ResetEvent(pTask->m_hStopEvent);
	while(1)
	{
		DWORD dwRtn = WaitForSingleObject(pTask->m_hStopEvent, 0);
		if ( dwRtn == WAIT_OBJECT_0)
		{   //	szLog = "退出函数..................\n";
			OutputDebugString(_T("退出在线函数..................\n"));
			break;
		}	

		 bRet =pTask->PingTest();
		 Sleep(1000);
	}

	pTask->StopPing();

	return 0;
}

void CPingTask::StartTask()
{
	StopTask();
	if (m_pPingTsk == NULL)
	{
		m_pPingTsk = AfxBeginThread( PingTestTask, (LPVOID)(this));
 		m_pPingTsk->m_bAutoDelete = FALSE;
		m_pPingTsk->ResumeThread();

		HANDLE pProcess = ::GetCurrentProcess();
		ASSERT(pProcess);
		//将句柄复制
		BOOL bRet = ::DuplicateHandle(pProcess,m_pPingTsk->m_hThread,pProcess,&m_hTaskHandle,DUPLICATE_SAME_ACCESS,true,DUPLICATE_SAME_ACCESS);
		//保证所复制的句柄要有效
		ASSERT(bRet);
	}
}

//停止收发
void CPingTask::StopTask( )
{
	if( m_hStopEvent )
		SetEvent( m_hStopEvent );

	//等待线程退出
	if ( m_pPingTsk != NULL && m_hTaskHandle)
	{
		Common::WaitForThreadToTerminate(m_hTaskHandle);
		CloseHandle(m_hTaskHandle);
		delete m_pPingTsk;
		m_pPingTsk = NULL;

		OutputDebugString(_T("PING线程退出 \n"));
	}
}
 