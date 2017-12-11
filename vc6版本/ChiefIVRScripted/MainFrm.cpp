// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ChiefIVRScripted.h"

#include "CustomTemplate.h"
#include "FileListFormView.h"
#include "VoiceFileFormView.h"
#include "NodeRemarkFormView.h"
#include "NodeTreeFormView.h"
#include "CheckBuildFormView.h"
#include "FindFormView.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CNodeTreeFormView *g_pNodeTreeFormView ;
extern CString g_strAppPath;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_COMMAND_EX(CG_ID_VIEW_OUTPUT, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_OUTPUT, OnUpdateControlBarMenu)
	ON_COMMAND_EX(CG_ID_VIEW_WORKSPACE, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_WORKSPACE, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBOX_BAR, OnUpdateViewToolboxBar)
	ON_WM_CLOSE()
	ON_COMMAND(ID_DELETE_NODE, OnDeleteNode)
	//}}AFX_MSG_MAP
	// Global help commands
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


static UINT BASED_CODE ToolBoxButtons[] =
{
	// same order as in the bitmap 
	ID_NOTSELECT,
	ID_WAITCALL,
	ID_START_PLAY,
	ID_CHECK_DIAL,
	ID_RELEASECALL,
	ID_CALLHOLD,
	ID_CHECK_TRANSFER,
	ID_START_RECORD,
	ID_BUILD_CONFERENCE,
	ID_DELETE_CONFERENCE,
	ID_ENTER_CONFERENCE,
	ID_LEAVE_CONFERENCE,
	ID_MEMBER_CONFERENCE,
	ID_SETVARIABLE,
	ID_COMPARE,
	ID_FUNCTION,
	ID_EXPRESS,
	ID_OPEND_DATABASE,
	ID_CLOSE_DATABASE,
	ID_DATABASE_BROWSE,
	ID_STOREDPROC,
	ID_EXCUTE_SQL,
	ID_DLL_LOAD,
	ID_DLL_UNLOAD,
	ID_DLL_CALL,
	ID_RECIVE_DTMF,
	ID_SEND_DTMFCODE,
	ID_DTMF_MENU,
	ID_EVENT_CHECK,
	ID_CALLLOGREQUEST,
	ID_NODE_END,
	ID_SWITCH,
	ID_ANSWER,
	ID_FAXRECIVE,
	ID_FAXSEND,
	ID_BLOCK,
	ID_RETURN,
	ID_GOTO,
	ID_CHANGE_VOICE,
	ID_TRACE_INFO,
	ID_MODULE,
	ID_MOUDLE_END,
	ID_CUSTOM_NODE
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{


}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	
	if (!m_wndScrtipBar.CreateEx(this) ||
		!m_wndScrtipBar.LoadToolBar(IDR_TOOLBAR_SCRIPT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndScrtipBar.SetBarStyle(m_wndScrtipBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);


	if (!m_wndBuildViewBar.CreateEx(this) ||
		!m_wndBuildViewBar.LoadToolBar(IDR_TOOLBAR_BUILD))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndBuildViewBar.SetBarStyle(m_wndBuildViewBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);



	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndBuildViewBar) ||
		!m_wndReBar.AddBar(&m_wndScrtipBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar)
		)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	m_wndReBar.SetBarStyle(m_wndReBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
	m_wndReBar.GetReBarCtrl( ).MinimizeBand(0);
	m_wndReBar.GetReBarCtrl( ).MaximizeBand(1);
	m_wndReBar.GetReBarCtrl( ).MaximizeBand(2);
	m_wndReBar.GetReBarCtrl( ).MinimizeBand(3);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);


	
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);


	LoadBarState(_T("General"));



	if (!CreateWorkspace())
		return -1;
	if (!CreateBottomBar())
		return -1;
	if (!CreateToolBox())
		return -1;


	DWORD dwExStyle = TBSTYLE_EX_DRAWDDARROWS;
	m_wndScrtipBar.GetToolBarCtrl().SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)dwExStyle);

	DWORD dwStyle = m_wndScrtipBar.GetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_WAITCALL));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndScrtipBar.SetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_WAITCALL), dwStyle);

	dwStyle = m_wndScrtipBar.GetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_OPEND_DATABASE));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndScrtipBar.SetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_OPEND_DATABASE), dwStyle);

	dwStyle = m_wndScrtipBar.GetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_SETVARIABLE));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndScrtipBar.SetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_SETVARIABLE), dwStyle);

	dwStyle = m_wndScrtipBar.GetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_DLL_LOAD));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndScrtipBar.SetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_DLL_LOAD), dwStyle);

	dwStyle = m_wndScrtipBar.GetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_EVENT_CHECK));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndScrtipBar.SetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_EVENT_CHECK), dwStyle);

	dwStyle = m_wndScrtipBar.GetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_CALLLOGREQUEST));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndScrtipBar.SetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_CALLLOGREQUEST), dwStyle);


	dwStyle = m_wndScrtipBar.GetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_BLOCK));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndScrtipBar.SetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_BLOCK), dwStyle);


	dwStyle = m_wndScrtipBar.GetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_CUSTOM_NODE));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndScrtipBar.SetButtonStyle(m_wndScrtipBar.CommandToIndex(ID_CUSTOM_NODE), dwStyle);



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
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
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

