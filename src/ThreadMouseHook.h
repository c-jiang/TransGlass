#pragma once



// ThreadMouseHook

class ThreadMouseHook : public CWinThread
{
    DECLARE_DYNCREATE(ThreadMouseHook)

public:
    ThreadMouseHook();           // protected constructor used by dynamic creation
    virtual ~ThreadMouseHook();

    BOOL EnableHook(HWND hWnd);
    BOOL DisableHook();

    static void SetKeyCombination(BOOL bCtrl, BOOL bAlt, BOOL bShift, BOOL bWin);

    virtual BOOL InitInstance();
    virtual int  ExitInstance();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    DECLARE_MESSAGE_MAP()

private:
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static inline BOOL CheckKeyCombination();

    static DWORD    s_dwThreadID;
    static HHOOK    s_hHook;
    static BOOL     s_bKeyCtrl;
    static BOOL     s_bKeyAlt;
    static BOOL     s_bKeyShift;
    static BOOL     s_bKeyWin;

    HWND            m_hWnd;
};
