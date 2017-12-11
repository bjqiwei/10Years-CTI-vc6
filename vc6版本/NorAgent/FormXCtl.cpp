// FormXCtl.cpp : Implementation of the CFormXCtrl ActiveX Control class.

#include "stdafx.h"
#include "NorAgent.h"
#include "FormXCtl.h"
#include "FormXPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CFormXCtrl, CFormControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CFormXCtrl, CFormControl)
	//{{AFX_MSG_MAP(CFormXCtrl)
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_HANGUP, OnBtnHangup)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_READY, OnBtnReady)
	ON_BN_CLICKED(IDC_BTN_OUTCALL, OnBtnOutcall)
	ON_BN_CLICKED(IDC_BTN_MONITOR, OnBtnMonitor)
	ON_BN_CLICKED(IDC_BTN_JOINCONF, OnBtnJoinconf)
	ON_BN_CLICKED(IDC_BTN_HOLD, OnBtnHold)
	ON_BN_CLICKED(IDC_BTN_GETACTIVECALL, OnBtnGetactivecall)
	ON_BN_CLICKED(IDC_BTN_SENDDATA, OnBtnSenddata)
	ON_BN_CLICKED(IDC_BTN_TRANSFER, OnBtnTransfer)
	ON_BN_CLICKED(IDC_BTN_REQUESTCONF, OnBtnRequestconf)
	ON_BN_CLICKED(IDC_BTN_SETBUSY, OnBtnSetbusy)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CFormXCtrl, CFormControl)
	//{{AFX_DISPATCH_MAP(CFormXCtrl)
	DISP_FUNCTION(CFormXCtrl, "AgentToAgentData", AgentToAgentData, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "AnswerCall", AnswerCall, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "CallAgent", CallAgent, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "CancelTransfer", CancelTransfer, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "ConferCall", ConferCall, VT_BOOL, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "DisMonitor", DisMonitor, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "GetActiveCall", GetActiveCall, VT_BOOL, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "HangUp", HangUp, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "Hold", Hold, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "JoinConference", JoinConference, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "Monitor", Monitor, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "OutCall", OutCall, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "PickupHold", PickupHold, VT_BOOL, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "TransferToAgent", TransferToAgent, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "TransferToIVR", TransferToIVR, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "TransferToOut", TransferToOut, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "InitNorAgent", InitNorAgent, VT_BOOL, VTS_I2 VTS_BSTR VTS_I2)
	DISP_FUNCTION(CFormXCtrl, "ExitNorAgent", ExitNorAgent, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "LogIn", LogIn, VT_BOOL, VTS_BSTR VTS_BSTR VTS_I2)
	DISP_FUNCTION(CFormXCtrl, "LogOut", LogOut, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CFormXCtrl, "Ready", Ready, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "NotReady", NotReady, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "SetBusy", SetBusy, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CFormXCtrl, "SetInServer", SetInServer, VT_BOOL, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CFormXCtrl, CFormControl)
	//{{AFX_EVENT_MAP(CFormXCtrl)
	EVENT_CUSTOM("OnHangup", FireOnHangup, VTS_NONE)
	EVENT_CUSTOM("OnNetAbend", FireOnNetAbend, VTS_NONE)
	EVENT_CUSTOM("OnOffHook", FireOnOffHook, VTS_NONE)
	EVENT_CUSTOM("OnReciveAgentData", FireOnReciveAgentData, VTS_I2  VTS_BSTR)
	EVENT_CUSTOM("OnResponesMakeCall", FireOnResponesMakeCall, VTS_BOOL)
	EVENT_CUSTOM("OnResponseAnswerCall", FireOnResponseAnswerCall, VTS_BOOL)
	EVENT_CUSTOM("OnResponseCancelTransfer", FireOnResponseCancelTransfer, VTS_BOOL)
	EVENT_CUSTOM("OnResponseConferCall", FireOnResponseConferCall, VTS_BOOL)
	EVENT_CUSTOM("OnResponseDisMonitor", FireOnResponseDisMonitor, VTS_BOOL)
	EVENT_CUSTOM("OnResponseHold", FireOnResponseHold, VTS_BOOL)
	EVENT_CUSTOM("OnResponseJoinConfer", FireOnResponseJoinConfer, VTS_BOOL)
	EVENT_CUSTOM("OnResponseLogin", FireOnResponseLogin, VTS_BOOL)
	EVENT_CUSTOM("OnResponseMonitor", FireOnResponseMonitor, VTS_BOOL)
	EVENT_CUSTOM("OnResponseTransfer", FireOnResponseTransfer, VTS_BOOL)
	EVENT_CUSTOM("OnRinging", FireOnRinging, VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("OnResponsePickUpHold", FireOnResponsePickUpHold, VTS_BOOL  VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("OnResponseLogOut", FireOnResponseLogOut, VTS_BOOL)
	EVENT_CUSTOM("OnInitNorAgentResponse", FireOnInitNorAgentResponse, VTS_BOOL)
	EVENT_CUSTOM("OnResponseReady", FireOnResponseReady, VTS_BOOL)
	EVENT_CUSTOM("OnResponseNotReady", FireOnResponseNotReady, VTS_BOOL)
	EVENT_CUSTOM("OnClickMonitor", FireOnClickMonitor, VTS_NONE)
	EVENT_CUSTOM("OnClickReady", FireOnClickReady, VTS_NONE)
	EVENT_CUSTOM("OnClickHold", FireOnClickHold, VTS_NONE)
	EVENT_CUSTOM("OnClickJoinConf", FireOnClickJoinConf, VTS_NONE)
	EVENT_CUSTOM("OnClickConfCall", FireOnClickConfCall, VTS_NONE)
	EVENT_CUSTOM("OnClickLogin", FireOnClickLogin, VTS_NONE)
	EVENT_CUSTOM("OnClickTransfer", FireOnClickTransfer, VTS_NONE)
	EVENT_CUSTOM("OnClickGetActiveCall", FireOnClickGetActiveCall, VTS_NONE)
	EVENT_CUSTOM("OnClickSendDate", FireOnClickSendDate, VTS_NONE)
	EVENT_CUSTOM("OnClickHangUp", FireOnClickHangUp, VTS_NONE)
	EVENT_CUSTOM("OnClickOutCall", FireOnClickOutCall, VTS_NONE)
	EVENT_CUSTOM("OnClickLogOut", FireOnClickLogOut, VTS_NONE)
	EVENT_CUSTOM("OnClickNotReady", FireOnClickNotReady, VTS_NONE)
	EVENT_CUSTOM("OnClickDisMonitor", FireOnClickDisMonitor, VTS_NONE)
	EVENT_CUSTOM("OnClickCancelTransfer", FireOnClickCancelTransfer, VTS_NONE)
	EVENT_CUSTOM("OnClickPickUpHold", FireOnClickPickUpHold, VTS_NONE)
	EVENT_CUSTOM("OnResponseGetActive", FireOnResponseGetActive, VTS_BOOL  VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("OnClickRequestConf", FireOnClickRequestConf, VTS_NONE)
	EVENT_CUSTOM("OnResponseSetBusy", FireOnResponseSetBusy, VTS_BOOL)
	EVENT_CUSTOM("OnResponseSetInServer", FireOnResponseSetInServer, VTS_BOOL)
	EVENT_CUSTOM("OnClickSetBusy", FireOnClickSetBusy, VTS_NONE)
	EVENT_CUSTOM("OnClickInServer", FireOnClickInServer, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CFormXCtrl, 1)
	PROPPAGEID(CFormXPropPage::guid)
END_PROPPAGEIDS(CFormXCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

//IMPLEMENT_OLECREATE_EX(CFormXCtrl, "FORMX.FormXCtrl.1",
IMPLEMENT_OLECREATE_EX(CFormXCtrl, "NorAgent.NorAgent.4",
	0xafdc1587, 0xf993, 0x11d1, 0x96, 0xed, 0xf0, 0x5f, 0x3, 0xc1, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CFormXCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_NorAgent =
		{ 0xafdc1585, 0xf993, 0x11d1, { 0x96, 0xed, 0xf0, 0x5f, 0x3, 0xc1, 0, 0 } };
const IID BASED_CODE IID_NorAgentEvents =
		{ 0xafdc1586, 0xf993, 0x11d1, { 0x96, 0xed, 0xf0, 0x5f, 0x3, 0xc1, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwFormXOleMisc =
	OLEMISC_SIMPLEFRAME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CFormXCtrl, IDS_FORMX, _dwFormXOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::CFormXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CFormXCtrl

BOOL CFormXCtrl::CFormXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_FORMX,
			IDB_FORMX,
			afxRegApartmentThreading,
			_dwFormXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::CFormXCtrl - Constructor

CFormXCtrl::CFormXCtrl():
CFormControl(CFormXCtrl::IDD, TRUE)
{

	InitializeIIDs(&IID_NorAgent, &IID_NorAgentEvents);

	EnableSimpleFrame();

	m_pMsgDealWith = NULL;
	m_pMsgDealWith = &m_MsgDealWith;
	m_lpIP = "127.0.0.1";
	m_nPort = 7000;
	m_nDN = 0;
	m_bInit = FALSE;


	m_bLogin = FALSE;
	m_bSetBusy = FALSE;
	m_bMonitor = FALSE;
	m_bTransfer = FALSE;

	m_strAgentID = "";
	m_nGroup = -1;

	//{{AFX_DATA_INIT(CFormXCtrl)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::~CFormXCtrl - Destructor

CFormXCtrl::~CFormXCtrl()
{

}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::OnDraw - Drawing function

void CFormXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!m_hWnd)
	{
		CFormControl::OnDraw(pdc, rcBounds, rcInvalid);
	}
	else
	{



		
	//TODO: Replace the following code with your own drawing code.
			pdc->FillSolidRect( &rcBounds, RGB(29,97,168) );
			//RGB(29,97,168)
			
			//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(pdc->GetBkColor())));
		//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
		//pdc->Ellipse(rcBounds);
	}
}
void CFormXCtrl::OnInitialUpdate()
{
	ASSERT_VALID(this);
	

	InitAllButton();


	if (!UpdateData(FALSE))
		TRACE0("UpdateData failed during CFormControl initial update.\n");
	SetFocus( );
}

/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::DoPropExchange - Persistence support

void CFormXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CFormControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

	// {{AFX_DATA(CFormXCtrl)
		enum {IDD = IDD_FORMX};
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::OnResetState - Reset control to default state

void CFormXCtrl::OnResetState()
{
	CFormControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl message handlers




void CFormXCtrl::DoDataExchange(CDataExchange* pDX) 
{
	//{{AFX_DATA_MAP(CXFormCtrl)
	DDX_Control(pDX, IDC_BTN_LOGIN, m_ctlLoginBtn);
	DDX_Control(pDX, IDC_BTN_HANGUP, m_ctlHangUpBtn);
	DDX_Control(pDX, IDC_BTN_TRANSFER, m_ctlTransferBtn);
	DDX_Control(pDX, IDC_BTN_HOLD, m_ctlHold);
	DDX_Control(pDX, IDC_BTN_READY,m_ctlReady);
	DDX_Control(pDX, IDC_BTN_OUTCALL,m_ctlOutCall);
	DDX_Control(pDX, IDC_BTN_MONITOR,m_ctlMonitor );
	DDX_Control(pDX, IDC_BTN_GETACTIVECALL,m_ctlGetCall );
	DDX_Control(pDX, IDC_BTN_REQUESTCONF,m_ctlRequestConf );
	DDX_Control(pDX, IDC_BTN_JOINCONF,m_ctlJoinConf );
	DDX_Control(pDX, IDC_BTN_SENDDATA,m_ctlSendData );
	DDX_Control(pDX, IDC_BTN_SETBUSY,m_ctlSetBusy);
	//}}AFX_DATA_MAP
	
	//CFormControl::DoDataExchange(pDX);
}

void CFormXCtrl::InitAllButton()
{
	SetButton( &m_ctlLoginBtn,IDI_ICON_LOGIN,"登陆", TRUE);
	m_bInit = FALSE;

	SetButton( &m_ctlHangUpBtn,IDI_ICON_HANGUP,"挂机", FALSE);
	m_bInit = FALSE;

	SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移", FALSE);
	m_bTransfer = FALSE;

	SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持", FALSE);
	m_bHold = TRUE;

	SetButton( &m_ctlReady,IDI_ICON_READY,"就绪", FALSE);
	m_bReady = FALSE;

	SetButton( &m_ctlOutCall,IDI_ICON_MAKECALL,"外拨", FALSE);

	SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听", FALSE);
	m_bMonitor = FALSE;

	SetButton( &m_ctlGetCall,IDI_ICON_GETACTIVECALL,"获取", FALSE);
	m_bMonitor = TRUE;

	SetButton( &m_ctlRequestConf,IDI_ICON_REQUESTCONF,"召开会议", FALSE);

	SetButton( &m_ctlJoinConf,IDI_ICON_JOINCONF,"参加会议", FALSE);
	
	SetButton( &m_ctlSendData,IDI_ICON_SENDDATA,"交流", FALSE);
	
	SetButton( &m_ctlSetBusy,IDI_ICON_INSERVER,"置闲", FALSE);
	m_bSetBusy = FALSE;
	return;

}




void CFormXCtrl::OnBtnHangup() 
{

	FireOnClickHangUp();
	SetFocus( );
}



BOOL CFormXCtrl::LogIn(LPCTSTR AgentID, LPCTSTR PWD, short Group) 
{
	if( NULL == AgentID || "" == AgentID  ){
		return FALSE;
	}
	if( NULL == PWD || "" == PWD  ){
		return FALSE;
	}
	m_strAgentID = AgentID;
	m_nGroup = Group;

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_LOGIN;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_LOGIN newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_LOGIN) );
	memmove( newMsg.szAgentID, AgentID, strlen(AgentID) > sizeof(newMsg.szAgentID) ? sizeof(newMsg.szAgentID) :  strlen(AgentID)  );
	memmove( newMsg.szPWD, PWD, strlen(PWD) > sizeof(newMsg.szPWD) ? sizeof(newMsg.szPWD) :  strlen(PWD)  );
	newMsg.nGroup = Group;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_LOGIN) );

	return SendData( (char *)&msg, sizeof(msg));

}

BOOL CFormXCtrl::LogOut(LPCTSTR PWD) 
{

	if( NULL == PWD || "" == PWD  ){
		return FALSE;
	}
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_LOGOUT;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;


	
	CFMSG_LOGOUT newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_LOGOUT) );
	memmove( newMsg.szAgentID, (LPCTSTR)m_strAgentID, strlen((LPCTSTR)m_strAgentID) > sizeof(newMsg.szAgentID) ? sizeof(newMsg.szAgentID) :  strlen((LPCTSTR)m_strAgentID)  );
	memmove( newMsg.szPWD, PWD, strlen(PWD) > sizeof(newMsg.szPWD) ? sizeof(newMsg.szPWD) :  strlen(PWD)  );
	newMsg.nGroup = m_nGroup;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_LOGOUT) );

	return SendData( (char *)&msg, sizeof(msg));
	return TRUE;
}

