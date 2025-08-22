// AddStyleDlg.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "afxdialogex.h"
#include "AddStyleDlg.h"


// CAddStyleDlg 对话框

IMPLEMENT_DYNAMIC(CAddStyleDlg, CDialog)

CAddStyleDlg::CAddStyleDlg(const std::vector<StyleMap::const_iterator>& available_styles,
	CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ADD_STYLE, pParent)
	, m_availableStyles(available_styles)
	, m_styleToAddStr(_T("00000000")), m_customize(FALSE) {

}

CAddStyleDlg::~CAddStyleDlg()
{
}

void CAddStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREDEFINED_STYLES, m_styleListCtrl);
	DDX_Text(pDX, IDC_NEW_STYLE_VALUE, m_styleToAddStr);
	DDX_Check(pDX, IDC_CUSTOMIZE_VALUE, m_customize);
	DDX_Control(pDX, IDC_NEW_STYLE_VALUE, m_styleToAddEdit);
}


BEGIN_MESSAGE_MAP(CAddStyleDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PREDEFINED_STYLES, &CAddStyleDlg::OnLvnItemchangedPredefinedStyles)
	ON_BN_CLICKED(IDC_CUSTOMIZE_VALUE, &CAddStyleDlg::OnBnClickedCustomizeValue)
	ON_BN_CLICKED(IDOK, &CAddStyleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddStyleDlg 消息处理程序

BOOL CAddStyleDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	m_styleToAddEdit.SetLimitText(8); // int32

	int i = 0;
	CString style_value_str;
	for (auto &it : m_availableStyles) {
		m_styleListCtrl.InsertItem(i, it->second);
		style_value_str.Format(L"0x%08XL", it->first);
		m_styleListCtrl.SetItemText(i, 1, style_value_str);
		m_styleListCtrl.SetItemData(i, it->first);

		++i;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAddStyleDlg::OnLvnItemchangedPredefinedStyles(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// when selection changes, update editbox content
	if (pNMLV->uChanged == LVIF_STATE) {
		bool need_update = true;

		// something became selected
		if (!(pNMLV->uOldState & LVIS_SELECTED) && pNMLV->uNewState & LVIS_SELECTED) {
			m_styleToAdd |= static_cast<DWORD>(m_styleListCtrl.GetItemData(pNMLV->iItem));
		}
		// something became deselected
		else if (pNMLV->uOldState & LVIS_SELECTED && !(pNMLV->uNewState & LVIS_SELECTED)) {
			m_styleToAdd &= ~static_cast<DWORD>(m_styleListCtrl.GetItemData(pNMLV->iItem));
		}
		else {
			need_update = false;
		}

		if (need_update) {
			m_styleToAddStr.Format(L"%08X", m_styleToAdd);
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
}

void CAddStyleDlg::OnBnClickedCustomizeValue() {
	UpdateData();
	if (m_customize) {
		m_styleListCtrl.EnableWindow(FALSE);
		m_styleToAddEdit.SetReadOnly(FALSE);
	}
	else {
		m_styleListCtrl.EnableWindow();
		m_styleToAddEdit.SetReadOnly();
		m_styleToAddStr.Format(L"%08X", m_styleToAdd);
		UpdateData(FALSE);
	}
}

void CAddStyleDlg::OnBnClickedOk() {
	if (m_customize) {
		UpdateData();
		const wchar_t *end_ptr = m_styleToAddStr;
		DWORD probable_style_value = wcstoul(m_styleToAddStr, const_cast<wchar_t**>(&end_ptr), 16);
		if (end_ptr[0] != 0) {
			MessageBoxW(L"您输入的不是有效的十六进制数，请重输。\n"
				L"不会输的话，取消勾选“自定义值”，然后从列表里选出你想要的。");
			return;
		}

		m_styleToAdd = probable_style_value;
	}

	CDialog::OnOK();
}
