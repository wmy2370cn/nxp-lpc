
/////////////////////////////////  Includes  //////////////////////////////////
#include "stdafx.h"
#include "ping.h"
#include "atlconv.h" 


/////////////////////////////////  Macros & Statics ///////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef CPING_USE_ICMP
BOOL CPing::sm_bAttemptedIcmpInitialise = FALSE;
lpIcmpCreateFile CPing::sm_pIcmpCreateFile = NULL;
lpIcmpSendEcho CPing::sm_pIcmpSendEcho = NULL;
lpIcmpCloseHandle CPing::sm_pIcmpCloseHandle = NULL;
#endif

__int64 CPing::sm_TimerFrequency = 0;


//Internal class which is used to ensure that the ICMP
//handle and winsock stack is closed upon exit 
class _CPING
{
public:
	_CPING();
	~_CPING();
protected:
#ifdef CPING_USE_ICMP
	HINSTANCE sm_hIcmp;
#endif
	
	friend class CPing;
};

_CPING::_CPING()
{
#ifdef CPING_USE_ICMP
	sm_hIcmp = NULL;
#endif
}

_CPING::~_CPING()
{
#ifdef CPING_USE_ICMP
	if (sm_hIcmp)
	{
		FreeLibrary(sm_hIcmp);
		sm_hIcmp = NULL;
	}
#endif
//	WSACleanup();
}

static _CPING _cpingData;

///////////////////////////////// Implementation //////////////////////////////

#ifdef CPING_USE_ICMP
BOOL CPing::Initialise() const
{
	if (!sm_bAttemptedIcmpInitialise)
	{
		sm_bAttemptedIcmpInitialise = TRUE;
		
		//Initialise the winsock stack
//		WSADATA wsa;
//		if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		{
//			TRACE(_T("Could not negotiate a correct version of WinSock\n"));
//			return FALSE;
		}
		
		//Load up the ICMP library
		_cpingData.sm_hIcmp = LoadLibrary(_T("ICMP.DLL"));
		if (_cpingData.sm_hIcmp == NULL)
		{
			TRACE(_T("Could not load up the ICMP DLL\n"));
			return FALSE;
		}
		
		//Retrieve pointers to the functions in the ICMP dll
		sm_pIcmpCreateFile = (lpIcmpCreateFile) GetProcAddress(_cpingData.sm_hIcmp,"IcmpCreateFile");
		sm_pIcmpSendEcho = (lpIcmpSendEcho) GetProcAddress(_cpingData.sm_hIcmp,"IcmpSendEcho" );
		sm_pIcmpCloseHandle = (lpIcmpCloseHandle) GetProcAddress(_cpingData.sm_hIcmp,"IcmpCloseHandle");
		
		if (sm_pIcmpCreateFile == NULL || sm_pIcmpSendEcho == NULL ||	sm_pIcmpCloseHandle == NULL)
			TRACE(_T("Could not find ICMP functions in the ICMP DLL\n"));
	}
	
	return (sm_pIcmpCreateFile != NULL && sm_pIcmpSendEcho != NULL &&	sm_pIcmpCloseHandle != NULL);
}
#endif //CPING_USE_ICMP

BOOL CPing::IsSocketReadible(SOCKET socket, DWORD dwTimeout, BOOL& bReadible)
{
	timeval timeout = {dwTimeout/1000, dwTimeout % 1000};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	int nStatus = select(0, &fds, NULL, NULL, &timeout);
	if (nStatus == SOCKET_ERROR)
	{
		return FALSE;
	}
	else
	{
		bReadible = !(nStatus == 0);
		return TRUE;
	}
}