BOOL CFormXCtrl::AgentToAgentData(LPCTSTR AimDN, LPCTSTR Buf) 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_AGENT_TO_AGENT;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	atoi(AimDN);
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	memmove( msg.szMessageBody, Buf, strlen(Buf) > sizeof(msg.szMessageBody) ? sizeof(msg.szMessageBody) :  strlen(Buf)  );
	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::AnswerCall() 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_ANSWER;//reg message type
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;


	CFMSG_ANSWERCALL newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_ANSWERCALL) );
	newMsg.nResult = 0;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_ANSWERCALL) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::CallAgent(LPCTSTR AimDN) 
{
	if( NULL == AimDN || "" == AimDN  ){
		return FALSE;
	}
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_MCALL;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;


	CFMSG_MAKECALL recMsg;
	memset(&recMsg, 0, sizeof(recMsg) );
	recMsg.nCallType = 0;
	memmove( recMsg.szTel, AimDN, strlen(AimDN) > sizeof(recMsg.szTel) ? sizeof(recMsg.szTel) :  strlen(AimDN)  );
	memmove( msg.szMessageBody , &recMsg, sizeof(CFMSG_MAKECALL) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::CancelTransfer() 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_CANCEL_TRANSFER;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_CANCEL_TRANSFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_CANCEL_TRANSFER) );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_CANCEL_TRANSFER) );
	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::ConferCall(short nType, LPCTSTR Tel) 
{
	
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_CONFER_CALL;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;


	CFMSG_CONFER_CALL recMsg;
	memset(&recMsg, 0, sizeof(recMsg) );
	recMsg.nCallType = nType;
	memmove( recMsg.szTel, Tel, strlen(Tel) > sizeof(recMsg.szTel) ? sizeof(recMsg.szTel) :  strlen(Tel)  );
	memmove( msg.szMessageBody , &recMsg, sizeof(CFMSG_CONFER_CALL) );


	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::DisMonitor() 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_DMONITOR;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_MONITOR newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_MONITOR) );
	
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_MONITOR) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::GetActiveCall(short nType, LPCTSTR Tel) 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_GETACTIVECALL;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;


	CFMSG_GETACTIVECALL newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_GETACTIVECALL) );
	newMsg.nResult = 0;
	newMsg.nType = nType;
	memmove( newMsg.szTel, Tel, strlen(Tel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(Tel)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_GETACTIVECALL) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::HangUp() 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_HANGUP;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::Hold() 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_HOLD;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_HOLD newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_HOLD) );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_HOLD) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::JoinConference(LPCTSTR AimDN) 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_INCONFER;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;


	CFMSG_IN_CONFER recMsg;
	memset(&recMsg, 0, sizeof(recMsg) );
	memmove( recMsg.szTel, AimDN, strlen(AimDN) > sizeof(recMsg.szTel) ? sizeof(recMsg.szTel) :  strlen(AimDN)  );
	memmove( msg.szMessageBody , &recMsg, sizeof(CFMSG_IN_CONFER) );


	return SendData( (char *)&msg, sizeof(msg));

	return TRUE;
}

