// SwitchPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "SwitchPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwitchPageSpc property page

IMPLEMENT_DYNCREATE(CSwitchPageSpc, CPropertyPage)

CSwitchPageSpc::CSwitchPageSpc() : CPropertyPage(CSwitchPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CSwitchPageSpc)
	m_nSwitchType = 0;
	m_strAimChnl = _T("");
	m_nAimType = 0;
	m_nWaitOption = -1;
	m_strPlayFile = _T("");
	m_nGetFreeChWay = -1;
	m_strBeginCH = _T("");
	m_strEndCH = _T("");
	m_nTimeOut = 0;
	//}}AFX_DATA_INIT
}

CSwitchPageSpc::~CSwitchPageSpc()
{
}

void CSwitchPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSwitchPageSpc)
	DDX_CBIndex(pDX, IDC_COMBO_SWITCHTYPE, m_nSwitchType);
	DDX_Text(pDX, IDC_EDIT_AIMCH, m_strAimChnl);
	DDX_Radio(pDX, IDC_RADIO_AIMCHTYPE1, m_nAimType);
	DDX_Radio(pDX, IDC_RADIO_WAITOPTION1, m_nWaitOption);
	DDX_Text(pDX, IDC_EDIT_PLAYFILE, m_strPlayFile);
	DDX_Radio(pDX, IDC_SWITCH_GETFREECH1, m_nGetFreeChWay);
	DDX_Text(pDX, IDC_EDIT_BEGIN_CH2, m_strBeginCH);
	DDX_Text(pDX, IDC_EDIT_END_CH2, m_strEndCH);
	DDX_Text(pDX, IDC_SWITCH_TIMEOUT, m_nTimeOut);
	DDV_MinMaxInt(pDX, m_nTimeOut, 0, 120);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSwitchPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CSwitchPageSpc)
	ON_BN_CLICKED(IDC_BUTTON_AIMCH, OnButtonAimch)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_RADIO_AIMCHTYPE1, OnRadioAimchtype1)
	ON_BN_CLICKED(IDC_RADIO_AIMCHTYPE2, OnRadioAimchtype2)
	ON_BN_CLICKED(IDC_RADIO_WAITOPTION1, OnRadioWaitoption1)
	ON_BN_CLICKED(IDC_RADIO_WAITOPTION2, OnRadioWaitoption2)
	ON_BN_CLICKED(IDC_RADIO_WAITOPTION3, OnRadioWaitoption3)
	ON_BN_CLICKED(IDC_BTN_BEGIN_CH, OnBtnBeginCh)
	ON_BN_CLICKED(IDC_BTN_END_CH, OnBtnEndCh)
	ON_CBN_SELCHANGE(IDC_COMBO_SWITCHTYPE, OnSelchangeComboSwitchtype)
	ON_BN_CLICKED(IDC_SWITCH_GETFREECH1, OnSwitchGetfreech1)
	ON_BN_CLICKED(IDC_SWITCH_GETFREECH2, OnSwitchGetfreech2)
	ON_BN_CLICKED(IDC_RADIO_AIMCHTYPE3, OnRadioAimchtype3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwitchPageSpc message handlers

void CSwitchPageSpc::OnButtonAimch() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strAimChnl =  str;
		UpdateData(FALSE);
	}

	return;
}

void CSwitchPageSpc::InitPage(CSwitchObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;


	m_nSwitchType = m_pNodeObj->m_nSwitchType;
	m_strAimChnl = m_pNodeObj->m_strAimChnl;
	m_nAimType = m_pNodeObj->m_nAimType;
	m_nWaitOption = m_pNodeObj->m_nWaitOption;
	m_strPlayFile = m_pNodeObj->m_strPlayFile;



	m_nGetFreeChWay = m_pNodeObj->m_nGetFreeChWay;
	m_strBeginCH = m_pNodeObj->m_strBeginCH;
	m_strEndCH = m_pNodeObj->m_strEndCH;
	m_nTimeOut= m_pNodeObj->m_nTimeOut;

	return;
}

void CSwitchPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj ) return;
	UpdateData(TRUE);

	m_pNodeObj->m_nSwitchType = m_nSwitchType;
	m_pNodeObj->m_strAimChnl = m_strAimChnl;
	m_pNodeObj->m_nAimType = m_nAimType;
	m_pNodeObj->m_nWaitOption = m_nWaitOption ;
	m_pNodeObj->m_strPlayFile = m_strPlayFile;


	m_pNodeObj->m_nGetFreeChWay= m_nGetFreeChWay;
	m_pNodeObj->m_strBeginCH= m_strBeginCH;
	m_pNodeObj->m_strEndCH= m_strEndCH;
	m_pNodeObj->m_nTimeOut= m_nTimeOut;

	CPropertyPage::OnOK();
}

