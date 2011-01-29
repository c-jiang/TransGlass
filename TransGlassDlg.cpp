
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
    , m_pMouseHook(NULL)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    UpdateAlphaSteps();
    m_bInitShow = false;
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
    ON_BN_CLICKED(IDC_BTN_TRAY, &CTransGlassDlg::OnBnClickedBtnTray)
    ON_BN_CLICKED(IDC_BTN_OPT, &CTransGlassDlg::OnBnClickedBtnOpt)
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

    // Register hot keys.
    RegisterHotKey(this->GetSafeHwnd(),
            HOTKEY_ID_WINDOW_ALPHA_INC,
            MOD_CONTROL,// | MOD_ALT,
            VK_UP);
    RegisterHotKey(this->GetSafeHwnd(),
            HOTKEY_ID_WINDOW_ALPHA_DEC,
            MOD_CONTROL,// | MOD_ALT,
            VK_DOWN);

    // Setup the hook here.
    m_pMouseHook = (ThreadMouseHook*)
            AfxBeginThread(RUNTIME_CLASS(ThreadMouseHook));
    m_pMouseHook->EnableHook(m_hWnd);

    // Create the tray icon in the system tray.
    InitNotifyIconData();

    if (! m_bInitShow) {
        PostMessage(WM_SYSCOMMAND, SC_CLOSE);
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}


BOOL CTransGlassDlg::DestroyWindow()
{
    // Unregister hot keys here.
    for (int i = HOTKEY_ID_BEGIN; i < HOTKEY_ID_END; ++i) {
        UnregisterHotKey(this->GetSafeHwnd(), i);
    }

    // Remove the hook here.
    m_pMouseHook->DisableHook();
    delete m_pMouseHook;

    // Remove the icon in the system tray.
    Shell_NotifyIcon(NIM_DELETE, &m_notifyIcon);

    return CDialogEx::DestroyWindow();
}


void CTransGlassDlg::OnCancel()
{
    // Hook: disable ESC for exiting the app.
}


void CTransGlassDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
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

    CWnd* pHwnd  = NULL;

    switch (nHotKeyId) {
    case HOTKEY_ID_WINDOW_ALPHA_INC:
        pHwnd = GetWindowForeground();
        IncreaseWindowAlpha(pHwnd);
        break;
    case HOTKEY_ID_WINDOW_ALPHA_DEC:
        pHwnd = GetWindowForeground();
        DecreaseWindowAlpha(pHwnd);
        break;
    default:
        break;
    }

    TRACE("<<< %s(%u, %u, %u)\n", __FUNCTION__, nHotKeyId, nKey1, nKey2);
}


LRESULT CTransGlassDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
    TRACE(">>> %s(%u, %lu)\n", __FUNCTION__, wParam, lParam);

    if ((lParam == WM_LBUTTONDOWN) || (lParam == WM_RBUTTONDOWN)) {
        // TODO
        ModifyStyleEx(0, WS_EX_TOPMOST);
        ShowWindow(SW_SHOW);
        Shell_NotifyIcon(NIM_DELETE, &m_notifyIcon);
    }

    TRACE("<<< %s(%u, %lu)\n", __FUNCTION__, wParam, lParam);
    return 0;
}


BOOL CTransGlassDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    TRACE("+++ %s(%u, %u, %u, %u)\n",
          __FUNCTION__, nFlags, zDelta, pt.x, pt.y);

    int iScrollValue = (zDelta / 120) * m_bAlphaStep;
    TRACE("+++ iScrollValue=%d\n", iScrollValue);

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


void CTransGlassDlg::OnBnClickedBtnTray()
{
    MinimizeToTray();
}


void CTransGlassDlg::OnBnClickedBtnOpt()
{
    CTransGlassOptionDlg dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
        // TODO: update the profile.
    } else if (nResponse == IDCANCEL) {
    }
}


void CTransGlassDlg::IncreaseWindowAlpha(CWnd* pHwnd)
{
    BYTE bAlpha = 0xFF;

    if (pHwnd) {
        pHwnd->GetLayeredWindowAttributes(NULL, &bAlpha, NULL);
        TRACE("+++ %s AlphaGot=%d\n", __FUNCTION__, bAlpha);
        if (bAlpha < m_bAlphaMax - m_bAlphaStep) {
            SetWindowAlpha(pHwnd, bAlpha + m_bAlphaStep);
        } else {
            SetWindowAlpha(pHwnd, m_bAlphaMax);
        }
    }
}


void CTransGlassDlg::DecreaseWindowAlpha(CWnd* pHwnd)
{
    BYTE bAlpha = 0xFF;

    if (pHwnd) {
        pHwnd->GetLayeredWindowAttributes(NULL, &bAlpha, NULL);
        TRACE("+++ %s AlphaGot=%d\n", __FUNCTION__, bAlpha);
        if (bAlpha >= m_bAlphaMin + m_bAlphaStep) {
            SetWindowAlpha(pHwnd, bAlpha - m_bAlphaStep);
        } else {
            SetWindowAlpha(pHwnd, m_bAlphaMin);
        }
    }
}


void CTransGlassDlg::SetWindowAlpha(CWnd* pHwnd, BYTE bAlpha)
{
    HWND hWnd = pHwnd->GetSafeHwnd();
    if (hWnd) {
        LONG lStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
        if (! (lStyle & WS_EX_LAYERED)) {
            ::SetWindowLong(hWnd, GWL_EXSTYLE, (lStyle | WS_EX_LAYERED));
        }
        pHwnd->SetLayeredWindowAttributes(0, bAlpha, LWA_ALPHA);
    }
}


void CTransGlassDlg::UpdateAlphaSteps()
{
    m_bAlphaStep = c_bAlphaDefStep;
    m_bAlphaMax  = c_bAlphaDefMax;

    int iAlphaMin = (int) c_bAlphaDefMax;
    while (iAlphaMin > (int) c_bAlphaDefMin) {
        iAlphaMin -= (int) m_bAlphaStep;
    }
    m_bAlphaMin = (BYTE) (iAlphaMin + m_bAlphaStep);
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
    _tcscpy_s(m_notifyIcon.szTip, TEXT(APPLICATION_NAME));
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
    CWnd* pWndApp = NULL;
    CWnd* pWndTmp = WindowFromPoint(*lpPt);

    // TODO: Test with QQ, StickNote, etc.
    if (pWndTmp) {
        while (TRUE) {
            pWndApp = pWndTmp->GetParent();
            if (pWndApp) {
                pWndTmp = pWndApp;
            } else {
                break;
            }
        }
        pWndApp = pWndTmp;
    }
    return pWndApp;
}
