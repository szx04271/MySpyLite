#include "pch.h"
#include "TabPage.h"


CTabPage::CTabPage(int nID, CWnd * pParent)
	: CDialog(nID, pParent)
{
	//CDialog::CDialog(nID, pParent);
}

IMPLEMENT_DYNAMIC(CTabPage, CDialog)

void CTabPage::OnOK()
{
}

BOOL CTabPage::PreTranslateMessage(MSG* pMsg)
{
	// : 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}



BOOL CTabPage::OnInitDialog() {
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(m_hWnd, ETDT_ENABLETAB);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