BOOL CFormXCtrl::Monitor(LPCTSTR AimDN) 
{
	if( NULL == AimDN || "" == AimDN  ){
		return FALSE;
	}
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_MONITOR;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_MONITOR newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_MONITOR) );
	memmove( newMsg.szTel, AimDN, strlen(AimDN) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(AimDN)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_MONITOR) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::OutCall(LPCTSTR Tel) 
{
	if( NULL == Tel || "" == Tel  ){
		return FALSE;
	}
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_MCALL;//reg message type
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;


	CFMSG_MAKECALL recMsg;
	memset(&recMsg, 0, sizeof(recMsg) );
	recMsg.nCallType = 1;
	memmove( recMsg.szTel, Tel, strlen(Tel) > sizeof(recMsg.szTel) ? sizeof(recMsg.szTel) :  strlen(Tel)  );
	memmove( msg.szMessageBody , &recMsg, sizeof(CFMSG_MAKECALL) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::PickupHold(short nType, LPCTSTR CallerID) 
{
	if( nType < 0 || nType > 1 ){
		return FALSE;
	}


	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_PICKUPHOLD;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_PICKUPHOLD newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_PICKUPHOLD) );
	newMsg.nType = nType;
	if( 1 == nType ){
	memmove( newMsg.szTel, CallerID, strlen(CallerID) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(CallerID)  );
	}
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_PICKUPHOLD) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::TransferToAgent(LPCTSTR AimDN) 
{
	if( NULL == AimDN || "" == AimDN  ){
		return FALSE;
	}
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_TRANSFER;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_TRANSFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_TRANSFER) );
	newMsg.nCallType = 0;
	memmove( newMsg.szTel, AimDN, strlen(AimDN) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(AimDN)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_TRANSFER) );
	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::TransferToIVR() 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_TRANSFER;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_TRANSFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_TRANSFER) );
	newMsg.nCallType = 2;//转移到IVR
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_TRANSFER) );
	return SendData( (char *)&msg, sizeof(msg));

	return TRUE;
}

