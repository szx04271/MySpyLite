// SendMsgDlg.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "SendMsgDlg.h"
#include "afxdialogex.h"
#include "Utils.h"

// CSendMsgDlg 对话框

IMPLEMENT_DYNAMIC(CSendMsgDlg, CDialog)

CSendMsgDlg::CSendMsgDlg(HWND hCurWnd, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SEND_MESSAGE, pParent)
	, m_type(0)
	, m_wp(0)
	, m_lp(0)
	, m_result(0)
	, m_hCurWnd(hCurWnd)
	, m_msg(0)
{

}

CSendMsgDlg::~CSendMsgDlg()
{
}

void CSendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_SEND_TYPE, m_type);
	DDX_Text(pDX, IDC_WPARAM, m_wp);
	DDX_Text(pDX, IDC_LPARAM, m_lp);
	DDX_Text(pDX, IDC_RESULT, m_result);
	DDX_Control(pDX, IDC_RESULT, m_resultCtrl);
	DDX_Text(pDX, IDC_MESSAGE, m_msg);
	DDX_Control(pDX, IDC_SEND_TYPE, m_typeCtrl);
}


BEGIN_MESSAGE_MAP(CSendMsgDlg, CDialog)
	ON_BN_CLICKED(IDC_SEND, &CSendMsgDlg::OnBnClickedSend)
	ON_CBN_SELCHANGE(IDC_SEND_TYPE, &CSendMsgDlg::OnCbnSelchangeSendType)
	ON_NOTIFY(NM_CLICK, IDC_MSG_DOC, &CSendMsgDlg::OnNMClickMsgDoc)
END_MESSAGE_MAP()


// CSendMsgDlg 消息处理程序


void CSendMsgDlg::OnBnClickedSend()
{
	// : 在此添加控件通知处理程序代码
	UPDATE_AND_CHECK_HWND();

	if (m_type == 0)
	{
		m_result = ::SendMessageW(m_hCurWnd, m_msg, m_wp, m_lp);
		UpdateData(false);
	}
	else
		::PostMessageW(m_hCurWnd, m_msg, m_wp, m_lp);
}


void CSendMsgDlg::OnCbnSelchangeSendType()
{
	// : 在此添加控件通知处理程序代码
	UpdateData();
	m_result = 0;
	UpdateData(false);
	m_resultCtrl.EnableWindow(m_type == 0);
}


void CSendMsgDlg::OnNMClickMsgDoc(NMHDR* pNMHDR, LRESULT* pResult)
{
	// : 在此添加控件通知处理程序代码
	ShellExecuteW(NULL, L"open", L"https://docs.microsoft.com/en-us/windows/win32/winmsg/about-messages-and-message-queues", NULL, NULL, SW_SHOWDEFAULT);
	*pResult = 0;
}


BOOL CSendMsgDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	m_typeCtrl.AddString(L"SendMessage(同步，有结果)");
	m_typeCtrl.AddString(L"PostMessage(异步，无结果)");
	m_typeCtrl.SetCurSel(m_type);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
