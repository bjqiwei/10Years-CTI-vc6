// ChiefIVRScripted.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ChiefIVRScripted.h"
#include "..//include//NodeObj.h"
#include "MyMultiDocTemplate.h"


#include "ChildFrm.h"
#include "DrawDoc.h"
#include "DrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString g_strAppPath;
void AFXAPI MyDDV_CheckChars(CDataExchange* pDX, CString const& value, int nMaxChars,int nMinChars, CString strMessage)
{
	ASSERT(nMaxChars >= 1);        // allow them something
	if (pDX->m_bSaveAndValidate && ( value.GetLength() > nMaxChars || value.GetLength() <= nMinChars ) )
	{
		AfxMessageBox(strMessage, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		pDX->Fail();
	}
	else if (pDX->m_hWndLastControl != NULL && pDX->m_bEditLastControl)
	{
		// limit the control max-chars automatically
		::SendMessage(pDX->m_hWndLastControl, EM_LIMITTEXT, nMaxChars, 0);
	}
}

void SetNodeImageList( CImageList *pImageList )
{
	int i=0;
	HICON hIcon[62];

	pImageList->Create(16,16,ILC_COLOR4,3,3);
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_TREE_ROOT);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_START);
	hIcon[2] = AfxGetApp()->LoadIcon(IDI_CALLHOLD);
	hIcon[3] = AfxGetApp()->LoadIcon(IDI_TRANSFER_START);
	hIcon[4] = AfxGetApp()->LoadIcon(IDI_WAITCALL);
	hIcon[5] = AfxGetApp()->LoadIcon(IDI_COMPARE);
	hIcon[6] = AfxGetApp()->LoadIcon(IDI_DIAL_START);
	hIcon[7] = AfxGetApp()->LoadIcon(IDI_PLAY_START);
	hIcon[8] = AfxGetApp()->LoadIcon(IDI_RECORD_START);
	hIcon[9] = AfxGetApp()->LoadIcon(IDI_RELASECALL);
	hIcon[10] = AfxGetApp()->LoadIcon(IDI_SETVARIABLE);
	hIcon[11] = AfxGetApp()->LoadIcon(IDI_TRANSFER_CHECK);
	hIcon[12] = AfxGetApp()->LoadIcon(IDI_RECORD_STOP);
	hIcon[13] = AfxGetApp()->LoadIcon(IDI_PLAY_CHECK);
	hIcon[14] = AfxGetApp()->LoadIcon(IDI_DATABASE_CLOASE);
	hIcon[15] = AfxGetApp()->LoadIcon(IDI_DATABASE_OPEN);
	hIcon[16] = AfxGetApp()->LoadIcon(IDI_EXCUTE_SQL);
	hIcon[17] = AfxGetApp()->LoadIcon(IDI_CONFERENCE_ENTER);
	hIcon[18] = AfxGetApp()->LoadIcon(IDI_CONFERENCE_LEAVE);
	hIcon[19] = AfxGetApp()->LoadIcon(IDI_DIAL_CHECK);
	hIcon[20] = AfxGetApp()->LoadIcon(IDI_VARIABLE_DBCON);
	hIcon[21] = AfxGetApp()->LoadIcon(IDI_VARIABLE_DLL);
	hIcon[22] = AfxGetApp()->LoadIcon(IDI_VARIABLE_STR);
	hIcon[23] = AfxGetApp()->LoadIcon(IDI_VARIABLE_INT);
	hIcon[24] = AfxGetApp()->LoadIcon(IDI_VARIABLE_DBREC);
	hIcon[25] = AfxGetApp()->LoadIcon(IDI_VARIABLE_FLOAT);
	hIcon[26] = AfxGetApp()->LoadIcon(IDI_DTMF_SEND);
	hIcon[27] = AfxGetApp()->LoadIcon(IDI_DTMF_RECIVE);
	hIcon[28] = AfxGetApp()->LoadIcon(IDI_DLL_LOAD);
	hIcon[29] = AfxGetApp()->LoadIcon(IDI_DLL_CALL);
	hIcon[30] = AfxGetApp()->LoadIcon(IDI_DLL_UNLOAD);
	hIcon[31] = AfxGetApp()->LoadIcon(IDI_EVENT_CHECK);
	hIcon[32] = AfxGetApp()->LoadIcon(IDI_DATABASE_BROWSE);
	hIcon[33] = AfxGetApp()->LoadIcon(IDI_DTMF_MENU);
	hIcon[34] = AfxGetApp()->LoadIcon(IDI_CALLLOGREQUEST);
	hIcon[35] = AfxGetApp()->LoadIcon(IDI_NODE_END);
	hIcon[36] = AfxGetApp()->LoadIcon(IDI_SWITCH);
	hIcon[37] = AfxGetApp()->LoadIcon(IDI_ANSWER);
	hIcon[38] = AfxGetApp()->LoadIcon(IDI_FAXRECIVE);
	hIcon[39] = AfxGetApp()->LoadIcon(IDI_FAXSEND);
	hIcon[40] = AfxGetApp()->LoadIcon(IDI_CONFER_BUILD);
	hIcon[41] = AfxGetApp()->LoadIcon(IDI_CONFER_DELETE);
	hIcon[42] = AfxGetApp()->LoadIcon(IDI_CONFER_MEMBER);
	hIcon[43] = AfxGetApp()->LoadIcon(IDI_CONFER_REQUEST);
	hIcon[44] = AfxGetApp()->LoadIcon(IDI_BLOCK);
	hIcon[45] = AfxGetApp()->LoadIcon(IDI_ENTER);
	hIcon[46] = AfxGetApp()->LoadIcon(IDI_RETURN);
	hIcon[47] = AfxGetApp()->LoadIcon(IDI_STOREDPROC);
	hIcon[48] = AfxGetApp()->LoadIcon(IDI_CHANGE_VOICE);
	hIcon[49] = AfxGetApp()->LoadIcon(IDI_ENDCALLEVENT_CHECK);
	hIcon[50] = AfxGetApp()->LoadIcon(IDI_ENDCALLEVENT_END);
	hIcon[51] = AfxGetApp()->LoadIcon(IDI_EXPRESS);
	hIcon[52] = AfxGetApp()->LoadIcon(IDI_FUNCTION);
	hIcon[53] = AfxGetApp()->LoadIcon(IDI_GOTO);
	hIcon[54] = AfxGetApp()->LoadIcon(IDI_ROUTECALL);
	hIcon[55] = AfxGetApp()->LoadIcon(IDI_TRACE_INFO);
	hIcon[56] = AfxGetApp()->LoadIcon(IDI_MODULE);
	hIcon[57] = AfxGetApp()->LoadIcon(IDI_MOUDLE_BEGIN);
	hIcon[58] = AfxGetApp()->LoadIcon(IDI_MOUDLE_END);
	hIcon[59] = AfxGetApp()->LoadIcon(IDI_CUSTOM_NODE);
	hIcon[60] = AfxGetApp()->LoadIcon(IDI_CC_TRANSFER);
	hIcon[61] = AfxGetApp()->LoadIcon(IDI_CC_CALLAGENT);
	for(i = 0 ; i < 62 ; i++)
	{
		pImageList->Add(hIcon[i]);
	}	
	
	return;
}

