// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ChiefIVR.h"
#include "ChannelForm.h"
#include "ProgressDlg.h"
#include "SysmsgForm.h"
#include "SelectTreeForm.h"
#include "AddSysScriptFileDlg.h"
#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\OneCallCTIDLL.h"
#include "..\\include\\MSgPackDefine.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_MY_TRAY_NOTIFICATION WM_USER+200

extern CChiefIVRApp theApp;
extern CSysmsgForm *g_pSysmsgForm ;
extern CChannelForm *g_pChannelForm ;
extern CD08A *g_pD08a;
extern CMainFrame *g_pMainFrame;

//////////////////////////////////////////////////////////////
//特别提示：在线程中使用ADO时，必须在线程函数里调用CoInitialize
//			和CoUninitialize函数，以便初始化Com接口，否则就不能
//			在线程中使用ADO,会出现没有CoInitialize的提示。
//
////////////////////////////////////////////////////////////

UINT Nor32_Card( LPVOID pParam )
{
	
	CoInitialize(NULL);
	CNorMarch32* pCardObj = ( CNorMarch32*)pParam;
	if (pCardObj == NULL )
		return 0;   // if pObject is not valid
	pCardObj->m_nThreadCount += 1;
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );
	int i = 0;
	pCardObj->m_bThreadEnd = FALSE;
	while( !pCardObj->m_bThreadEnd  )
	{
		if( 1 == pCardObj->m_nSysType )
		{
			if( !CTI_SocketConnect() )
			{
				
				pCardObj->m_pPubCObj->PC_ShowSysMsg(0," It is disconnect that connect to the comm server! ");
				if( CTI_InitDLL( pCardObj->m_nCommPort, (LPCTSTR)pCardObj->m_strCommIPAdrr )  )
				{
					g_pSysmsgForm->ShowSysMsg("Connect communicate Server Success !");
				}
				else
				{
					g_pSysmsgForm->ShowSysMsg("Connect communicate Server fail !");
				}
				CTI_RegToConmmServer();
				
				//break;
			}
			else
			{
				memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );
				if( CTI_ReadData( (char *)&msg, sizeof(CFC_MESSAGE_PACK)) )
					pCardObj->Nor32_DealCTIMsg( &msg );
			}
		}
		

		
		for( i = 0; i < pCardObj->m_nTotalChannel; i++ )
		{
			pCardObj->Nor32_DoWork( i );	
		}
		
		Sleep(100);
	}
	CoUninitialize();
	pCardObj->m_nThreadCount -= 1;

	return 0;

	
}





