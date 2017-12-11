// FileListFormView.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "FileListFormView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CObList  g_ViewList;
CFileListFormView *g_pFileListView = NULL;
/////////////////////////////////////////////////////////////////////////////
// CFileListFormView

IMPLEMENT_DYNCREATE(CFileListFormView, CFormView)

CFileListFormView::CFileListFormView()
	: CFormView(CFileListFormView::IDD)
{
	//{{AFX_DATA_INIT(CFileListFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_InitialUpdate=0;
	m_cy=0;
	m_cx=0;
	g_pFileListView = this;
}

CFileListFormView::~CFileListFormView()
{
}

void CFileListFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileListFormView)
	DDX_Control(pDX, IDC_TREE_FILE, m_ctrFileTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileListFormView, CFormView)
	//{{AFX_MSG_MAP(CFileListFormView)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, OnSelchangedTreeFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileListFormView diagnostics

#ifdef _DEBUG
void CFileListFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFileListFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileListFormView message handlers

void CFileListFormView::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{
		m_ctrFileTree.MoveWindow(0,0,m_cx,m_cy);
	}
	
}

void CFileListFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;
		HICON hIcon[3];

		m_ImageList.Create(16,16,0,3,3);
		hIcon[0] = AfxGetApp()->LoadIcon(IDI_NORMAL);
		hIcon[1] = AfxGetApp()->LoadIcon(IDI_MODULE);

		for(int i = 0 ; i < 3 ; i++)
		{ 
			m_ImageList.Add(hIcon[i]);
		}

		m_ctrFileTree.SetImageList(&m_ImageList,TVSIL_NORMAL);

		HTREEITEM hItem  = NULL;
		hItem  = m_ctrFileTree.InsertItem( "Normal Files",0,0, TVI_ROOT, TVI_LAST );
		m_ctrFileTree.SetItemData(hItem,1);
		hItem  = m_ctrFileTree.InsertItem( "Module Files",1,1, TVI_ROOT, TVI_LAST );
		m_ctrFileTree.SetItemData(hItem,2);

		m_ctrFileTree.MoveWindow(0,0,m_cx,m_cy);
	}

	return;
}

void CFileListFormView::InsertItem(DWORD dwData, int nDocType,CString strTitle)
{
	HTREEITEM hItem1 = NULL;
	HTREEITEM hItem2 = NULL;

	hItem1 = m_ctrFileTree.GetRootItem( );
	hItem2 = m_ctrFileTree.GetNextItem( hItem1,TVGN_NEXT );

	if( 0 == nDocType )
	{
		HTREEITEM hItem  = NULL;
		hItem  = m_ctrFileTree.InsertItem( strTitle,0,0, hItem1, TVI_LAST );
		m_ctrFileTree.SetItemData(hItem,dwData);
	}
	else
	{
		HTREEITEM hItem  = NULL;
		hItem  = m_ctrFileTree.InsertItem( strTitle,1,1, hItem2, TVI_LAST );
		m_ctrFileTree.SetItemData(hItem,dwData);
	}

	return;
}

HTREEITEM CFileListFormView::FindItemFromItemData(DWORD ID, HTREEITEM TreeItem) 
{ 
	 DWORD ItemData; 
	 HTREEITEM NextItem; 
	 HTREEITEM ResultItem = NULL; 
	 if (TreeItem == NULL) 
	  TreeItem = m_ctrFileTree.GetRootItem(); 


	 ItemData = m_ctrFileTree.GetItemData(TreeItem); 
	 if (ItemData == ID) 
	  return TreeItem; 


	 NextItem = m_ctrFileTree.GetNextItem(TreeItem, TVGN_CHILD); 
	 if (NextItem != NULL) 
	 { 
		  ResultItem = FindItemFromItemData(ID, NextItem); 
		  if (ResultItem != NULL) 
				return ResultItem; 
	 }


	 NextItem = m_ctrFileTree.GetNextItem(TreeItem, TVGN_NEXT); 
	 if (NextItem != NULL) 
			ResultItem = FindItemFromItemData(ID, NextItem); 
	 return ResultItem; 

} 
void CFileListFormView::DeleteItem(DWORD dwData)
{
	HTREEITEM hItem = FindItemFromItemData(dwData, NULL) ;
	if( NULL == hItem )
		return;
	m_ctrFileTree.DeleteItem( hItem );
}

void CFileListFormView::OnSelchangedTreeFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	BOOL bResult = FALSE;
	HTREEITEM hItem = m_ctrFileTree.GetSelectedItem();
	if( NULL == hItem  ) return;
    DWORD nItemKey = m_ctrFileTree.GetItemData(hItem);

	if( nItemKey > 10 )
	{
		POSITION pos = g_ViewList.GetHeadPosition();
		while(pos)
		{
			CDrawView *pView = (CDrawView *)g_ViewList.GetNext(pos);
			if( NULL != pView )
			{
				if( nItemKey == pView->m_dwID )
				{
					
					pView->GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
					//pView->GetParent()->ShowWindow(SW_SHOWNOACTIVATE);
					
					//pView->GetDocument()->OnShowViews(TRUE);
					
					//pView->UpdateWindow();
				}
			}
		}

	}
	*pResult = 0;
}
