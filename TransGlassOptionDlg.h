#pragma once


// CTransGlassOptionDlg dialog

class CTransGlassOptionDlg : public CDialog
{
    DECLARE_DYNAMIC(CTransGlassOptionDlg)

public:
    CTransGlassOptionDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CTransGlassOptionDlg();

// Dialog Data
    enum { IDD = IDD_OPTION_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()

public:
    BOOL m_bHotKeyEnable;
    BOOL m_bHotKeyCtrl;
    BOOL m_bHotKeyAlt;
    BOOL m_bHotKeyShift;
    BOOL m_bHotKeyWin;

    BOOL m_bMouseWheelEnable;
    BOOL m_bMouseWheelCtrl;
    BOOL m_bMouseWheelAlt;
    BOOL m_bMouseWheelShift;
    BOOL m_bMouseWheelWin;

    BOOL m_bStartMinimized;
    BOOL m_bAutoStartup;
};
