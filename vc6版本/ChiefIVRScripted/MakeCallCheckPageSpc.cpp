// MakeCallCheckPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "MakeCallCheckPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeCallCheckPageSpc property page

IMPLEMENT_DYNCREATE(CMakeCallCheckPageSpc, CPropertyPage)

CMakeCallCheckPageSpc::CMakeCallCheckPageSpc() : CPropertyPage(CMakeCallCheckPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CMakeCallCheckPageSpc)
	m_bAfterRings = FALSE;
	m_bNotAnswer = FALSE;
	m_nRings = 1;
	m_nTimes = 1;
	m_strPhone = _T("");
	m_strPrefix = _T("");
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;

	m_bConnect		= TRUE;
	m_bBusy			= FALSE;
	m_bNoDialTone	= FALSE;
	m_bNoAnswer		= FALSE;
	m_bNoRingBack	= FALSE;
	m_bError		= TRUE;
}

CMakeCallCheckPageSpc::~CMakeCallCheckPageSpc()
{
}

void CMakeCallCheckPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeCallCheckPageSpc)
	DDX_Control(pDX, IDC_LIST_EVENT, m_listEvent);
	DDX_Check(pDX, IDC_CHECK_AFTERRINGS, m_bAfterRings);
	DDX_Check(pDX, IDC_CHECK_NOTANSWER, m_bNotAnswer);
	DDX_Text(pDX, IDC_EDIT_RINGS, m_nRings);
	DDV_MinMaxUInt(pDX, m_nRings, 1, 10);
	DDX_Text(pDX, IDC_EDIT_TIME, m_nTimes);
	DDV_MinMaxUInt(pDX, m_nTimes, 1, 120);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_strPhone);
	DDX_Text(pDX, IDC_EDIT_PREFIX, m_strPrefix);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMakeCallCheckPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CMakeCallCheckPageSpc)
	ON_BN_CLICKED(IDC_BUTTON_PHONE, OnButtonPhone)
	ON_BN_CLICKED(IDC_CHECK_NOTANSWER, OnCheckNotanswer)
	ON_BN_CLICKED(IDC_CHECK_AFTERRINGS, OnCheckAfterrings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeCallCheckPageSpc message handlers

void CMakeCallCheckPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;

	UpdateData(TRUE);

	m_pNodeObj->m_bNotAnswer	= m_bNotAnswer ;
	m_pNodeObj->m_nTimes		= m_nTimes;
	m_pNodeObj->m_bAfterRings	= m_bAfterRings  ;
	m_pNodeObj->m_nRings		= m_nRings ;
	m_pNodeObj->m_strPhone		= m_strPhone;
	m_pNodeObj->m_strPrefix		= m_strPrefix;



	EditNextNode(7, m_listEvent.GetCheck(0) );
	EditNextNode(3, m_listEvent.GetCheck(1) );
	EditNextNode(4, m_listEvent.GetCheck(2) );
	EditNextNode(5, m_listEvent.GetCheck(3) );
	EditNextNode(6, m_listEvent.GetCheck(4) );
	EditNextNode(2, m_listEvent.GetCheck(5) );

	m_pNodeObj->UpDate();

	CPropertyPage::OnOK();
}

BOOL CMakeCallCheckPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_listEvent.SetExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );
	
	m_listEvent.InsertItem(0,"Connect");
	m_listEvent.InsertItem(1,"busy");
	m_listEvent.InsertItem(2,"No dial Tone");
	m_listEvent.InsertItem(3,"No Answer");
	m_listEvent.InsertItem(4,"No RingBack");
	m_listEvent.InsertItem(5,"Error");


	if( NULL == m_pNodeObj ) return TRUE;

	CNextNodeList *pNextObj = &(m_pNodeObj->m_NextNodeList);
	POSITION pos = pNextObj->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNextObj->GetNext(pos);
		
		switch( pObj->m_nType )
		{
		case NEXTTYPE_ERROR:
			m_listEvent.SetCheck(5,TRUE);
			break;
		case NEXTTYPE_CONNECT:
			m_listEvent.SetCheck(0,TRUE);
			break;
		case NEXTTYPE_BUSY:
			m_listEvent.SetCheck(1,TRUE);
			break;
		case NEXTTYPE_NO_DIALTONE:
			m_listEvent.SetCheck(2,TRUE);
			break;
		case NEXTTYPE_NO_ANSWER:
			m_listEvent.SetCheck(3,TRUE);
			break;
		case NEXTTYPE_NO_RINGBACK:
			m_listEvent.SetCheck(4,TRUE);
			break;
		default:
			break;


		}
		
	}
	EnableMyWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMakeCallCheckPageSpc::InitPage( CDialCheckObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	
	m_bNotAnswer	= m_pNodeObj->m_bNotAnswer;
	m_nTimes		= m_pNodeObj->m_nTimes;
	m_bAfterRings	= m_pNodeObj->m_bAfterRings;
	m_nRings		= m_pNodeObj->m_nRings;
	m_strPhone		= m_pNodeObj->m_strPhone;
	m_strPrefix		= m_pNodeObj->m_strPrefix;


	m_bConnect		= m_pNodeObj->m_bConnect;
	m_bBusy			= m_pNodeObj->m_bBusy;
	m_bNoDialTone	= m_pNodeObj->m_bNoDialTone;
	m_bNoAnswer		= m_pNodeObj->m_bNoAnswer;
	m_bNoRingBack	= m_pNodeObj->m_bNoRingBack;
	m_bError		= m_pNodeObj->m_bError;



	return;
}

