// FaxRecivePageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "FaxRecivePageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFaxRecivePageSpc property page

IMPLEMENT_DYNCREATE(CFaxRecivePageSpc, CPropertyPage)

CFaxRecivePageSpc::CFaxRecivePageSpc() : CPropertyPage(CFaxRecivePageSpc::IDD)
{
	//{{AFX_DATA_INIT(CFaxRecivePageSpc)
	m_strFolder = _T("");
	m_strPage = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
	m_strFileName = "";
}

CFaxRecivePageSpc::~CFaxRecivePageSpc()
{
}

void CFaxRecivePageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFaxRecivePageSpc)
	DDX_Control(pDX, IDC_EDIT_FAXFILE, m_edtFaxfile);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolder);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_strPage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFaxRecivePageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CFaxRecivePageSpc)
	ON_BN_CLICKED(IDC_BUTTON_FAXFILE, OnButtonFaxfile)
	ON_BN_CLICKED(IDC_BUTTON_FLODER, OnButtonFloder)
	ON_BN_CLICKED(IDC_BUTTON_PAGENUMBER, OnButtonPagenumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaxRecivePageSpc message handlers

BOOL CFaxRecivePageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_edtFaxfile.SetWindowText( m_strFileName );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFaxRecivePageSpc::OnOK() 
{
	if( NULL == m_pNodeObj ) return;
	UpdateData(TRUE);
	m_edtFaxfile.GetWindowText( m_strFileName );
	m_pNodeObj->m_strFileName = m_strFileName;
	m_pNodeObj->m_strFolder = m_strFolder;
	m_pNodeObj->m_strPage = m_strPage;

	CPropertyPage::OnOK();
}

void CFaxRecivePageSpc::InitPage(CFaxReciveObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_strFileName = m_pNodeObj->m_strFileName;
	m_strFolder   = m_pNodeObj->m_strFolder;
	m_strPage     = m_pNodeObj->m_strPage;


	return;

}

void CFaxRecivePageSpc::OnButtonFaxfile() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		
		m_edtFaxfile.SetSel(-1,0);
		m_edtFaxfile.ReplaceSel(str);
		
		UpdateData(FALSE);
	}

	return;
	
}

void CFaxRecivePageSpc::OnButtonFloder() 
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

void CFaxRecivePageSpc::OnButtonPagenumber() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strPage =  str;
		UpdateData(FALSE);
	}

	return;
	
}
