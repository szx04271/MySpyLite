#pragma once

#ifdef _M_IX86
#define FMT_PTR L"%X"
#define PLATFORM_STR L"32位"
#elif defined _M_X64
#define FMT_PTR L"%llX"
#define PLATFORM_STR L"64位"
#endif

#define VERSION_STR L"1.5"

inline constexpr const wchar_t* g_title = L"New Spylite [" PLATFORM_STR L"] v" VERSION_STR L" - by szx0427";