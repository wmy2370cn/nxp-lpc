
#pragma  once 

#include "UEServer.h"

class CSvrCommFrm;
class CClientNode : public ClientContext
{
public: 
	CClientNode( );
	~CClientNode( );
public:
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