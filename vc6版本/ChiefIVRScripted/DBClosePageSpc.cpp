// DBClosePageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "DBClosePageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBClosePageSpc property page

IMPLEMENT_DYNCREATE(CDBClosePageSpc, CPropertyPage)

CDBClosePageSpc::CDBClosePageSpc() : CPropertyPage(CDBClosePageSpc::IDD)
{
	//{{AFX_DATA_INIT(CDBClosePageSpc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc	= NULL;
	m_strDB = _T("");
}

CDBClosePageSpc::~CDBClosePageSpc()
{
}

void CDBClosePageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBClosePageSpc)
	DDX_Control(pDX, IDC_COMBO_DB, m_cobDB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDBClosePageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CDBClosePageSpc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBClosePageSpc message handlers

void CDBClosePageSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);

	
	if( m_cobDB.GetCurSel( ) > -1 ) 
		m_cobDB.GetLBText( m_cobDB.GetCurSel( ), m_strDB  );
	m_pNodeObj->m_strDB = m_strDB;
	m_pNodeObj->m_nDBKey = m_pNodeObj->GetVarKey( m_strDB );
	if( "#" == m_strDB.Left(1) )
	{
		m_pNodeObj->m_nDBType = 1;
	}
	else
	{
		m_pNodeObj->m_nDBType = 0;
	}

	CPropertyPage::OnOK();
}

BOOL CDBClosePageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return TRUE;
	
	CString str(_T(""));
	POSITION pos = m_pDoc->m_DBConnVarList.GetHeadPosition() ;
	while( pos )
	{
		
		CDBConnVar * pVar =m_pDoc->m_DBConnVarList.GetNext(pos) ;
		if( pVar != NULL )
		{
			if( pVar->m_nKey > 100 )
			{
				str.Format("@%d:%s", pVar->m_nKey, pVar->m_strName );
				m_cobDB.AddString( str );
			}
		}
		
	}

	m_cobDB.AddString( "#51:SysDatabase1");
	m_cobDB.AddString( "#52:SysDatabase2");
	m_cobDB.AddString( "#53:SysDatabase3");
	m_cobDB.AddString( "#54:SysDatabase4");
	m_cobDB.AddString( "#55:SysDatabase5");
	m_cobDB.AddString( "#56:SysDatabase6");
	m_cobDB.AddString( "#57:SysDatabase7");
	m_cobDB.AddString( "#58:SysDatabase8");
	m_cobDB.SelectString(-1, m_strDB);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDBClosePageSpc::InitPage(CDBCloseObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;

	m_strDB = pNodeObj->m_strDB;

	
	return;

}
