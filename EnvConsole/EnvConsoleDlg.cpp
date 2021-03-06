
// EnvConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnvConsole.h"
#include "EnvConsoleDlg.h"
#include "afxdialogex.h"
#include "AddDialog.h"
#include "DBOptions.h"

#import "msxml3.dll"
 
using namespace MSXML2;

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
	, m_bIsStarted(false)
	, m_iMapScale(0)
	, m_bIsAllConnected(false)
	, m_iScaleType(0)
	, m_ClutterRCS(0)
	, m_Coff(0)
	, m_EnvDis(0)
	, m_EleInterf(0)
	, m_iSelected(-1)
	, m_env(NULL)
	, m_conn(NULL)
	, m_mouseLoc(0)
	, m_nElapse(100)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAP);
	m_font.CreatePointFont(80, "Verdana", NULL);

	// Initialize constant strings.
	m_strType[0] = "Static";
	m_strType[1] = "Radar";
	m_strType[2] = "Target";

	m_strState[0] = "Disconnected";
	m_strState[1] = "Connected";

	// Initialize window size.
	this->m_large = CRect(0, 0, 980, 650);
	this->m_small = CRect(0, 0, 525, 650);
	this->m_canvas = CRect(10, 30, 510, 530);

	// Initialize options array.
	memset(m_arrOptions, 0, sizeof(int) * NUM);
	m_arrOptions[TARGET_LOCATION] = 1;
	m_arrOptions[DRAW_GRIDS] = 1;
}

void CEnvConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_SEPARATOR, m_separator);
	DDX_Control(pDX, IDC_DETAILS, m_btnDetails);
	DDX_Control(pDX, ID_START, m_btnStart);
	DDX_Control(pDX, IDC_CONN_ALL, m_btnConnectAll);
	DDX_Control(pDX, IDC_DISCONNECT, m_btnDisconnect);
	DDX_Slider(pDX, IDC_MAP_SCALE, m_iMapScale);
	DDV_MinMaxInt(pDX, m_iMapScale, 1, 20);
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
	ON_BN_CLICKED(IDCANCEL, &CEnvConsoleDlg::OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CEnvConsoleDlg::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_DELETE, &CEnvConsoleDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_SHOW_STATIC_OBJS, &CEnvConsoleDlg::OnBnClickedShowStaticObjs)
	ON_BN_CLICKED(IDC_IMPORT, &CEnvConsoleDlg::OnBnClickedImport)
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_TARGET_LOCATION, &CEnvConsoleDlg::OnTargetlocation)
	ON_COMMAND(ID_RADAR_LOCATION, &CEnvConsoleDlg::OnRadarLocation)
	ON_COMMAND(ID_STATIC_LOCATION, &CEnvConsoleDlg::OnStaticLocation)
	ON_BN_CLICKED(IDC_EXPORT, &CEnvConsoleDlg::OnBnClickedExport)
	ON_COMMAND(ID_DATABASE_CONNECT, &CEnvConsoleDlg::OnDatabaseConnect)
	ON_COMMAND(ID_DATABASE_DISCONNECT, &CEnvConsoleDlg::OnDatabaseDisconnect)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_DISCONNECT, &CEnvConsoleDlg::OnUpdateDatabaseDisconnect)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_CONNECT, &CEnvConsoleDlg::OnUpdateDatabaseConnect)
	ON_WM_INITMENUPOPUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_RULER, &CEnvConsoleDlg::OnRuler)
	ON_UPDATE_COMMAND_UI(ID_RULER, &CEnvConsoleDlg::OnUpdateRuler)
	ON_UPDATE_COMMAND_UI(ID_TARGET_LOCATION, &CEnvConsoleDlg::OnUpdateTargetLocation)
	ON_UPDATE_COMMAND_UI(ID_RADAR_LOCATION, &CEnvConsoleDlg::OnUpdateRadarLocation)
	ON_UPDATE_COMMAND_UI(ID_STATIC_LOCATION, &CEnvConsoleDlg::OnUpdateStaticLocation)
	ON_COMMAND(ID_DATABASE_CONFIGURATION, &CEnvConsoleDlg::OnDatabaseConfiguration)
	ON_MESSAGE(WM_PROGRESS, &CEnvConsoleDlg::OnProgress)
	ON_COMMAND(ID_FILE_IMPORT, &CEnvConsoleDlg::OnFileImport)
	ON_COMMAND(ID_FILE_EXPORT, &CEnvConsoleDlg::OnFileExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, &CEnvConsoleDlg::OnUpdateFileExport)
	ON_COMMAND(ID_FILE_CLEARALL, &CEnvConsoleDlg::OnFileClearall)
	ON_COMMAND(ID_DATABASE_UPLOAD, &CEnvConsoleDlg::OnDatabaseUpload)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_UPLOAD, &CEnvConsoleDlg::OnUpdateDatabaseUpload)
	ON_COMMAND(ID_DATABASE_DOWNLOAD, &CEnvConsoleDlg::OnDatabaseDownload)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_DOWNLOAD, &CEnvConsoleDlg::OnUpdateDatabaseDownload)
	ON_COMMAND(ID_DRAW_GRIDS, &CEnvConsoleDlg::OnDrawGrids)
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

	m_MainMenu.LoadMenu(IDR_MAINMENU); // Add main menu.
	SetMenu(&m_MainMenu);
	
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)  ||
		!m_wndToolBar.LoadToolBar(IDR_MAINTOOLBAR))
	{
		TRACE0("Failed to craete tool bar\n");
		return -1;
	}

	m_wndToolBar.SetButtonStyle(1,TBBS_CHECKBOX);
	m_wndToolBar.SetButtonStyle(2,TBBS_CHECKBOX);
	m_wndToolBar.SetButtonStyle(3, TBBS_DISABLED);
	m_wndToolBar.SetButtonStyle(4, TBBS_CHECKED);

	m_StatusBar.CreateEx(this,SBT_TOOLTIPS,WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,AFX_IDW_STATUS_BAR );
	m_StatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
