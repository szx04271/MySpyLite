#pragma once
#include "TabPage.h"
#include "StyleListCtrl.h"

// CClassPage 对话框

class CClassPage : public CTabPage
{
	DECLARE_DYNAMIC(CClassPage)

public:
	CClassPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CClassPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_CLASS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_clsname;
	CString m_clsStyleStr;
	CStyleListCtrl m_list_clsstyle;
	afx_msg void OnNMClickClassstylesMsdn(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_atom;
	CString m_hbrBkgnd;
	CString m_clsHicon;
	CString m_clsHiconSm;
	CString m_clsHmodule;
	CString m_clsWndproc;
	afx_msg void OnBnClickedRemoveClsStyle();
	void SetClsStyle(DWORD value);
	DWORD m_clsStyle{};
	afx_msg void OnBnClickedAddClsStyle();
};
