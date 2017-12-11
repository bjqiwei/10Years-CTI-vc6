// CcCallAgentPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "CcCallAgentPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCcCallAgentPage property page

IMPLEMENT_DYNCREATE(CCcCallAgentPage, CPropertyPage)

CCcCallAgentPage::CCcCallAgentPage() : CPropertyPage(CCcCallAgentPage::IDD)
{
	//{{AFX_DATA_INIT(CCcCallAgentPage)
	m_nTimeOut = 0;
	m_nWaitType = -1;
	m_strWaitPlayFile = _T("");
	m_nPlayType = -1;
	m_strAimDN = _T("");
	m_bIsPlayBeforeTalk = FALSE;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CCcCallAgentPage::~CCcCallAgentPage()
{
}

void CCcCallAgentPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCcCallAgentPage)
	DDX_Control(pDX, IDC_EDIT_PLAYFILE_EXPRESS, m_edtFileExpress);
	DDX_Control(pDX, IDC_LIST_PLAY, m_lstPlay);
	DDX_Text(pDX, IDC_SWITCH_TIMEOUT, m_nTimeOut);
	DDX_Radio(pDX, IDC_RADIO_WAITOPTION1, m_nWaitType);
	DDX_Text(pDX, IDC_EDIT_PLAYFILE, m_strWaitPlayFile);
	DDX_CBIndex(pDX, IDC_COMBO_PLAYTYPE, m_nPlayType);
	DDX_Text(pDX, IDC_EDIT_AIMDN, m_strAimDN);
	DDX_Check(pDX, IDC_CHECK_ISPLAY, m_bIsPlayBeforeTalk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCcCallAgentPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCcCallAgentPage)
	ON_BN_CLICKED(IDC_BUTTON_AIMDN, OnButtonAimdn)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_EXPRESS, OnButtonExpress)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PLAY, OnClickListPlay)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCcCallAgentPage message handlers

BOOL CCcCallAgentPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//EnableConferenceWindow();

	//播放语音的时候，有2种语音，一种是读变量
	m_lstPlay.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstPlay.InsertColumn(0,"Type",LVCFMT_LEFT,100,100);
	m_lstPlay.InsertColumn(1,"File or Express",LVCFMT_LEFT,400,400);

	m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
	m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );

	if( NULL == m_pNodeObj || NULL == m_pDoc ) return TRUE;

	CNodeDataList *pDadaList = &(m_pNodeObj->m_PlayList);
	POSITION pos = pDadaList->GetHeadPosition( );
	
	//下面开始初始化数据
	int i = 0;
	CNodeData *pData = NULL;
	while( pos )
	{
		pData = pDadaList->GetNext(pos);
		if( NULL != pData )
		{
			m_lstPlay.InsertItem( i, pData->m_strName );//参数变量
			m_lstPlay.SetItemText(i,1, pData->m_strData );//参数类型
			i++;
		}

	}



	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CCcCallAgentPage::GetPlayType(CString strPlayType)
{
	if("File" == strPlayType )
		return 0;
	else if("Number" == strPlayType )
		return 1;
	else if("TTS" == strPlayType )
		return 2;
	else if("Floating" == strPlayType )
		return 3;
	else if("Date" == strPlayType )
		return 4;
	else
		return 0;
}

CString CCcCallAgentPage::GetPlayStr(int nPlayType)
{
	switch( nPlayType )
	{
	case 0:
		return "File";
	case 1:
		return "Number";
	case 2:
		return "TTS";
	case 3:
		return "Floating";
	case 4:
		return "Date";
	default:
		return "File";
	}
}

void CCcCallAgentPage::InitPage(CCCCallAgentObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;

	m_nTimeOut= m_pNodeObj->m_nTimeOut;
	m_nWaitType= m_pNodeObj->m_nWaitType;
	m_strWaitPlayFile= m_pNodeObj->m_strWaitPlayFile;
	m_nPlayType= m_pNodeObj->m_nPlayType;
	m_strAimDN= m_pNodeObj->m_strAimDN;
	m_bIsPlayBeforeTalk= m_pNodeObj->m_bIsPlayBeforeTalk;


	return;
}
LRESULT CCcCallAgentPage::OnNew( WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);
	CString str("");
	m_edtFileExpress.GetWindowText(str);
	if( "" == str ) 
	{
		MessageBox("Please check input!");
		return 0;
	}
	int nIndex = m_lstPlay.GetItemCount();
	m_lstPlay.InsertItem(nIndex,GetPlayStr( m_nPlayType) );//参数类型
	m_lstPlay.SetItemText(nIndex,1, str );//参数类型


	return 0;
}
LRESULT CCcCallAgentPage::OnEdit( WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);

	int nIndex = m_lstPlay.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	CString str("");
	m_edtFileExpress.GetWindowText(str);
	if( "" == str ) 
	{
		MessageBox("Please check input!");
		return 0;
	}
	
	m_lstPlay.SetItemText(nIndex,0,GetPlayStr( m_nPlayType) );//参数类型
	m_lstPlay.SetItemText(nIndex,1, str );//参数类型

	return 0;
}

