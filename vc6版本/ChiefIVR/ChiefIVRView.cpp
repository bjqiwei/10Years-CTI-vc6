// ChiefIVRView.cpp : implementation of the CChiefIVRView class
//

#include "stdafx.h"
#include "ChiefIVR.h"


#include "ChiefIVRView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRView

IMPLEMENT_DYNCREATE(CChiefIVRView, CView)

BEGIN_MESSAGE_MAP(CChiefIVRView, CView)
	//{{AFX_MSG_MAP(CChiefIVRView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRView construction/destruction

CChiefIVRView::CChiefIVRView()
{
	// TODO: add construction code here

}

CChiefIVRView::~CChiefIVRView()
{
}

BOOL CChiefIVRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRView drawing

void CChiefIVRView::OnDraw(CDC* pDC)
{
	CChiefIVRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRView printing

BOOL CChiefIVRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CChiefIVRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CChiefIVRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRView diagnostics

#ifdef _DEBUG
void CChiefIVRView::AssertValid() const
{
	CView::AssertValid();
}

void CChiefIVRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CChiefIVRDoc* CChiefIVRView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChiefIVRDoc)));
	return (CChiefIVRDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRView message handlers
