// DTMFSendPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "DTMFSendPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDTMFSendPageSpc property page

IMPLEMENT_DYNCREATE(CDTMFSendPageSpc, CPropertyPage)

CDTMFSendPageSpc::CDTMFSendPageSpc() : CPropertyPage(CDTMFSendPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CDTMFSendPageSpc)
	m_strExpress = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CDTMFSendPageSpc::~CDTMFSendPageSpc()
{
}

void CDTMFSendPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDTMFSendPageSpc)
	DDX_Text(pDX, IDC_EDIT_EXPRESS, m_strExpress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDTMFSendPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CDTMFSendPageSpc)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDTMFSendPageSpc message handlers

void CDTMFSendPageSpc::OnButtonGet() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strExpress =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CDTMFSendPageSpc::InitPage(CDTMFSendObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;
	
	m_strExpress  = m_pNodeObj->m_strDTMF;

	
	return;
}

void CDTMFSendPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj ) return;


	UpdateData(TRUE);
	m_pNodeObj->m_strDTMF = m_strExpress;
	
	CPropertyPage::OnOK();
}

BOOL CDTMFSendPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
