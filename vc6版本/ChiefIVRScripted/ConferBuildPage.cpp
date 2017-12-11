// ConferBuildPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "ConferBuildPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConferBuildPage property page

IMPLEMENT_DYNCREATE(CConferBuildPage, CPropertyPage)

CConferBuildPage::CConferBuildPage() : CPropertyPage(CConferBuildPage::IDD)
{
	//{{AFX_DATA_INIT(CConferBuildPage)
	m_nBuildType = 0;
	m_strConferNO = _T("");
	m_strMaxMember = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CConferBuildPage::~CConferBuildPage()
{
}

void CConferBuildPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConferBuildPage)
	DDX_Radio(pDX, IDC_RADIO_BUILDTYPE1, m_nBuildType);
	DDX_Text(pDX, IDC_EDIT_CONFERNO, m_strConferNO);
	DDX_Text(pDX, IDC_EDIT_MAXMEMBER, m_strMaxMember);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConferBuildPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConferBuildPage)
	ON_BN_CLICKED(IDC_BUTTON_VAR, OnButtonVar)
	ON_BN_CLICKED(IDC_BUTTON_MAXMEMBERVAR, OnButtonMaxmembervar)
	ON_BN_CLICKED(IDC_RADIO_BUILDTYPE1, OnRadioBuildtype1)
	ON_BN_CLICKED(IDC_RADIO_BUILDTYPE2, OnRadioBuildtype2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConferBuildPage message handlers

void CConferBuildPage::OnButtonVar() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strConferNO =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CConferBuildPage::InitPage(CConferenceBulidObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_nBuildType = m_pNodeObj->m_nBuildType;
	m_strConferNO = m_pNodeObj->m_strConferNo;
	m_strMaxMember= m_pNodeObj->m_strMaxMember;
	return;
}

void CConferBuildPage::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);
	m_pNodeObj->m_nBuildType = m_nBuildType;
	m_pNodeObj->m_strConferNo = m_strConferNO;
	m_pNodeObj->m_strMaxMember = m_strMaxMember;
	CPropertyPage::OnOK();
}

BOOL CConferBuildPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EnableMyWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConferBuildPage::OnButtonMaxmembervar() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strMaxMember =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CConferBuildPage::OnRadioBuildtype1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CConferBuildPage::OnRadioBuildtype2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CConferBuildPage::EnableMyWindow()
{
	if( 0 == m_nBuildType )
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_CONFERNO);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_BUTTON_VAR);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

	}
	else
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_CONFERNO);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_BUTTON_VAR);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
	}	
}
