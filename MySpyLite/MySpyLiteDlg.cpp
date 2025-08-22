
// MySpyLiteDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MySpyLite.h"
#include "MySpyLiteDlg.h"
#include "afxdialogex.h"
#include "Constants.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMySpyLiteDlg 对话框



CMySpyLiteDlg::CMySpyLiteDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MYSPYLITE_DIALOG, pParent)
	, m_tabCursel(0)
	, m_sWnd(_T(""))
	, m_topmost(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_font.CreatePointFont(270, L"Tahoma");
}

void CMySpyLiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WND_FINDER, m_wndFinder);
	DDX_Text(pDX, IDC_HWND, m_sWnd);
	DDX_Check(pDX, IDC_TOPMOST, m_topmost);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Control(pDX, IDC_HWND, m_hexEdit);
}

BEGIN_MESSAGE_MAP(CMySpyLiteDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TOPMOST, &CMySpyLiteDlg::OnBnClickedTopmost)
	ON_BN_CLICKED(IDC_QUERY, &CMySpyLiteDlg::OnBnClickedQuery)
	ON_MESSAGE(WM_QUERYRESULT, &CMySpyLiteDlg::OnQueryResult)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CMySpyLiteDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CMySpyLiteDlg 消息处理程序

BOOL CMySpyLiteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetWindowTextW(g_title);

	m_wndFinder.SetFont(&m_font);

	m_tab.ModifyStyle(NULL, WS_CLIPCHILDREN);
	m_tab.InsertItem(0, L"常规");
	m_tab.InsertItem(1, L"样式");
	m_tab.InsertItem(2, L"类");
	m_tab.InsertItem(3, L"窗口");
	m_tab.InsertItem(4, L"图像");
	m_tab.InsertItem(5, L"工具");
	m_tab.InsertItem(6, L"关于");

	// calc sub dialog rect using tab rect
	CRect rc;
	m_tab.GetWindowRect(&rc);
	m_tab.ScreenToClient(&rc);
	m_tab.AdjustRect(FALSE, &rc);
	
	auto dc = GetDC();
	auto dpiFactor = dc->GetDeviceCaps(LOGPIXELSX) / 96.f;
	rc.top += 2 * dpiFactor;
	ReleaseDC(dc);

	m_page1.Create(IDD_PAGE_GENERAL, &m_tab);
	m_page1.MoveWindow(&rc);
	m_page1.ShowWindow(SW_SHOW);

	m_page2.Create(IDD_PAGE_STYLES, &m_tab);
	m_page2.MoveWindow(&rc);
	m_page2.ShowWindow(SW_HIDE);

	m_page3.Create(IDD_PAGE_CLASS, &m_tab);
	m_page3.MoveWindow(&rc);
	m_page3.ShowWindow(SW_HIDE);

	m_page4.Create(IDD_PAGE_WINDOWS, &m_tab);
	m_page4.MoveWindow(&rc);
	m_page4.ShowWindow(SW_HIDE);

	m_page5.Create(IDD_PAGE_IMAGE, &m_tab);
	m_page5.MoveWindow(&rc);
	m_page5.ShowWindow(SW_HIDE);

	m_page6.Create(IDD_PAGE_TOOLS, &m_tab);
	m_page6.MoveWindow(&rc);
	m_page6.ShowWindow(SW_HIDE);

	m_page7.Create(IDD_PAGE_ABOUT, &m_tab);
	m_page7.MoveWindow(&rc);
	m_page7.ShowWindow(SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMySpyLiteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMySpyLiteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMySpyLiteDlg::OnBnClickedTopmost()
{
	// : 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_topmost)
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void CMySpyLiteDlg::OnBnClickedQuery()
{
	// : 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_sWnd.MakeUpper();
	HWND hWnd;
	swscanf(m_sWnd, FMT_PTR, &hWnd);
	if (!::IsWindow(hWnd))
	{
		MessageBoxW(L"窗口句柄无效。");
		return;
	}
	theApp.m_curWnd = hWnd;

	UpdateGeneralData(hWnd);
	UpdateStylesData(hWnd);
	UpdateClassData(hWnd);
	UpdateWindowsData(hWnd);
	UpdateImageData(hWnd);
	UpdateToolsData(hWnd);
}

LRESULT CMySpyLiteDlg::OnQueryResult(WPARAM, LPARAM lParam)
{
	if (lParam)
	{
		m_sWnd = (LPCWSTR)lParam;
		UpdateData(FALSE);
	}
	OnBnClickedQuery();
	return 0;
}


void CMySpyLiteDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_tabCursel)
	{
	case 0:
		m_page1.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_page2.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_page3.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_page4.ShowWindow(SW_HIDE);
		break;
	case 4:
		m_page5.ShowWindow(SW_HIDE);
		break;
	case 5:
		m_page6.ShowWindow(SW_HIDE);
		break;
	case 6:
		m_page7.ShowWindow(SW_HIDE);
		break;
	}
	m_tabCursel = m_tab.GetCurSel();
	switch (m_tabCursel)
	{
	case 0:
		m_page1.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_page2.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_page3.ShowWindow(SW_SHOW);
		break;
	case 3:
		m_page4.ShowWindow(SW_SHOW);
		break;
	case 4:
		m_page5.ShowWindow(SW_SHOW);
		break;
	case 5:
		m_page6.ShowWindow(SW_SHOW);
		break;
	case 6:
		m_page7.ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}

void CMySpyLiteDlg::UpdateGeneralData(HWND hWnd)
{
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	WCHAR szBuf[1024];
	CRect wndRc, clientRc;
	DWORD dw;
	
	pWnd->GetWindowTextW(m_page1.m_title);
	::GetClassNameW(hWnd, szBuf, 1024);
	m_page1.m_clsName = szBuf;
	pWnd->GetWindowRect(&wndRc);
	m_page1.m_wndRc.Format(
		L"LT(%d, %d) RB(%d, %d) [%d x %d]",
		wndRc.left, wndRc.top,
		wndRc.right, wndRc.bottom,
		wndRc.Width(), wndRc.Height()
	);
	pWnd->GetClientRect(&clientRc);
	m_page1.m_cltRc.Format(
		L"LT(%d, %d) RB(%d, %d) [%d x %d]",
		clientRc.left, clientRc.top,
		clientRc.right, clientRc.bottom,
		clientRc.Width(), clientRc.Height()
	);

	// get client relative coord
	pWnd->ClientToScreen(&clientRc);
	CRect relativeRc{ clientRc.left - wndRc.left,clientRc.top - wndRc.top,
		clientRc.right - wndRc.left, clientRc.bottom - wndRc.top };
	m_page1.m_relativeRc.Format(
		L"LT(%d, %d) RB(%d, %d) [%d x %d]",
		relativeRc.left, relativeRc.top,
		relativeRc.right, relativeRc.bottom,
		relativeRc.Width(), relativeRc.Height()
	);

	m_page1.m_ctrlId = pWnd->GetDlgCtrlID();
	m_page1.m_tid = GetWindowThreadProcessId(hWnd, &m_page1.m_pid);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_page1.m_pid);
	if (dw = GetLastError())
		m_page1.m_path.Format(L"OpenProcess失败。错误代码：%u。", dw);
	else
	{
		// 32/64 bit GENERAL
		WCHAR dosPath[MAX_PATH + 1];
		GetProcessImageFileNameW(hProcess, dosPath, MAX_PATH + 1);
		if (dw = GetLastError())
		{
			m_page1.m_path.Format(L"GetProcessImageFileName失败。错误代码：%u。", dw);
		}
		else
		{
			std::wstring filePath = NtFilePathToDosPath(dosPath);
			m_page1.m_path = filePath.c_str();
		}

	}

	m_page1.UpdateData(FALSE);
}