/*
	CRect rect;
	GetClientRect(&rect);

	m_StatusBar.SetPaneInfo(0,ID_INDICATOR_CAPS,SBPS_NORMAL,rect.Width()/3);
	m_StatusBar.SetPaneInfo(1,ID_INDICATOR_NUM,SBPS_STRETCH ,rect.Width()/3);
*/
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,ID_INDICATOR_CAPS);
	m_StatusBar.SetPaneText(0, "",0);
	CTime t = CTime::GetCurrentTime();
	CString time = t.Format("  %H:%M:%S");
	m_StatusBar.SetPaneText(2,time,0);
	SetTimer(EVENT_CLOCK, 1000, NULL);
	m_StatusBar.GetStatusBarCtrl().SetIcon(3, AfxGetApp()->LoadIconA(IDI_DISCONNECT));
	CRect rect;
	m_StatusBar.GetItemRect(1, &rect);
	m_progress.Create(WS_CHILD | WS_VISIBLE, rect, &m_StatusBar, 100);
	m_progress.SetRange(0, 4000);
	m_progress.SetStep(1);
	
	// Set controls font.
	m_btnStart.SetFont(&m_font);
	m_btnConnectAll.SetFont(&m_font);
	m_btnDetails.SetFont(&m_font);
	m_btnDisconnect.SetFont(&m_font);
	m_btnStart.SetFont(&m_font);
	m_list.SetFont(&m_font);

	GetDlgItem(IDC_STATIC)->SetFont(&m_font);
	GetDlgItem(IDC_ADD)->SetFont(&m_font);
	GetDlgItem(IDC_IMPORT)->SetFont(&m_font);
	GetDlgItem(IDC_DELETE)->SetFont(&m_font);
	GetDlgItem(IDCANCEL)->SetFont(&m_font);
	GetDlgItem(IDC_SHOW_STATIC_OBJS)->SetFont(&m_font);
	GetDlgItem(IDC_SCALE_TYPE_1)->SetFont(&m_font);
	GetDlgItem(IDC_SCALE_TYPE_2)->SetFont(&m_font);

	// Separator position.
	m_separator.MoveWindow(520, 5, 2, 550);
	
	// Initialize list control.
	CRect r(530, 10, 955, 310);	// position.
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT ); // list control style: report
	
	m_list.MoveWindow(&r);
	
	// Add columns to list control.
	m_list.InsertColumn(0, "No.", 0, 50);
	m_list.InsertColumn(1, "Type", 0, 70);
	m_list.InsertColumn(2, "Location(Origin)", 0, 110);
	m_list.InsertColumn(3, "State", 0, 80);
	m_list.InsertColumn(4, "IP Address", 0, 90);
	m_list.InsertColumn(5, "Port", 0, 50);
	m_list.InsertColumn(6, "RCS", 0, 50);

	// Add some random components.
	m_components.push_back(TComponent(m_components.size() + 1, TYPE_RADAR, TLocation(300.9, 200.97), 0, CString("127.0.0.1"), 6000));

	double v = (1400. + genRand(1, 30)) / 1000.;
	double vx = -0.6 * v;
	double vy = 0.8 * v;
	double vz = 0;
	m_components.push_back(TComponent(m_components.size() + 1, TYPE_TARGET, TLocation(421.23, 87.03), 0, CString("--"), 0, TSpeed(vx, vy, vz)));

	m_components.push_back(TComponent(m_components.size() + 1, TYPE_STATIC, TLocation(331.86, 132.0), 0, CString("--"), 0));
	m_components.push_back(TComponent(m_components.size() + 1, TYPE_STATIC, TLocation(174.49, 338.99), 0, CString("--"), 0));
	m_components.push_back(TComponent(m_components.size() + 1, TYPE_STATIC, TLocation(450.32, 197.91), 0, CString("--"), 0));

	for(int i = 0;i < 5;i++)
	{
		Sleep(50);
		double x = genRand(1, 316) * genRand(1, 316) / 99.;
		Sleep(50);
		double y = genRand(1, 316) * genRand(1, 316) / 99.;
		m_components.push_back(TComponent(m_components.size() + 1, TYPE_STATIC, TLocation(x, y), 0, CString("--"), 0));
	}

	v = (100. + genRand(1, 30)) / 1000.;
	vx = 0.7 * v;
	vy = 0.7 * v;
	vz = 0;
	m_components.push_back(TComponent(m_components.size() + 1, TYPE_TARGET, TLocation(197.83, 128.73), 0, CString("--"), 0, TSpeed(vx, vy, vz)));

	v = (140. + genRand(1, 30)) / 1000.;
	vx = 0.6 * v;
	vy = -0.5 * v;
	vz = 0;
	m_components.push_back(TComponent(m_components.size() + 1, TYPE_TARGET, TLocation(239.34, 230.92), 0, CString("--"), 0, TSpeed(vx, vy, vz)));

	v = (90. + genRand(1, 30)) / 1000.;
	vx = 0.1 * v;
	vy = -0.8 * v;
	vz = 0;
	m_components.push_back(TComponent(m_components.size() + 1, TYPE_TARGET, TLocation(331.84, 330.12), 0, CString("--"), 0, TSpeed(vx, vy, vz)));

	for(int i = 0;i < 5;i++)
	{
		Sleep(50);
		double x = genRand(1, 316) * genRand(1, 316) / 99.;
		Sleep(50);
		double y = genRand(1, 316) * genRand(1, 316) / 99.;
		m_components.push_back(TComponent(m_components.size() + 1, TYPE_STATIC, TLocation(x, y), 0, CString("--"), 0));
	}
	// Update context of list control.
	UpdateList(); 

	// Set initial window size && button IDC_DETAILS caption
	m_btnDetails.SetWindowTextA(_T("Hide"));
	MoveWindow(&m_large);
	CenterWindow();

	m_btnDisconnect.EnableWindow(FALSE);
	m_btnStart.EnableWindow(FALSE);

	// Initialize IDC_COMBO_TIME controls context.
	for(int i = 1;i < 10;i++)
	{
		CString t;
		t.Format("%d", i * 20);
		m_comboTime.AddString(t);
	}
	m_comboTime.SelectString(0, "100");

	// Initialize IDC_COMBO_CLUTTER controls context.
	m_comboClutterType.AddString("Land");
	m_comboClutterType.AddString("Ocean");
	m_comboClutterType.AddString("Rain");
	m_comboClutterType.AddString("Mountain");	
	m_comboClutterType.SelectString(0, "Land");

	// DDX variable initial.
	m_sliderMapScale.SetRange(1, 20);
	m_iMapScale = 1;
	CString str;
	str.Format("%d", m_iMapScale);
	GetDlgItem(IDC_MAP_SCALE_T)->SetWindowTextA(str);

	m_SliderEnvDis.SetRange(0, 1000);
	m_EnvDis = 190;
	double tmp = m_EnvDis / 100.;
	str.Format("%.2lf", tmp);
	GetDlgItem(IDC_ENV_DIS_T)->SetWindowTextA(str);

	m_SliderEleInterf.SetRange(0, 1000);
	m_EleInterf = 643;
	tmp = m_EleInterf / 100.;
	str.Format("%.2lf", tmp);
	GetDlgItem(IDC_ELE_INTERF_T)->SetWindowTextA(str);

	m_ClutterRCS = 1.84;
	m_Coff = 0.67;
	UpdateData(FALSE);
	
	((CButton *)GetDlgItem(IDC_SHOW_STATIC_OBJS))->SetCheck(1);

