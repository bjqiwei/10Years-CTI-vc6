// CFTabCtrlBar.cpp: implementation of the CCFTabCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFTabCtrlBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CCFTabCtrlBar
//////////////////////////////////////////////////////////////////////

CCFTabCtrlBar::CCFTabCtrlBar()
{
	m_nActiveTab = 0;

	m_ToolTip = NULL;
	m_pActiveView = NULL;
	
}

CCFTabCtrlBar::~CCFTabCtrlBar()
{
	while(!m_views.IsEmpty())
	{
		TCB_ITEM *pMember=m_views.RemoveHead();
		delete pMember;
	}
}

#define IDC_TABCTRLBAR 1000

BEGIN_MESSAGE_MAP(CCFTabCtrlBar, CCFControlBar)
	//{{AFX_MSG_MAP(CCFTabCtrlBar)
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRLBAR, OnTabSelChange)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCFTabCtrlBar message handlers

// *** K.Stowell
void CCFTabCtrlBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd *pWnd = NULL;
	
	if (IsFloating()) {
		m_tabctrl.MoveWindow( 5, 5, lpwndpos->cx-10, lpwndpos->cy-7 );
		for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos)) {
			pWnd=m_views.GetAt(pos)->pWnd;
			pWnd->MoveWindow(8, 8, lpwndpos->cx-16, lpwndpos->cy-34);
		}
	}
	
	else if (IsHorzDocked()) {
		m_tabctrl.MoveWindow( 17, 5, lpwndpos->cx-25, lpwndpos->cy-17 );
		for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos)) {
			pWnd=m_views.GetAt(pos)->pWnd;
			pWnd->MoveWindow(20, 8, lpwndpos->cx-31, lpwndpos->cy-44);
		}
	}
	
	else {
		m_tabctrl.MoveWindow( 5, 20, lpwndpos->cx-17, lpwndpos->cy-31 );
		for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos)) {
			pWnd=m_views.GetAt(pos)->pWnd;
			pWnd->MoveWindow(8, 23, lpwndpos->cx-23, lpwndpos->cy-58);
		}
	}

	CCFControlBar::OnWindowPosChanged(lpwndpos);
}

// *** K.Stowell
CImageList* CCFTabCtrlBar::SetTabImageList(CImageList *pImageList)
{
	return m_tabctrl.SetImageList (pImageList);
}

// *** K.Stowell
BOOL CCFTabCtrlBar::ModifyTabStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	return m_tabctrl.ModifyStyle(dwRemove, dwAdd);
}

// *** K.Stowell
int CCFTabCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCFControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//Create the Tab Control
	if (!m_tabctrl.Create(WS_VISIBLE|WS_CHILD|TCS_BOTTOM|TCS_TOOLTIPS, 
		CRect(0,0,0,0), this, IDC_TABCTRLBAR))
	{
		TRACE0("Unable to create tab control bar\n");
		return -1;
	}
	
	// set "normal" GUI-font
	CFont *font = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	m_tabctrl.SetFont(font);

// VC5 Support.
#if _MSC_VER >= 1200
	m_ToolTip = m_tabctrl.GetToolTips();
	m_tabctrl.SetToolTips(m_ToolTip);
#else
	m_ToolTip = m_tabctrl.GetTooltips();
	m_tabctrl.SetTooltips(m_ToolTip);
#endif
	return 0;
}

// *** KStowell
void CCFTabCtrlBar::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetActiveView(m_tabctrl.GetCurSel());
	Invalidate();
}

//////////////////////////////////////////////////
// The remainder of this class was written by Dirk Clemens...

