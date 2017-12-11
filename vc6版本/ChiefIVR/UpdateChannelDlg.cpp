// UpdateChannelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivr.h"
#include "UpdateChannelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpdateChannelDlg dialog


CUpdateChannelDlg::CUpdateChannelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateChannelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpdateChannelDlg)
	m_bUpdateCalledID = FALSE;
	m_bUpdateScriptFile = FALSE;
	m_bUpdateType = FALSE;
	m_nChnlType = -1;
	m_strCalledID = _T("");
	m_strScriptFile = _T("");
	//}}AFX_DATA_INIT
}


void CUpdateChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpdateChannelDlg)
	DDX_Check(pDX, IDC_CHECK_UPDATE_CALLEDID, m_bUpdateCalledID);
	DDX_Check(pDX, IDC_CHECK_UPDATE_SCRIPTFILE, m_bUpdateScriptFile);
	DDX_Check(pDX, IDC_CHECK_UPDATE_TYPE, m_bUpdateType);
	DDX_CBIndex(pDX, IDC_COMBO_CHNLTYPE, m_nChnlType);
	DDX_Text(pDX, IDC_EDIT_CALLEDID, m_strCalledID);
	DDV_MaxChars(pDX, m_strCalledID, 31);
	DDX_Text(pDX, IDC_EDIT_SCRIPTFILE, m_strScriptFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpdateChannelDlg, CDialog)
	//{{AFX_MSG_MAP(CUpdateChannelDlg)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_TYPE, OnCheckUpdateType)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_CALLEDID, OnCheckUpdateCalledid)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_SCRIPTFILE, OnCheckUpdateScriptfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdateChannelDlg message handlers

void CUpdateChannelDlg::OnButtonFile() 
{
	UpdateData(TRUE);
	char szFilters[]="Chief Script Files (*.cef)|*.cef||";
		
	CFileDialog Dlg(FALSE,"cef","*.cef",OFN_FILEMUSTEXIST, szFilters, this);
	if(Dlg.DoModal()==IDOK)
	{
		m_strScriptFile = Dlg.GetPathName();
		UpdateData(FALSE);


	}

	return;
	
}

void CUpdateChannelDlg::OnCheckUpdateType() 
{
	// TODO: Add your control notification handler code here
	
}

void CUpdateChannelDlg::OnCheckUpdateCalledid() 
{
	// TODO: Add your control notification handler code here
	
}

void CUpdateChannelDlg::OnCheckUpdateScriptfile() 
{
	// TODO: Add your control notification handler code here
	
}
