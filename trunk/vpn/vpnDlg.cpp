
#include "stdafx.h"
#include "vpn.h"
#include "vpnDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

CvpnDlg::CvpnDlg(CWnd* pParent)
	: CDialogEx(CvpnDlg::IDD, pParent)
{
	m_uMsgShowBanner = ::RegisterWindowMessage( BANNER_URL );
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ULTRA);
}

void CvpnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTSUPPORT, m_SupportLink);
	DDX_Control(pDX, IDC_COPYRIGHT, m_CopyRightLink);
	DDX_Control(pDX, IDC_IMAGE, m_picCtrl);
	DDX_Control(pDX, IDC_EDITLOGIN, m_login);
	DDX_Control(pDX, IDC_EDITPASSWORD, m_password);
}

BEGIN_MESSAGE_MAP(CvpnDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCREATE, &CvpnDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDEXIT, &CvpnDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_CHECKPASSWORD, &CvpnDlg::OnBnClickedCheckpassword)
	ON_BN_CLICKED(IDC_DIAG, &CvpnDlg::OnBnClickedDiag)
	ON_EN_UPDATE(IDC_EDITLOGIN, &CvpnDlg::OnEnUpdateEditlogin)
	ON_EN_UPDATE(IDC_EDITPASSWORD, &CvpnDlg::OnEnUpdateEditpassword)
END_MESSAGE_MAP()

BOOL CvpnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	CString sVersion = "Version: ";
	sVersion += VERSIONSTRING;
	GetDlgItem(IDC_VERSION)->SetWindowText(sVersion);

	/*CString s1, s2;
	sVersion = XorStr(EXE_URL);
	for(int i = 0; i < sVersion.GetLength(); i++)
	{
		s1.Format("\\x%X", (byte)sVersion[i]);
		s2 += s1;
	}*/

	GetDlgItem(IDC_TEXTSUPPORT)->SetWindowText(XorStr(SUPPORT_TEXT));
	m_SupportLink.SetURL(XorStr(SUPPORT_URL));
	GetDlgItem(IDC_COPYRIGHT)->SetWindowText(XorStr(COPYRIGHT_TEXT));
	m_CopyRightLink.SetURL(XorStr(COPYRIGHT_URL));
	this->SetWindowText(XorStr(TITLE_TEXT));

	LOGFONT lf;                        // Used to create the CFont.
	fnt = GetDlgItem(IDCREATE)->GetFont();
	fnt->GetLogFont(&lf);
	lf.lfHeight = 32;                  // Request a 20-pixel-high font
	fnt32.CreateFontIndirect(&lf);    // Create the font.

	lf.lfHeight = 16;                  // Request a 20-pixel-high font
	fnt16.CreateFontIndirect(&lf);    // Create the font.

	m_login.SetFont(&fnt32);
	m_password.SetFont(&fnt32);
	GetDlgItem(IDC_TEXTLOGIN)->SetFont(&fnt32);
	GetDlgItem(IDC_TEXTPASSWORD)->SetFont(&fnt32);

	GetDlgItem(IDCREATE)->SetFont(&fnt16);
	GetDlgItem(IDEXIT)->SetFont(&fnt16);
	GetDlgItem(IDC_CHECKPASSWORD)->SetFont(&fnt16);
	GetDlgItem(IDC_GROUP)->SetFont(&fnt16);

	m_CopyRightLink.m_bAlwaysUnderlineText = FALSE; 
	m_SupportLink.SetHyperlinkFont(m_SupportLink.GetHyperlinkFontName(), 14, 
								   m_SupportLink.GetHyperlinkFontCharSet(), FALSE, FALSE);

	unsigned int dwThreadId;
    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, CvpnDlg::ThreadStaticEntryPoint, 
											NULL, 0, &dwThreadId);
	CloseHandle(hThread);
	return TRUE;
}

void CvpnDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	m_login.SetFocus();
}

