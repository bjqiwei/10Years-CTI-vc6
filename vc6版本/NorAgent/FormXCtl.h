#if !defined(AFX_FORMXCTL_H__AFDC1594_F993_11D1_96ED_F05F03C10000__INCLUDED_)
#define AFX_FORMXCTL_H__AFDC1594_F993_11D1_96ED_F05F03C10000__INCLUDED_

#include "BtnST.h"
#include "MsqDowith.h"	// Added by ClassView

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// FormXCtl.h : Declaration of the CFormXCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl : See FormXCtl.cpp for implementation.

class CFormXCtrl : public CFormControl
{
	DECLARE_DYNCREATE(CFormXCtrl)

// Constructor
public:
	CFormXCtrl();

	//{{AFX_DATA(CTestXView)
	enum{ IDD = IDD_FORMX };
	CButtonST m_ctlLoginBtn;
	CButtonST m_ctlHangUpBtn;
	CButtonST m_ctlTransferBtn;
	CButtonST m_ctlHold;
	CButtonST m_ctlReady;
	CButtonST m_ctlOutCall;
	CButtonST m_ctlMonitor;
	CButtonST m_ctlGetCall;
	CButtonST m_ctlRequestConf;
	CButtonST m_ctlJoinConf;
	CButtonST m_ctlSendData;
	CButtonST m_ctlSetBusy;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormXCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void OnInitialUpdate();
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CFormXCtrl();