BOOL CFormXCtrl::TransferToOut(LPCTSTR Tel) 
{
	if( NULL == Tel || "" == Tel  ){
		return FALSE;
	}
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_TRANSFER;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_TRANSFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_TRANSFER) );
	newMsg.nCallType = 1;
	memmove( newMsg.szTel, Tel, strlen(Tel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(Tel)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_TRANSFER) );
	return SendData( (char *)&msg, sizeof(msg));
}

void CFormXCtrl::OnTimer(UINT nIDEvent) 
{
	static n = 0;

	n = n + 1;
	if( n >= 30000 )
	{
		ConnectValidate();//发送socket 连接验证消息
		n = 0;
	}
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );
	
	if( !ReadData( (char*)&msg, sizeof(CFC_MESSAGE_PACK) ) ) 
		return ;
	
	int nMsgType = msg.nMsgType;
	switch( nMsgType )
	{
		case CFMTYPE_VALIDATE://socket 连接验证消息
			break;
		case CFMTYPE_MONITOR://Monitoring
			{
				CFMSG_MONITOR recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_MONITOR) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlMonitor,IDI_ICON_DISMONITOR,"取消监听", TRUE);
					m_bMonitor = TRUE;

					FireOnResponseMonitor( TRUE );
				}
				else
				{
					SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听", TRUE);
					m_bMonitor = FALSE;
					FireOnResponseMonitor( FALSE );
				}
				
			}
			break;
		case CFMTYPE_DMONITOR://Disable Monitoring
			{
				CFMSG_MONITOR recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_MONITOR) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听", TRUE);
					m_bMonitor = FALSE;

					FireOnResponseDisMonitor( TRUE );
				}
				else
				{
					SetButton( &m_ctlMonitor,IDI_ICON_DISMONITOR,"取消监听", FALSE);
					m_bMonitor = TRUE;
					FireOnResponseDisMonitor( FALSE );
				}
			}
			break;
		case CFMTYPE_MCALL://Make Call
			{
				CFMSG_MAKECALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_MAKECALL) );
				if( 0 == recMsg.nResult)
				{
					FireOnResponesMakeCall( TRUE );
				}
				else
				{
					FireOnResponesMakeCall( FALSE );
				}	
			}
			break;
		case CFMTYPE_INCONFER://In Conference
			{
				CFMSG_IN_CONFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_IN_CONFER) );
				if( 0 == recMsg.nResult){
					FireOnResponseJoinConfer( TRUE );
				}
				else
				{
					FireOnResponseJoinConfer( FALSE );
				}
			}
			break;
		case CFMTYPE_LCONFER://Leave Conference 
			{

			}
			break;
		case CFMTYPE_TRANSFER://Transfer
			{
				CFMSG_TRANSFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_TRANSFER) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER_CANCEL,"取消转移", FALSE);
					m_bTransfer = TRUE;

					FireOnResponseTransfer( TRUE );
				}
				else
				{
					SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移", TRUE);
					m_bTransfer = FALSE;

					FireOnResponseTransfer( FALSE );
				}	
			}
			break;
		case CFMTYPE_CANCEL_TRANSFER:
			{
				CFMSG_CANCEL_TRANSFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_CANCEL_TRANSFER) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移", TRUE);
					m_bTransfer = FALSE;

					FireOnResponseCancelTransfer( TRUE );
				}
				else
				{
					SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER_CANCEL,"取消转移", TRUE);
					m_bTransfer = TRUE;

					FireOnResponseCancelTransfer( FALSE );
				}
			}
			break;
		case CFMTYPE_ANSWER://Answer
			{
				CFMSG_ANSWERCALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_ANSWERCALL) );
				if( 0 == recMsg.nResult)
				{
					FireOnResponseAnswerCall( TRUE );
				}
				else
				{
					FireOnResponseAnswerCall( FALSE );
				}
			}
			break;
		case CFMTYPE_RELEASECALL://Release Call
			{

			}
			break;
		case CFMTYPE_HOLD://Hold Request
			{
				CFMSG_HOLD recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_HOLD) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持", TRUE);
					m_bHold = TRUE;

					FireOnResponseHold( TRUE );

					
				}
				else
				{
					SetButton( &m_ctlHold,IDI_ICON_HOLD,"保持", TRUE);
					m_bHold = FALSE;

					FireOnResponseHold( FALSE );
				}
			}
			break;
		case CFMTYPE_PICKUPHOLD://Pickup Hold Request
			{
				CFMSG_PICKUPHOLD_RES recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_PICKUPHOLD_RES) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlHold,IDI_ICON_HOLD,"保持", TRUE);
					m_bHold = FALSE;

					FireOnResponsePickUpHold( TRUE,recMsg.szCallerID ,recMsg.szCalledID  );
				}
				else
				{
					SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持", TRUE);
					m_bHold = TRUE;

					FireOnResponsePickUpHold( FALSE,recMsg.szCallerID ,recMsg.szCalledID  );
				}
			}
			break;
		case CFMTYPE_SENDFAX://Send Fax Request
			{

			}
			break;
		case CFMTYPE_LOGIN://Set Feature Login
			{
				
				CFMSG_LOGIN recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_LOGIN) );
				if( 0 == recMsg.nResult)
				{
					
					SetButton( &m_ctlLoginBtn,IDI_ICON_LOGOUT,"登出", TRUE);
					m_bLogin = TRUE;

					SetButton( &m_ctlReady,IDI_ICON_READY,"就绪", TRUE);
					m_bReady = FALSE;

					SetButton( &m_ctlHangUpBtn,IDI_ICON_HANGUP,"挂机", FALSE);
					

					SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移", FALSE);
					m_bTransfer = FALSE;
					
					SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持", TRUE);
					m_bHold = TRUE;

					SetButton( &m_ctlOutCall,IDI_ICON_MAKECALL,"外拨", TRUE);

					SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听", TRUE);
					m_bMonitor = FALSE;
					SetButton( &m_ctlRequestConf,IDI_ICON_REQUESTCONF,"召开会议", FALSE);
					
					SetButton( &m_ctlJoinConf,IDI_ICON_JOINCONF,"参加会议", TRUE);

					SetButton( &m_ctlSendData,IDI_ICON_SENDDATA,"交流", TRUE);

					SetButton( &m_ctlSetBusy,IDI_ICON_INSERVER,"置闲", TRUE);
					m_bSetBusy = FALSE;

					m_strAgentID = recMsg.szAgentID;
					m_nGroup = recMsg.nGroup;
					FireOnResponseLogin( TRUE );
					
				}
				else
				{

					m_bLogin = FALSE;
					SetButton( &m_ctlLoginBtn,IDI_ICON_LOGIN,"登陆",TRUE);


					SetButton( &m_ctlReady,IDI_ICON_READY,"就绪",FALSE);
					m_bReady = FALSE;

					SetButton( &m_ctlHangUpBtn,IDI_ICON_HANGUP,"挂机",FALSE);
					
					SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移",FALSE);
					m_bTransfer = FALSE;
					
					SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持",FALSE);
					m_bHold = TRUE;

					SetButton( &m_ctlOutCall,IDI_ICON_MAKECALL,"外拨",FALSE);

					SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听",FALSE);
					m_bMonitor = FALSE;
					SetButton( &m_ctlGetCall,IDI_ICON_GETACTIVECALL,"获取",FALSE);

					SetButton( &m_ctlRequestConf,IDI_ICON_REQUESTCONF,"召开会议",FALSE);

					SetButton( &m_ctlJoinConf,IDI_ICON_JOINCONF,"参加会议",FALSE);

					SetButton( &m_ctlSendData,IDI_ICON_SENDDATA,"交流",FALSE);

					SetButton( &m_ctlSetBusy,IDI_ICON_INSERVER,"置闲", FALSE);
					m_bSetBusy = FALSE;

					m_strAgentID = "";
					m_nGroup = -1;
					FireOnResponseLogin( FALSE );
					
				}


			}
			break;
		case CFMTYPE_LOGOUT://Set Feature Logout
			{
				CFMSG_LOGOUT recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_LOGOUT) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlLoginBtn,IDI_ICON_LOGIN,"登陆",TRUE);
					m_bLogin = FALSE;

					SetButton( &m_ctlReady,IDI_ICON_READY,"就绪",FALSE);
					m_bReady = FALSE;

					SetButton( &m_ctlHangUpBtn,IDI_ICON_HANGUP,"挂机",FALSE);

					SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移",FALSE);
					m_bTransfer = FALSE;
					
					SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持",FALSE);
					m_bHold = TRUE;


					SetButton( &m_ctlOutCall,IDI_ICON_MAKECALL,"外拨",FALSE);

					SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听",FALSE);
					m_bMonitor = FALSE;
					SetButton( &m_ctlGetCall,IDI_ICON_GETACTIVECALL,"获取",FALSE);

					SetButton( &m_ctlRequestConf,IDI_ICON_REQUESTCONF,"召开会议",FALSE);

					SetButton( &m_ctlJoinConf,IDI_ICON_JOINCONF,"参加会议",FALSE);
					
					SetButton( &m_ctlSendData,IDI_ICON_SENDDATA,"交流",FALSE);

					SetButton( &m_ctlSetBusy,IDI_ICON_INSERVER,"置闲", FALSE);
					m_bSetBusy = FALSE;
					FireOnResponseLogOut( TRUE );
				}
				else
				{
					m_bLogin = TRUE;

					SetButton( &m_ctlLoginBtn,IDI_ICON_LOGOUT,"登出",TRUE);

					SetButton( &m_ctlReady,IDI_ICON_READY,"就绪",TRUE);
					m_bReady = FALSE;

					SetButton( &m_ctlHangUpBtn,IDI_ICON_HANGUP,"挂机",FALSE);
					
					SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移",FALSE);
					m_bTransfer = FALSE;
					
					SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持",FALSE);
					m_bHold = TRUE;


					SetButton( &m_ctlOutCall,IDI_ICON_MAKECALL,"外拨",FALSE);

					SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听",FALSE);
					m_bMonitor = FALSE;
					SetButton( &m_ctlGetCall,IDI_ICON_GETACTIVECALL,"获取",FALSE);

					SetButton( &m_ctlRequestConf,IDI_ICON_REQUESTCONF,"召开会议",FALSE);

					SetButton( &m_ctlJoinConf,IDI_ICON_JOINCONF,"参加会议",FALSE);


					SetButton( &m_ctlSendData,IDI_ICON_SENDDATA,"交流",FALSE);
					SetButton( &m_ctlSetBusy,IDI_ICON_INSERVER,"置闲", FALSE);
					m_bSetBusy = FALSE;
					FireOnResponseLogOut( FALSE );
				}
			}
			break;
		case CFMTYPE_MSETBBUSY://Set Feature MSB
			{
				CFMSG_SET_BUSY recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_SET_BUSY) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlSetBusy,IDI_ICON_INSERVER,"置闲", TRUE);
					m_bSetBusy = FALSE;
					FireOnResponseSetBusy( FALSE );

				}
				else
				{
					SetButton( &m_ctlSetBusy,IDI_ICON_SETBUSY,"置忙", TRUE);
					m_bSetBusy = TRUE;
					FireOnResponseSetBusy( TRUE );
				}
			}
			break;
		case CFMTYPE_MSI://Set Feature MSI
			{

				CFMSG_INSERVER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_INSERVER) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlSetBusy,IDI_ICON_SETBUSY,"置忙", TRUE);
					m_bSetBusy = TRUE;
					FireOnResponseSetInServer( FALSE );

				}
				else
				{
					SetButton( &m_ctlSetBusy,IDI_ICON_INSERVER,"置闲", TRUE);
					m_bSetBusy = FALSE;
					FireOnResponseSetInServer( TRUE );
				}

			}
			break;
		case CFMTYPE_SFNOTR://Set Feature Not Ready
			{
				CFMSG_NOT_READY recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_NOT_READY) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlReady,IDI_ICON_READY,"就绪",TRUE);
					m_bReady = FALSE;

					FireOnResponseNotReady( TRUE );
				}
				else
				{
					SetButton( &m_ctlReady,IDI_ICON_NOTREADY,"休息",TRUE);
					m_bReady = TRUE;

					FireOnResponseNotReady( FALSE );
				}
			}
			break;
		case CFMTYPE_SFR://Set Feature  Ready
			{
				CFMSG_READY recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_READY) );
				if( 0 == recMsg.nResult)
				{
					SetButton( &m_ctlReady,IDI_ICON_NOTREADY,"休息",TRUE);
					m_bReady = TRUE;


					FireOnResponseReady( TRUE );
				}
				else
				{
					SetButton( &m_ctlReady,IDI_ICON_READY,"就绪",TRUE);
					m_bReady = FALSE;

					FireOnResponseReady( FALSE );
				}
			}
			break;
		case CFMTYPE_RINGING://Ringing Response
			{
				CFMSG_RINGING recMsg;
				memset(&recMsg, '\0', sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_RINGING) );
				FireOnRinging( recMsg.szTel, recMsg.szTel2 );

				SetButton( &m_ctlHangUpBtn,IDI_ICON_HANGUP,"挂机",TRUE);

				SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移",FALSE);
				m_bTransfer = FALSE;
				SetButton( &m_ctlOutCall,IDI_ICON_MAKECALL,"外拨",FALSE);

				SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持",FALSE);


				SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听",FALSE);
				m_bMonitor = FALSE;

				SetButton( &m_ctlGetCall,IDI_ICON_GETACTIVECALL,"获取",FALSE);

				SetButton( &m_ctlRequestConf,IDI_ICON_REQUESTCONF,"召开会议",FALSE);

				SetButton( &m_ctlJoinConf,IDI_ICON_JOINCONF,"参加会议",FALSE);
				SetInServer() ;

			}
			break;
		case CFMTYPE_HANGUP://Hangup event
			{
				SetButton( &m_ctlHangUpBtn,IDI_ICON_HANGUP,"挂机",FALSE);

				SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移",FALSE);
				m_bTransfer = FALSE;
				SetButton( &m_ctlOutCall,IDI_ICON_MAKECALL,"外拨",TRUE);

				SetButton( &m_ctlHold,IDI_ICON_PICKUPHOLD,"取回保持",TRUE);
				m_bHold = TRUE;
				SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听",TRUE);
				m_bMonitor = FALSE;
				SetButton( &m_ctlGetCall,IDI_ICON_GETACTIVECALL,"获取",TRUE);

				SetButton( &m_ctlRequestConf,IDI_ICON_REQUESTCONF,"召开会议",FALSE);
				
				SetButton( &m_ctlJoinConf,IDI_ICON_JOINCONF,"参加会议",FALSE);

				FireOnHangup();
			}
			break;
		case CFMTYPE_OFFHOOK://OffHook event
			{
				SetButton( &m_ctlHangUpBtn,IDI_ICON_HANGUP,"挂机",TRUE);

				SetButton( &m_ctlTransferBtn,IDI_ICON_TRANSFER,"转移",TRUE);
				m_bTransfer = FALSE;
				SetButton( &m_ctlOutCall,IDI_ICON_MAKECALL,"外拨",FALSE);

				SetButton( &m_ctlHold,IDI_ICON_HOLD,"保持",TRUE);
				m_bHold = FALSE;

				SetButton( &m_ctlMonitor,IDI_ICON_MONITOR,"监听",FALSE);
				m_bMonitor = FALSE;
				SetButton( &m_ctlGetCall,IDI_ICON_GETACTIVECALL,"获取",FALSE);

				SetButton( &m_ctlRequestConf,IDI_ICON_REQUESTCONF,"召开会议",TRUE);
				
				SetButton( &m_ctlJoinConf,IDI_ICON_JOINCONF,"参加会议",FALSE);

				SetButton( &m_ctlReady,IDI_ICON_READY,"就绪",TRUE);
				m_bReady = FALSE;


				FireOnOffHook();
			}
			break;
		case CFMTYPE_REG_COMM://reg to Comm and response
			{
				if( msg.nSecDMType < 0 )
				{
					m_bInit = FALSE;
					FireOnInitNorAgentResponse( FALSE);
					ExitNorAgent() ;
					
				}
				else
				{
					m_bInit = TRUE;
					FireOnInitNorAgentResponse( TRUE);
					
				}

			}
			break;
		case CFMTYPE_NET_ABEND://Net abend message type
			FireOnNetAbend();
			break;
		case CFMTYPE_CONFER_CALL:
			{
				CFMSG_CONFER_CALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_CONFER_CALL) );
				if( 0 == recMsg.nResult)
				{

					FireOnResponseConferCall( TRUE );
				}
				else
				{
					FireOnResponseConferCall( FALSE );
				}
			}
			break;
		case CFMTYPE_BUILD_CONFER:
			{
				
				CFMSG_BUILD_CONFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_RINGING) );
				

			}
			break;
		case CFMTYPE_AGENT_TO_AGENT:
			{
				FireOnReciveAgentData( msg.nSMID, msg.szMessageBody );
			}
			break;
		case CFMTYPE_GETACTIVECALL:
			{
				CFMSG_GETACTIVECALL_RES recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_GETACTIVECALL_RES) );
				if( 0 == recMsg.nResult)
				{

					SetButton( &m_ctlReady,IDI_ICON_GETACTIVECALL,"获取",FALSE);

					FireOnResponseGetActive( TRUE,recMsg.szCallerID ,recMsg.szCalledID  );
				}
				else
				{
					SetButton( &m_ctlReady,IDI_ICON_GETACTIVECALL,"获取",TRUE);

					FireOnResponseGetActive( FALSE,recMsg.szCallerID ,recMsg.szCalledID  );
				}
			}
			break;
		default:
			break;
	}
	
	
	COleControl::OnTimer(nIDEvent);
	
}

