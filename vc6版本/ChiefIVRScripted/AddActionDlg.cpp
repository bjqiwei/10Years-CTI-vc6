// AddActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "AddActionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddActionDlg dialog


CAddActionDlg::CAddActionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddActionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddActionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_strExpress="";
	m_nActionType = -1;
	m_bInit = FALSE;
}


void CAddActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddActionDlg)
	DDX_Control(pDX, IDC_EDIT_EXPRESS, m_ctrExpress);
	DDX_Control(pDX, IDC_LIST_ACTION, m_lstAction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddActionDlg, CDialog)
	//{{AFX_MSG_MAP(CAddActionDlg)
	ON_BN_CLICKED(IDC_BUTTON_MENU, OnButtonMenu)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ACTION, OnClickListAction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddActionDlg message handlers

BOOL CAddActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( !m_bInit )
	{
		m_bInit = TRUE;
		m_ctrExpress.SetWindowText(m_strExpress);
		InitActionList();
		
		int nCount = m_lstAction.GetItemCount();
		for( int i = 0; i < nCount; i++ )
		{
			if( m_nActionType == i  )
			{
				UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
				m_lstAction.SetItemState( i , flag,flag) ;
			}
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddActionDlg::OnOK() 
{

	m_ctrExpress.GetWindowText(m_strExpress);
	
	CDialog::OnOK();
}

void CAddActionDlg::InitActionList()
{
	m_lstAction.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstAction.InsertColumn(0,"Action",LVCFMT_CENTER,400,400);

	m_lstAction.InsertItem( 0, "SendFax( <File> )");
	m_lstAction.InsertItem( 1, "StartPlayFile(  <File> ; <Type> ; <Loop> )");
	m_lstAction.InsertItem( 2, "StartRecordFile( <File>  ; <Len> ; <Type> ;  <Loop> )");
	m_lstAction.InsertItem( 3, "GetAgentCall( <AgentDN> )");
	m_lstAction.InsertItem( 4, "EnterConfer( <Chnl> ; <conference> )");
	m_lstAction.InsertItem( 5, "LeaveConfer( <Chnl> ; <conference> )");
	m_lstAction.InsertItem( 6, "ReciveDtmf( <SaveTo> ; <Len> ; <EndDtmf> )");
	m_lstAction.InsertItem( 7, "ReciveFax( <File> )");
	m_lstAction.InsertItem( 8, "OffHook( <Chnl> )");
	m_lstAction.InsertItem( 9, "OutCall( <Tel> )");
	m_lstAction.InsertItem( 10, "StopPlayFile( )");
	m_lstAction.InsertItem( 11, "StopRecordFile()");
	m_lstAction.InsertItem( 12, "HangUp( <Chnl> )");
	m_lstAction.InsertItem( 13, "ClearDtmf( <Chnl> )");
	m_lstAction.InsertItem( 14, "GetActiveCall( <Type> ; <Tel> )");
	m_lstAction.InsertItem( 15, "AnswerCall( <Type> )");
	m_lstAction.InsertItem( 16, "Hold( <File> )");
	m_lstAction.InsertItem( 17, "PickUpHold( <Type> ; <Tel> )");
	m_lstAction.InsertItem( 18, "TransferToIVR( <NodeKey> )");

	m_lstAction.InsertItem( 19, "Monitor( <DN> )");
	m_lstAction.InsertItem( 20, "DisMonitor( )");
	m_lstAction.InsertItem( 21, "JoinConfer( <Tel> )");

	m_lstAction.InsertItem(22,"ResponseTransfer( <nResult> )");
	m_lstAction.InsertItem(23,"ResponsePickUpHold( <nResult> )");
	m_lstAction.InsertItem(24,"ResponseMonitor( <nResult> )");
	m_lstAction.InsertItem(25,"ResponseLogin( <nResult> )");
	m_lstAction.InsertItem(26,"ResponseJoinConfer( <nResult> )");
	m_lstAction.InsertItem(27,"ResponseHold( <nResult> )");
	m_lstAction.InsertItem(28,"ResponseGetActive( <nResult> )");
	m_lstAction.InsertItem(29,"ResponseDisMonitor( <nResult> )");
	m_lstAction.InsertItem(30,"ResponseConferCall( <nResult> )");
	m_lstAction.InsertItem(31,"ResponseCancelTransfer( <nResult> )");
	m_lstAction.InsertItem(32,"ResponseAnswerCall( <nResult> )");
	m_lstAction.InsertItem(33,"ResponesMakeCall( <nResult> )");

	m_lstAction.InsertItem(34,"ResponseLogOut( <nResult> )");
	m_lstAction.InsertItem(35,"ResponseReady( <nResult> )");
	m_lstAction.InsertItem(36,"ResponseNotReady( <nResult> )");

	m_lstAction.InsertItem(37,"ResponseSetBusy( <nResult> )");
	m_lstAction.InsertItem(38,"ResponseInServer( <nResult> )");



	return;

}

void CAddActionDlg::OnButtonMenu() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_ctrExpress.SetSel(-1,0);
		m_ctrExpress.ReplaceSel(str);
		UpdateData(FALSE);
	}

	return;
	
}

void CAddActionDlg::OnClickListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstAction.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;

	m_nActionType = nIndex;
	m_ctrExpress.SetWindowText( m_lstAction.GetItemText( nIndex ,0) );
	UpdateData(FALSE);

	*pResult = 0;
}


