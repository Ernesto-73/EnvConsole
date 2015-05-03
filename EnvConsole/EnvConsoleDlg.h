
// EnvConsoleDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <vector>
#include "atltypes.h"
#include "afxwin.h"

enum{TYPE_TARGET = 0, TYPE_RADAR, TYPE_MOUNTAIN};
enum{STATE_DISCONNECTED = 0, STATE_CONNECTED};



typedef struct TLocation{
	TLocation()
	{
		// default constructor.
	}

	TLocation(int _x, int _y, int _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	TLocation(int _x, int _y)
	{
		x = _x;
		y = _y;
		z = 0;
	}
	int x;
	int y;
	int z;
}TLocation;

typedef struct Target{
	Target()
	{
		
	}
	void Move()
	{
		x += vx;
		y += vy;
	}
	Target(double _x, double _y, double _z, double _vx, double _vy, double _vz)
	{
		x = _x;
		y = _y;
		z = _z;
		vx = _vx;
		vy = _vy;
		vz = _vz;
	}
	double x;
	double y;
	double z;
	double vx;
	double vy;
	double vz;
}Target;

typedef struct TComponent{
	TComponent()
	{
		// default constructor.
	}
	TComponent(int _no, int _type, TLocation &_loc, int _state, CString &_ip, int _host)
	{
		no = _no;
		type = _type;
		loc.x = _loc.x;
		loc.y = _loc.y;
		loc.z = _loc.z;
		state = _state;
		ip = _ip;
		host = _host;
	}

	int no;
	int type;
	TLocation loc;
	int state;
	CString ip;
	int host;
}TComponent;

// CEnvConsoleDlg dialog
class CEnvConsoleDlg : public CDialogEx
{
// Construction
public:
	CEnvConsoleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ENVCONSOLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
private:
	std::vector<TComponent> m_components;
public:
	CString m_strType[3];
	CString m_strState[2];
	std::vector<int> m_iTargetIdx;
	std::vector<int> m_iStaticIdx;
	std::vector<int> m_iRadarIdx;
	afx_msg void OnBnClickedConnAll();
private:
	std::vector<SOCKET> m_socket;
	void UpdateList(void);
	CRect m_canvas;
public:
	void Draw(CDC * pDC);
	CStatic m_seperator;
	afx_msg void OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	std::vector<Target> m_targets;
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int m_time;
	afx_msg void OnBnClickedDetails();
	CRect m_large;
	CRect m_small;
	CButton m_btnDetails;
	CButton m_btnStart;
	CButton m_btnConnectAll;
	CButton m_btnDisconnect;
	bool m_bIsAllConnected;
	afx_msg void OnBnClickedDisconnect();
	bool m_bIsStarted;
	afx_msg void OnBnClickedAdd();
	int m_iMapScale;
	CSliderCtrl m_sliderMapScale;
	afx_msg void OnNMCustomdrawMapScale(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iScaleType;
	double m_ClutterRCS;
	double m_Coff;
	CComboBox m_comboClutterType;
	CComboBox m_comboTime;
	int m_EnvDis;
	CSliderCtrl m_SliderEnvDis;
	afx_msg void OnNMCustomdrawEnvDis(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SliderEleInterf;
	int m_EleInterf;
	afx_msg void OnNMCustomdrawEleInterf(NMHDR *pNMHDR, LRESULT *pResult);
};
