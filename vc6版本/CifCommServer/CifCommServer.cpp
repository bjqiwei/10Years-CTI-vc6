// CifCommServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CifCommServer.h"

#include "MainFrm.h"
#include "CifCommServerDoc.h"
#include "CifCommServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerApp

BEGIN_MESSAGE_MAP(CCifCommServerApp, CWinApp)
	//{{AFX_MSG_MAP(CCifCommServerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerApp construction

CCifCommServerApp::CCifCommServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCifCommServerApp object

CCifCommServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerApp initialization

BOOL CCifCommServerApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

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
		RUNTIME_CLASS(CCifCommServerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCifCommServerView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_bNormalShow = TRUE;
	m_pMainWnd->SetWindowText(" NorthMarch CIF-Studio Communication Server ");
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
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
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CCFCommServerApp message handlers





BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) 
{
	if( NULL == m_pbmp)
	{
		m_pbmp = new CBitmap;
		m_pbmp->LoadBitmap(IDB_BITMAP_ABOUT);

	}
		BITMAP bm;
		CDC dcMem;
		m_pbmp->GetObject( sizeof(bm),(LPVOID)&bm);
		dcMem.CreateCompatibleDC( pDC );
		CBitmap *pBmpOld = (CBitmap*)dcMem.SelectObject(m_pbmp);
		CRect reClient;
		GetClientRect(reClient);
		pDC->StretchBlt(0,0,reClient.right - reClient.left, 
			reClient.bottom - reClient.top,
			&dcMem,
			0,0,
			bm.bmWidth,bm.bmHeight,SRCCOPY);
		dcMem.SelectObject(pBmpOld);
	
	return TRUE;
}
// App command to run the dialog
void CCifCommServerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerApp message handlers

