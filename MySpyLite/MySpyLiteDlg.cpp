
// MySpyLiteDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MySpyLite.h"
#include "MySpyLiteDlg.h"
#include "afxdialogex.h"
#include "Constants.h"

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

	CRect rc;
	m_tab.GetClientRect(&rc);
	CDC *pDC = GetDC();
	float factorX = pDC->GetDeviceCaps(LOGPIXELSX) / 96.0;
	float factorY = pDC->GetDeviceCaps(LOGPIXELSY) / 96.0;
	ReleaseDC(pDC);
	rc.top += 24 * factorY;
	rc.bottom -= 5 * factorY;
	rc.left += 5 * factorX;
	rc.right -= 5 * factorX;

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
	// : 在此添加控件通知处理程序代码
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
	CRect rc;
	DWORD dw;
	
	pWnd->GetWindowTextW(m_page1.m_title);
	::GetClassNameW(hWnd, szBuf, 1024);
	m_page1.m_clsName = szBuf;
	pWnd->GetWindowRect(&rc);
	m_page1.m_wndRc.Format(
		L"LT(%d, %d) RB(%d, %d) [%d x %d]",
		rc.left, rc.top,
		rc.right, rc.bottom,
		rc.Width(), rc.Height()
	);
	pWnd->GetClientRect(&rc);
	m_page1.m_cltRc.Format(
		L"LT(%d, %d) RB(%d, %d) [%d x %d]",
		rc.left, rc.top,
		rc.right, rc.bottom,
		rc.Width(), rc.Height()
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
			std::wstring filePath = NTFilePath2DosFilePath(dosPath);
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
	m_page2.m_stc_styles.Format(L"基本样式：0x%08XL", dwStyle);
	m_page2.m_stc_exstyles.Format(L"扩展样式：0x%08XL", dwExStyle);
	m_page2.m_list_styles.DeleteAllItems();
	m_page2.m_list_exstyles.DeleteAllItems();

	int i = 0;
	if (dwStyle & WS_BORDER)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_BORDER");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00800000L");
	}
	if (dwStyle & WS_CAPTION)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_CAPTION");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00C00000L");
	}
	if (dwStyle & WS_CHILD)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_CHILD (WS_CHILDWINDOW)");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x40000000L");
	}
	if (dwStyle & WS_CLIPCHILDREN)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_CLIPCHILDREN");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x02000000L");
	}
	if (dwStyle & WS_CLIPSIBLINGS)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_CLIPSIBLINGS");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x04000000L");
	}
	if (dwStyle & WS_DISABLED)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_DISABLED");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x08000000L");
	}
	if (dwStyle & WS_DLGFRAME)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_DLGFRAME");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00400000L");
	}
	if (dwStyle & WS_GROUP)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_GROUP");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00020000L");
	}
	if (dwStyle & WS_HSCROLL)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_HSCROLL");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00100000L");
	}

	if (dwStyle & WS_MAXIMIZE)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_MAXIMIZE");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x01000000L");
	}
	if (dwStyle & WS_MAXIMIZEBOX)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_MAXIMIZEBOX");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00010000L");
	}
	if (dwStyle & WS_MINIMIZE)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_MINIMIZE (WS_ICONIC)");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x20000000L");
	}
	if (dwStyle & WS_MINIMIZEBOX)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_MINIMIZEBOX");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00020000L");
	}
	//if (dwStyle & WS_OVERLAPPED)
	//{
	//	m_page2.m_list_styles.InsertItem(i, L"WS_OVERLAPPED (WS_TILED)");
	//	m_page2.m_list_styles.SetItemText(i++, 1, L"0x00000000L");
	//}
	//if (dwStyle & WS_OVERLAPPEDWINDOW)
	//{
	//	m_page2.m_list_styles.InsertItem(i, L"WS_OVERLAPPEDWINDOW (WS_TILEDWINDOW)");
	//	m_page2.m_list_styles.SetItemText(i++, 1, L"0x00CF0000L");
	//}
	if (dwStyle & WS_POPUP)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_POPUP");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x80000000L");
	}
	//if (dwStyle & WS_POPUPWINDOW)
	//{
	//	m_page2.m_list_styles.InsertItem(i, L"WS_POPUPWINDOW");
	//	m_page2.m_list_styles.SetItemText(i++, 1, L"0x80880000L");
	//}
	if (dwStyle & WS_SIZEBOX)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_SIZEBOX (WS_THICKFRAME)");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00040000L");
	}
	if (dwStyle & WS_SYSMENU)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_SYSMENU");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00080000L");
	}
	if (dwStyle & WS_TABSTOP)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_TABSTOP");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00010000L");
	}
	if (dwStyle & WS_VISIBLE)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_VISIBLE");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x10000000L");
	}
	if (dwStyle & WS_VSCROLL)
	{
		m_page2.m_list_styles.InsertItem(i, L"WS_VSCROLL");
		m_page2.m_list_styles.SetItemText(i++, 1, L"0x00200000L");
	}

	i = 0;
	if (dwExStyle & WS_EX_ACCEPTFILES)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_ACCEPTFILES");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000010L");
	}
	if (dwExStyle & WS_EX_APPWINDOW)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_APPWINDOW");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00040000L");
	}
	if (dwExStyle & WS_EX_CLIENTEDGE)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_CLIENTEDGE");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000200L");
	}
	if (dwExStyle & WS_EX_COMPOSITED)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_COMPOSITED");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x02000000L");
	}
	if (dwExStyle & WS_EX_CONTEXTHELP)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_CONTEXTHELP");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000400L");
	}
	if (dwExStyle & WS_EX_CONTROLPARENT)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_CONTROLPARENT");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00010000L");
	}
	if (dwExStyle & WS_EX_DLGMODALFRAME)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_DLGMODALFRAME");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000001L");
	}
	if (dwExStyle & WS_EX_LAYERED)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_LAYERED");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00080000L");
	}
	if (dwExStyle & WS_EX_LAYOUTRTL)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_LAYOUTRTL");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00400000L");
	}
	//if (dwExStyle & WS_EX_LEFT)
	//{
	//	m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_LEFT");
	//	m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000000L");
	//}
	if (dwExStyle & WS_EX_LEFTSCROLLBAR)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_LEFTSCROLLBAR");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00004000L");
	}
	//if (dwExStyle & WS_EX_LTRREADING)
	//{
	//	m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_LTRREADING");
	//	m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000000L");
	//}
	if (dwExStyle & WS_EX_MDICHILD)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_MDICHILD");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000040L");
	}
	if (dwExStyle & WS_EX_NOACTIVATE)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_NOACTIVATE");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x08000000L");
	}
	if (dwExStyle & WS_EX_NOINHERITLAYOUT)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_NOINHERITLAYOUT");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00100000L");
	}
	if (dwExStyle & WS_EX_NOPARENTNOTIFY)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_NOPARENTNOTIFY");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000004L");
	}
	if (dwExStyle & 0x00200000L)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_NOREDIRECTIONBITMAP");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00200000L");
	}
	//if (dwExStyle & WS_EX_OVERLAPPEDWINDOW)
	//{
	//	m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_OVERLAPPEDWINDOW");
	//	m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000300L");
	//}
	//if (dwExStyle & WS_EX_PALETTEWINDOW)
	//{
	//	m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_PALETTEWINDOW");
	//	m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000188L");
	//}
	if (dwExStyle & WS_EX_RIGHT)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_RIGHT");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00001000L");
	}
	//if (dwExStyle & WS_EX_RIGHTSCROLLBAR)
	//{
	//	m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_RIGHTSCROLLBAR");
	//	m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000000L");
	//}
	if (dwExStyle & WS_EX_RTLREADING)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_RTLREADING");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00002000L");
	}
	if (dwExStyle & WS_EX_STATICEDGE)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_STATICEDGE");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00020000L");
	}
	if (dwExStyle & WS_EX_TOOLWINDOW)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_TOOLWINDOW");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000080L");
	}
	if (dwExStyle & WS_EX_TOPMOST)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_TOPMOST");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000008L");
	}
	if (dwExStyle & WS_EX_TRANSPARENT)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_TRANSPARENT");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000020L");
	}
	if (dwExStyle & WS_EX_WINDOWEDGE)
	{
		m_page2.m_list_exstyles.InsertItem(i, L"WS_EX_WINDOWEDGE");
		m_page2.m_list_exstyles.SetItemText(i++, 1, L"0x00000100L");
	}

	m_page2.UpdateData(FALSE);
}

