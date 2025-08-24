
// MySpyLite.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号
#include "Utils.h"

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

	StyleMap m_wndStyleMap;
	StyleMap m_wndExStyleMap;
	StyleMap m_clsStyleMap;

	std::wstring m_wndProcFinderDllPath;

	virtual BOOL InitInstance();

	void InitWndStyleMap();
	void InitWndExStyleMap();
	void InitClsStyleMap();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMySpyLiteApp theApp;
