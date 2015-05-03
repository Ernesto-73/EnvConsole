
// EnvConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnvConsole.h"
#include "EnvConsoleDlg.h"
#include "afxdialogex.h"
#include "AddDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnvConsoleDlg dialog

CEnvConsoleDlg::CEnvConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnvConsoleDlg::IDD, pParent)
	, m_time(0)
	, m_bIsAllConnected(false)
	, m_bIsStarted(false)
	, m_iMapScale(0)
	, m_iScaleType(0)
	, m_ClutterRCS(0)
	, m_Coff(0)
	, m_EnvDis(0)
	, m_EleInterf(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strType[0] = "Target";
	m_strType[1] = "Radar";
	m_strType[2] = "Mountain";

	m_strState[0] = "Disconnected";
	m_strState[1] = "Connected";
	this->m_large = CRect(0, 0, 980, 600);
	this->m_small = CRect(0, 0, 535, 600);
	this->m_canvas = CRect(10, 10, 510, 510);
}

void CEnvConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_SEPERATOR, m_seperator);
	DDX_Control(pDX, IDC_DETAILS, m_btnDetails);
	DDX_Control(pDX, ID_START, m_btnStart);
	DDX_Control(pDX, IDC_CONN_ALL, m_btnConnectAll);
	DDX_Control(pDX, IDC_DISCONNECT, m_btnDisconnect);
	DDX_Slider(pDX, IDC_MAP_SCALE, m_iMapScale);
	DDV_MinMaxInt(pDX, m_iMapScale, 100, 2000);
	DDX_Control(pDX, IDC_MAP_SCALE, m_sliderMapScale);
	DDX_Radio(pDX, IDC_SCALE_TYPE_1, m_iScaleType);
	DDX_Text(pDX, IDC_CLUTTER_RCS, m_ClutterRCS);
	DDV_MinMaxDouble(pDX, m_ClutterRCS, 0, 10);
	DDX_Text(pDX, IDC_COFF, m_Coff);
	DDV_MinMaxDouble(pDX, m_Coff, 0, 10);
	DDX_Control(pDX, IDC_COMBO_CLUTTER, m_comboClutterType);
	DDX_Control(pDX, IDC_COMBO_TIME, m_comboTime);
	DDX_Slider(pDX, IDC_ENV_DIS, m_EnvDis);
	DDV_MinMaxInt(pDX, m_EnvDis, 0, 2000);
	DDX_Control(pDX, IDC_ENV_DIS, m_SliderEnvDis);
	DDX_Control(pDX, IDC_ELE_INTERF, m_SliderEleInterf);
	DDX_Slider(pDX, IDC_ELE_INTERF, m_EleInterf);
	DDV_MinMaxInt(pDX, m_EleInterf, 0, 2000);
}

BEGIN_MESSAGE_MAP(CEnvConsoleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONN_ALL, &CEnvConsoleDlg::OnBnClickedConnAll)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CEnvConsoleDlg::OnNMCustomdrawList)
	ON_BN_CLICKED(ID_START, &CEnvConsoleDlg::OnBnClickedStart)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_DETAILS, &CEnvConsoleDlg::OnBnClickedDetails)
	ON_BN_CLICKED(IDC_DISCONNECT, &CEnvConsoleDlg::OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_ADD, &CEnvConsoleDlg::OnBnClickedAdd)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MAP_SCALE, &CEnvConsoleDlg::OnNMCustomdrawMapScale)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ENV_DIS, &CEnvConsoleDlg::OnNMCustomdrawEnvDis)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ELE_INTERF, &CEnvConsoleDlg::OnNMCustomdrawEleInterf)
END_MESSAGE_MAP()


// CEnvConsoleDlg message handlers

