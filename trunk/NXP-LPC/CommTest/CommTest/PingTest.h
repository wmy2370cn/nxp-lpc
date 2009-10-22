
#pragma  once 
#include "ping.h"

class CPingTestDoc;
 

class CPingTask :public CPing
{
	friend UINT  PingTestTask (LPVOID lpParam);	
public:
	CPingTask( );
	CPingTask( CPingTestDoc *pDoc );

	~CPingTask( );
public:
	//开始收发
	void StartTask( );
	//停止收发
	void StopTask( );

	void Engine( );
	unsigned short   m_nID;
private:
	CPingTestDoc *m_pDocument;
	CWinThread *m_pPingTsk;
	HANDLE      m_hStopEvent;
	HANDLE      m_hTaskHandle;

	unsigned short m_nSeq;

	unsigned char m_nTTL;
	unsigned char m_nTOS;
	BOOL 	m_bDontFragment;
 	unsigned int m_nPacketSize;
	DWORD  m_dwDestIp;
	DWORD  m_dwTimeout;

	CPingReply  m_PingReply;

 	void InitDefault();

 	BOOL PingTest( ); 
};
 
  