/*
	ImportFromFile(_T("E:\\map_0.xml"));
	UpdateList();
	InvalidateRect(&m_canvas);
*/
	if(m_components.size() == 0)
		((CButton *)GetDlgItem(IDC_EXPORT))->EnableWindow(FALSE);
	else 
		((CButton *)GetDlgItem(IDC_EXPORT))->EnableWindow(TRUE);

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
		xBMP.CreateCompatibleBitmap(&dc, 500, 500);
		xDC.SelectObject(&xBMP);
		Draw(&xDC);

		CDC yDC;
		CBitmap yBMP;
		CRect r;
		GetClientRect(&r);
		yDC.CreateCompatibleDC(&dc);

		yBMP.CreateCompatibleBitmap(&dc,r.Width(),r.Height());
		yDC.SelectObject(&yBMP);

		COLORREF clr = GetSysColor(COLOR_3DFACE);
		yDC.FillSolidRect(r, clr); //Get the system color of dialog background

		yDC.BitBlt(m_canvas.left, m_canvas.top, m_canvas.Width(), m_canvas.Height(), &xDC, 0, 0, SRCCOPY);
		dc.BitBlt(0, 0, r.Width(), r.Height(), &yDC, 0, 0, SRCCOPY);

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
	
	// Connect to radar components.
	m_bIsConnected.resize(m_iRadarIdx.size());
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
		addrSrv.sin_port = htons(static_cast<u_short>(m_components[idx].host));

		if(!connect(sock, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR)))
		{
			m_components[idx].state = STATE_CONNECTED;
			m_bIsConnected[i] = true;
		}
		else {
			m_bIsConnected[i] = false;
		}
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
	CheckConnection();
	InvalidateRect(&m_canvas);
	m_btnDisconnect.EnableWindow(TRUE);
	m_btnStart.EnableWindow(TRUE);
	if(m_bIsAllConnected)
	{
		m_btnConnectAll.EnableWindow(FALSE);
	}
	else{
		CString str;
		m_btnDetails.GetWindowTextA(str);
		if(str == "Details")
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_DETAILS, BN_CLICKED), NULL);
		MessageBox("Some components are not connected, simulator\nmay not run correctly.", "Warning", MB_OK|MB_ICONWARNING);
		return ;
	}
}


void CEnvConsoleDlg::UpdateList(void)
{
	m_list.DeleteAllItems();
	m_iTargetIdx.clear();
	m_iStaticIdx.clear();
	m_iRadarIdx.clear();

	int idx = 0;
	for(std::vector<TComponent>::iterator it = m_components.begin();it != m_components.end();it++)
	{
		switch(it->type)
		{
		case TYPE_RADAR:
			m_iRadarIdx.push_back(idx);
			break;
		case TYPE_TARGET:
			m_iTargetIdx.push_back(idx);
			break;
		case TYPE_STATIC:
			m_iStaticIdx.push_back(idx);
			break;
		}
		idx++;

		if(m_arrOptions[HIDE_STATIC_OBJ] == 1 && it->type == TYPE_STATIC)
			continue;

		int pos = m_list.GetItemCount();
		CString str;
		str.Format("#%d", it->no);
		int nRow = m_list.InsertItem(pos, str);
		m_list.SetItemText(nRow, 1, m_strType[it->type]);
		str.Format("(%.2f, %.2f)",it->loc.x,it->loc.y);
		m_list.SetItemText(nRow, 2, str);
		m_list.SetItemText(nRow, 3,  m_strState[it->state]);

		if(it->ip == _T("0.0.0.0"))
			m_list.SetItemText(nRow, 4, _T("Auto Deploy"));
		else
			m_list.SetItemText(nRow, 4, it->ip);

		if(it->host == 0)
			str  = "--";
		else 
			str.Format("%d", it->host);
		m_list.SetItemText(nRow, 5, str);
		m_list.SetItemText(nRow, 6, _T("10.0"));
	}
}


