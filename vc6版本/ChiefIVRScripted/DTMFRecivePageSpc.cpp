// DTMFRecivePageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "DTMFRecivePageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDTMFRecivePageSpc property page

IMPLEMENT_DYNCREATE(CDTMFRecivePageSpc, CPropertyPage)

CDTMFRecivePageSpc::CDTMFRecivePageSpc() : CPropertyPage(CDTMFRecivePageSpc::IDD)
{
	//{{AFX_DATA_INIT(CDTMFRecivePageSpc)
	m_bClearDTMF = FALSE;
	m_strDTMF = _T("");
	m_strSaveTo = _T("");
	m_nEnd = 0;
	m_nDTMFCount = 0;
	m_nTimeOut = 60;
	m_nDTMFType = 0;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CDTMFRecivePageSpc::~CDTMFRecivePageSpc()
{
}

void CDTMFRecivePageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDTMFRecivePageSpc)
	DDX_Check(pDX, IDC_CHECK_CLEARDTMF, m_bClearDTMF);
	DDX_Text(pDX, IDC_EDIT_DTMF, m_strDTMF);
	DDV_MaxChars(pDX, m_strDTMF, 1);
	DDX_Text(pDX, IDC_EDIT_VAR, m_strSaveTo);
	DDX_Radio(pDX, IDC_RADIO_EDN1, m_nEnd);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nDTMFCount);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_nTimeOut);
	DDV_MinMaxInt(pDX, m_nTimeOut, 5, 120);
	DDX_Radio(pDX, IDC_DTMF_OPTION1, m_nDTMFType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDTMFRecivePageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CDTMFRecivePageSpc)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_RADIO_EDN1, OnRadioEdn1)
	ON_BN_CLICKED(IDC_RADIO_EDN2, OnRadioEdn2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDTMFRecivePageSpc message handlers

void CDTMFRecivePageSpc::OnButtonSave() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strSaveTo =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CDTMFRecivePageSpc::InitPage(CDTMFReciveObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_bClearDTMF = m_pNodeObj->m_bClearDTMF;
	m_strSaveTo  = m_pNodeObj->m_SaveDTMF.m_strName;
	m_nEnd       = m_pNodeObj->m_nEndCondition;
	m_nDTMFCount = m_pNodeObj->m_nDTMFCount;
	m_strDTMF    = m_pNodeObj->m_strDTMF;
	m_nTimeOut   = m_pNodeObj->m_nTimeOut;
	m_nDTMFType  = m_pNodeObj->m_nDTMFType;
	return;
}

void CDTMFRecivePageSpc::OnOK() 
{
	UpdateData(TRUE);

	 m_pNodeObj->m_bClearDTMF = m_bClearDTMF;
	 m_pNodeObj->m_SaveDTMF.m_strName = m_strSaveTo;
	 m_pNodeObj->m_SaveDTMF.m_nKey = m_pNodeObj->GetVarKey( m_strSaveTo );
	 m_pNodeObj->m_nEndCondition = m_nEnd;
	 m_pNodeObj->m_nDTMFCount = m_nDTMFCount;
	 m_pNodeObj->m_strDTMF = m_strDTMF;
	 m_pNodeObj->m_nTimeOut = m_nTimeOut;
	 m_pNodeObj->m_nDTMFType = m_nDTMFType;
	CPropertyPage::OnOK();
}

BOOL CDTMFRecivePageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EnableMyWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




BOOL CDTMFRecivePageSpc::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CPropertyPage::OnHelpInfo(pHelpInfo);
}

void CDTMFRecivePageSpc::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}

void CDTMFRecivePageSpc::OnRadioEdn1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CDTMFRecivePageSpc::OnRadioEdn2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CDTMFRecivePageSpc::EnableMyWindow()
{
	if( 0 == m_nEnd )
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_COUNT);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

	}
	else
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_COUNT);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
	}
}
