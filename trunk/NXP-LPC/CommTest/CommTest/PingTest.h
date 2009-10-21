
#pragma  once 

#include "ipstructs.h"

class CPingTestDoc;

struct CPingReply
{
	in_addr	 Address;		//The IP address of the replier
	unsigned long avgRTT;	//Round Trip time in Milliseconds
	unsigned long maxRTT;	
	unsigned long minRTT;
	int nError;
};

class CPingTask
{
	friend UINT  PingTestTask (LPVOID lpParam);	
public:
	CPingTask( );
	~CPingTask( );
public:
	//开始收发
	void StartTask( );
	//停止收发
	void StopTask( );

	void Engine( );
private:
	CWinThread *m_pPingTsk;
	HANDLE      m_hStopEvent;
	HANDLE      m_hTaskHandle;

	DWORD       m_nThreadID;
	unsigned short m_nSeq;

	unsigned char m_nTTL;
	unsigned char m_nTOS;
	BOOL 	m_bDontFragment;
	SOCKET  m_hSocketRaw;

	unsigned int m_nPacketSize;
	DWORD  m_dwDestIp;
	DWORD  m_dwTimeout;

	CPingReply  m_PingReply;

	unsigned char  *m_pSendBuf;
	unsigned char  *m_pRecvBuf;

	BOOL InitPing( );
	void StopPing( );
	BOOL PingTest( );

	BOOL IsSocketReadible(SOCKET socket, DWORD dwTimeout, BOOL& bReadible);
	USHORT	GenerateIPChecksum(USHORT* pBuffer, int nSize);
	void	FillIcmpData(LPICMP_HEADER pIcmp, int nData);
	int	DecodeResponse(unsigned char* pBuf, int nBytes, sockaddr_in* from) ;
};
 
  