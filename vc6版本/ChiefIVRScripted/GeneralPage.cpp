// GeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "GeneralPage.h"
#include "DrawDoc.h"
#include "NodeTreeFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage property page

extern CNodeTreeFormView *g_pNodeTreeFormView ;

IMPLEMENT_DYNCREATE(CGeneralPage, CPropertyPage)

CGeneralPage::CGeneralPage() : CPropertyPage(CGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CGeneralPage)
	m_strName = _T("");
	m_strDescription = _T("");
	m_strKey = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
	m_strPanelMsg = _T("");
	m_strKey = "";
}

CGeneralPage::~CGeneralPage()
{
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPage)
	DDX_Control(pDX, IDC_EDIT_PANELMSG, m_edtPanelMsg);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 120);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_EDIT_KEY, m_strKey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPage)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_BN_CLICKED(IDC_BUTTON_MSG, OnButtonMsg)
	ON_EN_CHANGE(IDC_EDIT_PANELMSG, OnChangeEditPanelmsg)
	ON_EN_SETFOCUS(IDC_EDIT_PANELMSG, OnSetfocusEditPanelmsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage message handlers

void CGeneralPage::OnOK() 
{
	m_edtPanelMsg.GetWindowText( m_strPanelMsg );
	if( NULL != m_pNodeObj && NULL != m_pDoc )
	{
		UpdateData(TRUE);
		
		g_pNodeTreeFormView->UpdateTreeItem(m_strName, m_pNodeObj->m_nKey );
		m_pNodeObj->m_strCaption = m_strName;
		m_pNodeObj->m_strDescription = m_strDescription;
		m_pNodeObj->m_strControlMsg = m_strPanelMsg;
			

	}
	CPropertyPage::OnOK();
}

void CGeneralPage::InitPage(CNodeObj *pObj, CDrawDoc *pDoc )
{
	if( NULL == pObj || NULL == pDoc )
		return;
	m_pDoc = pDoc;
	m_pNodeObj = pObj;

	m_strName = m_pNodeObj->m_strCaption;
	m_strDescription = m_pNodeObj->m_strDescription;
	m_strPanelMsg = m_pNodeObj->m_strControlMsg;
	m_strKey.Format("%d", m_pNodeObj->m_nKey );

	return;
}

void CGeneralPage::OnChangeEditName() 
{

	return;
}

void CGeneralPage::OnButtonMsg() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_edtPanelMsg.SetSel(-1,0);
		m_edtPanelMsg.ReplaceSel(str);

		UpdateData(FALSE);
	}
	return;
	
}

void CGeneralPage::OnChangeEditPanelmsg() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CGeneralPage::OnSetfocusEditPanelmsg() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_edtPanelMsg.SetWindowText( m_strPanelMsg );
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CGeneralPage::CheckNameIsAvail(CString strName)
{
	for(int i=0;i<strName.GetLength();i++)
	{
		 if(  '{' == strName.GetAt(i) )
			 return false;

		 if(  '}' == strName.GetAt(i) )
			 return false;

		 if(  '$' == strName.GetAt(i) )
			 return false;

		 if(  '"' == strName.GetAt(i) )
			 return false;
		 if(  '|' == strName.GetAt(i) )
			 return false;
		 if(  '&' == strName.GetAt(i) )
			 return false;
		 if(  '*' == strName.GetAt(i) )
			 return false;
		 if(  '/' == strName.GetAt(i) )
			 return false;
		 if(  '=' == strName.GetAt(i) )
			 return false;
		 if(  '>' == strName.GetAt(i) )
			 return false;
		 if(  '<' == strName.GetAt(i) )
			 return false;
		 if(  '@' == strName.GetAt(i) )
			 return false;
	}

	return true;
}
