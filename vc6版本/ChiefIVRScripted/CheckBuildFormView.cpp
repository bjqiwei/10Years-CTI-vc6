// CheckBuildFormView.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "CheckBuildFormView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCheckBuildFormView *g_pCheckBuildForm = NULL;
/////////////////////////////////////////////////////////////////////////////
// CCheckBuildFormView

IMPLEMENT_DYNCREATE(CCheckBuildFormView, CFormView)

CCheckBuildFormView::CCheckBuildFormView()
	: CFormView(CCheckBuildFormView::IDD)
{
	//{{AFX_DATA_INIT(CCheckBuildFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_InitialUpdate=0;
	m_cy=0;
	m_cx=0;
	g_pCheckBuildForm = this;
	m_pDrawView = NULL;
}

CCheckBuildFormView::~CCheckBuildFormView()
{
}

void CCheckBuildFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckBuildFormView)
	DDX_Control(pDX, IDC_CHECK_TREE, m_checkTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckBuildFormView, CFormView)
	//{{AFX_MSG_MAP(CCheckBuildFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_CHECK_TREE, OnClickCheckTree)
	ON_NOTIFY(NM_DBLCLK, IDC_CHECK_TREE, OnDblclkCheckTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_CHECK_TREE, OnSelchangedCheckTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCheckBuildFormView diagnostics

#ifdef _DEBUG
void CCheckBuildFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCheckBuildFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCheckBuildFormView message handlers

void CCheckBuildFormView::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{

		m_checkTree.MoveWindow(0,0,m_cx,m_cy);

	}
	
}

void CCheckBuildFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;

		HICON hIcon[5];

		m_ImageList.Create(16,16,0,3,3);
		hIcon[0] = AfxGetApp()->LoadIcon(IDI_TREE_ROOT);
		hIcon[1] = AfxGetApp()->LoadIcon(IDI_CHECK_NODE);
		hIcon[2] = AfxGetApp()->LoadIcon(IDI_CHECK_ERROR);
		hIcon[3] = AfxGetApp()->LoadIcon(IDI_CHECK_WARN);
		hIcon[4] = AfxGetApp()->LoadIcon(IDI_CHECK_NOTIFY);
		

		for(int i = 0 ; i < 5 ; i++)
		{ 
			m_ImageList.Add(hIcon[i]);
		}
		m_checkTree.SetImageList(&m_ImageList,TVSIL_NORMAL);


		m_checkTree.MoveWindow(0,0,m_cx,m_cy);
	}
	
	return;
}

void CCheckBuildFormView::InsertErrorItem(LPCTSTR lpszItem, HTREEITEM hParent)
{
	m_checkTree.InsertItem( lpszItem, 2 ,2, hParent, TVI_LAST );
	m_checkTree.Expand( hParent, TVE_EXPAND );
	
	return;
}

void CCheckBuildFormView::InsertWarningItem(LPCTSTR lpszItem, HTREEITEM hParent)
{
	m_checkTree.InsertItem( lpszItem, 4 ,4, hParent, TVI_LAST );
	m_checkTree.Expand( hParent, TVE_EXPAND );
	
	return;
}

HTREEITEM CCheckBuildFormView::InsertNodeItem(LPCTSTR lpszItem, int nItemKey ,HTREEITEM &hParent )
{
	if( NULL == hParent )
	{
		hParent = m_checkTree.GetRootItem();
	}
	HTREEITEM item = m_checkTree.InsertItem( lpszItem, 1 ,1, hParent, TVI_LAST );
	m_checkTree.SetItemData( item, nItemKey);
	m_checkTree.Expand( hParent, TVE_EXPAND );
	return item;
}

void CCheckBuildFormView::InsertNotifyItem(LPCTSTR lpszItem)
{
	m_checkTree.InsertItem( lpszItem,4,4, TVI_ROOT, TVI_LAST );
	return;
}
void CCheckBuildFormView::InserRootItem()
{
	m_checkTree.DeleteAllItems();
	m_checkTree.InsertItem( "Script",0,0, TVI_ROOT, TVI_LAST );
}




void CCheckBuildFormView::OnClickCheckTree(NMHDR* pNMHDR, LRESULT* pResult) 
{

	*pResult = 0;
}

void CCheckBuildFormView::OnDblclkCheckTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if( NULL == m_pDrawView  ) return;
	m_pDrawView->EditNodeObjProperty();
	
	Invalidate();
	*pResult = 0;
}

void CCheckBuildFormView::OnSelchangedCheckTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if( NULL == m_pDrawView  ) return;
	BOOL bResult = FALSE;
	HTREEITEM hItem = m_checkTree.GetSelectedItem();
	if( NULL == hItem  ) return;
	if( NULL == m_checkTree.GetParentItem(hItem) ) return;
    int nItemKey = m_checkTree.GetItemData(hItem);
	CNodeObj * pObj1 = m_pDrawView->FindNodeObj(nItemKey);
	if( NULL == pObj1 )
	{
		if( NULL == m_checkTree.GetChildItem(hItem) )
		{
			hItem = m_checkTree.GetParentItem(hItem);
			if( NULL != hItem  ){
				nItemKey = m_checkTree.GetItemData(hItem);
				pObj1 = m_pDrawView->FindNodeObj(nItemKey);
			}
		}
	}
	//µ±ÕÒµ½Ê±
	if( NULL != pObj1 )
	{
		HTREEITEM hItem1 = m_checkTree.GetParentItem(hItem);
		if( NULL != hItem1 )
		{
			int nItemKey2 = m_checkTree.GetItemData(hItem1);
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
				else if( NODETYPE_ENDCALL_BLOCK == pObj2->m_nType )
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
			else
			{
				if( NODETYPE_ENDCALL_BLOCK == pObj1->m_nType )
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
