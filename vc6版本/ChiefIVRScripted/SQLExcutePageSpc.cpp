// SQLExcutePageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "NodeTreeFormView.h"
#include "SQLExcutePageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNodeTreeFormView *g_pNodeTreeFormView ;
/////////////////////////////////////////////////////////////////////////////
// CSQLExcutePageSpc property page

IMPLEMENT_DYNCREATE(CSQLExcutePageSpc, CPropertyPage)

CSQLExcutePageSpc::CSQLExcutePageSpc() : CPropertyPage(CSQLExcutePageSpc::IDD)
{
	//{{AFX_DATA_INIT(CSQLExcutePageSpc)
	m_nSQLType = -1;
	m_strRSName = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj	= NULL;
	m_pDoc		= NULL;
	m_strSQL	= _T("");
	m_pRecsetVar = NULL;
	m_strDB	= _T("");
}

CSQLExcutePageSpc::~CSQLExcutePageSpc()
{
}

void CSQLExcutePageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSQLExcutePageSpc)
	DDX_Control(pDX, IDC_COMBO_DB, m_cobDB);
	DDX_Control(pDX, IDC_EDIT_SQL, m_editSQL);
	DDX_CBIndex(pDX, IDC_COMBO_CMDTYPE, m_nSQLType);
	DDX_Text(pDX, IDC_EDIT_RECSETNAME, m_strRSName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSQLExcutePageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CSQLExcutePageSpc)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_SQL, OnButtonSql)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSQLExcutePageSpc message handlers

void CSQLExcutePageSpc::OnOK() 
{
	if( NULL == m_pNodeObj ) return;
	UpdateData(TRUE);


	m_editSQL.GetWindowText(m_strSQL);
	if( m_cobDB.GetCurSel( ) > -1 ) 
		m_cobDB.GetLBText( m_cobDB.GetCurSel( ), m_strDB );
	m_pNodeObj->m_nDBKey	= m_pNodeObj->GetVarKey(m_strDB);
	m_pNodeObj->m_strDB		= m_strDB;
	if( "#" == m_pNodeObj->m_strDB.Left(1) )
	{
		m_pNodeObj->m_nDBType = 1;
	}
	else
	{
		m_pNodeObj->m_nDBType = 0;
	}

	
	m_pNodeObj->m_strRecordset  = m_strRSName;
	m_pNodeObj->m_nSQLType		= m_nSQLType;
	m_pNodeObj->m_strSQL		= m_strSQL;

	if( NULL == m_pRecsetVar ) return;

	
	g_pNodeTreeFormView->UpdateTreeItem( m_strRSName, m_pRecsetVar->m_nKey+8000 );
	m_pRecsetVar->m_strName		= m_strRSName;
	m_pRecsetVar->m_nSQLType	= m_nSQLType;
	m_pRecsetVar->m_strSQL		= m_strSQL;
	m_pRecsetVar->m_strDataBase = m_strDB;

	
	CPropertyPage::OnOK();
}

BOOL CSQLExcutePageSpc::OnInitDialog() 
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

	m_cobDB.AddString("#51:SysDatabase1");
	m_cobDB.AddString("#52:SysDatabase2");
	m_cobDB.AddString("#53:SysDatabase3");
	m_cobDB.AddString("#54:SysDatabase4");
	m_cobDB.AddString("#55:SysDatabase5");
	m_cobDB.AddString("#56:SysDatabase6");
	m_cobDB.AddString("#57:SysDatabase7");
	m_cobDB.AddString("#58:SysDatabase8");

	m_cobDB.SelectString(-1, m_strDB);

	m_editSQL.SetWindowText(m_strSQL);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSQLExcutePageSpc::InitPage(CSQLExcuteObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;

	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList);

	m_pRecsetVar  = pDoc->FindRecordsetVar( m_pNodeObj->m_nRecordsetKey );
	if( NULL == m_pRecsetVar ) return;

	m_nSQLType	= m_pRecsetVar->m_nSQLType;
	m_strSQL	= m_pRecsetVar->m_strSQL;
	m_strDB		= m_pRecsetVar->m_strDataBase;
	m_strRSName	= m_pRecsetVar->m_strName;
	
	return;
}

void CSQLExcutePageSpc::OnButtonSelect() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		
		m_editSQL.SetSel(-1,0);
		m_editSQL.ReplaceSel(str);
		UpdateData(FALSE);
	}

	return;
	
}

void CSQLExcutePageSpc::OnButtonSql() 
{
	CMenu menu;
	CPoint popPoint;	
	menu.LoadMenu( IDR_POP_EDIT );
	GetCursorPos(&popPoint);//得到鼠标位置
	
	int nSelection =	menu.GetSubMenu(5)->TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY|TPM_RETURNCMD  ,popPoint.x,popPoint.y ,this);
	
	CString str(_T(""));
	menu.GetMenuString( nSelection,str,5);

	m_editSQL.SetSel(-1,0);
	m_editSQL.ReplaceSel(str);

	menu.DestroyMenu();
	return;
	
}
