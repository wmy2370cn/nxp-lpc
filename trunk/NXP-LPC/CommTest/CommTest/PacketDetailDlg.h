#pragma once


// CPacketDetailDlg 对话框
class CPacket;
class CPacketDetailDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CPacketDetailDlg)

public:
	CPacketDetailDlg(CPacket *pPacket,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPacketDetailDlg();

	CPacket *m_pPacket;

private:
	CBCGPGridCtrl		m_wndGridTree;
	CImageList			m_Images;

	void  UpdateTree( );

	CBCGPGridRow* CreateNewRow ();
// 对话框数据
	enum { IDD = IDD_DLG_PACKET_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	UINT m_nId;
};
