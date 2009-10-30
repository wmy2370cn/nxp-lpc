
#pragma  once 

#include "UEServer.h"

class CSvrCommFrm;
class CClientNode
{
public: 
	CClientNode( );
	~CClientNode( );
public:
	ClientContext *m_pClientContext;
	CSvrCommFrm   *m_pFrm;

};

class CSvrComm :public CUEServer
{
 public:
	 CSvrComm( );
	 ~CSvrComm( );
 public:
	 BOOL m_bStart;

	 // Called when a new connection have been established.. 
	 virtual void NotifyNewConnection(ClientContext *pcontext);
	 virtual void NotifyDisconnectedClient(ClientContext *pContext);

};