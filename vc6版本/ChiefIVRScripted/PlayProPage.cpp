// PlayProPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "PlayProPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayProPage property page

IMPLEMENT_DYNCREATE(CPlayProPage, CPropertyPage)

CPlayProPage::CPlayProPage() : CPropertyPage(CPlayProPage::IDD)
{
	//{{AFX_DATA_INIT(CPlayProPage)
	m_bClearDTMF = FALSE;
	m_bReciveDTMF = FALSE;
	m_strDTMF = _T("");
	m_bPlayStop = FALSE;
	m_strConfNO = _T("");
	m_nPlayType = 0;
	m_nPlayWay = 0;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CPlayProPage::~CPlayProPage()
{
}

void CPlayProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayProPage)
	DDX_Control(pDX, IDC_EDIT_PLAYFILE, m_edtFileExpress);
	DDX_Control(pDX, IDC_LIST_PLAY, m_lstPlay);
	DDX_Check(pDX, IDC_CHECK_CLEARDTMF, m_bClearDTMF);
	DDX_Check(pDX, IDC_CHECK_RECIVE_DTMF, m_bReciveDTMF);
	DDX_Text(pDX, IDC_EDIT_DTMF, m_strDTMF);
	DDV_MaxChars(pDX, m_strDTMF, 128);
	DDX_Check(pDX, IDC_CHECK_PLAYSTOP, m_bPlayStop);
	DDX_Text(pDX, IDC_EDIT_CONFNO, m_strConfNO);
	DDX_CBIndex(pDX, IDC_COMBO_PLAYTYPE, m_nPlayType);
	DDX_CBIndex(pDX, IDC_COMBO_PLAYWAY, m_nPlayWay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayProPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPlayProPage)
	ON_BN_CLICKED(IDC_BUTTON_DTMF, OnButtonDtmf)
	ON_BN_CLICKED(IDC_CHECK_CLEARDTMF, OnCheckCleardtmf)
	ON_BN_CLICKED(IDC_CHECK_RECIVE_DTMF, OnCheckReciveDtmf)
	ON_BN_CLICKED(IDC_BUTTON_EXPRESS, OnButtonExpress)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PLAY, OnClickListPlay)
	ON_BN_CLICKED(IDC_BUTTON_CONFNO, OnButtonConfno)
	ON_CBN_SELCHANGE(IDC_COMBO_PLAYWAY, OnSelchangeComboPlayway)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_LISTBOXEXBUDDY_NEW, OnNew )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_EDIT, OnEdit )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DEL, OnDel )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_UP, OnUP )
	ON_MESSAGE( WM_LISTBOXEXBUDDY_DOWN, OnDown )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayProPage message handlers



void CPlayProPage::OnButtonDtmf() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strDTMF =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CPlayProPage::OnOK() 
{
	if( NULL == m_pNodeObj ) return;
	UpdateData(TRUE);

	m_pNodeObj->m_bClearDTMF = m_bClearDTMF;
	m_pNodeObj->m_bRecDTMF = m_bReciveDTMF;
	m_pNodeObj->m_strSaveDTMF = m_strDTMF ;
	m_pNodeObj->m_bPlayStop = m_bPlayStop;

	m_pNodeObj->m_strConfNO = m_strConfNO;
	m_pNodeObj->m_nPlayWay = m_nPlayWay;


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

BOOL CPlayProPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EnableConferenceWindow();

	//播放语音的时候，有2种语音，一种是读变量
	m_lstPlay.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstPlay.InsertColumn(0,"Type",LVCFMT_LEFT,100,100);
	m_lstPlay.InsertColumn(1,"File or Express",LVCFMT_LEFT,400,400);

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

	 m_ListBoxExBuddy.SubclassDlgItem( IDC_BUTTON_EDITOP, this );
	 m_ListBoxExBuddy.SetListbox( this->GetSafeHwnd() );


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayProPage::InitPage(CPlayStartObj *pNodeObj, CDrawDoc * pDoc )
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;

	m_bClearDTMF = m_pNodeObj->m_bClearDTMF;
	m_bReciveDTMF = m_pNodeObj->m_bRecDTMF;
	m_strDTMF = m_pNodeObj->m_strSaveDTMF;
	m_bPlayStop = m_pNodeObj->m_bPlayStop;

	m_strConfNO = m_pNodeObj->m_strConfNO;
	m_nPlayWay = m_pNodeObj->m_nPlayWay;


	return;
}

void CPlayProPage::OnCheckCleardtmf() 
{
	UpdateData(TRUE);
	
}

void CPlayProPage::OnCheckReciveDtmf() 
{
	UpdateData(TRUE);
	EnableConferenceWindow();
}

LRESULT CPlayProPage::OnNew( WPARAM wParam,LPARAM lParam)
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

LRESULT CPlayProPage::OnEdit( WPARAM wParam,LPARAM lParam)
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

LRESULT CPlayProPage::OnDel( WPARAM wParam,LPARAM lParam)
{

	int nIndex = m_lstPlay.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return 0;

	m_lstPlay.DeleteItem(nIndex);

	return 0;
}

LRESULT CPlayProPage::OnUP( WPARAM wParam,LPARAM lParam)
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

LRESULT CPlayProPage::OnDown( WPARAM wParam,LPARAM lParam)
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

	
	return 0;
}

void CPlayProPage::OnButtonExpress() 
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

void CPlayProPage::OnClickListPlay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstPlay.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;

	m_nPlayType = GetPlayType(m_lstPlay.GetItemText(nIndex,0));
	m_edtFileExpress.SetWindowText( m_lstPlay.GetItemText( nIndex ,1) );
	UpdateData(FALSE);

	*pResult = 0;
}




void CPlayProPage::OnButtonConfno() 
{
	
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strConfNO =  str;
		UpdateData(FALSE);
	}

	return;
	
}



void CPlayProPage::EnableConferenceWindow()
{
	if( 0 == m_nPlayWay )
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_CONFNO);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_BUTTON_CONFNO);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_CHECK_LOOPPLAY);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		/////////////////////////////////////
		phWnd = GetDlgItem(IDC_CHECK_CLEARDTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

		phWnd = GetDlgItem(IDC_CHECK_RECIVE_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

		phWnd = GetDlgItem(IDC_EDIT_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( m_bReciveDTMF);

		phWnd = GetDlgItem(IDC_BUTTON_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( m_bReciveDTMF);

		phWnd = GetDlgItem(IDC_CHECK_PLAYSTOP);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);

	}
	else
	{
		CWnd * phWnd = GetDlgItem(IDC_EDIT_CONFNO);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_BUTTON_CONFNO);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_CHECK_LOOPPLAY);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		//////////////////////////////////////
		phWnd = GetDlgItem(IDC_CHECK_CLEARDTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_CHECK_RECIVE_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_EDIT_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_BUTTON_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

		phWnd = GetDlgItem(IDC_CHECK_PLAYSTOP);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
	}	
}

CString CPlayProPage::GetPlayStr(int nPlayType)
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

int CPlayProPage::GetPlayType(CString strPlayType)
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

void CPlayProPage::OnSelchangeComboPlayway() 
{
	UpdateData(TRUE);
	EnableConferenceWindow();
	
}
