// ConferRequestPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "ConferRequestPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConferRequestPage property page

IMPLEMENT_DYNCREATE(CConferRequestPage, CPropertyPage)

CConferRequestPage::CConferRequestPage() : CPropertyPage(CConferRequestPage::IDD)
{
	//{{AFX_DATA_INIT(CConferRequestPage)
	m_nDialType = -1;
	m_nAction = -1;
	m_strAimTel = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CConferRequestPage::~CConferRequestPage()
{
}

void CConferRequestPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConferRequestPage)
	DDX_CBIndex(pDX, IDC_COMBO_DIALTYPE, m_nDialType);
	DDX_CBIndex(pDX, IDC_COMBO_ACTION, m_nAction);
	DDX_Text(pDX, IDC_EDIT_AIMTEL, m_strAimTel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConferRequestPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConferRequestPage)
	ON_BN_CLICKED(IDC_BUTTON_DialTel, OnBUTTONDialTel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConferRequestPage message handlers




void CConferRequestPage::InitPage(CConferRequestObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_nDialType			= m_pNodeObj->m_nDialType;
	m_nAction			= m_pNodeObj->m_nAction;
	m_strAimTel			= m_pNodeObj->m_strDialTel;
	
	return;
}

void CConferRequestPage::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);

	m_pNodeObj->m_nDialType			= m_nDialType;
	m_pNodeObj->m_nAction			= m_nAction;
	m_pNodeObj->m_strDialTel		= m_strAimTel;	
	
	CPropertyPage::OnOK();
}

BOOL CConferRequestPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConferRequestPage::OnBUTTONDialTel() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strAimTel =  str;
		UpdateData(FALSE);
	}

	return;	
}
