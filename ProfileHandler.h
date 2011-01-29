#pragma once

class ProfileHandler
{
public:
    ProfileHandler(CString& szProfilePath);
    ~ProfileHandler();

    void ReadProfile();
    void WriteProfile();
    void GenerateDefaultProfile();

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

protected:
    int GetHotKeyCombination();
    int GetMouseWheelCombination();
    CString Util_Int2CString(int iVal);

    CString m_szProfilePath;
};
