
// TransGlassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TransGlass.h"
#include "TransGlassDlg.h"
#include "TransGlassOptionDlg.h"
#include "ThreadMouseHook.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_NOTIFYICON           (WM_USER + 101)


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTransGlassDlg dialog

CTransGlassDlg::CTransGlassDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CTransGlassDlg::IDD, pParent)
    , m_bAlphaGranularity (0)
    , m_bAlphaLowLimit    (0)
    , m_threadMouseHook   (NULL)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    UpdateAlphaConfig(0xFF & theApp.m_pProfileHandler->m_iAlphaLowLimit,
                      0xFF & theApp.m_pProfileHandler->m_iAlphaGranularity);
}


void CTransGlassDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTransGlassDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_HOTKEY()
    ON_WM_MOUSEWHEEL()
    ON_MESSAGE(WM_NOTIFYICON, OnNotifyIcon)
    ON_COMMAND(IDC_TRAYICON_POPUP_ABOUT,  &CTransGlassDlg::OnTrayiconPopupAbout)
    ON_COMMAND(IDC_TRAYICON_POPUP_OPT,    &CTransGlassDlg::OnTrayiconPopupOpt)
    ON_COMMAND(IDC_TRAYICON_POPUP_SHOW,   &CTransGlassDlg::OnTrayiconPopupShow)
    ON_COMMAND(IDC_TRAYICON_POPUP_EXIT,   &CTransGlassDlg::OnTrayiconPopupExit)
    ON_BN_CLICKED(IDC_BTN_TRAY, &CTransGlassDlg::OnBnClickedBtnTray)
    ON_BN_CLICKED(IDC_BTN_OPT,  &CTransGlassDlg::OnBnClickedBtnOpt)
END_MESSAGE_MAP()


// CTransGlassDlg message handlers

BOOL CTransGlassDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (! strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // Update dialog text information.
    UpdateDlgTextInfo();

    // Register hot keys.
    if (theApp.m_pProfileHandler->m_bHotKeyEnable) {
        RegisterHotKeys();
    }

    // Setup the hook.
    m_threadMouseHook = (ThreadMouseHook*)
            AfxBeginThread(RUNTIME_CLASS(ThreadMouseHook));
    if (theApp.m_pProfileHandler->m_bMouseWheelEnable) {
        m_threadMouseHook->SetKeyCombination(
                theApp.m_pProfileHandler->m_bMouseWheelCtrl,
                theApp.m_pProfileHandler->m_bMouseWheelAlt,
                theApp.m_pProfileHandler->m_bMouseWheelShift,
                theApp.m_pProfileHandler->m_bMouseWheelWin);
        m_threadMouseHook->EnableHook(m_hWnd);
    }

    // Update system reg.
    UpdateSystemReg();

    // Create the tray icon in the system tray.
    InitNotifyIconData();

    // Minimize the window if required.
    if (theApp.m_pProfileHandler->m_bStartMinimized) {
        PostMessage(WM_SYSCOMMAND, SC_CLOSE);
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}


BOOL CTransGlassDlg::DestroyWindow()
{
    // Unregister hot keys here.
    UnregisterHotKeys();

    // Remove the hook here.
    m_threadMouseHook->DisableHook();
    delete m_threadMouseHook;

    // Remove the icon in the system tray.
    Shell_NotifyIcon(NIM_DELETE, &m_notifyIcon);

    return CDialogEx::DestroyWindow();
}


void CTransGlassDlg::OnCancel()
{
    // Make <ESC> minimize App to tray.
    PostMessage(WM_SYSCOMMAND, SC_CLOSE);
}


void CTransGlassDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        OnTrayiconPopupAbout();
    } else if ((nID & 0xFFF0) == SC_CLOSE) {
        MinimizeToTray();
    } else {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTransGlassDlg::OnPaint()
{
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialogEx::OnPaint();
    }
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTransGlassDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CTransGlassDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
    TRACE(">>> %s(%u, %u, %u)\n", __FUNCTION__, nHotKeyId, nKey1, nKey2);
    CWnd* pHwnd = GetWindowForeground();

    TRACE("--- pWnd:0x%08X\n", pHwnd->GetSafeHwnd());
    switch (nHotKeyId) {
    case HOTKEY_ID_WINDOW_ALPHA_INC:
        IncreaseWindowAlpha(pHwnd);
        break;
    case HOTKEY_ID_WINDOW_ALPHA_DEC:
        DecreaseWindowAlpha(pHwnd);
        break;
    default:
        break;
    }
    TRACE("<<< %s(%u, %u, %u)\n", __FUNCTION__, nHotKeyId, nKey1, nKey2);
}


