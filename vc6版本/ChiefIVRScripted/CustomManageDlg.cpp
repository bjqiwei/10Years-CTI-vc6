// CustomManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "CustomManageDlg.h"
#include "CustomTemplate.h"
#include "AddToTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomManageDlg dialog
extern CString g_strAppPath;

CCustomManageDlg::CCustomManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomManageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCustomManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomManageDlg)
	DDX_Control(pDX, IDC_LIST_MANAGE, m_lstManage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomManageDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomManageDlg)
	ON_BN_CLICKED(ID_CUSTOM_UPDATE, OnCustomUpdate)
	ON_BN_CLICKED(ID_CUSTOM_DELETE, OnCustomDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MANAGE, OnClickListManage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MANAGE, OnDblclkListManage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomManageDlg message handlers

BOOL CCustomManageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetNodeImageList( &m_ImageList );
	m_lstManage.SetImageList(&m_ImageList,LVSIL_SMALL);

	m_lstManage.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstManage.InsertColumn(0,"Name",LVCFMT_CENTER,100,100);
	m_lstManage.InsertColumn(1,"Icon",LVCFMT_CENTER,50,50);
	m_lstManage.InsertColumn(2,"Description",LVCFMT_CENTER,200,200);

	int i = 0;
	CCustomTemplateDoc doc;
	CString strFile =  g_strAppPath + "\\Template.nor";
	CString str("");
	if( doc.OnOpenDocument( strFile ) )
	{
		POSITION pos = doc.m_ObjLst.GetHeadPosition();
		while (pos != NULL)
		{
			CCustomObj  *pObj = (CCustomObj  *)doc.m_ObjLst.GetNext(pos);
			if( NULL == pObj ) continue;

			str.Format("%d", pObj->m_nIcoID );
			m_lstManage.InsertItem( i, pObj->m_strCaption,GetImageIndex( pObj->m_nIcoID) );
			m_lstManage.SetItemText( i, 1, str );
			m_lstManage.SetItemText( i, 2, pObj->m_strDescription);
			m_lstManage.SetItemData(i, pObj->m_nKey );
			i++;
		}//end while
	}


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomManageDlg::OnCustomUpdate() 
{
	// TODO: Add your control notification handler code here
	
}

void CCustomManageDlg::OnCustomDelete() 
{
	int nIndex = m_lstManage.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;
	m_lstManage.DeleteItem(nIndex);

	
}

int CCustomManageDlg::FindItem(int nKey)
{
	int i = -1;
	int nCount = m_lstManage.GetItemCount();
	if( nCount < 1 ){
		return -1;
	}
	for( i = 0; i < nCount; i++ )
	{
		if( nKey ==	m_lstManage.GetItemData(i) )
		{
			return i;
		}
	}
	return -1;
}

void CCustomManageDlg::OnOK() 
{
	CCustomTemplateDoc doc;
	CString strFile =  g_strAppPath + "\\Template.nor";
	CString str("");
	if( doc.OnOpenDocument( strFile ) )
	{
		POSITION pos = doc.m_ObjLst.GetHeadPosition() ;
		while( pos )
		{
			CCustomObj  *pObj = (CCustomObj  *)doc.m_ObjLst.GetAt(pos);
			if( pObj != NULL )
			{
				int i = FindItem( pObj->m_nKey );
				if( -1 == i )
				{
					//表示该项被删除
					doc.m_ObjLst.RemoveAt(pos);
					delete pObj;
					pObj = NULL;
					pos = doc.m_ObjLst.GetHeadPosition();
					continue;
				}
				else
				{
					//该项存在
					pObj->m_strCaption = m_lstManage.GetItemText(i,0);
					pObj->m_strDescription = m_lstManage.GetItemText(i,2);
					pObj->m_nIcoID = atoi( m_lstManage.GetItemText(i,1) );
				}
			}
			doc.m_ObjLst.GetNext(pos) ;
		}
		doc.OnSaveDocument(strFile);
	}
	
	CDialog::OnOK();
}

void CCustomManageDlg::OnClickListManage(NMHDR* pNMHDR, LRESULT* pResult) 
{

	
	*pResult = 0;
}

void CCustomManageDlg::OnDblclkListManage(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstManage.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;
	CAddToTemplateDlg dlg;
	
	dlg.m_nIco = GetImageIndex( atoi( m_lstManage.GetItemText( nIndex, 1) ) );
	dlg.m_strName = m_lstManage.GetItemText( nIndex, 0) ;
	dlg.m_strExpress = m_lstManage.GetItemText( nIndex, 2) ;
	if( IDOK == dlg.DoModal() )
	{
	
			CString str("");
			str.Format("%d", GetImageIcoID( dlg.m_nIco ) );
			
			LVITEM lvItem;
			lvItem.iItem = nIndex;
			lvItem.mask = LVIF_IMAGE;
			lvItem.iImage = dlg.m_nIco;
			m_lstManage.SetItem(&lvItem);
			m_lstManage.SetItemText( nIndex, 0, dlg.m_strName );
			m_lstManage.SetItemText( nIndex, 1, str );
			m_lstManage.SetItemText( nIndex, 2, dlg.m_strExpress);

	}

	*pResult = 0;
}




