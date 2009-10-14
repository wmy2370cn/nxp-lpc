
#pragma  once 


#include "EcBusSocket.h"
#include "DataBuf.h"
#include "commmsg.h"



class CClientCommDoc;
class CClientComm : public CEcBusSocket
{
	friend UINT  EthCommTask (LPVOID lpParam);		 
public:
	CClientComm( );
	~CClientComm( );
public:
	BOOL Connect ( DWORD dwTimeout = 2000);

	//开始收发
	void StartTask( );
	//停止收发
	void StopTask( );

	void Engine( );

	CDataBuf  m_SendBuf;

protected:
	SOCKET  m_nSocket;

	CCommMsgContainer m_CommMsg;


	CClientCommDoc *m_pDocument;

	DWORD  m_dwDestIp;
	unsigned int m_nDestPortNum;
	unsigned int m_nLocalPortNum;

	CWinThread *m_pCommTsk;
	HANDLE      m_hStopEvent;
	HANDLE m_hTaskHandle;

};
