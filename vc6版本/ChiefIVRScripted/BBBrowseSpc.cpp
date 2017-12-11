// BBBrowseSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "BBBrowseSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBBBrowseSpc property page

IMPLEMENT_DYNCREATE(CBBBrowseSpc, CPropertyPage)

CBBBrowseSpc::CBBBrowseSpc() : CPropertyPage(CBBBrowseSpc::IDD)
{
	//{{AFX_DATA_INIT(CBBBrowseSpc)
	m_nBrowseType = -1;
	m_strField = _T("");
	m_strVar = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj	= NULL;
	m_pDoc		= NULL;
}

CBBBrowseSpc::~CBBBrowseSpc()
{
}

void CBBBrowseSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBBBrowseSpc)
	DDX_Control(pDX, IDC_LIST_FIELDVALUES, m_lstFieldValues);
	DDX_Control(pDX, IDC_COMBO_RECORDSET, m_cobRecordset);
	DDX_CBIndex(pDX, IDC_COMBO_BROWTYPE, m_nBrowseType);
	DDX_Text(pDX, IDC_EDIT_FIELD, m_strField);
	DDX_Text(pDX, IDC_EDIT_VAR, m_strVar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBBBrowseSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CBBBrowseSpc)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FIELDVALUES, OnClickListFieldvalues)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBBBrowseSpc message handlers

void CBBBrowseSpc::InitPage(CDBBrowseObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;



	return;
}

void CBBBrowseSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return ;

	UpdateData(TRUE);

	if( m_cobRecordset.GetCurSel( ) > -1 ) 
		m_cobRecordset.GetLBText( m_cobRecordset.GetCurSel( ), m_pNodeObj->m_strRecordset );

	m_pNodeObj->m_nBrowseType = m_nBrowseType;
	m_pNodeObj->m_nRecordsetKey = m_pNodeObj->GetVarKey( m_pNodeObj->m_strRecordset );
	
	//先删除字段记录，然后再添加进去
	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList);
	while ( pDadaList->GetHeadPosition( ) )
	{
		delete pDadaList->RemoveHead();
	}

	
	int nCount = m_lstFieldValues.GetItemCount();
	for( int i = 0; i < nCount; i++ )
	{
		CNodeData *pVar = new CNodeData;
		pVar->m_strName = m_lstFieldValues.GetItemText(i,0) ;
		pVar->m_strData = m_lstFieldValues.GetItemText(i,1);
		pVar->m_nKey    = m_pNodeObj->GetVarKey(pVar->m_strData);
		pDadaList->AddTail(pVar);

	}


	UpdateData(FALSE);

	CPropertyPage::OnOK();
}

BOOL CBBBrowseSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_lstFieldValues.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstFieldValues.InsertColumn(0,"Field Name",LVCFMT_LEFT,100,100);
	m_lstFieldValues.InsertColumn(1,"Variable",LVCFMT_LEFT,120,100);

	if( NULL == m_pNodeObj || NULL == m_pDoc ) return TRUE;


	CString str(_T(""));
	POSITION pos = m_pDoc->m_RecordsetVariList.GetHeadPosition() ;
	while( pos )
	{
		
		CRecordsetVar * pVar =m_pDoc->m_RecordsetVariList.GetNext(pos) ;
		if( pVar != NULL )
		{
			str.Format("@%d:%s", pVar->m_nKey, pVar->m_strName );
			m_cobRecordset.AddString( str );
		}
		
	}
	m_cobRecordset.SelectString(-1, m_pNodeObj->m_strRecordset );
	m_nBrowseType = m_pNodeObj->m_nBrowseType;

	int i = 0;
	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList);
	pos = pDadaList->GetHeadPosition( );
	while( pos )
	{
		CNodeData *pData = pDadaList->GetNext(pos);
		if( NULL != pData )
		{
			m_lstFieldValues.InsertItem( i, pData->m_strName );//字段名称
			m_lstFieldValues.SetItemText(i,1,pData->m_strData );//变量名称
			
			i++;
		}

	}

	 m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
	 m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CBBBrowseSpc::OnButtonSelect() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strVar =  str;
		UpdateData(FALSE);
	}

	return;
	
}
LRESULT CBBBrowseSpc::OnNew( WPARAM wParam,LPARAM lParam)
{
	this->UpdateData(TRUE);

	if( "" == m_strVar ||  "" == m_strField ) 
	{
		MessageBox("Please check input!");
		return 0;
	}

	int nIndex = m_lstFieldValues.GetItemCount();
	m_lstFieldValues.InsertItem(nIndex, m_strField );
	m_lstFieldValues.SetItemText(nIndex,1, m_strVar );


	return 0;
}

LRESULT CBBBrowseSpc::OnEdit( WPARAM wParam,LPARAM lParam)
{
	this->UpdateData(TRUE);
	int nIndex = m_lstFieldValues.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	m_lstFieldValues.SetItemText( nIndex,0,m_strField);
	m_lstFieldValues.SetItemText(nIndex,1, m_strVar );


	return 0;
}

LRESULT CBBBrowseSpc::OnDel( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_lstFieldValues.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_lstFieldValues.DeleteItem(nIndex);

	return 0;
}

LRESULT CBBBrowseSpc::OnUP( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_lstFieldValues.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nAbove = m_lstFieldValues.GetNextItem( nIndex, LVNI_ABOVE ) ;
	if( -1 == nAbove )
		return 0;
	if( nIndex-1 > -1 )
	{
		
		m_lstFieldValues.InsertItem( nAbove,m_lstFieldValues.GetItemText(nIndex,0 ) );
		m_lstFieldValues.SetItemText(nAbove,1, m_lstFieldValues.GetItemText(nIndex + 1,1 ));//参数类型
		m_lstFieldValues.DeleteItem( nIndex + 1 );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstFieldValues.SetItemState(nAbove, flag,flag) ;

		m_strField = m_lstFieldValues.GetItemText(nAbove,0) ;
		m_strVar = m_lstFieldValues.GetItemText(nAbove,1) ;

		UpdateData(FALSE);
	}	
	return 0;
}
LRESULT CBBBrowseSpc::OnDown( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_lstFieldValues.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nBelow = m_lstFieldValues.GetNextItem( nIndex, LVNI_BELOW ) ;
	if( -1 == nBelow )
		return 0;
	if( nIndex+1 < m_lstFieldValues.GetItemCount() )
	{
		
		m_lstFieldValues.InsertItem( nBelow + 1 ,m_lstFieldValues.GetItemText(nIndex,0 ) );
		m_lstFieldValues.SetItemText(nBelow + 1,1, m_lstFieldValues.GetItemText(nIndex,1 ));//参数类型
		m_lstFieldValues.DeleteItem( nIndex );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstFieldValues.SetItemState(nBelow, flag,flag) ;

		m_strField = m_lstFieldValues.GetItemText(nBelow,0) ;
		m_strVar=m_lstFieldValues.GetItemText(nBelow,1) ;

		UpdateData(FALSE);
	}

	return 0;
}


void CBBBrowseSpc::OnClickListFieldvalues(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstFieldValues.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;
	m_strField = m_lstFieldValues.GetItemText(nIndex,0);
	m_strVar = m_lstFieldValues.GetItemText(nIndex,1);

	UpdateData(FALSE);
	
	*pResult = 0;
}