HCURSOR CvpnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD CvpnDlg::CreateRas() 
{
    DWORD dwErr = ERROR_SUCCESS, dwSize = 0, dwEntries = 0, i = 0;
    LPRASENTRYNAME pRasEntryName = NULL;
    LPRASENTRYA pRasEntry = NULL;
	RASCREDENTIALS Credentials;
	CString sConnectionName = CONNECTION_NAME;
	CString sHostName = HOST_NAME;
	CString lpszEntry = NULL;

    dwErr = RasEnumEntries(NULL, NULL, NULL, &dwSize, &dwEntries);
	if (dwErr == ERROR_INVALID_SIZE)
	{
		pRasEntryName = (LPRASENTRYNAME) LocalAlloc(LPTR, sizeof(RASENTRYNAME));
		pRasEntryName->dwSize = sizeof (RASENTRYNAME);
	    dwErr = RasEnumEntries(NULL, NULL, pRasEntryName, &dwSize, &dwEntries);
	}

    if (dwErr == ERROR_BUFFER_TOO_SMALL)
    {
        pRasEntryName = (LPRASENTRYNAME) LocalAlloc(LPTR, dwSize);
        if (pRasEntryName == NULL)
        {
            return -1;
        }

        pRasEntryName->dwSize = sizeof (RASENTRYNAME);
        dwErr = RasEnumEntries(NULL, NULL, pRasEntryName, &dwSize, &dwEntries);
        if (dwErr != ERROR_SUCCESS)
        {
			return dwErr;
        }

		for (i = 0; i < dwEntries; i++)
		{
			lpszEntry = LPSTR(pRasEntryName[i].szEntryName);
			if (lpszEntry != LPSTR("EPAM")) 
			{
				dwErr = RasDeleteEntry(NULL, lpszEntry); // Clean up: delete the new entry
				if (dwErr != ERROR_SUCCESS)
				{
					return dwErr;
				}
			}
		}
	}

    dwErr = RasGetEntryPropertiesA(NULL, "", NULL, &dwSize, NULL, NULL);
    if (dwErr == ERROR_BUFFER_TOO_SMALL)
    {
        pRasEntry = (LPRASENTRYA ) LocalAlloc(LPTR, dwSize);
        if (pRasEntry == NULL)
        {
            return -1;
        }

        pRasEntry->dwSize = dwSize;
        dwErr = RasGetEntryPropertiesA(NULL, "", pRasEntry, &dwSize, NULL, 0);
        if (dwErr != ERROR_SUCCESS)
        {
            return dwErr;
        }
    }

	XorStr(&sConnectionName);
	XorStr(&sHostName);

    pRasEntry = (LPRASENTRYA) LocalAlloc(LPTR, dwSize);
    pRasEntry->dwSize = dwSize;
	pRasEntry->dwfOptions  = !RASEO_RequireCHAP;
    pRasEntry->dwfOptions  = RASEO_ShowDialingProgress | RASEO_ModemLights | RASEO_RemoteDefaultGateway |
							 RASEO_RequireMsCHAP2 | RASEO_SecureLocalFiles ;
    pRasEntry->dwfOptions2 = RASEO2_DisableNbtOverIP | RASEO2_ReconnectIfDropped | RASEO2_Internet;
	pRasEntry->dwfNetProtocols = RASNP_Ip;
    pRasEntry->dwRedialCount = 99;
    pRasEntry->dwRedialPause = 30;
    pRasEntry->dwEncryptionType = ET_Optional;
	pRasEntry->dwType = RASET_Vpn;
	pRasEntry->dwVpnStrategy = VS_PptpOnly;   
    StringCchCopy(pRasEntry->szLocalPhoneNumber, CELEMS(pRasEntry->szLocalPhoneNumber), sHostName);
    StringCchCopy(pRasEntry->szDeviceType, CELEMS(pRasEntry->szDeviceType), DEVICE_TYPE);
    StringCchCopy(pRasEntry->szDeviceName, CELEMS(pRasEntry->szDeviceName), DEVICE_NAME);

    dwErr = RasSetEntryProperties(0, sConnectionName, pRasEntry, dwSize, NULL, NULL);
    if (dwErr != ERROR_SUCCESS)
    {
        return dwErr;
    }

	ZeroMemory(&Credentials, sizeof(RASCREDENTIALS));
	m_login.GetWindowText(Credentials.szUserName, sizeof(Credentials.szUserName));
	m_password.GetWindowText(Credentials.szPassword, sizeof(Credentials.szPassword));
    Credentials.dwSize = sizeof(RASCREDENTIALS);
    Credentials.dwMask = RASCM_UserName | RASCM_Password;
	dwErr = RasSetCredentials(NULL, sConnectionName, &Credentials, FALSE);
  
	if (dwErr != ERROR_SUCCESS)
    {
        return dwErr;
    }
	
	RASAUTODIALENTRY Entry;
	ZeroMemory(&Entry, sizeof(RASAUTODIALENTRY));
	Entry.dwSize = sizeof(RASAUTODIALENTRY);
	LPCTSTR lpEntryAddress = sConnectionName;
	StringCchCopy(Entry.szEntry, CELEMS(Entry.szEntry), sConnectionName);
	dwErr = RasSetAutodialAddress(NULL, 0, &Entry, Entry.dwSize, 1);

    LocalFree(pRasEntry);
	LocalFree(pRasEntryName);
    return ERROR_SUCCESS;
}

