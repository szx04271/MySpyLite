
// MySpyLite.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号

// style mapping
#define SM(style_macro) {style_macro, L ## #style_macro}

// CMySpyLiteApp:
// 有关此类的实现，请参阅 MySpyLite.cpp
//

class CMySpyLiteApp : public CWinApp
{
public:
	CMySpyLiteApp();

// 重写
public:
	HWND m_curWnd{};

	std::map<DWORD, LPCWSTR> m_wndStyleMap;
	std::map<DWORD, LPCWSTR> m_wndExStyleMap;
	std::map<DWORD, LPCWSTR> m_clsStyleMap;

	virtual BOOL InitInstance();

	void InitWndStyleMap();
	void InitWndExStyleMap();
	void InitClsStyleMap();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMySpyLiteApp theApp;
