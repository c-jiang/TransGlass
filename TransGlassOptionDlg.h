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
};
