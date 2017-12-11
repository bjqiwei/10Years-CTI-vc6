// ReleaseCallPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "ReleaseCallPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReleaseCallPage property page

IMPLEMENT_DYNCREATE(CReleaseCallPage, CPropertyPage)

CReleaseCallPage::CReleaseCallPage() : CPropertyPage(CReleaseCallPage::IDD)
{
	//{{AFX_DATA_INIT(CReleaseCallPage)
	m_bSaveDetail = FALSE;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
}

CReleaseCallPage::~CReleaseCallPage()
{
}

void CReleaseCallPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReleaseCallPage)
	DDX_Check(pDX, IDC_CHECK_SAVE_DETAIL, m_bSaveDetail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReleaseCallPage, CPropertyPage)
	//{{AFX_MSG_MAP(CReleaseCallPage)
	ON_BN_CLICKED(IDC_CHECK_SAVE_DETAIL, OnCheckSaveDetail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReleaseCallPage message handlers

void CReleaseCallPage::OnOK() 
{
	if( NULL == m_pNodeObj ) return;


	UpdateData(TRUE);
	m_pNodeObj->m_bSaveDetail = m_bSaveDetail;
	
	CPropertyPage::OnOK();
}

BOOL CReleaseCallPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReleaseCallPage::OnCheckSaveDetail() 
{
	// TODO: Add your control notification handler code here
	
}

void CReleaseCallPage::InitPage(CReleaseCallObj *pNodeObj)
{
	m_pNodeObj = pNodeObj;
	if( NULL == m_pNodeObj ) return;

	m_bSaveDetail = m_pNodeObj->m_bSaveDetail;



	return;
}
