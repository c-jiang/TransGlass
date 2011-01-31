#include "stdafx.h"
#include "ProfileHandler.h"


#define PROFILE_APPNAME                     TEXT("TransGlass")

#define PROFILE_KEY_HOTKEY_ENABLE           TEXT("HotKeyEnable")
#define PROFILE_KEY_HOTKEY_COMBINATION      TEXT("HotKeyCombination")
#define PROFILE_KEY_MOUSEWHEEL_ENABLE       TEXT("MouseWheelEnable")
#define PROFILE_KEY_MOUSEWHEEL_COMBINATION  TEXT("MouseWheelCombination")
#define PROFILE_KEY_START_MINIMIZED         TEXT("Minimized")
#define PROFILE_KEY_AUTO_STARTUP            TEXT("AutoStartup")
#define PROFILE_KEY_ALPHA_LOW_LIMIT         TEXT("AlphaLowLimit")
#define PROFILE_KEY_ALPHA_GRANULARITY       TEXT("AlphaGranularity")

/**
 * MSB  <Ctrl>   <Alt> <Shift>   <Win>  LSB
 *          8 ..... 4 ..... 2 ..... 1
 */
#define KEY_COMBINATION_CTRL                8
#define KEY_COMBINATION_ALT                 4
#define KEY_COMBINATION_SHIFT               2
#define KEY_COMBINATION_WIN                 1

#define PROFILE_DEF_HOTKEY_ENABLE           1
#define PROFILE_DEF_HOTKEY_COMBINATION      (KEY_COMBINATION_CTRL |     \
                                             KEY_COMBINATION_ALT)
#define PROFILE_DEF_MOUSEWHEEL_ENABLE       1
#define PROFILE_DEF_MOUSEWHEEL_COMBINATION  (KEY_COMBINATION_CTRL |     \
                                             KEY_COMBINATION_ALT)
#define PROFILE_DEF_START_MINIMIZED         1
#define PROFILE_DEF_AUTO_STARTUP            0
#define PROFILE_DEF_ALPHA_LOW_LIMIT         10
#define PROFILE_DEF_ALPHA_GRANULARITY       10


ProfileHandler::ProfileHandler(CString& szProfilePath)
    : m_szProfilePath       (szProfilePath)
    , m_bHotKeyEnable       (FALSE)
    , m_bHotKeyCtrl         (FALSE)
    , m_bHotKeyAlt          (FALSE)
    , m_bHotKeyShift        (FALSE)
    , m_bHotKeyWin          (FALSE)
    , m_bMouseWheelEnable   (FALSE)
    , m_bMouseWheelCtrl     (FALSE)
    , m_bMouseWheelAlt      (FALSE)
    , m_bMouseWheelShift    (FALSE)
    , m_bMouseWheelWin      (FALSE)
    , m_bStartMinimized     (FALSE)
    , m_bAutoStartup        (FALSE)
    , m_iAlphaLowLimit      (PROFILE_DEF_ALPHA_LOW_LIMIT)
    , m_iAlphaGranularity   (PROFILE_DEF_ALPHA_GRANULARITY)
{
    TRACE(">>> %s\n", __FUNCTION__);
    TRACE(TEXT("+++ %s\n"), m_szProfilePath);

    CFileStatus status;

    if (! CFile::GetStatus(m_szProfilePath, status)) {
        GenerateDefaultProfile();
    }
    ReadProfile();

    TRACE("<<< %s\n", __FUNCTION__);
}


ProfileHandler::~ProfileHandler()
{
}


