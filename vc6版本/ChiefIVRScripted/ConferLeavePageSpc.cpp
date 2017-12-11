// ConferLeavePageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "ConferLeavePageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConferLeavePageSpc property page

IMPLEMENT_DYNCREATE(CConferLeavePageSpc, CPropertyPage)

CConferLeavePageSpc::CConferLeavePageSpc() : CPropertyPage(CConferLeavePageSpc::IDD)
{
	//{{AFX_DATA_INIT(CConferLeavePageSpc)
	m_strConferenceNO = _T("");
	m_strChnlIndex = _T("-1");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CConferLeavePageSpc::~CConferLeavePageSpc()
{
}

void CConferLeavePageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConferLeavePageSpc)
	DDX_Text(pDX, IDC_EDIT_CONF_NO, m_strConferenceNO);
	DDV_MaxChars(pDX, m_strConferenceNO, 120);
	DDX_Text(pDX, IDC_EDIT_CHANNELINDEX, m_strChnlIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConferLeavePageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CConferLeavePageSpc)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_CHINDEX, OnButtonChindex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConferLeavePageSpc message handlers

void CConferLeavePageSpc::InitPage(CLeaveConferObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_strConferenceNO = m_pNodeObj->m_strConferNO ;
	m_strChnlIndex = m_pNodeObj->m_strChnlIndex ;
	
	return;
}

void CConferLeavePageSpc::OnButtonSelect() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strConferenceNO =  str;
		UpdateData(FALSE);
	}

	return;
	
}


void CConferLeavePageSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);
	m_pNodeObj->m_strConferNO = m_strConferenceNO ;
	m_pNodeObj->m_strChnlIndex = m_strChnlIndex ;
	
	CPropertyPage::OnOK();
}

BOOL CConferLeavePageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConferLeavePageSpc::OnButtonChindex() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strChnlIndex =  str;
		UpdateData(FALSE);
	}

	
}
