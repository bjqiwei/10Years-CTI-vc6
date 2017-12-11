// ConferRecordPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "ConferRecordPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConferRecordPage property page

IMPLEMENT_DYNCREATE(CConferRecordPage, CPropertyPage)

CConferRecordPage::CConferRecordPage() : CPropertyPage(CConferRecordPage::IDD)
{
	//{{AFX_DATA_INIT(CConferRecordPage)
	m_strConferNO = _T("");
	m_strFileExpress = _T("");
	m_nFileLength = 0;
	m_strFolder = _T("");
	//}}AFX_DATA_INIT
}

CConferRecordPage::~CConferRecordPage()
{
}

void CConferRecordPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConferRecordPage)
	DDX_Text(pDX, IDC_EDIT_CONF_NO3, m_strConferNO);
	DDX_Text(pDX, IDC_EDIT_FILE_EXPRESS, m_strFileExpress);
	DDX_Text(pDX, IDC_EDIT_FILELENGTH, m_nFileLength);
	DDV_MinMaxUInt(pDX, m_nFileLength, 1, 20);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConferRecordPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConferRecordPage)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_CONFERNO, OnButtonConferno)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, OnButtonFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConferRecordPage message handlers

void CConferRecordPage::OnButtonFile() 
{
	// TODO: Add your control notification handler code here
	
}

void CConferRecordPage::OnButtonConferno() 
{
	// TODO: Add your control notification handler code here
	
}

void CConferRecordPage::OnButtonFolder() 
{
	// TODO: Add your control notification handler code here
	
}