BOOL CMainFrame::CreateToolBox()
{




	// Create the Palette.  We are using hardcoded numbers for ease here
	// normally the location would be read in from an ini file.
	CPoint pt(GetSystemMetrics(SM_CXSCREEN) - 100,
		GetSystemMetrics(SM_CYSCREEN) / 3);


	


	return TRUE;
}

BOOL CMainFrame::CreateToolBar()
{
	if (!m_wndToolBar.CreateEx(this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		return FALSE;      // fail to create
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);



	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;       // fail to create
	}

	UINT nID, nStyle;
	int cxWidth;

	m_wndStatusBar.GetPaneInfo( 0, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo( 0, nID, SBPS_STRETCH|SBPS_NORMAL, cxWidth);

	LoadBarState(_T("General"));

	return TRUE;
}

BOOL CMainFrame::CreateWorkspace()
{
	// Create the image list to be used with the control bars.

	if( !m_ImageList.Create(IDB_IMAGELIST, 13, 1, RGB(0,255,0)) )
		return FALSE;

	m_wndWorkspace.SetBtnImageList ( &m_ImageList );
	m_wndWorkspace.SetMenuID(IDR_POPUP);

	// Initialize dialog bar m_wndWorkspace
	if (!m_wndWorkspace.Create(this, CG_ID_VIEW_WORKSPACE,
								_T("Workspace"), CSize(220,200)))
	{
		return FALSE;		// fail to create
	}

	// Add the views to the tab control.
	m_wndWorkspace.AddView(_T("Node View"),    RUNTIME_CLASS(CNodeTreeFormView));
	//m_wndWorkspace.AddView(_T("Voice List"),	RUNTIME_CLASS(CVoiceFileFormView));
	m_wndWorkspace.AddView(_T("BookMarks"),    RUNTIME_CLASS(CNodeRemarkFormView));
	m_wndWorkspace.AddView(_T("FileView"),    RUNTIME_CLASS(CFileListFormView));
	
	// Define the image list to use with the tab control
	m_LTabImages.Create (IDB_IL_TAB, 16, 1, RGB(0,255,0));
	m_wndWorkspace.SetTabImageList(&m_LTabImages);

	// allow bar to be resized when floating
	m_wndWorkspace.SetBarStyle(m_wndWorkspace.GetBarStyle() |
								CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndWorkspace.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndWorkspace, AFX_IDW_DOCKBAR_LEFT);

	return TRUE;
}

BOOL CMainFrame::CreateBottomBar()
{
	// Create the image list to be used with the control bars.

	if( !m_BImageList.Create(IDB_BIMAGELIST, 13, 1, RGB(0,255,0)) )
		return FALSE;


	m_wndBottomBar.SetBtnImageList ( &m_BImageList );
	m_wndBottomBar.SetMenuID(IDR_POPUP);

	// Initialize dialog bar m_wndWorkspace
	if (!m_wndBottomBar.Create(this, CG_ID_VIEW_OUTPUT,
								_T("Workspace"), CSize(220,200)))
	{
		return FALSE;		// fail to create
	}

	// Add the views to the tab control.
	m_wndBottomBar.AddView(_T("Check/Build"),    RUNTIME_CLASS(CCheckBuildFormView));
	m_wndBottomBar.AddView(_T("Find Node"), RUNTIME_CLASS(CFindFormView));
	
	
	// Define the image list to use with the tab control
	m_BTabImages.Create (IDB_IB_TAB, 16, 1, RGB(0,255,0));
	m_wndBottomBar.SetTabImageList(&m_BTabImages);

	// allow bar to be resized when floating
	m_wndBottomBar.SetBarStyle(m_wndBottomBar.GetBarStyle() |
								CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndBottomBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndBottomBar, AFX_IDW_DOCKBAR_BOTTOM);


	return TRUE;
}

void CMainFrame::OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	CChildFrame *pChild  = (CChildFrame *)GetActiveFrame( );
	CDrawDoc *pDoc = (CDrawDoc *)pChild->GetActiveDocument( );
	 if (pDoc == NULL || !pDoc->IsKindOf( RUNTIME_CLASS(CDrawDoc) ) )
	 {
		int i = 0;
	 }

	int nDocType = pDoc->m_nDocType ;
	
	CMenu menu;	
	if( 0 == pDoc->m_nDocType )//0 ief文件 1 nmf文件
	{
		menu.LoadMenu( IDR_CHIEFITYPE );
	}
	else
	{
		
		menu.LoadMenu( IDR_MODULE );
	}

	//添加新的子项 
	CCustomTemplateDoc doc;
	CString strFile =  g_strAppPath + "\\Template.nor";
	if( !doc.OnOpenDocument( strFile ) )
	{
		return;
	}
	
	POSITION pos = doc.m_ObjLst.GetHeadPosition();
	while (pos != NULL)
	{
		CCustomObj  *pObj = (CCustomObj  *)doc.m_ObjLst.GetNext(pos);
		if( NULL == pObj ) continue;
		int n = pObj->m_nKey;
		menu.GetSubMenu(1)->GetSubMenu(8)->AppendMenu(MF_STRING,n,_T(pObj->m_strCaption)); 
		
	}//end while
	
	

	CRect rc;
    CWnd *pWnd = NULL;
    
    switch (pnmtb->iItem)
    {
    case ID_WAITCALL:
        pWnd = &m_wndScrtipBar;
		pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);

		menu.GetSubMenu(1)->GetSubMenu(1)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
															rc.left, rc.bottom, this, &rc);
        break;

	case ID_OPEND_DATABASE:

        pWnd = &m_wndScrtipBar;
		pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);

		menu.GetSubMenu(1)->GetSubMenu(2)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
															rc.left, rc.bottom, this, &rc);
        break;

	case ID_SETVARIABLE:
        pWnd = &m_wndScrtipBar;
		pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);

		menu.GetSubMenu(1)->GetSubMenu(3)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
															rc.left, rc.bottom, this, &rc);
        break;
		
	case ID_DLL_LOAD:

        pWnd = &m_wndScrtipBar;
		pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);

		menu.GetSubMenu(1)->GetSubMenu(4)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
															rc.left, rc.bottom, this, &rc);
        break;
		
	case ID_EVENT_CHECK:
        pWnd = &m_wndScrtipBar;
		pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);

		menu.GetSubMenu(1)->GetSubMenu(5)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
															rc.left, rc.bottom, this, &rc);
        break;

	case ID_CALLLOGREQUEST:
        pWnd = &m_wndScrtipBar;
		pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);

		menu.GetSubMenu(1)->GetSubMenu(6)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
															rc.left, rc.bottom, this, &rc);
        break;
	case ID_BLOCK:
        pWnd = &m_wndScrtipBar;
		pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);

		menu.GetSubMenu(1)->GetSubMenu(7)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
															rc.left, rc.bottom, this, &rc);
        break;
	case ID_CUSTOM_NODE:
        pWnd = &m_wndScrtipBar;
		pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);

		menu.GetSubMenu(1)->GetSubMenu(8)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
															rc.left, rc.bottom, this, &rc);
        break;
    default:
        return;
    }
    

	menu.DestroyMenu();
}

