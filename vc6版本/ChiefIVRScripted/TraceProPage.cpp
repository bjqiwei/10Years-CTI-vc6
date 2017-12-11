// TraceProPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "DrawDoc.h"
#include "TraceProPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTraceProPage property page

IMPLEMENT_DYNCREATE(CTraceProPage, CPropertyPage)

CTraceProPage::CTraceProPage() : CPropertyPage(CTraceProPage::IDD)
{
	//{{AFX_DATA_INIT(CTraceProPage)
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
	m_strTrace="";
}

CTraceProPage::~CTraceProPage()
{
}

void CTraceProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTraceProPage)
	DDX_Control(pDX, IDC_EDIT_TRACE, m_ctrTrace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTraceProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTraceProPage)
	ON_BN_CLICKED(IDC_BUTTON_TRACE, OnButtonTrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraceProPage message handlers

void CTraceProPage::OnButtonTrace() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_ctrTrace.SetSel(-1,0);
		m_ctrTrace.ReplaceSel(str);
		UpdateData(FALSE);
	}

	return;
	
}

void CTraceProPage::InitPage(CTraceObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;
	m_strTrace = m_pNodeObj->m_strTrace;
	return;
}

BOOL CTraceProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_ctrTrace.SetWindowText(m_strTrace);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTraceProPage::OnOK() 
{
	m_ctrTrace.GetWindowText(m_strTrace);
	m_pNodeObj->m_strTrace = m_strTrace;
	
	CPropertyPage::OnOK();
}