void CvpnDlg::ManageCreateButton()
{
	CString sLogin, sPassword;

	m_login.GetWindowText(sLogin);
	m_password.GetWindowText(sPassword);

	if ( (sLogin.GetLength() > 0) && (sPassword.GetLength() > 0) ) 
	{
		GetDlgItem(IDCREATE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCREATE)->EnableWindow(FALSE);
	}
}

void CvpnDlg::OnBnClickedOk()
{
	DWORD dwRet = CreateRas();
	if( dwRet != 0 )
	{
		SetLastError(dwRet);
		ErrorExit("Ошибка при создании соединения.\n");
	}
	else
	{
		CString str;
		str.Format("Соединение %s создано успешно.", XorStr(CONNECTION_NAME));
		MessageBox(str, "Сообщение", MB_ICONINFORMATION | MB_OK);
		if( S_OK != CreateShortCut()) 
		{
			ErrorExit("Ярлык для соединения не создан.\n");
		}
	}
	CDialogEx::OnOK();
}

void CvpnDlg::OnBnClickedExit()
{
	ASSERT(AfxGetMainWnd() != NULL);
    AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

void CvpnDlg::OnBnClickedCheckpassword()
{
	if (m_password.GetPasswordChar() == '*')
	{
		m_password.SetPasswordChar(0);
	}
	else
	{
		m_password.SetPasswordChar('*');
	}
	
	m_login.SetFocus();
	m_password.SetFocus();
}

DWORD CvpnDlg::StartServices(CString *name, CString *msg)
{
	int result = 0;
	ULONG retcnt = 0;

	HRESULT hr = CoInitializeEx( 0, COINIT_MULTITHREADED );
	if ( FAILED( hr ) )
	{
		return -1;
	}

	// setup process-wide security context
	hr = CoInitializeSecurity( NULL, // we're not a server
							   -1, // we're not a server
							   NULL, // dito
							   NULL, // reserved
							   RPC_C_AUTHN_LEVEL_DEFAULT, // let DCOM decide
							   RPC_C_IMP_LEVEL_IMPERSONATE,
							   NULL,
							   EOAC_NONE,
							   NULL );
	if ( FAILED( hr ) )
	{
		CoUninitialize();
		return -1;
	}

	// connect to WMI
	IWbemLocator *locator = NULL;
	hr = CoCreateInstance(	CLSID_WbemLocator, //CLSID_WbemAdministrativeLocator, 
							0, 
							CLSCTX_INPROC_SERVER,
							IID_IWbemLocator, 
							(LPVOID *) &locator );
	if ( FAILED( hr ) )
	{
		CoUninitialize();
		return -1;
	}

	// connect to local service with current credentials
	IWbemServices *service = NULL;
	hr = locator->ConnectServer(L"root\\cimv2", 
								NULL, 
								NULL, 
								NULL,
								WBEM_FLAG_CONNECT_USE_MAX_WAIT,
								NULL, 
								0, 
								&service );
	if ( FAILED ( hr ) )
	{
		locator->Release();
		service->Release();
		CoUninitialize();	
		return -1;
	}


	hr = CoSetProxyBlanket(	service,                        // Indicates the proxy to set
							RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx 
							RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx 
							NULL,                        // Server principal name 
							RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
							RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
							NULL,                        // client identity
							EOAC_NONE );                   // proxy capabilities 

    if (FAILED( hr ))
    {
		locator->Release();
		service->Release();
		CoUninitialize();	
        return -1;               // Program has failed.
    }

	// execute a query
	IEnumWbemClassObject *enumerator = NULL;		//		hr = service->ExecQuery( L"WQL", L"SELECT * FROM Win32_NetworkAdapter WHERE Manufacturer !='Microsoft'",// AND NOT PNPDeviceID LIKE 'ROOT\%'",
	CString query = _T("SELECT * FROM Win32_Service WHERE Name = '") + *name + _T("'");
	hr = service->ExecQuery(L"WQL", _bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator);
    if (FAILED( hr ))
    {
		locator->Release();
		service->Release();
		CoUninitialize();	
        return -1;               // Program has failed.
    }

	hr = CoSetProxyBlanket(	enumerator,                        // Indicates the proxy to set
							RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx 
							RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx 
							NULL,                        // Server principal name 
							RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
							RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
							NULL,                        // client identity
							EOAC_NONE );                   // proxy capabilities 

    if (FAILED( hr ))
    {
		locator->Release();
		service->Release();
		CoUninitialize();	
        return -1;               // Program has failed.
    }

	IWbemClassObject	*pObj = NULL, 
						*pClass = NULL,
						*pInParamsDefinition = NULL, 
						*pClassInstance = NULL,
						*pOutParams = NULL;
	BSTR ClassName = SysAllocString(L"Win32_Service");
	BSTR MethodName = SysAllocString(L"StartService");
	hr = service->GetObject(ClassName, 0, NULL, &pClass, NULL);
	hr = pClass->GetMethod(MethodName, 0, &pInParamsDefinition, NULL);
	if (pInParamsDefinition)
		hr = pInParamsDefinition->SpawnInstance(0, &pClassInstance);

	hr = enumerator->Next(WBEM_INFINITE, 1L, &pObj, &retcnt);
	if ( SUCCEEDED( hr ))
	{
		if ( retcnt > 0 )
		{
			_variant_t var_val;
			hr = pObj->Get( L"Caption", 0, &var_val, NULL, NULL );
			if ( SUCCEEDED( hr ) )
			{
				_bstr_t str = var_val;
				*msg = _T("Название сервиса: ");
				*msg += (LPCTSTR) str;
			}
			hr = pObj->Get( L"Started", 0, &var_val, NULL, NULL );
			if ( SUCCEEDED( hr ) )
			{
				_bstr_t str = "NULL";
				if(var_val.boolVal) 
					*msg += ", Состояние: Запущен";
				else
					*msg += ", Состояние: Остановлен";
				if (!var_val.boolVal)
				{
					pObj->Get(L"__PATH", 0, &var_val, NULL, NULL);
					_bstr_t InstancePath = var_val.bstrVal;
					// Execute Method
					hr = service->ExecMethod(InstancePath, MethodName, 0, NULL, pClassInstance, &pOutParams, NULL);
					if ( SUCCEEDED( hr ) )
					{
						hr = pOutParams->Get(_bstr_t(L"ReturnValue"), 0, &var_val, NULL, NULL);
						_bstr_t str = "NULL";
						if(var_val.vt != VT_NULL) str = var_val;
						*msg += ", Попытка запустить сервис. Результат: ";
						*msg += (str == _bstr_t("0")) ? "Запущен" : "Ошибка";
					}
				}
			}
		}
	}

    locator->Release();
    service->Release();
    CoUninitialize();	
	return result;
}

DWORD CvpnDlg::GetMACaddress()
{
	CString sStr, sTemp;
    DWORD dwSize = 0, dwRetVal = 0, i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_INET;
    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

	outBufLen = sizeof (IP_ADAPTER_ADDRESSES);
    pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);

    // Make an initial call to GetAdaptersAddresses to get the size needed into the outBufLen variable
    if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
        FREE(pAddresses);
        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
    }

    if (pAddresses == NULL) 
	{
        return -1;
    }

	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if (dwRetVal == NO_ERROR) 
	{
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
		TCHAR hostName[255];
		gethostname(hostName, sizeof(hostName) );
		sStr.Format("Имя компьютера: %s\n\n\n", hostName);
        while (pCurrAddresses) 
		{
			sTemp.Format("Имя Адаптера: %wS\nMAC адрес адаптера: ", pCurrAddresses->Description);
			sStr += sTemp;

			if (pCurrAddresses->PhysicalAddressLength != 0) 
			{
                for (i = 0; i < pCurrAddresses->PhysicalAddressLength; i++) 
				{
                    if (i == (pCurrAddresses->PhysicalAddressLength - 1))
						sTemp.Format("%.2X", (int) pCurrAddresses->PhysicalAddress[i]);
                    else
						sTemp.Format("%.2X-", (int) pCurrAddresses->PhysicalAddress[i]);
					sStr += sTemp;
                }
            }

			sTemp.Format("\nCтатус соединения: %s\n\n\n", 
					pCurrAddresses->OperStatus == IfOperStatusUp ? "Установлено." : "Не установлено.");
			sStr += sTemp;

            pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast != NULL) 
			{
                for (i = 0; pUnicast != NULL; i++)
				{
                    pUnicast = pUnicast->Next;
				}
			}
			pCurrAddresses = pCurrAddresses->Next;
		}
		MessageBox(sStr, "Список сетевых адаптеров", MB_ICONINFORMATION | MB_OK);
	}
	return 0;
}

