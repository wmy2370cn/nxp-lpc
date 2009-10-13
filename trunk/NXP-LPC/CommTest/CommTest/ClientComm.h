
#pragma  once 


#include "EcBusSocket.h"
#include "DataBuf.h"


class CClientCommDoc;
class CClientSocket : public CEcBusSocket
{
	friend UINT  EthCommTask (LPVOID lpParam);		 
public:
	CClientSocket( );
	~CClientSocket( );
public:
	BOOL Connect ( DWORD dwTimeout = 2000);

	//开始收发
	void StartEngine( );
	//停止收发
	void StopEngine( );

protected:
	SOCKET  m_nSocket;

	CDataBuf  m_SendBuf;

	CClientCommDoc *m_pDocument;

	DWORD  m_dwDestIp;
	unsigned int m_nDestPortNum;
	unsigned int m_nLocalPortNum;

	CWinThread *m_pCommTsk;
	HANDLE      m_hStopEvent;
	HANDLE m_hTaskHandle;

};
