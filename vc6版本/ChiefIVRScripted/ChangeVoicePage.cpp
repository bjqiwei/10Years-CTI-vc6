// ChangeVoicePage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "ChangeVoicePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeVoicePage property page

IMPLEMENT_DYNCREATE(CChangeVoicePage, CPropertyPage)

CChangeVoicePage::CChangeVoicePage() : CPropertyPage(CChangeVoicePage::IDD)
{
	//{{AFX_DATA_INIT(CChangeVoicePage)
	m_nChangeVoiceType = 0;
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_pNodeObj = NULL;
}

CChangeVoicePage::~CChangeVoicePage()
{
}

void CChangeVoicePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeVoicePage)
	DDX_Radio(pDX, IDC_CHANGEVOICE_TYPE1, m_nChangeVoiceType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeVoicePage, CPropertyPage)
	//{{AFX_MSG_MAP(CChangeVoicePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeVoicePage message handlers

void CChangeVoicePage::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);
	
	m_pNodeObj->m_nChangeVoiceType = m_nChangeVoiceType;

	CPropertyPage::OnOK();
}

BOOL CChangeVoicePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChangeVoicePage::InitPage(CChangeVoiceObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_nChangeVoiceType	= m_pNodeObj->m_nChangeVoiceType;
	
}
