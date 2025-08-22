#pragma once

#define UPDATE_AND_CHECK_HWND() do{ UpdateData(); if (!::IsWindow(theApp.m_curWnd)) { MessageBoxW(L"窗口句柄无效。"); return; }} while(0)
#define CHECK_HWND() do{ if (!::IsWindow(theApp.m_curWnd)) { MessageBoxW(L"窗口句柄无效。"); return; }} while(0)

std::wstring NtFilePathToDosPath(std::wstring nt_path);

using StyleMap = std::list<std::pair<DWORD, LPCWSTR>>;

// style mapping
#define SM(style_macro) {style_macro, L ## #style_macro}

class CStyleListCtrl;
std::vector<StyleMap::const_iterator> GetOtherStyles(const StyleMap &all, const CStyleListCtrl &occupied_list);