BOOL CTransGlassDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    int iScrollValue = (zDelta / 120) * m_bAlphaGranularity;
    TRACE("+++ %s(%u, %d, [%ld, %ld])\n",
          __FUNCTION__, nFlags, iScrollValue, pt.x, pt.y);

    CWnd* pWnd = GetWindowUnderMouseCursor(&pt);
    if (pWnd) {
        if (iScrollValue > 0) {
            IncreaseWindowAlpha(pWnd);
        } else if (iScrollValue < 0) {
            DecreaseWindowAlpha(pWnd);
        }
    }

    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


LRESULT CTransGlassDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
    CMenu menu;

    switch (lParam) {
    case WM_LBUTTONDBLCLK:
        OnTrayiconPopupShow();
        break;
    case WM_RBUTTONDOWN:
        if (menu.LoadMenu(IDR_MENU_TRAYICON)) {
            CMenu *pPopup = menu.GetSubMenu(0);
            ASSERT(pPopup != NULL);
#if 0   // Here states the implementation for disabling some menu item.
            pPopup->EnableMenuItem(IDC_TRAYICON_POPUP_SHOW,
                                   MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
            pPopup->EnableMenuItem(IDC_TRAYICON_POPUP_EXIT,
                                   MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
#endif
            CPoint pt;
            GetCursorPos(&pt);
            SetForegroundWindow();
            pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                                   pt.x, pt.y, this);
        }
        break;
    default:
        break;
    }
    return 0;
}


void CTransGlassDlg::OnTrayiconPopupAbout()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}


void CTransGlassDlg::OnTrayiconPopupOpt()
{
    OnBnClickedBtnOpt();
}


void CTransGlassDlg::OnTrayiconPopupShow()
{
    ModifyStyleEx(0, WS_EX_TOPMOST);
    ShowWindow(SW_SHOWNORMAL);
    Shell_NotifyIcon(NIM_DELETE, &m_notifyIcon);
}


void CTransGlassDlg::OnTrayiconPopupExit()
{
    ::PostQuitMessage(0);
}


void CTransGlassDlg::OnBnClickedBtnTray()
{
    MinimizeToTray();
}


void CTransGlassDlg::OnBnClickedBtnOpt()
{
    CTransGlassOptionDlg dlg;
    BOOL* pOptDlgVal[] = {
        &dlg.m_bHotKeyEnable,
        &dlg.m_bHotKeyCtrl,
        &dlg.m_bHotKeyAlt,
        &dlg.m_bHotKeyShift,
        &dlg.m_bHotKeyWin,
        &dlg.m_bMouseWheelEnable,
        &dlg.m_bMouseWheelCtrl,
        &dlg.m_bMouseWheelAlt,
        &dlg.m_bMouseWheelShift,
        &dlg.m_bMouseWheelWin,
        &dlg.m_bStartMinimized,
        &dlg.m_bAutoStartup
    };
    BOOL* pProfileVal[] = {
        &theApp.m_pProfileHandler->m_bHotKeyEnable,
        &theApp.m_pProfileHandler->m_bHotKeyCtrl,
        &theApp.m_pProfileHandler->m_bHotKeyAlt,
        &theApp.m_pProfileHandler->m_bHotKeyShift,
        &theApp.m_pProfileHandler->m_bHotKeyWin,
        &theApp.m_pProfileHandler->m_bMouseWheelEnable,
        &theApp.m_pProfileHandler->m_bMouseWheelCtrl,
        &theApp.m_pProfileHandler->m_bMouseWheelAlt,
        &theApp.m_pProfileHandler->m_bMouseWheelShift,
        &theApp.m_pProfileHandler->m_bMouseWheelWin,
        &theApp.m_pProfileHandler->m_bStartMinimized,
        &theApp.m_pProfileHandler->m_bAutoStartup
    };
    ASSERT(sizeof(pOptDlgVal) == sizeof(pProfileVal));

    // Set the user profile here.
    for (int i = 0; i < sizeof(pOptDlgVal) / sizeof(pOptDlgVal[0]); ++i) {
        *pOptDlgVal[i] = *pProfileVal[i];
    }
    // TODO: Set alpha config values from profile to dialog.

    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
        // Update the user profile here.
        UpdateData(TRUE);
        bool bChanged = false;
        for (int i = 0; i < sizeof(pOptDlgVal) / sizeof(pOptDlgVal[0]); ++i) {
            if (*pOptDlgVal[i] != *pProfileVal[i]) {
                *pProfileVal[i] = *pOptDlgVal[i];
                bChanged = true;
            }
        }
        // TODO: Check alpha config values changed or not.

        if (bChanged) {
            theApp.m_pProfileHandler->WriteProfile();
            // Update hot key setting.
            UnregisterHotKeys();
            if (theApp.m_pProfileHandler->m_bHotKeyEnable) {
                RegisterHotKeys();
            }
            // Update hook setting.
            m_threadMouseHook->DisableHook();
            if (theApp.m_pProfileHandler->m_bMouseWheelEnable) {
                m_threadMouseHook->SetKeyCombination(
                        theApp.m_pProfileHandler->m_bMouseWheelCtrl,
                        theApp.m_pProfileHandler->m_bMouseWheelAlt,
                        theApp.m_pProfileHandler->m_bMouseWheelShift,
                        theApp.m_pProfileHandler->m_bMouseWheelWin);
                m_threadMouseHook->EnableHook(m_hWnd);
            }
            // Update auto startup setting.
            UpdateSystemReg();
            // Update text tips on dialog.
            UpdateDlgTextInfo();
            // Update alpha config.
            UpdateAlphaConfig(0xFF & theApp.m_pProfileHandler->m_iAlphaLowLimit,
                              0xFF & theApp.m_pProfileHandler->m_iAlphaGranularity);
        }
    }
}


