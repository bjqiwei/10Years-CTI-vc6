// FunctionPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "FunctionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFunctionPage property page

IMPLEMENT_DYNCREATE(CFunctionPage, CPropertyPage)

CFunctionPage::CFunctionPage() : CPropertyPage(CFunctionPage::IDD)
{
	//{{AFX_DATA_INIT(CFunctionPage)
	m_strReturnValues = _T("");
	//}}AFX_DATA_INIT
	m_strFunction= _T("");
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CFunctionPage::~CFunctionPage()
{
}

void CFunctionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFunctionPage)
	DDX_Control(pDX, IDC_EDIT_FUNCTION, m_ctrFunction);
	DDX_Text(pDX, IDC_EDIT_RETURNVALUES, m_strReturnValues);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFunctionPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFunctionPage)
	ON_BN_CLICKED(IDC_BUTTON_FUNCTION, OnButtonFunction)
	ON_BN_CLICKED(IDC_BUTTON_RETURN, OnButtonReturn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFunctionPage message handlers

void CFunctionPage::OnButtonFunction() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_ctrFunction.SetSel(-1,0);
		m_ctrFunction.ReplaceSel(str);
		UpdateData(FALSE);
	}

	return;
	
}

void CFunctionPage::OnButtonReturn() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strReturnValues = str;
		UpdateData(FALSE);
	}

	return;
	
}



void CFunctionPage::InitPage(CFunctionObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;
	m_strFunction = m_pNodeObj->m_strFunction;
	m_strReturnValues = m_pNodeObj->m_strReturnValues;
	return;
}

void CFunctionPage::OnOK() 
{
	m_ctrFunction.GetWindowText(m_strFunction);
	m_pNodeObj->m_strFunction = m_strFunction;
	m_pNodeObj->m_strReturnValues = m_strReturnValues;
	
	CPropertyPage::OnOK();
}

BOOL CFunctionPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_ctrFunction.SetWindowText(m_strFunction);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
