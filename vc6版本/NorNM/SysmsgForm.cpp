// SysmsgForm.cpp : implementation file
//

#include "stdafx.h"
#include "ChiefIVR.h"
#include "SysmsgForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CSysmsgForm *g_pSysmsgForm = NULL;
extern CChiefIVRApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSysmsgForm

IMPLEMENT_DYNCREATE(CSysmsgForm, CFormView)

CSysmsgForm::CSysmsgForm()
	: CFormView(CSysmsgForm::IDD)
{
	//{{AFX_DATA_INIT(CSysmsgForm)
	//}}AFX_DATA_INIT
	m_InitialUpdate = 0;
	m_cy = 0;
	m_cx = 0;
	g_pSysmsgForm = this;

}

CSysmsgForm::~CSysmsgForm()
{

}

void CSysmsgForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysmsgForm)
	DDX_Control(pDX, IDC_LIST_SYSTEM, m_listSystem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysmsgForm, CFormView)
	//{{AFX_MSG_MAP(CSysmsgForm)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_LBN_SETFOCUS(IDC_LIST_SYSTEM, OnSetfocusListSystem)
	ON_LBN_SELCHANGE(IDC_LIST_SYSTEM, OnSelchangeListSystem)
	ON_LBN_SELCANCEL(IDC_LIST_SYSTEM, OnSelcancelListSystem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysmsgForm diagnostics

#ifdef _DEBUG
void CSysmsgForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CSysmsgForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSysmsgForm message handlers

void CSysmsgForm::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{

		m_listSystem.MoveWindow(0,20,m_cx,m_cy-20);

	}
	
}

void CSysmsgForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;
		m_listSystem.MoveWindow(0,20,m_cx,m_cy-20);
		m_listSystem.SetHorizontalExtent(2000);
	}
	
}



void CSysmsgForm::ShowSysMsg(int nChnl, CString strMSG)
{
	strMSG = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S") + strMSG;
	int n_line = m_listSystem.GetCount();
	if (n_line > 200)
	{
		m_listSystem.InsertString(n_line, strMSG );
		m_listSystem.SetCurSel(n_line);
		m_listSystem.DeleteString(0);
		m_listSystem.ResetContent();
	}
	else
	{
		m_listSystem.InsertString(n_line, strMSG);
		m_listSystem.SetCurSel(n_line);
	}

}

void CSysmsgForm::ShowSysMsg(CString strMSG)
{
	strMSG = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S") + "  " + strMSG;
	int n_line = m_listSystem.GetCount();
	if (n_line > 200)
	{
		m_listSystem.InsertString(n_line, strMSG );
		m_listSystem.SetCurSel(n_line);
		m_listSystem.DeleteString(0);
		m_listSystem.ResetContent();
	}
	else
	{
		m_listSystem.InsertString(n_line, strMSG);
		m_listSystem.SetCurSel(n_line);
	}

	return;
}


void CSysmsgForm::OnClose() 
{
	
	CFormView::OnClose();
}

void CSysmsgForm::OnSetfocusListSystem() 
{

	return;
}

void CSysmsgForm::OnSelchangeListSystem() 
{

	return;
	
}


void CSysmsgForm::OnSelcancelListSystem() 
{
	// TODO: Add your control notification handler code here
	int i=0;
}


