// StyleListCtrl.cpp: 实现文件
//

#include "pch.h"
#include "MySpyLite.h"
#include "StyleListCtrl.h"


// CStyleListCtrl

IMPLEMENT_DYNAMIC(CStyleListCtrl, CListCtrl)

CStyleListCtrl::CStyleListCtrl()
{

}

CStyleListCtrl::~CStyleListCtrl()
{
}


BEGIN_MESSAGE_MAP(CStyleListCtrl, CListCtrl)
END_MESSAGE_MAP()



// CStyleListCtrl 消息处理程序

void CStyleListCtrl::PreSubclassWindow() {
	::SetWindowTheme(m_hWnd, L"Explorer", nullptr);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	CRect rc;
	GetClientRect(&rc);
	auto cx = rc.Width() - GetSystemMetrics(SM_CXVSCROLL);
	InsertColumn(0, L"样式名", LVCFMT_LEFT, cx * 2 / 3);
	InsertColumn(1, L"样式值", LVCFMT_LEFT, cx * 1 / 3);

	CListCtrl::PreSubclassWindow();
}