void CEnvConsoleDlg::Draw(CDC * pDC)
{
	// Reset background color.
	CRect r;
	GetClientRect(&r);
	pDC->FillSolidRect(&r,RGB(0, 0, 0));
	CString str;

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen *oPen = pDC->SelectObject(&pen);
	CBrush brush(RGB(0, 0, 0));
	CBrush *oBrush = pDC->SelectObject(&brush);

	// Draw the border
	pDC->MoveTo(0, 0);
	pDC->LineTo(499, 0);

	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 499);

	pDC->MoveTo(0, 499);
	pDC->LineTo(499, 499);

	pDC->MoveTo(499, 0);
	pDC->LineTo(499, 499);

	// Draw grids
	CFont font;
	font.CreateFont(14, 7, -100, 0, 10, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_ROMAN, "Verdana");	
	CFont *oFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255, 255, 255));

	if(m_arrOptions[DRAW_GRIDS])
	{
		pen.DeleteObject();
		pen.CreatePen(PS_DOT, 1, RGB(0, 150, 0));
		pDC->SelectObject(&pen);

		str = "0";
		pDC->TextOut(5, 5, str);
		for(int i = 1;i < 10;i++)
		{
			str.Format("%.0f", i * m_iMapScale * 50. / 10);
			pDC->TextOut(5, i * 50 - 1, str);
			pDC->TextOut(i * 50+5, 5, str);

			pDC->MoveTo(0, i * 50 - 1);
			pDC->LineTo(499, i * 50 - 1);

			pDC->MoveTo(i * 50 - 1, 0);
			pDC->LineTo(i * 50 - 1, 499);
		}
	}

	// Restore Pen
	font.DeleteObject();
	font.CreatePointFont(90, "Monaco", NULL);
	pDC->SelectObject(&font);

	// Set the text position
	int tx = 340;
	int ty = 480;

	// Set font size && color
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);


	str.Format("Simulation Time: %d s", m_time);

	// Set font text background color
	pDC->SetBkColor(RGB(0, 0, 0));
	pDC->TextOut(tx, ty, str);

	// Draw static objects
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	pDC->SelectObject(&pen);

	int sz = 6;
	for(int i = 0;i < (int)m_iStaticIdx.size();i++)
	{
		int idx = m_iStaticIdx[i];
		CPoint pt[3];

		pt[0].x = static_cast<int>(m_components[idx].loc.x / m_iMapScale);
		pt[0].y = static_cast<int>(m_components[idx].loc.y / m_iMapScale) - sz;

		pt[1].x = static_cast<int>(m_components[idx].loc.x / m_iMapScale) + sz;
		pt[1].y = static_cast<int>(m_components[idx].loc.y / m_iMapScale) + sz;

		pt[2].x = static_cast<int>(m_components[idx].loc.x / m_iMapScale) - sz;
		pt[2].y = static_cast<int>(m_components[idx].loc.y / m_iMapScale) + sz;
		pDC->Polygon(pt, 3);
		if(m_arrOptions[STATIC_LOCATION] == 1)
		{
			CString tmp;
			tmp.Format("(%.2lf, %.2lf)", m_components[idx].loc.x, m_components[idx].loc.y);
			pDC->TextOut(pt[1].x + 1, pt[1].y + 1,tmp);
		}
	}

	// Draw target objects
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	pDC->SelectObject(&pen);
	
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(0, 0, 255));
	pDC->SelectObject(&brush);

	for(int i = 0;i < (int)m_iTargetIdx.size();i++)
	{
		int idx = m_iTargetIdx[i];
		CRect r((int)m_components[idx].loc.x / m_iMapScale - sz, 
				(int)m_components[idx].loc.y / m_iMapScale - sz, 
				(int)m_components[idx].loc.x / m_iMapScale + sz,
				(int)m_components[idx].loc.y / m_iMapScale + sz);
		pDC->Ellipse(r);
		
		if(m_arrOptions[TARGET_LOCATION] == 1)
		{
			CString tmp;
			tmp.Format("(%.2lf, %.2lf)", m_components[idx].loc.x, m_components[idx].loc.y);
			pDC->TextOut((int)m_components[idx].loc.x / m_iMapScale + sz + 1, (int)m_components[idx].loc.y / m_iMapScale + sz + 1, tmp);
		}
	}

	// Draw radar objects
	int radis = 100 / m_iMapScale;
	for(int i = 0;i < (int)m_iRadarIdx.size();i++)
	{
		pen.DeleteObject();
		brush.DeleteObject();
		if(m_components[m_iRadarIdx[i]].state == STATE_CONNECTED)
		{
			pen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			brush.CreateSolidBrush(RGB(0, 255, 0));
		}
		else{
			pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			brush.CreateSolidBrush(RGB(255, 0, 0));
		}
		pDC->SelectObject(&pen);
		pDC->SelectObject(&brush);
		int idx = m_iRadarIdx[i];
		CPoint pt;
		CRect r(static_cast<int>(m_components[idx].loc.x / m_iMapScale) - sz, 
			static_cast<int>(m_components[idx].loc.y / m_iMapScale) - sz, 
			static_cast<int>(m_components[idx].loc.x / m_iMapScale) + sz,
			static_cast<int>(m_components[idx].loc.y/ m_iMapScale) + sz);
		pDC->FillRect(r, &brush);
		pDC->Rectangle(&r);

		if(m_arrOptions[RADAR_LOCATION] == 1)
		{
			CString tmp;
			tmp.Format("(%.2lf, %.2lf)", m_components[idx].loc.x, m_components[idx].loc.y);
			pDC->TextOut((int)m_components[idx].loc.x / m_iMapScale + sz + 1, (int)m_components[idx].loc.y / m_iMapScale + sz + 1, tmp);
		}
	
		CRect rt(static_cast<int>(m_components[idx].loc.x / m_iMapScale) - radis, 
				static_cast<int>(m_components[idx].loc.y / m_iMapScale) - radis, 
				static_cast<int>(m_components[idx].loc.x / m_iMapScale) + radis,
				static_cast<int>(m_components[idx].loc.y / m_iMapScale) + radis);
	
		CPen p(PS_DOT, 1, RGB(255, 255, 255));
		pDC->SelectObject(&p);

		CBrush *b = CBrush::FromHandle( (HBRUSH)GetStockObject(NULL_BRUSH));
		pDC->SelectObject(b);

		pDC->MoveTo(static_cast<int>(m_components[idx].loc.x / m_iMapScale), static_cast<int>(m_components[idx].loc.y / m_iMapScale));
		pDC->LineTo(static_cast<int>(m_components[idx].loc.x / m_iMapScale) + radis * 3 / 5, static_cast<int>(m_components[idx].loc.y / m_iMapScale) + radis * 4 / 5);
		pDC->Ellipse(rt);
	}

	// Draw selected objects.
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(200, 150, 0));
	pDC->SelectObject(&pen);

	CBrush *b = CBrush::FromHandle( (HBRUSH)GetStockObject(NULL_BRUSH));
	pDC->SelectObject(b);

	if(m_iSelected != -1)
	{
	/*
		CRect rt(static_cast<int>(m_components[m_iSelected].loc.x / m_iMapScale) - 15, 
			static_cast<int>(m_components[m_iSelected].loc.y / m_iMapScale) - 15, 
			static_cast<int>(m_components[m_iSelected].loc.x / m_iMapScale) + 15,
			static_cast<int>(m_components[m_iSelected].loc.y / m_iMapScale) + 15);
		pDC->Ellipse(rt);
	*/
		int x = (int)m_components[m_iSelected].loc.x / m_iMapScale;
		int y = (int)m_components[m_iSelected].loc.y / m_iMapScale;

		int a = 10;
		int b = 5;

		pDC->MoveTo(x - a, y - a);
		pDC->LineTo(x - a, y - b);

		pDC->MoveTo(x - a, y - a);
		pDC->LineTo(x - b, y - a);

		pDC->MoveTo(x + a, y - a);
		pDC->LineTo(x + a, y - b);

		pDC->MoveTo(x + a, y - a);
		pDC->LineTo(x + b, y - a);

		pDC->MoveTo(x + a, y + a);
		pDC->LineTo(x + a, y + b);

		pDC->MoveTo(x + a, y + a);
		pDC->LineTo(x + b, y + a);

		pDC->MoveTo(x - a, y + a);
		pDC->LineTo(x - a, y + b);

		pDC->MoveTo(x - a, y + a);
		pDC->LineTo(x - b, y + a);
	}
	
	// Arrow
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(240, 100, 0));
	pDC->SelectObject(&pen);
	if(m_arrOptions[ARROW_ON])
	{
		pDC->MoveTo(m_mouseLoc.x, 0);
		pDC->LineTo(m_mouseLoc.x, 500);
		pDC->MoveTo(0, m_mouseLoc.y);
		pDC->LineTo(500, m_mouseLoc.y);
	}

	pDC->SelectObject(oFont);
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
			if(IDNO == MessageBox("Some components are not connected, simulator may not run correctly. Are you sure to continue?", "Warning", MB_YESNO|MB_ICONWARNING))
				return ;
		}
	
		SetTimer(EVENT_SIM, m_nElapse, NULL);
		m_bIsStarted = true;
		m_btnStart.SetWindowTextA("Stop");
		m_btnDisconnect.EnableWindow(FALSE);
		m_btnConnectAll.EnableWindow(FALSE);
	}
	else{
		KillTimer(1);
		m_bIsStarted = false;
		m_btnStart.SetWindowTextA("Start");
		m_btnDisconnect.EnableWindow(TRUE);
		m_btnConnectAll.EnableWindow(TRUE);
	}

}

void CEnvConsoleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == EVENT_CLOCK)
	{
		CTime t = CTime::GetCurrentTime();
		CString time = t.Format("  %H:%M:%S");
		m_StatusBar.SetPaneText(2, time);
		return ;
	}

	for(int i = 0;i < (int)m_iTargetIdx.size();++i)
		m_components[m_iTargetIdx[i]].Move(m_nElapse);

	for(int k = 0;k < (int)m_iRadarIdx.size();k++)
	{
		CString buf;
		for(int i = 0;i < (int)m_iTargetIdx.size();++i)
		{
			int idx = m_iTargetIdx[i];
			double dis = Distance(	m_components[m_iRadarIdx[k]].loc.x,
									m_components[m_iRadarIdx[k]].loc.y,  
									m_components[idx].loc.x,  
									m_components[idx].loc.y);
			if(dis > 100.)
				continue;
			CString tmp;
			tmp.Format("%lf,%lf,%lf", m_components[idx].loc.x, m_components[idx].loc.y, m_components[idx].loc.z);
			buf += tmp;
			buf += ";";
		}

		for(int i = 0;i < (int)m_iStaticIdx.size();i++)
		{
			double dis = Distance(	m_components[m_iRadarIdx[k]].loc.x, 
									m_components[m_iRadarIdx[k]].loc.y, 
									m_components[m_iStaticIdx[i]].loc.x,
									m_components[m_iStaticIdx[i]].loc.y);
			if(dis > 100.)
				continue;
			CString tmp;
			int idx = m_iStaticIdx[i];
			tmp.Format("%d,%d,%d", static_cast<int>(m_components[idx].loc.x), static_cast<int>(m_components[idx].loc.y), 0);
			buf += tmp;
			buf += ";";
		}
		buf += "\0";
		char *sendBuf = buf.GetBuffer();

		if(!m_bIsConnected[k])
			continue;
	/*
		if(m_bIsAllConnected)
		{
			if(SOCKET_ERROR == send(m_socket[k], sendBuf, buf.GetLength() + 1, 0))
			{
				PostMessage(WM_COMMAND, MAKEWPARAM(ID_START, BN_CLICKED), NULL);
				PostMessage(WM_COMMAND, MAKEWPARAM(IDC_DISCONNECT, BN_CLICKED), NULL);
				MessageBox("Connection failed for unknown reason. Maybe a component or some of them has been disconnected. Please check your nerwork configuratoin.", "Warning", MB_OK|MB_ICONWARNING);
				return ;
			}
		}
	*/

		if(SOCKET_ERROR == send(m_socket[k], sendBuf, buf.GetLength() + 1, 0))
		{
			m_bIsConnected[k] = false;
			CheckConnection();
			UpdateList();
			continue;
		}
		if(m_arrOptions[DATABASE_CONNECTED])
		{
			char sql[200];
			sprintf_s(sql, "insert into ENVDATA values (systimestamp, '%s')", sendBuf);
			Statement *stmt = m_conn->createStatement();
			try{
				stmt->executeUpdate(sql);
			}catch(SQLException e)
			{
			//	stmt->executeUpdate();
				MessageBox(e.what());
				exit(0);
			}
			m_conn->commit();
			m_conn->terminateStatement(stmt);
		}
	}
	SendMessage(WM_PROGRESS);
	m_time++; 
	InvalidateRect(&m_canvas);
	CDialogEx::OnTimer(nIDEvent);
} 


BOOL CEnvConsoleDlg::OnEraseBkgnd(CDC* /*pDC*/)
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
	m_btnStart.EnableWindow(FALSE);
	InvalidateRect(&m_canvas);
}


void CEnvConsoleDlg::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
	CAddDialog AddDlg;
	if(IDOK == AddDlg.DoModal())
	{
		int type = AddDlg.m_type, port;
		double x = AddDlg.m_x;
		double y = AddDlg.m_y;
		double z = AddDlg.m_z;
		double vx, vy, vz;
		DWORD ip;
		CString strIP;
		switch(type)
		{
		case TYPE_STATIC:
			m_iStaticIdx.push_back(m_components.size());
			m_components.push_back(TComponent(m_components.size() + 1, TYPE_STATIC, TLocation(x, y), 0, CString("--"), 0));
			break;

		case TYPE_RADAR:
			port = AddDlg.m_port;
			ip = AddDlg.m_ip;
			strIP.Format(_T("%d.%d.%d.%d"), (ip>>24)&0xff, (ip>>16)&0xff, (ip>>8)&0xff, ip&0xff ) ;  
			m_iRadarIdx.push_back(m_components.size());
			m_components.push_back(TComponent(m_components.size() + 1, TYPE_RADAR, TLocation(x, y), 0, strIP, port));
			break;

		case TYPE_TARGET:
			vx = AddDlg.m_vx / 1000.0;
			vy = AddDlg.m_vy / 1000.0;
			vz = AddDlg.m_vz / 1000.0;
			m_iTargetIdx.push_back(m_components.size());
			m_components.push_back(TComponent(m_components.size() + 1, TYPE_TARGET, TLocation(x, y), 0, CString("--"), 0, TSpeed(vx, vy, 0)));
			break;

		default:;
		}
		UpdateList();
		InvalidateRect(&m_canvas);
	}

	if(m_components.size() == 0)
		((CButton *)GetDlgItem(IDC_EXPORT))->EnableWindow(FALSE);
	else 
		((CButton *)GetDlgItem(IDC_EXPORT))->EnableWindow(TRUE);
}


void CEnvConsoleDlg::OnNMCustomdrawMapScale(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateData(TRUE);
	CString str;
	str.Format("%d", m_iMapScale);
	GetDlgItem(IDC_MAP_SCALE_T)->SetWindowTextA(str);
	UpdateData(FALSE);
	InvalidateRect(&m_canvas);
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


double CEnvConsoleDlg::Distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}



void CEnvConsoleDlg::CheckConnection(void)
{
	m_bIsAllConnected = true;
	for(int i = 0;i < (int)m_bIsConnected.size();i++)
	{
		m_bIsAllConnected &= m_bIsConnected[i];
		m_components[m_iRadarIdx[i]].state = m_bIsConnected[i] ? STATE_CONNECTED:STATE_DISCONNECTED;
	}
}


void CEnvConsoleDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if(IDYES == MessageBox("Are you sure to quit?", "Question",MB_YESNO|MB_ICONQUESTION))
	{
		CDialogEx::OnCancel();
	}
	else {
		return ;
	}
}

void CEnvConsoleDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int i;
	for(i= 0; i < m_list.GetItemCount(); i++)
	{
		if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
			break;
	}

	if(i !=  m_list.GetItemCount())
	{
		m_iSelected = i;
		GetDlgItem(IDC_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		m_iSelected = -1;
		GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
	}
	
	InvalidateRect(&m_canvas);
}


void CEnvConsoleDlg::OnBnClickedDelete()
{
	// TODO: Add your control notification handler code here
/*
	int i;
	for(i = 0; i < m_list.GetItemCount(); i++)
	{
		if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
			break;
	}
*/
	std::vector<TComponent>::iterator it = m_components.begin() + m_iSelected;
	m_components.erase(it);
	UpdateList();
	InvalidateRect(&m_canvas);
	GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);

	m_iSelected = -1;
	if(m_components.size() == 0)
		((CButton *)GetDlgItem(IDC_EXPORT))->EnableWindow(FALSE);
	else 
		((CButton *)GetDlgItem(IDC_EXPORT))->EnableWindow(TRUE);
}

void CEnvConsoleDlg::OnBnClickedShowStaticObjs()
{
	// TODO: Add your control notification handler code here
	CButton *pCheck = (CButton *)GetDlgItem(IDC_SHOW_STATIC_OBJS);
	if(pCheck->GetCheck())
		m_arrOptions[HIDE_STATIC_OBJ] = 0;
	else
		m_arrOptions[HIDE_STATIC_OBJ] = 1;

	UpdateList();
}


void CEnvConsoleDlg::OnBnClickedImport()
{
	// TODO: Add your control notification handler code here
	CString strFile = _T("");
	CFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("XML Files (*.xml)|*.xml"), theApp.GetMainWnd()->GetWindow(GW_HWNDPREV), NULL);

	if(IDCANCEL == dlgFile.DoModal())
	{
		SetFocus();
		return ;
	}
	SetFocus();
	strFile = dlgFile.GetPathName();

	if(IDNO == MessageBox("Are you sure to import file '" + strFile +"'?", "Import File", MB_YESNO|MB_ICONINFORMATION))
		return ;

	ImportFromFile(strFile);
	UpdateList();
	InvalidateRect(&m_canvas);

	if(m_components.size() == 0)
		((CButton *)GetDlgItem(IDC_EXPORT))->EnableWindow(FALSE);
	else 
		((CButton *)GetDlgItem(IDC_EXPORT))->EnableWindow(TRUE);
}


