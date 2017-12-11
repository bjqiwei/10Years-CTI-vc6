// DTMFMenuPageSpc.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "DTMFMenuPageSpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDTMFMenuPageSpc property page

IMPLEMENT_DYNCREATE(CDTMFMenuPageSpc, CPropertyPage)

CDTMFMenuPageSpc::CDTMFMenuPageSpc() : CPropertyPage(CDTMFMenuPageSpc::IDD)
{
	//{{AFX_DATA_INIT(CDTMFMenuPageSpc)
	m_bClearDTMF = FALSE;
	m_nTimeOut = 0;
	//}}AFX_DATA_INIT
	m_pNodeObj = NULL;
	m_pDoc = NULL;
}

CDTMFMenuPageSpc::~CDTMFMenuPageSpc()
{
}

void CDTMFMenuPageSpc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDTMFMenuPageSpc)
	DDX_Control(pDX, IDC_LIST_EVENT, m_listEvent);
	DDX_Check(pDX, IDC_CHECK_CLEARDTMF, m_bClearDTMF);
	DDX_Text(pDX, IDC_DTMFMNU_TIMEOUT, m_nTimeOut);
	DDV_MinMaxInt(pDX, m_nTimeOut, 0, 2000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDTMFMenuPageSpc, CPropertyPage)
	//{{AFX_MSG_MAP(CDTMFMenuPageSpc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDTMFMenuPageSpc message handlers

void CDTMFMenuPageSpc::OnOK() 
{
	if( NULL == m_pNodeObj || NULL == m_pDoc ) return;

	UpdateData(TRUE);

	m_pNodeObj->m_bClearDTMF = m_bClearDTMF;
	m_pNodeObj->m_nTimeOut = m_nTimeOut;
	EditNextNode(60, m_listEvent.GetCheck(0) );
	EditNextNode(61, m_listEvent.GetCheck(1) );
	EditNextNode(62, m_listEvent.GetCheck(2) );
	EditNextNode(63, m_listEvent.GetCheck(3) );
	EditNextNode(64, m_listEvent.GetCheck(4) );
	EditNextNode(65, m_listEvent.GetCheck(5) );
	EditNextNode(66, m_listEvent.GetCheck(6) );
	EditNextNode(67, m_listEvent.GetCheck(7) );
	EditNextNode(68, m_listEvent.GetCheck(8) );
	EditNextNode(69, m_listEvent.GetCheck(9) );
	EditNextNode(70, m_listEvent.GetCheck(10) );
	EditNextNode(71, m_listEvent.GetCheck(11) );
	EditNextNode(72, m_listEvent.GetCheck(12) );
	EditNextNode(2, m_listEvent.GetCheck(13) );


	m_pNodeObj->UpDate();
	
	CPropertyPage::OnOK();
}

BOOL CDTMFMenuPageSpc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_listEvent.SetExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );
	
	m_listEvent.InsertItem(0,"DTMF 0");
	m_listEvent.InsertItem(1,"DTMF 1");
	m_listEvent.InsertItem(2,"DTMF 2");
	m_listEvent.InsertItem(3,"DTMF 3");
	m_listEvent.InsertItem(4,"DTMF 4");
	m_listEvent.InsertItem(5,"DTMF 5");
	m_listEvent.InsertItem(6,"DTMF 6");
	m_listEvent.InsertItem(7,"DTMF 7");
	m_listEvent.InsertItem(8,"DTMF 8");
	m_listEvent.InsertItem(9,"DTMF 9");
	m_listEvent.InsertItem(10,"DTMF #");
	m_listEvent.InsertItem(11,"DTMF *");
	m_listEvent.InsertItem(12,"TimeOut");
	m_listEvent.InsertItem(13,"Error");


	if( NULL == m_pNodeObj ) return TRUE;

	CNextNodeList *pNextObj = &(m_pNodeObj->m_NextNodeList);
	POSITION pos = pNextObj->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNextObj->GetNext(pos);
		
		switch( pObj->m_nType )
		{
		case 60:
			m_listEvent.SetCheck(0,TRUE);
			break;
		case 61:
			m_listEvent.SetCheck(1,TRUE);
			break;
		case 62:
			m_listEvent.SetCheck(2,TRUE);
			break;
		case 63:
			m_listEvent.SetCheck(3,TRUE);
			break;
		case 64:
			m_listEvent.SetCheck(4,TRUE);
			break;
		case 65:
			m_listEvent.SetCheck(5,TRUE);
			break;
		case 66:
			m_listEvent.SetCheck(6,TRUE);
			break;
		case 67:
			m_listEvent.SetCheck(7,TRUE);
			break;
		case 68:
			m_listEvent.SetCheck(8,TRUE);
			break;
		case 69:
			m_listEvent.SetCheck(9,TRUE);
			break;
		case 70:
			m_listEvent.SetCheck(10,TRUE);
			break;
		case 71:
			m_listEvent.SetCheck(11,TRUE);
			break;
		case 72:
			m_listEvent.SetCheck(12,TRUE);
			break;
		case 2:
			m_listEvent.SetCheck(13,TRUE);
			break;
		default:
			break;


		}
		
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDTMFMenuPageSpc::EditNextNode(int nType, BOOL bEditType)
{
	CNextNode * pObj  = m_pNodeObj->FindNextNodeObj( nType);

	switch( nType )
	{
	case NEXTTYPE_DTMF_0:
		if( bEditType )//添加
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 0",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_1:
		if( bEditType )//添加摘机
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 1",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_2:
		if( bEditType )//添加挂机
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 2",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_3:
		if( bEditType )//添加拍叉簧
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 3",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_4:
		if( bEditType )//添加收到按键
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 4",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_5:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 5",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_6:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 6",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_7:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 7",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_8:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 8",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
		

	case NEXTTYPE_DTMF_9:
		if( bEditType )//
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF 9",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_10:
		if( bEditType )//
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF #",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;		
	case NEXTTYPE_DTMF_11:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("DTMF *",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_DTMF_TIMEOUT:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("TimeOut",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	case NEXTTYPE_ERROR:
		if( bEditType )//添加收到传真信号
		{
			if( NULL == pObj )
			{
				m_pNodeObj->AddNextNodeObj("Error",_T(""),DATATYPE_INTEGER,nType,1);

			}
		}
		else//删除
		{
			if( NULL != pObj )
			{
				m_pNodeObj->RemoveNextNodeObj(pObj);
			}
		}
		break;
	default:
		break;


	}
	


	return;
}

void CDTMFMenuPageSpc::InitPage(CDTMFMenuObj *pObj, CDrawDoc *pDoc)
{
	if( NULL == pObj || NULL == pDoc ) return;
	m_pNodeObj = pObj;
	m_pDoc = pDoc;

	m_bClearDTMF = m_pNodeObj->m_bClearDTMF;
	m_nTimeOut = m_pNodeObj->m_nTimeOut;

	return;
}
