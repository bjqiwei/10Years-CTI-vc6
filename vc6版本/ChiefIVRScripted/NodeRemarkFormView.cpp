// NodeRemarkFormView.cpp : implementation file
//

#include "stdafx.h"

#include "chiefivrscripted.h"
#include "NodeRemarkFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNodeRemarkFormView *g_pFileRemark = NULL;

/////////////////////////////////////////////////////////////////////////////
// CNodeRemarkFormView

IMPLEMENT_DYNCREATE(CNodeRemarkFormView, CFormView)

CNodeRemarkFormView::CNodeRemarkFormView()
	: CFormView(CNodeRemarkFormView::IDD)
{
	//{{AFX_DATA_INIT(CNodeRemarkFormView)
	//}}AFX_DATA_INIT
	g_pFileRemark = this;
	m_InitialUpdate = 0;
	m_pDrawView = NULL;
}

CNodeRemarkFormView::~CNodeRemarkFormView()
{
}

void CNodeRemarkFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeRemarkFormView)
	DDX_Control(pDX, IDC_EDIT_RMARK, m_edtRemark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNodeRemarkFormView, CFormView)
	//{{AFX_MSG_MAP(CNodeRemarkFormView)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_RMARK, OnChangeEditRmark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeRemarkFormView diagnostics

#ifdef _DEBUG
void CNodeRemarkFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNodeRemarkFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNodeRemarkFormView message handlers

void CNodeRemarkFormView::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{

		m_edtRemark.MoveWindow(0,0,m_cx,m_cy);

	}
	
}

void CNodeRemarkFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;
		m_edtRemark.MoveWindow(0,0,m_cx,m_cy);
	}
	
}

void CNodeRemarkFormView::OnChangeEditRmark() 
{
	CString str="";

	m_edtRemark.GetWindowText(str);
	m_pDrawView->SaveFileRemakToDoc(str);

	return;
}
