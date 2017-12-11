// SetVariableProPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "SetVariableProPage.h"
#include "DrawDoc.h"
#include "AddNewVariable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetVariableProPage property page

IMPLEMENT_DYNCREATE(CSetVariableProPage, CPropertyPage)

CSetVariableProPage::CSetVariableProPage() : CPropertyPage(CSetVariableProPage::IDD)
{
	//{{AFX_DATA_INIT(CSetVariableProPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pnBaseVarKey	= NULL;
	m_pNodeObj		= NULL;
	m_pDoc			= NULL;
}

CSetVariableProPage::~CSetVariableProPage()
{
}

void CSetVariableProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetVariableProPage)
	DDX_Control(pDX, IDC_LIST_VAR, m_varList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetVariableProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSetVariableProPage)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST_VAR, OnItemclickListVar)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VAR, OnClickListVar)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VAR, OnDblclkListVar)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetVariableProPage message handlers







BOOL CSetVariableProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_varList.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_varList.InsertColumn(0,"Key",LVCFMT_LEFT,80,100);
	m_varList.InsertColumn(1,"Name",LVCFMT_LEFT,80,100);
	m_varList.InsertColumn(2,"Type",LVCFMT_LEFT,80,100);
	m_varList.InsertColumn(3,"Values",LVCFMT_LEFT,150,100);
	m_varList.InsertColumn(4,"Description",LVCFMT_LEFT,200,200);
	
	if( NULL != m_pNodeObj )
	{
		CString str(_T(""));

		
		POSITION pos = m_pDoc->m_NormalVarList.GetHeadPosition( );


		int i = 0;
		while (pos)
		{
			CNormalVar * pVar = m_pDoc->m_NormalVarList.GetNext(pos);
			if( NULL != pVar )
			{
				str.Format("%d", pVar->m_nKey );
				m_varList.InsertItem(i,str);
				m_varList.SetItemText(i,1, pVar->m_strName );
				m_varList.SetItemText(i,2, m_pNodeObj->GetDataTypeName(pVar->m_nVarDataType) );
				m_varList.SetItemText(i,3, pVar->m_strVariable );
				m_varList.SetItemText(i,4, pVar->m_strDescription );
			}
			i++;
		}
	}


     // Add the listbox buddy
     m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
     m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetVariableProPage::OnItemclickListVar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	


	*pResult = 0;
}

void CSetVariableProPage::OnClickListVar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CSetVariableProPage::OnOK() 
{
	
	if( NULL != m_pNodeObj )
	{
		UpdateData(TRUE);
		
		while( m_pDoc->m_NormalVarList.GetHeadPosition() )
		{
			delete m_pDoc->m_NormalVarList.RemoveHead();

		}



		int nCount = m_varList.GetItemCount();
		for( int i = 0; i < nCount; i++ )
		{
			CNormalVar *pVar = new CNormalVar;
			pVar->m_nKey = atoi( m_varList.GetItemText(i,0) );
			pVar->m_strName = m_varList.GetItemText(i,1);
			pVar->m_nVarDataType = m_pNodeObj->GetDataType( m_varList.GetItemText(i,2) );
			pVar->m_strVariable= m_varList.GetItemText(i,3);
			pVar->m_strDescription= m_varList.GetItemText(i,4);
			m_pDoc->m_NormalVarList.AddTail(pVar);

		}


	}
	CPropertyPage::OnOK();
}

void CSetVariableProPage::InitPage( CNodeObj * pNodeObj, CDrawDoc *pDoc, int *pnBaseVarKey )
{
	m_pNodeObj = pNodeObj;
	if( NULL == m_pNodeObj ) return;
	m_pnBaseVarKey = pnBaseVarKey;
	m_pDoc = pDoc;


	return;
}



BOOL CSetVariableProPage::JudgeNameValidity(CString strVarName)
{
	int nCount = m_varList.GetItemCount();
	for( int i = 0; i < nCount; i++ )
	{

		if( strVarName == m_varList.GetItemText(i,1) )
			return TRUE;
	}

	return FALSE;
}