BOOL CSwitchPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EnableMyWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSwitchPageSpc::OnButtonFile() 
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

void CSwitchPageSpc::OnRadioAimchtype1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CSwitchPageSpc::OnRadioAimchtype2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CSwitchPageSpc::OnRadioWaitoption1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CSwitchPageSpc::OnRadioWaitoption2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CSwitchPageSpc::OnRadioWaitoption3() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CSwitchPageSpc::EnableMyWindow()
{
	if( m_nSwitchType < 4 )//·ÇCall Agent »òCall Customer
	{
		CWnd * phWnd = GetDlgItem(IDC_SWITCH_GETFREECH1);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_SWITCH_GETFREECH2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_BEGIN_CH2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_BTN_BEGIN_CH);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_END_CH2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_BTN_END_CH);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION1);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION2);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION3);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_BUTTON_FILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_CONN_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_SWITCH_TIMEOUT);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
	}
	else//Call Agent »òCall Customer
	{
		CWnd * phWnd = GetDlgItem(IDC_SWITCH_GETFREECH1);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_SWITCH_GETFREECH2);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_BEGIN_CH2);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

		phWnd = GetDlgItem(IDC_BTN_BEGIN_CH);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_END_CH2);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_BTN_END_CH);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION1);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION2);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_RADIO_WAITOPTION3);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_BUTTON_FILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_CONN_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_SWITCH_TIMEOUT);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		/////////////
		if( 0 == m_nAimType )
		{
			CWnd * phWnd = GetDlgItem(IDC_EDIT_AIMCH);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
			phWnd = GetDlgItem(IDC_BUTTON_AIMCH);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
			phWnd = GetDlgItem(IDC_COMBO_CHTYPE);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
		}
		else
		{
			CWnd * phWnd = GetDlgItem(IDC_EDIT_AIMCH);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);
			phWnd = GetDlgItem(IDC_BUTTON_AIMCH);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);
			phWnd = GetDlgItem(IDC_COMBO_CHTYPE);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);
		}
		///////////////////////////////////
		if( 2 == m_nWaitOption )
		{
			CWnd * phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);
			phWnd = GetDlgItem(IDC_BUTTON_FILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( TRUE);

		}
		else
		{
			CWnd * phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
			phWnd = GetDlgItem(IDC_BUTTON_FILE);
			if( NULL != phWnd )
				phWnd->EnableWindow( FALSE);
		}



		///////////////////////////////////
		if( m_nSwitchType > 4 )
		{
			if( 0 == m_nGetFreeChWay )
			{
				CWnd * phWnd = GetDlgItem(IDC_EDIT_BEGIN_CH2);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
				phWnd = GetDlgItem(IDC_BTN_BEGIN_CH);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
				phWnd = GetDlgItem(IDC_EDIT_END_CH2);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
				phWnd = GetDlgItem(IDC_BTN_END_CH);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
			}
			else
			{
				CWnd * phWnd = GetDlgItem(IDC_EDIT_BEGIN_CH2);
				if( NULL != phWnd )
					phWnd->EnableWindow( TRUE);
				phWnd = GetDlgItem(IDC_BTN_BEGIN_CH);
				if( NULL != phWnd )
					phWnd->EnableWindow( TRUE);
				phWnd = GetDlgItem(IDC_EDIT_END_CH2);
				if( NULL != phWnd )
					phWnd->EnableWindow( TRUE);
				phWnd = GetDlgItem(IDC_BTN_END_CH);
				if( NULL != phWnd )
					phWnd->EnableWindow( TRUE);
			}
		}
		else
		{
				CWnd * phWnd = GetDlgItem(IDC_EDIT_BEGIN_CH2);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
				phWnd = GetDlgItem(IDC_BTN_BEGIN_CH);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
				phWnd = GetDlgItem(IDC_EDIT_END_CH2);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
				phWnd = GetDlgItem(IDC_BTN_END_CH);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
				phWnd = GetDlgItem(IDC_SWITCH_GETFREECH1);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);
				phWnd = GetDlgItem(IDC_SWITCH_GETFREECH2);
				if( NULL != phWnd )
					phWnd->EnableWindow( FALSE);				
		}

	}



}



void CSwitchPageSpc::OnBtnBeginCh() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strBeginCH =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CSwitchPageSpc::OnBtnEndCh() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strEndCH =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CSwitchPageSpc::OnSelchangeComboSwitchtype() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}





void CSwitchPageSpc::OnSwitchGetfreech1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CSwitchPageSpc::OnSwitchGetfreech2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}



void CSwitchPageSpc::OnRadioAimchtype3() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}
