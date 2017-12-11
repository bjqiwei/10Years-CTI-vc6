// ConnectNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "DrawDoc.h"
#include "DrawView.h"
#include "ConnectNodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectNodeDlg dialog


CConnectNodeDlg::CConnectNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectNodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectNodeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_pNextNodeSelected = NULL;
	m_nShowMode = 0;
	m_pView = NULL;
	m_nShowType = 0;
	m_pGoToObj = NULL;
}


void CConnectNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectNodeDlg)
	DDX_Control(pDX, IDC_TREE_NODELIST, m_nodeTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectNodeDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectNodeDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_NODELIST, OnDblclkTreeNodelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectNodeDlg message handlers

void CConnectNodeDlg::InitDlg(CDrawDoc *pDoc, CNextNode * pNextNodeSelected, CDrawView *pView , int nShowMode )
{
	if( NULL == pDoc || NULL == pNextNodeSelected ) return;
	m_pDoc = pDoc;
	m_pNextNodeSelected = pNextNodeSelected;
	m_nShowMode = nShowMode;
	m_pView = pView;
	m_nShowType = 0;
	return;
}


void CConnectNodeDlg::OnOK() 
{
	HTREEITEM hItem = m_nodeTree.GetSelectedItem();
	if( NULL == hItem  ) return;
	if( NULL == m_pDoc )  return;

	if( 0 == m_nShowType )
	{
		if( NULL == m_pNextNodeSelected ) return;
		CNodeObj *pObj = m_pDoc->FindNodeObj(m_nodeTree.GetItemData(hItem ));
		if( NULL == pObj ) return;

		m_pNextNodeSelected->ConnectToNodeObj( pObj->m_strCaption, pObj->m_nType,pObj->m_nKey,pObj->m_rect);
	}
	else
	{
		GoToConnect();
	}
	
	CDialog::OnOK();
}


BOOL CConnectNodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetNodeImageList( &m_ImageList );

		m_nodeTree.SetImageList(&m_ImageList,TVSIL_NORMAL);

		if( 0 == m_nShowType )
		{
			InitNodeTree();
		}
		else
		{
			InitGoToTree();
		}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CConnectNodeDlg::InitNodeTree()
{
	if( NULL == m_pDoc ) return;
	if( NULL == m_pNextNodeSelected ) return;
	if( 0 == m_nShowMode )
	{
		POSITION pos = m_pDoc->m_objects.GetHeadPosition();
		while( pos )
		{
			CNodeObj * pObj = m_pDoc->m_objects.GetNext(pos);
			if( NULL == pObj )
				continue;
			if( 0 != pObj->m_nType ) {
				HTREEITEM hItem = m_nodeTree.InsertItem( pObj->m_strCaption, GetImageIndex(pObj->m_nIcoID), GetImageIndex(pObj->m_nIcoID), TVI_ROOT, TVI_LAST );
				m_nodeTree.SetItemData(hItem,pObj->m_nKey);
				if( pObj->m_nKey == m_pNextNodeSelected->m_nNodeObjKey )
					m_nodeTree.SelectItem(hItem);
			}

		}
	}
	else if( 1== m_nShowMode )
	{
		CBlockObj *pBlockObj = m_pView->m_pBlockObj;
		if( NULL != pBlockObj )
		{
			POSITION pos = pBlockObj->m_ChildObjLst.GetHeadPosition();
			while( pos )
			{
				CNodeObj * pObj = pBlockObj->m_ChildObjLst.GetNext(pos);
				if( NULL == pObj )
					continue;
				if( 0 != pObj->m_nType ) {
					HTREEITEM hItem = m_nodeTree.InsertItem( pObj->m_strCaption, GetImageIndex(pObj->m_nIcoID), GetImageIndex(pObj->m_nIcoID), TVI_ROOT, TVI_LAST );
					m_nodeTree.SetItemData(hItem,pObj->m_nKey);
					if( pObj->m_nKey == m_pNextNodeSelected->m_nNodeObjKey )
						m_nodeTree.SelectItem(hItem);
				}

			}
		}

	}
	else
	{
		CEndCallBlockObj *pEndCallBlockObj =  m_pView->m_pEndCallBlockObj;
		if( NULL != pEndCallBlockObj )
		{
			POSITION pos = pEndCallBlockObj->m_ChildObjLst.GetHeadPosition();
			while( pos )
			{
				CNodeObj * pObj = pEndCallBlockObj->m_ChildObjLst.GetNext(pos);
				if( NULL == pObj )
					continue;
				if( 0 != pObj->m_nType ) {
					HTREEITEM hItem = m_nodeTree.InsertItem( pObj->m_strCaption, GetImageIndex(pObj->m_nIcoID), GetImageIndex(pObj->m_nIcoID), TVI_ROOT, TVI_LAST );
					m_nodeTree.SetItemData(hItem,pObj->m_nKey);
					if( pObj->m_nKey == m_pNextNodeSelected->m_nNodeObjKey )
						m_nodeTree.SelectItem(hItem);
				}

			}
		}
	}
	

	return;
}

void CConnectNodeDlg::OnDblclkTreeNodelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	
	*pResult = 0;
}