BOOL CEnvConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	m_seperator.MoveWindow(520, 5, 2, 550);
//	m_list.Create(LVS_REPORT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS, CRect(0, 0, 200, 500), this, 123);
	CRect r(530, 10, 530 + 425, 310);
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list.MoveWindow(&r);
	m_list.InsertColumn(0, "No.", 0, 30);
	m_list.InsertColumn(1, "Type", 0, 70);
	m_list.InsertColumn(2, "Location", 0, 80);
	m_list.InsertColumn(3, "State", 0, 100);
	m_list.InsertColumn(4, "IP Address", 0, 90);
	m_list.InsertColumn(5, "Port", 0, 50);

	m_iRadarIdx.push_back(m_components.size());
	m_components.push_back(TComponent(1, 1, TLocation(300, 200), 0, CString("127.0.0.1"), 6000));

	m_iTargetIdx.push_back(m_components.size());
	m_components.push_back(TComponent(2, 0, TLocation(421, 87), 0, CString("--"), 0));

	double v = (340. + genRand(1, 30)) / 1000.;
	double vx = -0.6 * v;
	double vy = 0.8 * v;
	double vz = 0;
	Target t(421., 87., 100., vx, vy, 0);
	m_targets.push_back(t);

	m_iStaticIdx.push_back(m_components.size());
	m_components.push_back(TComponent(3, 2, TLocation(331, 132), 0, CString("--"), 0));

	m_iStaticIdx.push_back(m_components.size());
	m_components.push_back(TComponent(4, 2, TLocation(174, 338), 0, CString("--"), 0));

	m_iStaticIdx.push_back(m_components.size());
	m_components.push_back(TComponent(5, 2, TLocation(450, 197), 0, CString("--"), 0));
	UpdateList();

	this->MoveWindow(&m_small);
	CenterWindow();

	m_btnDisconnect.EnableWindow(FALSE);

	for(int i = 1;i < 10;i++)
	{
		CString t;
		t.Format("%d", i * 100);
		m_comboTime.AddString(t);
	}
	m_comboTime.SelectString(0, "100");

	m_comboClutterType.AddString("Land");
	m_comboClutterType.AddString("Ocean");
	m_comboClutterType.AddString("Rain");
	m_comboClutterType.AddString("Mountain");	
	m_comboClutterType.SelectString(0, "Land");

	// DDX variable initial.
	m_sliderMapScale.SetRange(100, 2000);
	m_iMapScale = 1000;
	CString str;
	str.Format("%d", m_iMapScale);
	GetDlgItem(IDC_MAP_SCALE_T)->SetWindowTextA(str);

	m_SliderEnvDis.SetRange(0, 1000);
	m_EnvDis = 190;
	double tmp = m_EnvDis / 100.;
	str.Format("%.2lf", tmp);
	GetDlgItem(IDC_ENV_DIS_T)->SetWindowTextA(str);

	m_SliderEleInterf.SetRange(0, 1000);
	m_EleInterf = 143;
	tmp = m_EleInterf / 100.;
	str.Format("%.2lf", tmp);
	GetDlgItem(IDC_ELE_INTERF_T)->SetWindowTextA(str);

	m_ClutterRCS = 1.84;
	m_Coff = 0.67;
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEnvConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEnvConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CDC xDC;
		dc.SetBkColor(RGB(100,50,0));
		CBitmap xBMP;
		xDC.CreateCompatibleDC(&dc);
		xBMP.CreateCompatibleBitmap(&dc,500,500);
		xDC.SelectObject(&xBMP);
		Draw(&xDC);

		CDC yDC;
		CBitmap yBMP;
		CRect r;
		GetClientRect(&r);
		yDC.CreateCompatibleDC(&dc);

		yBMP.CreateCompatibleBitmap(&dc,r.Width(),r.Height());
		yDC.SelectObject(&yBMP);

		yDC.FillSolidRect(r, GetSysColor(COLOR_3DFACE)); //Get the system color of dialog background

		yDC.BitBlt(m_canvas.left, m_canvas.top, m_canvas.Width(), m_canvas.Height(), &xDC, 0, 0, SRCCOPY);
		dc.BitBlt(0, 0,r.Width(), r.Height(), &yDC, 0, 0, SRCCOPY);

		xBMP.DeleteObject();
		xDC.DeleteDC();
		yBMP.DeleteObject();
		yDC.DeleteDC();
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEnvConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEnvConsoleDlg::OnBnClickedConnAll()
{
	// TODO: Add your control notification handler code here

	// Connect to radar components
	m_bIsAllConnected = true;
	m_socket.clear();
	for(int i = 0;i < (int)m_iRadarIdx.size();i++)
	{
		int idx = m_iRadarIdx[i];
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if(INVALID_SOCKET == sock)
			continue;
		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr = inet_addr(m_components[idx].ip);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(m_components[idx].host);

		if(!connect(sock, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR)))
			m_components[idx].state = STATE_CONNECTED;
		else 
			m_bIsAllConnected = false;
		m_socket.push_back(sock);
	}

	// Connect to static components
	for(int i = 0;i < (int)m_iStaticIdx.size();i++)
	{
		int idx = m_iStaticIdx[i];
		m_components[idx].state = 1;
	}

	// Connect to target components
	for(int i = 0;i < (int)m_iTargetIdx.size();i++)
	{
		int idx = m_iTargetIdx[i];
		m_components[idx].state = 1;
	}
	UpdateList();
	if(m_bIsAllConnected)
	{
		m_btnConnectAll.EnableWindow(FALSE);
		m_btnDisconnect.EnableWindow(TRUE);
	}
	else{
		CString str;
		m_btnDetails.GetWindowTextA(str);
		if(str == "Details")
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_DETAILS, BN_CLICKED), NULL);
		MessageBox("Some components are not connected, simulator\ncannot run correctly.", "Warning", MB_OK|MB_ICONWARNING);
		return ;
	}
}


