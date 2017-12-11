// DLLCallPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "DLLCallPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLLCallPageSpc property page

IMPLEMENT_DYNCREATE(CDLLCallPageSpc, CPropertyPage)

CDLLCallPageSpc::CDLLCallPageSpc() : CPropertyPage(CDLLCallPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CDLLCallPageSpc)
	m_strFunction = _T("");
	m_nParmType = 0;
	m_strParmVar = _T("");
	//}}AFX_DATA_INIT
	m_strDLL = _T("");
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CDLLCallPageSpc::~CDLLCallPageSpc()
{
}

void CDLLCallPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLLCallPageSpc)
	DDX_Control(pDX, IDC_COMBO_DLL, m_cobDLL);
	DDX_Control(pDX, IDC_LIST_PARAMETER, m_lstParam);
	DDX_Text(pDX, IDC_EDIT_FUNCTION, m_strFunction);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nParmType);
	DDX_Text(pDX, IDC_EDIT_PARMVARIABLE, m_strParmVar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLLCallPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CDLLCallPageSpc)
	ON_BN_CLICKED(IDC_BUTTON_VAR, OnButtonVar)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARAMETER, OnClickListParameter)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLLCallPageSpc message handlers

void CDLLCallPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;

	UpdateData(TRUE);

	
	m_pNodeObj->m_strFunction = m_strFunction;


	if( m_cobDLL.GetCurSel( ) > -1 ) 
		m_cobDLL.GetLBText( m_cobDLL.GetCurSel( ), m_pNodeObj->m_strDLL );
	
	m_pNodeObj->m_nDLLKey = m_pNodeObj->GetVarKey(m_pNodeObj->m_strDLL);
	if( "#" == m_pNodeObj->m_strDLL.Left(1) )
	{
		m_pNodeObj->m_nHandleType = 1;
	}
	else
	{
		m_pNodeObj->m_nHandleType = 0;
	}

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
		pVar->m_strData = m_lstParam.GetItemText(i,0) ;

		if( "OutPut" == m_lstParam.GetItemText(i,1) )
			pVar->m_nDataType =0;
		else
			pVar->m_nDataType =1;
		
		pVar->m_nKey =  m_pNodeObj->GetVarKey( pVar->m_strData );
		pDadaList->AddTail(pVar);

	}


	
	CPropertyPage::OnOK();
}

BOOL CDLLCallPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_lstParam.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstParam.InsertColumn(0,"Variable",LVCFMT_LEFT,100,100);
	m_lstParam.InsertColumn(1,"Parameter Type ",LVCFMT_LEFT,200,200);

	if( NULL == m_pNodeObj || NULL == m_pDoc ) return TRUE;
	
	CString str(_T(""));
	POSITION pos = m_pDoc->m_DLLVariableList.GetHeadPosition() ;
	while( pos )
	{
		
		CDLLVariable * pVar =m_pDoc->m_DLLVariableList.GetNext(pos) ;
		if( pVar != NULL )
		{
			if( pVar->m_nKey > 100 )
			{
				str.Format("@%d:%s", pVar->m_nKey, pVar->m_strName );
				m_cobDLL.AddString( str );
			}
		}
		
	}

	m_cobDLL.AddString( "#59:SysDLLHandle1" );
	m_cobDLL.AddString( "#60:SysDLLHandle2" );
	m_cobDLL.AddString( "#61:SysDLLHandle3" );
	m_cobDLL.AddString( "#62:SysDLLHandle4" );
	m_cobDLL.AddString( "#63:SysDLLHandle5" );
	m_cobDLL.AddString( "#64:SysDLLHandle6" );
	m_cobDLL.AddString( "#65:SysDLLHandle7" );
	m_cobDLL.AddString( "#66:SysDLLHandle8" );

	m_cobDLL.SelectString(-1, m_strDLL);



	CNodeDataList *pDadaList = &(m_pNodeObj->m_NodeDataList);
	pos = pDadaList->GetHeadPosition( );
	
	//下面开始初始化数据
	int i = 0;
	CNodeData *pData = NULL;
	while( pos )
	{
		pData = pDadaList->GetNext(pos);
		if( NULL != pData )
		{
			m_lstParam.InsertItem( i, pData->m_strData );//参数变量
			if( 0 == pData->m_nDataType )
				m_lstParam.SetItemText(i,1,"OutPut" );//参数类型
			else
				m_lstParam.SetItemText(i,1,"InPut" );//参数类型

			i++;
		}

	}

	 m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
	 m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLLCallPageSpc::InitPage(CDLLCallObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;

	m_strDLL		= m_pNodeObj->m_strDLL;
	m_strFunction	= m_pNodeObj->m_strFunction;


	return;
}

void CDLLCallPageSpc::OnButtonVar() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strParmVar =  str;
		UpdateData(FALSE);
	}
	
	return;
	
}



LRESULT CDLLCallPageSpc::OnNew( WPARAM wParam,LPARAM lParam)
{
	this->UpdateData(TRUE);

	if( "" == m_strParmVar ) 
	{
		MessageBox("Please check input!");
		return 0;
	}

	int nIndex = m_lstParam.GetItemCount();
	CString str("");
	m_lstParam.InsertItem(nIndex, m_strParmVar );

	if( 0 == m_nParmType )
		m_lstParam.SetItemText(nIndex,1,"OutPut" );//参数类型
	else
		m_lstParam.SetItemText(nIndex,1,"InPut" );//参数类型

	return 0;
}
LRESULT CDLLCallPageSpc::OnEdit( WPARAM wParam,LPARAM lParam)
{
	this->UpdateData(TRUE);

	if( "" == m_strParmVar ) 
	{
		MessageBox("Please check input!");
		return 0;
	}

	int nIndex = m_lstParam.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	CString str("");
	m_lstParam.InsertItem(nIndex, m_strParmVar );

	if( 0 == m_nParmType )
		m_lstParam.SetItemText(nIndex,1,"OutPut" );//参数类型
	else
		m_lstParam.SetItemText(nIndex,1,"InPut" );//参数类型

	return 0;
}

LRESULT CDLLCallPageSpc::OnDel( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_lstParam.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_lstParam.DeleteItem(nIndex);

	return 0;
}

LRESULT CDLLCallPageSpc::OnUP( WPARAM wParam,LPARAM lParam)
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

		m_strParmVar = m_lstParam.GetItemText(nAbove,0) ;
		if( "OutPut" == m_lstParam.GetItemText(nAbove,1) )
			m_nParmType =0;
		else
			m_nParmType =1;
		UpdateData(FALSE);
	}
	return 0;
}
LRESULT CDLLCallPageSpc::OnDown( WPARAM wParam,LPARAM lParam)
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

		m_strParmVar = m_lstParam.GetItemText(nBelow,0) ;
		if( "OutPut" == m_lstParam.GetItemText(nBelow,1)  )
			m_nParmType =0;
		else
			m_nParmType =1;

		UpdateData(FALSE);
	}
	
	return 0;
}

void CDLLCallPageSpc::OnClickListParameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstParam.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;
	m_strParmVar = m_lstParam.GetItemText(nIndex,0);
	if( "OutPut" == m_lstParam.GetItemText(nIndex,1) )
		m_nParmType =0;
	else
		m_nParmType =1;
	UpdateData(FALSE);
	*pResult = 0;
}
