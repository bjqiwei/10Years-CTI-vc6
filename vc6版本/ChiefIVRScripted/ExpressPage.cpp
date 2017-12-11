// ExpressPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "ExpressPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpressPage property page

IMPLEMENT_DYNCREATE(CExpressPage, CPropertyPage)

CExpressPage::CExpressPage() : CPropertyPage(CExpressPage::IDD)
{
	//{{AFX_DATA_INIT(CExpressPage)
	m_strExpressValues = _T("");
	//}}AFX_DATA_INIT
	m_strExpress = "";
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CExpressPage::~CExpressPage()
{
}

void CExpressPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpressPage)
	DDX_Control(pDX, IDC_EDIT_EXPRESS, m_ctrExpress);
	DDX_Text(pDX, IDC_EDIT_EXPRESS_VALUES, m_strExpressValues);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpressPage, CPropertyPage)
	//{{AFX_MSG_MAP(CExpressPage)
	ON_BN_CLICKED(IDC_BUTTON_EXPRESS, OnButtonExpress)
	ON_BN_CLICKED(IDC_BUTTON_EXPRESSVALUES, OnButtonExpressvalues)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpressPage message handlers

void CExpressPage::OnButtonExpress() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_ctrExpress.SetSel(-1,0);
		m_ctrExpress.ReplaceSel(str);
		UpdateData(FALSE);
	}

	return;
	
}

void CExpressPage::OnButtonExpressvalues() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strExpressValues = str;
		UpdateData(FALSE);
	}

	return;
	
}

void CExpressPage::InitPage(CExpressObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;
	m_strExpress = m_pNodeObj->m_strExpress;
	m_strExpressValues = m_pNodeObj->m_strExpressValues;
	return;
}

void CExpressPage::OnOK() 
{
	m_ctrExpress.GetWindowText(m_strExpress);
	m_pNodeObj->m_strExpress = m_strExpress;
	m_pNodeObj->m_strExpressValues = m_strExpressValues;
	CPropertyPage::OnOK();
}

BOOL CExpressPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_ctrExpress.SetWindowText(m_strExpress);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