LRESULT CCcCallAgentPage::OnDel( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_lstPlay.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_lstPlay.DeleteItem(nIndex);

	return 0;
}

LRESULT CCcCallAgentPage::OnUP( WPARAM wParam,LPARAM lParam)
{
	int nIndex = m_lstPlay.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nAbove = m_lstPlay.GetNextItem( nIndex, LVNI_ABOVE ) ;
	if( -1 == nAbove )
		return 0;
	if( nIndex-1 > -1 )
	{
		
		m_lstPlay.InsertItem( nAbove,m_lstPlay.GetItemText(nIndex,0 ) );
		m_lstPlay.SetItemText(nAbove,1, m_lstPlay.GetItemText(nIndex + 1,1 ));//参数类型
		m_lstPlay.DeleteItem( nIndex + 1 );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstPlay.SetItemState(nAbove, flag,flag) ;

		m_nPlayType = GetPlayType( m_lstPlay.GetItemText(nAbove,0) );
		m_edtFileExpress.SetWindowText( m_lstPlay.GetItemText(nAbove,1) );

		UpdateData(FALSE);
	}
	
	return 0;
}

LRESULT CCcCallAgentPage::OnDown( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_lstPlay.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;
	int nBelow = m_lstPlay.GetNextItem( nIndex, LVNI_BELOW ) ;
	if( -1 == nBelow )
		return 0;
	if( nIndex+1 < m_lstPlay.GetItemCount() )
	{
		
		m_lstPlay.InsertItem( nBelow + 1 ,m_lstPlay.GetItemText(nIndex,0 ) );
		m_lstPlay.SetItemText(nBelow + 1,1, m_lstPlay.GetItemText(nIndex,1 ));//参数类型
		m_lstPlay.DeleteItem( nIndex );
		UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
		m_lstPlay.SetItemState(nBelow, flag,flag) ;

		m_nPlayType = GetPlayType( m_lstPlay.GetItemText(nBelow,0) );
		m_edtFileExpress.SetWindowText( m_lstPlay.GetItemText(nBelow,1) );

		UpdateData(FALSE);
	}

	
	return 0;
}

void CCcCallAgentPage::OnButtonAimdn() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strAimDN =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CCcCallAgentPage::OnButtonFile() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strWaitPlayFile =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CCcCallAgentPage::OnButtonExpress() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str1("");
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_edtFileExpress.GetWindowText(str1);
		m_edtFileExpress.SetSel(-1,0);
		m_edtFileExpress.ReplaceSel(str);
		UpdateData(FALSE);
	}

	return;
	
}

void CCcCallAgentPage::OnOK() 
{
	if( NULL == m_pNodeObj ) return;
	UpdateData(TRUE);

	m_pNodeObj->m_nTimeOut = m_nTimeOut;
	m_pNodeObj->m_nWaitType = m_nWaitType;
	m_pNodeObj->m_strWaitPlayFile = m_strWaitPlayFile;
	m_pNodeObj->m_nPlayType = m_nPlayType;
	m_pNodeObj->m_strAimDN = m_strAimDN;
	m_pNodeObj->m_bIsPlayBeforeTalk = m_bIsPlayBeforeTalk;


	CNodeDataList *pDadaList = &(m_pNodeObj->m_PlayList);
	POSITION pos = pDadaList->GetHeadPosition( );
	
	//先删除参数记录，然后再添加进去
	while ( pDadaList->GetHeadPosition( ) )
	{
		delete pDadaList->RemoveHead();
	}

	int nCount = m_lstPlay.GetItemCount();
	for( int i = 0; i < nCount; i++ )
	{
		CNodeData *pVar = new CNodeData;
		pVar->m_strName = m_lstPlay.GetItemText(i,0);
		pVar->m_strData = m_lstPlay.GetItemText(i,1);
		pVar->m_nDataType = ( ("NumberExpress" == pVar->m_strName)?1:0 );

		pVar->m_nDataType = GetPlayType( pVar->m_strName  );
		pDadaList->AddTail(pVar);

	}

	
	CPropertyPage::OnOK();
}

void CCcCallAgentPage::OnClickListPlay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstPlay.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;

	m_nPlayType = GetPlayType(m_lstPlay.GetItemText(nIndex,0));
	m_edtFileExpress.SetWindowText( m_lstPlay.GetItemText( nIndex ,1) );
	UpdateData(FALSE);

	*pResult = 0;
	
}
