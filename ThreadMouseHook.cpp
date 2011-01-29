// ThreadMouseHook.cpp : implementation file
//

#include "stdafx.h"
#include "TransGlass.h"
#include "ThreadMouseHook.h"


#define WM_MOUSEWHEEL_HOOKMSG   (WM_APP + 101)


// ThreadMouseHook

IMPLEMENT_DYNCREATE(ThreadMouseHook, CWinThread)


ThreadMouseHook::ThreadMouseHook()
    : m_hWnd(NULL)
{
}


ThreadMouseHook::~ThreadMouseHook()
{
}


DWORD ThreadMouseHook::s_dwThreadID(NULL);
HHOOK ThreadMouseHook::s_hHook(NULL);
BOOL  ThreadMouseHook::s_bKeyCtrl(FALSE);
BOOL  ThreadMouseHook::s_bKeyAlt(FALSE);
BOOL  ThreadMouseHook::s_bKeyShift(FALSE);
BOOL  ThreadMouseHook::s_bKeyWin(FALSE);


BOOL ThreadMouseHook::EnableHook(HWND hWnd)
{
    BOOL bRet = TRUE;

    m_hWnd = hWnd;
    if (! s_dwThreadID) {
        s_hHook = ::SetWindowsHookEx(WH_MOUSE_LL,
                                     (HOOKPROC) LowLevelMouseProc, 
                                     AfxGetInstanceHandle(),
                                     0);
        if (s_hHook) {
            s_dwThreadID = this->m_nThreadID;
        } else {
            bRet = FALSE;
        }
    }
    return bRet;
}


BOOL ThreadMouseHook::DisableHook()
{
    BOOL bRet = FALSE;

    if (s_dwThreadID && s_hHook) {
        ::UnhookWindowsHookEx(s_hHook);
        s_dwThreadID = NULL;
        s_hHook      = NULL;
        bRet = TRUE;
    }
    return bRet;
}


void ThreadMouseHook::SetKeyCombination(BOOL bCtrl,
                                        BOOL bAlt,
                                        BOOL bShift,
                                        BOOL bWin)
{
    s_bKeyCtrl  = bCtrl;
    s_bKeyAlt   = bAlt;
    s_bKeyShift = bShift;
    s_bKeyWin   = bWin;
}


BOOL ThreadMouseHook::CheckKeyCombination()
{
    BOOL bRet = (s_bKeyCtrl  ? (0x8000 & GetKeyState(VK_CONTROL)) : TRUE)
             && (s_bKeyAlt   ? (0x8000 & GetKeyState(VK_MENU))    : TRUE)
             && (s_bKeyShift ? (0x8000 & GetKeyState(VK_SHIFT))   : TRUE)
             && (s_bKeyWin
                 ? (0x8000 & (GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)))
                 : TRUE);
    return bRet;
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


LRESULT CALLBACK ThreadMouseHook::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{ 
    if ((0 > nCode) && s_hHook) {
        ::CallNextHookEx(s_hHook, nCode, wParam, lParam);
        return 0;
    }

    if (WM_MOUSEWHEEL == wParam) {
        if (CheckKeyCombination()) {
            PMSLLHOOKSTRUCT pstLLHook = (PMSLLHOOKSTRUCT) lParam;
            ::PostThreadMessage(s_dwThreadID,
                                WM_MOUSEWHEEL_HOOKMSG,
                                pstLLHook->mouseData,
                                MAKELPARAM(pstLLHook->pt.x, pstLLHook->pt.y));
            return -1;
        }
    }

    return ::CallNextHookEx(s_hHook, nCode, wParam, lParam);
} 


BEGIN_MESSAGE_MAP(ThreadMouseHook, CWinThread)
END_MESSAGE_MAP()