void CMySpyLiteDlg::UpdateStylesData(HWND hWnd)
{
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	DWORD dwStyle = pWnd->GetStyle();
	DWORD dwExStyle = pWnd->GetExStyle();

	m_page2.SetStyle(dwStyle);
	m_page2.SetExStyle(dwExStyle);
}

void CMySpyLiteDlg::UpdateClassData(HWND hWnd)
{
	WCHAR cls_name[1024];
	GetClassNameW(hWnd, cls_name, 1024);
	m_page3.m_clsname = cls_name;
	
	// 跨进程调用GetClassInfoEx会失败，只能用GetClassLongPtr了
	HMODULE cls_hmod = reinterpret_cast<HMODULE>(GetClassLongPtrW(hWnd, GCLP_HMODULE));
	auto cls_atom = static_cast<UINT>(GetClassLongPtrW(hWnd, GCW_ATOM));
	auto cls_style = static_cast<DWORD>(GetClassLongPtrW(hWnd, GCL_STYLE));

	m_page3.m_clsHmodule.Format(L"0x" FMT_PTR, cls_hmod);
	m_page3.m_atom.Format(L"%u", cls_atom);
	m_page3.m_hbrBkgnd.Format(L"0x" FMT_PTR, GetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND));
	m_page3.m_clsHicon.Format(L"0x" FMT_PTR, GetClassLongPtrW(hWnd, GCLP_HICON));
	m_page3.m_clsHiconSm.Format(L"0x" FMT_PTR, GetClassLongPtrW(hWnd, GCLP_HICONSM));
	m_page3.m_clsWndproc.Format(L"0x%p", GetClassLongPtrW(hWnd, GCLP_WNDPROC));

	m_page3.m_clsStyleStr.Format(L"0x%08XL", cls_style);
	m_page3.SetClsStyle(cls_style);
}

