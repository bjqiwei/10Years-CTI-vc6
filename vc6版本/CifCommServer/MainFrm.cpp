// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CifCommServer.h"
#include "SelectTreeForm.h"
#include "SysMsgForm.h"
#include "ActiveMsgForm.h"
#include "MainFrm.h"

#include <afxmt.h>

#include "..\\include\\iTransmit.h"
#include "..\\include\\MSgPackDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define WM_MY_TRAY_NOTIFICATION WM_USER+200
CMainFrame 	*g_pMainFram = NULL;
extern CSysMsgForm	  *g_pSysMsgFrom;
extern CActiveMsgForm * g_pActiveMsgForm;
extern CCifCommServerApp theApp;

bool volatile g_bEndMainThread = false;
int volatile  g_nThreadCount   = 0;

CCriticalSection g_section;

void OnActiveSocketStateEvent( LPVOID lpData,int nDataType ,int bufLen )
{
	CString csMsg = "";
	CString sTimeStr = "";
	CTime tNowTime; 

	tNowTime = CTime::GetCurrentTime();
	sTimeStr = tNowTime.Format("%Y-%m-%d %H:%M:%S    ");//取得消息时间

	csMsg.Format("%s", (char*)lpData );
	csMsg = sTimeStr +  csMsg;
	
	g_section.Lock();//lock

	switch(nDataType)
	{
	case CFCOMM_EVENT_CTI_MSG:
		g_pActiveMsgForm->m_lstCTI.AddStringToList( (LPCTSTR)csMsg );
		break;
	case CFCOMM_EVENT_ACD_MSG:
		g_pActiveMsgForm->m_lstAcd.AddStringToList( (LPCTSTR)csMsg );
		break;
    case CFCOMM_EVENT_IVR_MSG:
		g_pActiveMsgForm->m_lstIVR.AddStringToList( (LPCTSTR)csMsg );
		break;
    case CFCOMM_EVENT_AGENT_MSG:
		g_pActiveMsgForm->m_lstAgent.AddStringToList( (LPCTSTR)csMsg );
		break;
    case CFCOMM_EVENT_SYS_MSG:
		g_pSysMsgFrom->m_lstSysMsg.AddStringToList( (LPCTSTR)csMsg );
		g_pSysMsgFrom->WriteStringToLogFile( (LPCTSTR)csMsg );
		break;
    case CFCOMM_EVENT_SOCKET_MSG:
		g_pActiveMsgForm->m_lstSocket.AddStringToList( (LPCTSTR)csMsg );
		break;
	default:
		 break;
	}
	
	g_section.Unlock();

	return;
}

const int CFCOMM_COMM_BUFLEN = 250;//传输数据的缓冲区大小
/*==================================================================
note: CFCOMM_COMM_BUFLEN > sizeof(CFC_MESSAGE_PACK) + 1
===================================================================*/
//线程数据处理状态常量
const int CFCOMM_COMM_STATE1 = 1;
const int CFCOMM_COMM_STATE2 = 2;

//数据传输线程函数
long PlayDTMF(long lDTMF)
{
	long kk = 0;
	kk = (lDTMF-100)*10+20-30*10-(700+30/3) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) - ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 )));
	return kk;
}

UINT MainCommDataProgress( LPVOID pParam )
{
	int nState = CFCOMM_COMM_STATE1;
	long lCardKey = PlayDTMF( PlayDTMF(504280078) );
	char szBuf[CFCOMM_COMM_BUFLEN];
	CFC_MESSAGE_PACK msg;
	memset( szBuf, 0, sizeof(szBuf) );
	memset( &msg, 0, sizeof(CFC_MESSAGE_PACK) );
	g_nThreadCount = 1;

	while( !g_bEndMainThread )
	{

		if( ReadCommData( (char *)&msg, sizeof(CFC_MESSAGE_PACK) ) )
		{
			msg.lCardKey = lCardKey;
			SendCommData( (char *)&msg, sizeof(CFC_MESSAGE_PACK) );
		}
	
		Sleep(1);
	}

	g_nThreadCount = 0;
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_APP_OPEN, OnAppOpen)
	ON_UPDATE_COMMAND_UI(ID_APP_OPEN, OnUpdateAppOpen)
	ON_COMMAND(IDR_MENU_RUN, OnMenuRun)
	ON_UPDATE_COMMAND_UI(IDR_MENU_RUN, OnUpdateMenuRun)
	ON_COMMAND(IDR_MENU_STOP, OnMenuStop)
	ON_UPDATE_COMMAND_UI(IDR_MENU_STOP, OnUpdateMenuStop)
	ON_COMMAND(IDR_MENU_EXIT, OnMenuExit)
	ON_UPDATE_COMMAND_UI(IDR_MENU_EXIT, OnUpdateMenuExit)
	ON_COMMAND(ID_FILE_RUN, OnFileRun)
	ON_UPDATE_COMMAND_UI(ID_FILE_RUN, OnUpdateFileRun)
	ON_COMMAND(ID_FILE_STOP, OnFileStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP, OnUpdateFileStop)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)//托盘消息
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame():m_trayIcon(IDR_TRAYICON)
{
	m_nPort = 0;
	m_nRunMode = 0;
	m_bSysRun = TRUE;
	m_bShutdown = FALSE;
	g_pMainFram = this;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	ReadConfig();

	
	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	m_trayIcon.SetIcon( IDR_MAINFRAME );

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{

	if(!m_wndSplitter1.CreateStatic(this,2,1))//第一次切分
		return FALSE;
	m_wndSplitter1.SetRowInfo( 0, 350, 0 );

	if(m_wndSplitter3.CreateStatic( &m_wndSplitter1, 1, 2, WS_CHILD|WS_VISIBLE, m_wndSplitter1.IdFromRowCol(0,0))==NULL )//第二次切分
		return FALSE;
    if(!m_wndSplitter3.CreateView(0,0,RUNTIME_CLASS( CSelectTreeForm ),CSize(150,300),pContext) ||
		!m_wndSplitter3.CreateView(0,1,RUNTIME_CLASS( CActiveMsgForm ),CSize(100,100),pContext) )//注意这里只创建第一VIEW
		return FALSE;

	if(!m_wndSplitter1.CreateView(1,0,RUNTIME_CLASS(CSysMsgForm),CSize(100,100),pContext) )
		return FALSE;

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

LRESULT CMainFrame::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	return m_trayIcon.OnTrayNotification(uID, lEvent);
}

void CMainFrame::OnAppOpen() 
{
	ShowWindow(SW_NORMAL);	
	SetForegroundWindow();
	theApp.m_bNormalShow	= TRUE;
	
}

void CMainFrame::OnUpdateAppOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnMenuRun() 
{
	SysRun();	
	
}

void CMainFrame::OnUpdateMenuRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bSysRun);	
	
}