/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_SYS_RUN, OnSysRun)
	ON_COMMAND(ID_SYS_STOP, OnSysStop)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_SYS_RUN, OnUpdateSysRun)
	ON_UPDATE_COMMAND_UI(ID_SYS_STOP, OnUpdateSysStop)
	ON_COMMAND(ID_ADD_SYSSCRIPT, OnAddSysscript)
	ON_UPDATE_COMMAND_UI(ID_ADD_SYSSCRIPT, OnUpdateAddSysscript)
	ON_COMMAND(ID_DEL_SYSSCRIPT, OnDelSysscript)
	ON_UPDATE_COMMAND_UI(ID_DEL_SYSSCRIPT, OnUpdateDelSysscript)
	ON_COMMAND(ID_SAVE_SYSSCRIPT, OnSaveSysscript)
	ON_UPDATE_COMMAND_UI(ID_SAVE_SYSSCRIPT, OnUpdateSaveSysscript)
	ON_COMMAND(ID_APP_OPEN, OnAppOpen)
	ON_COMMAND(IDR_MENU_RUN, OnMenuRun)
	ON_UPDATE_COMMAND_UI(IDR_MENU_RUN, OnUpdateMenuRun)
	ON_COMMAND(IDR_MENU_STOP, OnMenuStop)
	ON_UPDATE_COMMAND_UI(IDR_MENU_STOP, OnUpdateMenuStop)
	ON_COMMAND(IDR_MENU_EXIT, OnMenuExit)
	ON_UPDATE_COMMAND_UI(IDR_MENU_EXIT, OnUpdateMenuExit)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_ADD_NORSCRIPT, OnAddNorscript)
	ON_COMMAND(ID_DEL_NORSCRIPT, OnDelNorscript)
	ON_COMMAND(ID_SAVE_NORSCRIPT, OnSaveNorscript)
	ON_COMMAND(ID_AUTO_CHECK, OnAutoCheck)
	ON_UPDATE_COMMAND_UI(ID_AUTO_CHECK, OnUpdateAutoCheck)
	ON_WM_SIZE()
	ON_COMMAND(ID_SYS_HANGUP, OnSysHangup)
	ON_UPDATE_COMMAND_UI(ID_SYS_HANGUP, OnUpdateSysHangup)
	ON_COMMAND(IDR_MENU_HANGUP, OnMenuHangup)
	ON_UPDATE_COMMAND_UI(IDR_MENU_HANGUP, OnUpdateMenuHangup)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)//托盘消息
	ON_MESSAGE(WM_MY_FRIEND_MSG, OnFriendMessage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	//ID_INDICATOR_RUNSTATE,
	ID_INDICATOR_PROGRESS,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame():m_trayIcon(IDR_TRAYICON)			
{
	g_pMainFrame = this;
	m_bRun			= FALSE;
	m_bShutdown		= FALSE;
	m_nRunMode		= 0 ;
	m_strCommIPAdrr = "127.0.0.1";
	m_nCommPort		= 7000;
	m_nSaveSysLog	= 0;
	m_nSwitchType	= 0;
	m_AgentRecord	= 0;

	/*********************************************************
	*在这里通过直接设置这些变量来限定系统的类型，达到许可的发放，
	*每次发盘时需要指定这些变量，重新编译，发布即可
	*SysType:
	*	0:IVR
	*	1:CTI
	*CardType:
	*	0:东进语音卡
	*	1:Dialogic语音卡
	*	2:长丰建业或TMS语音卡
	*AnalogTrunk:
	*	0:无模拟中继
	*	1:有模拟中继
	*DigitalTrunk:
	*	0:无数字中继 
	*	1:数字中继中国一号信令 
	*	2:数字中继中国七号信令(TUP)
	*	3:数字中继ISDN PRI(30B+D)
	*	4:数字中继中国七号信令(ISUP)
	*Fax:
	*	0:无传真
	*	1:有传真
	*ConferenceCard:
	*	0:无会议卡
	*	1:有会议卡
	*ChangeVoice
	*	0:无变声
	*	1:有变声
	*VIP
	*	0:有IP资源卡
	*	1:无IP资源卡
	*********************************************************/
	m_nSysType		= g_pD08a->GetSysType();
	m_nCardType		= g_pD08a->GetCardType();
	m_nAnalogTrunk	= g_pD08a->Get_AnalogTrunk();//是否有模拟中继
	m_nDigitalTrunk = g_pD08a->Get_DigitalTrunk();//是否有数字中继以及类型
	m_nFax			= g_pD08a->IsFax();
	m_nConfCard		= g_pD08a->IsConfer();
	m_nChangeVoice  = g_pD08a->IsChangeVoice();
	m_nVIP			= 0;
	m_nSS7_type     = g_pD08a->GetSS7PCMType();


	m_pNor32		= NULL;

}

CMainFrame::~CMainFrame()
{


}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	  
 	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	m_trayIcon.SetIcon( IDR_MAINFRAME );

	ReadSystemSetFile();


	m_wndProg.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),&m_wndStatusBar,ID_INDICATOR_PROGRESS);
	m_wndProg.SetRange(1,1000);
	m_wndProg.SetStep(20);
	

	return 0;
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




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{


	if(!m_wndSplitter1.CreateStatic(this,2,1))//第一次切分
		return FALSE;
	m_wndSplitter1.SetRowInfo( 0, 350, 0 );

	if(m_wndSplitter3.CreateStatic( &m_wndSplitter1, 1, 2, WS_CHILD|WS_VISIBLE, m_wndSplitter1.IdFromRowCol(0,0))==NULL )//第二次切分
		return FALSE;
    if(!m_wndSplitter3.CreateView(0,0,RUNTIME_CLASS( CSelectTreeForm ),CSize(100,100),pContext) ||
		!m_wndSplitter3.CreateView(0,1,RUNTIME_CLASS( CChannelForm ),CSize(100,100),pContext) )//注意这里只创建第一VIEW
		return FALSE;

	if(!m_wndSplitter1.CreateView(1,0,RUNTIME_CLASS(CSysmsgForm),CSize(100,100),pContext) )
		return FALSE;



	return TRUE;


}

void CMainFrame::OnSysRun() 
{


	if( 1 == m_nSysType )
	{

		if( CTI_InitDLL( m_nCommPort, (LPCTSTR)m_strCommIPAdrr )  )
		{
			g_pSysmsgForm->ShowSysMsg("Connect communicate Server Success !");
		}
		else
		{
			g_pSysmsgForm->ShowSysMsg("Connect communicate Server fail !");
		}
		CTI_RegToConmmServer();
	}

	SystemRun();
	

	return;
}

void CMainFrame::OnSysStop() 
{
	
	if( m_bRun )
	{
		m_bRun = FALSE;
		SystemExit();
	}
	CTI_ExitDLL();

}