HBRUSH CEnvConsoleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SelectObject(&m_font);
	return hbr;
}


void CEnvConsoleDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_arrOptions[ARROW_ON])
		return;
	CPoint tl = m_canvas.TopLeft();
	CPoint br = m_canvas.BottomRight();
	if(point.x > tl.x && point.y > tl.y && point.x < br.x && point.y < br.y)
	{
		CMenu menu;
		VERIFY(menu.LoadMenuA(IDR_MENU_CANVAS));
		CMenu *popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		CWnd *pWndPopupOwner = this;
	
		while(pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		popup->CheckMenuItem(0, MF_BYPOSITION | (m_arrOptions[TARGET_LOCATION] == 1 ?MF_CHECKED :MF_UNCHECKED));
		popup->CheckMenuItem(1, MF_BYPOSITION | (m_arrOptions[RADAR_LOCATION] == 1 ?MF_CHECKED :MF_UNCHECKED));
		popup->CheckMenuItem(2, MF_BYPOSITION | (m_arrOptions[STATIC_LOCATION] == 1 ?MF_CHECKED :MF_UNCHECKED));

		ClientToScreen(&point);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);

	}
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CEnvConsoleDlg::OnTargetlocation()
{
	// TODO: Add your command handler code here
	if(m_arrOptions[TARGET_LOCATION] == 1)
		m_arrOptions[TARGET_LOCATION] = 0;
	else 
		m_arrOptions[TARGET_LOCATION] = 1;
	InvalidateRect(&m_canvas);
}


void CEnvConsoleDlg::OnRadarLocation()
{
	// TODO: Add your command handler code here
	if(m_arrOptions[RADAR_LOCATION] == 1)
		m_arrOptions[RADAR_LOCATION] = 0;
	else 
		m_arrOptions[RADAR_LOCATION] = 1;
	InvalidateRect(&m_canvas);
}


void CEnvConsoleDlg::OnStaticLocation()
{
	// TODO: Add your command handler code here
	if(m_arrOptions[STATIC_LOCATION] == 1)
		m_arrOptions[STATIC_LOCATION] = 0;
	else 
		m_arrOptions[STATIC_LOCATION] = 1;
	InvalidateRect(&m_canvas);
}


void CEnvConsoleDlg::OnBnClickedExport()
{
	// TODO: Add your control notification handler code here
	CString strFile = _T("");
//	CFileDialog  dlgFile(FALSE);
	
	CFileDialog dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, theApp.GetMainWnd()->GetWindow(GW_HWNDPREV), NULL);
	dlgFile.m_ofn.lpstrTitle = _T("Export");
	dlgFile.m_ofn.lpstrFilter = _T("XML Files (*.xml)");
	dlgFile.m_ofn.lpstrDefExt = _T("xml");

	if(IDOK == dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();

		::CoInitialize(NULL);
		MSXML2::IXMLDOMDocumentPtr XMLDOC;
		MSXML2::IXMLDOMElementPtr XMLROOT;

		HRESULT HR = XMLDOC.CreateInstance(_uuidof(MSXML2::DOMDocument30));
		if(!SUCCEEDED(HR))
		{
			MessageBox(_T("Faild!"));
			return ;
		}
		XMLROOT = XMLDOC->createElement("Env_components");
		XMLDOC->appendChild(XMLROOT);
		CString strTmp;
		MSXML2::IXMLDOMElementPtr XMLNODE, nodeIP, nodePort, nodeLoc, nodeType, nodeX, nodeY, nodeZ, nodeSpeed, nodeVx, nodeVy, nodeVz;
		for(int i = 0;i < (int)m_components.size();i++)
		{

			XMLNODE = XMLDOC->createElement((_bstr_t)("components"));

			// No.
			strTmp.Format("%d", m_components[i].no);
			XMLNODE->setAttribute("no", (_variant_t)strTmp);

			// type 
			strTmp.Format("%d", m_components[i].type);
			XMLNODE->setAttribute("type", (_variant_t)strTmp);

			// type
			nodeType = XMLDOC->createElement((_bstr_t)("type"));
			nodeType->put_text((_bstr_t)m_strType[m_components[i].type]);

			// location
			nodeLoc = XMLDOC->createElement((_bstr_t)("location"));

			nodeX = XMLDOC->createElement((_bstr_t)("x"));
			strTmp.Format("%f", m_components[i].loc.x);
			nodeX->put_text((_bstr_t)strTmp);
			nodeLoc->appendChild(nodeX);

			nodeY = XMLDOC->createElement((_bstr_t)("y"));
			strTmp.Format("%f", m_components[i].loc.y);
			nodeY->put_text((_bstr_t)strTmp);
			nodeLoc->appendChild(nodeY);

			nodeZ = XMLDOC->createElement((_bstr_t)("z"));
			strTmp.Format("%f", m_components[i].loc.z);
			nodeZ->put_text((_bstr_t)strTmp);
			nodeLoc->appendChild(nodeZ);

			// ip address and port info.
			if(m_components[i].type == TYPE_RADAR)
			{
				nodeIP = XMLDOC->createElement((_bstr_t)("ip_address"));
				nodePort = XMLDOC->createElement((_bstr_t)("port"));
				nodeIP->put_text((_bstr_t)m_components[i].ip);
				strTmp.Format("%d", m_components[i].host);
				nodePort->put_text((_bstr_t)strTmp);

				XMLNODE->appendChild(nodeIP);
				XMLNODE->appendChild(nodePort);
			}

			if(m_components[i].type == TYPE_TARGET)
			{
				nodeSpeed = XMLDOC->createElement((_bstr_t)("speed"));

				nodeVx = XMLDOC->createElement((_bstr_t)("vx"));
				strTmp.Format("%f", m_components[i].speed.vx);
				nodeVx->put_text((_bstr_t)strTmp);
				nodeSpeed->appendChild(nodeVx);

				nodeVy = XMLDOC->createElement((_bstr_t)("vy"));
				strTmp.Format("%f", m_components[i].speed.vy);
				nodeVy->put_text((_bstr_t)strTmp);
				nodeSpeed->appendChild(nodeVy);

				nodeVz = XMLDOC->createElement((_bstr_t)("vz"));
				strTmp.Format("%f", m_components[i].speed.vz);
				nodeVz->put_text((_bstr_t)strTmp);
				nodeSpeed->appendChild(nodeVz);

				XMLNODE->appendChild(nodeSpeed);
			}

			XMLNODE->appendChild(nodeType);
			XMLNODE->appendChild(nodeLoc);
			XMLROOT->appendChild(XMLNODE);
		}
		CComVariant FileName(strFile);
		XMLDOC->save(FileName);
		XMLNODE.Release();
		XMLROOT.Release();
		nodeIP.Release();
		nodePort.Release();
		nodeType.Release();
		nodeLoc.Release();
		nodeX.Release();
		nodeY.Release();
		nodeZ.Release();
		nodeSpeed.Release();
		nodeVx.Release();
		nodeVy.Release();
		nodeVz.Release();
		XMLDOC.Release();
		::CoUninitialize();
	}
	SetFocus();
}


