
#pragma  once 


#include "EcBusSocket.h"
#include "DataBuf.h"
#include "commmsg.h"



class CClientCommDoc;
class CClientComm : public CEcBusSocket
{
	friend UINT  TcpClientTask (LPVOID lpParam);	
	friend UINT  UdpClientTask (LPVOID lpParam);	
	friend class CClientCommDoc;
public:
	CClientComm( );
	~CClientComm( );
public:
	BOOL Connect ( DWORD dwTimeout = 2000);
	BOOL ConnectToSvr ( DWORD dwTimeout = 2000);

	BOOL m_bAutoClose;
	unsigned int m_nAutoCloseTime;

	//开始收发
	void StartTask( );
	//停止收发
	void StopTask( );

	void TcpEngine( );
	CCommMsgContainer m_CommMsg;
	CDataBuf  m_SendBuf;

	unsigned long m_nSendPacketCnt;
	unsigned long m_nRecvPacketCnt;

	unsigned long m_nSendByteCnt;
	unsigned long m_nRecvByteCnt;
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
