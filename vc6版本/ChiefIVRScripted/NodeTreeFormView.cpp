// NodeTreeFormView.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "NodeTreeFormView.h"
#include "AddNormalVariable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeTreeFormView

CNodeTreeFormView *g_pNodeTreeFormView = NULL;

IMPLEMENT_DYNCREATE(CNodeTreeFormView, CFormView)

CNodeTreeFormView::CNodeTreeFormView()
	: CFormView(CNodeTreeFormView::IDD)
{
	//{{AFX_DATA_INIT(CNodeTreeFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDrawView = NULL;
	g_pNodeTreeFormView = this;
	m_InitialUpdate=0;
	m_cy=0;
	m_cx=0;
}

CNodeTreeFormView::~CNodeTreeFormView()
{
}

void CNodeTreeFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeTreeFormView)
	DDX_Control(pDX, IDC_NODE_TREE, m_nodeTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNodeTreeFormView, CFormView)
	//{{AFX_MSG_MAP(CNodeTreeFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_NODE_TREE, OnClickNodeTree)
	ON_NOTIFY(NM_DBLCLK, IDC_NODE_TREE, OnDblclkNodeTree)
	ON_NOTIFY(NM_RCLICK, IDC_NODE_TREE, OnRclickNodeTree)
	ON_NOTIFY(NM_RDBLCLK, IDC_NODE_TREE, OnRdblclkNodeTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_NODE_TREE, OnSelchangingNodeTree)
	ON_COMMAND(ID_NORMAL_VARIABLE, OnNormalVariable)
	ON_COMMAND(ID_DATABASE_VARIABLE, OnDatabaseVariable)
	ON_COMMAND(ID_DLL_HADLE, OnDllHadle)
	ON_NOTIFY(TVN_SELCHANGED, IDC_NODE_TREE, OnSelchangedNodeTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeTreeFormView diagnostics

#ifdef _DEBUG
void CNodeTreeFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNodeTreeFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNodeTreeFormView message handlers

void CNodeTreeFormView::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{

		m_nodeTree.MoveWindow(0,0,m_cx,m_cy);

	}
	
}

void CNodeTreeFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;
		SetNodeImageList( &m_ImageList );
		
		m_nodeTree.SetImageList(&m_ImageList,TVSIL_NORMAL);

		
		m_nodeTree.MoveWindow(0,0,m_cx,m_cy);
	}
	
}



void CNodeTreeFormView::DeleteItem(int nItemKey )
{
	HTREEITEM hItem = FindItemFromItemData(nItemKey, NULL) ;
	if( NULL == hItem )
		return;
	m_nodeTree.DeleteItem( hItem );

	return;
}

void CNodeTreeFormView::DeleteAllItems()
{
	m_nodeTree.DeleteAllItems();
	return;
}

void CNodeTreeFormView::InserRootItem( int nDocType )
{
	HTREEITEM hItem  = NULL;
	hItem  = m_nodeTree.InsertItem( "Script",0,0, TVI_ROOT, TVI_LAST );
	m_nodeTree.SetItemData(hItem,1);
	hItem  = m_nodeTree.InsertItem( "Normal Variable",0,0, TVI_ROOT, TVI_LAST );
	m_nodeTree.SetItemData(hItem,2);
	hItem  = m_nodeTree.InsertItem( "DataBase Variable",0,0, TVI_ROOT, TVI_LAST );
	m_nodeTree.SetItemData(hItem,3);
	hItem  = m_nodeTree.InsertItem( "DLL Handle",0,0, TVI_ROOT, TVI_LAST );
	m_nodeTree.SetItemData(hItem,4);
	if( 0 == nDocType )
	{
		hItem  = m_nodeTree.InsertItem( "EndCall Handling",0,0, TVI_ROOT, TVI_LAST );
		m_nodeTree.SetItemData(hItem,5);
	}
}

