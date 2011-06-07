
// vpn.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		
#include <Windowsx.h>


class CvpnApp : public CWinApp
{
protected:	

public:
	UINT	m_uMsgCheckInst;
	UINT	m_uMsgGetClose;

	CvpnApp();
	virtual ~CvpnApp( void );

	BOOL PostInstanceMessage( WPARAM wParam, LPARAM lParam );		
	BOOL PostCloseMessage( WPARAM wParam, LPARAM lParam );		

// Переопределение
public:
	virtual BOOL InitInstance();
	virtual BOOL OnAnotherInstanceMessage(LPMSG pMsg);
	virtual BOOL OnCloseMessage(LPMSG pMsg);
	virtual BOOL PreTranslateMessage( LPMSG pMsg );

	DECLARE_MESSAGE_MAP()
};

extern CvpnApp theApp;