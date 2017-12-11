// FindFormView.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "DrawDoc.h"
#include "DrawView.h"
#include "FindFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFindFormView *g_FindFormView = NULL;
/////////////////////////////////////////////////////////////////////////////
// CFindFormView

IMPLEMENT_DYNCREATE(CFindFormView, CFormView)

CFindFormView::CFindFormView()
	: CFormView(CFindFormView::IDD)
{
	//{{AFX_DATA_INIT(CFindFormView)
	m_nFindType = 0;
	m_strFindStr = _T("");
	m_strMatchesCount = _T("");
	//}}AFX_DATA_INIT
	m_InitialUpdate=0;
	m_cy=0;
	m_cx=0;
	m_pDrawView = NULL;
	g_FindFormView = this;
}

CFindFormView::~CFindFormView()
{
}

void CFindFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindFormView)
	DDX_Control(pDX, IDC_FIND_LIST, m_ctrFindList);
	DDX_CBIndex(pDX, IDC_FINDTYPE, m_nFindType);
	DDX_Text(pDX, IDC_EDIT_FINDSTR, m_strFindStr);
	DDX_Text(pDX, IDC_STATIC_MATCHES, m_strMatchesCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindFormView, CFormView)
	//{{AFX_MSG_MAP(CFindFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_FINDNODE, OnButtonFindnode)
	ON_NOTIFY(NM_DBLCLK, IDC_FIND_LIST, OnDblclkFindList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FIND_LIST, OnItemchangedFindList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindFormView diagnostics

#ifdef _DEBUG
void CFindFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFindFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFindFormView message handlers

void CFindFormView::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{

		m_ctrFindList.MoveWindow(0,30,m_cx,m_cy-30);

	}
	
}

void CFindFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;
		SetNodeImageList( &m_ImageList );
		
		InsertMyColumn();
		
		m_ctrFindList.SetImageList(&m_ImageList,LVSIL_SMALL);//必须是LVSIL_SMALL才能显示图标
		m_ctrFindList.MoveWindow(0,30,m_cx,m_cy-30);
		
	}

	
}



void CFindFormView::InsertMyColumn()
{
	m_ctrFindList.SetExtendedStyle(  LVS_EX_FULLROWSELECT  );
	m_ctrFindList.InsertColumn( 0, "Node Name", LVCFMT_LEFT, 150, 150 );
	m_ctrFindList.InsertColumn( 1, "Node Tree", LVCFMT_LEFT, 550, 550 );

	return;
}


void CFindFormView::OnButtonFindnode() 
{
	UpdateData(TRUE);
	m_strMatchesCount="";
	UpdateData(FALSE);
	m_ctrFindList.DeleteAllItems();
	UpdateData(TRUE);
	if( "" == m_strFindStr ) return;
	if( NULL == m_pDrawView ) return;
	CDrawDoc *pDoc = m_pDrawView->GetDocument( );
	if( NULL == pDoc ) return;


	CString strNodeTree("Script");
	CString strUpperFind = m_strFindStr;
	CString strUpperSource("");
	strUpperFind.MakeUpper();
	int i = 0;
	POSITION pos = pDoc->m_objects.GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = pDoc->m_objects.GetPrev(pos);
		if( NULL == pObj ) continue;
		if( 0 == m_nFindType ){
			//节点名称查询
			strUpperSource = pObj->m_strCaption;
			strUpperSource.MakeUpper();
			if(   strUpperSource.Find(strUpperFind) > -1  ){
				if(  NODETYPE_ENDCALL_BLOCK != pObj->m_nType ){
					int nIndex = m_ctrFindList.InsertItem( i, pObj->m_strCaption, GetImageIndex(pObj->m_nIcoID) );
					m_ctrFindList.SetItemData( nIndex, pObj->m_nKey );
					m_ctrFindList.SetItemText( i, 1, strNodeTree );
					strNodeTree = "Script";
					i++;
					if(  NODETYPE_BLOCK != pObj->m_nType )
						continue;
				}
			}
		}
		else {
			//节点描述查询
			strUpperSource = pObj->m_strDescription;
			strUpperSource.MakeUpper();
			if(  strUpperSource.Find(strUpperFind) > -1  ){
				if(  NODETYPE_ENDCALL_BLOCK != pObj->m_nType ){
					int nIndex = m_ctrFindList.InsertItem( i, pObj->m_strCaption, GetImageIndex(pObj->m_nIcoID) );
					m_ctrFindList.SetItemData( nIndex, pObj->m_nKey );
					m_ctrFindList.SetItemText( i, 1, strNodeTree );
					strNodeTree = "Script";
					i++;
					if(  NODETYPE_BLOCK != pObj->m_nType )
						continue;
				}
			}
		}


		if(  NODETYPE_BLOCK == pObj->m_nType ){
			strNodeTree += "\\" + pObj->m_strCaption;
			FindObjFromBlock((CBlockObj*)pObj, m_strFindStr,m_nFindType,strNodeTree ,&i );
			strNodeTree = "Script";
			continue;
		}
		else if(  NODETYPE_ENDCALL_BLOCK == pObj->m_nType ){
			strNodeTree = pObj->m_strCaption;
			FindObjFromEndCallBlock((CEndCallBlockObj*)pObj, m_strFindStr,m_nFindType,strNodeTree,&i);
			continue;
			
		}


	}
	
	m_strMatchesCount.Format("%d Matches",i);
	UpdateData(FALSE);
	return ;
	
}



