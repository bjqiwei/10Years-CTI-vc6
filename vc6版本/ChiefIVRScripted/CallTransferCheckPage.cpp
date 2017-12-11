// CallTransferCheckPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "CallTransferCheckPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallTransferCheckPage property page

IMPLEMENT_DYNCREATE(CCallTransferCheckPage, CPropertyPage)

CCallTransferCheckPage::CCallTransferCheckPage() : CPropertyPage(CCallTransferCheckPage::IDD)
{
	//{{AFX_DATA_INIT(CCallTransferCheckPage)
	m_bAfterRings = FALSE;
	m_bNotAnswer = FALSE;
	m_nRings = 1;
	m_nTimes = 1;
	m_nWaitOption = -1;
	m_strPlayFile = _T("");
	m_nDialType = 0;
	m_strExpression = _T("");
	m_bClearDTMF = TRUE;
	m_nTransferAim = 0;
	m_nTransferType = 0;
	m_strCancelDTMF = _T("");
	m_strCompleteDTMF = _T("");
	m_bTalkWitDN = FALSE;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CCallTransferCheckPage::~CCallTransferCheckPage()
{
}

void CCallTransferCheckPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallTransferCheckPage)
	DDX_Check(pDX, IDC_CHECK_AFTERRINGS, m_bAfterRings);
	DDX_Check(pDX, IDC_CHECK_NOTANSWER, m_bNotAnswer);
	DDX_Text(pDX, IDC_EDIT_RINGS, m_nRings);
	DDV_MinMaxUInt(pDX, m_nRings, 1, 10);
	DDX_Text(pDX, IDC_EDIT_TIME, m_nTimes);
	DDV_MinMaxUInt(pDX, m_nTimes, 1, 120);
	DDX_Radio(pDX, IDC_RADIO_WAITOPTION1, m_nWaitOption);
	DDX_Text(pDX, IDC_EDIT_PLAYFILE, m_strPlayFile);
	DDX_Radio(pDX, IDC_RADIO_DIALTYPE1, m_nDialType);
	DDX_Text(pDX, IDC_EDIT_EXPRESSION2, m_strExpression);
	DDX_Check(pDX, IDC_CHECK_CLEARDTMF, m_bClearDTMF);
	DDX_Radio(pDX, IDC_RADIO_TRANSFERAIM1, m_nTransferAim);
	DDX_Radio(pDX, IDC_RADIO_TRANSFERTYPE1, m_nTransferType);
	DDX_Text(pDX, IDC_EDIT_CANCELTRANDTMF, m_strCancelDTMF);
	DDV_MaxChars(pDX, m_strCancelDTMF, 1);
	DDX_Text(pDX, IDC_EDIT_COMPLETEDTMF, m_strCompleteDTMF);
	DDX_Check(pDX, IDC_CHECK_TALKWITHAIMDN, m_bTalkWitDN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCallTransferCheckPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCallTransferCheckPage)
	ON_BN_CLICKED(IDC_BUTTON_PLAYFILE, OnButtonPlayfile)
	ON_BN_CLICKED(IDC_BUTTON_EX, OnButtonEx)
	ON_BN_CLICKED(IDC_RADIO_TRANSFERTYPE1, OnRadioTransfertype1)
	ON_BN_CLICKED(IDC_RADIO_TRANSFERTYPE2, OnRadioTransfertype2)
	ON_BN_CLICKED(IDC_RADIO_TRANSFERTYPE3, OnRadioTransfertype3)
	ON_BN_CLICKED(IDC_RADIO_DIALTYPE1, OnRadioDialtype1)
	ON_BN_CLICKED(IDC_RADIO_DIALTYPE2, OnRadioDialtype2)
	ON_BN_CLICKED(IDC_CHECK_NOTANSWER, OnCheckNotanswer)
	ON_BN_CLICKED(IDC_CHECK_AFTERRINGS, OnCheckAfterrings)
	ON_BN_CLICKED(IDC_RADIO_WAITOPTION1, OnRadioWaitoption1)
	ON_BN_CLICKED(IDC_RADIO_WAITOPTION2, OnRadioWaitoption2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallTransferCheckPage message handlers

void CCallTransferCheckPage::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);
	m_pNodeObj->m_bClearDTMF	= m_bClearDTMF;
	m_pNodeObj->m_bAfterRings	= m_bAfterRings;
	m_pNodeObj->m_bNotAnswer	= m_bNotAnswer;
	m_pNodeObj->m_nRings		= m_nRings;
	m_pNodeObj->m_nTimes		= m_nTimes;
	m_pNodeObj->m_strPlayFile	= m_strPlayFile;
	m_pNodeObj->m_nWaitOption	= m_nWaitOption;	
	m_pNodeObj->m_nDialType		= m_nDialType;
	m_pNodeObj->m_strExpression = m_strExpression;
	m_pNodeObj->m_nTransferAim = m_nTransferAim;
	m_pNodeObj->m_nTransferType = m_nTransferType;
	m_pNodeObj->m_strCancelDTMF = m_strCancelDTMF;

	m_pNodeObj->m_bTalkWitDN = m_bTalkWitDN;
	m_pNodeObj->m_strCompleteDTMF = m_strCompleteDTMF;

	m_pNodeObj->UpDate();
	CPropertyPage::OnOK();
}