void ProfileHandler::ReadProfile()
{
    int iVal;

    iVal = GetPrivateProfileInt(PROFILE_APPNAME,
                                PROFILE_KEY_HOTKEY_ENABLE,
                                PROFILE_DEF_HOTKEY_ENABLE,
                                m_szProfilePath);
    m_bHotKeyEnable = (BOOL) iVal;

    iVal = GetPrivateProfileInt(PROFILE_APPNAME,
                                PROFILE_KEY_HOTKEY_COMBINATION,
                                PROFILE_DEF_HOTKEY_COMBINATION,
                                m_szProfilePath);
    m_bHotKeyCtrl  = ((iVal & KEY_COMBINATION_CTRL)  != 0);
    m_bHotKeyAlt   = ((iVal & KEY_COMBINATION_ALT)   != 0);
    m_bHotKeyShift = ((iVal & KEY_COMBINATION_SHIFT) != 0);
    m_bHotKeyWin   = ((iVal & KEY_COMBINATION_WIN)   != 0);

    iVal = GetPrivateProfileInt(PROFILE_APPNAME,
                                PROFILE_KEY_MOUSEWHEEL_ENABLE,
                                PROFILE_DEF_MOUSEWHEEL_ENABLE,
                                m_szProfilePath);
    m_bMouseWheelEnable = (BOOL) iVal;

    iVal = GetPrivateProfileInt(PROFILE_APPNAME,
                                PROFILE_KEY_MOUSEWHEEL_COMBINATION,
                                PROFILE_DEF_MOUSEWHEEL_COMBINATION,
                                m_szProfilePath);
    m_bMouseWheelCtrl  = ((iVal & KEY_COMBINATION_CTRL)  != 0);
    m_bMouseWheelAlt   = ((iVal & KEY_COMBINATION_ALT)   != 0);
    m_bMouseWheelShift = ((iVal & KEY_COMBINATION_SHIFT) != 0);
    m_bMouseWheelWin   = ((iVal & KEY_COMBINATION_WIN)   != 0);

    iVal = GetPrivateProfileInt(PROFILE_APPNAME,
                                PROFILE_KEY_START_MINIMIZED,
                                PROFILE_DEF_START_MINIMIZED,
                                m_szProfilePath);
    m_bStartMinimized = (BOOL) iVal;

    iVal = GetPrivateProfileInt(PROFILE_APPNAME,
                                PROFILE_KEY_AUTO_STARTUP,
                                PROFILE_DEF_AUTO_STARTUP,
                                m_szProfilePath);
    m_bAutoStartup = (BOOL) iVal;

    iVal = GetPrivateProfileInt(PROFILE_APPNAME,
                                PROFILE_KEY_ALPHA_LOW_LIMIT,
                                PROFILE_DEF_ALPHA_LOW_LIMIT,
                                m_szProfilePath);
    m_iAlphaLowLimit = iVal;

    iVal = GetPrivateProfileInt(PROFILE_APPNAME,
                                PROFILE_KEY_ALPHA_GRANULARITY,
                                PROFILE_DEF_ALPHA_GRANULARITY,
                                m_szProfilePath);
    m_iAlphaGranularity = iVal;
}


void ProfileHandler::WriteProfile()
{
    TRACE(">>> %s\n", __FUNCTION__);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_HOTKEY_ENABLE,
                              Util_Int2CString(m_bHotKeyEnable),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_HOTKEY_COMBINATION,
                              Util_Int2CString(GetHotKeyCombination()),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_MOUSEWHEEL_ENABLE,
                              Util_Int2CString(m_bMouseWheelEnable),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_MOUSEWHEEL_COMBINATION,
                              Util_Int2CString(GetMouseWheelCombination()),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_START_MINIMIZED,
                              Util_Int2CString(m_bStartMinimized),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_AUTO_STARTUP,
                              Util_Int2CString(m_bAutoStartup),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_ALPHA_LOW_LIMIT,
                              Util_Int2CString(m_iAlphaLowLimit),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_ALPHA_GRANULARITY,
                              Util_Int2CString(m_iAlphaGranularity),
                              m_szProfilePath);
    TRACE("<<< %s\n", __FUNCTION__);
}


void ProfileHandler::GenerateDefaultProfile()
{
    TRACE(">>> %s\n", __FUNCTION__);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_HOTKEY_ENABLE,
                              Util_Int2CString(PROFILE_DEF_HOTKEY_ENABLE),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_HOTKEY_COMBINATION,
                              Util_Int2CString(PROFILE_DEF_HOTKEY_COMBINATION),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_MOUSEWHEEL_ENABLE,
                              Util_Int2CString(PROFILE_DEF_MOUSEWHEEL_ENABLE),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_MOUSEWHEEL_COMBINATION,
                              Util_Int2CString(PROFILE_DEF_MOUSEWHEEL_COMBINATION),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_START_MINIMIZED,
                              Util_Int2CString(PROFILE_DEF_START_MINIMIZED),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_AUTO_STARTUP,
                              Util_Int2CString(PROFILE_DEF_AUTO_STARTUP),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_ALPHA_LOW_LIMIT,
                              Util_Int2CString(PROFILE_DEF_ALPHA_LOW_LIMIT),
                              m_szProfilePath);
    WritePrivateProfileString(PROFILE_APPNAME,
                              PROFILE_KEY_ALPHA_GRANULARITY,
                              Util_Int2CString(PROFILE_DEF_ALPHA_GRANULARITY),
                              m_szProfilePath);
    TRACE("<<< %s\n", __FUNCTION__);
}


int ProfileHandler::GetHotKeyCombination()
{
    int iVal = (m_bHotKeyCtrl  ? KEY_COMBINATION_CTRL  : 0)
             | (m_bHotKeyAlt   ? KEY_COMBINATION_ALT   : 0)
             | (m_bHotKeyShift ? KEY_COMBINATION_SHIFT : 0)
             | (m_bHotKeyWin   ? KEY_COMBINATION_WIN   : 0);
    return iVal;
}


int ProfileHandler::GetMouseWheelCombination()
{
    int iVal = (m_bMouseWheelCtrl  ? KEY_COMBINATION_CTRL  : 0)
             | (m_bMouseWheelAlt   ? KEY_COMBINATION_ALT   : 0)
             | (m_bMouseWheelShift ? KEY_COMBINATION_SHIFT : 0)
             | (m_bMouseWheelWin   ? KEY_COMBINATION_WIN   : 0);
    return iVal;
}


CString ProfileHandler::Util_Int2CString(int iVal)
{
    CString szOut;
    szOut.Format(TEXT("%d"), iVal);
    return szOut;
}