	DECLARE_OLECREATE_EX(CFormXCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CFormXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CFormXCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CFormXCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CFormXCtrl)
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnHangup();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnReady();
	afx_msg void OnBtnOutcall();
	afx_msg void OnBtnMonitor();
	afx_msg void OnBtnJoinconf();
	afx_msg void OnBtnHold();
	afx_msg void OnBtnGetactivecall();
	afx_msg void OnBtnSenddata();
	afx_msg void OnBtnTransfer();
	afx_msg void OnBtnRequestconf();
	afx_msg void OnBtnSetbusy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CFormXCtrl)
	afx_msg BOOL AgentToAgentData(LPCTSTR AimDN, LPCTSTR Buf);
	afx_msg BOOL AnswerCall();
	afx_msg BOOL CallAgent(LPCTSTR AimDN);
	afx_msg BOOL CancelTransfer();
	afx_msg BOOL ConferCall(short nType, LPCTSTR Tel);
	afx_msg BOOL DisMonitor();
	afx_msg BOOL GetActiveCall(short nType, LPCTSTR Tel);
	afx_msg BOOL HangUp();
	afx_msg BOOL Hold();
	afx_msg BOOL JoinConference(LPCTSTR AimDN);
	afx_msg BOOL Monitor(LPCTSTR AimDN);
	afx_msg BOOL OutCall(LPCTSTR Tel);
	afx_msg BOOL PickupHold(short nType, LPCTSTR CallerID);
	afx_msg BOOL TransferToAgent(LPCTSTR AimDN);
	afx_msg BOOL TransferToIVR();
	afx_msg BOOL TransferToOut(LPCTSTR Tel);
	afx_msg BOOL InitNorAgent(short nDN, LPCTSTR IP, short nPort);
	afx_msg void ExitNorAgent();
	afx_msg BOOL LogIn(LPCTSTR AgentID, LPCTSTR PWD, short Group);
	afx_msg BOOL LogOut(LPCTSTR PWD);
	afx_msg BOOL Ready();
	afx_msg BOOL NotReady();
	afx_msg BOOL SetBusy();
	afx_msg BOOL SetInServer();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CFormXCtrl)
	void FireOnHangup()
		{FireEvent(eventidOnHangup,EVENT_PARAM(VTS_NONE));}
	void FireOnNetAbend()
		{FireEvent(eventidOnNetAbend,EVENT_PARAM(VTS_NONE));}
	void FireOnOffHook()
		{FireEvent(eventidOnOffHook,EVENT_PARAM(VTS_NONE));}
	void FireOnReciveAgentData(short nDN, LPCTSTR buf)
		{FireEvent(eventidOnReciveAgentData,EVENT_PARAM(VTS_I2  VTS_BSTR), nDN, buf);}
	void FireOnResponesMakeCall(BOOL bResult)
		{FireEvent(eventidOnResponesMakeCall,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseAnswerCall(BOOL bResult)
		{FireEvent(eventidOnResponseAnswerCall,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseCancelTransfer(BOOL bResult)
		{FireEvent(eventidOnResponseCancelTransfer,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseConferCall(BOOL bResult)
		{FireEvent(eventidOnResponseConferCall,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseDisMonitor(BOOL bResult)
		{FireEvent(eventidOnResponseDisMonitor,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseHold(BOOL bResult)
		{FireEvent(eventidOnResponseHold,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseJoinConfer(BOOL bResult)
		{FireEvent(eventidOnResponseJoinConfer,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseLogin(BOOL bResult)
		{FireEvent(eventidOnResponseLogin,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseMonitor(BOOL bResult)
		{FireEvent(eventidOnResponseMonitor,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseTransfer(BOOL bResult)
		{FireEvent(eventidOnResponseTransfer,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnRinging(LPCTSTR CallerID, LPCTSTR CalledID)
		{FireEvent(eventidOnRinging,EVENT_PARAM(VTS_BSTR  VTS_BSTR), CallerID, CalledID);}
	void FireOnResponsePickUpHold(BOOL bResult, LPCTSTR CallerID, LPCTSTR CalledID)
		{FireEvent(eventidOnResponsePickUpHold,EVENT_PARAM(VTS_BOOL  VTS_BSTR  VTS_BSTR), bResult, CallerID, CalledID);}
	void FireOnResponseLogOut(BOOL bResult)
		{FireEvent(eventidOnResponseLogOut,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnInitNorAgentResponse(BOOL bResult)
		{FireEvent(eventidOnInitNorAgentResponse,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseReady(BOOL bResult)
		{FireEvent(eventidOnResponseReady,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseNotReady(BOOL bResult)
		{FireEvent(eventidOnResponseNotReady,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnClickMonitor()
		{FireEvent(eventidOnClickMonitor,EVENT_PARAM(VTS_NONE));}
	void FireOnClickReady()
		{FireEvent(eventidOnClickReady,EVENT_PARAM(VTS_NONE));}
	void FireOnClickHold()
		{FireEvent(eventidOnClickHold,EVENT_PARAM(VTS_NONE));}
	void FireOnClickJoinConf()
		{FireEvent(eventidOnClickJoinConf,EVENT_PARAM(VTS_NONE));}
	void FireOnClickConfCall()
		{FireEvent(eventidOnClickConfCall,EVENT_PARAM(VTS_NONE));}
	void FireOnClickLogin()
		{FireEvent(eventidOnClickLogin,EVENT_PARAM(VTS_NONE));}
	void FireOnClickTransfer()
		{FireEvent(eventidOnClickTransfer,EVENT_PARAM(VTS_NONE));}
	void FireOnClickGetActiveCall()
		{FireEvent(eventidOnClickGetActiveCall,EVENT_PARAM(VTS_NONE));}
	void FireOnClickSendDate()
		{FireEvent(eventidOnClickSendDate,EVENT_PARAM(VTS_NONE));}
	void FireOnClickHangUp()
		{FireEvent(eventidOnClickHangUp,EVENT_PARAM(VTS_NONE));}
	void FireOnClickOutCall()
		{FireEvent(eventidOnClickOutCall,EVENT_PARAM(VTS_NONE));}
	void FireOnClickLogOut()
		{FireEvent(eventidOnClickLogOut,EVENT_PARAM(VTS_NONE));}
	void FireOnClickNotReady()
		{FireEvent(eventidOnClickNotReady,EVENT_PARAM(VTS_NONE));}
	void FireOnClickDisMonitor()
		{FireEvent(eventidOnClickDisMonitor,EVENT_PARAM(VTS_NONE));}
	void FireOnClickCancelTransfer()
		{FireEvent(eventidOnClickCancelTransfer,EVENT_PARAM(VTS_NONE));}
	void FireOnClickPickUpHold()
		{FireEvent(eventidOnClickPickUpHold,EVENT_PARAM(VTS_NONE));}
	void FireOnResponseGetActive(BOOL bResult, LPCTSTR CallerID, LPCTSTR CalledID)
		{FireEvent(eventidOnResponseGetActive,EVENT_PARAM(VTS_BOOL  VTS_BSTR  VTS_BSTR), bResult, CallerID, CalledID);}
	void FireOnClickRequestConf()
		{FireEvent(eventidOnClickRequestConf,EVENT_PARAM(VTS_NONE));}
	void FireOnResponseSetBusy(BOOL bResult)
		{FireEvent(eventidOnResponseSetBusy,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnResponseSetInServer(BOOL bResult)
		{FireEvent(eventidOnResponseSetInServer,EVENT_PARAM(VTS_BOOL), bResult);}
	void FireOnClickSetBusy()
		{FireEvent(eventidOnClickSetBusy,EVENT_PARAM(VTS_NONE));}
	void FireOnClickInServer()
		{FireEvent(eventidOnClickInServer,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	void SetButton( CButtonST *pBtn, UINT ICO, CString strCaption, BOOL bEndable );
	BOOL ConnectValidate( void );
	CMsgDealWith m_MsgDealWith;
	CMsgDealWith * m_pMsgDealWith;
	
	BOOL m_bInit;
	BOOL m_bLogin;
	BOOL m_bSetBusy;
	BOOL m_bMonitor;
	BOOL m_bTransfer;
	CString m_strAgentID;
	int m_nGroup;

	BOOL m_bReady;
	BOOL m_bHold;

	LPCTSTR m_lpIP;
	short m_nPort;
	short m_nDN;

	void InitAllButton( void );
	enum {
	//{{AFX_DISP_ID(CFormXCtrl)
	dispidAgentToAgentData = 1L,
	dispidAnswerCall = 2L,
	dispidCallAgent = 3L,
	dispidCancelTransfer = 4L,
	dispidConferCall = 5L,
	dispidDisMonitor = 6L,
	dispidGetActiveCall = 7L,
	dispidHangUp = 8L,
	dispidHold = 9L,
	dispidJoinConference = 10L,
	dispidMonitor = 11L,
	dispidOutCall = 12L,
	dispidPickupHold = 13L,
	dispidTransferToAgent = 14L,
	dispidTransferToIVR = 15L,
	dispidTransferToOut = 16L,
	dispidInitNorAgent = 17L,
	dispidExitNorAgent = 18L,
	dispidLogIn = 19L,
	dispidLogOut = 20L,
	dispidReady = 21L,
	dispidNotReady = 22L,
	dispidSetBusy = 23L,
	dispidSetInServer = 24L,
	eventidOnHangup = 1L,
	eventidOnNetAbend = 2L,
	eventidOnOffHook = 3L,
	eventidOnReciveAgentData = 4L,
	eventidOnResponesMakeCall = 5L,
	eventidOnResponseAnswerCall = 6L,
	eventidOnResponseCancelTransfer = 7L,
	eventidOnResponseConferCall = 8L,
	eventidOnResponseDisMonitor = 9L,
	eventidOnResponseHold = 10L,
	eventidOnResponseJoinConfer = 11L,
	eventidOnResponseLogin = 12L,
	eventidOnResponseMonitor = 13L,
	eventidOnResponseTransfer = 14L,
	eventidOnRinging = 15L,
	eventidOnResponsePickUpHold = 16L,
	eventidOnResponseLogOut = 17L,
	eventidOnInitNorAgentResponse = 18L,
	eventidOnResponseReady = 19L,
	eventidOnResponseNotReady = 20L,
	eventidOnClickMonitor = 21L,
	eventidOnClickReady = 22L,
	eventidOnClickHold = 23L,
	eventidOnClickJoinConf = 24L,
	eventidOnClickConfCall = 25L,
	eventidOnClickLogin = 26L,
	eventidOnClickTransfer = 27L,
	eventidOnClickGetActiveCall = 28L,
	eventidOnClickSendDate = 29L,
	eventidOnClickHangUp = 30L,
	eventidOnClickOutCall = 31L,
	eventidOnClickLogOut = 32L,
	eventidOnClickNotReady = 33L,
	eventidOnClickDisMonitor = 34L,
	eventidOnClickCancelTransfer = 35L,
	eventidOnClickPickUpHold = 36L,
	eventidOnResponseGetActive = 37L,
	eventidOnClickRequestConf = 38L,
	eventidOnResponseSetBusy = 39L,
	eventidOnResponseSetInServer = 40L,
	eventidOnClickSetBusy = 41L,
	eventidOnClickInServer = 42L,
	//}}AFX_DISP_ID
	};
private:
	BOOL ReadData(LPSTR buf, int nLen);
	BOOL SendData(LPSTR buf, int nLen);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMXCTL_H__AFDC1594_F993_11D1_96ED_F05F03C10000__INCLUDED)
