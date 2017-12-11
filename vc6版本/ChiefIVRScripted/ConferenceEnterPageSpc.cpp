// ConferenceEnterPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "ConferenceEnterPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConferenceEnterPageSpc property page

IMPLEMENT_DYNCREATE(CConferenceEnterPageSpc, CPropertyPage)

CConferenceEnterPageSpc::CConferenceEnterPageSpc() : CPropertyPage(CConferenceEnterPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CConferenceEnterPageSpc)
	m_nAction = 0;
	m_strConferenceNO = _T("$8:ConferIndex");
	m_strChnlIndex = _T("-1");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CConferenceEnterPageSpc::~CConferenceEnterPageSpc()
{
}

void CConferenceEnterPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConferenceEnterPageSpc)
	DDX_CBIndex(pDX, IDC_COMBO_ACTION, m_nAction);
	DDX_Text(pDX, IDC_EDIT_CONF_NO, m_strConferenceNO);
	DDV_MaxChars(pDX, m_strConferenceNO, 120);
	DDX_Text(pDX, IDC_EDIT_CHANNELINDEX, m_strChnlIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConferenceEnterPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CConferenceEnterPageSpc)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_CHINDEX, OnButtonChindex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConferenceEnterPageSpc message handlers

void CConferenceEnterPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);
	m_pNodeObj->m_strConferNO = m_strConferenceNO;
	m_pNodeObj->m_nAction = m_nAction;
	m_pNodeObj->m_strChnlIndex	= m_strChnlIndex;


	CPropertyPage::OnOK();
}

BOOL CConferenceEnterPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConferenceEnterPageSpc::InitPage(CEnterConferObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_strConferenceNO	= m_pNodeObj->m_strConferNO ;
	m_nAction			= m_pNodeObj->m_nAction;
	m_strChnlIndex		= m_pNodeObj->m_strChnlIndex;
	
	return;
}

void CConferenceEnterPageSpc::OnButtonSelect() 
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



void CConferenceEnterPageSpc::OnButtonChindex() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strChnlIndex =  str;
		UpdateData(FALSE);
	}

	return;
	
}
