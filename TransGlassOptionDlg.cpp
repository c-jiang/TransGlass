// TransGlassOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TransGlass.h"
#include "TransGlassOptionDlg.h"
#include "afxdialogex.h"


// CTransGlassOptionDlg dialog

IMPLEMENT_DYNAMIC(CTransGlassOptionDlg, CDialog)

CTransGlassOptionDlg::CTransGlassOptionDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTransGlassOptionDlg::IDD, pParent)
    , m_bHotKeyEnable(FALSE)
    , m_bHotKeyCtrl(FALSE)
    , m_bHotKeyAlt(FALSE)
    , m_bHotKeyShift(FALSE)
    , m_bHotKeyWin(FALSE)
    , m_bMouseWheelEnable(FALSE)
    , m_bMouseWheelCtrl(FALSE)
    , m_bMouseWheelAlt(FALSE)
    , m_bMouseWheelShift(FALSE)
    , m_bMouseWheelWin(FALSE)
    , m_bStartMinimized(FALSE)
    , m_bAutoStartup(FALSE)
{
}


CTransGlassOptionDlg::~CTransGlassOptionDlg()
{
}


void CTransGlassOptionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Check(pDX, IDC_CHECK_HOTKEY,            m_bHotKeyEnable);
    DDX_Check(pDX, IDC_CHECK_HOTKEY_CTRL,       m_bHotKeyCtrl);
    DDX_Check(pDX, IDC_CHECK_HOTKEY_ALT,        m_bHotKeyAlt);
    DDX_Check(pDX, IDC_CHECK_HOTKEY_SHIFT,      m_bHotKeyShift);
    DDX_Check(pDX, IDC_CHECK_HOTKEY_WIN,        m_bHotKeyWin);

    DDX_Check(pDX, IDC_CHECK_MOUSEWHEEL,        m_bMouseWheelEnable);
    DDX_Check(pDX, IDC_CHECK_MOUSEWHEEL_CTRL,   m_bMouseWheelCtrl);
    DDX_Check(pDX, IDC_CHECK_MOUSEWHEEL_ALT,    m_bMouseWheelAlt);
    DDX_Check(pDX, IDC_CHECK_MOUSEWHEEL_SHIFT,  m_bMouseWheelShift);
    DDX_Check(pDX, IDC_CHECK_MOUSEWHEEL_WIN,    m_bMouseWheelWin);

    DDX_Check(pDX, IDC_CHECK_MINIMIZED,         m_bStartMinimized);
    DDX_Check(pDX, IDC_CHECK_AUTOSTARTUP,       m_bAutoStartup);
}


BEGIN_MESSAGE_MAP(CTransGlassOptionDlg, CDialog)
END_MESSAGE_MAP()


// CTransGlassOptionDlg message handlers
