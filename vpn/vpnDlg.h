
// vpnDlg.h : файл заголовка
//
#include "ras.h"
#include "raserror.h"
#pragma comment (lib, "Rasapi32.lib")

#include <strsafe.h>
#include <raserror.h>   // Ras error definitions
#include <Windowsx.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <wbemidl.h>


#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Wbemuuid.lib")

#include "Hyperlink.h"
#include "PictureCtrl.h"
#include "afxinet.h"
#include "targetver.h" 
#include "afxwin.h"
#include "afxmaskededit.h"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#define CELEMS(x) ((sizeof(x))/(sizeof(x[0]))) // Macro for counting maximum characters that will fit into a buffer
#define EXE_OLD_NAME	"vpn_old.exe"
#define EXE_NEW_NAME	"vpn_new.exe"
#define EXE_NAME		"vpn.exe"
#define CONNECTION_NAME	"QHPVE\n^P\nQE$"						//ultra.zt.ua
#define HOST_NAME		"RTJ\nQHPVE\n^P\nQE$"					//vpn.ultra.zt.ua


#ifdef DEBUG
	#define VERSION_URL		"\x4C\x50\x50\x54\x1E\xB\xB\x51\x48\x50\x56\x45\xA\x5E\x50\xA\x51\x45\xB\x52\x54\x4A\xB" // "http://ultra.zt.ua/vpn/"
	#define EXE_URL			"\x4C\x50\x50\x54\x1E\xB\xB\x51\x48\x50\x56\x45\xA\x5E\x50\xA\x51\x45\xB\x52\x54\x4A\xB\x52\x54\x4A\xA\x41\x5C\x41" //"http://ultra.zt.ua/vpn/vpn.exe"
	#define BANNER_URL		"\x4C\x50\x50\x54\x1E\xB\xB\x51\x48\x50\x56\x45\xA\x5E\x50\xA\x51\x45\xB\x46\x45\x4A\x4A\x41\x56\xA\x4E\x54\x43" //"http://ultra.zt.ua/banner.jpg"
#else
	#define VERSION_URL		"\x4C\x50\x50\x54\x1E\xB\xB\x52\x4D\x40\x41\x4B\xA\x51\x48\x50\x56\x45\xA\x5E\x50\xA\x51\x45\xB\x52\x54\x4A\xB" // "http://video.ultra.zt.ua/vpn/"
	#define EXE_URL			"\x4C\x50\x50\x54\x1E\xB\xB\x52\x4D\x40\x41\x4B\xA\x51\x48\x50\x56\x45\xA\x5E\x50\xA\x51\x45\xB\x52\x54\x4A\xB\x52\x54\x4A\xA\x41\x5C\x41" //"http://video.ultra.zt.ua/vpn/vpn.exe"
	#define BANNER_URL		"\x4C\x50\x50\x54\x1E\xB\xB\x52\x4D\x40\x41\x4B\xA\x51\x48\x50\x56\x45\xA\x5E\x50\xA\x51\x45\xB\x46\x45\x4A\x4A\x41\x56\xA\x4E\x54\x43" //"http://video.ultra.zt.ua/banner.jpg"
#endif

#define SUPPORT_TEXT	"\xF6\xE1\xEF\xE1\xF0\xEA\xE9\xFF\x1E\x4\x10\x9\x17\x15\x9\x1C\x11\x8\x4\x68\x4D\x42\x41\x4\xC\x14\x12\x17\xD\x4\x14\x16\x17\x9\x10\x14\x9\x10\x14\x8\x4\x6F\x5D\x4D\x52\x57\x50\x45\x56\x4\xC\x14\x1D\x1C\xD\x4\x14\x14\x10\x9\x12\x17\x9\x16\x1C" //#define SUPPORT_TEXT	"ТЕЛЕФОНЫ: 4-31-85, Life (063) 023-40-40, Kyivstar (098) 004-63-28"
#define SUPPORT_URL		"\x4C\x50\x50\x54\x1E\xB\xB\x52\x54\x4A\xA\x51\x48\x50\x56\x45\xA\x5E\x50\xA\x51\x45\xB" //http://vpn.ultra.zt.ua/
#define COPYRIGHT_TEXT	"\xC\x67\xD\x4\x67\x4B\x54\x5D\x56\x4D\x43\x4C\x50\x4\x3\x16\x14\x15\x15\x4\x71\x68\x70\x76\x65\xA\x7E\x70\xA\x71\x65" //"(C) Copyright '2011 ULTRA.ZT.UA"
#define COPYRIGHT_URL	"\x4C\x50\x50\x54\x1E\xB\xB\x51\x48\x50\x56\x45\xA\x5E\x50\xA\x51\x45\xB" //http://ultra.zt.ua/
#define DEVICE_TYPE		"vpn"
#define DEVICE_NAME		"VPN"
#define TITLE_TEXT		"\xEB\xD4\xCA\xC6\xC4\xCD\xC0\xC1\xD4\x4\xC7\xCA\xD4\xCA\xC0\xC4\x4\xE5\xC1\xD4\xC0\xCC\xD3\xC1\xC6\x4\x71\x68\x70\x76\x65\xA\x7E\x70\xA\x71\x65" //"Провайдер города Бердичев ULTRA.ZT.UA"
#define TRANSFER_SIZE 4096

#pragma once


static IStream* m_pStream;
static UINT		m_uMsgShowBanner;

class CvpnDlg : public CDialogEx
{
public:
	CFont *fnt, fnt16, fnt32;
	CHyperlink m_SupportLink;	
	CMFCLinkCtrl m_CopyRightLink;
	CPictureCtrl m_picCtrl;
	CEdit m_login;
	CEdit m_password;
	CString m_strOldLogin;
	CString m_strOldPwd;
	
	CvpnDlg(CWnd* pParent = NULL);	// constructor
	DWORD CreateRas(); 
	HRESULT CreateShortCut();
	void ManageCreateButton();
	DWORD GetMACaddress();
	void XorStr(CString*);
	CString XorStr(CString source);
	void ErrorExit(LPTSTR lpszFunction);
	BOOLEAN CompareVersion(TCHAR cVersion[]);
	DWORD AutoUpdate();
	DWORD RunAnotherInstance();
	BOOLEAN LoadBanner(CvpnDlg *pthX);
	static unsigned __stdcall ThreadStaticEntryPoint(void * pThis);
	void ThreadEntryPoint(CvpnDlg *pthX);
	BOOL PostShowBannerMessage( WPARAM wParam, LPARAM lParam );		
	virtual BOOL OnShowBannerMessage(LPMSG pMsg);
	virtual BOOL PreTranslateMessage( LPMSG pMsg );
	bool InternetOkay();
	DWORD StartServices(CString *name, CString *msg);
	
	enum { IDD = IDD_VPN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedExit();
	afx_msg void OnStnClickedBanner();
	afx_msg void OnBnClickedCheckpassword();
	afx_msg void OnBnClickedDiag();
	afx_msg void OnEnUpdateEditlogin();
	afx_msg void OnEnUpdateEditpassword();
};
