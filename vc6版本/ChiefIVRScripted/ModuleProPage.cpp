// ModuleProPage.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "DrawDoc.h"
#include "ModuleProPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModuleProPage property page

IMPLEMENT_DYNCREATE(CModuleProPage, CPropertyPage)

CModuleProPage::CModuleProPage() : CPropertyPage(CModuleProPage::IDD)
{
	//{{AFX_DATA_INIT(CModuleProPage)
	m_strModuleFile = _T("");
	m_strVariable = _T("");
	//}}AFX_DATA_INIT
}

CModuleProPage::~CModuleProPage()
{
}

void CModuleProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModuleProPage)
	DDX_Control(pDX, IDC_LIST_MODULEEND, m_lstModuleEnd);
	DDX_Control(pDX, IDC_LIST_PARAMETER, m_ctrParamLst);
	DDX_Text(pDX, IDC_EDIT_MODULE_FILE, m_strModuleFile);
	DDX_Text(pDX, IDC_EDIT_VAR, m_strVariable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModuleProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CModuleProPage)
	ON_BN_CLICKED(IDC_BUTTON_MODULFILE, OnButtonModulfile)
	ON_BN_CLICKED(IDC_BUTTON_VAR, OnButtonVar)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARAMETER, OnClickListParameter)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleProPage message handlers

void CModuleProPage::OnButtonModulfile() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	
	char szFilters[]="Module Excute script(*.cmf)|*.cmf||";	 			  
	CFileDialog Dlg(TRUE,"","",OFN_FILEMUSTEXIST, szFilters, this);

	if(Dlg.DoModal()==IDOK)
	{
		m_strModuleFile = Dlg.GetPathName();
		CModuleDocument doc;
		if( doc.OnOpenDocument( m_strModuleFile ) )
		{
			m_ctrParamLst.DeleteAllItems( );
			CParameterList *pList = &(doc.m_ModuleObj.m_ParameterLst);
			if( NULL == pList ) return;
			POSITION pos = pList->GetHeadPosition( );
			int i = 0;
			while (pos)
			{
				CParameter * pVar = pList->GetNext(pos);
				if( NULL != pVar )
				{
					m_ctrParamLst.InsertItem(i, pVar->m_strName );
					m_ctrParamLst.SetItemData( i, pVar->m_nKey );
					m_ctrParamLst.SetItemText(i,1, pVar->m_strVariable  );
					m_ctrParamLst.SetItemText(i,2, pVar->m_strDescription );
				}
				i++;
			}
			
			//处理ModuleEnd 列表
			CNodeObjList *pObjList =  &(doc.m_ModuleObj.m_objects);
			if( NULL == pObjList ) return;
			int nCount = m_lstModuleEnd.GetItemCount();
			while( nCount > 2 )
			{
				m_lstModuleEnd.DeleteItem(2);
				nCount = m_lstModuleEnd.GetItemCount();
			}
			

			pos = pObjList->GetHeadPosition( );
			i = 2;
			while (pos)
			{
				CNodeObj * pVar = pObjList->GetNext(pos);
				if( NULL != pVar )
				{
					if( NODETYPE_MOUDLE_END == pVar->m_nType )
					{
						m_lstModuleEnd.InsertItem(i, pVar->m_strCaption );
						m_lstModuleEnd.SetItemData(i,pVar->m_nKey );
						i++;
					}
				}
				
			}

		}
	}

	UpdateData(FALSE);
	return;	
}

void CModuleProPage::InitPage(CModuleObj *pObj, CDrawDoc *pDoc)
{

	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;
	m_strModuleFile = m_pNodeObj->m_strFile;


	return;

}

LRESULT CModuleProPage::OnEdit( WPARAM wParam,LPARAM lParam)
{
	ASSERT_VALID(m_pDoc);
	UpdateData(TRUE);

	int nIndex = m_ctrParamLst.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	if( "" == m_strVariable ) 
	{
		MessageBox("Please check input!");
		return 0;
	}

	m_ctrParamLst.SetItemText(nIndex,1, m_strVariable  );

	return 0;
}


LRESULT CModuleProPage::OnUP( WPARAM wParam,LPARAM lParam)
{

	return 0;
}

LRESULT CModuleProPage::OnDown( WPARAM wParam,LPARAM lParam)
{

	return 0;
}

void CModuleProPage::OnOK() 
{
	m_pNodeObj->m_strFile = m_strModuleFile;
	m_pNodeObj->RemoveParameterList();
	int nCount = m_ctrParamLst.GetItemCount();
	CString str("");
	for( int i =0; i < nCount; i++ )
	{
		CParameter *pNewObj = new CParameter;
		if( NULL == pNewObj ) return;
		pNewObj->m_strName = m_ctrParamLst.GetItemText(i,0);
		pNewObj->m_strVariable = m_ctrParamLst.GetItemText(i,1);
		pNewObj->m_nKey = m_ctrParamLst.GetItemData( i );
		pNewObj->m_strDescription = m_ctrParamLst.GetItemText(i,2);
		m_pNodeObj->m_ParameterLst.AddTail( pNewObj );
	}

	m_pNodeObj->RemoveAllNextNode();
	nCount = m_lstModuleEnd.GetItemCount();
	for( i = 0 ; i < nCount; i++ )
	{
		m_pNodeObj->AddNextNodeObj( m_lstModuleEnd.GetItemText(i,0), "", DATATYPE_INTEGER, m_lstModuleEnd.GetItemData(i), i+1 );
	}

	CPropertyPage::OnOK();
}

BOOL CModuleProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_ctrParamLst.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_ctrParamLst.InsertColumn(0,"Name",LVCFMT_LEFT,80,100);
	m_ctrParamLst.InsertColumn(1,"Variable",LVCFMT_LEFT,150,100);
	m_ctrParamLst.InsertColumn(2,"Description",LVCFMT_LEFT,200,200);

	m_strModuleFile = m_pNodeObj->m_strFile;
	POSITION pos = m_pNodeObj->m_ParameterLst.GetHeadPosition( );
	int i = 0;
	while (pos)
	{
		CParameter * pVar = m_pNodeObj->m_ParameterLst.GetNext(pos);
		if( NULL != pVar )
		{
			m_ctrParamLst.InsertItem(i, pVar->m_strName );
			m_ctrParamLst.SetItemData( i, pVar->m_nKey );
			m_ctrParamLst.SetItemText(i,1, pVar->m_strVariable  );
			m_ctrParamLst.SetItemText(i,2, pVar->m_strDescription );
		}
		i++;
	}

	m_lstModuleEnd.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstModuleEnd.InsertColumn(0,"Name",LVCFMT_LEFT,120,120);
	pos = m_pNodeObj->m_NextNodeList.GetHeadPosition( );
	i = 0;
	while (pos)
	{
		CNextNode * pVar = m_pNodeObj->m_NextNodeList.GetNext(pos);
		if( NULL != pVar )
		{
			m_lstModuleEnd.InsertItem(i, pVar->m_strName );
			m_lstModuleEnd.SetItemData(i,pVar->m_nType );
		}
		i++;
	}


	 m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
	 m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	 UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModuleProPage::OnButtonVar() 
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

void CModuleProPage::OnClickListParameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT_VALID(m_pDoc);
	UpdateData(TRUE);
	int nIndex = m_ctrParamLst.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;

	m_strVariable = m_ctrParamLst.GetItemText(nIndex,1);

	UpdateData(FALSE);
	
	*pResult = 0;
}
