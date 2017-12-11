// WaitCallSpecificProPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "WaitCallSpecificProPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWaitCallSpecificProPage property page

IMPLEMENT_DYNCREATE(CWaitCallSpecificProPage, CPropertyPage)

CWaitCallSpecificProPage::CWaitCallSpecificProPage() : CPropertyPage(CWaitCallSpecificProPage::IDD)
{
	//{{AFX_DATA_INIT(CWaitCallSpecificProPage)
	m_nRings = 0;
	m_bClearDtmf = FALSE;
	m_bReciveCallerID = FALSE;
	m_bReciveCalledID = FALSE;
	m_strSaveCalled = _T("");
	m_strSaveCallerID = _T("");
	m_bAnswerCall = FALSE;
	//}}AFX_DATA_INIT

	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CWaitCallSpecificProPage::~CWaitCallSpecificProPage()
{
}

void CWaitCallSpecificProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitCallSpecificProPage)
	DDX_Text(pDX, IDC_EDIT_RINGS, m_nRings);
	DDV_MinMaxUInt(pDX, m_nRings, 0, 5);
	DDX_Check(pDX, IDC_CHECK_CDTMF, m_bClearDtmf);
	DDX_Check(pDX, IDC_CHECK_RCALLERID, m_bReciveCallerID);
	DDX_Check(pDX, IDC_CHECK_RCALLEDID, m_bReciveCalledID);
	DDX_Text(pDX, IDC_EDIT_SAVECALLED, m_strSaveCalled);
	DDX_Text(pDX, IDC_EDIT_SAVECALLERID, m_strSaveCallerID);
	DDX_Check(pDX, IDC_CHECK_ANSWERCALL, m_bAnswerCall);
	//}}AFX_DATA_MAP
	//if(m_bReciveCallerID)
	//	MyDDV_CheckChars(pDX, m_strSaveCallerID, 256,1,"Please Check Save CallerID to input!");
	//if(m_bReciveCalledID)
	//	MyDDV_CheckChars(pDX, m_strSaveCalled, 256,1,"Please Check Save CalledID to input!");

}


BEGIN_MESSAGE_MAP(CWaitCallSpecificProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CWaitCallSpecificProPage)
	ON_BN_CLICKED(IDC_BUTTON_SAVECALLED, OnButtonSavecalled)
	ON_BN_CLICKED(IDC_BUTTON_SAVECALLERID, OnButtonSavecallerid)
	ON_BN_CLICKED(IDC_CHECK_CDTMF, OnCheckCdtmf)
	ON_BN_CLICKED(IDC_CHECK_RCALLERID, OnCheckRcallerid)
	ON_BN_CLICKED(IDC_CHECK_RCALLEDID, OnCheckRcalledid)
	ON_BN_CLICKED(IDC_CHECK_ANSWERCALL, OnCheckAnswercall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitCallSpecificProPage message handlers



void CWaitCallSpecificProPage::OnButtonSavecalled() 
{

	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strSaveCalled =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CWaitCallSpecificProPage::OnButtonSavecallerid() 
{

	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strSaveCallerID =  str;
		UpdateData(FALSE);
	}
	

	return;
}



BOOL CWaitCallSpecificProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();


	UpdateData(FALSE);
	EnableMyWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaitCallSpecificProPage::OnCheckCdtmf() 
{
	
}

void CWaitCallSpecificProPage::OnCheckRcallerid() 
{
	
	UpdateData(TRUE);
	EnableMyWindow();
}

void CWaitCallSpecificProPage::OnCheckRcalledid() 
{
	UpdateData(TRUE);
	EnableMyWindow();
}




void CWaitCallSpecificProPage::InitPage( CWaitCallObj * pObj ,CDrawDoc *pDoc )
{
	
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;
	
	
	m_nRings  = m_pNodeObj->m_nRings;
	m_bClearDtmf = m_pNodeObj->m_bClearDTMF;
	m_bReciveCallerID = m_pNodeObj->m_bRecCallerID;
	m_bReciveCalledID = m_pNodeObj->m_bRecCalledID;
	m_strSaveCallerID = m_pNodeObj->m_CallerID.m_strName;
	m_strSaveCalled   = m_pNodeObj->m_CalledID.m_strName;
	m_bAnswerCall	  = m_pNodeObj->m_bAnswerCall;

	return;
}

void CWaitCallSpecificProPage::OnOK() 
{
	
	if( NULL == m_pNodeObj ) return;

	UpdateData(TRUE);

	m_pNodeObj->m_nRings = m_nRings;
	m_pNodeObj->m_bClearDTMF = m_bClearDtmf;
	m_pNodeObj->m_bRecCallerID = m_bReciveCallerID ;
	m_pNodeObj->m_bAnswerCall  = m_bAnswerCall;

	m_pNodeObj->m_bRecCalledID = m_bReciveCalledID  ;
	m_pNodeObj->m_CallerID.m_strName = m_strSaveCallerID;
	m_pNodeObj->m_CalledID.m_strName = m_strSaveCalled;
	
	m_pNodeObj->m_CallerID.m_nKey = m_pNodeObj->GetVarKey(m_strSaveCallerID);
	m_pNodeObj->m_CalledID.m_nKey = m_pNodeObj->GetVarKey(m_strSaveCalled);
	

	CPropertyPage::OnOK();
}

void CWaitCallSpecificProPage::OnCheckAnswercall() 
{
	UpdateData(TRUE);

}

void CWaitCallSpecificProPage::EnableMyWindow()
{

	CWnd * phWnd = GetDlgItem(IDC_EDIT_SAVECALLERID);
	if( NULL != phWnd )
		phWnd->EnableWindow( m_bReciveCallerID);
	phWnd = GetDlgItem(IDC_BUTTON_SAVECALLERID);
	if( NULL != phWnd )
		phWnd->EnableWindow( m_bReciveCallerID);


	phWnd = GetDlgItem(IDC_EDIT_SAVECALLED);
	if( NULL != phWnd )
		phWnd->EnableWindow( m_bReciveCalledID);
	phWnd = GetDlgItem(IDC_BUTTON_SAVECALLED);
	if( NULL != phWnd )
		phWnd->EnableWindow( m_bReciveCalledID);
}