BOOL CFormXCtrl::ReadData(LPSTR buf, int nLen)
{
	if( m_pMsgDealWith->SocketConnect() )
	{
		return m_pMsgDealWith->ReadData( buf, nLen);
	}
	else
	{
		FireOnNetAbend();
		return FALSE;
	}
}

BOOL CFormXCtrl::SendData(LPSTR buf, int nLen)
{
	if( m_pMsgDealWith->SocketConnect() )
	{
		return m_pMsgDealWith->SendData( buf, nLen);
	}
	else
	{
		return FALSE;
	}
}

BOOL CFormXCtrl::InitNorAgent(short nDN, LPCTSTR IP, short nPort) 
{

	if( m_bInit ){
		return FALSE;
	}
	if( nPort <= 2000 ){
		return FALSE;
	}
	if( NULL == IP || "" == IP ){
		return FALSE;
	}
	
	
	if( ! m_pMsgDealWith->Initialize(nPort , IP,  0, 0 ) ){
		return FALSE ;
	}
	SetTimer(0,50,NULL);

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_REG_COMM;//reg message type
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;
	msg.nSecDMType=	0;
	msg.nSecDModularID	=	0;

	m_nDN = nDN;
	if( SendData( (char *)&msg, sizeof(msg)) ){
		m_bInit = TRUE;
		return TRUE;
	}

	return TRUE;
}

