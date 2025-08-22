// ClassPage.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "ClassPage.h"
#include "afxdialogex.h"
#include "Utils.h"

// CClassPage 对话框

IMPLEMENT_DYNAMIC(CClassPage, CTabPage)

CClassPage::CClassPage(CWnd* pParent /*=nullptr*/)
	: CTabPage(IDD_PAGE_CLASS, pParent)
	, m_clsname(_T(""))
	, m_clsStyleStr(_T("0x00000000L"))
	, m_atom(_T("")), m_hbrBkgnd(_T("")), m_clsHicon(_T("")), m_clsHiconSm(_T("")), m_clsHmodule(_T("")), m_clsWndproc(_T("")) {

}

CClassPage::~CClassPage()
{
}

void CClassPage::DoDataExchange(CDataExchange* pDX)
{
	CTabPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLASS_NAME_2, m_clsname);
	DDX_Text(pDX, IDC_CLASS_STYLE, m_clsStyleStr);
	DDX_Control(pDX, IDC_LIST_CLASS_STYLES, m_list_clsstyle);
	DDX_Text(pDX, IDC_CLASS_ATOM, m_atom);
	DDX_Text(pDX, IDC_CLASS_HBRBKGND, m_hbrBkgnd);
	DDX_Text(pDX, IDC_CLASS_HICON, m_clsHicon);
	DDX_Text(pDX, IDC_CLASS_HICONSM, m_clsHiconSm);
	DDX_Text(pDX, IDC_CLASS_HMODULE, m_clsHmodule);
	DDX_Text(pDX, IDC_CLASS_WNDPROC, m_clsWndproc);
}


BEGIN_MESSAGE_MAP(CClassPage, CTabPage)
	ON_NOTIFY(NM_CLICK, IDC_CLASSSTYLES_MSDN, &CClassPage::OnNMClickClassstylesMsdn)
	ON_BN_CLICKED(IDC_REMOVE_CLS_STYLE, &CClassPage::OnBnClickedRemoveClsStyle)
	ON_BN_CLICKED(IDC_ADD_CLS_STYLE, &CClassPage::OnBnClickedAddClsStyle)
END_MESSAGE_MAP()


// CClassPage 消息处理程序

void CClassPage::OnNMClickClassstylesMsdn(NMHDR *pNMHDR, LRESULT *pResult)
{
	// : 在此添加控件通知处理程序代码
	ShellExecuteW(NULL, L"open", L"https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles", NULL, NULL, SW_SHOW);

	*pResult = 0;
}

void CClassPage::OnBnClickedRemoveClsStyle() {
	UPDATE_AND_CHECK_HWND();

	auto pos = m_list_clsstyle.GetFirstSelectedItemPosition();
	if (!pos) {
		MessageBoxW(L"请选中至少一个类样式。");
		return;
	}

	int cursel_style_index;
	do {
		cursel_style_index = m_list_clsstyle.GetNextSelectedItem(pos);
		m_clsStyle &= ~static_cast<DWORD>(m_list_clsstyle.GetItemData(cursel_style_index));
	} while (pos);

	SetClassLongW(theApp.m_curWnd, GCL_STYLE, m_clsStyle);
	SetClsStyle(GetClassLongW(theApp.m_curWnd, GCL_STYLE));
}

void CClassPage::SetClsStyle(DWORD value) {
	m_clsStyle = value;
	m_clsStyleStr.Format(L"0x%08XL", value);

	m_list_clsstyle.DeleteAllItems();
	int item_index = 0;
	CString style_value_str;
	for (auto& style_item : theApp.m_clsStyleMap) {
		if (value & style_item.first) {
			style_value_str.Format(L"0x%08XL", style_item.first);
			m_list_clsstyle.InsertItem(item_index, style_item.second);
			m_list_clsstyle.SetItemText(item_index, 1, style_value_str);
			m_list_clsstyle.SetItemData(item_index, style_item.first); // store actual style value

			++item_index;
		}
	}

	UpdateData(FALSE);
}

void CClassPage::OnBnClickedAddClsStyle() {
	
}
