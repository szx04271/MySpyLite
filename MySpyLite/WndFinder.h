#pragma once
#include "afxwin.h"

class CWndFinder :
	public CStatic
{
public:
	CWndFinder();
	~CWndFinder();
protected:
	bool m_pushed;
	HWND m_hLastFound;
	HPEN m_hPen;
//	HPEN m_hOldPen;
	HBRUSH m_hNullBrush;
//	HBRUSH m_hOldBrush;
//	int m_oldROP;
public:
	HCURSOR m_hCross;
	CWnd *m_pEdit;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void ReleaseLastFound();
};

