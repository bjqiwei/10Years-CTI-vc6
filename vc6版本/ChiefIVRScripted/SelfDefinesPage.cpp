// SelfDefinesPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "SelfDefinesPage.h"
#include "AddActionDlg.h"
#include "AddEventDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelfDefinesPage property page

IMPLEMENT_DYNCREATE(CSelfDefinesPage, CPropertyPage)

CSelfDefinesPage::CSelfDefinesPage() : CPropertyPage(CSelfDefinesPage::IDD)
{
	//{{AFX_DATA_INIT(CSelfDefinesPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSelfDefinesPage::~CSelfDefinesPage()
{

}

void CSelfDefinesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelfDefinesPage)
	DDX_Control(pDX, IDC_LIST_EVENT, m_lstEvent);
	DDX_Control(pDX, IDC_LIST_ACTION, m_lstAction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelfDefinesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSelfDefinesPage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EVENT, OnDblclkListEvent)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACTION, OnDblclkListAction)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfDefinesPage message handlers

void CSelfDefinesPage::InitPage(CCustomObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;
}
LRESULT CSelfDefinesPage::OnActionNew( WPARAM wParam,LPARAM lParam)
{

	CAddActionDlg Dlg;
	Dlg.m_pDoc = m_pDoc;
	if( Dlg.DoModal() == IDOK )
	{
		if( "" == Dlg.m_strExpress ) 
		{
			MessageBox("Please check input!");
			return 0;
		}
		int nIndex = m_lstAction.GetItemCount();
		m_lstAction.InsertItem( nIndex, Dlg.m_strExpress );
		m_lstAction.SetItemData(nIndex,Dlg.m_nActionType);
	}

	return 0;
}

LRESULT CSelfDefinesPage::OnActionEdit( WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);

	int nIndex = m_lstAction.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	CAddActionDlg Dlg;
	Dlg.m_pDoc = m_pDoc;
	Dlg.m_strExpress = m_lstAction.GetItemText(nIndex,0);
	Dlg.m_nActionType = m_lstAction.GetItemData(nIndex);
	if( Dlg.DoModal() == IDOK )
	{
		if( "" == Dlg.m_strExpress ) 
		{
			MessageBox("Please check input!");
			return 0;
		}
		m_lstAction.SetItemText(nIndex,0, Dlg.m_strExpress );
		m_lstAction.SetItemData(nIndex,Dlg.m_nActionType);
		
	}

	return 0;
}

LRESULT CSelfDefinesPage::OnActionDel( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_lstAction.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_lstAction.DeleteItem(nIndex);

	return 0;
}

LRESULT CSelfDefinesPage::OnActionUP( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_lstAction.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nAbove = m_lstAction.GetNextItem( nIndex, LVNI_ABOVE ) ;
	if( -1 == nAbove )
		return 0;
	if( nIndex-1 > -1 )
	{
		
		m_lstAction.InsertItem( nAbove,m_lstAction.GetItemText(nIndex,0 ) );
		m_lstAction.SetItemText(nAbove,1, m_lstAction.GetItemText(nIndex + 1,1 ));//参数类型
		m_lstAction.DeleteItem( nIndex + 1 );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstAction.SetItemState(nAbove, flag,flag) ;

		UpdateData(FALSE);
	}
	return 0;
}

LRESULT CSelfDefinesPage::OnActionDown( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_lstAction.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nBelow = m_lstAction.GetNextItem( nIndex, LVNI_BELOW ) ;
	if( -1 == nBelow )
		return 0;
	if( nIndex+1 < m_lstAction.GetItemCount() )
	{
		
		m_lstAction.InsertItem( nBelow + 1 ,m_lstAction.GetItemText(nIndex,0 ) );
		m_lstAction.SetItemText(nBelow + 1,1, m_lstAction.GetItemText(nIndex,1 ));//参数类型
		m_lstAction.DeleteItem( nIndex );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstAction.SetItemState(nBelow, flag,flag) ;

		UpdateData(FALSE);
	}
	
	return 0;
}



LRESULT CSelfDefinesPage::OnEventNew( WPARAM wParam,LPARAM lParam)
{
	CAddEventDlg Dlg;
	if( Dlg.DoModal() == IDOK )
	{
		int nIndex = m_lstEvent.GetItemCount();
		m_lstEvent.InsertItem( nIndex, Dlg.m_strEventName );
		m_lstEvent.SetItemData(nIndex,Dlg.m_nEventType);
	}
	

	return 0;
}

LRESULT CSelfDefinesPage::OnEventEdit( WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);
	CAddEventDlg Dlg;
	if( Dlg.DoModal() == IDOK )
	{
		int nIndex = m_lstEvent.GetItemCount();
		m_lstEvent.InsertItem( nIndex, Dlg.m_strEventName );
		m_lstEvent.SetItemData(nIndex,Dlg.m_nEventType);
	}
	return 0;
}

LRESULT CSelfDefinesPage::OnEventDel( WPARAM wParam,LPARAM lParam)
{
	
	int nIndex = m_lstEvent.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_lstEvent.DeleteItem(nIndex);

	return 0;
}

LRESULT CSelfDefinesPage::OnEventUP( WPARAM wParam,LPARAM lParam)
{
	
	int nIndex = m_lstEvent.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	
	if( nIndex-1 > -1 )
	{
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstEvent.SetItemState(nIndex-1, flag,flag) ;
		UpdateData(FALSE);
	}
	
	return 0;
}