void CFindFormView::FindObjFromEndCallBlock(CEndCallBlockObj *pObj, CString strFind, int nFindType, CString &strNodeTree, int *pnIndex)
{
	CString strUpperFind = strFind ;
	CString strUpperSource("");
	strUpperFind.MakeUpper();
	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj1 = pObj->m_ChildObjLst.GetNext(pos);
		if( pObj1 == NULL ) continue;
		if( 0 == nFindType )//按名称查询
			strUpperSource = pObj1->m_strCaption;
		else//按描述查询
			strUpperSource = pObj1->m_strDescription;

		strUpperSource.MakeUpper();
		if(  strUpperSource.Find(strUpperFind) > -1 )
		{

			int n = m_ctrFindList.InsertItem( *pnIndex, pObj1->m_strCaption, GetImageIndex(pObj1->m_nIcoID) );
			m_ctrFindList.SetItemData( n, pObj1->m_nKey );
			m_ctrFindList.SetItemText( *pnIndex, 1, strNodeTree );
			*pnIndex = *pnIndex + 1;
			continue;
		}


	}

	return ;
}

void CFindFormView::FindObjFromBlock(CBlockObj *pObj, CString strFind, int nFindType, CString &strNodeTree, int *pnIndex)
{
	CString strUpperFind = strFind ;
	CString strUpperSource("");
	strUpperFind.MakeUpper();
	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj2 = pObj->m_ChildObjLst.GetNext(pos);
		if( pObj2 == NULL ) continue;
		if( 0 == nFindType )//按名称查询
			strUpperSource = pObj2->m_strCaption;
		else//按描述查询
			strUpperSource = pObj2->m_strDescription;

		strUpperSource.MakeUpper();
		if( strUpperSource.Find(strUpperFind) > -1 )
		{
			int n = m_ctrFindList.InsertItem( *pnIndex, pObj2->m_strCaption, GetImageIndex(pObj2->m_nIcoID) );
			m_ctrFindList.SetItemData( n, pObj2->m_nKey );
			m_ctrFindList.SetItemText( *pnIndex, 1, strNodeTree );
			*pnIndex = *pnIndex + 1;
			if(  NODETYPE_BLOCK != pObj2->m_nType )
				continue;
		}

		if(  NODETYPE_BLOCK == pObj2->m_nType ){
			strNodeTree += "\\"+pObj2->m_strCaption;
			FindObjFromBlock( (CBlockObj *)pObj2, strFind,nFindType,strNodeTree,pnIndex );//递归调用
		}
		
	

	}

	return ;
}

void CFindFormView::OnDblclkFindList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if( NULL == m_pDrawView  ) return;
	m_pDrawView->EditNodeObjProperty();
	
	Invalidate();
	*pResult = 0;
}

void CFindFormView::OnItemchangedFindList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( NULL == m_pDrawView ) return;
	 int nIndex = m_ctrFindList.GetNextItem(-1, LVNI_SELECTED);  
	 if( -1 != nIndex )
	 {
		int nKey = m_ctrFindList.GetItemData(nIndex);
		int nParentKey = -1;
		CNodeObj *pObj = m_pDrawView->FindNodeObj(nKey, nParentKey );
		CNodeObj *pParentNode = m_pDrawView->FindNodeObj(nParentKey);
		if( NULL == pObj ) return;
		if( NULL != pParentNode ){
			if(  NODETYPE_BLOCK == pParentNode->m_nType ){
				m_pDrawView->m_nShowMode = 1;
				m_pDrawView->m_pBlockObj = (CBlockObj *)pParentNode;

				m_pDrawView->SetScrollPos(SB_HORZ,0,TRUE);
				m_pDrawView->SetScrollPos(SB_VERT,0,TRUE);
				m_pDrawView->Invalidate(TRUE);
				m_pDrawView->ShowFindObjToSelected(pObj);
			}
			else if( NODETYPE_ENDCALL_BLOCK == pParentNode->m_nType ){
				
				m_pDrawView->m_nShowMode = 2;
				m_pDrawView->SetScrollPos(SB_HORZ,0,TRUE);
				m_pDrawView->SetScrollPos(SB_VERT,0,TRUE);
				m_pDrawView->Invalidate(TRUE);
				m_pDrawView->ShowFindObjToSelected(pObj);
			}
			else{
				m_pDrawView->ShowFindObjToSelected(pObj);
				
				m_pDrawView->m_nShowMode = 0;
				m_pDrawView->m_pBlockObj = NULL;
			}
		}
		else{
			m_pDrawView->ShowFindObjToSelected(pObj);
			
			m_pDrawView->m_nShowMode = 0;
			m_pDrawView->m_pBlockObj = NULL;
		}
	 }
	
	
	*pResult = 0;
}