void CMySpyLiteDlg::UpdateWindowsData(HWND hWnd)
{
	m_page4.m_hCurrentWnd = hWnd;
	m_page4.m_parentWnd.Format(FMT_PTR, ::GetParent(hWnd));
	m_page4.m_ownerWnd.Format(FMT_PTR, ::GetWindow(hWnd, GW_OWNER));
	m_page4.m_prevWnd.Format(FMT_PTR, ::GetWindow(hWnd, GW_HWNDPREV));
	m_page4.m_nextWnd.Format(FMT_PTR, ::GetWindow(hWnd, GW_HWNDNEXT));
	m_page4.m_list_children.DeleteAllItems();
	m_page4.UpdateData(FALSE);
}

void CMySpyLiteDlg::UpdateImageData(HWND hWnd) {
	CImage& img = m_page5.m_img;

	if (!img.IsNull())
		img.Destroy();

	CRect rc;
	::GetWindowRect(hWnd, &rc);
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	if (rc.left < 0)
		rc.left = 0;
	if (rc.right > cxScreen - 1)
		rc.right = cxScreen - 1;
	if (rc.top < 0)
		rc.top = 0;
	if (rc.bottom > cyScreen - 1)
		rc.bottom = cyScreen - 1;

	if (
		rc.left > cxScreen - 1 ||
		rc.right < 0 ||
		rc.top > cyScreen - 1 ||
		rc.bottom < 0 ||
		rc.Width() <= 0 ||
		rc.Height() <= 0
		)
	{
		m_page5.m_hscroll.SetScrollRange(0, 0);
		m_page5.m_hscroll.EnableWindow(FALSE);
		m_page5.m_vscroll.SetScrollRange(0, 0);
		m_page5.m_vscroll.EnableWindow(FALSE);

		img.Create(300, 30, 16);
		WCHAR szString[] = L"窗口不可见或已最小化。";
		HDC h = img.GetDC();
		auto old_brush = ::SelectObject(h, GetStockObject(WHITE_BRUSH));
		Rectangle(h, 0, 0, 301, 31);
		TextOutW(h, 0, 0, szString, wcslen(szString));
		SelectObject(h, old_brush);
		img.ReleaseDC();
	}

	if (rc.Width() > m_page5.m_imgCx)
	{
		m_page5.m_hscroll.EnableWindow(TRUE);
		m_page5.m_hscroll.SetScrollRange(0, rc.Width() - m_page5.m_imgCx);
	}
	else
	{
		m_page5.m_hscroll.SetScrollRange(0, 0);
		m_page5.m_hscroll.EnableWindow(FALSE);
	}
	if (rc.Height() > m_page5.m_imgCy)
	{
		m_page5.m_vscroll.EnableWindow(TRUE);
		m_page5.m_vscroll.SetScrollRange(0, rc.Height() - m_page5.m_imgCy);
	}
	else
	{
		m_page5.m_vscroll.SetScrollRange(0, 0);
		m_page5.m_vscroll.EnableWindow(FALSE);
	}

	img.Create(rc.Width(), rc.Height(), 24);
	HDC hImgDc = img.GetDC();
	HDC hDeskDc = ::GetDC(NULL);
	BitBlt(hImgDc, 0, 0, rc.Width(), rc.Height(), hDeskDc, rc.left, rc.top, SRCCOPY);

	img.ReleaseDC();
	::ReleaseDC(NULL, hDeskDc);
	m_page5.Invalidate(TRUE);
	m_page5.UpdateWindow();
}

void CMySpyLiteDlg::UpdateToolsData(HWND hWnd)
{
	CWnd* pWnd = CWnd::FromHandle(hWnd);

	m_page6.m_chkVisible = pWnd->IsWindowVisible();
	m_page6.m_chkEnabled = pWnd->IsWindowEnabled();
	m_page6.m_chkReadonly = pWnd->GetStyle()&ES_READONLY ? TRUE : FALSE;
	m_page6.m_chkTopmost = pWnd->GetExStyle()&WS_EX_TOPMOST ? TRUE : FALSE;
	m_page6.m_chkMaximized = pWnd->IsZoomed();
	m_page6.m_chkMinimized = pWnd->IsIconic();

	CRect rc;
	pWnd->GetWindowRect(&rc);
	m_page6.m_xpos = rc.left;
	m_page6.m_ypos = rc.top;
	m_page6.m_cxpos = rc.Width();
	m_page6.m_cypos = rc.Height();

	pWnd->GetWindowTextW(m_page6.m_title);

	m_page6.UpdateData(FALSE);
}


void CMySpyLiteDlg::OnOK()
{
	// 此处留空，不调用基类，以防止按enter时自动关闭程序
}


BOOL CMySpyLiteDlg::PreTranslateMessage(MSG* pMsg)
{
	// : 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		int ret = MessageBoxW(L"您按下了 Esc 键，是否要退出程序？", NULL, MB_ICONQUESTION | MB_YESNO);
		if (ret != IDYES)
			return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && GetFocus() == GetDlgItem(IDC_HWND))
	{
		OnBnClickedQuery();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

