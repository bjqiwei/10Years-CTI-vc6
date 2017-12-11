// AddEventDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "AddEventDlg.h"
#include "..//include//NodeObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddEventDlg dialog


CAddEventDlg::CAddEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddEventDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddEventDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nEventType = -1;
	m_strEventName = "";
}


void CAddEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddEventDlg)
	DDX_Control(pDX, IDC_LIST_EVENT, m_lstEvent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddEventDlg, CDialog)
	//{{AFX_MSG_MAP(CAddEventDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_EVENT, OnClickListEvent)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EVENT, OnDblclkListEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddEventDlg message handlers

BOOL CAddEventDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitEventList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddEventDlg::OnOK() 
{
	int nIndex = m_lstEvent.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;

	m_nEventType = nIndex;	
	m_strEventName = m_lstEvent.GetItemText( nIndex,0 );
	CDialog::OnOK();
}

void CAddEventDlg::InitEventList()
{
	m_lstEvent.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstEvent.InsertColumn(0,"Event",LVCFMT_CENTER,230,230);

	m_lstEvent.InsertItem( 0, "Next");
	m_lstEvent.InsertItem( 1, "Error");
	m_lstEvent.InsertItem( 2, "Busy");
	m_lstEvent.InsertItem( 3, "NoDialTone");
	m_lstEvent.InsertItem( 4, "NoAnswer");
	m_lstEvent.InsertItem( 5, "NoRingBack");
	m_lstEvent.InsertItem( 6, "Conect");
	m_lstEvent.InsertItem( 7, "Nomatch");
	m_lstEvent.InsertItem( 8, "DTMF 0");
	m_lstEvent.InsertItem( 9, "DTMF 1");
	m_lstEvent.InsertItem( 10, "DTMF 2");
	m_lstEvent.InsertItem( 11, "DTMF 3");
	m_lstEvent.InsertItem( 12, "DTMF 4");
	m_lstEvent.InsertItem( 13, "DTMF 5");
	m_lstEvent.InsertItem( 14, "DTMF 6");
	m_lstEvent.InsertItem( 15, "DTMF 7");
	m_lstEvent.InsertItem( 16, "DTMF 8");
	m_lstEvent.InsertItem( 17, "DTMF 9");
	m_lstEvent.InsertItem( 18, "DTMF 10");
	m_lstEvent.InsertItem( 19, "DTMF 11");
	m_lstEvent.InsertItem( 20, "DTMFTimeout");
	m_lstEvent.InsertItem( 21, "OffHook");
	m_lstEvent.InsertItem( 22, "Hangup");
	m_lstEvent.InsertItem( 23, "Hookflash");
	m_lstEvent.InsertItem( 24, "ReceDTMF");
	m_lstEvent.InsertItem( 25, "RecFaxTone");
	m_lstEvent.InsertItem( 26, "AgentNotReady");
	m_lstEvent.InsertItem( 27, "AgentReady");
	m_lstEvent.InsertItem( 28, "ConfDestroyed");
	m_lstEvent.InsertItem( 29, "AgentLogin");
	m_lstEvent.InsertItem( 30, "AgentLogout");
	m_lstEvent.InsertItem( 31, "AgentSetBusy");
	m_lstEvent.InsertItem( 32, "AgentInServer");
	m_lstEvent.InsertItem( 33, "AgentAnswer");
	m_lstEvent.InsertItem( 34, "AgentMakeCall");
	m_lstEvent.InsertItem( 35, "AgentTransfer");
	m_lstEvent.InsertItem( 36, "AgentConfer");
	m_lstEvent.InsertItem( 37, "AgentHangup");
	m_lstEvent.InsertItem( 38, "AgentSendData");
	m_lstEvent.InsertItem( 39, "AgentSendFax");
	m_lstEvent.InsertItem( 40, "Ringing");
	m_lstEvent.InsertItem( 41, "Nothing");
	m_lstEvent.InsertItem( 42, "EOF==TRUE");
	m_lstEvent.InsertItem( 43, "AgentHold");
	m_lstEvent.InsertItem( 44, "AgentPickupHold");
	m_lstEvent.InsertItem( 45, "AgentMonitor");
	m_lstEvent.InsertItem( 46, "AgentDisMonitor");
	m_lstEvent.InsertItem( 47, "AgentBuildConfer");
	m_lstEvent.InsertItem( 48, "ElapseTime");
	m_lstEvent.InsertItem( 49, "CancelTransfer");
	m_lstEvent.InsertItem( 50, "AgentGetActiveCall");


	m_lstEvent.SetItemData( 0,NEXTTYPE_NEXT);
	m_lstEvent.SetItemData( 1,NEXTTYPE_ERROR);
	m_lstEvent.SetItemData( 2,NEXTTYPE_BUSY);
	m_lstEvent.SetItemData( 3,NEXTTYPE_NO_DIALTONE);
	m_lstEvent.SetItemData( 4,NEXTTYPE_NO_ANSWER );
	m_lstEvent.SetItemData( 5,NEXTTYPE_NO_RINGBACK);
	m_lstEvent.SetItemData( 6,NEXTTYPE_CONNECT);
	m_lstEvent.SetItemData( 7,NEXTTYPE_NOMATCH);
	m_lstEvent.SetItemData( 8,NEXTTYPE_DTMF_0);
	m_lstEvent.SetItemData( 9,NEXTTYPE_DTMF_1);
	m_lstEvent.SetItemData( 10,NEXTTYPE_DTMF_2);
	m_lstEvent.SetItemData( 11,NEXTTYPE_DTMF_3);
	m_lstEvent.SetItemData( 12,NEXTTYPE_DTMF_4);
	m_lstEvent.SetItemData( 13,NEXTTYPE_DTMF_5);
	m_lstEvent.SetItemData( 14,NEXTTYPE_DTMF_6);
	m_lstEvent.SetItemData( 15,NEXTTYPE_DTMF_7);
	m_lstEvent.SetItemData( 16,NEXTTYPE_DTMF_8);
	m_lstEvent.SetItemData( 17,NEXTTYPE_DTMF_9);
	m_lstEvent.SetItemData( 18,NEXTTYPE_DTMF_10);
	m_lstEvent.SetItemData( 19,NEXTTYPE_DTMF_11);
	m_lstEvent.SetItemData( 20,NEXTTYPE_DTMF_TIMEOUT);
	m_lstEvent.SetItemData( 21,NEXTTYPE_OFFHOOK);
	m_lstEvent.SetItemData( 22,NEXTTYPE_HANGUP);
	m_lstEvent.SetItemData( 23,NEXTTYPE_HOOKFLASH);
	m_lstEvent.SetItemData( 24,NEXTTYPE_RECDTMF);
	m_lstEvent.SetItemData( 25,NEXTTYPE_RECFAXTONE);
	m_lstEvent.SetItemData( 26,NEXTTYPE_AGENT_READY);
	m_lstEvent.SetItemData( 27,NEXTTYPE_AGENT_NOTREADY);
	m_lstEvent.SetItemData( 28,NEXTTYPE_CONFDESTROYED);
	m_lstEvent.SetItemData( 29,NEXTTYPE_AGENT_LOGIN_MSG);
	m_lstEvent.SetItemData( 30,NEXTTYPE_AGENT_OUT_MSG);
	m_lstEvent.SetItemData( 31,NEXTTYPE_AGENT_BUSY_MSG);
	m_lstEvent.SetItemData( 32,NEXTTYPE_AGENT_INSERVE_MSG);
	m_lstEvent.SetItemData( 33,NEXTTYPE_AGENT_ANSWER_MSG);
	m_lstEvent.SetItemData( 34,NEXTTYPE_AGENT_MAKECALL_MSG );
	m_lstEvent.SetItemData( 35,NEXTTYPE_AGENT_TRANSFER_MSG );
	m_lstEvent.SetItemData( 36,NEXTTYPE_AGENT_CONFER_MSG);
	m_lstEvent.SetItemData( 37,NEXTTYPE_AGENT_HANGUP_MSG);
	m_lstEvent.SetItemData( 38,NEXTTYPE_AGENT_SENDDATA_MSG );
	m_lstEvent.SetItemData( 39,NEXTTYPE_AGENT_SENDFAX_MSG);
	m_lstEvent.SetItemData( 40,NEXTTYPE_RINGING);
	m_lstEvent.SetItemData( 41,NEXTTYPE_NOTHING);
	m_lstEvent.SetItemData( 42,NEXTTYPE_EOF_FALSE);
	m_lstEvent.SetItemData( 43,NEXTTYPE_AGENT_HOLD_MSG);
	m_lstEvent.SetItemData( 44,NEXTTYPE_AGENT_PICKUPHOLD_MSG);
	m_lstEvent.SetItemData( 45,NEXTTYPE_AGENT_MONITOR_MSG);
	m_lstEvent.SetItemData( 46,NEXTTYPE_AGENT_DISMONITOR_MSG);
	m_lstEvent.SetItemData( 47,NEXTTYPE_AGENT_BUILDCONFER_MSG);
	m_lstEvent.SetItemData( 48,NEXTTYPE_ELAPSE_TIME);
	m_lstEvent.SetItemData( 49,NEXTTYPE_CANCEL_TRANSFER);
	m_lstEvent.SetItemData( 50,NEXTTYPE_AGENT_GETACTIVECALL);





	return;
}

void CAddEventDlg::OnClickListEvent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstEvent.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;

	m_nEventType = nIndex;	
	m_strEventName = m_lstEvent.GetItemText( nIndex,0 );	
	*pResult = 0;
}

void CAddEventDlg::OnDblclkListEvent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_lstEvent.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return ;

	m_nEventType = nIndex;	
	m_strEventName = m_lstEvent.GetItemText( nIndex,0 );	
	*pResult = 0;
	this->OnOK();
}
