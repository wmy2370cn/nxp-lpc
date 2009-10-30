

#include "stdafx.h"
#include "SvrComm.h"

#include "Common.h"
#include "LogDataApi.h" 
 

CSvrComm::CSvrComm( )
{
	m_bStart = FALSE;
}

CSvrComm::~CSvrComm( )
{


}

void CSvrComm::NotifyNewConnection(ClientContext *pcontext)
{

}

void CSvrComm::NotifyDisconnectedClient(ClientContext *pContext)
{

}