LRESULT CSetVariableProPage::OnNew( WPARAM wParam,LPARAM lParam)
{
	CAddNewVariable dlg;
	if( dlg.DoModal() == IDOK )
	{
		BOOL bResult = JudgeNameValidity( dlg.m_strName );
		if( bResult ) 
		{
			MessageBox("The name of the Node is not Validity!");
			return 0;
		}

		CString str(_T(""));
		int nIndex = m_varList.GetItemCount();
		*m_pnBaseVarKey += 1;
		str.Format("%d",*m_pnBaseVarKey);
		m_varList.InsertItem(nIndex, str );
		m_varList.SetItemText(nIndex,1, dlg.m_strName );
		m_varList.SetItemText(nIndex,2, dlg.m_strType );
		m_varList.SetItemText(nIndex,3, dlg.m_strValues );
		m_varList.SetItemText(nIndex,4, dlg.m_strDescription );
		
	}
	

	return 0;
}

LRESULT CSetVariableProPage::OnEdit( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_varList.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	CAddNewVariable dlg;
	

	dlg.m_strName =  m_varList.GetItemText(nIndex,1) ;
	dlg.m_strType = m_varList.GetItemText(nIndex,2);
	dlg.m_strValues = m_varList.GetItemText(nIndex,3) ;
	dlg.m_strDescription = m_varList.GetItemText(nIndex,4) ;
	if( dlg.DoModal() == IDOK )
	{
		m_varList.SetItemText(nIndex,1, dlg.m_strName );
		m_varList.SetItemText(nIndex,2, dlg.m_strType );
		m_varList.SetItemText(nIndex,3, dlg.m_strValues );
		m_varList.SetItemText(nIndex,4, dlg.m_strDescription );
	}
	



	return 0;
}

LRESULT CSetVariableProPage::OnDel( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_varList.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_varList.DeleteItem(nIndex);

	return 0;
}

LRESULT CSetVariableProPage::OnUP( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_varList.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nAbove = m_varList.GetNextItem( nIndex, LVNI_ABOVE ) ;
	if( -1 == nAbove )
		return 0;
	if( nIndex-1 > -1 )
	{
		
		m_varList.InsertItem( nAbove,m_varList.GetItemText(nIndex,0 ) );
		m_varList.SetItemText(nAbove,1, m_varList.GetItemText(nIndex + 1,1 ));//参数类型
		m_varList.SetItemText(nAbove,2, m_varList.GetItemText(nIndex + 1,2 ));//参数类型
		m_varList.SetItemText(nAbove,3, m_varList.GetItemText(nIndex + 1,3 ));//参数类型
		m_varList.DeleteItem( nIndex + 1 );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_varList.SetItemState(nAbove, flag,flag) ;

		UpdateData(FALSE);
	}
	return 0;
}

LRESULT CSetVariableProPage::OnDown( WPARAM wParam,LPARAM lParam)
{
	
	int nIndex = m_varList.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nBelow = m_varList.GetNextItem( nIndex, LVNI_BELOW ) ;
	if( -1 == nBelow )
		return 0;
	if( nIndex+1 < m_varList.GetItemCount() )
	{
		
		m_varList.InsertItem( nBelow + 1 ,m_varList.GetItemText(nIndex,0 ) );
		m_varList.SetItemText(nBelow + 1,1, m_varList.GetItemText(nIndex,1 ));//参数类型
		m_varList.SetItemText(nBelow + 1,2, m_varList.GetItemText(nIndex,2 ));//参数类型
		m_varList.SetItemText(nBelow + 1,3, m_varList.GetItemText(nIndex,3 ));//参数类型
		m_varList.DeleteItem( nIndex );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_varList.SetItemState(nBelow, flag,flag) ;


		UpdateData(FALSE);
	}

	return 0;
}




void CSetVariableProPage::OnDblclkListVar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_varList.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 != nIndex )
	{

		CAddNewVariable dlg;
		

		dlg.m_strName =  m_varList.GetItemText(nIndex,1) ;
		dlg.m_strType = m_varList.GetItemText(nIndex,2);
		dlg.m_strValues = m_varList.GetItemText(nIndex,3) ;
		dlg.m_strDescription = m_varList.GetItemText(nIndex,4) ;
		if( dlg.DoModal() == IDOK )
		{
			m_varList.SetItemText(nIndex,1, dlg.m_strName );
			m_varList.SetItemText(nIndex,2, dlg.m_strType );
			m_varList.SetItemText(nIndex,3, dlg.m_strValues );
			m_varList.SetItemText(nIndex,4, dlg.m_strDescription );
		}
	
	}
	
	*pResult = 0;
}