void CTransGlassDlg::UpdateDlgTextInfo()
{
    typedef struct {
        UINT        ctrlId;
        CStatic**   ppctrlText;
        CString*    pszText;
    } eItem_UpdateDlgTextInfo;

    CStatic* pHotKeyTip1(NULL);
    CStatic* pHotKeyTip2(NULL);
    CStatic* pMouseWheelTip1(NULL);
    CStatic* pMouseWheelTip2(NULL);
    CString  szHotKeyTip1(TEXT(""));
    CString  szHotKeyTip2(TEXT(""));
    CString  szMouseWheelTip1(TEXT(""));
    CString  szMouseWheelTip2(TEXT(""));

    eItem_UpdateDlgTextInfo internalTbl[] = {
        { IDC_STATIC_HOTKEY_TIP1,     &pHotKeyTip1,     &szHotKeyTip1      },
        { IDC_STATIC_HOTKEY_TIP2,     &pHotKeyTip2,     &szHotKeyTip2      },
        { IDC_STATIC_MOUSEWHEEL_TIP1, &pMouseWheelTip1, &szMouseWheelTip1  },
        { IDC_STATIC_MOUSEWHEEL_TIP2, &pMouseWheelTip2, &szMouseWheelTip2  }
    };
    int iTblSize = sizeof(internalTbl) / sizeof(internalTbl[0]);

    for (int i = 0; i < iTblSize; ++i) {
        *internalTbl[i].ppctrlText = (CStatic*) GetDlgItem(internalTbl[i].ctrlId);
    }

    if (theApp.m_pProfileHandler->m_bHotKeyEnable) {
        szHotKeyTip1 = APP_TEXT_HOTKEY_ENABLE;
        if (theApp.m_pProfileHandler->m_bHotKeyCtrl) {
            szHotKeyTip2 += APP_TEXT_KEY_CTRL;
            szHotKeyTip2 += APP_TEXT_KEY_HYPHEN;
        }
        if (theApp.m_pProfileHandler->m_bHotKeyAlt) {
            szHotKeyTip2 += APP_TEXT_KEY_ALT;
            szHotKeyTip2 += APP_TEXT_KEY_HYPHEN;
        }
        if (theApp.m_pProfileHandler->m_bHotKeyShift) {
            szHotKeyTip2 += APP_TEXT_KEY_SHIFT;
            szHotKeyTip2 += APP_TEXT_KEY_HYPHEN;
        }
        if (theApp.m_pProfileHandler->m_bHotKeyWin) {
            szHotKeyTip2 += APP_TEXT_KEY_WIN;
            szHotKeyTip2 += APP_TEXT_KEY_HYPHEN;
        }
        if (szHotKeyTip2.GetLength() == 0) {
            szHotKeyTip2 += APP_TEXT_KEY_NONE;
            szHotKeyTip2 += APP_TEXT_KEY_HYPHEN;
        }
        szHotKeyTip2 += APP_TEXT_KEY_HOTKEY;
    } else {
        szHotKeyTip1 = APP_TEXT_HOTKEY_DISABLE;
    }

    if (theApp.m_pProfileHandler->m_bMouseWheelEnable) {
        szMouseWheelTip1 = APP_TEXT_MOUSEWHEEL_ENABLE;
        if (theApp.m_pProfileHandler->m_bMouseWheelCtrl) {
            szMouseWheelTip2 += APP_TEXT_KEY_CTRL;
            szMouseWheelTip2 += APP_TEXT_KEY_HYPHEN;
        }
        if (theApp.m_pProfileHandler->m_bMouseWheelAlt) {
            szMouseWheelTip2 += APP_TEXT_KEY_ALT;
            szMouseWheelTip2 += APP_TEXT_KEY_HYPHEN;
        }
        if (theApp.m_pProfileHandler->m_bMouseWheelShift) {
            szMouseWheelTip2 += APP_TEXT_KEY_SHIFT;
            szMouseWheelTip2 += APP_TEXT_KEY_HYPHEN;
        }
        if (theApp.m_pProfileHandler->m_bMouseWheelWin) {
            szMouseWheelTip2 += APP_TEXT_KEY_WIN;
            szMouseWheelTip2 += APP_TEXT_KEY_HYPHEN;
        }
        if (szMouseWheelTip2.GetLength() == 0) {
            szMouseWheelTip2 += APP_TEXT_KEY_NONE;
            szMouseWheelTip2 += APP_TEXT_KEY_HYPHEN;
        }
        szMouseWheelTip2 += APP_TEXT_KEY_MOUSEWHEEL;
    } else {
        szMouseWheelTip1 = APP_TEXT_MOUSEWHEEL_DISABLE;
    }

    for (int i = 0; i < iTblSize; ++i) {
        (*internalTbl[i].ppctrlText)->SetWindowText(*internalTbl[i].pszText);
    }
}


