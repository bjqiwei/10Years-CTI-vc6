// CompareProPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "CompareProPage.h"
#include "EditCompareListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompareProPage property page

IMPLEMENT_DYNCREATE(CCompareProPage, CPropertyPage)

CCompareProPage::CCompareProPage() : CPropertyPage(CCompareProPage::IDD)
{
	//{{AFX_DATA_INIT(CCompareProPage)
	m_strVariable = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CCompareProPage::~CCompareProPage()
{
}

void CCompareProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompareProPage)
	DDX_Control(pDX, IDC_LIST_VAR, m_listVar);
	DDX_Text(pDX, IDC_EDIT_VARIABLE, m_strVariable);
	DDV_MaxChars(pDX, m_strVariable, 120);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompareProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCompareProPage)
	ON_BN_CLICKED(IDC_BUTTON_VARIABLE, OnButtonVariable)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST_VAR, OnItemclickListVar)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VAR, OnDblclkListVar)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompareProPage message handlers


void CCompareProPage::OnOK() 
{

	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);
	m_pNodeObj->m_nVarKey = m_pNodeObj->GetVarKey( m_strVariable );
	m_pNodeObj->m_strVarName = m_strVariable;
	RemoveNextNode();
	int nCount = m_listVar.GetItemCount();
	for( int i = 0; i < nCount; i++ )
	{
		m_pNodeObj->AddNextNodeObj( m_listVar.GetItemText(i,1),m_listVar.GetItemText(i,3),0,atoi(m_listVar.GetItemText(i,0)),i+2);
	}
	m_pNodeObj->UpDate();	


	CPropertyPage::OnOK();
}

BOOL CCompareProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	m_listVar.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	
	m_listVar.InsertColumn(0,"TypeKey",LVCFMT_LEFT,120,120);
	m_listVar.InsertColumn(1,"Description",LVCFMT_LEFT,120,120);
	m_listVar.InsertColumn(2,"TypeName",LVCFMT_LEFT,120,120);
	m_listVar.InsertColumn(3,"Expression",LVCFMT_LEFT,120,120);
	
	if( NULL == m_pNodeObj ) return TRUE;
	
	int nIndex = 0;
	CString str(_T(""));
	CNextNodeList *pNextObj = &(m_pNodeObj->m_NextNodeList);
	POSITION pos = pNextObj->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNextObj->GetNext(pos);
		if( NULL == pObj  ) continue;
	
		if( pObj->m_nType > 19 )
		{
			str.Format("%d", pObj->m_nType);
			m_listVar.InsertItem(nIndex, str );
			m_listVar.SetItemText(nIndex, 1, pObj->m_strName );
			m_listVar.SetItemText(nIndex, 2, GetTestTypeName( pObj->m_nType ) );
			m_listVar.SetItemText(nIndex, 3, pObj->m_strData );
			nIndex++;
		}
	
	}
	
     // Add the listbox buddy
     m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
     m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCompareProPage::OnButtonVariable() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strVariable =  str;
		UpdateData(FALSE);
	}

	return;
	
}




void CCompareProPage::InitPage(CCompareObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_strVariable  = m_pNodeObj->m_strVarName ;

	return;

}

CString CCompareProPage::GetTestTypeName(int nType)
{
	CString str(_T(""));
	switch(nType)
	{
	case 50:
		str = "==";
		break;
	case 51:
		str = "!=";
		break;
	case 52:
		str = "<";
		break;
	case 53:
		str = "<=";
		break;
	case 54:
		str = ">";
		break;
	case 55:
		str = ">=";
		break;
	}

	return str;
}

CCompareProPage::RemoveNextNode()
{
	CNextNodeList *pNextObj = &(m_pNodeObj->m_NextNodeList);
	POSITION pos = pNextObj->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNextObj->GetNext(pos);
		if( NULL == pObj  ) continue;
		if( pObj->m_nType > 19 ) m_pNodeObj->RemoveNextNodeObj(pObj);
	
	}

	return NULL;
}

void CCompareProPage::OnItemclickListVar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

LRESULT  CCompareProPage::OnNew( WPARAM wParam,LPARAM lParam)
{
	CEditCompareListDlg  dlg;
	dlg.m_pDoc = m_pDoc;
	CString str(_T(""));
	if(dlg.DoModal() == IDOK )
	{
		int nIndex = m_listVar.GetItemCount();
		str.Format("%d",dlg.m_nType);
		m_listVar.InsertItem(nIndex, str );
		m_listVar.SetItemText(nIndex, 1, dlg.m_strDscription );
		m_listVar.SetItemText(nIndex, 2, GetTestTypeName( dlg.m_nType ) );
		m_listVar.SetItemText(nIndex, 3, dlg.m_strExpression);
		


	}

	return 0;
}

