// CallLogRequestPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "CallLogRequestPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallLogRequestPageSpc property page

IMPLEMENT_DYNCREATE(CCallLogRequestPageSpc, CPropertyPage)

CCallLogRequestPageSpc::CCallLogRequestPageSpc() : CPropertyPage(CCallLogRequestPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CCallLogRequestPageSpc)
	m_nRequestType = 0;
	//}}AFX_DATA_INIT
}

CCallLogRequestPageSpc::~CCallLogRequestPageSpc()
{
}

void CCallLogRequestPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallLogRequestPageSpc)
	DDX_Radio(pDX, IDC_RADIO_REQUEST1, m_nRequestType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCallLogRequestPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CCallLogRequestPageSpc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallLogRequestPageSpc message handlers

void CCallLogRequestPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj ) return;


	UpdateData(TRUE);

	m_pNodeObj->m_nRequestType = m_nRequestType ;
	
	
	CPropertyPage::OnOK();
}

BOOL CCallLogRequestPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCallLogRequestPageSpc::InitPage(CCallLogRequestObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_nRequestType = m_pNodeObj->m_nRequestType;



	return;
}
