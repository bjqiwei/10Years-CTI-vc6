// CallRecordProPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "CallRecordProPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallRecordProPage property page

IMPLEMENT_DYNCREATE(CCallRecordProPage, CPropertyPage)

CCallRecordProPage::CCallRecordProPage() : CPropertyPage(CCallRecordProPage::IDD)
{
	//{{AFX_DATA_INIT(CCallRecordProPage)
	m_strFolder = _T("");
	m_nFileLength = 1;
	m_strConferNO = _T("");
	m_strEndDTMF = _T("");
	m_nRecordType = 0;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
	m_strFileName = _T("");
}

CCallRecordProPage::~CCallRecordProPage()
{
}

void CCallRecordProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallRecordProPage)
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_edtFileName);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolder);
	DDV_MaxChars(pDX, m_strFolder, 250);
	DDX_Text(pDX, IDC_EDIT_FILELENGTH, m_nFileLength);
	DDV_MinMaxInt(pDX, m_nFileLength, 0, 3600);
	DDX_Text(pDX, IDC_EDIT_CONFERNO4, m_strConferNO);
	DDX_Text(pDX, IDC_EDIT_RECORDEND_DTMF, m_strEndDTMF);
	DDV_MaxChars(pDX, m_strEndDTMF, 1);
	DDX_CBIndex(pDX, IDC_COMBO_RECORDTYPE, m_nRecordType);
	//}}AFX_DATA_MAP
	

}


BEGIN_MESSAGE_MAP(CCallRecordProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCallRecordProPage)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, OnButtonFolder)
	ON_BN_CLICKED(IDC_BUTTON_CONFNO, OnButtonConfno)
	ON_CBN_SELCHANGE(IDC_COMBO_RECORDTYPE, OnSelchangeComboRecordtype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallRecordProPage message handlers

void CCallRecordProPage::OnButtonFile() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_edtFileName.SetSel(-1,0);
		m_edtFileName.ReplaceSel(str);
		
		UpdateData(FALSE);
	}

	return;
	
}

void CCallRecordProPage::OnButtonFolder() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strFolder =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CCallRecordProPage::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;

	UpdateData(TRUE);
	m_edtFileName.GetWindowText( m_strFileName );
	m_pNodeObj->m_strFileName = m_strFileName;
	m_pNodeObj->m_strFolder = m_strFolder;
	m_pNodeObj->m_nFileLength = m_nFileLength;
	m_pNodeObj->m_nRecordType = m_nRecordType;
	m_pNodeObj->m_strConferNO = m_strConferNO;
	m_pNodeObj->m_strEndDTMF  = m_strEndDTMF;

	CPropertyPage::OnOK();
}

BOOL CCallRecordProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_edtFileName.SetWindowText( m_strFileName );
	EnableConferenceWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCallRecordProPage::InitPage(CRecordObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_strFileName  = m_pNodeObj->m_strFileName ;
	m_strFolder	   = m_pNodeObj->m_strFolder ;
	m_nFileLength  = m_pNodeObj->m_nFileLength ;
	m_nRecordType = m_pNodeObj->m_nRecordType;
	m_strConferNO = m_pNodeObj->m_strConferNO;
	m_strEndDTMF = m_pNodeObj->m_strEndDTMF;

	return;
}


void CCallRecordProPage::OnButtonConfno() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strConferNO =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CCallRecordProPage::EnableConferenceWindow()
{
	if( 0 ==  m_nRecordType )
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_CONFERNO4);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_BUTTON_CONFNO);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
	}
	else
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_CONFERNO4);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_BUTTON_CONFNO);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

	}
	
}



void CCallRecordProPage::OnSelchangeComboRecordtype() 
{
	UpdateData(TRUE);
	EnableConferenceWindow();
	
}
