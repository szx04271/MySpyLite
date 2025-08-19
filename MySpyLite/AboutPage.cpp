// AboutPage.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "AboutPage.h"
#include "afxdialogex.h"
#include "Constants.h"

// CAboutPage 对话框

IMPLEMENT_DYNAMIC(CAboutPage, CTabPage)

CAboutPage::CAboutPage(CWnd* pParent /*=nullptr*/)
	: CTabPage(IDD_PAGE_ABOUT, pParent)
{

}

CAboutPage::~CAboutPage()
{
}

void CAboutPage::DoDataExchange(CDataExchange* pDX)
{
	CTabPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutPage, CTabPage)
END_MESSAGE_MAP()


// CAboutPage 消息处理程序



BOOL CAboutPage::OnInitDialog() {
	CTabPage::OnInitDialog();

	GetDlgItem(IDC_VERSION_DISPLAY)->SetWindowTextW(L"版本 " VERSION_STR);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
