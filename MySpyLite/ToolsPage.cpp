// ToolsPage.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "ToolsPage.h"
#include "afxdialogex.h"
#include "SendMsgDlg.h"
#include "Constants.h"


// CToolsPage 对话框

IMPLEMENT_DYNAMIC(CToolsPage, CTabPage)

CToolsPage::CToolsPage(CWnd* pParent /*=nullptr*/)
	: CTabPage(IDD_PAGE_TOOLS, pParent)
	, m_chkVisible(FALSE)
	, m_chkEnabled(FALSE)
	, m_chkReadonly(FALSE)
	, m_chkTopmost(FALSE)
	, m_chkMaximized(FALSE)
	, m_chkMinimized(FALSE)
	, m_xpos(0)
	, m_ypos(0)
	, m_cxpos(0)
	, m_cypos(0)
	, m_title(_T(""))
	, m_hCurWnd(NULL)
{

}

CToolsPage::~CToolsPage()
{
}

void CToolsPage::DoDataExchange(CDataExchange* pDX)
{
	CTabPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_VISIBLE, m_chkVisible);
	DDX_Check(pDX, IDC_ENABLED, m_chkEnabled);
	DDX_Check(pDX, IDC_READONLY, m_chkReadonly);
	DDX_Check(pDX, IDC_TOPMOST, m_chkTopmost);
	DDX_Check(pDX, IDC_MAXIMIZED, m_chkMaximized);
	DDX_Check(pDX, IDC_MINIMIZED, m_chkMinimized);
	DDX_Text(pDX, IDC_X_POS, m_xpos);
	DDX_Text(pDX, IDC_Y_POS, m_ypos);
	DDX_Text(pDX, IDC_CX_POS, m_cxpos);
	DDX_Text(pDX, IDC_CY_POS, m_cypos);
	DDX_Text(pDX, IDC_NEW_TITLE, m_title);
}


BEGIN_MESSAGE_MAP(CToolsPage, CTabPage)
	ON_BN_CLICKED(IDC_VISIBLE, &CToolsPage::OnBnClickedVisible)
	ON_BN_CLICKED(IDC_ENABLED, &CToolsPage::OnBnClickedEnabled)
	ON_BN_CLICKED(IDC_READONLY, &CToolsPage::OnBnClickedReadonly)
	ON_BN_CLICKED(IDC_TOPMOST, &CToolsPage::OnBnClickedTopmost)
	ON_BN_CLICKED(IDC_MAXIMIZED, &CToolsPage::OnBnClickedMaximized)
	ON_BN_CLICKED(IDC_MINIMIZED, &CToolsPage::OnBnClickedMinimized)
	ON_BN_CLICKED(IDC_KILL_NORMAL, &CToolsPage::OnBnClickedKillNormal)
	ON_BN_CLICKED(IDC_KILL_SETPARENT, &CToolsPage::OnBnClickedKillSetparent)
	ON_BN_CLICKED(IDC_KILL_SENDMSG, &CToolsPage::OnBnClickedKillSendmsg)
	ON_BN_CLICKED(IDC_WAIT_AND_QUERY, &CToolsPage::OnBnClickedWaitAndQuery)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_GET_FONT_INFO, &CToolsPage::OnBnClickedGetFontInfo)
	ON_BN_CLICKED(IDC_APPLY_POS, &CToolsPage::OnBnClickedApplyPos)
	ON_BN_CLICKED(IDC_APPLY_TITLE, &CToolsPage::OnBnClickedApplyTitle)
	ON_BN_CLICKED(IDC_SEND_MESSAGE, &CToolsPage::OnBnClickedSendMessage)
END_MESSAGE_MAP()


// CToolsPage 消息处理程序


#define CHECK_HWND() do{ UpdateData(); if (!::IsWindow(m_hCurWnd)) { MessageBoxW(L"窗口句柄无效。"); return; }} while(0)

void CToolsPage::OnBnClickedVisible()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	::ShowWindow(m_hCurWnd, m_chkVisible ? SW_SHOW : SW_HIDE);
}


void CToolsPage::OnBnClickedEnabled()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	::EnableWindow(m_hCurWnd, m_chkEnabled);
}


void CToolsPage::OnBnClickedReadonly()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	::SendMessageW(m_hCurWnd, EM_SETREADONLY, (WPARAM)m_chkReadonly, 0);
}


void CToolsPage::OnBnClickedTopmost()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	::SetWindowPos(m_hCurWnd, m_chkTopmost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void CToolsPage::OnBnClickedMaximized()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	::ShowWindow(m_hCurWnd, m_chkMaximized ? SW_SHOWMAXIMIZED : SW_RESTORE);
}


void CToolsPage::OnBnClickedMinimized()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	::ShowWindow(m_hCurWnd, m_chkMinimized ? SW_SHOWMINIMIZED : SW_RESTORE);
}


void CToolsPage::OnBnClickedKillNormal()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	::PostMessageW(m_hCurWnd, WM_CLOSE, 0, 0);
}


