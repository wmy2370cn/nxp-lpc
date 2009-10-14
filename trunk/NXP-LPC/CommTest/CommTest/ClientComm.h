
#pragma  once 


#include "EcBusSocket.h"
#include "DataBuf.h"
#include "commmsg.h"



class CClientCommDoc;
class CClientComm : public CEcBusSocket
{
	friend UINT  EthCommTask (LPVOID lpParam);	
	friend class CClientCommDoc;
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
	CCommMsgContainer m_CommMsg;
	CDataBuf  m_SendBuf;
protected:
	void ExecMsgCmd( CCommMsg & msg  );

	
	SOCKET  m_nSocket;
	unsigned char *m_pRawBuf;


	CClientCommDoc *m_pDocument;

	DWORD  m_dwDestIp;
	unsigned int m_nDestPortNum;
	unsigned int m_nLocalPortNum;

	CWinThread *m_pCommTsk;
	HANDLE      m_hStopEvent;
	HANDLE m_hTaskHandle;

};
