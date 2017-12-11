// ActiveMsgForm.cpp : implementation file
//

#include "stdafx.h"
#include "CifCommServer.h"
#include "ActiveMsgForm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CActiveMsgForm * g_pActiveMsgForm = NULL;

/////////////////////////////////////////////////////////////////////////////
// CActiveMsgForm

IMPLEMENT_DYNCREATE(CActiveMsgForm, CFormView)

CActiveMsgForm::CActiveMsgForm()
	: CFormView(CActiveMsgForm::IDD)
{
	//{{AFX_DATA_INIT(CActiveMsgForm)
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
	g_pActiveMsgForm = this;
}

CActiveMsgForm::~CActiveMsgForm()
{
}

void CActiveMsgForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActiveMsgForm)
	DDX_Control(pDX, IDC_SOCKET_LIST, m_lstSocket);
	DDX_Control(pDX, IDC_IVR_LIST, m_lstIVR);
	DDX_Control(pDX, IDC_CTI_LIST, m_lstCTI);
	DDX_Control(pDX, IDC_AGENT_LIST, m_lstAgent);
	DDX_Control(pDX, IDC_ACD_LIST, m_lstAcd);
	DDX_Text(pDX, IDC_STATIC_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActiveMsgForm, CFormView)
	//{{AFX_MSG_MAP(CActiveMsgForm)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveMsgForm diagnostics

#ifdef _DEBUG
void CActiveMsgForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CActiveMsgForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CActiveMsgForm message handlers

void CActiveMsgForm::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{
		
		m_lstSocket.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstIVR.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstCTI.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstAgent.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstAcd.MoveWindow(0,20,m_cx,m_cy-20);


	}
	
}

void CActiveMsgForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;

		m_lstSocket.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstIVR.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstCTI.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstAgent.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstAcd.MoveWindow(0,20,m_cx,m_cy-20);

		m_strTitle = "Active Socket Message";
		m_lstSocket.ShowWindow(SW_SHOW);
		m_lstIVR.ShowWindow(SW_HIDE);
		m_lstCTI.ShowWindow(SW_HIDE);
		m_lstAgent.ShowWindow(SW_HIDE);
		m_lstAcd.ShowWindow(SW_HIDE);


		UpdateData(FALSE);
		

	}
	

}

void CActiveMsgForm::ShowList(CString strItem)
{
	if( "ActiveSocket" == strItem )
	{
		m_strTitle = "Active Socket:";
		m_lstSocket.ShowWindow(SW_SHOW);
		m_lstIVR.ShowWindow(SW_HIDE);
		m_lstCTI.ShowWindow(SW_HIDE);
		m_lstAgent.ShowWindow(SW_HIDE);
		m_lstAcd.ShowWindow(SW_HIDE);
	}
	else if( "CITMessage" == strItem )
	{
		m_strTitle = "Active CTI Message:";
		m_lstSocket.ShowWindow(SW_HIDE);
		m_lstIVR.ShowWindow(SW_HIDE);
		m_lstAgent.ShowWindow(SW_HIDE);
		m_lstCTI.ShowWindow(SW_SHOW);
		m_lstAcd.ShowWindow(SW_HIDE);
	}
	else if( "AgentMessage" == strItem )
	{
		m_strTitle = "Active Agent Message:";
		m_lstSocket.ShowWindow(SW_HIDE);
		m_lstAgent.ShowWindow(SW_SHOW);
		m_lstIVR.ShowWindow(SW_HIDE);
		m_lstCTI.ShowWindow(SW_HIDE);
		m_lstAcd.ShowWindow(SW_HIDE);
	}
	
	else if( "IVRMessage" == strItem )
	{
		m_strTitle = "Active IVR Message:";
		m_lstSocket.ShowWindow(SW_HIDE);
		m_lstAgent.ShowWindow(SW_HIDE);
		m_lstIVR.ShowWindow(SW_SHOW);
		m_lstCTI.ShowWindow(SW_HIDE);
		m_lstAcd.ShowWindow(SW_HIDE);
	}
	else if( "ACDMessage" == strItem )
	{
		m_strTitle = "Active ACD Message:";
		m_lstSocket.ShowWindow(SW_HIDE);
		m_lstAgent.ShowWindow(SW_HIDE);
		m_lstIVR.ShowWindow(SW_HIDE);
		m_lstAcd.ShowWindow(SW_SHOW);
		m_lstCTI.ShowWindow(SW_HIDE);
	}

	
	UpdateData(FALSE);
}
