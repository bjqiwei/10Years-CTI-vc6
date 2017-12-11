// EventCheckPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "EventCheckPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventCheckPageSpc property page

IMPLEMENT_DYNCREATE(CEventCheckPageSpc, CPropertyPage)

CEventCheckPageSpc::CEventCheckPageSpc() : CPropertyPage(CEventCheckPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CEventCheckPageSpc)
	m_nElapseTime = 60;
	m_bClearDTMF = FALSE;
	m_bPlay = FALSE;
	m_nPlayType = -1;
	m_strPlayDTMF = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
	m_strPlayFile = "";
}

CEventCheckPageSpc::~CEventCheckPageSpc()
{
}

void CEventCheckPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventCheckPageSpc)
	DDX_Control(pDX, IDC_EDIT_PLAYFILE, m_edtPlayFile);
	DDX_Control(pDX, IDC_LIST_EVENT, m_listEvent);
	DDX_Text(pDX, IDC_EDIT_ELAPSETIME, m_nElapseTime);
	DDV_MinMaxInt(pDX, m_nElapseTime, 0, 6000);
	DDX_Check(pDX, IDC_CHECK_CLEARDTMF, m_bClearDTMF);
	DDX_Check(pDX, IDC_CHECK_PLAY, m_bPlay);
	DDX_CBIndex(pDX, IDC_COMBO_PLAYTYPE, m_nPlayType);
	DDX_Text(pDX, IDC_EDIT_SSPLAY_DTMF, m_strPlayDTMF);
	DDV_MaxChars(pDX, m_strPlayDTMF, 1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventCheckPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CEventCheckPageSpc)
	ON_BN_CLICKED(IDC_BUTTON_PLAYFILE, OnButtonPlayfile)
	ON_BN_CLICKED(IDC_CHECK_PLAY, OnCheckPlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventCheckPageSpc message handlers

void CEventCheckPageSpc::EditNextNode(int nType, BOOL bEditType)
{
	CNextNode * pObj  = m_pNodeObj->FindNextNodeObj( nType);

	switch( nType )
	{
	case NEXTTYPE_ERROR:
		if( bEditType )//添加
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("Error",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_OFFHOOK:
		if( bEditType )//添加摘机
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("OffHook",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_HANGUP:
		if( bEditType )//添加挂机
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("HangUp",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_HOOKFLASH:
		if( bEditType )//添加拍叉簧
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("HookFlash",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_RECDTMF:
		if( bEditType )//添加收到按键
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("RecDTMF",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_RECFAXTONE:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("RecFaxTone",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_READY:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentReady",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_NOTREADY:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentNotReady",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_CONFDESTROYED:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("ConfDestroyed",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
		

	case NEXTTYPE_AGENT_LOGIN_MSG:
		if( bEditType )//
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentLogin",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_OUT_MSG:
		if( bEditType )//
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentLogOut",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;		
	case NEXTTYPE_AGENT_BUSY_MSG:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentBusy",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_INSERVE_MSG:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentInservice",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_ANSWER_MSG:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentAnswer",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_MAKECALL_MSG:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentMakeCall",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_TRANSFER_MSG:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentTransfer",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_CONFER_MSG:
		if( bEditType )
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentConference",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_HANGUP_MSG:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentHangUp",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_SENDDATA_MSG:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentSendData",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_SENDFAX_MSG:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentSendFax",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_RINGING:
		if( bEditType )//添加收到振铃
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("Ringing",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_NOTHING:
		if( bEditType )//添加收到振铃
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("Nothing",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_HOLD_MSG:
		if( bEditType )//添加收到振铃
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentHold",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_PICKUPHOLD_MSG:
		if( bEditType )//添加收到振铃
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentPickUpHold",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_MONITOR_MSG:
		if( bEditType )//添加收到振铃
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentMonitor",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_DISMONITOR_MSG:
		if( bEditType )//添加收到振铃
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentDisMonitor",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_BUILDCONFER_MSG:
		if( bEditType )//添加收到振铃
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentBuildConfer",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_ELAPSE_TIME:
		if( bEditType )//添加时钟
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("ElapseTime",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_AGENT_GETACTIVECALL:
		if( bEditType )//取得活动呼叫
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("AgentGetActiveCall",_T(""),DATATYPE_INTEGER,nType,1);
			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	default:
		break;


	}
	


	return;
}

void CEventCheckPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;

	UpdateData(TRUE);
	m_edtPlayFile.GetWindowText(m_strPlayFile);
	m_pNodeObj->m_bOffHook		= m_listEvent.GetCheck(0);
	m_pNodeObj->m_bHangUp		= m_listEvent.GetCheck(1);
	m_pNodeObj->m_bHookFlash	= m_listEvent.GetCheck(2);
	m_pNodeObj->m_bRecDTMF		= m_listEvent.GetCheck(3);
	m_pNodeObj->m_bRecFaxTone	= m_listEvent.GetCheck(4);
	m_pNodeObj->m_bError		= m_listEvent.GetCheck(5);
	m_pNodeObj->m_bAppExit		= m_listEvent.GetCheck(6);
	m_pNodeObj->m_bFaxSendTone	= m_listEvent.GetCheck(7);
	m_pNodeObj->m_bConfDestroyed	= m_listEvent.GetCheck(8);
	m_pNodeObj->m_bAgentLoginMsg	= m_listEvent.GetCheck(9);
	m_pNodeObj->m_bAgentLogOutMsg	= m_listEvent.GetCheck(10);
	m_pNodeObj->m_bAgentBusyMsg		= m_listEvent.GetCheck(11);
	m_pNodeObj->m_bAgentInserveMsg	= m_listEvent.GetCheck(12);
	m_pNodeObj->m_bAgentAnswerMsg	= m_listEvent.GetCheck(13);
	m_pNodeObj->m_bAgentMakeCallMsg	= m_listEvent.GetCheck(14);
	m_pNodeObj->m_bAgentTransferMsg	= m_listEvent.GetCheck(15);
	m_pNodeObj->m_bAgentConferMsg		= m_listEvent.GetCheck(16);
	m_pNodeObj->m_bAgentHangUpMsg		= m_listEvent.GetCheck(17);
	m_pNodeObj->m_bAgentSendDataMsg		= m_listEvent.GetCheck(18);
	m_pNodeObj->m_bAgentSendFaxMsg		= m_listEvent.GetCheck(19);
	m_pNodeObj->m_bRinging		= m_listEvent.GetCheck(20);
	m_pNodeObj->m_bNothing		= m_listEvent.GetCheck(21);
	m_pNodeObj->m_bAgentHold	= m_listEvent.GetCheck(22);
	m_pNodeObj->m_bAgentPickupHold	= m_listEvent.GetCheck(23);
	m_pNodeObj->m_bAgentMonitor		= m_listEvent.GetCheck(24);
	m_pNodeObj->m_bAgentDisMonitor	= m_listEvent.GetCheck(25);
	m_pNodeObj->m_bAgentBuildConfer	= m_listEvent.GetCheck(26);
	m_pNodeObj->m_bElapseTime		= m_listEvent.GetCheck(27);
	m_pNodeObj->m_nElapseTime		= m_nElapseTime;		
	m_pNodeObj->m_bClearDTMF		= m_bClearDTMF;
	m_pNodeObj->m_bAgentGetActiveCall = m_listEvent.GetCheck(28);

	m_pNodeObj->m_bPlay = m_bPlay;
	m_pNodeObj->m_strPlayFile =m_strPlayFile;
	m_pNodeObj->m_nPlayType= m_nPlayType;
	m_pNodeObj->m_strPlayDTMF= m_strPlayDTMF;
	

	EditNextNode(NEXTTYPE_OFFHOOK, m_listEvent.GetCheck(0) );
	EditNextNode(NEXTTYPE_HANGUP, m_listEvent.GetCheck(1) );
	EditNextNode(NEXTTYPE_HOOKFLASH, m_listEvent.GetCheck(2) );
	EditNextNode(NEXTTYPE_RECDTMF, m_listEvent.GetCheck(3) );
	EditNextNode(NEXTTYPE_RECFAXTONE, m_listEvent.GetCheck(4) );
	EditNextNode(NEXTTYPE_ERROR, m_listEvent.GetCheck(5) );
	EditNextNode(NEXTTYPE_AGENT_READY, m_listEvent.GetCheck(6) );
	EditNextNode(NEXTTYPE_AGENT_NOTREADY, m_listEvent.GetCheck(7) );
	EditNextNode(NEXTTYPE_CONFDESTROYED, m_listEvent.GetCheck(8) );

	EditNextNode(NEXTTYPE_AGENT_LOGIN_MSG, m_listEvent.GetCheck(9) );
	EditNextNode(NEXTTYPE_AGENT_OUT_MSG, m_listEvent.GetCheck(10) );
	EditNextNode(NEXTTYPE_AGENT_BUSY_MSG, m_listEvent.GetCheck(11) );

	EditNextNode(NEXTTYPE_AGENT_INSERVE_MSG, m_listEvent.GetCheck(12) );
	EditNextNode(NEXTTYPE_AGENT_ANSWER_MSG, m_listEvent.GetCheck(13) );
	EditNextNode(NEXTTYPE_AGENT_MAKECALL_MSG, m_listEvent.GetCheck(14) );

	EditNextNode(NEXTTYPE_AGENT_TRANSFER_MSG, m_listEvent.GetCheck(15) );
	EditNextNode(NEXTTYPE_AGENT_CONFER_MSG, m_listEvent.GetCheck(16) );

	EditNextNode(NEXTTYPE_AGENT_HANGUP_MSG, m_listEvent.GetCheck(17) );
	EditNextNode(NEXTTYPE_AGENT_SENDDATA_MSG, m_listEvent.GetCheck(18) );
	EditNextNode(NEXTTYPE_AGENT_SENDFAX_MSG, m_listEvent.GetCheck(19) );
	EditNextNode(NEXTTYPE_RINGING, m_listEvent.GetCheck(20) );
	EditNextNode(NEXTTYPE_NOTHING, m_listEvent.GetCheck(21) );
	EditNextNode(NEXTTYPE_AGENT_HOLD_MSG, m_listEvent.GetCheck(22) );
	EditNextNode(NEXTTYPE_AGENT_PICKUPHOLD_MSG, m_listEvent.GetCheck(23) );
	EditNextNode(NEXTTYPE_AGENT_MONITOR_MSG, m_listEvent.GetCheck(24) );
	EditNextNode(NEXTTYPE_AGENT_DISMONITOR_MSG, m_listEvent.GetCheck(25) );
	EditNextNode(NEXTTYPE_AGENT_BUILDCONFER_MSG, m_listEvent.GetCheck(26) );
	EditNextNode(NEXTTYPE_ELAPSE_TIME, m_listEvent.GetCheck(27) );
	EditNextNode(NEXTTYPE_AGENT_GETACTIVECALL, m_listEvent.GetCheck(28) );


	m_pNodeObj->UpDate();

	
	CPropertyPage::OnOK();
}

BOOL CEventCheckPageSpc::OnInitDialog() 
{	
	CPropertyPage::OnInitDialog();
	m_edtPlayFile.SetWindowText(m_strPlayFile);
	m_listEvent.SetExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );
	
	m_listEvent.InsertItem(0,"OffHook");
	m_listEvent.InsertItem(1,"HangUp");
	m_listEvent.InsertItem(2,"HookFlash");
	m_listEvent.InsertItem(3,"RecDTMF");
	m_listEvent.InsertItem(4,"RecFaxTone");
	m_listEvent.InsertItem(5,"Error");
	m_listEvent.InsertItem(6,"AgentReady");
	m_listEvent.InsertItem(7,"AgentNotReady");
	m_listEvent.InsertItem(8,"ConfDestroyed");
	m_listEvent.InsertItem(9,"AgentLogin");
	m_listEvent.InsertItem(10,"AgentLogOut");
	m_listEvent.InsertItem(11,"AgentBusy");
	m_listEvent.InsertItem(12,"AgentInservice");
	m_listEvent.InsertItem(13,"AgentAnswer");
	m_listEvent.InsertItem(14,"AgentMakeCall");
	m_listEvent.InsertItem(15,"AgentTransfer");
	m_listEvent.InsertItem(16,"AgentConference");
	m_listEvent.InsertItem(17,"AgentHangUp");
	m_listEvent.InsertItem(18,"AgentSendData");
	m_listEvent.InsertItem(19,"AgentSendFax");
	m_listEvent.InsertItem(20,"Ringing");
	m_listEvent.InsertItem(21,"Nothing");
	m_listEvent.InsertItem(22,"AgentHold");
	m_listEvent.InsertItem(23,"AgentPickupHold");
	m_listEvent.InsertItem(24,"AgentMonitor");
	m_listEvent.InsertItem(25,"AgentDisMonitor");
	m_listEvent.InsertItem(26,"AgentBulidConfer");
	m_listEvent.InsertItem(27,"ElapseTime");
	m_listEvent.InsertItem(28,"AgentGetActiveCall");
	if( NULL == m_pNodeObj ) return TRUE;

	
	m_listEvent.SetCheck(0,m_pNodeObj->m_bOffHook);
	m_listEvent.SetCheck(1,m_pNodeObj->m_bHangUp);
	m_listEvent.SetCheck(2,m_pNodeObj->m_bHookFlash);
	m_listEvent.SetCheck(3,m_pNodeObj->m_bRecDTMF);
	m_listEvent.SetCheck(4,m_pNodeObj->m_bRecFaxTone);
	m_listEvent.SetCheck(5,m_pNodeObj->m_bError);
	m_listEvent.SetCheck(6,m_pNodeObj->m_bAppExit);
	m_listEvent.SetCheck(7,m_pNodeObj->m_bFaxSendTone);
	m_listEvent.SetCheck(8,m_pNodeObj->m_bConfDestroyed);
	m_listEvent.SetCheck(9,m_pNodeObj->m_bAgentLoginMsg);
	m_listEvent.SetCheck(10,m_pNodeObj->m_bAgentLogOutMsg);
	m_listEvent.SetCheck(11,m_pNodeObj->m_bAgentBusyMsg);
	m_listEvent.SetCheck(12,m_pNodeObj->m_bAgentInserveMsg);
	m_listEvent.SetCheck(13,m_pNodeObj->m_bAgentAnswerMsg);
	m_listEvent.SetCheck(14,m_pNodeObj->m_bAgentMakeCallMsg);
	m_listEvent.SetCheck(15,m_pNodeObj->m_bAgentTransferMsg);
	m_listEvent.SetCheck(16,m_pNodeObj->m_bAgentConferMsg);
	m_listEvent.SetCheck(17,m_pNodeObj->m_bAgentHangUpMsg);
	m_listEvent.SetCheck(18,m_pNodeObj->m_bAgentSendDataMsg);
	m_listEvent.SetCheck(19,m_pNodeObj->m_bAgentSendFaxMsg);
	m_listEvent.SetCheck(20,m_pNodeObj->m_bRinging);
	m_listEvent.SetCheck(21,m_pNodeObj->m_bNothing);
	m_listEvent.SetCheck(22,m_pNodeObj->m_bAgentHold);
	m_listEvent.SetCheck(23,m_pNodeObj->m_bAgentPickupHold);		
	m_listEvent.SetCheck(24,m_pNodeObj->m_bAgentMonitor);
	m_listEvent.SetCheck(25,m_pNodeObj->m_bAgentDisMonitor);
	m_listEvent.SetCheck(26,m_pNodeObj->m_bAgentBuildConfer);
	m_listEvent.SetCheck(27,m_pNodeObj->m_bElapseTime);
	m_listEvent.SetCheck(28,m_pNodeObj->m_bAgentGetActiveCall);

	MyEnableWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEventCheckPageSpc::InitPage(CEventCheckObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;
	m_bClearDTMF = m_pNodeObj->m_bClearDTMF;
	m_nElapseTime = m_pNodeObj->m_nElapseTime;
	m_bPlay = m_pNodeObj->m_bPlay;
	m_strPlayFile = m_pNodeObj->m_strPlayFile;
	m_nPlayType= m_pNodeObj->m_nPlayType;
	m_strPlayDTMF= m_pNodeObj->m_strPlayDTMF;

	return;
}

void CEventCheckPageSpc::OnButtonPlayfile() 
{
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_edtPlayFile.SetSel(-1,0);
		m_edtPlayFile.ReplaceSel(str);
		UpdateData(FALSE);
	}

	return;
	
}

void CEventCheckPageSpc::OnCheckPlay() 
{
	UpdateData(TRUE);
	MyEnableWindow();
}

void CEventCheckPageSpc::MyEnableWindow()
{
	if( m_bPlay )
	{
		CWnd * phWnd = GetDlgItem(IDC_COMBO_PLAYTYPE);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_BUTTON_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
		phWnd = GetDlgItem(IDC_EDIT_SSPLAY_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( TRUE);
	}
	else
	{
		CWnd * phWnd = GetDlgItem(IDC_COMBO_PLAYTYPE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_BUTTON_PLAYFILE);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);
		phWnd = GetDlgItem(IDC_EDIT_SSPLAY_DTMF);
		if( NULL != phWnd )
			phWnd->EnableWindow( FALSE);

	}	

	return;
}
