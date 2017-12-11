// ConferDeletePage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "ConferDeletePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConferDeletePage property page

IMPLEMENT_DYNCREATE(CConferDeletePage, CPropertyPage)

CConferDeletePage::CConferDeletePage() : CPropertyPage(CConferDeletePage::IDD)
{
	//{{AFX_DATA_INIT(CConferDeletePage)
	m_nDeleteType = 0;
	m_strConferNO = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CConferDeletePage::~CConferDeletePage()
{
}

void CConferDeletePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConferDeletePage)
	DDX_Radio(pDX, IDC_RADIO_DELETETYPE1, m_nDeleteType);
	DDX_Text(pDX, IDC_EDIT_CONFERNO, m_strConferNO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConferDeletePage, CPropertyPage)
	//{{AFX_MSG_MAP(CConferDeletePage)
	ON_BN_CLICKED(IDC_BUTTON_VAR, OnButtonVar)
	ON_BN_CLICKED(IDC_RADIO_DELETETYPE1, OnRadioDeletetype1)
	ON_BN_CLICKED(IDC_RADIO_DELETETYPE2, OnRadioDeletetype2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConferDeletePage message handlers

void CConferDeletePage::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);
	m_pNodeObj->m_nDeleteType = m_nDeleteType;
	m_pNodeObj->m_strConferNo = m_strConferNO;

	CPropertyPage::OnOK();
}

BOOL CConferDeletePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EnableMyWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConferDeletePage::OnButtonVar() 
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

void CConferDeletePage::InitPage(CConferenceDeleteObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_nDeleteType = m_pNodeObj->m_nDeleteType;
	m_strConferNO = m_pNodeObj->m_strConferNo;

	return;
}

void CConferDeletePage::EnableMyWindow()
{
	if( 0 == m_nDeleteType )
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

void CConferDeletePage::OnRadioDeletetype1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CConferDeletePage::OnRadioDeletetype2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}