UINT GetImageIcoID( int ImageIndex )
{
	switch( ImageIndex )
	{
	case 0:
		return IDI_TREE_ROOT;
		break;
	case 1:
		return IDI_START;
		break;
	case 2:
		return IDI_CALLHOLD;
		break;
	case 3:
		return IDI_TRANSFER_START;
		break;
	case 4:
		return IDI_WAITCALL;
		break;
	case 5:
		return IDI_COMPARE;
		break;
	case 6:
		return IDI_DIAL_START;
		break;
	case 7:
		return IDI_PLAY_START;
		break;
	case 8:
		return IDI_RECORD_START;
		break;
	case 9:
		return IDI_RELASECALL;
		break;
	case 10:
		return IDI_SETVARIABLE;
		break;
	case 11:
		return IDI_TRANSFER_CHECK;
		break;
	case 12:
		return IDI_RECORD_STOP;
		break;
	case 13:
		return IDI_PLAY_CHECK;
		break;
	case 14:
		return IDI_DATABASE_CLOASE;
		break;
	case 15:
		return IDI_DATABASE_OPEN;
		break;
	case 16:
		return IDI_EXCUTE_SQL;
		break;
	case 17:
		return IDI_CONFERENCE_ENTER;
		break;
	case 18:
		return IDI_CONFERENCE_LEAVE;
		break;
	case 19:
		return IDI_DIAL_CHECK;
		break;
	case 20:
		return IDI_VARIABLE_DBCON;
		break;
	case 21:
		return IDI_VARIABLE_DLL;
		break;
	case 22:
		return IDI_VARIABLE_STR;
		break;
	case 23:
		return IDI_VARIABLE_INT;
		break;
	case 24:
		return IDI_VARIABLE_DBREC;
		break;
	case 25:
		return IDI_VARIABLE_FLOAT;
		break;

	case 26:
		return IDI_DTMF_SEND;
		break;
	case 27:
		return IDI_DTMF_RECIVE;
		break;
	case 28:
		return IDI_DLL_LOAD;
		break;
	case 29:
		return IDI_DLL_CALL;
		break;
	case 30:
		return IDI_DLL_UNLOAD;
		break;
	case 31:
		return IDI_EVENT_CHECK;
		break;
	case 32:
		return IDI_DATABASE_BROWSE;
		break;
	case 33:
		return IDI_DTMF_MENU;
		break;		
	case 34:
		return IDI_CALLLOGREQUEST;
		break;
	case 35:
		return IDI_NODE_END;
		break;
	case 36:
		return IDI_SWITCH;
		break;
	case 37:
		return IDI_ANSWER;
		break;
	case 38:
		return IDI_FAXRECIVE;
		break;
	case 39:
		return IDI_FAXSEND;
		break;
	case 40:
		return IDI_CONFER_BUILD;
		break;
	case 41 :
		return IDI_CONFER_DELETE;
		break;
	case 42 :
		return IDI_CONFER_MEMBER;
		break;
	case 43 :
		return IDI_CONFER_REQUEST;
		break;
	case 44 :
		return IDI_BLOCK;
		break;
	case 45 :
		return IDI_ENTER;
		break;
	case 46 :
		return IDI_RETURN;
		break;
	case 47 :
		return IDI_STOREDPROC;
		break;
	case 48 :
		return IDI_CHANGE_VOICE;
		break;
	case 49 :
		return IDI_ENDCALLEVENT_CHECK;
		break;
	case 50 :
		return IDI_ENDCALLEVENT_END;
		break;	
	case 51 :
		return IDI_EXPRESS;
		break;
	case 52 :
		return IDI_FUNCTION;
		break;	
	case 53 :
		return IDI_GOTO;
		break;
	case 54 :
		return IDI_ROUTECALL;
		break;	
	case 55 :
		return IDI_TRACE_INFO;
		break;
	case 56 :
		return IDI_MODULE;
		break;
	case 57 :
		return IDI_MOUDLE_BEGIN;
		break;
	case 58 :
		return IDI_MOUDLE_END;
		break;
	case 59:
		return IDI_CUSTOM_NODE;
		break;
	case 60 :
		return IDI_CC_TRANSFER;
		break;
	case 61:
		return IDI_CC_CALLAGENT;
		break;
	default:
		return IDI_CUSTOM_NODE;
		break;
	}

}

