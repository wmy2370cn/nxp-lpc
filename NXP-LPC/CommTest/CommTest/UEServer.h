/****************************************Copyright (c)**************************************************
**                                江苏科技股份有限公司
**                                     事业部
**                                     服务端网络程序    
**
**                                 http://www.xxx.com.cn
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 2 
**描        述:  
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本: v1.00
** 日　期:  
** 描　述: 原始版本
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:  
** 日　期:  
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "SvrCommMsg.h"
#include <vector>
#include <deque>
#include <list>
#include <hash_map>
#include <string>

const int MAX_SEQ_NUM = 5001;

struct ClientContext; 

typedef stdext::hash_map <unsigned int,CSvrCommPacket*> BUFFER_MAP ;
typedef stdext::hash_map <unsigned int,CSvrCommPacket*>::iterator BUFFER_ITER;
typedef std::pair <unsigned int, CSvrCommPacket*> BUFFER_PAIR;

typedef stdext::hash_map <SOCKET,ClientContext*> CONTEXT_MAP ;
typedef stdext::hash_map <SOCKET,ClientContext*>::iterator CONTEXT_ITER;
typedef std::pair <SOCKET,ClientContext*> CONTEXT_PAIR;
// This struct is used to past around some information about the client. 
struct ClientContext
{
	 // The Connection socket. 
	SOCKET				m_nSocket;
	 // The lock used to update and read variabels.
	CCriticalSection m_ContextLock; 
	// Reserved for DisconnectClient if needed. 
	unsigned int			m_nID; 
	// Very Important variable used with ReleaseClientContext. (Avoids Access Violation)
	int					m_nNumberOfPendlingIO; 	 
	// Send in order variables. 
	unsigned int					m_nSendSeqNum;
	unsigned int					m_nCurSendSeqNum;
	BUFFER_MAP						m_SendBufferMap;
	 
	// Read in order variables
 	unsigned int					m_nReadSeqNum ;
	unsigned int					m_nCurReadSeqNum ;
	BUFFER_MAP						m_ReadBufferMap;
 
	// File transfer stuff. 
#ifdef TRANSFERFILEFUNCTIONALITY	
	CFile m_File;
	unsigned int m_iMaxFileBytes;
	unsigned int m_iFileBytes; 
	BOOL m_bFileSendMode;
	BOOL m_bFileReceivedMode;
#endif

	// Package Overlapped Buffer..
	// Used to get a complete package when we have several pending reads. 
	CSvrCommPacket* m_pBuffOverlappedPackage;	

	// Extra info you can put what ever you whant here.. 
	CString m_sReceived;
	int m_iNumberOfReceivedMsg;
	BOOL m_bUpdate;
};
enum IOType 
{
	IOInitialize, // The client just connected
	IORead, // Read from the client. 
	IOReadCompleted, // Read completed
	IOWrite, // Write to the Client
	IOWriteCompleted, // Write Completed.
	IOZeroByteRead, // Read zero Byte from client (dummy for avoiding The System Blocking error) 
	IOZeroReadCompleted, // Read Zero Byte  completed. (se IOZeroByteRead)
	IOTransmitFileCompleted, //TransmitFileCompleted.
	IOPostedPackage, // Used to post Packages into IOCP port. 
};


class CUEServer
{
public:
	CUEServer( );
	~CUEServer( );
public:
// 	static	void CleanupLibrary(void);
// 	static int InitLibrary(void);
	static	bool GetIpList( std::list <std::string> &r_iplist ) ;

	BOOL CreateCompletionPort();
		// ShutDowns The Server. 
	void ShutDown();
	// Starts the server. 
	BOOL Startup();

	// Starts the server, 
	BOOL Start(int nPort=999,int nMaxNumConnections=1201,int nMaxIOWorkers=1,int nOfWorkers=0,int nMaxNumberOfFreeBuffer=100,int iMaxNumberOfFreeContext=50,BOOL bOrderedSend=TRUE, BOOL bOrderedRead=TRUE,int nNumberOfPendlingReads=5);
	CString GetHostIP();
	DWORD m_dwLocalIp;
private:
	void LogString(CString &szLog ,int nStrType);
	BOOL SetupIOWorkers();

	BOOL SetupListner();

	// Used to bin sockets to Completionport. 
	inline BOOL AssociateSocketWithCompletionPort(SOCKET socket, HANDLE hCompletionPort, DWORD dwCompletionKey);
	// Makes tha last peperation for an connection so IOWORKER can start to work with it. 
	inline BOOL AssociateIncomingClientWithContext(SOCKET clientSocket);

	inline void FreeClientContext();
	// Disconnects A client. 
	inline void DisconnectClient(ClientContext* pContext, BOOL bGraceful=FALSE);
	// Used to avoid access violation..
	inline int ExitIOLoop(ClientContext *pContext);
	// Used to avoid access violation..
	inline void EnterIOLoop(ClientContext *pContext);
	// clear the memory of the buffers. Should only be called when no pendling operations are in use. 
	inline void FreeBuffers();

	// Adds nSize bytes to buffer and flush the other buffer. 
	inline BOOL AddAndFlush(CSvrCommPacket *pFromBuff, CSvrCommPacket *pToBuff, unsigned int nSize);
	inline CSvrCommPacket * SplitBuffer(CSvrCommPacket *pBuff, UINT nSize);
	//Closes The Worker Threads
	inline void ShutDownWorkers();
	// Closes The IO Workers
	inline void ShutDownIOWorkers();
protected:
	static UINT WorkerThreadProc(LPVOID pParam);
	static UINT ListnerThreadProc(LPVOID pParam);
	static UINT IOWorkerThreadProc(LPVOID pParam);
	//Signals No more Accepting Connections.. 
	BOOL m_bServerStarted;

	// Add a client Context to hashMap,.
	inline BOOL AddClientContext(ClientContext* mp);
	// Allocates a ClientContext and return a pointer ot it. 
	inline ClientContext* AllocateContext();


	// Used to avoid SYSTEM Blocking Bugg. 
	inline void OnZeroByteReadCompleted(ClientContext *pContext, DWORD dwIoSize,CSvrCommPacket *pOverlapBuff=NULL);
	inline void OnZeroByteRead(ClientContext *pContext,CSvrCommPacket *pOverlapBuff=NULL);

	// Process the internal messages. 
	inline void ProcessIOMessage(CSvrCommPacket *pOverlapBuff, ClientContext* pContext, DWORD dwSize);	
	// Process received Packages 
	inline void ProcessPackage(ClientContext *pContext, DWORD dwIoSize,CSvrCommPacket *pOverlapBuff);

	BOOL ReleaseBuffer(CSvrCommPacket *pBuff);

	// Release buffers. 
	inline void ReleaseBufferMap(BUFFER_MAP *map);

	// Used to clean up the Send and receive hash map. 
	// Deletes the ClientContext or just put it in the FreeClientContext list. 
	inline BOOL ReleaseClientContext(ClientContext *pContext);
	// Aborts A socket without removing it from contextmap.	
	inline void AbortiveClose(ClientContext *mp);



	volatile unsigned int m_NumberOfActiveConnections;
	// Called when a new connection have been established.. 
	virtual void NotifyNewConnection(ClientContext *pcontext);
	// Called when a empty ClientContext structure are allocated. 
	virtual void NotifyNewClientContext(ClientContext *pContext);
	// A client have Disconnected. 
	virtual void NotifyDisconnectedClient(ClientContext *pContext);
	// File send/Transefer Completed. 
	virtual void NotifyFileCompleted(ClientContext *pcontext);
	// A Package have arrived. 
	virtual inline void NotifyReceivedPackage(CSvrCommPacket *pOverlapBuff,int nSize,ClientContext *pContext);
	// An ClientContext is going to be deleted insert more cleanup code if nesseary.  
	virtual void NotifyContextRelease(ClientContext *pContext);
	// An Write have been Completed
	virtual inline void NotifyWriteCompleted(ClientContext *pContext, DWORD dwIoSize,CSvrCommPacket *pOverlapBuff);
	// Used for log
//	virtual void AppendLog(CString msg);

	// Creates a new buffer or returns a buffer from the FreeBufferList and configure the buffer. 
	CSvrCommPacket* AllocateBuffer(int nType);


	// Unlocks the memory used by the overlapped IO, to avoid WSAENOBUFS problem. 
	inline BOOL AZeroByteRead(ClientContext *pContext,CSvrCommPacket *pOverlapBuff);

	// Used by IO Workers. 
	inline void OnWriteCompleted(ClientContext *pContext, DWORD dwIoSize,CSvrCommPacket *pOverlapBuff);
	inline void OnWrite(ClientContext *pContext, DWORD dwIoSize,CSvrCommPacket *pOverlapBuff);
	inline void OnReadCompleted(ClientContext *pContext, DWORD dwIoSize,CSvrCommPacket *pOverlapBuff=NULL);
	inline void OnRead(ClientContext *pContext,CSvrCommPacket *pOverlapBuff=NULL);
	inline void OnInitialize(ClientContext* pContext, DWORD dwIoSize,CSvrCommPacket *pOverlapBuff=NULL);
	inline void OnPostedPackage(ClientContext *pContext,CSvrCommPacket *pOverlapBuff);

	// Do a Asyncorn Send. Never call this function outside of Notifyxxxx(...) functions. 
	inline BOOL ASend(ClientContext *pContext,CSvrCommPacket *pOverlapBuff);
	// Do a Asyncorn Read.
	inline BOOL ARead(ClientContext *pContext,CSvrCommPacket *pOverlapBuff=NULL);

		// Used to  avoid inorder packages (if you are useing more than one I/O Worker Thread)  
	inline CSvrCommPacket * GetNextReadBuffer(ClientContext *pContext,CSvrCommPacket *pBuff=NULL);
	// Used to  avoid inorder packages (if you are useing more than one I/O Worker Thread)  
	inline CSvrCommPacket* GetNextSendBuffer(ClientContext *pContext,CSvrCommPacket *pBuff=NULL);
	inline void IncreaseSendSeqNum(ClientContext *pContext);
	inline void IncreaseReadSeqNum(ClientContext *pContext);

	// Used to avoid inorder Read packages
	inline void MakeOrderdRead(ClientContext *pContext,CSvrCommPacket *pBuff);
	// Error Convertion.. 
	CString ErrorCode2Text(DWORD dw);

private:
	HANDLE	m_hCompletionPort;
	SOCKET	m_nListenSocket;   
	HANDLE	m_hEvent; 
	// Port number. 
	unsigned int m_nPortNumber; 
	// Maximum Number of Connections. 
	unsigned int m_iMaxNumConnections;
	// Number of Workers running intitially. 
	unsigned int m_nOfWorkers;

	//Signals ShutDown
	volatile BOOL m_bShutDown;
	//Signals No more Accepting Connections.. 
	volatile BOOL m_bAcceptConnections;

	CWinThread* m_pListenThread;
	HANDLE m_hListenHandle;

	// IO Worker Thread list. 
	std:: vector <CWinThread *> m_IOWorkerList;
	// Number of IOWorkers running..
	unsigned int m_nIOWorkers;
	// Number of IOWorker. 
	unsigned int m_iMaxIOWorkers;
 
	// Maximum number of Contexts who is in the FreeContextList
	unsigned int m_iMaxNumberOfFreeContext;

	// Maximum number of buffer which is not used. 
	unsigned int m_iMaxNumberOfFreeBuffer;

	CCriticalSection m_FreeContextListLock;
	std::vector <ClientContext *> m_arrFreeContext;
 //	CPtrList m_FreeContextList;


	// Free Buffer List.. 
	CCriticalSection m_FreeBufferListLock;
//	CPtrList m_FreeBufferList;
	std::deque <CSvrCommPacket *> m_arrFreeBuffer;


	// OccupiedBuffer List.. (Buffers that is currently used) 
	CCriticalSection m_BufferListLock;
//	CPtrList m_BufferList;
	std::deque <CSvrCommPacket *> m_arrBuffer;

	CCriticalSection m_ContextMapLock;
	CONTEXT_MAP m_ContextMap;


	// One ipPerConnection 
	BOOL m_bOneIPPerConnection;
	// Number of Pendling Reads, used for performance
	int m_iNumberOfPendlingReads;
	// When Set to TRUE Reads are processed In Order.. 
	BOOL m_bReadInOrder;
	// Make the sends in order. 
	BOOL m_bSendInOrder;
};