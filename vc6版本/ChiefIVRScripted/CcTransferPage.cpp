// CcTransferPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "CcTransferPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCcTransferPage property page

IMPLEMENT_DYNCREATE(CCcTransferPage, CPropertyPage)

CCcTransferPage::CCcTransferPage() : CPropertyPage(CCcTransferPage::IDD)
{
	//{{AFX_DATA_INIT(CCcTransferPage)
	m_nTransferType = -1;
	m_nTimeOut = 0;
	m_nWaitType = -1;
	m_strPlayFile = _T("");
	m_strCompleteDTMF = _T("");
	m_strCancelDTMF = _T("");
	m_strAimDN = _T("");
	m_bTimeOut = FALSE;
	m_bTalkWitDN = FALSE;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CCcTransferPage::~CCcTransferPage()
{
}

void CCcTransferPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCcTransferPage)
	DDX_Radio(pDX, IDC_RADIO_TRANSFERAIM1, m_nTransferType);
	DDX_Text(pDX, IDC_EDIT_TIME, m_nTimeOut);
	DDX_Radio(pDX, IDC_RADIO_WAITOPTION1, m_nWaitType);
	DDX_Text(pDX, IDC_EDIT_PLAYFILE, m_strPlayFile);
	DDX_Text(pDX, IDC_EDIT_COMPLETEDTMF, m_strCompleteDTMF);
	DDX_Text(pDX, IDC_EDIT_CANCELTRANDTMF, m_strCancelDTMF);
	DDX_Text(pDX, IDC_EDIT_AIMDN, m_strAimDN);
	DDX_Check(pDX, IDC_CHECK_TIMEOUT, m_bTimeOut);
	DDX_Check(pDX, IDC_CHECK_TALKWITHAIMDN, m_bTalkWitDN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCcTransferPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCcTransferPage)
	ON_BN_CLICKED(IDC_BUTTON_AIMDN, OnButtonAimdn)
	ON_BN_CLICKED(IDC_BUTTON_PLAYFILE, OnButtonPlayfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCcTransferPage message handlers

void CCcTransferPage::OnOK() 
{
	if( NULL == m_pNodeObj ) return;
	UpdateData(TRUE);

	m_pNodeObj->m_nTransferType = m_nTransferType;
	m_pNodeObj->m_nTimeOut = m_nTimeOut;
	m_pNodeObj->m_nWaitType = m_nWaitType;
	m_pNodeObj->m_strPlayFile = m_strPlayFile;
	m_pNodeObj->m_strCompleteDTMF = m_strCompleteDTMF;
	m_pNodeObj->m_strCancelDTMF = m_strCancelDTMF;
	m_pNodeObj->m_strAimDN = m_strAimDN ;
	m_pNodeObj->m_bTimeOut = m_bTimeOut;
	m_pNodeObj->m_bTalkWitDN = m_bTalkWitDN;
	
	CPropertyPage::OnOK();
}

BOOL CCcTransferPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCcTransferPage::InitPage(CCCTransferObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;

	m_nTransferType = m_pNodeObj->m_nTransferType;
	m_nTimeOut = m_pNodeObj->m_nTimeOut;
	m_nWaitType = m_pNodeObj->m_nWaitType;
	m_strPlayFile = m_pNodeObj->m_strPlayFile;
	m_strCompleteDTMF = m_pNodeObj->m_strCompleteDTMF;
	m_strCancelDTMF = m_pNodeObj->m_strCancelDTMF;
	m_strAimDN = m_pNodeObj->m_strAimDN;
	m_bTimeOut = m_pNodeObj->m_bTimeOut;
	m_bTalkWitDN = m_pNodeObj->m_bTalkWitDN;

	return;
}

void CCcTransferPage::OnButtonAimdn() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strAimDN =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CCcTransferPage::OnButtonPlayfile() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strPlayFile =  str;
		UpdateData(FALSE);
	}

	return;
	
}
