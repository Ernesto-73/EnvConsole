#pragma once
#include "afxwin.h"


// CSplash

class CSplash : public CWnd
{
	DECLARE_DYNAMIC(CSplash)

public:
	CSplash();
	virtual ~CSplash();
	void Create(UINT nBitmapID);

protected:
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bitmap;
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


