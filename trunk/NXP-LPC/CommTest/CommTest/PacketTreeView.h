#pragma once


// CPacketTreeView 视图

class CPacketTreeView : public CView
{
	DECLARE_DYNCREATE(CPacketTreeView)

protected:
	CPacketTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPacketTreeView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


