// StylesPage.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "StylesPage.h"
#include "afxdialogex.h"
#include "Utils.h"
#include "AddStyleDlg.h"

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
	ON_BN_CLICKED(IDC_ADD_WND_STYLE, &CStylesPage::OnBnClickedAddWndStyle)
	ON_BN_CLICKED(IDC_REMOVE_WND_EXSTYLE, &CStylesPage::OnBnClickedRemoveWndExstyle)
	ON_BN_CLICKED(IDC_ADD_WND_EXSTYLE, &CStylesPage::OnBnClickedAddWndExstyle)
END_MESSAGE_MAP()


// CStylesPage 消息处理程序

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
	UPDATE_AND_CHECK_HWND();

	auto pos = m_list_styles.GetFirstSelectedItemPosition();
	if (!pos) {
		MessageBoxW(L"请选中至少一个窗口样式。");
		return;
	}

	int cursel_style_index;
	do {
		cursel_style_index = m_list_styles.GetNextSelectedItem(pos);
		m_styles &= ~static_cast<DWORD>(m_list_styles.GetItemData(cursel_style_index));
	} while (pos);

	SetWindowLongW(theApp.m_curWnd, GWL_STYLE, m_styles);
	SetStyle(GetWindowLongW(theApp.m_curWnd, GWL_STYLE));
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
			m_list_styles.SetItemData(item_index, style_item.first); // store actual style value

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
			m_list_exstyles.SetItemData(item_index, style_item.first); // store actual style value

			++item_index;
		}
	}

	UpdateData(FALSE);
}

void CStylesPage::OnBnClickedAddWndStyle() {
	CHECK_HWND();

	auto available_styles = GetOtherStyles(theApp.m_wndStyleMap, m_list_styles);
	CAddStyleDlg dlg(available_styles, this);
	dlg.DoModal();

	CHECK_HWND();
	SetWindowLongW(theApp.m_curWnd, GWL_STYLE, m_styles | dlg.m_styleToAdd);
	SetStyle(GetWindowLongW(theApp.m_curWnd, GWL_STYLE));
}

void CStylesPage::OnBnClickedRemoveWndExstyle() {
	UPDATE_AND_CHECK_HWND();

	auto pos = m_list_exstyles.GetFirstSelectedItemPosition();
	if (!pos) {
		MessageBoxW(L"请选中至少一个扩展窗口样式。");
		return;
	}

	int cursel_style_index;
	do {
		cursel_style_index = m_list_exstyles.GetNextSelectedItem(pos);
		m_exstyles &= ~static_cast<DWORD>(m_list_exstyles.GetItemData(cursel_style_index));
	} while (pos);

	SetWindowLongW(theApp.m_curWnd, GWL_EXSTYLE, m_exstyles);
	SetExStyle(GetWindowLongW(theApp.m_curWnd, GWL_EXSTYLE));
}

void CStylesPage::OnBnClickedAddWndExstyle() {
	// TODO: 在此添加控件通知处理程序代码
}
