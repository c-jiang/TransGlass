
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

#define APP_TEXT_HOTKEY_ENABLE      TEXT("Hot Key feature is enabled as following combination:")
#define APP_TEXT_HOTKEY_DISABLE     TEXT("Hot Key feature is disabled.")
#define APP_TEXT_MOUSEWHEEL_ENABLE  TEXT("Mouse Wheel feature is enabled as following combination:")
#define APP_TEXT_MOUSEWHEEL_DISABLE TEXT("Mouse Wheel feature key is disabled.")
#define APP_TEXT_KEY_CTRL           TEXT("<Ctrl>")
#define APP_TEXT_KEY_ALT            TEXT("<Alt>")
#define APP_TEXT_KEY_SHIFT          TEXT("<Shift>")
#define APP_TEXT_KEY_WIN            TEXT("<Win>")
#define APP_TEXT_KEY_NONE           TEXT("(none)")
#define APP_TEXT_KEY_HYPHEN         TEXT(" + ")
#define APP_TEXT_KEY_HOTKEY         TEXT("<Arrow Key Up/Down>")
#define APP_TEXT_KEY_MOUSEWHEEL     TEXT("<Mouse Wheel Up/Down>")


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
