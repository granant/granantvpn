#include "stdafx.h"
#include "vpn.h"
#include "vpnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CvpnApp, CWinApp)
	//ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CvpnApp::CvpnApp()
{
}

CvpnApp::~CvpnApp( void )
{	
	m_uMsgCheckInst = NULL;	
	m_uMsgGetClose = NULL;	
}

BOOL CvpnApp::PostInstanceMessage( WPARAM wParam, LPARAM lParam )
{
	//ASSERT( m_uMsgCheckInst != NULL );
	DWORD dwReceipents = BSM_APPLICATIONS; //| BSM_ALLDESKTOPS;

	LONG lRet = ::BroadcastSystemMessage( BSF_IGNORECURRENTTASK | BSF_FORCEIFHUNG | 
					BSF_POSTMESSAGE, &dwReceipents, m_uMsgCheckInst, wParam, lParam );

	return (BOOL)( lRet != -1 );
}

BOOL CvpnApp::PostCloseMessage( WPARAM wParam, LPARAM lParam )
{
	//ASSERT( m_uMsgGetClose != NULL );
	DWORD dwReceipents = BSM_APPLICATIONS; //| BSM_ALLDESKTOPS;

	LONG lRet = ::BroadcastSystemMessage( BSF_IGNORECURRENTTASK | BSF_FORCEIFHUNG | 
					BSF_POSTMESSAGE, &dwReceipents, m_uMsgGetClose, wParam, lParam );

	return (BOOL)( lRet != -1 );
}



BOOL CvpnApp::PreTranslateMessage( LPMSG pMsg )
{
	if( ( pMsg->message != 0 ) && ( pMsg->message == m_uMsgCheckInst ) )
		return OnAnotherInstanceMessage( pMsg );	
	if(  ( pMsg->message != 0 ) && ( pMsg->message == m_uMsgGetClose ) )
		return OnCloseMessage( pMsg );	
	return CWinApp::PreTranslateMessage( pMsg );	
}


BOOL CvpnApp::OnAnotherInstanceMessage( LPMSG pMsg )
{
	if( ::IsWindow( m_pMainWnd->GetSafeHwnd() ) )
	{
		CWnd* pPopupWnd = m_pMainWnd->GetLastActivePopup();
		pPopupWnd->BringWindowToTop();

		if( !m_pMainWnd->IsWindowVisible() )
			m_pMainWnd->ShowWindow( SW_SHOWNORMAL ); 
		else if( m_pMainWnd->IsIconic() )
			m_pMainWnd->ShowWindow( SW_RESTORE );
		
		pPopupWnd->BringWindowToTop();
		pPopupWnd->SetForegroundWindow(); 
	}
	return TRUE;
}

BOOL CvpnApp::OnCloseMessage( LPMSG pMsg )
{
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
	return TRUE;
}


CvpnApp theApp;

BOOL CvpnApp::InitInstance()
{
	DeleteFile(EXE_OLD_NAME);
	ASSERT( m_uMsgCheckInst == NULL ); // Only once
	m_uMsgCheckInst = ::RegisterWindowMessage( EXE_NAME );
	m_uMsgGetClose = ::RegisterWindowMessage( EXE_OLD_NAME );
	
	ASSERT( m_uMsgCheckInst >= 0xC000 && m_uMsgCheckInst <= 0xFFFF );

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(EXE_OLD_NAME, &FindFileData);
	DWORD dwErr = GetLastError();
	if (dwErr != ERROR_FILE_NOT_FOUND)
	{
		FindClose(hFind);
		this->PostCloseMessage( NULL, NULL );
	}

	HANDLE hMutex  = CreateMutex( NULL, FALSE, EXE_NAME);
	DWORD  dwError = GetLastError();	
			
	if( hMutex != NULL )
	{
		ReleaseMutex( hMutex );
		hMutex = NULL;

		// Another instance of application is running:
		if( dwError == ERROR_ALREADY_EXISTS || dwError == ERROR_ACCESS_DENIED )
		{
			if (dwErr == ERROR_FILE_NOT_FOUND)
			{
				this->PostInstanceMessage( NULL, NULL );
				return FALSE;
			}
		}
	}	
	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	// ������� ��������� ��������, � ������, ���� ���������� ���� ��������
	// ������������� ������ �������� ��� �����-���� ��� �������� ����������.
	CShellManager *pShellManager = new CShellManager;

	// ����������� �������������
	// ���� ��� ����������� �� ������������ � ���������� ��������� ������
	// ��������� ������������ �����, ���������� ������� �� ���������
	// ���������� �������� �������������, ������� �� ���������
	// �������� ������ �������, � ������� �������� ���������
	// TODO: ������� �������� ��� ������ �� ���-������ ����������,
	// �������� �� �������� �����������
	SetRegistryKey(_T("��������� ����������, ��������� � ������� ������� ����������"));

	CvpnDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDCREATE)
	{
		// TODO: ������� ��� ��� ��������� �������� ����������� ����
		//  � ������� ������ "��"
	}
	else if (nResponse == IDEXIT)
	{
		// TODO: ������� ��� ��� ��������� �������� ����������� ����
		//  � ������� ������ "������"
	}

	// ������� ��������� ��������, ��������� ����.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ��������� ���������� ���� �������, ���������� �������� FALSE, ����� ����� ���� ����� ��
	//  ���������� ������ ������� ���������� ��������� ����������.
	return FALSE;
}