LRESULT  CCompareProPage::OnEdit( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_listVar.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	CEditCompareListDlg  dlg;
	dlg.m_pDoc = m_pDoc;
	CString strType = m_listVar.GetItemText( nIndex, 2 );

	if( "==" == strType )
	{
		dlg.m_nType = 50;
	}
	else if( "!=" == strType )
	{
		dlg.m_nType = 51;
	}
	else if( "<" == strType )
	{
		dlg.m_nType = 52;
	}
	else if( "<=" == strType )
	{
		dlg.m_nType = 53;
	}
	else if( ">" == strType )
	{
		dlg.m_nType = 54;
	}
	else if( ">=" == strType )
	{
		dlg.m_nType = 55;
	}
	else
		return 0;

	dlg.m_strDscription = m_listVar.GetItemText( nIndex, 1 );
	dlg.m_strExpression = m_listVar.GetItemText( nIndex, 3 );

	CString str(_T(""));
	if(dlg.DoModal() == IDOK )
	{
		m_listVar.SetItemText(nIndex, 1, dlg.m_strDscription );
		m_listVar.SetItemText(nIndex, 2, GetTestTypeName( dlg.m_nType ) );
		m_listVar.SetItemText(nIndex, 3, dlg.m_strExpression);
		

	}

	return 0;
}
LRESULT  CCompareProPage::OnDel( WPARAM wParam,LPARAM lParam)
{
	
	POSITION pos = m_listVar.GetFirstSelectedItemPosition();

   while (pos)
   {
	  int nItem = m_listVar.GetNextSelectedItem(pos);
	  m_listVar.DeleteItem(nItem);
	  return 0;

   }

	return 0;
}

LRESULT  CCompareProPage::OnUP( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_listVar.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nAbove = m_listVar.GetNextItem( nIndex, LVNI_ABOVE ) ;
	if( -1 == nAbove )
		return 0;
	if( nIndex-1 > -1 )
	{
		
		m_listVar.InsertItem( nAbove,m_listVar.GetItemText(nIndex,0 ) );
		m_listVar.SetItemText(nAbove,1, m_listVar.GetItemText(nIndex + 1,1 ));//参数类型
		m_listVar.SetItemText(nAbove,2, m_listVar.GetItemText(nIndex + 1,2 ));//参数类型
		m_listVar.SetItemText(nAbove,3, m_listVar.GetItemText(nIndex + 1,3 ));//参数类型
		m_listVar.DeleteItem( nIndex + 1 );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_listVar.SetItemState(nAbove, flag,flag) ;


		UpdateData(FALSE);
	}	
	return 0;
}

LRESULT  CCompareProPage::OnDown( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_listVar.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nBelow = m_listVar.GetNextItem( nIndex, LVNI_BELOW ) ;
	if( -1 == nBelow )
		return 0;
	if( nIndex+1 < m_listVar.GetItemCount() )
	{
		
		m_listVar.InsertItem( nBelow + 1 ,m_listVar.GetItemText(nIndex,0 ) );
		m_listVar.SetItemText(nBelow + 1,1, m_listVar.GetItemText(nIndex,1 ));//参数类型
		m_listVar.SetItemText(nBelow + 1,2, m_listVar.GetItemText(nIndex,2 ));//参数类型
		m_listVar.SetItemText(nBelow + 1,3, m_listVar.GetItemText(nIndex,3 ));//参数类型
		m_listVar.DeleteItem( nIndex );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_listVar.SetItemState(nBelow, flag,flag) ;

		UpdateData(FALSE);
	}

	return 0;
}

void CCompareProPage::OnDblclkListVar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_listVar.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 != nIndex )
	{

		CEditCompareListDlg  dlg;
		dlg.m_pDoc = m_pDoc;
		CString strType = m_listVar.GetItemText( nIndex, 2 );

		if( "==" == strType )
		{
			dlg.m_nType = 50;
		}
		else if( "!=" == strType )
		{
			dlg.m_nType = 51;
		}
		else if( "<" == strType )
		{
			dlg.m_nType = 52;
		}
		else if( "<=" == strType )
		{
			dlg.m_nType = 53;
		}
		else if( ">" == strType )
		{
			dlg.m_nType = 54;
		}
		else if( ">=" == strType )
		{
			dlg.m_nType = 55;
		}
		else
			return ;

		dlg.m_strDscription = m_listVar.GetItemText( nIndex, 1 );
		dlg.m_strExpression = m_listVar.GetItemText( nIndex, 3 );
		
		CString str(_T(""));
		if(dlg.DoModal() == IDOK )
		{
			str.Format("%d",dlg.m_nType);
			m_listVar.SetItemText(nIndex, 0, str );
			m_listVar.SetItemText(nIndex, 1, dlg.m_strDscription );
			m_listVar.SetItemText(nIndex, 2, GetTestTypeName( dlg.m_nType ) );
			m_listVar.SetItemText(nIndex, 3, dlg.m_strExpression);
			

		}

	}
	
	*pResult = 0;
}