void CEnvConsoleDlg::UpdateList(void)
{
	m_list.DeleteAllItems();
	for(int i = 0;i < (int)m_components.size();i++)
	{
		int pos = m_list.GetItemCount();
		CString str;
		str.Format("%d", m_components[i].no);
		int nRow = m_list.InsertItem(pos, str);
		m_list.SetItemText(nRow, 1, m_strType[m_components[i].type]);
		str.Format("(%d, %d)",m_components[i].loc.x, m_components[i].loc.y);
		m_list.SetItemText(nRow, 2, str);
		m_list.SetItemText(nRow, 3,  m_strState[m_components[i].state]);
		m_list.SetItemText(nRow, 4, m_components[i].ip);
		if(m_components[i].host == 0)
			str  = "--";
		else 
			str.Format("%d", m_components[i].host);
		m_list.SetItemText(nRow, 5, str);
	}
	/*
	m_list.SetBkColor(RGB(255,255,255));
	m_list.SetTextBkColor(RGB(160,180,220));
	*/
}


void CEnvConsoleDlg::Draw(CDC * pDC)
{
	CRect r;
	GetClientRect(&r);
	pDC->FillSolidRect(&r,RGB(255, 255, 255));

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen *oPen = pDC->SelectObject(&pen);
	CBrush brush(RGB(0, 0, 0));
	CBrush *oBrush = pDC->SelectObject(&brush);

	pDC->MoveTo(0, 0);
	pDC->LineTo(499, 0);

	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 499);

	pDC->MoveTo(0, 499);
	pDC->LineTo(499, 499);

	pDC->MoveTo(499, 0);
	pDC->LineTo(499, 499);

	pen.DeleteObject();
	pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	pDC->SelectObject(&pen);

	for(int i = 1;i < 10;i++)
	{
		pDC->MoveTo(0, i * 50 - 1);
		pDC->LineTo(499, i * 50 - 1);

		pDC->MoveTo(i * 50 - 1, 0);
		pDC->LineTo(i * 50 - 1, 499);
	}

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&pen);

	int sz = 5;
	for(int i = 0;i < (int)m_iStaticIdx.size();i++)
	{
		int idx = m_iStaticIdx[i];
		CPoint pt[3];

		pt[0].x = m_components[idx].loc.x;
		pt[0].y = m_components[idx].loc.y - sz;

		pt[1].x = m_components[idx].loc.x + sz;
		pt[1].y = m_components[idx].loc.y + sz;

		pt[2].x = m_components[idx].loc.x - sz;
		pt[2].y = m_components[idx].loc.y + sz;
		pDC->Polygon(pt, 3);
	}

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	pDC->SelectObject(&pen);
	
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(0, 0, 255));
	pDC->SelectObject(&brush);

	for(int i = 0;i < (int)m_targets.size();i++)
	{
		CRect r((int)m_targets[i].x - sz, 
				(int)m_targets[i].y - sz, 
				(int)m_targets[i].x + sz,
				(int)m_targets[i].y + sz);
		pDC->Ellipse(r);
	}

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(255, 0, 0));
	
	sz = 5;
	int radis = 100;
	for(int i = 0;i < (int)m_iRadarIdx.size();i++)
	{
		pDC->SelectObject(&pen);
		pDC->SelectObject(&brush);
		int idx = m_iRadarIdx[i];
		CPoint pt;
		CRect r(m_components[idx].loc.x - sz, 
			m_components[idx].loc.y - sz, 
			m_components[idx].loc.x + sz,
			m_components[idx].loc.y + sz);
		pDC->FillRect(r, &brush);

		CRect rt(m_components[idx].loc.x - radis, 
			m_components[idx].loc.y - radis, 
			m_components[idx].loc.x + radis,
			m_components[idx].loc.y + radis);
	
		CPen p(PS_DOT, 1, RGB(0, 0, 0));
		pDC->SelectObject(&p);

		CBrush *b = CBrush::FromHandle( (HBRUSH)GetStockObject(NULL_BRUSH));
		pDC->SelectObject(b);

		pDC->MoveTo(m_components[idx].loc.x, m_components[idx].loc.y);
		pDC->LineTo(m_components[idx].loc.x + radis * 3 / 5, m_components[idx].loc.y + radis * 4 / 5);
		pDC->Ellipse(rt);
	}

	int tx = 340;
	int ty = 480;
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	pDC->SetTextColor(RGB(50,50,50));
	CString str;
	str.Format("Simulation Time: %d s", m_time);
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->TextOut(tx, ty, str);

	
	pDC->SelectObject(oPen);
	pDC->SelectObject(oBrush);
}