void CToolsPage::OnBnClickedKillSetparent()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	CStatic stc;
	stc.Create(L"", 0, { 0,0,0,0 }, this, IDC_STATIC);
	::SetParent(m_hCurWnd, stc.GetSafeHwnd());
	stc.SendMessage(WM_CLOSE);
}


void CToolsPage::OnBnClickedKillSendmsg()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	int ret = MessageBoxW(L"在硬件配置较低的计算机上执行此操作可能导致系统运行速度降低，或使本程序崩溃。\n您确定要执行吗？", 
		L"警告", MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2);
	if (ret != IDYES)
		return;

	GetDlgItem(IDC_KILL_SENDMSG)->SetWindowTextW(L"执行中 请稍候");
	GetDlgItem(IDC_KILL_SENDMSG)->EnableWindow(FALSE);
	for (UINT msg = 0; msg < 0xffff; msg++)
		::PostMessageW(m_hCurWnd, msg, 0, 0);
	GetDlgItem(IDC_KILL_SENDMSG)->SetWindowTextW(L"强力关闭 (方式2) (不推荐)");
	GetDlgItem(IDC_KILL_SENDMSG)->EnableWindow(TRUE);
}


void CToolsPage::OnBnClickedWaitAndQuery()
{
	// : 在此添加控件通知处理程序代码
	SetTimer(1, 1000, NULL);
}


void CToolsPage::OnTimer(UINT_PTR nIDEvent)
{
	// : 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		static int n = 0;
		static CString str;

		POINT pt;
		GetCursorPos(&pt);
		HWND hwnd = ::WindowFromPoint(pt);
		if (n == 10)
		{
			n = 0;
			KillTimer(1);
			str.Format(FMT_PTR, hwnd);
			GetParent()->GetParent()->SendMessageW(WM_QUERYRESULT, 0, (LPARAM)(LPCWSTR)str);
			GetParent()->GetParent()->SetWindowTextW(g_title);
			MessageBeep(MB_OK);
			CTabPage::OnTimer(nIDEvent);
			return;
		}
		
		str.Format(L"当前窗口：0x%X，剩余时间：%d秒", hwnd, 10 - (n++));
		GetParent()->GetParent()->SetWindowTextW(str);
	}

	CTabPage::OnTimer(nIDEvent);
}


void CToolsPage::OnBnClickedGetFontInfo()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	LOGFONTW lf;
	
	CFont *pFont = CWnd::FromHandle(m_hCurWnd)->GetFont();
	if (pFont != nullptr)
		pFont->GetLogFont(&lf);
	else
	{
		lf.lfHeight = 16;
		lf.lfWidth = 7;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfWeight = 700;
		lf.lfItalic = 0;
		lf.lfUnderline = 0;
		lf.lfStrikeOut = 0;
		lf.lfCharSet = 134;
		lf.lfOutPrecision = 1;
		lf.lfClipPrecision = 2;
		lf.lfQuality = 2;
		lf.lfPitchAndFamily = 34;
		wcscpy(lf.lfFaceName, L"System");
	}

	CString str;
	str.Format(
		L"LOGFONT 结构体：\n"
		L"lfHeight = %d,\n"
		L"lfWidth = %d, \n"
		L"lfEscapement = %d,\n"
		L"lfOrientation = %d,\n"
		L"lfWeight = %d,\n"
		L"lfItalic = %u,\n"
		L"lfUnderline = %u,\n"
		L"lfStrikeOut = %u,\n"
		L"lfCharset = %u,\n"
		L"lfOutPrecision = %u,\n"
		L"lfClipPrecision = %u,\n"
		L"lfQuality = %u,\n"
		L"lfPitchAndFamily = %u,\n"
		L"lfFaceName = %s.",
		lf.lfHeight,
		lf.lfWidth,
		lf.lfEscapement,
		lf.lfOrientation,
		lf.lfWeight,
		lf.lfItalic,
		lf.lfUnderline,
		lf.lfStrikeOut,
		lf.lfCharSet,
		lf.lfOutPrecision,
		lf.lfClipPrecision,
		lf.lfQuality,
		lf.lfPitchAndFamily,
		lf.lfFaceName
	);
	MessageBoxW(str, L"读取结果", MB_ICONINFORMATION);
}


void CToolsPage::OnBnClickedApplyPos()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	int x = 0, y = 0;
	auto hParent = ::GetParent(m_hCurWnd);
	if (hParent)
	{
		RECT rc;
		::GetClientRect(hParent, &rc);
		POINT pt = { rc.left,rc.top };
		::ClientToScreen(hParent, &pt);
		x = pt.x;
		y = pt.y;
	}
	::SetWindowPos(m_hCurWnd, NULL, m_xpos - x, m_ypos - y, m_cxpos, m_cypos, SWP_NOZORDER);
}


void CToolsPage::OnBnClickedApplyTitle()
{
	// : 在此添加控件通知处理程序代码
	CHECK_HWND();
	::SetWindowTextW(m_hCurWnd, m_title);
}


void CToolsPage::OnBnClickedSendMessage()
{
	// : 在此添加控件通知处理程序代码
	CSendMsgDlg dlg(m_hCurWnd);
	dlg.DoModal();
}
