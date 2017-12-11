// VoiceFileFormView.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "VoiceFileFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVoiceFileFormView

IMPLEMENT_DYNCREATE(CVoiceFileFormView, CFormView)

CVoiceFileFormView::CVoiceFileFormView()
	: CFormView(CVoiceFileFormView::IDD)
{
	//{{AFX_DATA_INIT(CVoiceFileFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_InitialUpdate=0;
	m_cy=0;
	m_cx=0;
}

CVoiceFileFormView::~CVoiceFileFormView()
{
}

void CVoiceFileFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVoiceFileFormView)
	DDX_Control(pDX, IDC_VOXFILE_LIST, m_voxfileList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVoiceFileFormView, CFormView)
	//{{AFX_MSG_MAP(CVoiceFileFormView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVoiceFileFormView diagnostics

#ifdef _DEBUG
void CVoiceFileFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CVoiceFileFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVoiceFileFormView message handlers

void CVoiceFileFormView::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{

		m_voxfileList.MoveWindow(0,0,m_cx,m_cy);

	}
	
}

void CVoiceFileFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;
		m_voxfileList.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
		m_voxfileList.MoveWindow(0,0,m_cx,m_cy);
	}
	
}