void CFormXCtrl::ExitNorAgent() 
{
	KillTimer(0);
	m_nDN = 0;
	m_pMsgDealWith->ExitInstance();
	m_bInit = FALSE;


	return;
}




BOOL CFormXCtrl::Ready( ) 
{


	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_SFR;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_READY newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_READY) );
	memmove( newMsg.szAgentID, (LPCTSTR)m_strAgentID, strlen((LPCTSTR)m_strAgentID) > sizeof(newMsg.szAgentID) ? sizeof(newMsg.szAgentID) :  strlen((LPCTSTR)m_strAgentID)  );
	newMsg.nGroup = m_nGroup;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_READY) );

	return SendData( (char *)&msg, sizeof(msg));
}

BOOL CFormXCtrl::NotReady( ) 
{


	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_SFNOTR;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_NOT_READY newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_NOT_READY) );
	memmove( newMsg.szAgentID, (LPCTSTR)m_strAgentID, strlen((LPCTSTR)m_strAgentID) > sizeof(newMsg.szAgentID) ? sizeof(newMsg.szAgentID) :  strlen((LPCTSTR)m_strAgentID)  );
	newMsg.nGroup = m_nGroup;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_NOT_READY) );

	return SendData( (char *)&msg, sizeof(msg));
}

void CFormXCtrl::OnBtnLogin() 
{
	
	if( m_bLogin )
	{
		FireOnClickLogOut();
	}
	else
	{
		FireOnClickLogin();
	}
	SetFocus( );
	
}
void CFormXCtrl::OnBtnReady() 
{
	if( m_bReady )
	{
		FireOnClickNotReady();
	}
	else
	{
		FireOnClickReady();
	}
	SetFocus( );
	
}

