// ThreadMouseHook.cpp : implementation file
//

#include "stdafx.h"
#include "TransGlass.h"
#include "ThreadMouseHook.h"


#define WM_MOUSEWHEEL_HOOKMSG   (WM_APP + 101)


static bool SetHook(DWORD dwThreadID);
static bool UnsetHook();
static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

static DWORD      m_dwThreadID = NULL;
static HHOOK      g_hHook      = NULL;


// ThreadMouseHook

IMPLEMENT_DYNCREATE(ThreadMouseHook, CWinThread)

ThreadMouseHook::ThreadMouseHook()
    : m_hWnd(NULL)
    , m_fpSetHook(NULL)
    , m_fpUnsetHook(NULL)
{
}


ThreadMouseHook::~ThreadMouseHook()
{
}


BOOL ThreadMouseHook::EnableHook(HWND hWnd)
{
    m_hWnd = hWnd;
    m_fpSetHook = SetHook;
    m_fpUnsetHook = UnsetHook;

    if (! m_fpSetHook(this->m_nThreadID)) {
        return FALSE;
    } else {
        return TRUE;
    }
}


BOOL ThreadMouseHook::DisableHook()
{
    if (! m_fpUnsetHook()) {
        return FALSE;
    } else {
        return TRUE;
    }
}


BOOL ThreadMouseHook::InitInstance()
{
    return TRUE;
}


int ThreadMouseHook::ExitInstance()
{
    return CWinThread::ExitInstance();
}


BOOL ThreadMouseHook::PreTranslateMessage(MSG* pMsg)
{
    switch (pMsg->message) {   
    case WM_MOUSEWHEEL_HOOKMSG:
        PostMessage(m_hWnd, WM_MOUSEWHEEL, pMsg->wParam, pMsg->lParam);
        break;
    default:
        break;
    }
    return CWinThread::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(ThreadMouseHook, CWinThread)
END_MESSAGE_MAP()


// ThreadMouseHook message handlers




bool SetHook(DWORD dwThreadID)
{
    if (m_dwThreadID) {
        return true;
    }
    g_hHook = ::SetWindowsHookEx(WH_MOUSE_LL,
        (HOOKPROC) LowLevelMouseProc, 
        AfxGetInstanceHandle(),
        0);
    if (g_hHook) {
        m_dwThreadID = dwThreadID;
        return true;
    }
    return false;
}


bool UnsetHook()
{
    if (! m_dwThreadID) {
        return false;
    }
    if (g_hHook) {
        ::UnhookWindowsHookEx(g_hHook);
        m_dwThreadID = NULL;
        g_hHook = NULL;
        return true;
    }
    return false;
}


LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{ 
    if ((0 > nCode) && g_hHook) {
        ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
        return 0;
    }

    if (WM_MOUSEWHEEL == wParam) {
        if ((0x8000 & ::GetKeyState(VK_CONTROL)) &&
            (0x8000 & ::GetKeyState(VK_MENU)))
        {
            PMSLLHOOKSTRUCT pstLLHook = (PMSLLHOOKSTRUCT) lParam;
            ::PostThreadMessage(m_dwThreadID,
                                WM_MOUSEWHEEL_HOOKMSG,
                                pstLLHook->mouseData,
                                MAKELPARAM(pstLLHook->pt.x, pstLLHook->pt.y));
            return -1;
        }
    }

    return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
} 