void CMySpyLiteDlg::UpdateClassData(HWND hWnd)
{
	WCHAR szBuf[1024];
	GetClassNameW(hWnd, szBuf, 1024);
	m_page3.m_clsname = szBuf;
	
	DWORD dwStyles = (DWORD)GetClassLongW(hWnd, GCL_STYLE);
	m_page3.m_clsstyles.Format(L"0x%08XL", dwStyles);
	m_page3.m_list_clsstyle.DeleteAllItems();
	int i = 0;
	if (dwStyles & CS_BYTEALIGNCLIENT)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_BYTEALIGNCLIENT");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00001000L");
	}
	if (dwStyles & CS_BYTEALIGNWINDOW)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_BYTEALIGNWINDOW");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00002000L");
	}
	if (dwStyles & CS_CLASSDC)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_CLASSDC");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00000040L");
	}
	if (dwStyles & CS_DBLCLKS)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_DBLCLKS");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00000008L");
	}
	if (dwStyles & CS_DROPSHADOW)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_DROPSHADOW");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00020000L");
	}
	if (dwStyles & CS_GLOBALCLASS)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_GLOBALCLASS");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00004000L");
	}
	if (dwStyles & CS_HREDRAW)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_HREDRAW");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00000002L");
	}
	if (dwStyles & CS_NOCLOSE)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_NOCLOSE");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00000200L");
	}
	if (dwStyles & CS_OWNDC)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_OWNDC");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00000020L");
	}
	if (dwStyles & CS_PARENTDC)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_PARENTDC");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00000080L");
	}
	if (dwStyles & CS_SAVEBITS)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_SAVEBITS");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00000800L");
	}
	if (dwStyles & CS_VREDRAW)
	{
		m_page3.m_list_clsstyle.InsertItem(i, L"CS_VREDRAW");
		m_page3.m_list_clsstyle.SetItemText(i++, 1, L"0x00000001L");
	}

	m_page3.UpdateData(FALSE);
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

