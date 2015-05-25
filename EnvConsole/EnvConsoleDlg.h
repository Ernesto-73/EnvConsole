
// EnvConsoleDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <vector>
#include "atltypes.h"
#include "afxwin.h"
#include "afxext.h"
#include "occi.h"

using namespace oracle::occi;;

enum{STATE_DISCONNECTED = 0, STATE_CONNECTED};
enum{HIDE_STATIC_OBJ = 0, TARGET_LOCATION, RADAR_LOCATION , STATIC_LOCATION, DATABASE_CONNECTED, ARROW_ON, HIDE_CURSOR, NUM};
enum{EVENT_SIM = 1, EVENT_CLOCK};

static UINT BASED_CODE indicators[]=   
{
	ID_SEPARATOR,
	ID_INDICATOR_PROGRESS,
	ID_INDICATOR_TIME,
	ID_INDICATOR_VERSION
};

typedef struct TLocation{
	TLocation()
	{
		// default constructor.
	}

	TLocation(double _x, double _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	TLocation(double _x, double _y)
	{
		x = _x;
		y = _y;
		z = 0;
	}
	double x;
	double y;
	double z;
}TLocation;

typedef struct TSpeed{
	TSpeed(){}
	TSpeed(double _vx, double _vy, double _vz)
	{
		vx = _vx;
		vy = _vy;
		vz = _vz;
	}

	double vx;
	double vy;
	double vz;
}TSpeed;

typedef struct TComponent{
	TComponent()
	{
		// default constructor.
	}

	TComponent(int _no, int _type, TLocation _loc, int _state, CString _ip, int _host)
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

	TComponent(int _no, int _type, TLocation _loc, int _state, CString _ip, int _host, TSpeed t)
	{
		no = _no;
		type = _type;
		loc.x = _loc.x;
		loc.y = _loc.y;
		loc.z = _loc.z;
		state = _state;
		ip = _ip;
		host = _host;

		speed.vx = t.vx;
		speed.vy = t.vy;
		speed.vz = t.vz;
	}
	
	void Move()
	{
		loc.x += speed.vx;
		loc.y += speed.vy;
	}

	int no;
	int type;
	TLocation loc;
	int state;
	CString ip;
	int host;
	TSpeed speed;
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

	std::vector<bool> m_bIsConnected; // Just for radar components.

	afx_msg void OnBnClickedConnAll();
private:
	std::vector<SOCKET> m_socket;
	void UpdateList(void);
	CRect m_canvas;
	int m_arrOptions[NUM];
public:
	void Draw(CDC * pDC);
	CStatic m_separator;
	afx_msg void OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	
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
	double Distance(double x1, double y1, double x2, double y2);
	void CheckConnection(void);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedShowStaticObjs();
	afx_msg void OnBnClickedImport();
	CFont m_font;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int m_iSelected;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTargetlocation();
	afx_msg void OnRadarLocation();
	afx_msg void OnStaticLocation();
	afx_msg void OnBnClickedExport();
private:
	void ImportFromFile(CString strName);
	CMenu m_MainMenu;
	CToolBar m_wndToolBar;
	CStatusBar  m_StatusBar;
public:
	afx_msg void OnDatabaseConnect();
private:
	Environment* m_env;
public:
	Connection* m_conn;
	afx_msg void OnDatabaseDisconnect();
	afx_msg void OnUpdateDatabaseDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDatabaseConnect(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	CPoint m_mouseLoc;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRuler();
	afx_msg void OnUpdateRuler(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTargetLocation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRadarLocation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStaticLocation(CCmdUI *pCmdUI);
	afx_msg void OnDatabaseConfiguration();
};