void CNodeTreeFormView::InsertItem( int nparent ,int nItemKey, LPCTSTR lpszItem, UINT nImage, UINT nSelectedImage)
{
	if( nparent < 1 && nparent > 4  ) return;
	HTREEITEM hItem1 = NULL;
	HTREEITEM hItem2 = NULL;
	HTREEITEM hItem3 = NULL;
	HTREEITEM hItem4 = NULL;
	HTREEITEM ParentItemNode = NULL;

	

	hItem1 = m_nodeTree.GetRootItem( );
	hItem2 = m_nodeTree.GetNextItem( hItem1,TVGN_NEXT );
	hItem3 = m_nodeTree.GetNextItem( hItem2,TVGN_NEXT );
	hItem4 = m_nodeTree.GetNextItem( hItem3,TVGN_NEXT );

	if( 1 == nparent )
	{
		if( !hItem1 ) return;
			ParentItemNode = hItem1;
	}
	else if( 2 == nparent )
	{
		if( !hItem2 ) return;
		ParentItemNode = hItem2;
	}
	else if( 3 == nparent )
	{
		if( !hItem3 ) return;
		ParentItemNode = hItem3;
	}
	else if( 4 == nparent )
	{
		if( !hItem4 ) return;
		ParentItemNode = hItem4;
	}


	HTREEITEM hItem = m_nodeTree.InsertItem( lpszItem, GetImageIndex(nImage) ,GetImageIndex(nSelectedImage), ParentItemNode, TVI_LAST );
	if( !m_nodeTree.SetItemData(hItem,nItemKey) )
		AfxMessageBox("Set tree itemdata false");
	m_nodeTree.Expand( ParentItemNode, TVE_EXPAND );




	return;

}
void CNodeTreeFormView::UpdateTreeItem(LPCTSTR lpszNewText , int nItemKey )
{

	HTREEITEM hItem = FindItemFromItemData(nItemKey, NULL) ;
	if( NULL == hItem )
		return;
	m_nodeTree.SetItemText( hItem, lpszNewText );


	return ;
}




void CNodeTreeFormView::OnClickNodeTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
}

void CNodeTreeFormView::OnDblclkNodeTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if( NULL == m_pDrawView  ) return;
	m_pDrawView->EditNodeObjProperty();
	Invalidate();


	*pResult = 0;
}

void CNodeTreeFormView::OnRclickNodeTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CNodeTreeFormView::OnRdblclkNodeTree(NMHDR* pNMHDR, LRESULT* pResult) 
{

	
	*pResult = 0;
}



void CNodeTreeFormView::OnSelchangingNodeTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;


	
	*pResult = 0;
}


void CNodeTreeFormView::OnNormalVariable() 
{
	CAddNormalVariable dlg;

	if( dlg.DoModal() == IDOK )
	{
		
	}
	
}

void CNodeTreeFormView::OnDatabaseVariable() 
{
	// TODO: Add your command handler code here
	
}

void CNodeTreeFormView::OnDllHadle() 
{
	// TODO: Add your command handler code here
	
}


void CNodeTreeFormView::InsertBlockItem(int nBlockKey, int nItemKey, LPCTSTR lpszItem, UINT nImage, UINT nSelectedImage)
{
	HTREEITEM hBlockItem = FindItemFromItemData(nBlockKey, NULL) ;
	if( NULL == hBlockItem )
		return;

	HTREEITEM hItem = m_nodeTree.InsertItem( lpszItem, GetImageIndex(nImage) ,GetImageIndex(nSelectedImage), hBlockItem, TVI_LAST );
	if( !m_nodeTree.SetItemData(hItem,nItemKey) )
		AfxMessageBox("Set tree itemdata false");
	m_nodeTree.Expand( hBlockItem, TVE_EXPAND );

	return;
}

HTREEITEM CNodeTreeFormView::FindItemFromItemData(DWORD ID, HTREEITEM TreeItem) 
{ 
	 DWORD ItemData; 
	 HTREEITEM NextItem; 
	 HTREEITEM ResultItem = NULL; 
	 if (TreeItem == NULL) 
	  TreeItem = m_nodeTree.GetRootItem(); 


	 ItemData = m_nodeTree.GetItemData(TreeItem); 
	 if (ItemData == ID) 
	  return TreeItem; 


	 NextItem = m_nodeTree.GetNextItem(TreeItem, TVGN_CHILD); 
	 if (NextItem != NULL) 
	 { 
		  ResultItem = FindItemFromItemData(ID, NextItem); 
		  if (ResultItem != NULL) 
				return ResultItem; 
	 }


	 NextItem = m_nodeTree.GetNextItem(TreeItem, TVGN_NEXT); 
	 if (NextItem != NULL) 
			ResultItem = FindItemFromItemData(ID, NextItem); 
	 return ResultItem; 

} 