void CFormXCtrl::OnBtnOutcall() 
{
	FireOnClickOutCall();
	SetFocus( );
}

void CFormXCtrl::OnBtnMonitor() 
{
	m_ctlMonitor.EnableWindow(FALSE);
	if( m_bMonitor )
	{
		
		FireOnClickDisMonitor();
	}
	else
	{
		FireOnClickMonitor();
	}
	SetFocus( );
	
}
void CFormXCtrl::OnBtnRequestconf() 
{
	FireOnClickRequestConf();
	SetFocus( );
	
}
void CFormXCtrl::OnBtnJoinconf() 
{
	FireOnClickJoinConf();
	SetFocus( );
}

void CFormXCtrl::OnBtnHold() 
{
	m_ctlHold.EnableWindow(FALSE);
	if( m_bHold )
	{
		FireOnClickPickUpHold();
	}
	else
	{
		FireOnClickHold();
	}
	SetFocus( );
	
}

void CFormXCtrl::OnBtnGetactivecall() 
{
	FireOnClickGetActiveCall();
	SetFocus( );
}

void CFormXCtrl::OnBtnSenddata() 
{
	FireOnClickSendDate();
	SetFocus( );
}

void CFormXCtrl::OnBtnTransfer() 
{
	if( m_bTransfer )
	{

		FireOnClickCancelTransfer();
	}
	else
	{
		FireOnClickTransfer();
	}
	SetFocus( );
}




