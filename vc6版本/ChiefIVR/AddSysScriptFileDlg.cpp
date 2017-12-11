// AddSysScriptFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivr.h"
#include "AddSysScriptFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddSysScriptFileDlg dialog


CAddSysScriptFileDlg::CAddSysScriptFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddSysScriptFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddSysScriptFileDlg)
	m_strFile = _T("");
	m_strRemark = _T("");
	//}}AFX_DATA_INIT
}


void CAddSysScriptFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddSysScriptFileDlg)
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFile);
	DDX_Text(pDX, IDC_EDIT_REMARK, m_strRemark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddSysScriptFileDlg, CDialog)
	//{{AFX_MSG_MAP(CAddSysScriptFileDlg)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddSysScriptFileDlg message handlers

void CAddSysScriptFileDlg::OnButtonFile() 
{
	char szFilters[]="Chief Script Files (*.cef)|*.cef||";
		
	CFileDialog Dlg(FALSE,"cef","*.cef",OFN_FILEMUSTEXIST, szFilters, this);
	if(Dlg.DoModal()==IDOK)
	{
		m_strFile = Dlg.GetPathName();
		UpdateData(FALSE);


	}

	return;
	
}

void CAddSysScriptFileDlg::OnOK() 
{
	UpdateData(TRUE);
	if( "" == m_strFile )
	{
		MessageBox("Please input script file !");
		return;
		
	}


	CDialog::OnOK();
}
