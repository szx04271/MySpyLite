#pragma once


// CStyleListCtrl

class CStyleListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CStyleListCtrl)

public:
	CStyleListCtrl();
	virtual ~CStyleListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
};


