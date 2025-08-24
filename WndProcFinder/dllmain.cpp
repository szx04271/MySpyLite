// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#define WND_FINDER_DLL
#include "../MySpyLite/Constants.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        WaitNamedPipeW(g_pipeName, INFINITE);
        HANDLE hPipe = CreateFileW(g_pipeName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hPipe != INVALID_HANDLE_VALUE) {
            LONG_PTR wndproc{};
            HWND hwnd{};
            DWORD junk;
            ReadFile(hPipe, &hwnd, sizeof(HWND), &junk, nullptr);
            wndproc = GetWindowLongPtrW(hwnd, GWLP_WNDPROC);
            // whether we succeed here, we should write sizeof(LONG_PTR) bytes to pipe,
            // because spylite main window is waiting for this dll's response.
            WriteFile(hPipe, &wndproc, sizeof(wndproc), &junk, nullptr);
            CloseHandle(hPipe);
        }
        
        return FALSE; // 返回FALSE，以便卸载该DLL
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