void CMainFrame::OnUpdateViewToolboxBar(CCmdUI* pCmdUI) 
{
	
	
}

void CMainFrame::OnClose() 
{
	
	
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnDeleteNode() 
{
	if( NULL == g_pNodeTreeFormView ) return;

	g_pNodeTreeFormView->DeleteTreeNode();

	return;
	
}

BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIFrameWnd::OnNotify(wParam, lParam, pResult);
}

void CMainFrame::ShowCustomerMenu()
{
	
	HMENU hMainMenu =::GetMenu(m_hWnd);// LoadMenu(AfxGetApp( )->m_hInstance,MAKEINTRESOURCE(IDR_CHIEFITYPE));//
	HMENU hSubMenu = ::GetSubMenu(hMainMenu,1);
	HMENU hSubMenu1 = ::GetSubMenu(hSubMenu,8);
	//添加新的子项 
	CCustomTemplateDoc doc;
	CString strFile =  g_strAppPath + "\\Template.nor";
	if( !doc.OnOpenDocument( strFile ) )
	{
		doc.OnNewDocument();
		doc.OnSaveDocument( strFile );
	}

	POSITION pos = doc.m_ObjLst.GetHeadPosition();
	while (pos != NULL)
	{
		CCustomObj  *pObj = (CCustomObj  *)doc.m_ObjLst.GetNext(pos);
		if( NULL == pObj ) continue;
		int n = pObj->m_nKey;
		::DeleteMenu(hSubMenu1,n,MF_BYCOMMAND);
		::AppendMenu(hSubMenu1,MF_STRING,n,_T(pObj->m_strCaption)); 
				
	}//end while


	::SetMenu(m_hWnd, hMainMenu);//显示菜单



}
