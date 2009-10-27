
// CommTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "CommTest.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "CommTestDoc.h"
#include "CommTestView.h"

#include "ClientChildFrm.h"
#include "ClientCommDoc.h"  
#include "ClientOptionView.h"

#include "PingChildFrm.h"
#include "PingTestDoc.h"
#include "PingOptionView.h"

#include "SvrChildFrm.h"
#include "SvrCommDoc.h"
#include "SvrOptionView.h"

#include "LogDataApi.h"
#include  "boost/memory.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCommTestApp

BEGIN_MESSAGE_MAP(CCommTestApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCommTestApp::OnAppAbout)
END_MESSAGE_MAP()


// CCommTestApp ����

CCommTestApp::CCommTestApp() :
CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{

	m_bHiColorIcons = TRUE;
	m_pDocTemplate = NULL;
	m_pPingDocTemplate = NULL;
	m_pClientDocTemplate = NULL;
	m_pSvrDocTemplate  = NULL;
	// TODO: �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	NS_BOOST_MEMORY::enableMemoryLeakCheck();

}

// Ψһ��һ�� CCommTestApp ����

CCommTestApp theApp;


// CCommTestApp ��ʼ��

BOOL CCommTestApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	globalData.SetDPIAware ();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("CommTestStudio"));
//	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	SetRegistryBase (_T("Settings"));

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CBCGPToolTipParams ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(BCGP_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CBCGPToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	RegisterLog( _T("MAIN"));

	m_pDocTemplate = new CMultiDocTemplate(IDR_CommTestTYPE,
		RUNTIME_CLASS(CCommTestDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CCommTestView));
	if (!m_pDocTemplate)
		return FALSE;
	AddDocTemplate(m_pDocTemplate);

	m_pPingDocTemplate = new CMultiDocTemplate(IDR_PING_TEST,
		RUNTIME_CLASS(CPingTestDoc),
		RUNTIME_CLASS(CPingChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CPingOptionView));
	if (!m_pPingDocTemplate)
		return FALSE;
	AddDocTemplate(m_pPingDocTemplate);

	m_pClientDocTemplate = new CMultiDocTemplate(IDR_CLIENT_COMM,
		RUNTIME_CLASS(CClientCommDoc),
		RUNTIME_CLASS(CClientChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CClientOptionView));
	if (!m_pClientDocTemplate)
		return FALSE;

	AddDocTemplate(m_pClientDocTemplate);

	m_pSvrDocTemplate = new CMultiDocTemplate(IDR_CLIENT_COMM,
		RUNTIME_CLASS(CSvrCommDoc),
		RUNTIME_CLASS(CSvrChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CSvrOptionView));
	if (!m_pSvrDocTemplate)
		return FALSE;

	AddDocTemplate(m_pSvrDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������


	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CCommTestApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCommTestApp �Զ������/���淽��

void CCommTestApp::PreLoadState()
{
// 	BOOL bNameValid;
// 	CString strName;
// 	bNameValid = strName.LoadString(IDS_EDIT_MENU);
// 	ASSERT(bNameValid);
// 	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
// 	bNameValid = strName.LoadString(IDS_EXPLORER);
// 	ASSERT(bNameValid);
// 	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CCommTestApp::LoadCustomState()
{
}

void CCommTestApp::SaveCustomState()
{
}

// CCommTestApp ��Ϣ��������




int CCommTestApp::ExitInstance()
{
	// TODO: �ڴ�����ר�ô����/����û���
	BCGCBProCleanUp();
	UnregisterLog( _T("MAIN"));

	return __super::ExitInstance();
}