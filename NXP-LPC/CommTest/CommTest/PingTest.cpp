

#include "stdafx.h" 
#include "PingTest.h"
#include "PingTestDoc.h"
#include  <ws2tcpip.h>
#include "Common.h"
#include "LogDataApi.h"

#define MIN_ICMP_PACKET_SIZE 8    //minimum 8 byte icmp packet (just header)
#define MAX_ICMP_PACKET_SIZE 65500 //Maximum icmp packet size

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
 	m_nPacketSize = MIN_ICMP_PACKET_SIZE;

	m_nTTL = 255;
	m_nTOS = 0;
	m_dwTimeout = 2000;
	 
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

BOOL  CPingTask::PingTest( )
{
	ASSERT(m_dwDestIp != INADDR_NONE && m_dwDestIp != 0);
	if(m_dwDestIp == INADDR_NONE || m_dwDestIp == 0)
		return FALSE;

	DWORD dwAddr = htonl(m_dwDestIp);
	BOOL bRet = FALSE;
	CPingMsg  msg;
	msg.m_Address.S_un.S_addr = dwAddr;
	bRet = PingUsingICMP(dwAddr, m_PingReply,m_nTTL,m_dwTimeout,m_nPacketSize);
//	bRet = PingUsingWinsock(dwAddr,m_PingReply,m_nTTL,m_dwTimeout,m_nPacketSize,m_nSeq,m_nID);
		
	ASSERT(m_pDocument);

	if (m_pDocument)
	{
		msg.m_nStatus = m_PingReply.EchoReplyStatus;
		msg.m_dwRTT = m_PingReply.RTT;
		msg.m_nTaskId = m_nID;
		m_pDocument->m_PingMsgCntnr.AddPingMsg(msg);

		if (bRet == TRUE)
		{
			
			// 		CString szLog;
			// 		szLog.Format(_T("[#%d]Pinging %d.%d.%d.%d %d字节 返回[%d ms]。"),m_nID, m_PingReply.Address.s_net,m_PingReply.Address.s_host ,
			// 			m_PingReply.Address.s_lh ,m_PingReply.Address.s_impno ,m_nPacketSize,m_PingReply.RTT    );
			// 
			// 		LogString(szLog.GetBuffer(szLog.GetLength()),NORMAL_STR );
			// 		szLog.ReleaseBuffer();
		}
		else
		{

		}
	} 
 
	return bRet;
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

	BOOL bRet = FALSE;

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
 