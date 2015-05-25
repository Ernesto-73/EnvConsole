// DBOptions.cpp : implementation file
//

#include "stdafx.h"
#include "EnvConsole.h"
#include "DBOptions.h"
#include "afxdialogex.h"


// DBOptions dialog

IMPLEMENT_DYNAMIC(DBOptions, CDialogEx)

DBOptions::DBOptions(CWnd* pParent /*=NULL*/)
	: CDialogEx(DBOptions::IDD, pParent)
	, m_iPort(1521)
	, m_strUserName(_T("javier"))
	, m_strPassword(_T("zh2348"))
	, m_strSID(_T("ORCL"))
	, m_boolSrvNameOrSID(1)
	, m_strConfigName(_T("_CONF_"))
	, m_strDataName(_T("_ENVDATA_"))
	, m_iSize(4)
	, m_bOverwrite(TRUE)
	, m_bShowProgress(TRUE)
	, m_ip(0)
{
	m_font.CreatePointFont(80, "Verdana", NULL);
	m_ip= ntohl( inet_addr("10.106.3.128"));
}

DBOptions::~DBOptions()
{
}

void DBOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT, m_iPort);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_SID, m_strSID);
	DDX_Text(pDX, IDC_CONFIG_NAME, m_strConfigName);
	DDX_Text(pDX, IDC_DATA_NAME, m_strDataName);
	DDX_Radio(pDX, IDC_RADIO_SRV_NAME, m_boolSrvNameOrSID);
	DDX_Text(pDX, IDC_TABLE_SIZE, m_iSize);
	DDV_MinMaxInt(pDX, m_iSize, 0, 10000);
	DDX_Check(pDX, IDC_CHECK_OVERWRITE, m_bOverwrite);
	DDX_Check(pDX, IDC_CHECK_SHOW_PROGRESS, m_bShowProgress);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_ip);
}


BEGIN_MESSAGE_MAP(DBOptions, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// DBOptions message handlers


HBRUSH DBOptions::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SelectObject(&m_font);
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


BOOL DBOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetDlgItem(IDC_RADIO_SRV_NAME)->SetFont(&m_font);
	GetDlgItem(IDC_CHECK_OVERWRITE)->SetFont(&m_font);
	GetDlgItem(IDC_CHECK_SHOW_PROGRESS)->SetFont(&m_font);
	GetDlgItem(IDC_CONNECTION)->SetFont(&m_font);
	GetDlgItem(IDC_TABLE)->SetFont(&m_font);
	GetDlgItem(IDC_IPADDRESS)->SetFont(&m_font);
	
	((CComboBox *)GetDlgItem(IDC_COMBO_AS))->AddString(_T("Default"));
	((CComboBox *)GetDlgItem(IDC_COMBO_AS))->AddString(_T("SYSDBA"));
	((CComboBox *)GetDlgItem(IDC_COMBO_AS))->AddString(_T("SYSOPER"));
	((CComboBox *)GetDlgItem(IDC_COMBO_AS))->SetCurSel(0);

	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("Verbose"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("Normal"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
