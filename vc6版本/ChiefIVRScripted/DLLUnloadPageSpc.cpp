// DLLUnloadPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "DLLUnloadPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLLUnloadPageSpc property page

IMPLEMENT_DYNCREATE(CDLLUnloadPageSpc, CPropertyPage)

CDLLUnloadPageSpc::CDLLUnloadPageSpc() : CPropertyPage(CDLLUnloadPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CDLLUnloadPageSpc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc	= NULL;
	m_strDLLHandle = _T("");
}

CDLLUnloadPageSpc::~CDLLUnloadPageSpc()
{
}

void CDLLUnloadPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLLUnloadPageSpc)
	DDX_Control(pDX, IDC_COMBO_DLLHANDLE, m_cobDLLHandle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLLUnloadPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CDLLUnloadPageSpc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLLUnloadPageSpc message handlers

void CDLLUnloadPageSpc::InitPage(CDLLUnLoadObj *pNodeObj, CDrawDoc *pDoc)
{
	if( NULL == pNodeObj || NULL == pDoc ) return;
	m_pNodeObj = pNodeObj;
	m_pDoc = pDoc;



	m_strDLLHandle = m_pNodeObj->m_strDLL;

	return;

}

void CDLLUnloadPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;
	UpdateData(TRUE);

	if( m_cobDLLHandle.GetCurSel( ) > -1 ) 
			m_cobDLLHandle.GetLBText( m_cobDLLHandle.GetCurSel( ), m_pNodeObj->m_strDLL );
	
	m_pNodeObj->m_nDLLKey = m_pNodeObj->GetVarKey(m_pNodeObj->m_strDLL);
	if( "#" == m_pNodeObj->m_strDLL.Left(1) )
	{
		m_pNodeObj->m_nHandleType = 1;
	}
	else
	{
		m_pNodeObj->m_nHandleType = 0;
	}
	
	CPropertyPage::OnOK();
}

BOOL CDLLUnloadPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return TRUE;
	
	

	CString str(_T(""));
	POSITION pos = m_pDoc->m_DLLVariableList.GetHeadPosition() ;
	while( pos )
	{
		
		CDLLVariable * pVar =m_pDoc->m_DLLVariableList.GetNext(pos) ;
		if( pVar != NULL )
		{
			if( pVar->m_nKey > 100 )
			{
				str.Format("@%d:%s", pVar->m_nKey, pVar->m_strName );
				m_cobDLLHandle.AddString( str );
			}
		}
		
	}

	m_cobDLLHandle.AddString( "#59:SysDLLHandle1");
	m_cobDLLHandle.AddString( "#60:SysDLLHandle2");
	m_cobDLLHandle.AddString( "#61:SysDLLHandle3");
	m_cobDLLHandle.AddString( "#62:SysDLLHandle4");
	m_cobDLLHandle.AddString( "#63:SysDLLHandle5");
	m_cobDLLHandle.AddString( "#64:SysDLLHandle6");
	m_cobDLLHandle.AddString( "#65:SysDLLHandle7");
	m_cobDLLHandle.AddString( "#66:SysDLLHandle8");


	m_cobDLLHandle.SelectString(-1, m_strDLLHandle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
