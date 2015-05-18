
// EnvConsole.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Splash.h"


// CEnvConsoleApp:
// See EnvConsole.cpp for the implementation of this class
//

class CEnvConsoleApp : public CWinApp
{
public:
	CEnvConsoleApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	CSplash m_splash;
};

extern CEnvConsoleApp theApp;