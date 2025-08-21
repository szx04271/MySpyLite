#pragma once

#define CHECK_HWND() do{ UpdateData(); if (!::IsWindow(theApp.m_curWnd)) { MessageBoxW(L"窗口句柄无效。"); return; }} while(0)

std::wstring NtFilePathToDosPath(std::wstring nt_path);