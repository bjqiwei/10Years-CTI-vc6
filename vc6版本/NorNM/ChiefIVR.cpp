// ChiefIVR.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ChiefIVR.h"
#include "MainFrm.h"
//#include "CIF5188.h"
#include "ChiefIVRView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CD08A g_d08a;
CD08A *g_pD08a = &(g_d08a);
CPublicCardObj *g_pPubObj=NULL;
CMainFrame *g_pMainFrame=NULL;
/////////////////////////////////////////////////////////////////////////////
// CChiefIVRApp

BEGIN_MESSAGE_MAP(CChiefIVRApp, CWinApp)
	//{{AFX_MSG_MAP(CChiefIVRApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRApp construction

CChiefIVRApp::CChiefIVRApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChiefIVRApp object

CChiefIVRApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRApp initialization

BOOL CChiefIVRApp::InitInstance()
{
	#ifdef _DEBUG
		afxMemDF = allocMemDF | checkAlwaysMemDF;
	#endif //_DEBUG

	// 此程序只能运行一次，用互斥量来判断程序是否已运行
    HANDLE m_hMutex=CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS) { return FALSE; }

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	
	AfxEnableControlContainer();
	

	char  pCurDir[MAX_PATH]; 
	GetCurrentDirectory(MAX_PATH, pCurDir);
	m_strPath.Format("%s",pCurDir);

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if( !g_pD08a->OpenSense4() )
	{
		AfxMessageBox("Init the Key fail");
		return FALSE;

	}


	// Initialize OLE libraries
	
	if (!AfxOleInit())
	{
		AfxMessageBox("Ole init fail!");
		return FALSE;
	}
	
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CChiefIVRDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CChiefIVRView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_bNormalShow = TRUE;
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->SetWindowText("NorthMarch CIF-Studio IVR&CTI");
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	g_pMainFrame->AutoRun();

	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CBitmap *m_pbmp;

	CAboutDlg();
	virtual ~CAboutDlg();
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CEdit	m_edtWarn;
	CEdit	m_edtAddation;
	CEdit	m_edtLicence;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_pbmp = NULL;
}

CAboutDlg::~CAboutDlg()
{
	if( m_pbmp )
	{
		m_pbmp->Detach();
		m_pbmp->DeleteObject();
		delete m_pbmp;
		m_pbmp = NULL;

	}

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_EDIT_WARNING, m_edtWarn);
	DDX_Control(pDX, IDC_EDIT_ADDATION, m_edtAddation);
	DDX_Control(pDX, IDC_EDIT_LICENCE, m_edtLicence);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_edtWarn.SetWindowText( "Warning: This computer program is protected by copyright law and international treaties. Unauthorized reproduction or distribution of this program, or any portion of it, may result in severe civil and criminal penalties, and will be prosecuted under the maximum extent possible under law." );

	//m_edtAddation.SetSel(-1,0);
	CString strLicence="";
	CString strAddation="";
	CString strCompany = "";
	CString strHeTong = "";
	CString strID = "";
	CString strDate = "";

	g_pD08a->GetCompany( strCompany ) ;
	g_pD08a->GetHeTongBianHao( strHeTong );
	g_pD08a->GetProuctID( strID );
	g_pD08a->GetLicenceDate( strDate );
	strLicence = "Licensed to: " + strCompany ;
	strLicence = strLicence + "\r\nContract serial number: " ;
	strLicence = strLicence + strHeTong;
	strLicence = strLicence + "\r\nProduct ID: ";
	strLicence = strLicence + strID;
	m_edtLicence.SetWindowText( strLicence );
	
	strAddation = "version number ? 3.1";
	strAddation = strAddation + "\r\nLicensed Date : " + strDate;
	m_edtAddation.SetWindowText( strAddation );
	//m_edtAddation.SetSel(-1,1);
	//m_edtAddation.SetWindowText( "klllk\r\nhhhhhhh" );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// App command to run the dialog
void CChiefIVRApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////
// CChiefIVRApp message handlers