void CvpnDlg::OnBnClickedDiag()
{
	if(InternetOkay())
	{
		MessageBox("Соединение с интернетом работает без ошибок.", "Соединение", MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		MessageBox("Соединение с интернетом отсутствует.", "Соединение", MB_ICONSTOP | MB_OK);
	}

	CString name = "TapiSrv", msg = "", str = ""; //"TapiSrv"
	if(StartServices(&name, &msg) == 0)	str += msg + "\n";
	name = "RasMan";
	if(StartServices(&name, &msg) == 0)	str += msg + "\n";
	name = "Dhcp";
	if(StartServices(&name, &msg) == 0)	str += msg + "\n";
	name = "Dnscache";
	if(StartServices(&name, &msg) == 0)	str += msg + "\n";

	MessageBox(str, "Диагностика сервисов", MB_ICONINFORMATION | MB_OK);

	GetMACaddress();
}

DWORD CvpnDlg::AutoUpdate()
{
	DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
	DWORD dwRet, dwContext = 1,  nRead = 1, dwErr = 0, dwServiceType = 0;
	DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
	LPDWORD dRemoteBuffer;
	TCHAR sRemoteVersion[] = "\0\0\0\0\0\0\0\0\0\0\0\0";
	TCHAR *MyFullProgramName= NULL;
	CString sServerName, sObject;
	CHttpConnection* pServer = NULL;   
	CHttpFile* pFile = NULL;
	INTERNET_PORT nPort;

	CInternetSession session(NULL, dwContext, dwAccessType);

	if (!AfxParseURL(XorStr(VERSION_URL), dwServiceType, sServerName, sObject, nPort) || 
		dwServiceType != INTERNET_SERVICE_HTTP)
	{
		pFile->Close();
		pServer->Close();
		session.Close();
		return -1;
	}

	pServer = session.GetHttpConnection(sServerName, nPort);
	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
									sObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
	pFile->AddRequestHeaders("text\r\n");
	TRY
	{
		pFile->SendRequest();
	}
	CATCH(CInternetException, ex)
	{
	}
	END_CATCH

	pFile->QueryInfoStatusCode(dwRet);

	if (dwRet == HTTP_STATUS_OK)
	{
		nRead = pFile->Read((void *)&sRemoteVersion, 12);
		if (CompareVersion(sRemoteVersion))
		{
			if(MessageBox("Доступна новая версия программы. \nОбновить ?", 
						  "Сообщение", MB_ICONINFORMATION | MB_YESNO) == IDYES)
			{
				pFile->Close();
				pServer->Close();

				if (!AfxParseURL(XorStr(EXE_URL), dwServiceType, sServerName, sObject, nPort) || 
					dwServiceType != INTERNET_SERVICE_HTTP)
				{
					pFile->Close();
					pServer->Close();
					session.Close();
					return -1;
				}

				pServer = session.GetHttpConnection(sServerName, nPort);
				pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
												sObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
				pFile->AddRequestHeaders("text\r\n");
				TRY
				{
					pFile->SendRequest();
				}
				CATCH(CInternetException, ex)
				{
				}
				END_CATCH

				pFile->QueryInfoStatusCode(dwRet);

				if (dwRet == HTTP_STATUS_OK)
				{
					dRemoteBuffer = (LPDWORD) LocalAlloc(LPTR, TRANSFER_SIZE);
					CFile lNewVersion(EXE_NEW_NAME, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

					while(nRead > 0 )
					{
						nRead = pFile->Read(dRemoteBuffer, TRANSFER_SIZE);
						if( nRead > 0 )
						{
							lNewVersion.Write(dRemoteBuffer, nRead);
						}
					}
					lNewVersion.Close();
					pFile->Close();
					pServer->Close();
					session.Close();		

					_get_pgmptr(&MyFullProgramName);
					MoveFileEx(MyFullProgramName, EXE_OLD_NAME, MOVEFILE_REPLACE_EXISTING);
					dwErr = GetLastError();
					if( dwErr != 0 ) ErrorExit("");
					else 
					{
						MoveFileEx(EXE_NEW_NAME, EXE_NAME, MOVEFILE_REPLACE_EXISTING);
						dwErr = GetLastError();
						if( dwErr != 0 ) ErrorExit("");
						else 
						{
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

void CvpnDlg::ErrorExit(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code
    LPVOID lpMsgBuf, lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%sКод ошибки: %d\nОписание ошибки: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox((LPCTSTR)lpDisplayBuf, "Ошибка", MB_ICONSTOP | MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

void CvpnDlg::XorStr(CString* source)
{
	for (int i = 0; i < source->GetLength(); i++)
	{
		source->SetAt(i, source->GetAt(i) ^ '$');
	}
}

CString CvpnDlg::XorStr(CString source)
{
	for (int i = 0; i < source.GetLength(); i++)
	{
		source.SetAt(i, source[i] ^ '$');
	}
	return source;
}

BOOLEAN CvpnDlg::CompareVersion(TCHAR cVersion[])
{
	CString sRemoteVersion = cVersion, sExeVersion = VERSIONSTRING;
	sRemoteVersion.Replace(".", "");
	sExeVersion.Replace(".", "");
	if (atoi(sRemoteVersion) > atoi(sExeVersion)) return TRUE;
	return FALSE;
}

HRESULT CvpnDlg::CreateShortCut()
{
	HRESULT hRes = CoInitializeEx(NULL, COINIT_SPEED_OVER_MEMORY);
    if (SUCCEEDED(hRes))
    {
	    IShellLink* psl;
		hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
								IID_IShellLink, (LPVOID*)&psl);
	    if (SUCCEEDED(hRes))
		{
	        IPersistFile* ppf;
			hRes = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
			if (SUCCEEDED(hRes))
			{
				ITEMIDLIST* pidl1 = NULL;
				hRes = SHGetFolderLocation(NULL, CSIDL_CONNECTIONS, NULL, 0, &pidl1);
				if (SUCCEEDED(hRes))
				{
					IShellFolderPtr desktop, ncfolder;
					SHGetDesktopFolder(&desktop);
					desktop->BindToObject(pidl1, NULL, IID_IShellFolder, (void**)&ncfolder);

					IEnumIDListPtr items;
					ncfolder->EnumObjects(NULL, SHCONTF_NONFOLDERS, &items);
					ITEMIDLIST* pidl2 = NULL;
					while (S_OK == items->Next(1, &pidl2, NULL))
					{
						STRRET sr = {STRRET_WSTR};
						ncfolder->GetDisplayNameOf(pidl2, SHGDN_NORMAL, &sr);
						TCHAR buf[MAX_PATH] = _T("");
						StrRetToBuf(&sr, pidl2, buf, MAX_PATH);

						if (0 == _tcsicmp(buf, _T(XorStr(CONNECTION_NAME))))
						{
							ITEMIDLIST* pidl3 = ILCombine(pidl1, pidl2);
							hRes = psl->SetIDList(pidl3);
							if (SUCCEEDED(hRes))
							{
								TCHAR szDeskTopPath[MAX_PATH];
								hRes = SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, szDeskTopPath);
								if (SUCCEEDED(hRes))
								{
									strcat_s(szDeskTopPath, "\\");
									strcat_s(szDeskTopPath, XorStr(CONNECTION_NAME));
									strcat_s(szDeskTopPath, ".lnk");

									WCHAR wcShortCutPath[MAX_PATH];
									MultiByteToWideChar(CP_ACP, 0, szDeskTopPath, -1, wcShortCutPath, MAX_PATH);
									hRes = ppf->Save(wcShortCutPath, TRUE);
									ppf->Release();
								}
							}		        
							ILFree(pidl3);
							ILFree(pidl2);
							break;
						}
						ILFree(pidl2);
						pidl2 = NULL;
					}
					ILFree(pidl1);
				}
			}
			psl->Release();
		}
    }
	return hRes;
}

BOOLEAN CvpnDlg::LoadBanner(CvpnDlg *pthX)
{
	DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
	DWORD dwRet, dwErr = 0, dwServiceType = 0, dwContext = 1, dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
	ULONG nRead = 1;
	BYTE* pData;
	CString sHeaders = _T("text\r\n"), sServerName, sObject, sServer;
	CHttpConnection* pServer = NULL;   
	CHttpFile* pFile = NULL;
	INTERNET_PORT nPort;
	BOOLEAN lbRet = FALSE;

	CInternetSession session(NULL, dwContext, dwAccessType);
	{
		if (!AfxParseURL(XorStr(BANNER_URL), dwServiceType, sServerName, sObject, nPort) 
			|| dwServiceType != INTERNET_SERVICE_HTTP)
		{
			pFile->Close();
			pServer->Close();
			session.Close();
			return FALSE;
		}

		pServer = session.GetHttpConnection(sServerName, nPort);
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, 
										sObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		pFile->AddRequestHeaders(sHeaders);
		TRY
		{
			pFile->SendRequest();
		}
		CATCH(CInternetException, ex)
		{
		}
		END_CATCH

		pFile->QueryInfoStatusCode(dwRet);
		
		if (dwRet == HTTP_STATUS_OK)
		{
			pData = (BYTE*) LocalAlloc(LPTR, TRANSFER_SIZE);
			DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
			if(dwResult != S_OK)
			{
				ErrorExit("");
				pFile->Close();
				pServer->Close();
				session.Close();		
				return FALSE;
			}

			while(nRead = pFile->Read(pData, TRANSFER_SIZE))
			{
				dwResult = m_pStream->Write(pData, nRead, NULL);
				if(dwResult != S_OK)
				{
					ErrorExit("");
					pFile->Close();
					pServer->Close();
					session.Close();		
					//m_pStream->Release();
					return FALSE;
				}
			}
			LocalFree(pData);
			lbRet = TRUE;
		}
		pFile->Close();
		pServer->Close();
		session.Close();		
		return lbRet;
	}
}

BOOL CvpnDlg::PreTranslateMessage( LPMSG pMsg )
{
	if(  ( pMsg->message != 0 ) && ( pMsg->message == m_uMsgShowBanner ) )
		return OnShowBannerMessage( pMsg );	
	return CDialogEx::PreTranslateMessage( pMsg );	
}

BOOL CvpnDlg::OnShowBannerMessage( LPMSG pMsg )
{
	if (m_picCtrl.LoadFromStream(m_pStream))
	{
		m_pStream->Release();
		m_picCtrl.ShowWindow(TRUE);
		m_uMsgShowBanner = NULL;
	}
	return TRUE;
}

BOOL CvpnDlg::PostShowBannerMessage( WPARAM wParam, LPARAM lParam )
{
	DWORD dwReceipents = BSM_APPLICATIONS;

	LONG lRet = ::BroadcastSystemMessage( BSF_IGNORECURRENTTASK | BSF_FORCEIFHUNG | 
					BSF_POSTMESSAGE, &dwReceipents, m_uMsgShowBanner, wParam, lParam );
	return (BOOL)( lRet != -1 );
}

unsigned __stdcall CvpnDlg::ThreadStaticEntryPoint(void *pThis)
{
	CvpnDlg *pthX = (CvpnDlg*)pThis;   // the tricky cast
	pthX->ThreadEntryPoint(pthX);    // now call the true entry-point-function
	return 0;
}

DWORD CvpnDlg::RunAnotherInstance() 
{
	STARTUPINFO si; 
	PROCESS_INFORMATION pi; 
	memset(&si, 0, sizeof(si)); 
	memset(&pi, 0, sizeof(pi)); 
	si.cb = sizeof(si); 

	if (CreateProcess(EXE_NAME, NULL, NULL, NULL, FALSE, 
								NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
		return 1;
	}
	return 0;
}

void CvpnDlg::ThreadEntryPoint(CvpnDlg *pthX)
{
	TRY
	{
		if( AutoUpdate() == 1)
		{
			RunAnotherInstance();
		}
		else
		{
			if (LoadBanner(pthX))
			{
				PostShowBannerMessage( NULL, NULL );
			}
			DeleteFile(EXE_OLD_NAME);
		}
	}
	CATCH(CInternetException, ex)
	{
	}
	END_CATCH
}

void CvpnDlg::OnEnUpdateEditlogin()
{
	int nS1, nS2;
	CString str;
	m_login.GetWindowText(str);
	m_login.GetSel(nS1, nS2);

	for (int n = 0; n < str.GetLength(); ++n)
	{
		if(!((((str[n] >= 'A') && (str[n] <= 'Z')) || 
			((str[n] >= '0') && (str[n] <= '9')))  &&
			(str.GetLength() < 16) ))
		{
			m_login.SetWindowText(m_strOldLogin);
			m_login.SetSel(nS1 - 1, nS2 - 1);
			return;
		}
	}
	ManageCreateButton();
	m_strOldLogin = str;
}

void CvpnDlg::OnEnUpdateEditpassword()
{
	int nS1, nS2;
	CString str;
	m_password.GetWindowText(str);
	m_password.GetSel(nS1, nS2);

	for (int n = 0; n < str.GetLength(); ++n)
	{
		if(!((((str[n] >= 'A') && (str[n] <= 'Z')) || 
			((str[n] >= '0') && (str[n] <= '9')))  &&
			(str.GetLength() < 16) ))
		{
			m_password.SetWindowText(m_strOldPwd);
			m_password.SetSel(nS1 - 1, nS2 - 1);
			return;
		}
	}
	ManageCreateButton();
	m_strOldPwd = str;
}

bool CvpnDlg::InternetOkay()
{
	// Important step - ensure we have an internet connection. We don't want to force a dial-up.
	DWORD dwType;
	if (!InternetGetConnectedState(&dwType, 0))
	{
		return false;
	}
	return true;
}