LRESULT CSelfDefinesPage::OnNew( WPARAM wParam,LPARAM lParam)
{
	if( IDC_BUTTON_ACTION == wParam )
	{
		return OnActionNew(wParam,lParam);
	}
	else if( IDC_BUTTON_EVENT == wParam )
	{
		return OnEventNew(wParam,lParam);
	}


	return 0;
}

LRESULT CSelfDefinesPage::OnEdit( WPARAM wParam,LPARAM lParam)
{
	if( IDC_BUTTON_ACTION == wParam )
	{
		return OnActionEdit(wParam,lParam);
	}
	else if( IDC_BUTTON_EVENT == wParam )
	{
		return OnEventEdit(wParam,lParam);
	}


	return 0;
}

LRESULT CSelfDefinesPage::OnDel( WPARAM wParam,LPARAM lParam)
{

	if( IDC_BUTTON_ACTION == wParam )
	{
		return OnActionDel(wParam,lParam);
	}
	else if( IDC_BUTTON_EVENT == wParam )
	{
		return OnEventDel(wParam,lParam);
	}


	return 0;
}

LRESULT CSelfDefinesPage::OnUP( WPARAM wParam,LPARAM lParam)
{
	if( IDC_BUTTON_ACTION == wParam )
	{
		return OnActionUP(wParam,lParam);
	}
	else if( IDC_BUTTON_EVENT == wParam )
	{
		return OnEventUP(wParam,lParam);
	}

	
	return 0;
}

LRESULT CSelfDefinesPage::OnDown( WPARAM wParam,LPARAM lParam)
{
	if( IDC_BUTTON_ACTION == wParam )
	{
		return OnActionDown(wParam,lParam);
	}
	
	return 0;
}


BOOL CSelfDefinesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	 m_ActionListBoxExBuddy.MySubclassDlgItem( IDC_BUTTON_ACTION, this );
	 m_ActionListBoxExBuddy.SetListbox( this->GetSafeHwnd() );
	
	 m_EventListBoxExBuddy.MySubclassDlgItem( IDC_BUTTON_EVENT, this );
	 m_EventListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	 
	m_lstEvent.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstEvent.InsertColumn(0,"Event",LVCFMT_CENTER,395,395);

	m_lstAction.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstAction.InsertColumn(0,"Action",LVCFMT_CENTER,395,395);

	CNextNodeList *pNNodeLst = &(m_pNodeObj->m_NextNodeList);
	POSITION pos = pNNodeLst->GetHeadPosition();
	int i = 0;
	while( pos )
	{
		CNextNode * pObj =pNNodeLst->GetNext(pos);
		if( pObj != NULL )
		{
			m_lstEvent.InsertItem( i, pObj->m_strName );//参数变量
			m_lstEvent.SetItemData( i,pObj->m_nType );
			i++;
		}

	}

	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList );
	pos = pDadaList->GetHeadPosition( );
	i = 0;
	CNodeData *pData = NULL;
	while( pos )
	{
		pData = pDadaList->GetNext(pos);
		if( NULL != pData )
		{
			m_lstAction.InsertItem( i, pData->m_strName );//参数变量
			m_lstAction.SetItemData( i,pData->m_nDataType );
			i++;
		}

	}



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelfDefinesPage::OnOK() 
{
	if( NULL == m_pNodeObj ) return;
	UpdateData(TRUE);

	//更新Action
	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList);
	while ( pDadaList->GetHeadPosition( ) )
	{
		delete pDadaList->RemoveHead();
	}

	int nCount = m_lstAction.GetItemCount();
	for( int i = 0; i < nCount; i++ )
	{
		CNodeData *pVar = new CNodeData;
		pVar->m_strName = m_lstAction.GetItemText(i,0);
		pVar->m_nDataType= m_lstAction.GetItemData(i);
		pDadaList->AddTail(pVar);

	}
	
	//更新Event
	m_pNodeObj->RemoveAllNextNode();
	nCount = m_lstEvent.GetItemCount();
	for( int j = 0; j < nCount; j++ )
	{
		m_pNodeObj->AddNextNodeObj( m_lstEvent.GetItemText(j,0),_T(""),0,m_lstEvent.GetItemData(j),j+1);
	}

	m_pNodeObj->UpDate();	

	CPropertyPage::OnOK();
}

void CSelfDefinesPage::OnDblclkListEvent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	CAddEventDlg Dlg;
	if( Dlg.DoModal() == IDOK )
	{
		int nIndex = m_lstEvent.GetItemCount();
		m_lstEvent.InsertItem( nIndex, Dlg.m_strEventName );
		m_lstEvent.SetItemData(nIndex,Dlg.m_nEventType);
	}
	
	*pResult = 0;
}

void CSelfDefinesPage::OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	*pResult = 0;
	int nIndex = m_lstAction.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return;

	CAddActionDlg Dlg;
	Dlg.m_pDoc = m_pDoc;
	Dlg.m_strExpress = m_lstAction.GetItemText(nIndex,0);
	Dlg.m_nActionType = m_lstAction.GetItemData(nIndex);
	if( Dlg.DoModal() == IDOK )
	{
		if( "" == Dlg.m_strExpress ) 
		{
			MessageBox("Please check input!");
			return;
		}
		m_lstAction.SetItemText(nIndex,0, Dlg.m_strExpress );
		m_lstAction.SetItemData(nIndex,Dlg.m_nActionType);
	}
	return;
	

}