void CMakeCallCheckPageSpc::EditNextNode(int nType, BOOL bEditType)
{

	CNextNode * pObj  = m_pNodeObj->FindNextNodeObj( nType);

	switch( nType )
	{
	case NEXTTYPE_ERROR:
		if( bEditType )//Ìí¼Ó
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("Error",_T(""),DATATYPE_INTEGER,nType,1);
			}
			m_pNodeObj->m_bError = TRUE;
		}
		else//É¾³ý
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
			m_pNodeObj->m_bError = FALSE;
		}
		break;
	case NEXTTYPE_CONNECT:
		if( bEditType )//Ìí¼Ó
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("Connect",_T(""),DATATYPE_INTEGER,nType,1);	
			}
			m_pNodeObj->m_bConnect = TRUE;
		}
		else//É¾³ý
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
			m_pNodeObj->m_bConnect = FALSE;
		}
		break;
	case NEXTTYPE_BUSY:
		if( bEditType )//Ìí¼Ó
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("busy",_T(""),DATATYPE_INTEGER,nType,1);
			}
			m_pNodeObj->m_bBusy = TRUE;
		}
		else//É¾³ý
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
			m_pNodeObj->m_bBusy = FALSE;
		}
		break;
	case NEXTTYPE_NO_DIALTONE:
		if( bEditType )//Ìí¼Ó
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("No Dial Tone",_T(""),DATATYPE_INTEGER,nType,1);
			}
			m_pNodeObj->m_bNoDialTone = TRUE;
		}
		else//É¾³ý
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
			m_pNodeObj->m_bNoDialTone = FALSE;
		}
		break;
	case NEXTTYPE_NO_ANSWER:
		if( bEditType )//Ìí¼Ó
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("No Answer",_T(""),DATATYPE_INTEGER,nType,1);
			}
			m_pNodeObj->m_bNoAnswer = TRUE;
		}
		else//É¾³ý
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
			m_pNodeObj->m_bNoAnswer = FALSE;
		}
		break;
	case NEXTTYPE_NO_RINGBACK:
		if( bEditType )//Ìí¼Ó
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("No RingBack",_T(""),DATATYPE_INTEGER,nType,1);
			}
			m_pNodeObj->m_bNoRingBack = TRUE;
		}
		else//É¾³ý
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
			m_pNodeObj->m_bNoRingBack = FALSE;
		}
		break;
	default:
		break;


	}
	


	return;
}




void CMakeCallCheckPageSpc::OnButtonPhone() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
		m_strPhone =  str;
		UpdateData(FALSE);
	}

	return;
	
}

void CMakeCallCheckPageSpc::OnCheckNotanswer() 
{
	UpdateData(TRUE);
	EnableMyWindow();
	
}

void CMakeCallCheckPageSpc::OnCheckAfterrings() 
{
	UpdateData(TRUE);
	EnableMyWindow();

	
}

void CMakeCallCheckPageSpc::EnableMyWindow()
{
	CWnd * phWnd = GetDlgItem(IDC_EDIT_TIME);
	if( NULL != phWnd )
		phWnd->EnableWindow( m_bNotAnswer);
	phWnd = GetDlgItem(IDC_EDIT_RINGS);
	if( NULL != phWnd )
		phWnd->EnableWindow( m_bAfterRings);
}



