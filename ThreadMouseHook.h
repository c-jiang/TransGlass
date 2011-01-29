#pragma once



// ThreadMouseHook

typedef bool (*SetHook_fp)(DWORD ulThreadID);
typedef bool (*UnsetHook_fp)();


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
    SetHook_fp      m_fpSetHook;
    UnsetHook_fp    m_fpUnsetHook;
    HWND            m_hWnd;
};
