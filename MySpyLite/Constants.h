#pragma once

#ifndef WND_FINDER_DLL

#ifdef _M_IX86
#define FMT_PTR L"%X"
#define PLATFORM_STR L"32位"
#define IDR_WNDPROC_FINDER IDR_WNDPROC_FINDER_32
#elif defined _M_X64
#define FMT_PTR L"%llX"
#define PLATFORM_STR L"64位"
#define IDR_WNDPROC_FINDER IDR_WNDPROC_FINDER_64
#endif

// 'f'=fork
#define VERSION_STR L"f1.07.00"

inline constexpr const wchar_t* g_title = L"New Spylite [" PLATFORM_STR L"] 版本 " VERSION_STR L" - by szx0427";
inline constexpr const wchar_t* g_regSubKey = L"Software\\szx0427\\NewSpyLite";
inline constexpr const wchar_t* g_regValueName = L"WndprocNeeded";

#endif // !defined WND_FINDER_DLL

enum MyMessages : UINT {
	WM_QUERYRESULT = WM_USER + 1
};

inline constexpr LPCWSTR g_pipeName = L"\\\\.\\pipe\\SZX_SPYLITE_WNDPROC_FINDER_20250824";