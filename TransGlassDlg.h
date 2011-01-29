
// TransGlassDlg.h : header file
//

#pragma once


// Declaration
class ThreadMouseHook;


// CTransGlassDlg dialog
class CTransGlassDlg : public CDialogEx
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

    static const BYTE c_bAlphaDefMin  = 10;
    static const BYTE c_bAlphaDefMax  = 255;
    static const BYTE c_bAlphaDefStep = 10;

    BYTE              m_bAlphaStep;
    BYTE              m_bAlphaMin;
    BYTE              m_bAlphaMax;
    ThreadMouseHook*  m_threadMouseHook;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
    HICON m_hIcon;
    NOTIFYICONDATA m_notifyIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();
    virtual void OnCancel();

    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
    afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnBnClickedBtnTray();
    afx_msg void OnBnClickedBtnOpt();
    DECLARE_MESSAGE_MAP()

private:
    void RegisterHotKeys();
    void UnregisterHotKeys();
    void IncreaseWindowAlpha(CWnd* pHwnd);
    void DecreaseWindowAlpha(CWnd* pHwnd);
    void SetWindowAlpha(CWnd* pHwnd, BYTE bAlpha);
    void UpdateAlphaSteps();
    void InitNotifyIconData();
    void MinimizeToTray();
    CWnd* GetWindowForeground();
    CWnd* GetWindowUnderMouseCursor(LPPOINT lpPt);
};
