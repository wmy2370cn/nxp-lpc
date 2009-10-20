// PingTestDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "PingTestDoc.h"


// CPingTestDoc

IMPLEMENT_DYNCREATE(CPingTestDoc, CDocument)

CPingTestDoc::CPingTestDoc()
{
	m_dwDestIp = 0;
}

BOOL CPingTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPingTestDoc::~CPingTestDoc()
{
}


BEGIN_MESSAGE_MAP(CPingTestDoc, CDocument)
END_MESSAGE_MAP()


// CPingTestDoc 诊断

#ifdef _DEBUG
void CPingTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CPingTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CPingTestDoc 序列化

void CPingTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}
#endif


// CPingTestDoc 命令