void CConnectNodeDlg::InitGoToTree()
{
	if( NULL == m_pDoc || NULL == m_pGoToObj ) return;
	if( 2 == m_nShowMode )
	{
		CEndCallBlockObj *pEndCallBlockObj =  m_pView->m_pEndCallBlockObj;
		if( NULL != pEndCallBlockObj )
		{
			POSITION pos = pEndCallBlockObj->m_ChildObjLst.GetHeadPosition();
			while( pos )
			{
				CNodeObj * pObj = pEndCallBlockObj->m_ChildObjLst.GetNext(pos);
				if( NULL == pObj )
					continue;
				if( 0 != pObj->m_nType ) {
					HTREEITEM hItem = m_nodeTree.InsertItem( pObj->m_strCaption, GetImageIndex(pObj->m_nIcoID), GetImageIndex(pObj->m_nIcoID), TVI_ROOT, TVI_LAST );
					m_nodeTree.SetItemData(hItem,pObj->m_nKey);
					if( pObj->m_nKey == m_pGoToObj->m_nNextObjKey )
						m_nodeTree.SelectItem(hItem);
				}

			}
		}
	}
	else
	{
		POSITION pos = m_pDoc->m_objects.GetHeadPosition();
		while( pos )
		{
			CNodeObj * pObj = m_pDoc->m_objects.GetNext(pos);
			if( NULL == pObj )
				continue;
			if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
				continue;
			if( 0 != pObj->m_nType ) 
			{
				HTREEITEM hItem = m_nodeTree.InsertItem( pObj->m_strCaption, GetImageIndex(pObj->m_nIcoID), GetImageIndex(pObj->m_nIcoID), TVI_ROOT, TVI_LAST );
				m_nodeTree.SetItemData(hItem,pObj->m_nKey);
				if( pObj->m_nKey == m_pGoToObj->m_nNextObjKey )
					m_nodeTree.SelectItem(hItem);
				if( NODETYPE_BLOCK == pObj->m_nType )
				{
					InitBlockMember( hItem , (CBlockObj*)pObj );
				}
			}

		}


	}

	return;
}

void CConnectNodeDlg::InitBlockMember(HTREEITEM hParentItem, CBlockObj *pObj)
{
	CBlockObj *pBlockObj = pObj;
	if( NULL != pBlockObj )
	{
		POSITION pos = pBlockObj->m_ChildObjLst.GetHeadPosition();
		while( pos )
		{
			CNodeObj * pObj = pBlockObj->m_ChildObjLst.GetNext(pos);
			if( NULL == pObj )
				continue;
			if( 0 != pObj->m_nType ) 
			{
				HTREEITEM hItem = m_nodeTree.InsertItem( pObj->m_strCaption, GetImageIndex(pObj->m_nIcoID), GetImageIndex(pObj->m_nIcoID), hParentItem, TVI_LAST );
				m_nodeTree.SetItemData(hItem,pObj->m_nKey);
				if( pObj->m_nKey == m_pGoToObj->m_nNextObjKey )
					m_nodeTree.SelectItem(hItem);
				if( NODETYPE_BLOCK  == pObj->m_nType )
				{
					InitBlockMember( hItem,  (CBlockObj*)pObj );
				}
			}

		}
	}
	return;
}

void CConnectNodeDlg::InitGoToDlg(CDrawDoc *pDoc, CGoToObj *pGoToObj, CDrawView *pView, int nShowMode)
{
	if( NULL == pDoc || NULL == pGoToObj ) return;
	m_pDoc = pDoc;
	m_pGoToObj = pGoToObj;
	m_nShowMode = nShowMode;
	m_pView = pView;
	m_nShowType = 1;
	return;
}

void CConnectNodeDlg::GoToConnect()
{
	HTREEITEM hItem = m_nodeTree.GetSelectedItem();
	if( NULL == hItem  ) return;
	if( NULL == m_pGoToObj ) return;
	if( NULL == m_pDoc )  return;
	int nNextKey = m_nodeTree.GetItemData(hItem );
	CNodeObj *pObj = m_pDoc->FindNodeObj( nNextKey );
	if( NULL == pObj ) return;
	CString strPath = m_nodeTree.GetItemText( hItem );
	HTREEITEM hParentItem = m_nodeTree.GetParentItem( hItem );
	while( hParentItem )
	{
		strPath = m_nodeTree.GetItemText( hParentItem ) + "\\" + strPath;
		hParentItem = m_nodeTree.GetParentItem( hParentItem );
	}
	m_pGoToObj->m_nNextObjKey = nNextKey;
	m_pGoToObj->m_strConnPath = m_pGoToObj->m_strCaption + "->" + strPath;
	return;
}
