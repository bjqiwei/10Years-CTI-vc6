// RouteCallProPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "RouteCallProPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRouteCallProPage property page

IMPLEMENT_DYNCREATE(CRouteCallProPage, CPropertyPage)

CRouteCallProPage::CRouteCallProPage() : CPropertyPage(CRouteCallProPage::IDD)
{
	//{{AFX_DATA_INIT(CRouteCallProPage)
	m_strTelNum = _T("");
	m_strWaitFile = _T("");
	m_nWaitOption = -1;
	m_strEndCh = _T("");
	m_strConnFile = _T("");
	m_strBeginCH = _T("");
	m_nConnOption = -1;
	//}}AFX_DATA_INIT
	m_pNodeObj	= NULL;
	m_pDoc		= NULL;
}

CRouteCallProPage::~CRouteCallProPage()
{
}

void CRouteCallProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRouteCallProPage)
	DDX_Text(pDX, IDC_EDIT_TELNUM, m_strTelNum);
	DDX_Text(pDX, IDC_EDIT_WAIT_PLAYFILE, m_strWaitFile);
	DDX_Radio(pDX, IDC_ROUTE_WAITOPTION1, m_nWaitOption);
	DDX_Text(pDX, IDC_EDIT_END_CH, m_strEndCh);
	DDX_Text(pDX, IDC_EDIT_CONN_PLAYFILE, m_strConnFile);
	DDX_Text(pDX, IDC_EDIT_BEGIN_CH, m_strBeginCH);
	DDX_Radio(pDX, IDC_ROUTE_CONNOPTION1, m_nConnOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRouteCallProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRouteCallProPage)
	ON_BN_CLICKED(IDC_BTN_TEL, OnBtnTel)
	ON_BN_CLICKED(IDC_BTN_BEGIN_CH, OnBtnBeginCh)
	ON_BN_CLICKED(IDC_BTN_END_CH, OnBtnEndCh)
	ON_BN_CLICKED(IDC_BTN_WAITFILE, OnBtnWaitfile)
	ON_BN_CLICKED(IDC_BTN_CONNFILE, OnBtnConnfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRouteCallProPage message handlers

void CRouteCallProPage::OnBtnTel() 
{
	// TODO: Add your control notification handler code here
	
}

void CRouteCallProPage::OnBtnBeginCh() 
{
	// TODO: Add your control notification handler code here
	
}

void CRouteCallProPage::OnBtnEndCh() 
{
	// TODO: Add your control notification handler code here
	
}

void CRouteCallProPage::OnBtnWaitfile() 
{
	// TODO: Add your control notification handler code here
	
}

void CRouteCallProPage::OnBtnConnfile() 
{
	// TODO: Add your control notification handler code here
	
}

void CRouteCallProPage::InitPage(CRouteCallObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;



	m_strBeginCH = m_pNodeObj->m_strBeginCh;
	m_strEndCh = m_pNodeObj->m_strEndCh;
	m_strTelNum = m_pNodeObj->m_strTel;
	m_nWaitOption = m_pNodeObj->m_nWaitOption;
	m_strWaitFile = m_pNodeObj->m_strWaitFile;
	m_nConnOption = m_pNodeObj->m_nConnOption;
	m_strConnFile = m_pNodeObj->m_strConnFile;


	return;
}

void CRouteCallProPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}

BOOL CRouteCallProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
		if( NULL == m_pNodeObj || NULL == m_pDoc ) return TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
