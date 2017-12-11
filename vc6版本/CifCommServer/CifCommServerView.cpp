// CifCommServerView.cpp : implementation of the CCifCommServerView class
//

#include "stdafx.h"
#include "CifCommServer.h"

#include "CifCommServerDoc.h"
#include "CifCommServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerView

IMPLEMENT_DYNCREATE(CCifCommServerView, CView)

BEGIN_MESSAGE_MAP(CCifCommServerView, CView)
	//{{AFX_MSG_MAP(CCifCommServerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerView construction/destruction

CCifCommServerView::CCifCommServerView()
{
	// TODO: add construction code here

}

CCifCommServerView::~CCifCommServerView()
{
}

BOOL CCifCommServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerView drawing

void CCifCommServerView::OnDraw(CDC* pDC)
{
	CCifCommServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerView diagnostics

#ifdef _DEBUG
void CCifCommServerView::AssertValid() const
{
	CView::AssertValid();
}

void CCifCommServerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCifCommServerDoc* CCifCommServerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCifCommServerDoc)));
	return (CCifCommServerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerView message handlers
