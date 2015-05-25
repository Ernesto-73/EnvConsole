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
{

}

DBOptions::~DBOptions()
{
}

void DBOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DBOptions, CDialogEx)
END_MESSAGE_MAP()


// DBOptions message handlers