void CTransGlassDlg::RegisterHotKeys()
{
    UINT uiMod = (theApp.m_pProfileHandler->m_bHotKeyCtrl  ? MOD_CONTROL : 0)
               | (theApp.m_pProfileHandler->m_bHotKeyAlt   ? MOD_ALT     : 0)
               | (theApp.m_pProfileHandler->m_bHotKeyShift ? MOD_SHIFT   : 0)
               | (theApp.m_pProfileHandler->m_bHotKeyWin   ? MOD_WIN     : 0);

    RegisterHotKey(this->GetSafeHwnd(),
                   HOTKEY_ID_WINDOW_ALPHA_INC,
                   uiMod,
                   VK_UP);
    RegisterHotKey(this->GetSafeHwnd(),
                   HOTKEY_ID_WINDOW_ALPHA_DEC,
                   uiMod,
                   VK_DOWN);
}


void CTransGlassDlg::UnregisterHotKeys()
{
    for (int i = HOTKEY_ID_BEGIN; i < HOTKEY_ID_END; ++i) {
        UnregisterHotKey(this->GetSafeHwnd(), i);
    }
}


void CTransGlassDlg::UpdateSystemReg()
{
    HKEY hKey;
    if (ERROR_SUCCESS == RegOpenKey(HKEY_CURRENT_USER,
                                    APPLICATION_REG_PATH,
                                    &hKey)) {
        if (theApp.m_pProfileHandler->m_bAutoStartup) {
            CString szStr("Hello,world!");
            RegSetValueEx(hKey,
                          APPLICATION_REG_NAME,
                          0,
                          REG_SZ,
                          (const BYTE*) theApp.m_szAppPath.GetBuffer(),
                          (theApp.m_szAppPath.GetLength() + 1) * sizeof(TCHAR));
        } else {
            RegDeleteValue(hKey, APPLICATION_REG_NAME);
        }
        RegCloseKey(hKey);
    }
}


void CTransGlassDlg::IncreaseWindowAlpha(CWnd* pHwnd)
{
    BYTE bAlpha = 0xFF;

    if (pHwnd) {
        pHwnd->GetLayeredWindowAttributes(NULL, &bAlpha, NULL);
        TRACE("+++ %s AlphaOld=%d\n", __FUNCTION__, bAlpha);
        if ((int) (bAlpha + m_bAlphaGranularity) < (int) m_bAlphaMaxValue) {
            SetWindowAlpha(pHwnd, bAlpha + m_bAlphaGranularity);
            TRACE("+++ %s AlphaNew=%d\n", __FUNCTION__, bAlpha + m_bAlphaGranularity);
        } else {
            SetWindowAlpha(pHwnd, m_bAlphaMaxValue);
            TRACE("+++ %s AlphaNew=AlphaMaxValue\n", __FUNCTION__);
        }
    }
}