BOOL CCFTabCtrlBar::AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext)
{	

#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	CCreateContext context;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		//  client if possible
		CView* pOldView = NULL;
		if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(context.m_pCurrentFrame == NULL);
			context.m_pLastView = pOldView;
			context.m_pCurrentDoc = pOldView->GetDocument();
			if (context.m_pCurrentDoc != NULL)
				context.m_pNewDocTemplate =
				context.m_pCurrentDoc->GetDocTemplate();
		}
		pContext = &context;
	}
	
	CWnd* pWnd = NULL;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL
		
    ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created
	
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	CRect rect;
	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, this, 0, pContext))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	m_pActiveView = (CView*) pWnd;

	TCB_ITEM *pMember=new TCB_ITEM;
	pMember->pWnd=pWnd;
	memmove(  pMember->szLabel, lpszLabel, strlen(lpszLabel)>sizeof(pMember->szLabel) ? sizeof(pMember->szLabel):strlen(lpszLabel) );
	//strcpy( pMember->szLabel, lpszLabel);
	m_views.AddTail(pMember);

	// ToolTip support for tabs.
	if((m_views.GetCount()-1)==0) {
		m_ToolTip->AddTool( &m_tabctrl, lpszLabel,
			NULL, m_views.GetCount()-1 );
	}
	else {
		m_ToolTip->AddTool( &m_tabctrl, lpszLabel,
			CRect(0,0,0,0), m_views.GetCount()-1 );
	}
	
	int nViews = m_views.GetCount();
	if (nViews!=1)
	{
		pWnd->EnableWindow(FALSE);
		pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		((CFrameWnd *)GetParent())->SetActiveView((CView *)m_pActiveView);
	}

	TC_ITEM tci;
	tci.mask = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText = (LPTSTR)(LPCTSTR)lpszLabel;
	tci.iImage = nViews-1;
	m_tabctrl.InsertItem(nViews, &tci);

	return TRUE;
}

void CCFTabCtrlBar::RemoveView(int nView)
{
	ASSERT_VALID(this);
	ASSERT(nView >= 0);

	// remove the page from internal list
	m_views.RemoveAt(m_views.FindIndex(nView));
}

void CCFTabCtrlBar::SetActiveView(int nNewTab)
{
	ASSERT_VALID(this);
	ASSERT(nNewTab >= 0);

	if (nNewTab!=-1 && nNewTab!=m_nActiveTab)
	{
        TCB_ITEM *newMember=m_views.GetAt(m_views.FindIndex(nNewTab));
        TCB_ITEM *oldMember=NULL;
		
        if (m_nActiveTab!=-1)
        {
            oldMember=m_views.GetAt(m_views.FindIndex(m_nActiveTab));
            oldMember->pWnd->EnableWindow(FALSE);
            oldMember->pWnd->ShowWindow(SW_HIDE);
        }
        newMember->pWnd->EnableWindow(TRUE);
        newMember->pWnd->ShowWindow(SW_SHOW);
        newMember->pWnd->SetFocus();

        m_pActiveView = (CView *)newMember->pWnd;
		((CFrameWnd *)GetParent())->SetActiveView(m_pActiveView);

        m_nActiveTab = nNewTab;
		// select the tab (if tab programmatically changed)
		m_tabctrl.SetCurSel(m_nActiveTab);
    }
}

void CCFTabCtrlBar::SetActiveView(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	int nNewTab = 0;
	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		TCB_ITEM *pMember=m_views.GetAt(pos);
		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			//first hide old first view
            m_pActiveView->EnableWindow(FALSE);
            m_pActiveView->ShowWindow(SW_HIDE);
			
			// set new active view
			m_pActiveView = (CView*)pMember->pWnd;
			// enable, show, set focus to new view
			m_pActiveView->EnableWindow(TRUE);
			m_pActiveView->ShowWindow(SW_SHOW);
			m_pActiveView->SetFocus();
			
			((CFrameWnd *)GetParent())->SetActiveView(m_pActiveView);

	        m_nActiveTab = nNewTab;
			// select the tab
			m_tabctrl.SetCurSel(m_nActiveTab);

			break;
		}
		nNewTab++;
    }
}

CView* CCFTabCtrlBar::GetActiveView()
{
	return m_pActiveView;
}

CView* CCFTabCtrlBar::GetView(int nView)
{
	ASSERT_VALID(this);
	ASSERT(nView >= 0);

	if (nView!=-1)
	{
        TCB_ITEM *pMember=m_views.GetAt(m_views.FindIndex(nView));
		return (CView*)pMember->pWnd;
	}
	else
		return NULL;
}

CView* CCFTabCtrlBar::GetView(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		TCB_ITEM *pMember=m_views.GetAt(pos);
		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			return (CView*)pMember->pWnd;
		}
    }
	return NULL;
}