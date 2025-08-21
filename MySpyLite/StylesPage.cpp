// StylesPage.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "StylesPage.h"
#include "afxdialogex.h"
#include "Utils.h"

// CStylesPage 对话框

IMPLEMENT_DYNAMIC(CStylesPage, CTabPage)

CStylesPage::CStylesPage(CWnd* pParent /*=nullptr*/)
	: CTabPage(IDD_PAGE_STYLES, pParent)
	, m_stc_styles(_T("基本样式：0x00000000L"))
	, m_stc_exstyles(_T("扩展样式：0x00000000L"))
{

}

CStylesPage::~CStylesPage()
{
}

void CStylesPage::DoDataExchange(CDataExchange* pDX)
{
	CTabPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STYLES, m_list_styles);
	DDX_Control(pDX, IDC_LIST_EXSTYLES, m_list_exstyles);
	DDX_Text(pDX, IDC_BASIC_STYLES, m_stc_styles);
	DDX_Text(pDX, IDC_EX_STYLES, m_stc_exstyles);
}


BEGIN_MESSAGE_MAP(CStylesPage, CTabPage)
	ON_NOTIFY(NM_CLICK, IDC_STYLES_MSDN, &CStylesPage::OnNMClickStylesMsdn)
	ON_NOTIFY(NM_CLICK, IDC_EXSTYLES_MSDN, &CStylesPage::OnNMClickExstylesMsdn)
	ON_BN_CLICKED(IDC_REMOVE_WND_STYLE, &CStylesPage::OnBnClickedRemoveWndStyle)
END_MESSAGE_MAP()


// CStylesPage 消息处理程序


BOOL CStylesPage::OnInitDialog()
{
	CTabPage::OnInitDialog();

	// :  在此添加额外的初始化
	CRect rc;
	int cx;

	SetWindowTheme(m_list_styles, L"Explorer", nullptr);
	m_list_styles.SetExtendedStyle(m_list_styles.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_list_styles.GetClientRect(&rc);
	cx = rc.Width() - GetSystemMetrics(SM_CXVSCROLL);
	m_list_styles.InsertColumn(0, L"样式名称", LVCFMT_LEFT, cx * 2 / 3);
	m_list_styles.InsertColumn(1, L"样式值", LVCFMT_LEFT, cx * 1 / 3);

	SetWindowTheme(m_list_exstyles, L"Explorer", nullptr);
	m_list_exstyles.SetExtendedStyle(m_list_styles.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_list_exstyles.GetClientRect(&rc);
	cx = rc.Width() - GetSystemMetrics(SM_CXVSCROLL);
	m_list_exstyles.InsertColumn(0, L"样式名称", LVCFMT_LEFT, cx * 2 / 3);
	m_list_exstyles.InsertColumn(1, L"样式值", LVCFMT_LEFT, cx * 1 / 3);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CStylesPage::OnNMClickStylesMsdn(NMHDR *pNMHDR, LRESULT *pResult)
{
	// : 在此添加控件通知处理程序代码
	ShellExecuteW(NULL, L"open", L"https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles", NULL, NULL, SW_SHOWDEFAULT);

	*pResult = 0;
}


void CStylesPage::OnNMClickExstylesMsdn(NMHDR *pNMHDR, LRESULT *pResult)
{
	// : 在此添加控件通知处理程序代码
	ShellExecuteW(NULL, L"open", L"https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles", NULL, NULL, SW_SHOWDEFAULT);

	*pResult = 0;
}

void CStylesPage::OnBnClickedRemoveWndStyle() {
	CHECK_HWND();

	auto pos = m_list_styles.GetFirstSelectedItemPosition();
	if (!pos) {
		MessageBoxW(L"请选中一个窗口样式。");
		return;
	}
	auto cur_sel_style_index = m_list_styles.GetNextSelectedItem(pos);
	
}

void CStylesPage::SetStyle(DWORD value) {
	m_styles = value;
	m_stc_styles.Format(L"基本样式：0x%08XL", value);

	m_list_styles.DeleteAllItems();
	int item_index = 0;
	CString style_value_str;
	for (auto& style_item : theApp.m_wndStyleMap) {
		if (value & style_item.first) {
			style_value_str.Format(L"0x%08XL", style_item.first);
			m_list_styles.InsertItem(item_index, style_item.second);
			m_list_styles.SetItemText(item_index, 1, style_value_str);
			++item_index;
		}
	}

	UpdateData(FALSE);
}

void CStylesPage::SetExStyle(DWORD value) {
	m_exstyles = value;
	m_stc_exstyles.Format(L"扩展样式：0x%08XL", value);

	m_list_exstyles.DeleteAllItems();
	int item_index = 0;
	CString style_value_str;
	for (auto& style_item : theApp.m_wndExStyleMap) {
		if (value & style_item.first) {
			style_value_str.Format(L"0x%08XL", style_item.first);
			m_list_exstyles.InsertItem(item_index, style_item.second);
			m_list_exstyles.SetItemText(item_index, 1, style_value_str);
			++item_index;
		}
	}

	UpdateData(FALSE);
}