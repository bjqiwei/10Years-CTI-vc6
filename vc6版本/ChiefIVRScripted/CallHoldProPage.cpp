// CallHoldProPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "CallHoldProPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallHoldProPage property page

IMPLEMENT_DYNCREATE(CCallHoldProPage, CPropertyPage)

CCallHoldProPage::CCallHoldProPage() : CPropertyPage(CCallHoldProPage::IDD)
{
	//{{AFX_DATA_INIT(CCallHoldProPage)
	m_nHold = -1;
	m_strFile = _T("");
	m_bPlay = FALSE;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CCallHoldProPage::~CCallHoldProPage()
{
}

void CCallHoldProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallHoldProPage)
	DDX_Radio(pDX, IDC_RADIO_HOLD1, m_nHold);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFile);
	DDV_MaxChars(pDX, m_strFile, 255);
	DDX_Check(pDX, IDC_CHECK_PLAY, m_bPlay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCallHoldProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCallHoldProPage)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_RADIO_HOLD1, OnRadioHold1)
	ON_BN_CLICKED(IDC_RADIO_HOLD2, OnRadioHold2)
	ON_BN_CLICKED(IDC_CHECK_PLAY, OnCheckPlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallHoldProPage message handlers

void CCallHoldProPage::InitPage( CCallHoldObj * pObj ,CDrawDoc *pDoc )
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_nHold = m_pNodeObj->m_nCallHold;
	m_bPlay = m_pNodeObj->m_bPlayVox;
	m_strFile = m_pNodeObj->m_strPlayFile;


	return;


}

void CCallHoldProPage::OnButtonFile() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strFile =  str;
		UpdateData(FALSE);
	}
	

	return;

}

void CCallHoldProPage::OnRadioHold1() 
{

	
}

void CCallHoldProPage::OnRadioHold2() 
{

	
}

void CCallHoldProPage::OnOK() 
{
	if( NULL == m_pNodeObj ) return;


	UpdateData(TRUE);

	m_pNodeObj->m_nCallHold = m_nHold ;
	m_pNodeObj->m_bPlayVox = m_bPlay  ;
	m_pNodeObj->m_strPlayFile = m_strFile  ;	

	CPropertyPage::OnOK();
}

void CCallHoldProPage::OnCheckPlay() 
{
	
	//m_bPlay = !m_bPlay;
	UpdateData(TRUE);
	EnableMyWindow();

}

void CCallHoldProPage::EnableMyWindow()
{
	CWnd * phWnd = GetDlgItem(IDC_EDIT_FILE);
	if( NULL != phWnd )
		phWnd->EnableWindow( m_bPlay);
	phWnd = GetDlgItem(IDC_BUTTON_FILE);
	if( NULL != phWnd )
		phWnd->EnableWindow( m_bPlay);

	return;
}

BOOL CCallHoldProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EnableMyWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