void CEnvConsoleDlg::ImportFromFile(CString strName)
{
	::CoInitialize(NULL);
	MSXML2::IXMLDOMDocumentPtr xmlDoc; 
	MSXML2::IXMLDOMElementPtr nodeRoot;
	MSXML2::IXMLDOMNodeListPtr nodes;
	MSXML2::IXMLDOMNodePtr node, subnode;
	HRESULT HR = xmlDoc.CreateInstance(_uuidof(MSXML2::DOMDocument30));
	if(!SUCCEEDED(HR))
	{
		MessageBox(_T("faild!!"));
		return;
	}

	CComVariant FileName(strName);
	xmlDoc->load(FileName);
	nodeRoot = xmlDoc->GetdocumentElement();
	nodeRoot->get_childNodes(&nodes);
	long num;
	nodes->get_length(&num);
	CString strTmp;
	strTmp.Format("%d", num);

	CComVariant varVal;
	CComBSTR strVal;
	CString strTrans;
	int port;
	CString ip;
	TLocation loc;
	TSpeed speed;
	m_components.clear();
	int base = m_components.size();
	for(int i = 0; i < num;i++)
	{
		nodes->get_item(i, &node);

		// get Type field.
		subnode = node->selectSingleNode(OLESTR("./@type"));
		subnode->get_nodeValue(&varVal);
		strTrans = COLE2CT(varVal.bstrVal);
		int type = _ttoi(strTrans);

		// get No. field
		subnode = node->selectSingleNode(OLESTR("./@no"));
		subnode->get_nodeValue(&varVal);
		strTrans = COLE2CT(varVal.bstrVal);
		int no = _ttoi(strTrans);

		// get Location field
		subnode = node->selectSingleNode(OLESTR("./location/x"));
		subnode->get_text(&strVal);
		strTrans = COLE2CT(strVal);
		loc.x = _ttof(strTrans);

		subnode = node->selectSingleNode(OLESTR("./location/y"));
		subnode->get_text(&strVal);
		strTrans = COLE2CT(strVal);
		loc.y = _ttof(strTrans);

		subnode = node->selectSingleNode(OLESTR("./location/z"));
		subnode->get_text(&strVal);
		strTrans = COLE2CT(strVal);
		loc.z = _ttof(strTrans);

		// get other filed
		switch(type)
		{
		case TYPE_STATIC:
			m_components.push_back(TComponent(no + base, type, loc, 0, _T("--"), 0));
			break;

		case TYPE_RADAR:
			// get Port field
			subnode = node->selectSingleNode(OLESTR("./port"));
			subnode->get_text(&strVal);
			strTrans = COLE2CT(strVal);
			port = _ttoi(strTrans);

			// get ip address filed
			subnode = node->selectSingleNode(OLESTR("./ip_address"));
			subnode->get_text(&strVal);
			strTrans = COLE2CT(strVal);
			ip = strTrans;

			m_components.push_back(TComponent(no + base, type, loc, 0, ip, port));
			break;

		case TYPE_TARGET:
			// get Speed field
			subnode = node->selectSingleNode(OLESTR("./speed/vx"));
			subnode->get_text(&strVal);
			strTrans = COLE2CT(strVal);
			speed.vx = _ttof(strTrans);

			subnode = node->selectSingleNode(OLESTR("./speed/vy"));
			subnode->get_text(&strVal);
			strTrans = COLE2CT(strVal);
			speed.vy = _ttof(strTrans);

			subnode = node->selectSingleNode(OLESTR("./speed/vz"));
			subnode->get_text(&strVal);
			strTrans = COLE2CT(strVal);
			speed.vz = _ttof(strTrans);

			m_components.push_back(TComponent(no + base, type, loc, 0, _T("--"), 0, speed));
			break;

		default:;
		}
	}
	nodes.Release();
	node.Release();
	subnode.Release();
	nodeRoot.Release();
	xmlDoc.Release();
	::CoUninitialize();
}


void CEnvConsoleDlg::OnDatabaseConnect()
{
	// TODO: Add your command handler code here
	try
	{
		m_env = Environment::createEnvironment(Environment::THREADED_MUTEXED);
	}catch(SQLException &e)
	{
		MessageBox(e.what());
	}

	std::string name = "";
	std::string pass = "";
	std::string srvName = "10.106.3.128:1521/ORCL";

	try
	{
		m_conn = m_env->createConnection(name, pass, srvName);
		MessageBox(_T("Dadatabse connected!"));
		m_arrOptions[DATABASE_CONNECTED] = 1;
		m_StatusBar.GetStatusBarCtrl().SetIcon(3, AfxGetApp()->LoadIconA(IDI_CONNECT));

		m_wndToolBar.SetButtonStyle(2, TBBS_DISABLED);
		m_wndToolBar.SetButtonStyle(3, TBBS_BUTTON);
	}
	catch(SQLException &e)
	{
		CString str;
		str.Format("%s, Failed to connect database",e.what());
		MessageBox(str);
		return ;
	}

	Statement *stmt = m_conn->createStatement();
	try
	{
		stmt->executeUpdate("create table CONF \
							(ClutterType VARCHAR2(10), \
							Coefficient float(126), \
							Disturbance float(126), \
							Interference float(126), \
							ClutterRCS float(126) )"
							);
	}catch(SQLException &e)
	{
		int r = stmt->executeUpdate("delete from CONF");
		CString str;
		str.Format("%s %d rows deleted.",e.what(), r);
		MessageBox(str);
	}

	try
	{
		stmt->executeUpdate("create table EnvData \
							( Time TIMESTAMP(6), \
							Frame VARCHAR2(1000))\
							STORAGE  ( INITIAL 100M NEXT 50M \
										MINEXTENTS 1 MAXEXTENTS 50 PCTINCREASE 5)"
							);
	}catch(SQLException &e)
	{
		MessageBox(e.what());
	}

	stmt->executeUpdate("insert into CONF values ('land', 0.67, 1.90, 6.43, 1.84)");
	m_conn->terminateStatement(stmt);
}


void CEnvConsoleDlg::OnDatabaseDisconnect()
{
	// TODO: Add your command handler code here
	if(IDOK == MessageBox("Are you sure to disconnect from database?", "Disconnect", MB_OKCANCEL | MB_ICONQUESTION))
	{
		m_env->terminateConnection(m_conn);
		Environment::terminateEnvironment(m_env);
		m_env = NULL;
		m_conn = NULL;
		m_arrOptions[DATABASE_CONNECTED] = 0;
		m_StatusBar.GetStatusBarCtrl().SetIcon(3, AfxGetApp()->LoadIconA(IDI_DISCONNECT));
		m_wndToolBar.SetButtonStyle(2, TBBS_BUTTON);
		m_wndToolBar.SetButtonStyle(3, TBBS_DISABLED);
	}
}


