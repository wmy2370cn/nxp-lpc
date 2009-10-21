#pragma once

#include <vector>
#include "PingTest.h"

// CPingTestDoc 文档
const unsigned int UPDATA_VIEW_PING_NORMAL = 1;
//const unsigned int UPDATA_VIEW_CLIENT_PACKET = 2;

class CPingTestDoc : public CDocument
{
	DECLARE_DYNCREATE(CPingTestDoc)

public:
	CPingTestDoc();
	virtual ~CPingTestDoc();

public:
	DWORD m_dwDestIp;
	UINT m_nTaskCnt;
	UINT m_nPingDataSize;
	UINT m_nIntTime;
	BOOL m_bAutoDelay;

	BOOL m_bPinging;

	//开始收发
	void StartTask( );
	//停止收发
	void StopTask( );
	std::vector <CPingTask *> m_arrTask;

private:
	void FreeMem( );


#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCloseDocument();
};
