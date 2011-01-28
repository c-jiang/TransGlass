
// TransGlassDlg.h : header file
//

#pragma once


// CTransGlassDlg dialog
class CTransGlassDlg : public CDialogEx
{
// Construction
public:
    CTransGlassDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
    enum { IDD = IDD_TRANSGLASS_DIALOG };

    enum eHotKeyId {
        HOTKEY_ID_01 = 0,
        HOTKEY_ID_02,
        HOTKEY_ID_03,
        HOTKEY_ID_04,
        HOTKEY_ID_MAX
    };

    static const BYTE c_bAlphaDefMin  = 10;
    static const BYTE c_bAlphaDefMax  = 255;
    static const BYTE c_bAlphaDefStep = 10;

    BYTE m_bAlphaStep;
    BYTE m_bAlphaMin;
    BYTE m_bAlphaMax;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();

    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
    DECLARE_MESSAGE_MAP()

    bool GetWindowAlpha(CWnd* pHwnd, BYTE* pbAlpha);
    void SetWindowAlpha(CWnd* pHwnd, BYTE bAlpha);
    void UpdateAlphaSteps();
};