BOOL CFormXCtrl::ConnectValidate()
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_VALIDATE;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	return SendData( (char *)&msg, sizeof(msg));
}

void CFormXCtrl::SetButton(CButtonST *pBtn, UINT ICO, CString strCaption, BOOL bEndable)
{

	pBtn->SetColor(0,RGB(255,255,0),TRUE);//白色
	pBtn->SetColor(3,RGB(255,255,0),TRUE);//白色
	pBtn->SetColor(2,RGB(29,97,168),TRUE);//兰色
	pBtn->SetColor(2,RGB(29,97,168),TRUE);//兰色


	pBtn->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 255,FALSE);
	pBtn->SetIcon(ICO);
	
	//pBtn->SetRounded(TRUE);
	pBtn->SetWindowText(strCaption);
	pBtn->EnableWindow(bEndable);

	SetFocus( );
	return;
}



BOOL CFormXCtrl::SetBusy() 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_MSETBBUSY;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_SET_BUSY newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_SET_BUSY) );
	memmove( newMsg.szAgentID, (LPCTSTR)m_strAgentID, strlen((LPCTSTR)m_strAgentID) > sizeof(newMsg.szAgentID) ? sizeof(newMsg.szAgentID) :  strlen((LPCTSTR)m_strAgentID)  );
	newMsg.nGroup = m_nGroup;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_SET_BUSY) );

	return SendData( (char *)&msg, sizeof(msg));

}

BOOL CFormXCtrl::SetInServer() 
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_MSI;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	m_nDN;

	
	CFMSG_INSERVER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_INSERVER) );
	memmove( newMsg.szAgentID, (LPCTSTR)m_strAgentID, strlen((LPCTSTR)m_strAgentID) > sizeof(newMsg.szAgentID) ? sizeof(newMsg.szAgentID) :  strlen((LPCTSTR)m_strAgentID)  );
	newMsg.nGroup = m_nGroup;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_INSERVER) );

	return SendData( (char *)&msg, sizeof(msg));
}

void CFormXCtrl::OnBtnSetbusy() 
{
	if( m_bSetBusy )
	{
		FireOnClickSetBusy();
	}
	else
	{
		FireOnClickInServer();
	}
	SetFocus( );


}
