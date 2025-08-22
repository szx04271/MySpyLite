#pragma once

#ifdef _M_IX86
#define FMT_PTR L"%X"
#define PLATFORM_STR L"32位"
#elif defined _M_X64
#define FMT_PTR L"%llX"
#define PLATFORM_STR L"64位"
#endif

// 'f'=fork
#define VERSION_STR L"f1.06"

inline constexpr const wchar_t* g_title = L"New Spylite [" PLATFORM_STR L"] 版本 " VERSION_STR L" - by szx0427";