BOOL CCallTransferCheckPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	EnableMyWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCallTransferCheckPage::InitPage(CTransferCheckObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_bClearDTMF	= m_pNodeObj->m_bClearDTMF;
	m_bAfterRings	= m_pNodeObj->m_bAfterRings;
	m_bNotAnswer	= m_pNodeObj->m_bNotAnswer;
	m_nRings		= m_pNodeObj->m_nRings;
	m_nTimes		= m_pNodeObj->m_nTimes;
	m_strPlayFile	= m_pNodeObj->m_strPlayFile;
	m_nWaitOption	= m_pNodeObj->m_nWaitOption;
	m_nDialType		= m_pNodeObj->m_nDialType;
	m_strExpression = m_pNodeObj->m_strExpression;
	m_nTransferAim = m_pNodeObj->m_nTransferAim;
	m_nTransferType = m_pNodeObj->m_nTransferType;
	m_strCancelDTMF = m_pNodeObj->m_strCancelDTMF;
	m_bTalkWitDN = m_pNodeObj->m_bTalkWitDN;
	m_strCompleteDTMF = m_pNodeObj->m_strCompleteDTMF;
}


void CCallTransferCheckPage::OnButtonPlayfile() 
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

void CCallTransferCheckPage::OnButtonEx() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strExpression = str;
		
		UpdateData(FALSE);
	}

	return;	
}

void CCallTransferCheckPage::OnRadioTransfertype1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CCallTransferCheckPage::OnRadioTransfertype2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CCallTransferCheckPage::OnRadioTransfertype3() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CCallTransferCheckPage::OnRadioDialtype1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CCallTransferCheckPage::OnRadioDialtype2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CCallTransferCheckPage::OnCheckNotanswer() 
{
	UpdateData(TRUE);
	EnableMyWindow();

}

void CCallTransferCheckPage::OnCheckAfterrings() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CCallTransferCheckPage::OnRadioWaitoption1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CCallTransferCheckPage::OnRadioWaitoption2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CCallTransferCheckPage::EnableMyWindow()
{
	if( 0 == m_nTransferType )
	{
		CWnd * phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM1);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM3);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_DIALTYPE1);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_RADIO_DIALTYPE2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_CHECK_CLEARDTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		
		phWnd = GetDlgItem(IDC_EDIT_EXPRESSION2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_BUTTON_EX);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_CHECK_NOTANSWER);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_CHECK_AFTERRINGS);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_TIME);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_RINGS);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_CHECK_TALKWITHAIMDN);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_COMPLETEDTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		//////////////////////////////////
		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION1);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION2);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

		if( 0 == m_nWaitOption )
		{

			phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
			phWnd = GetDlgItem(IDC_BUTTON_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
		}
		else
		{
			phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);
			phWnd = GetDlgItem(IDC_BUTTON_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);

		}

	}
	else if( 1 == m_nTransferType )
	{
		CWnd * phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM1);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM3);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_DIALTYPE1);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_RADIO_DIALTYPE2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_CHECK_CLEARDTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_EXPRESSION2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_BUTTON_EX);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_CHECK_NOTANSWER);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_CHECK_AFTERRINGS);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_TIME);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_RINGS);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_RINGS);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION1);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_BUTTON_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_CHECK_TALKWITHAIMDN);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_COMPLETEDTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

	}
	else
	{
		CWnd * phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM1);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM2);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_RADIO_TRANSFERAIM3);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_RADIO_DIALTYPE1);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

		phWnd = GetDlgItem(IDC_RADIO_DIALTYPE2);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_CHECK_CLEARDTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		if( 0 == m_nDialType )
		{
			phWnd = GetDlgItem(IDC_EDIT_EXPRESSION2);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);

			phWnd = GetDlgItem(IDC_BUTTON_EX);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
		}
		else
		{
			phWnd = GetDlgItem(IDC_EDIT_EXPRESSION2);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);

			phWnd = GetDlgItem(IDC_BUTTON_EX);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);
		}


		phWnd = GetDlgItem(IDC_CHECK_NOTANSWER);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_CHECK_AFTERRINGS);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

		phWnd = GetDlgItem(IDC_EDIT_TIME);
		if( NULL != phWnd )
			phWnd->EnableWindow( m_bNotAnswer);
		phWnd = GetDlgItem(IDC_EDIT_RINGS);
		if( NULL != phWnd )
			phWnd->EnableWindow( m_bAfterRings);

		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION1);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION2);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		if( 0 == m_nWaitOption )
		{

			phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
			phWnd = GetDlgItem(IDC_BUTTON_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
		}
		else
		{
			phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);
			phWnd = GetDlgItem(IDC_BUTTON_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);

		}
		phWnd = GetDlgItem(IDC_CHECK_TALKWITHAIMDN);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_COMPLETEDTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

	}
	return;
}
