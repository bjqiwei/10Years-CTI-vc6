// OpenDBPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "DrawDoc.h"
#include "NodeTreeFormView.h"
#include "OpenDBPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNodeTreeFormView *g_pNodeTreeFormView ;
/////////////////////////////////////////////////////////////////////////////
// COpenDBPage property page

IMPLEMENT_DYNCREATE(COpenDBPage, CPropertyPage)

COpenDBPage::COpenDBPage() : CPropertyPage(COpenDBPage::IDD)
{
	//{{AFX_DATA_INIT(COpenDBPage)
	m_nDBType = 0;
	m_nDB = 0;
	m_strConn = _T("");
	m_strDBIdentify = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc	= NULL;
	m_pDBConnVar = NULL;
}

COpenDBPage::~COpenDBPage()
{
}

void COpenDBPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenDBPage)
	DDX_Radio(pDX, IDC_RADIO_TYPE1, m_nDBType);
	DDX_CBIndex(pDX, IDC_COMBO_DB, m_nDB);
	DDX_Text(pDX, IDC_EDIT_CONSTR, m_strConn);
	DDX_Text(pDX, IDC_EDIT_DBIDENTIFY, m_strDBIdentify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenDBPage, CPropertyPage)
	//{{AFX_MSG_MAP(COpenDBPage)
	ON_BN_CLICKED(IDC_BUTTON_BUILDCONN, OnButtonBuildconn)
	ON_BN_CLICKED(IDC_RADIO_TYPE1, OnRadioType1)
	ON_BN_CLICKED(IDC_RADIO_TYPE2, OnRadioType2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenDBPage message handlers

void COpenDBPage::OnOK() 
{

	UpdateData( TRUE );
	if( NULL == m_pDBConnVar ) return;
	if( 0 == m_nDBType && "" == m_strDBIdentify )
	{
		AfxMessageBox("Please input Identify !");
		return;
	}

	m_pNodeObj->m_nDBType = m_nDBType;
	m_pNodeObj->m_strConn = m_strConn;
	m_pDBConnVar->m_strConn = m_strConn;
	g_pNodeTreeFormView->DeleteItem( m_pDBConnVar->m_nKey + 8000);
	if( 0 == m_nDBType )
	{
		m_pDBConnVar->m_nKey = m_pNodeObj->m_nKey;
		m_pDBConnVar->m_strName = m_strDBIdentify;
		m_pNodeObj->m_nDBKey	= m_pNodeObj->m_nKey;
		m_pNodeObj->m_strDBName = m_strDBIdentify;
		
		g_pNodeTreeFormView->InsertItem(3,m_pDBConnVar->m_nKey+8000,m_pDBConnVar->m_strName, IDI_VARIABLE_DBCON , IDI_VARIABLE_DBCON );
	}
	else
	{
		switch( m_nDB )
		{
		case 0:
			m_pNodeObj->m_strDBName = "#51:SysDatabase1";
			m_pNodeObj->m_nDBKey = 51;
			break;
		case 1:
			m_pNodeObj->m_strDBName = "#52:SysDatabase2";
			m_pNodeObj->m_nDBKey = 52;
			break;
		case 2:
			m_pNodeObj->m_strDBName = "#53:SysDatabase3";
			m_pNodeObj->m_nDBKey = 53;
			break;
		case 3:
			m_pNodeObj->m_strDBName = "#53:SysDatabase4";
			m_pNodeObj->m_nDBKey = 54;
			break;
		case 4:
			m_pNodeObj->m_strDBName = "#54:SysDatabase4";
			m_pNodeObj->m_nDBKey = 54;
			break;
		case 5:
			m_pNodeObj->m_strDBName = "#55:SysDatabase4";
			m_pNodeObj->m_nDBKey = 55;
			break;
		case 6:
			m_pNodeObj->m_strDBName = "#56:SysDatabase4";
			m_pNodeObj->m_nDBKey = 56;
			break;
		case 7:
			m_pNodeObj->m_strDBName = "#57:SysDatabase4";
			m_pNodeObj->m_nDBKey = 57;
			break;
		case 8:
			m_pNodeObj->m_strDBName = "#57:SysDatabase4";
			m_pNodeObj->m_nDBKey = 58;
			break;
		}
		m_pDBConnVar->m_nKey = m_pNodeObj->m_nDBKey;
		m_pDBConnVar->m_strName = m_pNodeObj->m_strDBName;
	}


	CPropertyPage::OnOK();
}

BOOL COpenDBPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EnableMyWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COpenDBPage::InitPage( CDBOpenObj * pNodeObj, CDrawDoc *pDoc)
{
	
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;
	
	m_pDBConnVar = m_pDoc->FindDBConnectVariable( m_pNodeObj->m_nDBKey );
	if( NULL == m_pDBConnVar ) return;

	m_nDBType = m_pNodeObj->m_nDBType;
	if( 0 == m_nDBType )
	{
		
		m_strDBIdentify = m_pNodeObj->m_strDBName;
	}
	else
	{
		switch( m_pNodeObj->m_nDBKey )
		{
		case 51:
			{
				m_nDB = 0;
			}
			break;
		case 52:
			{
				m_nDB = 1;
			}
			break;
		case 53:
			{
				m_nDB = 2;
			}
			break;
		case 54:
			{
				m_nDB = 3;
			}
			break;
		case 55:
			{
				m_nDB = 4;
			}
			break;
		case 56:
			{
				m_nDB = 5;
			}
			break;
		case 57:
			{
				m_nDB = 6;
			}
			break;
		case 58:
			{
				m_nDB = 7;
			}
			break;
		default:
			m_nDB = 0;
			break;
		}
		
	
	}
	

	m_strConn = m_pNodeObj->m_strConn;

	
	

}

void COpenDBPage::OnButtonBuildconn() 
{
	UpdateData(TRUE);
	CMenu menu;
	CPoint popPoint;	
	menu.LoadMenu( IDR_POP_EDIT );
	GetCursorPos(&popPoint);//得到鼠标位置
	
	int nSelection =	menu.GetSubMenu(6)->TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY|TPM_RETURNCMD  ,popPoint.x,popPoint.y ,this);
	
	CString str(_T(""));
	menu.GetMenuString( nSelection,str,6);
	m_strConn = str;
	menu.DestroyMenu();
	UpdateData(FALSE);
	return;
}

void COpenDBPage::OnRadioType1() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void COpenDBPage::OnRadioType2() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void COpenDBPage::EnableMyWindow()
{
	if( 0 == m_nDBType )
	{
		CWnd * phWnd = GetDlgItem(IDC_COMBO_DB);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_DBIDENTIFY);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

	}
	else
	{
		CWnd * phWnd = GetDlgItem(IDC_COMBO_DB);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_DBIDENTIFY);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
	}
}