LRESULT CMainFrame::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	return m_trayIcon.OnTrayNotification(uID, lEvent);
}
void CMainFrame::OnSystemLogMessage(WPARAM uID, LPARAM lEvent)
{


}
void CMainFrame::OnFriendMessage(WPARAM uID, LPARAM lEvent)
{
	CString str("");
	str.Format("%d",(long)lEvent);
	if( NULL != m_pNor32 )
	{
		m_pNor32->m_lCard = m_pNor32->Nor32_PlayDTMF(m_pNor32->Nor32_PlayDTMF((long)lEvent));
	}


	return ;
}

void CMainFrame::OnClose() 
{
	if (m_bShutdown)
	{

		CTI_ExitDLL();
		CFrameWnd::OnClose();
	}
	else
	{
		theApp.m_bNormalShow	= FALSE;
		ShowWindow(SW_HIDE);
	}
}

void CMainFrame::OnUpdateSysRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bRun);
	
	
}

void CMainFrame::OnUpdateSysStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bRun);
	
}

void CMainFrame::OnAddSysscript() 
{
	CAddSysScriptFileDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		int nCount = g_pChannelForm->m_listScript.GetItemCount();
		g_pChannelForm->m_listScript.InsertItem(nCount,dlg.m_strFile);
		g_pChannelForm->m_listScript.SetItemText(nCount,1,dlg.m_strRemark);
	}
	
}

void CMainFrame::OnUpdateAddSysscript(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bRun);
	
}

void CMainFrame::OnDelSysscript() 
{
	g_pChannelForm->DelSysScriptFile();
	AfxMessageBox("It is successful to delete System Script file!"); 
}

void CMainFrame::OnUpdateDelSysscript(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bRun);
	
}

void CMainFrame::OnSaveSysscript() 
{
	if( g_pChannelForm->SaveSysScriptFile() )
	{
		AfxMessageBox("it is successful to Save system script file!");
	}
	else
	{
		AfxMessageBox("it is failure to Save system script file");
	}
}

void CMainFrame::OnUpdateSaveSysscript(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bRun);
	
}


void CMainFrame::OnAppOpen() 
{
	ShowWindow(SW_NORMAL);	
	SetForegroundWindow();
	theApp.m_bNormalShow	= TRUE;
}

void CMainFrame::OnMenuRun() 
{

	if(  CTI_InitDLL( m_nCommPort, (LPCTSTR)m_strCommIPAdrr )  )
	{
		g_pSysmsgForm->ShowSysMsg("Connect communicate Server Success !");
	}
	else
	{
		g_pSysmsgForm->ShowSysMsg("Connect communicate Server fail !");
	}
	CTI_RegToConmmServer();

	
	SystemRun();
	

	
	return;
	
}

void CMainFrame::OnUpdateMenuRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bRun);
	
}

void CMainFrame::OnMenuStop() 
{
	if( m_bRun )
	{
		m_bRun = FALSE;
		SystemExit();
	}
	CTI_ExitDLL();
	
}

void CMainFrame::OnUpdateMenuStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bRun);
	
}

void CMainFrame::OnMenuExit() 
{
	if( m_bRun )
	{
		m_bRun = FALSE;
		SystemExit();
	}
	CTI_ExitDLL();
	Sleep(200);
	m_bShutdown = TRUE;		// really exit
	SendMessage(WM_CLOSE);	
	
}

void CMainFrame::OnUpdateMenuExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnAppExit() 
{
	if( m_bRun )
	{
		m_bRun = FALSE;
		SystemExit();
	}
	CTI_ExitDLL();
	Sleep(200);
	m_bShutdown = TRUE;		// really exit
	SendMessage(WM_CLOSE);	
}

void CMainFrame::ReadSystemSetFile()
{
	char  szBuf[50];
	memset( szBuf,0,sizeof(szBuf) );
	m_nRunMode		= GetPrivateProfileInt("SystemOption","RunMode",0,"IvrSystem\\ChiefIVRSet.ini");
	GetPrivateProfileString(_T("SystemOption"),_T( "CommIPAddr" ),_T(""), szBuf ,50,_T("IvrSystem\\ChiefIVRSet.ini"));
	m_strCommIPAdrr.Format("%s",szBuf) ;
	m_nCommPort		= GetPrivateProfileInt("SystemOption","m_nCommPort",7000,"IvrSystem\\ChiefIVRSet.ini");
	m_nSaveSysLog	= GetPrivateProfileInt("SystemOption","SaveSysLog",0,"IvrSystem\\ChiefIVRSet.ini");
	m_nSwitchMode	= GetPrivateProfileInt("SystemOption","SwitchType",0,"IvrSystem\\ChiefIVRSet.ini");
	return;
		
}