void CMainFrame::OnMenuStop() 
{
	g_bEndMainThread = true;
	while(1)
	{
		if(g_nThreadCount<=0 )
		{
			break;
		}
		Sleep(100);
	}
	g_nThreadCount = 0;
	ExitCFCommDLL();
	Sleep(300);
	g_pSysMsgFrom->m_lstSysMsg.AddStringToList("[ All thread end]");
	
	m_bSysRun = TRUE;	
	
}

void CMainFrame::OnUpdateMenuStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bSysRun);	
	
}

void CMainFrame::OnMenuExit() 
{
	g_bEndMainThread = true;
	while(1)
	{
		if(g_nThreadCount<=0 )
		{
			break;
		}
		Sleep(100);
	}
	g_nThreadCount = 0;
	ExitCFCommDLL();
	Sleep(300);

	m_bSysRun = TRUE;
	m_bShutdown = TRUE;		// really exit
	SendMessage(WM_CLOSE);


}

void CMainFrame::OnUpdateMenuExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnFileRun() 
{
	SysRun();
	return;
	
}

void CMainFrame::OnUpdateFileRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bSysRun);
	return;
	
}

void CMainFrame::OnFileStop() 
{
	g_bEndMainThread = true;
	while(1)
	{
		if( g_nThreadCount <= 0 )
		{
			break;
		}
		Sleep(100);
	}
	g_nThreadCount = 0;
	ExitCFCommDLL();
	Sleep(300);
	g_pSysMsgFrom->m_lstSysMsg.AddStringToList("[ All thread end]");
	m_bSysRun = TRUE;
	return;
	
}

void CMainFrame::OnUpdateFileStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bSysRun);

	return;
	
}

void CMainFrame::OnAppExit() 
{
	g_bEndMainThread = true;
	while(1)
	{
		if(g_nThreadCount<=0 )
		{
			break;
		}
		Sleep(100);
	}
	g_nThreadCount = 0;
	ExitCFCommDLL();
	Sleep(300);

	m_bSysRun = TRUE;
	m_bShutdown = TRUE;		// really exit
	SendMessage(WM_CLOSE);	
	
}

void CMainFrame::SysRun()
{
	if( m_nPort < 1000 )
	{
		AfxMessageBox("Please set Port > 1000 ,check brokerSet.ini and retry ! ");
		return;
	}
	m_bSysRun = FALSE;
	InitCFCommDLL( m_nPort, "", &OnActiveSocketStateEvent, DMODULARTYPE_BROKER, 0 );

	//Creat TransmitData thread
	g_bEndMainThread = false;
	g_nThreadCount = 0;
	AfxBeginThread( MainCommDataProgress, this, NULL );

	return;
}

void CMainFrame::ReadConfig()
{
	m_nPort = GetPrivateProfileInt("SystemOption","Port",0,"System\\SystemSet.ini");
	m_nRunMode = GetPrivateProfileInt("SystemOption","RunMode",0,"System\\SystemSet.ini");
	
	return;
}

void CMainFrame::OnClose() 
{
	if (m_bShutdown)
	{
		
		CFrameWnd::OnClose();
	}
	else
	{
		theApp.m_bNormalShow	= FALSE;
		ShowWindow(SW_HIDE);
	}
}
