// GeneralPage.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "GeneralPage.h"
#include "afxdialogex.h"
#include "Constants.h"

// CGeneralPage 对话框

IMPLEMENT_DYNAMIC(CGeneralPage, CTabPage)

CGeneralPage::CGeneralPage(CWnd* pParent /*=nullptr*/)
	: CTabPage(IDD_PAGE_GENERAL, pParent)
	, m_clsName(_T(""))
	, m_cltRc(_T(""))
	, m_ctrlId(0)
	, m_path(_T(""))
	, m_pid(0)
	, m_wndRc(_T(""))
	, m_tid(0)
	, m_title(_T(""))
	, m_relativeRc(_T("")), m_wndProcStr(_T("")), m_whether64Bit(_T("")) {
	SHSTOCKICONINFO ssii{ sizeof(ssii) };
	SHGetStockIconInfo(SIID_INFO, SHGSI_ICON | SHGSI_SMALLICON, &ssii);
	m_hInfoIcon = ssii.hIcon;
}

CGeneralPage::~CGeneralPage()
{
	DestroyIcon(m_hInfoIcon);
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CTabPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLASSNAME, m_clsName);
	DDX_Text(pDX, IDC_CLIENT_RECT, m_cltRc);
	DDX_Text(pDX, IDC_CTRL_ID, m_ctrlId);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_PID, m_pid);
	DDX_Text(pDX, IDC_RECT, m_wndRc);
	DDX_Text(pDX, IDC_TID, m_tid);
	DDX_Text(pDX, IDC_TITLE, m_title);
	DDX_Text(pDX, IDC_CLIENT_RELATIVE_POS, m_relativeRc);
	DDX_Control(pDX, IDC_WNDPROC_REMINDER_ICON, m_wndprocReminderIcon);
	DDX_Control(pDX, IDC_USE_WNDPROC, m_useWndProcCtrl);
	DDX_Text(pDX, IDC_WNDPROC, m_wndProcStr);
	DDX_Text(pDX, IDC_WHETHER_64BIT, m_whether64Bit);
	DDX_Control(pDX, IDC_WNDPROC, m_wndprocEditBox);
}


BEGIN_MESSAGE_MAP(CGeneralPage, CTabPage)
	ON_NOTIFY(NM_CLICK, IDC_OPEN_DIR, &CGeneralPage::OnNMClickOpenDir)
	ON_BN_CLICKED(IDC_USE_WNDPROC, &CGeneralPage::OnBnClickedUseWndproc)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CGeneralPage 消息处理程序


void CGeneralPage::OnNMClickOpenDir(NMHDR *pNMHDR, LRESULT *pResult)
{
	// : 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (PathFileExistsW(m_path))
		ShellExecuteW(NULL, L"open", L"explorer.exe", L"/select," + m_path, NULL, SW_SHOWNORMAL);
	else
		MessageBoxW(L"文件不存在。", L"错误", MB_ICONWARNING);

	*pResult = 0;
}

BOOL CGeneralPage::OnInitDialog() {
	CTabPage::OnInitDialog();

	m_wndprocReminderIcon.SetIcon(m_hInfoIcon);

	// judge if we need wndproc
	CRegKey reg_key;
	LSTATUS status = reg_key.Open(HKEY_CURRENT_USER, g_regSubKey);
	if (status == ERROR_SUCCESS) {
		DWORD need_wndproc_dw = 0;
		status = reg_key.QueryDWORDValue(g_regValueName, need_wndproc_dw);
		if (status == ERROR_SUCCESS && need_wndproc_dw) {
			TryReleaseWndProcFinderDll();
			if (theApp.m_wndProcFinderDllPath.empty() == false) {
				m_useWndProcCtrl.SetCheck(TRUE);
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGeneralPage::OnBnClickedUseWndproc() {
	if (m_useWndProcCtrl.GetCheck()) {
		TryReleaseWndProcFinderDll();
	}
	else {
		m_wndProcStr.Empty();
		m_wndprocEditBox.SetWindowTextW(L"");
	}
}

void CGeneralPage::TryReleaseWndProcFinderDll() {
	bool release_success = ReleaseWndProcFinderDll();
	if (!release_success) {
		DWORD err = GetLastError();
		CString err_msg;
		err_msg.Format(L"释放DLL失败，last error为%u。", err);
		MessageBoxW(err_msg);
		m_useWndProcCtrl.SetCheck(FALSE);
		m_wndprocEditBox.SetWindowTextW(L"");
	}
}

void CGeneralPage::OnDestroy() {
	CRegKey reg_key;
	auto st = reg_key.Create(HKEY_CURRENT_USER, g_regSubKey);
	if (st == ERROR_SUCCESS) {
		reg_key.SetDWORDValue(g_regValueName, m_useWndProcCtrl.GetCheck());
	}

	CTabPage::OnDestroy();
}
