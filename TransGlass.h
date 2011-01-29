
// TransGlass.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "ProfileHandler.h"


#define APPLICATION_NAME            TEXT("Transparent Glass")
#define APPLICATION_REG_NAME        TEXT("TransGlass")
#define APPLICATION_REG_PATH        TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run")


class CTransGlassApp : public CWinApp
{
public:
    CTransGlassApp();
    virtual ~CTransGlassApp();

// Overrides
public:
    virtual BOOL InitInstance();

// Implementation
    DECLARE_MESSAGE_MAP()

public:
    ProfileHandler* m_pProfileHandler;
    CString         m_szAppPath;
};

extern CTransGlassApp theApp;