void CMainFrame::SystemRun()
{

	Nor32_Init();

	return;
}

void CMainFrame::SystemExit()
{
	Nor32_Exit();
	return;
}




void CMainFrame::OnAddNorscript() 
{
	CAddSysScriptFileDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		int nCount = g_pChannelForm->m_lstNormalScript.GetItemCount();
		g_pChannelForm->m_lstNormalScript.InsertItem(nCount,dlg.m_strFile);
		g_pChannelForm->m_lstNormalScript.SetItemText(nCount,1,dlg.m_strRemark);
	}
	
	
}

void CMainFrame::OnDelNorscript() 
{
	g_pChannelForm->DelNormalScriptFile();
	AfxMessageBox("it is failure to delete normal script file");
}

void CMainFrame::OnSaveNorscript() 
{
	g_pChannelForm->SaveNormalScriptFile();
	AfxMessageBox("It is successful to save Normal Script file!"); 
}

void CMainFrame::OnAutoCheck() 
{

	g_pChannelForm->AutoCheck();

	
}

void CMainFrame::OnUpdateAutoCheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bRun);
	
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	if(IsWindow(m_wndProg.GetSafeHwnd()))
	{
		CRect rect;
		m_wndStatusBar.GetItemRect(1,rect);
		m_wndProg.MoveWindow(rect);
	
	}
	
}



void CMainFrame::StepIt()
{
	m_wndProg.StepIt();
	return;
}

void CMainFrame::SystemHangUp()
{

	return;
}

void CMainFrame::OnSysHangup() 
{
	if( m_bRun )
	{
		
		SystemHangUp();
	}
	
}

void CMainFrame::OnUpdateSysHangup(CCmdUI* pCmdUI) 
{
		pCmdUI->Enable(m_bRun);
	
}

void CMainFrame::OnMenuHangup() 
{
	if( m_bRun )
	{
		
		SystemHangUp();
	}
	
}

void CMainFrame::OnUpdateMenuHangup(CCmdUI* pCmdUI) 
{
		pCmdUI->Enable(m_bRun);
	
}

void CMainFrame::AutoRun()
{
	if( 1==m_nRunMode )
	{
		OnSysRun() ;
	}
	return;
}
void CMainFrame::Nor32_Exit()
{
	if( NULL != m_pNor32 )
	{
		delete m_pNor32;
		m_pNor32 = NULL;
	}

	return;
}
void CMainFrame::Nor32_Init()
{

	CProgressDlg Dlg;
	Dlg.Create(IDD_DIALOG_PROGRESS,this);
	Dlg.ShowWindow(SW_SHOW);

	CListCtrl	*plstChannel = &(g_pChannelForm->m_lstChannel);
	CListBox	*plistSystem = &(g_pSysmsgForm->m_listSystem);
	CListCtrl	*plistScript = &(g_pChannelForm->m_listScript);
	CListBox	*pCommList	= &(g_pChannelForm->m_CommList);
	CListCtrl	*plistTCPIP = &(g_pChannelForm->m_listTCPIP);
	CListCtrl	*plstConfer= &(g_pChannelForm->m_lstConfer);
	CListCtrl	*plstFax= &(g_pChannelForm->m_lstFax);


	m_pNor32 = new CNorMarch32 ;
	m_pNor32->m_nSysType = m_nSysType;
	m_pNor32->m_nCommPort = m_nCommPort;
	m_pNor32->m_strCommIPAdrr = m_strCommIPAdrr;
	m_pNor32->m_hWnd = GetSafeHwnd( );
	m_pNor32->m_strPath = theApp.m_strPath;
	m_pNor32->m_nFax			= m_nFax;
	m_pNor32->m_nSwitchMode	= m_nSwitchMode;
	m_pNor32->m_pwndProg = &(Dlg.m_wndProg);

	BOOL bResult = m_pNor32->Nor32_InitSystem( plstChannel, plistSystem,plistScript, pCommList, plistTCPIP,plstConfer,plstFax);												 
	if( bResult ) 
	{	
		m_bRun = TRUE;
		m_pNor32->m_bInitVoiceCard = TRUE;
		AfxBeginThread( Nor32_Card, m_pNor32, NULL );
		g_pSysmsgForm->ShowSysMsg("init Script file and Driver load Success !");
	}
	else
	{
		g_pSysmsgForm->ShowSysMsg("init Script file or Driver load fail !");
		m_bRun = FALSE;
		m_pNor32->m_bInitVoiceCard = FALSE;
		delete m_pNor32;
		m_pNor32 = NULL;
		
	}
	
	Dlg.DestroyWindow();
	return;
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::DestroyWindow();
}
