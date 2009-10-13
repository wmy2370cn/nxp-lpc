
#pragma  once 


#include "EcBusSocket.h"

class CClientCommDoc;
class CClientSocket : public CEcBusSocket
{
public:
	CClientSocket( );
	~CClientSocket( );
public:
	BOOL Connect ( DWORD dwTimeout = 2000);
	SOCKET  m_nSocket;

	CClientCommDoc *m_pDocument;

	DWORD  m_dwDestIp;
	unsigned int m_nDestPortNum;
	unsigned int m_nLocalPortNum;


};