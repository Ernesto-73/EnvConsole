#pragma once
#include "afxwin.h"

enum{TYPE_STATIC = 0, TYPE_RADAR, TYPE_TARGET};

// CAddDialog dialog

int genRand(int min, int max);

class CAddDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDialog)

public:
	CAddDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddDialog();

// Dialog Data
	enum { IDD = IDD_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboType;
	virtual BOOL OnInitDialog();
	double m_x;
	double m_y;
	double m_z;
	double m_vx;
	double m_vy;
	double m_vz;
	int m_port;
	DWORD m_ip;
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedOk();
	int m_type;
	CString m_strIP;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CFont m_font;
};
