#pragma once


// DBOptions dialog

class DBOptions : public CDialogEx
{
	DECLARE_DYNAMIC(DBOptions)

public:
	DBOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~DBOptions();

// Dialog Data
	enum { IDD = IDD_DB_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
