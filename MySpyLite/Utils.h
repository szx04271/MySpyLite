#pragma once

#define UPDATE_AND_CHECK_HWND() do{ UpdateData(); if (!::IsWindow(theApp.m_curWnd)) { MessageBoxW(L"窗口句柄无效。"); return; }} while(0)
#define CHECK_HWND() do{ if (!::IsWindow(theApp.m_curWnd)) { MessageBoxW(L"窗口句柄无效。"); return; }} while(0)

std::wstring NtFilePathToDosPath(std::wstring nt_path);

using StyleMap = std::list<std::pair<DWORD, LPCWSTR>>;

// style mapping
#define SM(style_macro) {style_macro, L ## #style_macro}

class CStyleListCtrl;
std::vector<StyleMap::const_iterator> GetOtherStyles(const StyleMap &all, const CStyleListCtrl &occupied_list);

std::wstring GetTempFilePath();

bool ReleaseWndProcFinderDll();

bool InjectDll(DWORD pid, const std::wstring & dll_path);

// -1 for unknown, 0 for false, 1 for true
int8_t Is64BitProcess(HANDLE hProcess);