// StoreProcPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "NodeTreeFormView.h"
#include "StoreProcPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNodeTreeFormView *g_pNodeTreeFormView ;
/////////////////////////////////////////////////////////////////////////////
// CStoreProcPage property page

IMPLEMENT_DYNCREATE(CStoreProcPage, CPropertyPage)

CStoreProcPage::CStoreProcPage() : CPropertyPage(CStoreProcPage::IDD)
{
	//{{AFX_DATA_INIT(CStoreProcPage)
	m_strStoreProName = _T("");
	m_strParameterVar = _T("");
	m_strRSName = _T("");
	m_bReturnRecordSet = FALSE;
	m_strReturn = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj	= NULL;
	m_pDoc		= NULL;
	m_strDB = "";
	m_pRecsetVar = NULL;
}

CStoreProcPage::~CStoreProcPage()
{
}

void CStoreProcPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStoreProcPage)
	DDX_Control(pDX, IDC_COMBO_DB, m_cobDB);
	DDX_Control(pDX, IDC_LIST_PARAMETER, m_lstParam);
	DDX_Text(pDX, IDC_EDIT_STOREPRONAME, m_strStoreProName);
	DDV_MaxChars(pDX, m_strStoreProName, 30);
	DDX_Text(pDX, IDC_EDIT_PARAMETERVALUE, m_strParameterVar);
	DDX_Text(pDX, IDC_EDIT_RECSETNAME, m_strRSName);
	DDX_Check(pDX, IDC_CHECK_GETREORDSET, m_bReturnRecordSet);
	DDX_Text(pDX, IDC_EDIT_RETURNVALUE, m_strReturn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStoreProcPage, CPropertyPage)
	//{{AFX_MSG_MAP(CStoreProcPage)
	ON_BN_CLICKED(IDC_BUTTON_PARAMVALUE, OnButtonParamvalue)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARAMETER, OnClickListParameter)
	ON_BN_CLICKED(IDC_BUTTON_RETURNVALUE, OnButtonReturnvalue)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStoreProcPage message handlers

void CStoreProcPage::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);

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

	if( NULL == m_pRecsetVar ) return;
	g_pNodeTreeFormView->UpdateTreeItem( m_strRSName, m_pRecsetVar->m_nKey+8000 );
	m_pRecsetVar->m_strName		= m_strRSName;
	m_pRecsetVar->m_strDataBase = m_strDB;


	m_pNodeObj->m_strStoreProName = m_strStoreProName;
	m_pNodeObj->m_strReturn = m_strReturn;
	m_pNodeObj->m_bReturnRecordSet = m_bReturnRecordSet;


	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList);
	POSITION pos = pDadaList->GetHeadPosition( );
	//先删除参数记录，然后再添加进去
	while ( pDadaList->GetHeadPosition( ) )
	{
		delete pDadaList->RemoveHead();
	}

	int nCount = m_lstParam.GetItemCount();
	for( int i = 0; i < nCount; i++ )
	{
		CNodeData *pVar = new CNodeData;
		pVar->m_strName = m_lstParam.GetItemText(i,0) ;
	
		pDadaList->AddTail(pVar);

	}

	
	CPropertyPage::OnOK();
}

BOOL CStoreProcPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_lstParam.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstParam.InsertColumn(0,"Parameter Variable",LVCFMT_CENTER,200,200);

	if( NULL == m_pNodeObj || NULL == m_pDoc ) return TRUE;
	

	//下面开始初始化数据
	CString str("");
	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList);
	POSITION pos = pDadaList->GetHeadPosition( );
	int i = 0;
	CNodeData *pData = NULL;
	while( pos )
	{
		pData = pDadaList->GetNext(pos);
		if( NULL != pData )
		{
			m_lstParam.InsertItem( i, pData->m_strName );//参数变量
			i++;
		}

	}

	 m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
	 m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	pos = m_pDoc->m_DBConnVarList.GetHeadPosition() ;
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

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStoreProcPage::InitPage(CStoreProObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;


	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList);
	m_pRecsetVar  = pDoc->FindRecordsetVar( m_pNodeObj->m_nRecordsetKey );
	if( NULL == m_pRecsetVar ) return;

	m_strDB		= m_pRecsetVar->m_strDataBase;
	m_strRSName	= m_pRecsetVar->m_strName;

	m_strStoreProName	= m_pNodeObj->m_strStoreProName;
	m_strParameterVar = m_pNodeObj->m_strParameterVar;
	m_strReturn			= m_pNodeObj->m_strReturn;
	m_bReturnRecordSet	= m_pNodeObj->m_bReturnRecordSet;

	return;
}
LRESULT CStoreProcPage::OnNew( WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);
	
	if( "" == m_strParameterVar ) 
	{
		MessageBox("Please input Parameter Value!");
		return 0;
	}

	int nIndex = m_lstParam.GetItemCount();
	CString str("");
	m_lstParam.InsertItem(nIndex, m_strParameterVar );

  
	return 0;
}
LRESULT CStoreProcPage::OnEdit( WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);

	if( "" == m_strParameterVar ) 
	{
		MessageBox("Please input Parameter Value!");
		return 0;
	}

	int nIndex = m_lstParam.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	CString str("");
	m_lstParam.SetItemText(nIndex, 0, m_strParameterVar );


	return 0;
}

LRESULT CStoreProcPage::OnDel( WPARAM wParam,LPARAM lParam)
{
	
	int nIndex = m_lstParam.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_lstParam.DeleteItem(nIndex);

	return 0;
}

LRESULT CStoreProcPage::OnUP( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_lstParam.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nAbove = m_lstParam.GetNextItem( nIndex, LVNI_ABOVE ) ;
	if( -1 == nAbove )
		return 0;
	if( nIndex-1 > -1 )
	{
		
		m_lstParam.InsertItem( nAbove,m_lstParam.GetItemText(nIndex,0 ) );
		m_lstParam.SetItemText(nAbove,1, m_lstParam.GetItemText(nIndex + 1,1 ));//参数类型
		m_lstParam.DeleteItem( nIndex + 1 );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstParam.SetItemState(nAbove, flag,flag) ;

		m_strParameterVar = m_lstParam.GetItemText(nAbove,0) ;


		UpdateData(FALSE);
	}
	return 0;
}
LRESULT CStoreProcPage::OnDown( WPARAM wParam,LPARAM lParam)
{
	
	int nIndex = m_lstParam.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nBelow = m_lstParam.GetNextItem( nIndex, LVNI_BELOW ) ;
	if( -1 == nBelow )
		return 0;
	if( nIndex+1 < m_lstParam.GetItemCount() )
	{
		
		m_lstParam.InsertItem( nBelow + 1 ,m_lstParam.GetItemText(nIndex,0 ) );
		m_lstParam.SetItemText(nBelow + 1,1, m_lstParam.GetItemText(nIndex,1 ));//参数类型
		m_lstParam.DeleteItem( nIndex );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstParam.SetItemState(nBelow, flag,flag) ;

		m_strParameterVar = m_lstParam.GetItemText(nBelow,0);

		UpdateData(FALSE);
	}
	

	return 0;
}


void CStoreProcPage::OnButtonParamvalue() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strParameterVar =  str;
		UpdateData(FALSE);
	}
	
	return;
	
}

void CStoreProcPage::OnClickListParameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstParam.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;
	m_strParameterVar = m_lstParam.GetItemText(nIndex, 0 );

	UpdateData(FALSE);
	
	*pResult = 0;
}

void CStoreProcPage::OnButtonReturnvalue() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strReturn =  str;
		UpdateData(FALSE);
	}

	return;
	
}
