// AddDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EnvConsole.h"
#include "AddDialog.h"
#include "afxdialogex.h"


// CAddDialog dialog
 unsigned int seed = 1;
int genRand(int min, int max)
{
	seed += (max / 2 + min);
	seed = seed % UINT_MAX;
	seed += (unsigned int)time(NULL);
	srand(seed);
	return rand() % (max + 1 - min) + min;
}

IMPLEMENT_DYNAMIC(CAddDialog, CDialogEx)

CAddDialog::CAddDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddDialog::IDD, pParent)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_vx(0)
	, m_vy(0)
	, m_vz(0)
	, m_port(0)
	, m_ip(0)
	, m_type(0)
	, m_strIP(_T("127.0.0.1"))
{

}

CAddDialog::~CAddDialog()
{
}

void CAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TYPE, m_comboType);
	DDX_Text(pDX, IDC_X, m_x);
	DDV_MinMaxDouble(pDX, m_x, 50, 450);
	DDX_Text(pDX, IDC_Y, m_y);
	DDV_MinMaxDouble(pDX, m_y, 50, 450);
	DDX_Text(pDX, IDC_Z, m_z);
	DDV_MinMaxDouble(pDX, m_z, 0, 5000);
	DDX_Text(pDX, IDC_VX, m_vx);
	DDV_MinMaxDouble(pDX, m_vx, 0, 1200);
	DDX_Text(pDX, IDC_VY, m_vy);
	DDV_MinMaxDouble(pDX, m_vy, 0, 1200);
	DDX_Text(pDX, IDC_VZ, m_vz);
	DDV_MinMaxDouble(pDX, m_vz, 0, 1200);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDV_MinMaxInt(pDX, m_port, 0, 99999);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_ip);
}


BEGIN_MESSAGE_MAP(CAddDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_TYPE, &CAddDialog::OnCbnSelchangeType)
	ON_BN_CLICKED(IDOK, &CAddDialog::OnBnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAddDialog message handlers


BOOL CAddDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_font.CreatePointFont(80, "Verdana", NULL);
	GetDlgItem(IDC_STATIC)->SetFont(&m_font);
	GetDlgItem(IDOK)->SetFont(&m_font);
	GetDlgItem(IDCANCEL)->SetFont(&m_font);

	// TODO:  Add extra initialization here
	m_comboType.AddString("Static");
	m_comboType.AddString("Radar");
	m_comboType.AddString("Target");

	m_comboType.SelectString(0, "Static");
	GetDlgItem(IDC_VX)->EnableWindow(FALSE);
	GetDlgItem(IDC_VY)->EnableWindow(FALSE);
	GetDlgItem(IDC_VZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_PORT)->EnableWindow(FALSE);

	m_x = genRand(50, 450);
	m_y = genRand(50, 450);
	m_z = genRand(0, 5000);

	m_vx = genRand(0, 1200);
	m_vy = genRand(0, 1200);
	m_vz = genRand(0, 1200);

/*
	hostent *host;
	char hostName[100];
	gethostname(hostName, 100);
	host = gethostbyname(hostName); 
	m_strIP = inet_ntoa(*(struct in_addr *)host->h_addr_list[0]);
*/
	// For Debug: set m_strIP to be 127.0.0.1
	m_ip = ntohl(inet_addr(m_strIP));
	m_port = 6000;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAddDialog::OnCbnSelchangeType()
{
	// TODO: Add your control notification handler code here
	int index = m_comboType.GetCurSel();
	switch(index)
	{
	case TYPE_TARGET:
		GetDlgItem(IDC_VX)->EnableWindow(TRUE);
		GetDlgItem(IDC_VY)->EnableWindow(TRUE);
		GetDlgItem(IDC_VZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
		break;
	case TYPE_RADAR:
		GetDlgItem(IDC_VX)->EnableWindow(FALSE);
		GetDlgItem(IDC_VY)->EnableWindow(FALSE);
		GetDlgItem(IDC_VZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		break;
	case TYPE_STATIC:
		GetDlgItem(IDC_VX)->EnableWindow(FALSE);
		GetDlgItem(IDC_VY)->EnableWindow(FALSE);
		GetDlgItem(IDC_VZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
		break;
	default:;
	}
}



void CAddDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_type = m_comboType.GetCurSel();
	CDialogEx::OnOK();
}


HBRUSH CAddDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SelectObject(&m_font);
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
