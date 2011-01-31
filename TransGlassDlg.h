
// TransGlassDlg.h : header file
//

#pragma once

#include "ThreadMouseHook.h"


// Declaration


// CTransGlassDlg dialog
class CTransGlassDlg : public CDialog
{
// Construction
public:
    CTransGlassDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
    enum { IDD = IDD_TRANSGLASS_DIALOG };

    enum eHotKeyId {
        HOTKEY_ID_BEGIN             = 0,
        HOTKEY_ID_WINDOW_ALPHA_INC  = 0,
        HOTKEY_ID_WINDOW_ALPHA_DEC,
        HOTKEY_ID_END
    };

    BYTE                m_bAlphaLowLimit;
    BYTE                m_bAlphaGranularity;
    ThreadMouseHook*    m_threadMouseHook;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
    HICON           m_hIcon;
    NOTIFYICONDATA  m_notifyIcon;

    // Generated message map functions
    virtual BOOL    OnInitDialog();
    virtual BOOL    DestroyWindow();
    virtual void    OnCancel();

    afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg BOOL    OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void    OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void    OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
    afx_msg BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
    afx_msg void    OnTrayiconPopupAbout();
    afx_msg void    OnTrayiconPopupOpt();
    afx_msg void    OnTrayiconPopupShow();
    afx_msg void    OnTrayiconPopupExit();
    afx_msg void    OnBnClickedBtnTray();
    afx_msg void    OnBnClickedBtnOpt();

    DECLARE_MESSAGE_MAP()

protected:
    void  UpdateDlgTextInfo();
    void  RegisterHotKeys();
    void  UnregisterHotKeys();
    void  UpdateSystemReg();
    void  IncreaseWindowAlpha(CWnd* pHwnd);
    void  DecreaseWindowAlpha(CWnd* pHwnd);
    void  SetWindowAlpha(CWnd* pHwnd, BYTE bAlpha);
    void  UpdateLocalAlphaConfig();
    void  InitNotifyIconData();
    void  MinimizeToTray();
    CWnd* GetWindowForeground();
    CWnd* GetWindowUnderMouseCursor(LPPOINT lpPt);
};
