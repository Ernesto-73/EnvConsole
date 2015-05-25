// Splash.cpp : implementation file
//

#include "stdafx.h"
#include "Splash.h"


// CSplash

IMPLEMENT_DYNAMIC(CSplash, CWnd)

CSplash::CSplash()
{

}

CSplash::~CSplash()
{
}

void CSplash::Create(UINT nBitmapID)  
{  
    m_bitmap.LoadBitmap(nBitmapID);  
	BITMAP bitmap;  
    m_bitmap.GetBitmap(&bitmap);  
	//CreateEx(0,AfxRegisterWndClass(0),"",WS_POPUP|WS_VISIBLE|WS_BORDER,0,0,bitmap.bmWidth,bitmap.bmHeight,NULL,0);  
    CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),  
            NULL, WS_POPUP | WS_VISIBLE, 0, 0, bitmap.bmWidth, bitmap.bmHeight, NULL , NULL); 
	SetTimer(1, 3000, NULL);
}  

BEGIN_MESSAGE_MAP(CSplash, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CSplash message handlers
void CSplash::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

    BITMAP bitmap;  
    m_bitmap.GetBitmap(&bitmap);  
    CDC dcComp;  
    dcComp.CreateCompatibleDC(&dc);  
    dcComp.SelectObject(&m_bitmap);  
    // draw bitmap  
    dc.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dcComp,0,0,SRCCOPY);  
//	SetTimer(1, 4000, NULL);
}


void CSplash::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
	CWnd::OnTimer(nIDEvent);
}


int CSplash::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	return 0;
}
