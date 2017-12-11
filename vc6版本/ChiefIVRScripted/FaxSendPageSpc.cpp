// FaxSendPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "FaxSendPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFaxSendPageSpc property page

IMPLEMENT_DYNCREATE(CFaxSendPageSpc, CPropertyPage)

CFaxSendPageSpc::CFaxSendPageSpc() : CPropertyPage(CFaxSendPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CFaxSendPageSpc)
	m_strFileName = _T("");
	m_strFolder = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CFaxSendPageSpc::~CFaxSendPageSpc()
{
}

void CFaxSendPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFaxSendPageSpc)
	DDX_Text(pDX, IDC_EDIT_FAXFILE, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFaxSendPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CFaxSendPageSpc)
	ON_BN_CLICKED(IDC_BUTTON_FAXFOLDER, OnButtonFaxfolder)
	ON_BN_CLICKED(IDC_BUTTON_FAX, OnButtonFax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaxSendPageSpc message handlers

void CFaxSendPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj ) return;
	UpdateData(TRUE);
	m_pNodeObj->m_strFileName = m_strFileName;
	m_pNodeObj->m_strFolder = m_strFolder;

	
	CPropertyPage::OnOK();
}

BOOL CFaxSendPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFaxSendPageSpc::InitPage(CFaxSendObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_strFileName = m_pNodeObj->m_strFileName;
	m_strFolder   = m_pNodeObj->m_strFolder;




	return;

}



void CFaxSendPageSpc::OnButtonFax() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strFileName = str;
		UpdateData(FALSE);
	}

	return;
	
}

void CFaxSendPageSpc::OnButtonFaxfolder() 
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

