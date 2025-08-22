#pragma once
#include "afxdialogex.h"
#include "StyleListCtrl.h"
#include "Utils.h"

// CAddStyleDlg 对话框

class CAddStyleDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddStyleDlg)

public:
	CAddStyleDlg(const std::vector<StyleMap::const_iterator>& available_styles,
		CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddStyleDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_STYLE };
#endif

	const std::vector<StyleMap::const_iterator> &m_availableStyles;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStyleListCtrl m_styleListCtrl;
	afx_msg void OnLvnItemchangedPredefinedStyles(NMHDR* pNMHDR, LRESULT* pResult);

	DWORD m_styleToAdd{};
	CString m_styleToAddStr;
	BOOL m_customize;
	afx_msg void OnBnClickedCustomizeValue();
	CEdit m_styleToAddEdit;
	afx_msg void OnBnClickedOk();
};