int GetImageIndex(UINT nIco)
{
	switch( nIco )
	{
	case IDI_TREE_ROOT:
		return 0;
		break;
	case IDI_START:
		return 1;
		break;
	case IDI_CALLHOLD:
		return 2;
		break;
	case IDI_TRANSFER_START:
		return 3;
		break;
	case IDI_WAITCALL:
		return 4;
		break;
	case IDI_COMPARE:
		return 5;
		break;
	case IDI_DIAL_START:
		return 6;
		break;
	case IDI_PLAY_START:
		return 7;
		break;
	case IDI_RECORD_START:
		return 8;
		break;
	case IDI_RELASECALL:
		return 9;
		break;
	case IDI_SETVARIABLE:
		return 10;
		break;
	case IDI_TRANSFER_CHECK:
		return 11;
		break;
	case IDI_RECORD_STOP:
		return 12;
		break;
	case IDI_PLAY_CHECK:
		return 13;
		break;
	case IDI_DATABASE_CLOASE:
		return 14;
		break;
	case IDI_DATABASE_OPEN:
		return 15;
		break;
	case IDI_EXCUTE_SQL:
		return 16;
		break;
	case IDI_CONFERENCE_ENTER:
		return 17;
		break;
	case IDI_CONFERENCE_LEAVE:
		return 18;
		break;
	case IDI_DIAL_CHECK:
		return 19;
		break;
	case IDI_VARIABLE_DBCON:
		return 20;
		break;
	case IDI_VARIABLE_DLL:
		return 21;
		break;
	case IDI_VARIABLE_STR:
		return 22;
		break;
	case IDI_VARIABLE_INT:
		return 23;
		break;
	case IDI_VARIABLE_DBREC:
		return 24;
		break;
	case IDI_VARIABLE_FLOAT:
		return 25;
		break;

	case IDI_DTMF_SEND:
		return 26;
		break;
	case IDI_DTMF_RECIVE:
		return 27;
		break;
	case IDI_DLL_LOAD:
		return 28;
		break;
	case IDI_DLL_CALL:
		return 29;
		break;
	case IDI_DLL_UNLOAD:
		return 30;
		break;
	case IDI_EVENT_CHECK:
		return 31;
		break;
	case IDI_DATABASE_BROWSE:
		return 32;
		break;
	case IDI_DTMF_MENU:
		return 33;
		break;		
	case IDI_CALLLOGREQUEST:
		return 34;
		break;
	case IDI_NODE_END:
		return 35;
		break;
	case IDI_SWITCH:
		return 36;
		break;
	case IDI_ANSWER:
		return 37;
		break;
	case IDI_FAXRECIVE:
		return 38;
		break;
	case IDI_FAXSEND:
		return 39;
		break;
	case IDI_CONFER_BUILD:
		return 40;
		break;
	case IDI_CONFER_DELETE:
		return 41;
		break;
	case IDI_CONFER_MEMBER:
		return 42;
		break;
	case IDI_CONFER_REQUEST:
		return 43;
		break;
	case IDI_BLOCK:
		return 44;
		break;
	case IDI_ENTER:
		return 45;
		break;
	case IDI_RETURN:
		return 46;
		break;
	case IDI_STOREDPROC:
		return 47;
		break;
	case IDI_CHANGE_VOICE:
		return 48;
		break;
	case IDI_ENDCALLEVENT_CHECK:
		return 49;
		break;
	case IDI_ENDCALLEVENT_END:
		return 50;
		break;	
	case IDI_EXPRESS:
		return 51;
		break;
	case IDI_FUNCTION:
		return 52;
		break;	
	case IDI_GOTO:
		return 53;
		break;
	case IDI_ROUTECALL:
		return 54;
		break;	
	case IDI_TRACE_INFO:
		return 55;
		break;
	case IDI_MODULE:
		return 56;
		break;
	case IDI_MOUDLE_BEGIN:
		return 57;
		break;
	case IDI_MOUDLE_END:
		return 58;
		break;
	case IDI_CUSTOM_NODE:
		return 59;
		break;
	case IDI_CC_TRANSFER :
		return 60;
		break;
	case IDI_CC_CALLAGENT:
		return 61;
		break;
	default:
		return 0;
		break;
	}

}
/////////////////////////////////////////////////////////////////////////////
// CChiefIVRScriptedApp

