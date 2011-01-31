
// TransGlass.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "TransGlass.h"
#include "TransGlassDlg.h"
#include "ProfileHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransGlassApp
BEGIN_MESSAGE_MAP(CTransGlassApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTransGlassApp construction
CTransGlassApp::CTransGlassApp()
    : m_pProfileHandler(NULL)
{
    TCHAR tcPath[MAX_PATH];
    GetModuleFileName(NULL, tcPath, sizeof(tcPath));
    m_szAppPath = tcPath;

    CString szIniPath = m_szAppPath.Mid(0, m_szAppPath.GetLength() - 3)
                      + TEXT("ini");
    m_pProfileHandler = new ProfileHandler(szIniPath);
}


CTransGlassApp::~CTransGlassApp()
{
    if (m_pProfileHandler) {
        delete m_pProfileHandler;
        m_pProfileHandler = NULL;
    }
}


// The one and only CTransGlassApp object
CTransGlassApp theApp;


// CTransGlassApp initialization
BOOL CTransGlassApp::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CreateMutex(NULL, TRUE, APPLICATION_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBox(NULL,
                   TEXT("TransGlass is already running."),
                   APPLICATION_NAME,
                   MB_OK | MB_ICONERROR);
        return FALSE;
    }

    CWinApp::InitInstance();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored

    CTransGlassDlg dlg;
    m_pMainWnd = &dlg;
#if 0
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
    } else if (nResponse == IDCANCEL) {
    }
#else
    dlg.DoModal();
#endif

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}