void CEnvConsoleDlg::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	// TODO: Add your control notification handler code here
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrText = RGB(0, 0, 0), clrTextBk = RGB(255, 255, 255);

		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
		CString str_3 = m_list.GetItemText(nItem ,3);
		CString str_1 = m_list.GetItemText(nItem ,1);

		switch(pLVCD->iSubItem)
		{
		case 3:
			if(str_3 == "Disconnected")
				clrText = RGB(164, 13, 20);
			else
				clrText = RGB(28, 147, 64);
			break;
		case 1:
			if(str_1 == "Radar")
				clrTextBk = RGB(247, 145, 150);
			else if(str_1 == "Target")
				clrTextBk = RGB(154, 159, 228);
			else
				clrTextBk = RGB(178, 178, 178);
			break;
		default:;
		}

		pLVCD->clrText = clrText;
		pLVCD->clrTextBk = clrTextBk;
		*pResult = CDRF_DODEFAULT;
	}
}


void CEnvConsoleDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	if(!m_bIsStarted)
	{
		if(!m_bIsAllConnected)
		{
			MessageBox("Some components are not connected, simulator\ncannot run correctly.", "Warning", MB_OK|MB_ICONWARNING);
			return ;
		}
		SetTimer(1, 100, NULL);
		m_bIsStarted = true;
		m_btnStart.SetWindowTextA("Pause");
		m_btnDisconnect.EnableWindow(FALSE);
	}
	else{
		KillTimer(1);
		m_bIsStarted = false;
		m_btnStart.SetWindowTextA("Start");
		m_btnDisconnect.EnableWindow(TRUE);
	}

}

void CEnvConsoleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	for(int i = 0;i < (int)m_targets.size();++i)
	{
		m_targets[i].Move();
		char sendBuf[200]; 
		sprintf_s(sendBuf, "%lf,%lf,%lf", m_targets[i].x, m_targets[i].y, 0.);
		if(m_bIsAllConnected)
		{
			if(SOCKET_ERROR == send(m_socket[i], sendBuf, strlen(sendBuf) + 1, 0))
			{
				PostMessage(WM_COMMAND, MAKEWPARAM(ID_START, BN_CLICKED), NULL);
				PostMessage(WM_COMMAND, MAKEWPARAM(IDC_DISCONNECT, BN_CLICKED), NULL);
				MessageBox("Connection failed for unkonwn reason.", "Warning", MB_OK|MB_ICONWARNING);
				return ;
			}
		}
	}
	m_time++;
	InvalidateRect(&m_canvas);
	CDialogEx::OnTimer(nIDEvent);
} 


BOOL CEnvConsoleDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CEnvConsoleDlg::OnBnClickedDetails()
{
	// TODO: Add your control notification handler code here
	CString str;
	m_btnDetails.GetWindowTextA(str);
	if(str == "Details")
	{
		this->MoveWindow(&m_large);
		CenterWindow();
		m_btnDetails.SetWindowTextA("Hide");
	}
	
	if(str == "Hide")
	{
		this->MoveWindow(&m_small);
		CenterWindow();
		m_btnDetails.SetWindowTextA("Details");
	}
}


void CEnvConsoleDlg::OnBnClickedDisconnect()
{
	// TODO: Add your control notification handler code here
	//Disconnect from static components
	m_bIsAllConnected = false;
	m_socket.clear();
	for(int i = 0;i < (int)m_iRadarIdx.size();i++)
	{
		int idx = m_iRadarIdx[i];
		m_components[idx].state = STATE_DISCONNECTED;
	}

	// Disconnect from static components
	for(int i = 0;i < (int)m_iStaticIdx.size();i++)
	{
		int idx = m_iStaticIdx[i];
		m_components[idx].state = STATE_DISCONNECTED;
	}

	// Disconnect from target components
	for(int i = 0;i < (int)m_iTargetIdx.size();i++)
	{
		int idx = m_iTargetIdx[i];
		m_components[idx].state = STATE_DISCONNECTED;
	}
	UpdateList();
	m_btnConnectAll.EnableWindow(TRUE);
	m_btnDisconnect.EnableWindow(FALSE);
}


void CEnvConsoleDlg::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
	CAddDialog AddDlg;
	AddDlg.DoModal();
}


void CEnvConsoleDlg::OnNMCustomdrawMapScale(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateData(TRUE);
	CString str;
	str.Format("%d", m_iMapScale);
	GetDlgItem(IDC_MAP_SCALE_T)->SetWindowTextA(str);
	UpdateData(FALSE);
}


void CEnvConsoleDlg::OnNMCustomdrawEnvDis(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateData(TRUE);
	CString str;
	double tmp = m_EnvDis / 100.;
	str.Format("%.2lf", tmp);
	GetDlgItem(IDC_ENV_DIS_T)->SetWindowTextA(str);
	UpdateData(FALSE);
}


void CEnvConsoleDlg::OnNMCustomdrawEleInterf(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateData(TRUE);
	CString str;
	double tmp = m_EleInterf / 100.;
	str.Format("%.2lf", tmp);
	GetDlgItem(IDC_ELE_INTERF_T)->SetWindowTextA(str);
	UpdateData(FALSE);
}
