// ConferPlayPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "ConferPlayPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConferPlayPage property page

IMPLEMENT_DYNCREATE(CConferPlayPage, CPropertyPage)

CConferPlayPage::CConferPlayPage() : CPropertyPage(CConferPlayPage::IDD)
{
	//{{AFX_DATA_INIT(CConferPlayPage)
	m_strConferNO = _T("");
	m_strFileExpress = _T("");
	//}}AFX_DATA_INIT
}

CConferPlayPage::~CConferPlayPage()
{
}

void CConferPlayPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConferPlayPage)
	DDX_Text(pDX, IDC_EDIT_CONF_NO, m_strConferNO);
	DDX_Text(pDX, IDC_EDIT_FILEEXPRESS, m_strFileExpress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConferPlayPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConferPlayPage)
	ON_BN_CLICKED(IDC_BUTTON_FILEEXPRESS, OnButtonFileexpress)
	ON_BN_CLICKED(IDC_BUTTON_CONFERNO, OnButtonConferno)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConferPlayPage message handlers

void CConferPlayPage::OnButtonFileexpress() 
{
	// TODO: Add your control notification handler code here
	
}

void CConferPlayPage::OnButtonConferno() 
{
	// TODO: Add your control notification handler code here
	
}
