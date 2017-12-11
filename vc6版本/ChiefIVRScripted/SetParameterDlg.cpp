// SetParameterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "SetParameterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetParameterDlg dialog


CSetParameterDlg::CSetParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetParameterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetParameterDlg)
	m_strName = _T("");
	m_strDescription = _T("");
	//}}AFX_DATA_INIT
}


void CSetParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetParameterDlg)
	DDX_Control(pDX, IDC_LIST_PARAMETER, m_ctrParamLst);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPT_VAR, m_strDescription);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetParameterDlg, CDialog)
	//{{AFX_MSG_MAP(CSetParameterDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARAMETER, OnClickListParameter)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetParameterDlg message handlers

BOOL CSetParameterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ctrParamLst.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_ctrParamLst.InsertColumn(0,"Key",LVCFMT_LEFT,150,100);
	m_ctrParamLst.InsertColumn(1,"Name",LVCFMT_LEFT,80,100);
	m_ctrParamLst.InsertColumn(2,"Description",LVCFMT_LEFT,200,200);
	CString str("");

	if( NULL != m_pDoc ) 
	{
		POSITION pos = m_pDoc->m_ParameterLst.GetHeadPosition( );

		int i = 0;
		while (pos)
		{
			CParameter * pVar = m_pDoc->m_ParameterLst.GetNext(pos);
			if( NULL != pVar )
			{
				str.Format("%d",pVar->m_nKey) ;
				m_ctrParamLst.InsertItem(i,str );
				m_ctrParamLst.SetItemData(i,pVar->m_nKey);
				m_ctrParamLst.SetItemText(i,1, pVar->m_strName  );
				m_ctrParamLst.SetItemText(i,2, pVar->m_strDescription );
			}
			i++;
		}
	}



	 m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
	 m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetParameterDlg::Init(CDrawDoc *pDoc)
{
	if( NULL == pDoc ) return;
	m_pDoc = pDoc;

	return;
}

LRESULT CSetParameterDlg::OnNew( WPARAM wParam,LPARAM lParam)
{
	ASSERT_VALID(m_pDoc);
	UpdateData(TRUE);

	if( "" == m_strName  ) 
	{
		MessageBox("Please check input!");
		return 0;
	}
	if( ParamIsExit(m_strName) )
	{
		MessageBox("It Has the identical parameter,Name again please!");
		return 0;
	}
	int nIndex = m_ctrParamLst.GetItemCount();
	m_pDoc->m_nParamBaseKey = m_pDoc->m_nParamBaseKey + 1;
	CString str("");
	str.Format("%d",m_pDoc->m_nParamBaseKey);
	m_ctrParamLst.InsertItem(nIndex,str);
	m_ctrParamLst.SetItemData( nIndex, m_pDoc->m_nParamBaseKey );
	m_ctrParamLst.SetItemText(nIndex,1, m_strName);
	m_ctrParamLst.SetItemText(nIndex,2, m_strDescription );

	return 0;
}

LRESULT CSetParameterDlg::OnEdit( WPARAM wParam,LPARAM lParam)
{
	ASSERT_VALID(m_pDoc);
	UpdateData(TRUE);

	int nIndex = m_ctrParamLst.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	if( "" == m_strName  ) 
	{
		MessageBox("Please check input!");
		return 0;
	}
	if( ParamIsExit(m_strName) )
	{
		if( m_strName != m_ctrParamLst.GetItemText(nIndex,0) )
		{
			MessageBox("It Has the identical parameter,Name again please!");
			return 0;
		}
	}

	m_ctrParamLst.SetItemText(nIndex,1, m_strName );
	m_ctrParamLst.SetItemText(nIndex,2, m_strDescription );

	return 0;
}

LRESULT CSetParameterDlg::OnDel( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_ctrParamLst.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_ctrParamLst.DeleteItem(nIndex);

	return 0;
}

LRESULT CSetParameterDlg::OnUP( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_ctrParamLst.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nAbove = m_ctrParamLst.GetNextItem( nIndex, LVNI_ABOVE ) ;
	if( -1 == nAbove )
		return 0;
	if( nIndex-1 > -1 )
	{
		
		m_ctrParamLst.InsertItem( nAbove,m_ctrParamLst.GetItemText(nIndex,0 ) );
		m_ctrParamLst.SetItemText(nAbove,1, m_ctrParamLst.GetItemText(nIndex + 1,1 ));//参数类型
		m_ctrParamLst.SetItemText(nAbove,2, m_ctrParamLst.GetItemText(nIndex + 1,2 ));//参数类型
		m_ctrParamLst.DeleteItem( nIndex + 1 );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_ctrParamLst.SetItemState(nAbove, flag,flag) ;

		m_strName = m_ctrParamLst.GetItemText(nAbove,1) ;
		m_strDescription = m_ctrParamLst.GetItemText(nAbove,2) ;

		UpdateData(FALSE);
	}

	return 0;
}

LRESULT CSetParameterDlg::OnDown( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_ctrParamLst.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nBelow = m_ctrParamLst.GetNextItem( nIndex, LVNI_BELOW ) ;
	if( -1 == nBelow )
		return 0;
	if( nIndex+1 < m_ctrParamLst.GetItemCount() )
	{
		
		m_ctrParamLst.InsertItem( nBelow + 1 ,m_ctrParamLst.GetItemText(nIndex,0 ) );
		m_ctrParamLst.SetItemText(nBelow + 1,1, m_ctrParamLst.GetItemText(nIndex,1 ));//参数类型
		m_ctrParamLst.SetItemText(nBelow + 1,2, m_ctrParamLst.GetItemText(nIndex,2 ));//参数类型
		m_ctrParamLst.DeleteItem( nIndex );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_ctrParamLst.SetItemState(nBelow, flag,flag) ;

		m_strName = m_ctrParamLst.GetItemText(nBelow,1) ;
		m_strDescription = m_ctrParamLst.GetItemText(nBelow,2);

		UpdateData(FALSE);
	}

	
	return 0;


}

BOOL CSetParameterDlg::ParamIsExit(CString strName)
{
	int nCount = m_ctrParamLst.GetItemCount();
	CString str("");
	for( int i =0; i < nCount; i++ )
	{
		if( strName == m_ctrParamLst.GetItemText(i,1) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CSetParameterDlg::OnClickListParameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT_VALID(m_pDoc);
	UpdateData(TRUE);
	int nIndex = m_ctrParamLst.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;

	m_strName = m_ctrParamLst.GetItemText(nIndex,1);
	m_strDescription = m_ctrParamLst.GetItemText(nIndex,2);


	UpdateData(FALSE);

	*pResult = 0;
}

void CSetParameterDlg::OnOK() 
{
	ASSERT_VALID(m_pDoc);
	m_pDoc->RemoveAllParameter();
	
	int nCount = m_ctrParamLst.GetItemCount();
	CString str("");
	for( int i =0; i < nCount; i++ )
	{
		CParameter *pNewObj = new CParameter;
		if( NULL == pNewObj ) return;
		pNewObj->m_strName = m_ctrParamLst.GetItemText(i,1);
		pNewObj->m_nKey = m_ctrParamLst.GetItemData(i);
		pNewObj->m_strDescription = m_ctrParamLst.GetItemText(i,2);
		m_pDoc->m_ParameterLst.AddTail( pNewObj );
	}


	CDialog::OnOK();
}