void CMySpyLiteDlg::UpdateImageData(HWND hWnd)
{
	CImage &img = m_page5.m_img;

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
}

void CMySpyLiteDlg::UpdateToolsData(HWND hWnd)
{
	m_page6.m_hCurWnd = hWnd;
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
	// : 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
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
		// TODO: 看看当焦点不在编辑框上时按enter会不会关闭程序
		OnBnClickedQuery();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

std::wstring CMySpyLiteDlg::NTFilePath2DosFilePath(std::wstring name)
{
	WCHAR szDriveStr[MAX_PATH+1] = { 0 };
	WCHAR szDeviceStr[MAX_PATH+1] = { 0 };
	WCHAR szDrive[3] = { 0 };
	int cchDevName = 0;

	if (GetLogicalDriveStringsW(MAX_PATH, szDriveStr) == 0)
	{
		return L"";
	}
	for (int i = 0; szDriveStr[i]; i += 4)
	{
		memcpy(szDrive, szDriveStr + i, 2*sizeof(WCHAR));
		if (!QueryDosDeviceW(szDrive, szDeviceStr, MAX_PATH))
		{
			return L"";
		}
		cchDevName = wcslen(szDeviceStr);
		if (wcsnicmp(szDeviceStr, name.c_str(), cchDevName) == 0) //比较前缀
		{
			name.replace(0, cchDevName, szDrive);
			return name;
		}
	}
	return L"";
}
