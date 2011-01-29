#pragma once



// ThreadMouseHook

class ThreadMouseHook : public CWinThread
{
    DECLARE_DYNCREATE(ThreadMouseHook)

public:
    ThreadMouseHook();           // protected constructor used by dynamic creation
    virtual ~ThreadMouseHook();

    BOOL    EnableHook(HWND hWnd);
    BOOL    DisableHook();

    virtual BOOL InitInstance();
    virtual int  ExitInstance();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    DECLARE_MESSAGE_MAP()

private:
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    HWND            m_hWnd;
    static DWORD    s_dwThreadID;
    static HHOOK    s_hHook;
};
