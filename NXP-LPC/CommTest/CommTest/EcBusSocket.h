//功能描述:该类是SOCKET通讯综合模型，该类完成后将包装各种SOCKET
//         通讯模式(阻塞select,异步窗口,异步事件,重叠I/O,完成I/O)
//         等.它可以被各种通讯程序方便采用.尤其是基于SERVER/CLIENT
//         的SERVER程序会提高大幅度性能.如果使用人要完成特定的通讯
//         功能可以从该类继承，增加对线程池的支持功能。

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <winsock2.h>

//宏
#define Min(a, b)               (((a) < (b)) ? (a): (b))
#define Max(a, b)               (((a) > (b)) ? (a): (b))
#define INext(i, n)             ((((i) + 1) < (n)) ? ((i) + 1): 0)
#define IPrev(i, n)             (((i) > 0) ? ((i) - 1): ((n) - 1))

const int SOCKET_SUCCESS = 0;//与SOCKET_ERROR对应的成功标志
const DWORD	SHUTDOWN_RECV_TIMEOUT = 2*1000;  // 完美关闭时延迟2秒
const DWORD DEFAULT_TIMEOUT = 3*1000;// 3秒 缺省超时
const int SOCKET_BUFFER_SIZE	= 5192;// 缓冲区缺省大小
const DWORD CONNECT_TIMEOUT	= 3*1000;// 3秒 连接超时
const DWORD SEND_TIMEOUT		= 1*1000;// 1秒 发送超时
const DWORD RECV_TIMEOUT		= 2*1000;// 2秒 接收超时 
const int _EOF						= 0x100;// 无效接收字符
const DWORD BLOCKED_SNDRCV_SLEEP	= 100; // 100毫秒(发生阻塞时等待[睡眠]时间)
const int SOCKET_MAJOR_VERSION 	= 2;
const int SOCKET_MINOR_VERSION	= 2;


static	void CleanupLibrary(void);
static int InitLibrary(void);

class CEcBusSocket  
{
public:
//构造函数
	CEcBusSocket();
	virtual ~CEcBusSocket();
private:
	int m_nErrorNo;
	CRITICAL_SECTION m_csErrorNo;

public:
	
private:
	int SetSocketOption(SOCKET hSocket);
	int BlockSocket(SOCKET hSocket, BOOL bBlock);
	//发送一次缓冲区数据，不过有可能没有全部发送
	int Send_Event(SOCKET hSocket, char const * pszBuffer, int nBufferSize, DWORD dwTimeout=SEND_TIMEOUT);
	int SendTo_Event(SOCKET hSocket, const struct sockaddr * pTo,int nAddrLen,char const * pszBuffer, int nBufferSize, DWORD dwTimeout=SEND_TIMEOUT);
	int Send_Block(SOCKET hSocket,char const * pszBuffer, 	int nBufferSize, DWORD dwTimeout=SEND_TIMEOUT);

	int SendTo_Block(SOCKET hSocket, const struct sockaddr * pTo,int nAddrLen,char const * pszBuffer, int nBufferSize, DWORD dwTimeout=SEND_TIMEOUT);
protected:
	//异步
	int RecvLL(SOCKET hSocket, char *pszBuffer, int nBufferSize);
	int SendLL(SOCKET hSocket, char const * pszBuffer, int nBufferSize);

	inline int GetLastError()//获取最近一次操作的错误代码
	{
		return (m_nErrorNo);
	};
	int BindSocket(SOCKET hSocket, struct sockaddr * pSocketAddress, int nAddrLen);
	int BindSocketEx(SOCKET hSocket,int nPort);
	int ListenSocket(SOCKET hSocket, int nConnections);
	SOCKET CreateSocket(int nAddressFamily= AF_INET, int nType=SOCK_STREAM,int nProtocol= 0);
	void CloseSocket(SOCKET hSocket, BOOL bHardClose=FALSE);
	DWORD GetIP(const char* name,BOOL bFixNtDNS= FALSE);

	//(阻塞模型)
	SOCKET Accept_Block(SOCKET hSocket, struct sockaddr * pSocketAddress,int *nAddrLen);
	int Connect_Block(SOCKET hSocket, const struct sockaddr * pSocketAddress, int nAddrLen);
	//发送全部缓冲区数据，直到超时或发送完毕
	int SendData_Block(SOCKET hSocket,char const * pszBuffer, int nBufferSize, DWORD dwTimeout=SEND_TIMEOUT);
	//阻塞接收数据，该函数停止是在该端口被关闭(接收长度为0)，或出错误的时候
	int RecvData_Block(SOCKET hSocket, char *pszBuffer, int nBufferSize, DWORD dwTimeout= RECV_TIMEOUT);
	int RecvDataFrom_Block( SOCKET hSocket, struct sockaddr * pFrom,  int nAddrlen,char *pszBuffer, int nBufferSize, DWORD dwTimeout=RECV_TIMEOUT);

	//(事件模型)一个线程等待一个事件
	int Connect_Event(SOCKET hSocket, const struct sockaddr * pSocketAddress, int nAddrLen,DWORD dwTimeout=CONNECT_TIMEOUT);
	//TCP
	int RecvData_Event(SOCKET hSocket, char *pszBuffer, int nBufferSize, DWORD dwTimeout= RECV_TIMEOUT);
	//UDP
	int RecvDataFrom_Event( SOCKET hSocket, struct sockaddr * pFrom, int nAddrlen,char *pszBuffer, int nBufferSize,   DWORD dwTimeout=RECV_TIMEOUT);
	//发送所有数据或超时
	int SendData_Event(SOCKET hSocket, char const * pszBuffer, int nBufferSize, DWORD dwTimeout=SEND_TIMEOUT);
	//设置错误代码
	inline void SetLastError(int iErrorCode)
	{
		::EnterCriticalSection(&m_csErrorNo);
		m_nErrorNo = iErrorCode;
		::LeaveCriticalSection(&m_csErrorNo);
	};	
};
