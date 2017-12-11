// DLLLoadPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "NodeTreeFormView.h"
#include "DLLLoadPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNodeTreeFormView *g_pNodeTreeFormView ;
/////////////////////////////////////////////////////////////////////////////
// CDLLLoadPageSpc property page

IMPLEMENT_DYNCREATE(CDLLLoadPageSpc, CPropertyPage)

CDLLLoadPageSpc::CDLLLoadPageSpc() : CPropertyPage(CDLLLoadPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CDLLLoadPageSpc)
	m_strHandle = _T("");
	m_strFile = _T("");
	m_nHandleType = 0;
	m_nDLLHandle =0;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc	= NULL;
	m_pDLLVar = NULL;
}

CDLLLoadPageSpc::~CDLLLoadPageSpc()
{
}

void CDLLLoadPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLLLoadPageSpc)
	DDX_Text(pDX, IDC_EDIT_HANDLE, m_strHandle);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFile);
	DDX_Radio(pDX, IDC_RADIO_HANDLE1, m_nHandleType);
	DDX_CBIndex(pDX, IDC_COMBO_DLL, m_nDLLHandle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLLLoadPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CDLLLoadPageSpc)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_RADIO_HANDLE1, OnRadioHandle1)
	ON_BN_CLICKED(IDC_RADIO_HANDLE2, OnRadioHandle2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLLLoadPageSpc message handlers

void CDLLLoadPageSpc::OnOK() 
{
	UpdateData( TRUE);
	if( NULL == m_pDLLVar ) return;

	CString strOldName = m_pDLLVar->m_strName;
	
	
	m_pDLLVar->m_strDLLFileName = m_strFile ;
	m_pNodeObj->m_nHandleType  	= m_nHandleType;
	m_pNodeObj->m_strDLLFile = m_strFile;
	g_pNodeTreeFormView->DeleteItem( m_pDLLVar->m_nKey + 8000);
	if( 0 == m_nHandleType )
	{
		m_pDLLVar->m_nKey			= m_pNodeObj->m_nKey;
		m_pNodeObj->m_nDLLKey		= m_pNodeObj->m_nKey;
		m_pDLLVar->m_strName		= m_strHandle;
		g_pNodeTreeFormView->InsertItem(4,m_pDLLVar->m_nKey+8000,m_pDLLVar->m_strName, IDI_VARIABLE_DLL , IDI_VARIABLE_DLL );
	}
	else
	{
		switch( m_nDLLHandle  )
		{
		case 0:
			{
				m_pDLLVar->m_nKey = 59;
				m_pNodeObj->m_nDLLKey = 59;
				m_pDLLVar->m_strName = "#59:SysDLLHandle1" ;
			}
			break;
		case 1:
			{
				m_pDLLVar->m_nKey = 60;
				m_pNodeObj->m_nDLLKey = 60;
				m_pDLLVar->m_strName = "#60:SysDLLHandle2" ;
			}
			break;
		case 2:
			{
				m_pDLLVar->m_nKey = 61;
				m_pNodeObj->m_nDLLKey = 61;
				m_pDLLVar->m_strName = "#61:SysDLLHandle3" ;
			}
			break;
		case 3:
			{
				m_pDLLVar->m_nKey = 62;
				m_pNodeObj->m_nDLLKey = 62;
				m_pDLLVar->m_strName = "#62:SysDLLHandle4" ;
			}
			break;
		case 4:
			{
				m_pDLLVar->m_nKey = 63;
				m_pNodeObj->m_nDLLKey = 63;
				m_pDLLVar->m_strName = "#63:SysDLLHandle5" ;
			}
			break;
		case 5:
			{
				m_pDLLVar->m_nKey = 64;
				m_pNodeObj->m_nDLLKey = 64;
				m_pDLLVar->m_strName = "#64:SysDLLHandle6" ;
			}
			break;
		case 6:
			{
				m_pDLLVar->m_nKey = 65;
				m_pNodeObj->m_nDLLKey = 65;
				m_pDLLVar->m_strName = "#65:SysDLLHandle7" ;
			}
			break;
		case 7:
			{
				m_pDLLVar->m_nKey = 66;
				m_pNodeObj->m_nDLLKey = 66;
				m_pDLLVar->m_strName = "#66:SysDLLHandle8" ;
			}
			break;
		}

	}


	CPropertyPage::OnOK();
}

BOOL CDLLLoadPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EnableMyWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLLLoadPageSpc::InitPage(CDLLLoadObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;


	m_pDLLVar = pDoc->FindDLLVariable( m_pNodeObj->m_nDLLKey );

	if( NULL == m_pDLLVar ) return;

	
	
	m_nHandleType = m_pNodeObj->m_nHandleType;
	if( 0 == m_pNodeObj->m_nHandleType )
	{
		m_strHandle = m_pDLLVar->m_strName;
		
	}
	else
	{
		switch(m_pDLLVar->m_nKey  )
		{
		case 59:
			{
				m_nDLLHandle = 0;
			}
			break;
		case 60:
			{
				m_nDLLHandle = 1;
			}
			break;
		case 61:
			{
				m_nDLLHandle = 2;
			}
			break;
		case 62:
			{
				m_nDLLHandle = 3;
			}
			break;
		case 63:
			{
				m_nDLLHandle = 4;
			}
			break;
		case 64:
			{
				m_nDLLHandle = 5;
			}
			break;
		case 65:
			{
				m_nDLLHandle = 6;
			}
			break;
		case 66:
			{
				m_nDLLHandle = 7;
			}
			break;
		default:
			m_nDLLHandle = 0;
			break;
		}

	}


	m_strFile	= m_pNodeObj->m_strDLLFile;

	return;
}

void CDLLLoadPageSpc::OnButtonFile() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strFile = str;
		UpdateData(FALSE);
	}

	return;	
}



void CDLLLoadPageSpc::OnRadioHandle1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CDLLLoadPageSpc::OnRadioHandle2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CDLLLoadPageSpc::EnableMyWindow()
{
	if( 0 == m_nHandleType )
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_HANDLE);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_COMBO_DLL);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

	}
	else
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_HANDLE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_COMBO_DLL);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
	}
}