void CNodeTreeFormView::InsertEndCallBlockItem(int nItemKey, LPCTSTR lpszItem, UINT nImage, UINT nSelectedImage)
{
	HTREEITEM hItem1 = NULL;
	HTREEITEM hItem2 = NULL;
	HTREEITEM hItem3 = NULL;
	HTREEITEM hItem4 = NULL;
	HTREEITEM hItem5 = NULL;
	HTREEITEM ParentItemNode = NULL;

	hItem1 = m_nodeTree.GetRootItem( );
	hItem2 = m_nodeTree.GetNextItem( hItem1,TVGN_NEXT );
	hItem3 = m_nodeTree.GetNextItem( hItem2,TVGN_NEXT );
	hItem4 = m_nodeTree.GetNextItem( hItem3,TVGN_NEXT );
	hItem5 = m_nodeTree.GetNextItem( hItem4,TVGN_NEXT );

	if( NULL != hItem5 )
	{
		HTREEITEM hItem = m_nodeTree.InsertItem( lpszItem, GetImageIndex(nImage) ,GetImageIndex(nSelectedImage), hItem5, TVI_LAST );
		if( !m_nodeTree.SetItemData(hItem,nItemKey) )
			AfxMessageBox("Set tree itemdata false");
		m_nodeTree.Expand( hItem5, TVE_EXPAND );
	}
	return;
}

void CNodeTreeFormView::OnSelchangedNodeTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( NULL == m_pDrawView  ) return;
	BOOL bResult = FALSE;
	HTREEITEM hItem = m_nodeTree.GetSelectedItem();
	if( NULL == hItem  ) return;
	HTREEITEM hParentItem = m_nodeTree.GetParentItem(hItem);
	if( NULL == hParentItem ) return;
	int nParentKey = m_nodeTree.GetItemData(hParentItem);
	if( nParentKey >= 2 && nParentKey <= 4 ) return;
    int nItemKey = m_nodeTree.GetItemData(hItem);
	CNodeObj * pObj1 = m_pDrawView->FindNodeObj(nItemKey);
	//当找到时
	if( NULL != pObj1 )
	{
		HTREEITEM hItem1 = m_nodeTree.GetParentItem(hItem);
		if( NULL != hItem1 )
		{
			int nItemKey2 = m_nodeTree.GetItemData(hItem1);
			CNodeObj * pObj2 = pObj2 = m_pDrawView->FindNodeObj(nItemKey2);
			if( NULL != pObj2 )
			{
				if(  NODETYPE_BLOCK == pObj2->m_nType )
				{
					m_pDrawView->m_nShowMode = 1;
					m_pDrawView->m_pBlockObj = (CBlockObj *)pObj2;

					m_pDrawView->SetScrollPos(SB_HORZ,0,TRUE);
					m_pDrawView->SetScrollPos(SB_VERT,0,TRUE);
					m_pDrawView->Invalidate(TRUE);
					m_pDrawView->ShowFindObjToSelected(pObj1);
					

				}
				else
				{
					m_pDrawView->ShowFindObjToSelected(pObj1);
					
					m_pDrawView->m_nShowMode = 0;
					m_pDrawView->m_pBlockObj = NULL;
				}

			}
			else
			{
				if( 5 == nItemKey2 )
				{
					
					m_pDrawView->m_nShowMode = 2;
					m_pDrawView->SetScrollPos(SB_HORZ,0,TRUE);
					m_pDrawView->SetScrollPos(SB_VERT,0,TRUE);
					m_pDrawView->Invalidate(TRUE);
					m_pDrawView->ShowFindObjToSelected(pObj1);
				}
				else
				{
					m_pDrawView->ShowFindObjToSelected(pObj1);
					
					m_pDrawView->m_nShowMode = 0;
					m_pDrawView->m_pBlockObj = NULL;
				}
			}
		}
		else
		{
			m_pDrawView->ShowFindObjToSelected(pObj1);
			
			m_pDrawView->m_nShowMode = 0;
			m_pDrawView->m_pBlockObj = NULL;
		}

	}

	*pResult = 0;
}

void CNodeTreeFormView::DeleteTreeNode( void )
{

	if( NULL == m_pDrawView  ) return;
	BOOL bResult = FALSE;
	HTREEITEM hItem = m_nodeTree.GetSelectedItem();
	if( NULL == hItem  ) return;
	HTREEITEM hItem1 = m_nodeTree.GetParentItem(hItem);
	if( NULL == hItem1  ) return;
    int nItemKey = m_nodeTree.GetItemData(hItem);
	int nItemKey1 = m_nodeTree.GetItemData(hItem1);
	if( nItemKey1 > 1 && nItemKey1 < 5 ) return;

	CNodeObj * pObj1 = m_pDrawView->FindNodeObj(nItemKey);
	if( NODETYPE_START == pObj1->m_nType ) return;
	if( NODETYPE_MOUDLE_BEGIN == pObj1->m_nType ) return;
	//当找到时
	if( NULL != pObj1 ) 
	{
		m_pDrawView->Select(pObj1, FALSE);
		m_pDrawView->DeleteTreeNodeSelected();

	}
	return;
}
