
#pragma  once 

#include <vector>

class CPingTestDoc;

class CPingTest
{
public:
	CPingTest();
	~CPingTest( );

public:
	//开始收发
	void StartTask( );
	//停止收发
	void StopTask( );

	void Engine( );

	CPingTestDoc *m_pDocument;

	std::vector <CWinThread *> m_arrTask;

	DWORD  m_dwDestIp;
	unsigned int m_nDestPortNum;
	unsigned int m_nLocalPortNum;
 
	HANDLE      m_hStopEvent;
	HANDLE m_hTaskHandle;
 
};

  