#ifdef CPING_USE_ICMP
BOOL CPing::Ping(LPCTSTR pszHostName, CPingReply& pr, UCHAR nTTL, DWORD dwTimeout, UCHAR nPacketSize) const
{
	//For correct operation of the T2A macro, see TN059
	USES_CONVERSION;
	
	//Make sure everything is initialised
	if (!Initialise())
		return FALSE;
	
	ASSERT(_cpingData.sm_hIcmp); //ICMP dll must be open
	
	LPSTR lpszAscii = T2A((LPTSTR) pszHostName);
	//Convert from dotted notation if required
	unsigned long	addr = inet_addr(lpszAscii);
	if (addr == INADDR_NONE)
	{
		//Not a dotted address, then do a lookup of the name
		hostent* hp = gethostbyname(lpszAscii);
		if (hp)
			memcpy(&addr, hp->h_addr, hp->h_length);
		else
		{
			TRACE(_T("Could not resolve the host name %s\n"), pszHostName);
			return FALSE;
		}
	}
	
	//Create the ICMP handle
	HANDLE hIP = sm_pIcmpCreateFile();
	if (hIP == INVALID_HANDLE_VALUE)
	{
		TRACE(_T("Could not get a valid ICMP handle\n"));
		return FALSE;
	}
	
	//Set up the option info structure
	IP_OPTION_INFORMATION OptionInfo;
	ZeroMemory(&OptionInfo, sizeof(IP_OPTION_INFORMATION));
	OptionInfo.Ttl = nTTL;
	
	//Set up the data which will be sent
	unsigned char* pBuf = new unsigned char[nPacketSize];
	memset(pBuf, 'E', nPacketSize);
	
	//Do the actual Ping
	int nReplySize = sizeof(ICMP_ECHO_REPLY) + max(MIN_ICMP_PACKET_SIZE, nPacketSize);
	unsigned char* pReply = new unsigned char[nReplySize];
	ICMP_ECHO_REPLY* pEchoReply = (ICMP_ECHO_REPLY*) pReply;
	DWORD nRecvPackets = sm_pIcmpSendEcho(hIP, addr, pBuf, nPacketSize, &OptionInfo, pReply, nReplySize, dwTimeout);
	
	//Check we got the packet back
	BOOL bSuccess = (nRecvPackets == 1);
	
	//Check the IP status is OK (O is IP Success)
	if (bSuccess && (pEchoReply->Status != 0))
	{
		bSuccess = FALSE;
		SetLastError(pEchoReply->Status);
	}
	
	//Check we got the same amount of data back as we sent
	if (bSuccess)
	{
		bSuccess = (pEchoReply->DataSize == nPacketSize);
		if (!bSuccess)
			SetLastError(ERROR_UNEXP_NET_ERR);
	}
	
	//Check the data we got back is what was sent
	if (bSuccess)
	{
		char* pReplyData = (char*) pEchoReply->Data;
		for (int i=0; i<nPacketSize && bSuccess; i++)
			bSuccess = (pReplyData[i] == 'E');
		
		if (!bSuccess)
			SetLastError(ERROR_UNEXP_NET_ERR);
	}
	
	//Close the ICMP handle
	sm_pIcmpCloseHandle(hIP);
	
	if (bSuccess)
	{
		//Ping was successful, copy over the pertinent info
		//into the return structure
		pr.Address.S_un.S_addr = pEchoReply->Address;
		pr.RTT = pEchoReply->RoundTripTime;
	}
	
	//Free up the memory we allocated
	delete [] pBuf;
	delete [] pReply;
	
	//return the status
	return bSuccess; 
}
BOOL CPing::ExecutePing(LPCTSTR pszHostName, CPingReply& pr, WORD nPacketSize, DWORD dwTimeout, UCHAR nTTL ) 
{
	//For correct operation of the T2A macro, see TN059
	USES_CONVERSION;
 	
	//Make sure everything is initialised
	if (!Initialise())
		return FALSE;
	
	ASSERT(_cpingData.sm_hIcmp); //ICMP dll must be open
	
	LPSTR lpszAscii = T2A((LPTSTR) pszHostName);
	//Convert from dotted notation if required
	unsigned long	addr = inet_addr(lpszAscii);
	if (addr == INADDR_NONE)
	{
		//Not a dotted address, then do a lookup of the name
		hostent* hp = gethostbyname(lpszAscii);
		if (hp)
			memcpy(&addr, hp->h_addr, hp->h_length);
		else
		{
			TRACE(_T("Could not resolve the host name %s\n"), pszHostName);
			return FALSE;
		}
	}
	
	//Create the ICMP handle
	HANDLE hIP = sm_pIcmpCreateFile();
	if (hIP == INVALID_HANDLE_VALUE)
	{
		TRACE(_T("Could not get a valid ICMP handle\n"));
		return FALSE;
	}
	
	//Set up the option info structure
	IP_OPTION_INFORMATION OptionInfo;
	ZeroMemory(&OptionInfo, sizeof(IP_OPTION_INFORMATION));
	OptionInfo.Ttl = nTTL;
	
	//Set up the data which will be sent
//	unsigned char* pBuf = new unsigned char[nPacketSize];
//	memset(pBuf, 'E', nPacketSize);
	memset(m_szBuff, 'E', sizeof(m_szBuff));

	//Do the actual Ping
	int nReplySize = sizeof(ICMP_ECHO_REPLY) + max(MIN_ICMP_PACKET_SIZE, nPacketSize);
	unsigned char* pReply = new unsigned char[nReplySize];
	ICMP_ECHO_REPLY* pEchoReply = (ICMP_ECHO_REPLY*) pReply;
	DWORD nRecvPackets = sm_pIcmpSendEcho(hIP, addr, m_szBuff, nPacketSize, &OptionInfo, pReply, nReplySize, dwTimeout);
	
	//Check we got the packet back
	BOOL bSuccess = (nRecvPackets == 1);
	
	//Check the IP status is OK (O is IP Success)
	if (bSuccess && (pEchoReply->Status != 0))
	{
		bSuccess = FALSE;
		SetLastError(pEchoReply->Status);
	}
	
	//Check we got the same amount of data back as we sent
	if (bSuccess)
	{
		bSuccess = (pEchoReply->DataSize == nPacketSize);
		if (!bSuccess)
			SetLastError(ERROR_UNEXP_NET_ERR);
	}
	
	//Check the data we got back is what was sent
	if (bSuccess)
	{
		char* pReplyData = (char*) pEchoReply->Data;
		for (int i=0; i<nPacketSize && bSuccess; i++)
			bSuccess = (pReplyData[i] == 'E');
		
		if (!bSuccess)
			SetLastError(ERROR_UNEXP_NET_ERR);
	}
	
	//Close the ICMP handle
	sm_pIcmpCloseHandle(hIP);
	
	if (bSuccess)
	{
		//Ping was successful, copy over the pertinent info
		//into the return structure
		pr.Address.S_un.S_addr = pEchoReply->Address;
		pr.RTT = pEchoReply->RoundTripTime;
	}
	
	//Free up the memory we allocated
//	delete [] pBuf;
	delete [] pReply;
	
	//return the status
	return bSuccess; 
}
#endif //CPING_USE_ICMP