void CEnvConsoleDlg::OnUpdateDatabaseDisconnect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_arrOptions[DATABASE_CONNECTED])
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}


void CEnvConsoleDlg::OnUpdateDatabaseConnect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_arrOptions[DATABASE_CONNECTED])
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEnvConsoleDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT /*nIndex*/, BOOL /*bSysMenu*/)
{
	ENSURE_VALID(pPopupMenu);

	// check the enabled state of various menu items
	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // parent == child for tracking popup
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = GetTopLevelParent();
		// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = pParent->GetMenu()->GetSafeHmenu()) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nItemIndex = 0; nItemIndex < nIndexMax; nItemIndex++)
			{
				if (::GetSubMenu(hParentMenu, nItemIndex) == pPopupMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // pop-ups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// adjust for menu deletions and additions
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CEnvConsoleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_arrOptions[ARROW_ON])
		return ;

	CPoint tl = m_canvas.TopLeft();
	CPoint br = m_canvas.BottomRight();
	if(point.x > tl.x && point.y > tl.y && point.x < br.x && point.y < br.y)
	{
		if(!m_arrOptions[HIDE_CURSOR])
		{
			m_arrOptions[HIDE_CURSOR] = 1;
			ShowCursor(FALSE);
		}
		m_mouseLoc = point;
		InvalidateRect(&m_canvas);
		CString str;
		str.Format("  Location (x = %d, y = %d)  ", point.x, point.y);
		m_StatusBar.SetPaneText(0, str);
	}
	else
	{
		if(m_arrOptions[HIDE_CURSOR])
		{
			m_arrOptions[HIDE_CURSOR] = 0;
			ShowCursor(TRUE);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CEnvConsoleDlg::OnRuler()
{
	// TODO: Add your command handler code here
	if(m_arrOptions[ARROW_ON])
	{
		m_arrOptions[ARROW_ON] = 0;
		m_wndToolBar.SetButtonStyle(1, TBBS_BUTTON);
	}
	else
	{
		m_arrOptions[ARROW_ON] = 1;
		m_wndToolBar.SetButtonStyle(1, TBBS_CHECKED);
	}
	InvalidateRect(&m_canvas);
}


void CEnvConsoleDlg::OnUpdateRuler(CCmdUI *pCmdUI)
{
	if(m_arrOptions[ARROW_ON])
	{	
		pCmdUI->SetCheck(1);
		
	}
	else{
		pCmdUI->SetCheck(0);
	}
}


void CEnvConsoleDlg::OnUpdateTargetLocation(CCmdUI *pCmdUI)
{
	if(m_arrOptions[TARGET_LOCATION] == 1)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CEnvConsoleDlg::OnUpdateRadarLocation(CCmdUI *pCmdUI)
{
	if(m_arrOptions[RADAR_LOCATION] == 1)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CEnvConsoleDlg::OnUpdateStaticLocation(CCmdUI *pCmdUI)
{
	if(m_arrOptions[STATIC_LOCATION] == 1)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CEnvConsoleDlg::OnDatabaseConfiguration()
{
	DBOptions DBdlg;
	DBdlg.DoModal();
}


afx_msg LRESULT CEnvConsoleDlg::OnProgress(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_progress.StepIt();
	return 0;
}


void CEnvConsoleDlg::OnFileImport()
{
	SendMessage(WM_COMMAND, MAKEWPARAM(IDC_IMPORT, BN_CLICKED), NULL);
}


void CEnvConsoleDlg::OnFileExport()
{
	SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EXPORT, BN_CLICKED), NULL);
}


void CEnvConsoleDlg::OnUpdateFileExport(CCmdUI *pCmdUI)
{
	if(m_components.size() == 0)
		pCmdUI->Enable(FALSE);
	else 
		pCmdUI->Enable(TRUE);

}


void CEnvConsoleDlg::OnFileClearall()
{
	m_components.clear();
	UpdateList();
	InvalidateRect(&m_canvas);
}


void CEnvConsoleDlg::OnDatabaseUpload()
{
	Statement *stmt = m_conn->createStatement();
	try
	{
		stmt->executeUpdate("create table RADAR_LIST \
							(ID number(10), \
							location_x float(126), \
							location_y float(126), \
							location_z float(126), \
							ip	varchar2(100), \
							port number(10))");
	}
	catch (SQLException &e)
	{
		int r = stmt->executeUpdate("delete from RADAR_LIST");
		CString str;
		str.Format("%s %d rows deleted.",e.what(), r);
		MessageBox(str);
	}

	for(std::size_t i = 0;i < m_iRadarIdx.size();i++)
	{
		int idx = m_iRadarIdx[i];
		CString sql;
		sql.Format("insert into RADAR_LIST values(%d, %f, %f, %f, '%s', %d)",
					m_components[idx].no,
					m_components[idx].loc.x,
					m_components[idx].loc.y,
					m_components[idx].loc.z,
					m_components[idx].ip.GetBuffer(),
					m_components[idx].host);
		try
		{
			stmt->executeUpdate(sql.GetBuffer());
		}catch(SQLException &e)
		{
			MessageBox(e.what());
		}
	}
	m_conn->commit();
	m_conn->terminateStatement(stmt);
	
}


void CEnvConsoleDlg::OnUpdateDatabaseUpload(CCmdUI *pCmdUI)
{
	if(!m_arrOptions[DATABASE_CONNECTED])
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEnvConsoleDlg::OnDatabaseDownload()
{
	CString query = "select * from RADAR_LIST where ID = ";
	Statement *stmt = m_conn->createStatement();
	for(std::size_t i = 0;i < m_iRadarIdx.size();i++)
	{
		int idx = m_iRadarIdx[i];
		query.Format("select * from RADAR_LIST where ID = %d", m_components[idx].no);
		try
		{
			ResultSet *rs = stmt->executeQuery(query.GetBuffer());
			while(rs->next())
			{
				m_components[idx].loc.x = rs->getFloat(2);
				m_components[idx].loc.y = rs->getFloat(3);
				m_components[idx].loc.z = rs->getFloat(4);
				std::string ip = rs->getString(5);
				m_components[idx].ip.Format("%s", ip.c_str());
				m_components[idx].host = rs->getInt(6);
			}
		}
		catch (SQLException &e)
		{
			MessageBox(e.what());
		}
	}
	MessageBox(_T("Download success!"));
	UpdateList();
	InvalidateRect(&m_canvas);
}


void CEnvConsoleDlg::OnUpdateDatabaseDownload(CCmdUI *pCmdUI)
{
	if(!m_arrOptions[DATABASE_CONNECTED])
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEnvConsoleDlg::OnDrawGrids()
{
	// TODO: Add your command handler code here
	if(m_arrOptions[DRAW_GRIDS] == 0)
	{
		m_arrOptions[DRAW_GRIDS] = 1;
		m_wndToolBar.SetButtonStyle(4, TBBS_CHECKED);
	}
	else{
		m_arrOptions[DRAW_GRIDS] = 0;
		m_wndToolBar.SetButtonStyle(4, TBBS_BUTTON);
	}
	InvalidateRect(&m_canvas);
}