BEGIN_MESSAGE_MAP(CChiefIVRScriptedApp, CWinApp)
	//{{AFX_MSG_MAP(CChiefIVRScriptedApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRScriptedApp construction

CChiefIVRScriptedApp::CChiefIVRScriptedApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChiefIVRScriptedApp object

CChiefIVRScriptedApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRScriptedApp initialization

BOOL CChiefIVRScriptedApp::InitInstance()
{
	// example for afxMemDF
	#ifdef _DEBUG
		afxMemDF = allocMemDF | checkAlwaysMemDF;
	#endif //_DEBUG


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	AfxEnableControlContainer();
	
	char  pCurDir[MAX_PATH]; 
	GetCurrentDirectory(MAX_PATH, pCurDir);
	g_strAppPath.Format("%s",pCurDir);


	

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
	SetRegistryKey(_T("China Beijing NorthMarch"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.



	
	CMyMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMyMultiDocTemplate(
		IDR_CHIEFITYPE,
		RUNTIME_CLASS(CDrawDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDrawView));
	pDocTemplate->SetContainerInfo(IDR_CHIEFITYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);
	

	CMyMultiDocTemplate* pDocTemplate2;
	pDocTemplate2 = new CMyMultiDocTemplate(
		IDR_MODULE,
		RUNTIME_CLASS(CDrawDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDrawView));
	pDocTemplate2->SetContainerInfo(IDR_MODULE);
	AddDocTemplate(pDocTemplate2);

	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	

	


	m_nCmdShow=SW_SHOWMAXIMIZED;//×î´ó»¯
	m_pMainWnd->SetWindowText("NortMarch CIF-Studio Script Editor");
	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

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
	afx_msg void OnStaticWww();
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
	ON_BN_CLICKED(IDC_STATIC_WWW, OnStaticWww)
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
void CChiefIVRScriptedApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////
// CChiefIVRScriptedApp message handlers


void CChiefIVRScriptedApp::OnFileNew() 
{

	m_pDocManager->OnFileNew();

	int n = m_pDocManager->GetOpenDocumentCount();

}

void CChiefIVRScriptedApp::AddDocTemplate(CDocTemplate *pTemplate)
{
	if (m_pDocManager == NULL)
		m_pDocManager = new CMyDocManager;
	m_pDocManager->AddDocTemplate(pTemplate);
}

void CChiefIVRScriptedApp::OnFileOpen() 
{

	ASSERT(m_pDocManager != NULL);
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled
	
	 CDocument *pDoc = m_pDocManager->OpenDocumentFile(newName);
	 

}



void CAboutDlg::OnStaticWww() 
{
	ShellExecute(NULL,"open","http://www.normarch.com",NULL,NULL,   SW_SHOW);   
	
}
