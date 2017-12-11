// GetConferMemberPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "GetConferMemberPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetConferMemberPage property page

IMPLEMENT_DYNCREATE(CGetConferMemberPage, CPropertyPage)

CGetConferMemberPage::CGetConferMemberPage() : CPropertyPage(CGetConferMemberPage::IDD)
{
	//{{AFX_DATA_INIT(CGetConferMemberPage)
	m_strConferNO = _T("");
	m_strMemberCount = _T("");
	m_strListenNum = _T("");
	m_strMaxMemberNum = _T("");
	m_strMaxSpeakerNum = _T("");
	m_strMaxSpeakingNum = _T("");
	m_strSpeakerNum = _T("");
	m_strSpeakingNum = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CGetConferMemberPage::~CGetConferMemberPage()
{
}

void CGetConferMemberPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetConferMemberPage)
	DDX_Text(pDX, IDC_EDIT_CONFERNO, m_strConferNO);
	DDX_Text(pDX, IDC_EDIT_MEMBERCOUNT, m_strMemberCount);
	DDX_Text(pDX, IDC_EDIT_LISTEN_NUM, m_strListenNum);
	DDV_MaxChars(pDX, m_strListenNum, 50);
	DDX_Text(pDX, IDC_EDIT_MAX_MEMBER, m_strMaxMemberNum);
	DDV_MaxChars(pDX, m_strMaxMemberNum, 50);
	DDX_Text(pDX, IDC_EDIT_MAX_SPEAKER, m_strMaxSpeakerNum);
	DDV_MaxChars(pDX, m_strMaxSpeakerNum, 50);
	DDX_Text(pDX, IDC_EDIT_MAX_SPEAKING, m_strMaxSpeakingNum);
	DDX_Text(pDX, IDC_EDIT_SPEAKER, m_strSpeakerNum);
	DDV_MaxChars(pDX, m_strSpeakerNum, 50);
	DDX_Text(pDX, IDC_EDIT_SPEAKING, m_strSpeakingNum);
	DDV_MaxChars(pDX, m_strSpeakingNum, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetConferMemberPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGetConferMemberPage)
	ON_BN_CLICKED(IDC_BUTTON_CONFNO, OnButtonConfno)
	ON_BN_CLICKED(IDC_BUTTON_CONFMEMBER, OnButtonConfmember)
	ON_BN_CLICKED(IDC_BUTTON_MAX_SPEAKING, OnButtonMaxSpeaking)
	ON_BN_CLICKED(IDC_BUTTON_MAX_SPEAKER, OnButtonMaxSpeaker)
	ON_BN_CLICKED(IDC_BUTTON_MAX_MEMBER, OnButtonMaxMember)
	ON_BN_CLICKED(IDC_BUTTON_SPEAKING, OnButtonSpeaking)
	ON_BN_CLICKED(IDC_BUTTON_SPEAKER, OnButtonSpeaker)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, OnButtonListen)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_CONFNO, OnDoubleclickedButtonConfno)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetConferMemberPage message handlers

void CGetConferMemberPage::OnButtonConfno() 
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

void CGetConferMemberPage::OnButtonConfmember() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strMemberCount =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CGetConferMemberPage::InitPage(CGetConferMemberObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_strConferNO		= m_pNodeObj->m_strConferNo;
	m_strMemberCount	= m_pNodeObj->m_strMemberCount;
	m_strMaxMemberNum	= m_pNodeObj->m_strMaxMemberNum;
	m_strMaxSpeakerNum	= m_pNodeObj->m_strMaxSpeakerNum;
	m_strMaxSpeakingNum = m_pNodeObj->m_strMaxSpeakingNum;
	m_strListenNum		= m_pNodeObj->m_strListenNum;
	m_strSpeakerNum		= m_pNodeObj->m_strSpeakerNum;
	m_strSpeakingNum	= m_pNodeObj->m_strSpeakingNum;
	return;
}

void CGetConferMemberPage::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);
	m_pNodeObj->m_strConferNo		= m_strConferNO;
	m_pNodeObj->m_strMemberCount	= m_strMemberCount;
	m_pNodeObj->m_strMaxMemberNum	= m_strMaxMemberNum;
	m_pNodeObj->m_strMaxSpeakerNum	= m_strMaxSpeakerNum;
	m_pNodeObj->m_strMaxSpeakingNum = m_strMaxSpeakingNum;
	m_pNodeObj->m_strListenNum		= m_strListenNum;
	m_pNodeObj->m_strSpeakerNum		= m_strSpeakerNum;
	m_pNodeObj->m_strSpeakingNum	= m_strSpeakingNum;

	CPropertyPage::OnOK();
}

BOOL CGetConferMemberPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGetConferMemberPage::OnButtonMaxSpeaking() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strMaxSpeakingNum =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CGetConferMemberPage::OnButtonMaxSpeaker() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strMaxSpeakerNum =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CGetConferMemberPage::OnButtonMaxMember() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strMaxMemberNum =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CGetConferMemberPage::OnButtonSpeaking() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strSpeakingNum =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CGetConferMemberPage::OnButtonSpeaker() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strSpeakerNum =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CGetConferMemberPage::OnButtonListen() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strListenNum =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CGetConferMemberPage::OnDoubleclickedButtonConfno() 
{
	// TODO: Add your control notification handler code here
	
}