void CTransGlassDlg::DecreaseWindowAlpha(CWnd* pHwnd)
{
    BYTE bAlpha = 0xFF;

    if (pHwnd) {
        pHwnd->GetLayeredWindowAttributes(NULL, &bAlpha, NULL);
        TRACE("+++ %s AlphaOld=%d\n", __FUNCTION__, bAlpha);
        if ((int) bAlpha >= (int) (m_bAlphaLowLimit + m_bAlphaGranularity)) {
            SetWindowAlpha(pHwnd, bAlpha - m_bAlphaGranularity);
            TRACE("+++ %s AlphaNew=%d\n", __FUNCTION__, bAlpha - m_bAlphaGranularity);
        } else {
            SetWindowAlpha(pHwnd, m_bAlphaLowLimit);
            TRACE("+++ %s AlphaNew=AlphaLowLimit\n", __FUNCTION__);
        }
    }
}


void CTransGlassDlg::SetWindowAlpha(CWnd* pHwnd, BYTE bAlpha)
{
    HWND hWnd = pHwnd->GetSafeHwnd();
    if (hWnd) {
        LONG lStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
        if (! (lStyle & WS_EX_LAYERED)) {
            lStyle |= WS_EX_LAYERED;
#ifdef DEBUG
            LONG ret = ::SetWindowLong(hWnd, GWL_EXSTYLE, lStyle);
            if (! ret) {
                TRACE("!!! %s - ErrorCode:%lu\n",
                      __FUNCTION__, ::GetLastError());
            } else {
                TRACE("*** %s - NewStyle:0x%08X, PrevStyle:0x%08X\n",
                      __FUNCTION__, lStyle, ret);
            }
#else
            ::SetWindowLong(hWnd, GWL_EXSTYLE, lStyle);
#endif
        }
        pHwnd->SetLayeredWindowAttributes(0, bAlpha, LWA_ALPHA);
    }
}


void CTransGlassDlg::UpdateAlphaConfig(BYTE bLowLimit, BYTE bGranularity)
{
    m_bAlphaLowLimit    = bLowLimit;
    m_bAlphaGranularity = bGranularity;

    int iAlphaMin = (int) m_bAlphaMaxValue;
    while (iAlphaMin >= (int) m_bAlphaLowLimit) {
        iAlphaMin -= (int) m_bAlphaGranularity;
    }
    m_bAlphaLowLimit = (BYTE) (iAlphaMin + m_bAlphaGranularity);
}


void CTransGlassDlg::InitNotifyIconData()
{
    m_notifyIcon.cbSize = (DWORD) sizeof(NOTIFYICONDATA);
    m_notifyIcon.hWnd   = this->m_hWnd;
    m_notifyIcon.uID    = IDR_MAINFRAME;
    m_notifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_notifyIcon.uCallbackMessage = WM_NOTIFYICON;
    m_notifyIcon.hIcon  = LoadIcon(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCE(IDR_MAINFRAME));
    _tcscpy_s(m_notifyIcon.szTip, APPLICATION_NAME);
}


void CTransGlassDlg::MinimizeToTray()
{
    ShowWindow(SW_HIDE);
    Shell_NotifyIcon(NIM_ADD, &m_notifyIcon);
}


CWnd* CTransGlassDlg::GetWindowForeground()
{
    return GetDesktopWindow()->GetForegroundWindow();
}


CWnd* CTransGlassDlg::GetWindowUnderMouseCursor(LPPOINT lpPt)
{
    TRACE(">>> %s([%ld, %ld])\n", __FUNCTION__, lpPt->x, lpPt->y);
    CWnd* pHwnd = WindowFromPoint(*lpPt);
    TRACE("--- pHwnd:0x%08X\n", pHwnd->GetSafeHwnd());

    if (pHwnd) {
        while (TRUE) {
            LONG lStyle = ::GetWindowLong(pHwnd->GetSafeHwnd(), GWL_STYLE);
            if (! (lStyle & WS_CHILDWINDOW)) {
                break;
            } else {
                pHwnd = pHwnd->GetParent();
                if (! pHwnd) {
                    break;
                }
            }
        }
    }
    TRACE("<<< %s([%ld, %ld])\n", __FUNCTION__, lpPt->x, lpPt->y);
#ifdef DEBUG
    if (pHwnd) {
        TRACE("    @@:0x%08X\n", pHwnd->GetSafeHwnd());
    } else {
        TRACE("    @@:NULL\n");
    }
#endif
    return pHwnd;
}
