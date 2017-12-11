// NodeObj.cpp: implementation of the CNodeObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodeObj.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDBConnVar, CObject, 0)
CDBConnVar::CDBConnVar()
{
	m_strName		= "";
	m_nVarDataType	= 0 ;
	m_nKey			= 0;
	m_strConn		= "";
	
	
}

CDBConnVar::CDBConnVar( const CDBConnVar &s )
{
	
	m_strName		= s.m_strName ;
	m_nVarDataType	= s.m_nVarDataType ;
	m_nKey			= s.m_nKey;
	m_strConn		= s.m_strConn;
}


CDBConnVar & CDBConnVar::operator =( const CDBConnVar &s )
{
	
	if(this == &s)
		return *this;
	m_strName		= s.m_strName ;
	m_nVarDataType	= s.m_nVarDataType ;
	m_nKey			= s.m_nKey;
	m_strConn		= s.m_strConn;


	return *this;
}
CDBConnVar::~CDBConnVar()
{
	
}
void CDBConnVar::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	if (ar.IsStoring())//保存数据
	{
		ar << m_strName;
		ar << m_nVarDataType;
		ar << m_nKey;
		ar << m_strConn;
		

	}
	else//读数据
	{
		ar >> m_strName;
		ar >> m_nVarDataType;
		ar >> m_nKey;
		ar >> m_strConn;
		
	}

}




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL(CNodeObj, CObject, 0)

CNodeObj::CNodeObj()
{

	m_width			= 96;
	m_high			= 25;
	m_rect.left		= 0;
	m_rect.top		= 0;
	m_rect.right	= m_width;
	m_rect.bottom   = m_high;
	m_bSelected     = TRUE;
	m_hIcon			= NULL;
	m_nType			= 0;
	m_strCaption = "";

	m_strControlMsg = "";
	m_strDescription = "";

	m_MoveRect = m_rect;
	m_nKey = 0;

	m_nClassType = 0;
	m_nBlockKey	= 0;

}
CNodeObj::CNodeObj( CPoint point )
{
	m_width			= 96;
	m_high			= 25;
	
	m_rect.left     = point.x ;
	m_rect.top      = point.y ;
	m_rect.right	= m_rect.left + m_width;
	m_rect.bottom   = m_rect.top  + m_high;

	m_bSelected     = TRUE;
	m_hIcon			= NULL;
	m_nType			= 0;

	m_strCaption = "";
	m_strControlMsg = "";
	m_strDescription = "";


	m_MoveRect = m_rect;
	m_nKey = 0;

	m_nClassType = 0;
	m_nBlockKey	= 0;
}

//该函数返回为-1表示该字符串不是变量
inline int CNodeObj::GetVarKey(CString strVar)
{
	if( "" == strVar ) 
	{
		return -1;
	}
	strVar.Replace("|",NULL);
	strVar.TrimLeft();
	if( "@" != strVar.Left(1) && "$" != strVar.Left(1) && "#" != strVar.Left(1))
	{
		return -1;
	}
	
	int n = strVar.Find(':');
	if(  n < 0 )
	{
		return -1;
	}

	int nVarKey = atoi( strVar.Mid(1,n) );
	return nVarKey;

	
}
CNodeObj::CNodeObj( const CNodeObj &s )
{
	Remove();
	m_width			= s.m_width;
	m_high			= s.m_high;
	m_rect			= s.m_rect;
	m_bSelected     = s.m_bSelected;
	m_hIcon			= s.m_hIcon;
	m_nType			= s.m_nType;
	m_strCaption	= s.m_strCaption;
	m_MoveRect		= s.m_MoveRect;
	m_nKey			= s.m_nKey;
	m_nIcoID        = s.m_nIcoID;
	
	m_strControlMsg = s.m_strControlMsg;
	m_strDescription = s.m_strDescription;
	m_nClassType = s.m_nClassType;
	m_nBlockKey	=  s.m_nBlockKey;

	POSITION pos = s.m_NextNodeList.GetHeadPosition();
	while( pos )
	{
		CNextNode *pObj = s.m_NextNodeList.GetNext(pos);
		if( pObj != NULL )
		{
			CNextNode *pNewObj = new CNextNode;
			*pNewObj = *pObj;
			m_NextNodeList.AddTail( pNewObj );
		}
	}

	pos = s.m_NodeDataList.GetHeadPosition();
	while( pos )
	{
		CNodeData *pObj = s.m_NodeDataList.GetNext(pos);
		if( pObj != NULL )
		{
			CNodeData *pNewObj = new CNodeData;
			*pNewObj = *pObj;
			m_NodeDataList.AddTail( pNewObj );
		}
	}


}
CNodeObj & CNodeObj::operator =( const CNodeObj &s )
{
	if(this == &s)
		return *this;

	Remove();

	m_width			= s.m_width;
	m_high			= s.m_high;
	m_rect			= s.m_rect;
	m_bSelected     = s.m_bSelected;
	m_hIcon			= s.m_hIcon;
	m_nType			= s.m_nType;
	m_strCaption	= s.m_strCaption;
	m_MoveRect		= s.m_MoveRect;
	m_nKey			= s.m_nKey;
	m_nIcoID        = s.m_nIcoID;
	
	m_strControlMsg = s.m_strControlMsg;
	m_strDescription = s.m_strDescription;
	m_nClassType = s.m_nClassType;
	m_nBlockKey	=  s.m_nBlockKey;

	POSITION pos = s.m_NextNodeList.GetHeadPosition();
	while( pos )
	{
		CNextNode *pObj = s.m_NextNodeList.GetNext(pos);
		if( pObj != NULL )
		{
			CNextNode *pNewObj = new CNextNode;
			*pNewObj = *pObj;
			m_NextNodeList.AddTail( pNewObj );
		}
	}

	pos = s.m_NodeDataList.GetHeadPosition();
	while( pos )
	{
		CNodeData *pObj = s.m_NodeDataList.GetNext(pos);
		if( pObj != NULL )
		{
			CNodeData *pNewObj = new CNodeData;
			*pNewObj = *pObj;
			m_NodeDataList.AddTail( pNewObj );
		}
	}

	return *this;
}


CNodeObj::~CNodeObj()
{
	Remove();
}
void CNodeObj::OnDraw(CDC *pDC)
{
	if( NODETYPE_ENDCALL_BLOCK == m_nType ) return;

	OnDrawBaseRect( pDC );
	OnDrawIco( pDC, m_hIcon);
	OnDrawCaption( pDC);

	CNextNodeList *pNNodeLst = &(m_NextNodeList);
   	POSITION pos = pNNodeLst->GetHeadPosition();
	while (pos != NULL)
	{
		CNextNode* pObj = pNNodeLst->GetNext(pos);
		pObj->m_LineObj.m_BeginRect = m_rect ;
		pObj->m_NodeObjRect = m_rect ;
		pObj->OnDraw( pDC );
		
		
	}
	



	return;
}



void CNodeObj::MoveTo( CPoint point )
{
	m_rect.left = point.x ;
	m_rect.top  = point.y ;


	m_rect.right	= point.x + m_width;
	m_rect.bottom   = point.y + m_high + 3 + ( 13 + 2)  * m_NextNodeList.GetCount();//13是子结点的高度

	CNextNodeList *pNNodeLst = &(m_NextNodeList);
   	POSITION pos = pNNodeLst->GetHeadPosition();
	while (pos != NULL)
	{
		CNextNode* pObj = pNNodeLst->GetNext(pos);
		pObj->MoveTo( point,m_rect );
		
	}
	
	m_MoveRect = m_rect;

	return;
}



void CNodeObj::OnDrawBaseRect(CDC *pDC)
{
	CRect rect;
	rect = m_rect;
	pDC->Draw3dRect( &rect,RGB(212, 208, 200) , RGB(64,64,64));

	

	rect.left   += 1;
	rect.top     +=1;
	rect.bottom  -=1;
	rect.right -=1;

	pDC->Draw3dRect( &rect, RGB(255, 255, 255) , RGB(128,128,128) );


	rect.left   += 1;
	rect.top     +=1;
	rect.bottom  -=1;
	rect.right -=1;

	pDC->FillSolidRect( &rect, RGB(212, 208, 200) );
	




}

void CNodeObj::OnDrawIco(CDC *pDC, HICON hIcon )
{
	CRect rect;
	rect = m_rect;
	rect.left    += 3;
	rect.top     +=5;
	rect.right  = rect.left + 16;
	rect.bottom  = rect.top + 16;


	pDC->Draw3dRect( &rect, RGB(128,128,128), RGB(255, 255, 255)  );

	pDC->SetMapMode(MM_TEXT);
	pDC->DrawIcon( rect.left, rect.top, hIcon );

}

void CNodeObj::OnDrawCaption( CDC *pDC )
{
	CRect rect;
	rect = m_rect;


	rect.left   += 21;
	rect.top    += 5;
	rect.right  = rect.left + 72;
	rect.bottom = rect.top + 16;

	LOGFONT lf; 
	memset(&lf,0,sizeof(LOGFONT)); 
	lf.lfHeight			= 14; //字体大小
	lf.lfWeight			= FW_BOLD; 
	lf.lfEscapement		= 0;    
	lf.lfOrientation	= 0;    
	lf.lfItalic			= false; 
	lf.lfUnderline		= false; 
	lf.lfStrikeOut		= false; 
	lf.lfCharSet		= ANSI_CHARSET; 
	lf.lfPitchAndFamily	= 20; //字体类别

	CFont font;
	font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&font);// 将字体选进DC

	if( m_bSelected )
		pDC->FillSolidRect( &rect, RGB(0, 0, 255) );
	else
		pDC->FillSolidRect( &rect, RGB(128, 128, 128) );

	pDC->SetTextColor( RGB(255, 255, 255) );

	pDC->SetBkMode( TRANSPARENT); 
	pDC->DrawText( m_strCaption,   &rect , DT_CENTER|DT_END_ELLIPSIS );

	pDC->SelectObject(pOldFont); // 回复旧字体-这是必须的
	pOldFont = NULL;
}

inline BOOL CNodeObj::Intersects(const CRect &rect)
{
	ASSERT_VALID(this);

	CRect fixed = m_rect;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	m_bSelected = !(rectT & fixed).IsRectEmpty();

	return m_bSelected;
}


inline void CNodeObj::SetBaseInfo(  LPCTSTR lpszCaption, HICON hIcon , int nType ,int nKey )
{
	m_hIcon			= hIcon;
	m_nType			= nType;
	m_strCaption = lpszCaption;
	
	m_nKey = nKey;

	return;

}

BOOL CNodeObj::AddNextNodeObj( LPCTSTR lpszName, LPCTSTR lpszData, int nDataType, int nType, int nIndex )
{
	CNextNode *pNewNextNode = new CNextNode;
	if( NULL == pNewNextNode )
		return FALSE;

	m_NextNodeList.AddTail( pNewNextNode );

	CPoint point;
	point.x = m_rect.left ;
	point.y = m_rect.top ;
	pNewNextNode->SetNodeInfo( point, lpszName, lpszData, nDataType, nType, m_NextNodeList.GetCount() );
	

	m_rect.right	= point.x + m_width;
	m_rect.bottom   = point.y + m_high + 1 + (pNewNextNode->GetHigh() + 2)  * m_NextNodeList.GetCount();

	m_MoveRect = m_rect;
	pNewNextNode->m_NodeObjRect = m_rect;
	pNewNextNode->m_LineObj.m_BeginRect = m_rect;
	return TRUE;
}

void CNodeObj::RemoveNextNodeObj( CNextNode *pObj )
{
	CNextNodeList *pNNodeLst = &(m_NextNodeList);
	POSITION pos = pNNodeLst->Find(pObj);
	if (pos != NULL)
	{
		pNNodeLst->RemoveAt(pos);
		delete pObj;
		pObj = NULL;
	}

	int nIndex = 1;
	CPoint point = CPoint( m_rect.left , m_rect.top );
	pos = pNNodeLst->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNNodeLst->GetNext(pos);
		if( pObj != NULL ) pObj->UpdateNodeInfo( point, nIndex );
		nIndex++;
	}


	m_rect.right	= point.x + m_width;
	m_rect.bottom   = point.y + m_high + 1 + ( 13 + 2)  * m_NextNodeList.GetCount();



	return;
}

void CNodeObj::Remove()
{
	while ( m_NextNodeList.GetHeadPosition() )
	{
		delete m_NextNodeList.RemoveHead();
	}

	while ( m_NodeDataList.GetHeadPosition() )
	{
		delete m_NodeDataList.RemoveHead();
	}


	return;
}



void CNodeObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);


	if (ar.IsStoring())//保存数据
	{
		ar << m_width;
		ar << m_high;
		ar << m_rect;
		ar << m_MoveRect;
		ar << m_strCaption;
		ar << m_nIcoID;
		ar << (WORD)m_bSelected;
		ar << m_nType;
		ar << m_nKey;
		ar << m_strDescription;
		ar << m_strControlMsg;
		ar << m_nClassType;
		ar << m_nBlockKey;
		m_NextNodeList.Serialize(ar);
		m_NodeDataList.Serialize(ar);


	}
	else//读数据
	{
		WORD wTemp;
		ar >> m_width;
		ar >> m_high;
		ar >> m_rect;
		ar >> m_MoveRect;
		ar >> m_strCaption;
		ar >> m_nIcoID;
		ar >> wTemp; m_bSelected = (BOOL)wTemp;
		ar >> m_nType;
		ar >> m_nKey;
		ar >> m_strDescription;
		ar >> m_strControlMsg;
		ar >> m_nClassType;
		ar >> m_nBlockKey;
		m_NextNodeList.Serialize(ar);
		m_NodeDataList.Serialize(ar);

	}
}

inline BOOL CNodeObj::HitTest(CPoint point)
{

	ASSERT_VALID(this);

	if (point.x >= m_rect.left && point.x < m_rect.right &&
		point.y >= m_rect.top && point.y < m_rect.bottom)
	{
		m_bSelected = TRUE;
		return TRUE;
	}
	else
	{
		m_bSelected = FALSE;
		return FALSE;
	}
}

long CNodeObj::GetHigh()
{
	return m_high;
}

long CNodeObj::GetWidth()
{
	return m_width;
}

CNextNode * CNodeObj::HitTestNextNode(CPoint point)
{
	ASSERT_VALID(this);
	CNextNodeList *pNNodeLst = &(m_NextNodeList);
   	POSITION pos = pNNodeLst->GetHeadPosition();
	while (pos != NULL)
	{
		CNextNode *pObj =  pNNodeLst->GetNext(pos);	
		if( pObj->HitTest( point ) )
		{

			return pObj;
		}
	}

	return NULL;
	

}



void CNodeObj::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;
	return;
}

inline CPoint CNodeObj::GetLinePoint()
{
	CPoint point;

	point.x = m_rect.left  ;
	point.y = m_rect.top + 8;

	return point;
}



void  CNodeObj::MoveLineObj( int nNodeObjKey , CRect EndRect )
{
	CNextNodeList *pNNodeLst = &(m_NextNodeList);
   	POSITION pos = pNNodeLst->GetHeadPosition();
	while (pos != NULL)
	{
		CNextNode *pObj = pNNodeLst->GetNext(pos);	
		if( NULL != pObj )
		{
			if( nNodeObjKey == pObj->m_nNodeObjKey )
			{
				pObj->m_LineObj.m_EndRect = EndRect;
			}

		}

	}
	
	return ;
}

void CNodeObj::RemoveNodeData()
{
	CNodeDataList *pDLst = &(m_NodeDataList);
	while ( pDLst->GetHeadPosition() )
	{
		delete pDLst->RemoveHead();
	}


	return;
}


inline int	CNodeObj::GetDataType( CString strDataType ) 
{
	strDataType.TrimRight();
	strDataType.TrimLeft();

	int nType = 0;

	if( strDataType =="Integer" )
		nType = DATATYPE_INTEGER;
	else if( strDataType =="Double" )
		nType = DATATYPE_FLOAT;
	else if( strDataType =="String" )
		nType = DATATYPE_STRING;
	else if( strDataType =="Date" )
		nType = DATATYPE_DATE;
	else if( strDataType =="Time" )
		nType = DATATYPE_TIME;
	else if( strDataType =="DateTime" )
		nType = DATATYPE_DATATIME;

	return nType;
}
inline CString	CNodeObj::GetDataTypeName( int nType ) 
{

	CString str="";
	switch(nType)
	{

	case DATATYPE_INTEGER:
		str="Integer";
		break;
	case DATATYPE_FLOAT:
		str="Double";
		break;
	case DATATYPE_STRING:
		str="String";
		break;
	case DATATYPE_DATE:
		str="Date";
		break;
	case DATATYPE_TIME:
		str="Time";
		break;
	case DATATYPE_DATATIME:
		str="DateTime";
		break;
	default:
		break;
	}

	return str;
}


//nType是指NextNodeObj的类型，类型定义见参考
CNextNode * CNodeObj::FindNextNodeObj(int nType)
{
	CNextNodeList *pNNodeLst = &(m_NextNodeList);
	POSITION pos = pNNodeLst->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNNodeLst->GetNext(pos);
		if( pObj != NULL )
		{
			if( nType == pObj->m_nType )
				return pObj;

		}

	}

	return NULL;
}

void CNodeObj::UpDate()
{
	CPoint point;
	point.x = m_rect.left ;
	point.y = m_rect.top  ;


	m_rect.bottom   = point.y  + m_high + 3 + (13 + 2)  * m_NextNodeList.GetCount();
	
	int nIndex = 1;
	POSITION pos = m_NextNodeList.GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =m_NextNodeList.GetNext(pos);
		if( pObj != NULL )
		{
			pObj->UpdateNodeInfo(point,nIndex);
			nIndex++;

		}

	}

	return;

}

CNextNode * CNodeObj::FindNextNodeObj(CString strName)
{
	CNextNodeList *pNNodeLst = &(m_NextNodeList);
	POSITION pos = pNNodeLst->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNNodeLst->GetNext(pos);
		if( pObj != NULL )
		{
			if( strName == pObj->m_strName  )
				return pObj;

		}

	}

	return NULL;
}

//取得节点对象的类型名
inline CString CNodeObj::GetNodeObjTypeName()
{
	CString str(_T(""));
	switch( m_nType )
	{
	case NODETYPE_WAITCALL:
		str = "WaitCall";
		break;
	case NODETYPE_START_PLAY:
		str = "Play";
		break;
	case NODETYPE_CHECK_DIAL:
		str = "MakeCall";
		break;
	case NODETYPE_RELEASECALL:
		str = "ReleaseCall";
		break;
	case NODETYPE_CALLHOLD:
		str = "CallHold";
		break;
	case NODETYPE_CHECK_TRANSFER:
		str = "Transfer";
		break;
	case NODETYPE_START_RECORD:
		str = "Record";
		break;
	case NODETYPE_SETVARIABLE:
		str = "SetVariable";
		break;
	case NODETYPE_COMPARE:
		str = "Compare";
		break;
	case NODETYPE_ENTER_CONFERENCE:
		str = "EnterConference";
		break;
	case NODETYPE_LEAVE_CONFERENCE:
		str = "LeaveConference";
		break;
	case NODETYPE_OPEN_DATABASE:
		str = "DBOpen";
		break;
	case NODETYPE_CLOSE_DATABASE:
		str = "DBClose";
		break;
	case NODETYPE_EXCUTE_SQL:
		str = "ExcuteSQL";
		break;
	case  NODETYPE_DATABASE_BROWSE:
		str = "DBBrowse";
		break;
	case  NODETYPE_DLL_LOAD:
		str = "DLLLoad";
		break;
	case  NODETYPE_DLL_UNLOAD:
		str = "DLLUnLoad";
		break;
	case  NODETYPE_DLL_CALL:
		str = "DLLCALL";
		break;
	case  NODETYPE_SEND_DTMFCODE:
		str = "DTMFSend";
		break;
	case  NODETYPE_RECIVE_DTMF:
		str = "DTMFRecive";
		break;
	case  NODETYPE_EVENT_CHECK:
		str = "EventCheck";
		break;
	case NODETYPE_DTMF_MENU:
		str = "DTMFMenu";
		break;
	case NODETYPE_CALLLOGREQUEST:
		str = "CallLogRequest";
		break;
	case NODETYPE_NODE_END:
		str = "End";
		break;
	case NODETYPE_SWITCH:
		str = "Switch";
		break;
	case NODETYPE_ANSWERCALL:
		str = "AnswerCall";
		break;
	case NODETYPE_FAXRECIVE:
		str = "FaxRecive";
		break;
	case NODETYPE_FAXSEND:
		str = "FaxSend";
		break;
	case NODETYPE_CONFERENCE_BUILD:
		str = "ConferenceBuild";
		break;
	case NODETYPE_CONFERENCE_DELETE:
		str = "ConferenceDelete";
		break;
	case NODETYPE_GET_CONFERENCE_MEMBER:
		str = "GetConferenceMember";
		break;
	case NODETYPE_CONFERENCE_REQUEST:
		str = "ConferenceCall";
		break;
	case NODETYPE_BLOCK:
		str = "Block";
		break;
	case NODETYPE_ENTER:
		str = "Enter";
		break;
	case NODETYPE_RETURN:
		str = "Return";
		break;
	case NODETYPE_STOREPRO :
		str = "StorePro";
		break;
	case NODETYPE_CHANGEVOICE :
		str = "ChangeVoice";
		break;
	case NODETYPE_EXPRESS :
		str = "Express";
		break;
	case NODETYPE_FUNCTION :
		str = "Function";
		break;
	case NODETYPE_GOTO:
		str = "GoTo";
		break;
	case NODETYPE_TRACE_INFO:
		str = "Trace";
		break;
	case NODETYPE_MOUDLE:
		str = "Moudle";
		break;
	case NODETYPE_CUSTOM :
		str = "Custom";
		break;
	case NODETYPE_CC_CALLAGENT :
		str = "CallAgent";
		break;
	case NODETYPE_CC_TRANSFER :
		str = "CCTransfer";
		break;
	default:
		str = "";
		break;
	}

	return str;
}

void CNodeObj::ChangeLineColor( CPoint point )
{
	
	CNextNodeList *pNNodeLst = &(m_NextNodeList);
   	POSITION pos = pNNodeLst->GetHeadPosition();
	while (pos != NULL)
	{
		CNextNode *pObj =  pNNodeLst->GetNext(pos);	
		if( NULL == pObj ) continue;

		if( pObj->HitTest( point ) )
		{
			//选中
			if( 0 != pObj->m_nNodeObjKey )
			{
				pObj->m_LineObj.m_crLineColor = RGB(255, 0, 0);//红色
				

			}
			
		}
		else
		{
			if( 0 != pObj->m_nNodeObjKey )
			{
				pObj->m_LineObj.m_crLineColor = RGB(0, 0, 255);
				
			}
			
		}
	}

	return ;
	
}

//断掉自己的Next与指定Key值等于nKey的CNodeObj的连接，参数nKey为该指向结点的Key
void CNodeObj::DisConnectToNode(int nKey)
{
	CNextNodeList *pNNodeLst = &(m_NextNodeList);
	POSITION pos = pNNodeLst->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNNodeLst->GetNext(pos);
		if( pObj != NULL )
		{
			if( nKey == pObj->m_nNodeObjKey )
				pObj->DisConnectToNodeObj();

		}

	}
	

	return ;
}




//判断该结点是否连接到了key = nThisKey的结点对象
BOOL CNodeObj::CheckConnectToThis(int nThisKey)
{
	POSITION pos = m_NextNodeList.GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =m_NextNodeList.GetNext(pos);
		if( pObj != NULL )
		{
			if( nThisKey == pObj->m_nNodeObjKey )
				return TRUE;

		}

	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// CWaitCallObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CWaitCallObj, CNodeObj, 0)

CWaitCallObj::CWaitCallObj()
{
	m_bRecCalledID = FALSE;
	m_bRecCallerID = FALSE;
	m_bClearDTMF = FALSE;
	m_nRings = 3;
	m_bAnswerCall = TRUE;
}

CWaitCallObj::CWaitCallObj( CPoint point )
	: CNodeObj(point)
{
	m_bRecCalledID = FALSE;
	m_bRecCallerID = FALSE;
	m_bClearDTMF = FALSE;
	m_nRings = 3;
	m_bAnswerCall = TRUE;
}
CWaitCallObj::CWaitCallObj( const CWaitCallObj &s )
	:CNodeObj(s)
{
	
	m_CalledID = s.m_CalledID;
	m_bRecCalledID = s.m_bRecCalledID;
	m_CallerID = s.m_CallerID;
	m_bRecCallerID = s.m_bRecCallerID;
	m_bClearDTMF = s.m_bClearDTMF;
	m_nRings = s.m_nRings;
	m_bAnswerCall = s.m_bAnswerCall;
}
CWaitCallObj & CWaitCallObj::operator =( const CWaitCallObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_CalledID = s.m_CalledID;
	m_bRecCalledID = s.m_bRecCalledID;
	m_CallerID = s.m_CallerID;
	m_bRecCallerID = s.m_bRecCallerID;
	m_bClearDTMF = s.m_bClearDTMF;
	m_nRings = s.m_nRings;
	m_bAnswerCall = s.m_bAnswerCall;

	return *this;
	
}

CWaitCallObj::~CWaitCallObj()
{

}
void CWaitCallObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_bRecCalledID;
		ar << m_bRecCallerID;
		ar << m_bClearDTMF;
		ar << m_nRings;
		ar << m_bAnswerCall;
		m_CalledID.Serialize( ar );
		m_CallerID.Serialize( ar );

	}
	else//读数据
	{
		ar >> m_bRecCalledID;
		ar >> m_bRecCallerID;
		ar >> m_bClearDTMF;
		ar >> m_nRings;
		ar >> m_bAnswerCall;
		m_CalledID.Serialize( ar );
		m_CallerID.Serialize( ar );

	}


}


//////////////////////////////////////////////////////////////////////
// CPlayStartObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPlayStartObj, CNodeObj, 0)

CPlayStartObj::CPlayStartObj()
{
	m_bRecDTMF = FALSE;
	m_bClearDTMF = FALSE;
	m_bPlayStop = FALSE;
	m_strSaveDTMF = "";
	m_strConfNO = "";
	m_nPlayWay = 0;
	

}

CPlayStartObj::CPlayStartObj( CPoint point )
	: CNodeObj(point)
{
	m_bRecDTMF = FALSE;
	m_bClearDTMF = FALSE;
	m_bPlayStop = FALSE;
	m_strSaveDTMF = "";
	m_strConfNO = "";
	m_nPlayWay = 0;



}
CPlayStartObj::CPlayStartObj( const CPlayStartObj &s )
	:CNodeObj(s)
{
	
	m_strSaveDTMF = s.m_strSaveDTMF;
	m_bRecDTMF = s.m_bRecDTMF;
	m_bClearDTMF = s.m_bClearDTMF;
	m_bPlayStop  = s.m_bPlayStop;
	m_strConfNO = s.m_strConfNO;
	m_nPlayWay = s.m_nPlayWay;



	POSITION pos = s.m_PlayList.GetHeadPosition();
	while( pos )
	{
		CNodeData *pObj = s.m_PlayList.GetNext(pos);
		if( pObj != NULL )
		{
			CNodeData *pNewObj = new CNodeData;
			*pNewObj = *pObj;
			m_PlayList.AddTail( pNewObj );
		}
	}



}
CPlayStartObj & CPlayStartObj::operator =( const CPlayStartObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strSaveDTMF = s.m_strSaveDTMF;
	m_bRecDTMF = s.m_bRecDTMF;
	m_bClearDTMF = s.m_bClearDTMF;
	m_bPlayStop  = s.m_bPlayStop;
	m_strConfNO = s.m_strConfNO;
	m_nPlayWay = s.m_nPlayWay;



	POSITION pos = s.m_PlayList.GetHeadPosition();
	while( pos )
	{
		CNodeData *pObj = s.m_PlayList.GetNext(pos);
		if( pObj != NULL )
		{
			CNodeData *pNewObj = new CNodeData;
			*pNewObj = *pObj;
			m_PlayList.AddTail( pNewObj );
		}
	}
	return *this;
	
}

CPlayStartObj::~CPlayStartObj()
{
	while ( m_PlayList.GetHeadPosition() )
	{
		delete m_PlayList.RemoveHead();
	}
}
void CPlayStartObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_bRecDTMF;
		ar << m_bClearDTMF;
		ar << m_strSaveDTMF;
		ar << m_bPlayStop;
		ar << m_strConfNO;
		ar << m_nPlayWay;
		m_PlayList.Serialize(ar);

	}
	else//读数据
	{
		ar >> m_bRecDTMF;
		ar >> m_bClearDTMF;
		ar >> m_strSaveDTMF;
		ar >> m_bPlayStop;
		ar >> m_strConfNO;
		ar >> m_nPlayWay;
		m_PlayList.Serialize(ar);

	}


}


//////////////////////////////////////////////////////////////////////
// CDialCheckObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDialCheckObj, CNodeObj, 0)

CDialCheckObj::CDialCheckObj()
{
	m_bAfterRings	= FALSE;
	m_bNotAnswer	= FALSE;
	m_nRings		= 6;
	m_nTimes		= 60;
	m_strPhone		= "";
	m_strPrefix		= "";

	m_bConnect		= TRUE;
	m_bBusy			= FALSE;
	m_bNoDialTone	= FALSE;
	m_bNoAnswer		= FALSE;
	m_bNoRingBack	= FALSE;
	m_bError		= TRUE;


}

CDialCheckObj::CDialCheckObj( CPoint point )
	: CNodeObj(point)
{
	m_bAfterRings	= FALSE;
	m_bNotAnswer	= FALSE;
	m_nRings		= 6;
	m_nTimes		= 60;
	m_strPhone		= "";
	m_strPrefix		= "";


	m_bConnect		= TRUE;
	m_bBusy			= FALSE;
	m_bNoDialTone	= FALSE;
	m_bNoAnswer		= FALSE;
	m_bNoRingBack	= FALSE;
	m_bError		= TRUE;



}
CDialCheckObj::CDialCheckObj( const CDialCheckObj &s )
	:CNodeObj(s)
{
	
	m_bAfterRings	= s.m_bAfterRings;
	m_bNotAnswer	= s.m_bNotAnswer;
	m_nRings		= s.m_nRings;
	m_nTimes		= s.m_nTimes;
	m_strPhone		= s.m_strPhone;
	m_strPrefix		= s.m_strPrefix;


	m_bConnect		= s.m_bConnect;
	m_bBusy			= s.m_bBusy;
	m_bNoDialTone	= s.m_bNoDialTone;
	m_bNoAnswer		= s.m_bNoAnswer;
	m_bNoRingBack	= s.m_bNoRingBack;
	m_bError		= s.m_bError;



}
CDialCheckObj & CDialCheckObj::operator =( const CDialCheckObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_bAfterRings	= s.m_bAfterRings;
	m_bNotAnswer	= s.m_bNotAnswer;
	m_nRings		= s.m_nRings;
	m_nTimes		= s.m_nTimes;
	m_strPhone		= s.m_strPhone;
	m_strPrefix		= s.m_strPrefix;


	m_bConnect		= s.m_bConnect;
	m_bBusy			= s.m_bBusy;
	m_bNoDialTone	= s.m_bNoDialTone;
	m_bNoAnswer		= s.m_bNoAnswer;
	m_bNoRingBack	= s.m_bNoRingBack;
	m_bError		= s.m_bError;


	return *this;
	
}

CDialCheckObj::~CDialCheckObj()
{

}
void CDialCheckObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_bAfterRings;
		ar << m_bNotAnswer;
		ar << m_nRings;
		ar << m_nTimes;
		ar << m_strPhone;
		ar << m_strPrefix;


		ar << m_bConnect;
		ar << m_bBusy;
		ar << m_bNoDialTone;
		ar << m_bNoAnswer;
		ar << m_bNoRingBack;
		ar << m_bError;


	}
	else//读数据
	{
		ar >> m_bAfterRings;
		ar >> m_bNotAnswer;
		ar >> m_nRings;
		ar >> m_nTimes;
		ar >> m_strPhone;
		ar >> m_strPrefix;

		ar >> m_bConnect;
		ar >> m_bBusy;
		ar >> m_bNoDialTone;
		ar >> m_bNoAnswer;
		ar >> m_bNoRingBack;
		ar >> m_bError;

		
	}


}


//////////////////////////////////////////////////////////////////////
// CReleaseCallObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CReleaseCallObj, CNodeObj, 0)

CReleaseCallObj::CReleaseCallObj()
{
	m_bSaveDetail = FALSE;

}

CReleaseCallObj::CReleaseCallObj( CPoint point )
	: CNodeObj(point)
{
	m_bSaveDetail = FALSE;

}
CReleaseCallObj::CReleaseCallObj( const CReleaseCallObj &s )
	:CNodeObj(s)
{
	
	m_bSaveDetail = s.m_bSaveDetail;

}
CReleaseCallObj & CReleaseCallObj::operator =( const CReleaseCallObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_bSaveDetail = s.m_bSaveDetail;


	return *this;
	
}

CReleaseCallObj::~CReleaseCallObj()
{

}
void CReleaseCallObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_bSaveDetail;


	}
	else//读数据
	{
		ar >> m_bSaveDetail;


	}


}


//////////////////////////////////////////////////////////////////////
// CCallHoldObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCallHoldObj, CNodeObj, 0)

CCallHoldObj::CCallHoldObj()
{
	m_nCallHold = 0;
	m_bPlayVox = FALSE;
	m_strPlayFile = "";

}

CCallHoldObj::CCallHoldObj( CPoint point )
	: CNodeObj(point)
{
	m_nCallHold = 0;
	m_bPlayVox = FALSE;
	m_strPlayFile = "";

}
CCallHoldObj::CCallHoldObj( const CCallHoldObj &s )
	:CNodeObj(s)
{
	
	m_nCallHold = s.m_nCallHold;
	m_bPlayVox = s.m_bPlayVox;
	m_strPlayFile = s.m_strPlayFile;

}
CCallHoldObj & CCallHoldObj::operator =( const CCallHoldObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nCallHold = s.m_nCallHold;
	m_bPlayVox = s.m_bPlayVox;
	m_strPlayFile = s.m_strPlayFile;


	return *this;
	
}

CCallHoldObj::~CCallHoldObj()
{

}
void CCallHoldObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nCallHold;
		ar << m_bPlayVox;
		ar << m_strPlayFile;

	}
	else//读数据
	{
		ar >> m_nCallHold;
		ar >> m_bPlayVox;
		ar >> m_strPlayFile;

	}


}



//////////////////////////////////////////////////////////////////////
// CTransferCheckObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CTransferCheckObj, CNodeObj, 0)
CTransferCheckObj::CTransferCheckObj()
{
	m_bClearDTMF	= TRUE;
	m_bAfterRings	= FALSE;
	m_bNotAnswer	= FALSE;
	m_nRings		= 6;
	m_nTimes		= 60;
	m_nWaitOption	= 0;
	m_strPlayFile	= "";

	m_nDialType		= 0;
	m_strExpression = "";
	m_nTransferAim = 0;
	m_nTransferType = 0;
	m_strCancelDTMF = "#";
}

CTransferCheckObj::CTransferCheckObj( CPoint point )
	: CNodeObj(point)
{
	m_bClearDTMF	= TRUE;
	m_bAfterRings	= FALSE;
	m_bNotAnswer	= FALSE;
	m_nRings		= 6;
	m_nTimes		= 60;
	m_nWaitOption	= 0;
	m_strPlayFile	= "";

	m_nDialType		= 0;
	m_strExpression = "";
	m_nTransferAim = 0;
	m_nTransferType = 0;
	m_strCancelDTMF = "#";
	m_bTalkWitDN = FALSE;
	m_strCompleteDTMF="*";
}
CTransferCheckObj::CTransferCheckObj( const CTransferCheckObj &s )
	:CNodeObj(s)
{
	m_bClearDTMF	= s.m_bClearDTMF;
	m_bAfterRings	= s.m_bAfterRings;
	m_bNotAnswer	= s.m_bNotAnswer;
	m_nRings		= s.m_nRings;
	m_nTimes		= s.m_nTimes;
	m_nWaitOption	= s.m_nWaitOption;
	m_strPlayFile	= s.m_strPlayFile;
	m_nDialType		= s.m_nDialType;
	m_strExpression = s.m_strExpression;
	m_nTransferAim = s.m_nTransferAim;
	m_nTransferType = s.m_nTransferType;
	m_strCancelDTMF = s.m_strCancelDTMF;
	m_bTalkWitDN = s.m_bTalkWitDN;
	m_strCompleteDTMF=s.m_strCompleteDTMF;

}
CTransferCheckObj & CTransferCheckObj::operator =( const CTransferCheckObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_bClearDTMF	= s.m_bClearDTMF;
	m_bAfterRings	= s.m_bAfterRings;
	m_bNotAnswer	= s.m_bNotAnswer;
	m_nRings		= s.m_nRings;
	m_nTimes		= s.m_nTimes;
	m_nWaitOption	= s.m_nWaitOption;
	m_strPlayFile	= s.m_strPlayFile;
	m_nDialType		= s.m_nDialType;
	m_strExpression = s.m_strExpression;
	m_nTransferAim = s.m_nTransferAim;
	m_nTransferType = s.m_nTransferType;
	m_strCancelDTMF = s.m_strCancelDTMF;

	m_bTalkWitDN = s.m_bTalkWitDN;
	m_strCompleteDTMF=s.m_strCompleteDTMF;
	return *this;
	
}

CTransferCheckObj::~CTransferCheckObj()
{

}
void CTransferCheckObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_bClearDTMF;
		ar << m_bAfterRings;
		ar << m_bNotAnswer;
		ar << m_nRings;
		ar << m_nTimes;
		ar << m_nWaitOption;
		ar << m_strPlayFile;
		ar << m_nDialType;
		ar << m_strExpression;
		ar << m_nTransferAim;
		ar << m_nTransferType;
		ar << m_strCancelDTMF;
		ar << m_bTalkWitDN;
		ar << m_strCompleteDTMF;
	}
	else//读数据
	{
		ar >> m_bClearDTMF;
		ar >> m_bAfterRings;
		ar >> m_bNotAnswer;
		ar >> m_nRings;
		ar >> m_nTimes;
		ar >> m_nWaitOption;
		ar >> m_strPlayFile;
		ar >> m_nDialType;
		ar >> m_strExpression;
		ar >> m_nTransferAim;
		ar >> m_nTransferType;
		ar >> m_strCancelDTMF;
		ar >> m_bTalkWitDN;
		ar >> m_strCompleteDTMF;
	}


}



//////////////////////////////////////////////////////////////////////
// CCallLogRequestObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCallLogRequestObj, CNodeObj, 0)

CCallLogRequestObj::CCallLogRequestObj()
{
	m_nRequestType = 0;

}

CCallLogRequestObj::CCallLogRequestObj( CPoint point )
	: CNodeObj(point)
{
	m_nRequestType = FALSE;

}
CCallLogRequestObj::CCallLogRequestObj( const CCallLogRequestObj &s )
	:CNodeObj(s)
{
	
	m_nRequestType = s.m_nRequestType;

}
CCallLogRequestObj & CCallLogRequestObj::operator =( const CCallLogRequestObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nRequestType = s.m_nRequestType;


	return *this;
	
}

CCallLogRequestObj::~CCallLogRequestObj()
{

}
void CCallLogRequestObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nRequestType;


	}
	else//读数据
	{
		ar >> m_nRequestType;


	}


}


//////////////////////////////////////////////////////////////////////
// CDLLLoadObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDLLLoadObj, CNodeObj, 0)

CDLLLoadObj::CDLLLoadObj()
{
	m_nHandleType = 0;
	m_nDLLKey= -1;
	m_strDLLFile = "";

}

CDLLLoadObj::CDLLLoadObj( CPoint point )
	: CNodeObj(point)
{
	m_nHandleType = 0;
	m_nDLLKey= 0;
	m_strDLLFile = "";
}
CDLLLoadObj::CDLLLoadObj( const CDLLLoadObj &s )
	:CNodeObj(s)
{
	m_nHandleType = s.m_nHandleType;
	m_nDLLKey = s.m_nDLLKey;
	m_strDLLFile = s.m_strDLLFile;
}
CDLLLoadObj & CDLLLoadObj::operator =( const CDLLLoadObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nHandleType = s.m_nHandleType;
	m_nDLLKey = s.m_nDLLKey;
	m_strDLLFile = s.m_strDLLFile;

	return *this;
	
}

CDLLLoadObj::~CDLLLoadObj()
{

}
void CDLLLoadObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		ar << m_nHandleType;
		ar << m_nDLLKey;
		ar << m_strDLLFile;

	}
	else//读数据
	{

		ar >> m_nHandleType;
		ar >> m_nDLLKey;
		ar >> m_strDLLFile;

	}


}


//////////////////////////////////////////////////////////////////////
// CDLLCallObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDLLCallObj, CNodeObj, 0)

CDLLCallObj::CDLLCallObj()
{
	m_nHandleType = 0;
	m_nDLLKey= -1;
	m_strDLL	= "";
	m_strFunction = "";	
}

CDLLCallObj::CDLLCallObj( CPoint point )
	: CNodeObj(point)
{
	m_nHandleType = 0;
	m_nDLLKey= 0;
	m_strDLL	= "";
	m_strFunction = "";	
}
CDLLCallObj::CDLLCallObj( const CDLLCallObj &s )
	:CNodeObj(s)
{
	m_nHandleType = s.m_nHandleType;
	m_nDLLKey = s.m_nDLLKey;
	m_strDLL	= s.m_strDLL;
	m_strFunction = s.m_strFunction;
}
CDLLCallObj & CDLLCallObj::operator =( const CDLLCallObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nHandleType = s.m_nHandleType;
	m_nDLLKey = s.m_nDLLKey;
	m_strDLL	= s.m_strDLL;
	m_strFunction = s.m_strFunction;
	return *this;
	
}

CDLLCallObj::~CDLLCallObj()
{

}
void CDLLCallObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		ar << m_nHandleType;
		ar << m_nDLLKey;
		ar << m_strDLL;
		ar << m_strFunction;
	}
	else//读数据
	{

		ar >> m_nHandleType;
		ar >> m_nDLLKey;
		ar >> m_strDLL;
		ar >> m_strFunction;
	}


}

//////////////////////////////////////////////////////////////////////
// CDLLUnLoadObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDLLUnLoadObj, CNodeObj, 0)

CDLLUnLoadObj::CDLLUnLoadObj()
{
	m_nHandleType = 0;
	m_nDLLKey= -1;
	m_strDLL = "";

}

CDLLUnLoadObj::CDLLUnLoadObj( CPoint point )
	: CNodeObj(point)
{
	m_nHandleType = 0;
	m_nDLLKey= 0;
	m_strDLL = "";
}
CDLLUnLoadObj::CDLLUnLoadObj( const CDLLUnLoadObj &s )
	:CNodeObj(s)
{
	m_nHandleType = s.m_nHandleType;
	m_nDLLKey = s.m_nDLLKey;
	m_strDLL = s.m_strDLL;
}
CDLLUnLoadObj & CDLLUnLoadObj::operator =( const CDLLUnLoadObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nHandleType = s.m_nHandleType;
	m_nDLLKey = s.m_nDLLKey;
	m_strDLL = s.m_strDLL;

	return *this;
	
}

CDLLUnLoadObj::~CDLLUnLoadObj()
{

}
void CDLLUnLoadObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		ar << m_nHandleType;
		ar << m_nDLLKey;
		ar << m_strDLL;

	}
	else//读数据
	{

		ar >> m_nHandleType;
		ar >> m_nDLLKey;
		ar >> m_strDLL;

	}


}


//////////////////////////////////////////////////////////////////////
// CDBOpenObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDBOpenObj, CNodeObj, 0)

CDBOpenObj::CDBOpenObj()
{
	m_nDBType = 0;
	m_nDBKey= -1;
	m_strConn = "";
	m_strDBName = "";
}

CDBOpenObj::CDBOpenObj( CPoint point )
	: CNodeObj(point)
{
	m_nDBType = 0;
	m_nDBKey= 0;
	m_strConn = "";
	m_strDBName = "";

}
CDBOpenObj::CDBOpenObj( const CDBOpenObj &s )
	:CNodeObj(s)
{
	m_nDBType = s.m_nDBType;
	m_nDBKey = s.m_nDBKey;
	m_strConn = s.m_strConn;
	m_strDBName = s.m_strDBName;

}
CDBOpenObj & CDBOpenObj::operator =( const CDBOpenObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nDBType = s.m_nDBType;
	m_nDBKey = s.m_nDBKey;
	m_strConn = s.m_strConn;
	m_strDBName = s.m_strDBName;

	return *this;
	
}

CDBOpenObj::~CDBOpenObj()
{

}
void CDBOpenObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		ar << m_nDBType;
		ar << m_nDBKey;
		ar << m_strConn;
		ar << m_strDBName;

	}
	else//读数据
	{

		ar >> m_nDBType;
		ar >> m_nDBKey;
		ar >> m_strConn;
		ar >> m_strDBName;

	}


}

//////////////////////////////////////////////////////////////////////
// CSQLExcuteObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CSQLExcuteObj, CNodeObj, 0)

CSQLExcuteObj::CSQLExcuteObj()
{
	m_nRecordsetKey = -1;
	m_strRecordset = "";
	m_nSQLType = 0;
	m_strSQL = "";
	
	m_nDBKey = -1;
	m_nDBType = 0;
	m_strDB = "";
}

CSQLExcuteObj::CSQLExcuteObj( CPoint point )
	: CNodeObj(point)
{
	m_nRecordsetKey = -1;
	m_strRecordset = "";
	m_nSQLType = 0;
	m_strSQL = "";
	
	m_nDBKey = -1;
	m_nDBType = 0;
	m_strDB = "";
}
CSQLExcuteObj::CSQLExcuteObj( const CSQLExcuteObj &s )
	:CNodeObj(s)
{
	m_nRecordsetKey = s.m_nRecordsetKey;
	m_strRecordset = s.m_strRecordset;
	m_nSQLType = s.m_nSQLType;
	m_strSQL = s.m_strSQL;
	m_nDBKey = s.m_nDBKey;
	m_nDBType = s.m_nDBType;
	m_strDB  = s.m_strDB;
}
CSQLExcuteObj & CSQLExcuteObj::operator =( const CSQLExcuteObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nRecordsetKey = s.m_nRecordsetKey;
	m_strRecordset = s.m_strRecordset;
	m_nSQLType = s.m_nSQLType;
	m_strSQL = s.m_strSQL;
	m_nDBKey = s.m_nDBKey;
	m_nDBType = s.m_nDBType;
	m_strDB  = s.m_strDB;
	return *this;
	
}

CSQLExcuteObj::~CSQLExcuteObj()
{

}
void CSQLExcuteObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		ar << m_nRecordsetKey;
		ar << m_strRecordset;
		ar << m_nSQLType;
		ar << m_strSQL;
		
		ar << m_nDBKey;
		ar << m_nDBType;
		ar << m_strDB;
	}
	else//读数据
	{

		ar >> m_nRecordsetKey;
		ar >> m_strRecordset;
		ar >> m_nSQLType;
		ar >> m_strSQL;
		
		ar >> m_nDBKey;
		ar >> m_nDBType;
		ar >> m_strDB;
	}


}

//////////////////////////////////////////////////////////////////////
// CDBBrowseObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDBBrowseObj, CNodeObj, 0)

CDBBrowseObj::CDBBrowseObj()
{
	m_nRecordsetKey = 0;
	m_strRecordset = "";
	m_nBrowseType = 0;
}

CDBBrowseObj::CDBBrowseObj( CPoint point )
	: CNodeObj(point)
{
	m_nRecordsetKey = 0;
	m_strRecordset = "";
	m_nBrowseType = 0;

}
CDBBrowseObj::CDBBrowseObj( const CDBBrowseObj &s )
	:CNodeObj(s)
{
	m_nRecordsetKey = s.m_nRecordsetKey;
	m_strRecordset = s.m_strRecordset;
	m_nBrowseType = s.m_nBrowseType;
	
}
CDBBrowseObj & CDBBrowseObj::operator =( const CDBBrowseObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nRecordsetKey = s.m_nRecordsetKey;
	m_strRecordset = s.m_strRecordset;
	m_nBrowseType = s.m_nBrowseType;

	return *this;
	
}

CDBBrowseObj::~CDBBrowseObj()
{

}
void CDBBrowseObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		ar << m_nRecordsetKey;
		ar << m_strRecordset;
		ar << m_nBrowseType;
	
	}
	else//读数据
	{

		ar >> m_nRecordsetKey;
		ar >> m_strRecordset;
		ar >> m_nBrowseType;
		
	}


}

//////////////////////////////////////////////////////////////////////
// CDBCloseObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDBCloseObj, CNodeObj, 0)

CDBCloseObj::CDBCloseObj()
{
	m_nDBType = 0;
	m_nDBKey= -1;
	m_strDB = "";

}

CDBCloseObj::CDBCloseObj( CPoint point )
	: CNodeObj(point)
{
	m_nDBType = 0;
	m_nDBKey= 0;
	m_strDB = "";
}
CDBCloseObj::CDBCloseObj( const CDBCloseObj &s )
	:CNodeObj(s)
{
	m_nDBType = s.m_nDBType;
	m_nDBKey = s.m_nDBKey;
	m_strDB = s.m_strDB;
}
CDBCloseObj & CDBCloseObj::operator =( const CDBCloseObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nDBType = s.m_nDBType;
	m_nDBKey = s.m_nDBKey;
	m_strDB = s.m_strDB;

	return *this;
	
}

CDBCloseObj::~CDBCloseObj()
{

}
void CDBCloseObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		ar << m_nDBType;
		ar << m_nDBKey;
		ar << m_strDB;

	}
	else//读数据
	{

		ar >> m_nDBType;
		ar >> m_nDBKey;
		ar >> m_strDB;

	}


}

//////////////////////////////////////////////////////////////////////
// CDTMFReciveObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDTMFReciveObj, CNodeObj, 0)

CDTMFReciveObj::CDTMFReciveObj()
{
	m_bClearDTMF = FALSE;
	m_nEndCondition = 0;
	m_nDTMFCount = 1;
	m_strDTMF = "#";
	m_nTimeOut = 60;
	m_nDTMFType = 0;

}

CDTMFReciveObj::CDTMFReciveObj( CPoint point )
	: CNodeObj(point)
{
	m_bClearDTMF	= FALSE;
	m_nEndCondition = 0;
	m_nDTMFCount	= 1;
	m_strDTMF		= "#";
	m_nTimeOut = 60;
	m_nDTMFType = 0;

}
CDTMFReciveObj::CDTMFReciveObj( const CDTMFReciveObj &s )
	:CNodeObj(s)
{
	m_SaveDTMF		= s.m_SaveDTMF;
	m_bClearDTMF	= s.m_bClearDTMF;
	m_nEndCondition = s.m_nEndCondition;
	m_nDTMFCount	= s.m_nDTMFCount;
	m_strDTMF		= s.m_strDTMF;
	m_nTimeOut = s.m_nTimeOut;
	m_nDTMFType = s.m_nDTMFType;

}
CDTMFReciveObj & CDTMFReciveObj::operator =( const CDTMFReciveObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_SaveDTMF		= s.m_SaveDTMF;
	m_bClearDTMF	= s.m_bClearDTMF;
	m_nEndCondition = s.m_nEndCondition;
	m_nDTMFCount	= s.m_nDTMFCount;
	m_strDTMF		= s.m_strDTMF;
	m_nTimeOut = s.m_nTimeOut;
	m_nDTMFType = s.m_nDTMFType;

	return *this;
	
}

CDTMFReciveObj::~CDTMFReciveObj()
{

}
void CDTMFReciveObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nEndCondition;
		ar << m_bClearDTMF;
		m_SaveDTMF.Serialize( ar );
		ar << m_strDTMF;
		ar << m_nDTMFCount;
		ar << m_nTimeOut;
		ar << m_nDTMFType;

	}
	else//读数据
	{
		ar >> m_nEndCondition;
		ar >> m_bClearDTMF;
		m_SaveDTMF.Serialize( ar );
		ar >> m_strDTMF;
		ar >> m_nDTMFCount;
		ar >>m_nTimeOut;
		ar >> m_nDTMFType;

	}


}




//////////////////////////////////////////////////////////////////////
// CEventCheckObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CEventCheckObj, CNodeObj, 0)

CEventCheckObj::CEventCheckObj()
{
	m_bError  = 	TRUE;
	m_bOffHook  = 	FALSE;
	m_bHangUp  = 	FALSE;
	m_bHookFlash  = 	FALSE;
	m_bRecDTMF  = 	FALSE;
	m_bRecFaxTone  = 	FALSE;
	m_bAppExit  = 	FALSE;
	m_bFaxSendTone  = 	FALSE;
	m_bConfDestroyed  = 	FALSE;
	m_bAgentLoginMsg  = 	FALSE;
	m_bAgentLogOutMsg  = 	FALSE;
	m_bAgentBusyMsg  = 	FALSE;
	m_bAgentInserveMsg  = 	FALSE;
	m_bAgentAnswerMsg  = 	FALSE;
	m_bAgentMakeCallMsg  = 	FALSE;
	m_bAgentTransferMsg  = 	FALSE;
	m_bAgentConferMsg  = 	FALSE;
	m_bAgentHangUpMsg  = 	FALSE;
	m_bAgentSendDataMsg  = 	FALSE;
	m_bAgentSendFaxMsg  = 	FALSE;
	m_bRinging  = 	FALSE;
	m_bNothing  = 	FALSE;
	m_bAgentHold  = 	FALSE;
	m_bAgentPickupHold  = 	FALSE;
	m_bAgentMonitor = 	FALSE;
	m_bAgentDisMonitor = 	FALSE;
	m_bAgentBuildConfer = FALSE;
	m_bElapseTime = FALSE;
	m_nElapseTime = 120;
	m_bClearDTMF = FALSE;
	m_bPlay	= FALSE;
	m_strPlayFile = "";
	m_nPlayType = 0;
	m_strPlayDTMF = "#";
	m_bAgentGetActiveCall = FALSE;
}

CEventCheckObj::CEventCheckObj( CPoint point )
	: CNodeObj(point)
{
	m_bError  = 	TRUE;
	m_bOffHook  = 	FALSE;
	m_bHangUp  = 	FALSE;
	m_bHookFlash  = 	FALSE;
	m_bRecDTMF  = 	FALSE;
	m_bRecFaxTone  = 	FALSE;
	m_bAppExit  = 	FALSE;
	m_bFaxSendTone  = 	FALSE;
	m_bConfDestroyed  = 	FALSE;
	m_bAgentLoginMsg  = 	FALSE;
	m_bAgentLogOutMsg  = 	FALSE;
	m_bAgentBusyMsg  = 	FALSE;
	m_bAgentInserveMsg  = 	FALSE;
	m_bAgentAnswerMsg  = 	FALSE;
	m_bAgentMakeCallMsg  = 	FALSE;
	m_bAgentTransferMsg  = 	FALSE;
	m_bAgentConferMsg  = 	FALSE;
	m_bAgentHangUpMsg  = 	FALSE;
	m_bAgentSendDataMsg  = 	FALSE;
	m_bAgentSendFaxMsg  = 	FALSE;
	m_bRinging  = 	FALSE;
	m_bNothing  = 	FALSE;
	m_bAgentHold  = 	FALSE;
	m_bAgentPickupHold  = 	FALSE;
	m_bAgentMonitor = 	FALSE;
	m_bAgentDisMonitor = 	FALSE;
	m_bAgentBuildConfer = FALSE;
	m_bElapseTime = FALSE;
	m_nElapseTime = 120;
	m_bClearDTMF = FALSE;
	m_bPlay	= FALSE;
	m_strPlayFile = "";
	m_nPlayType = 0;
	m_strPlayDTMF = "#";
	m_bAgentGetActiveCall = FALSE;
}
CEventCheckObj::CEventCheckObj( const CEventCheckObj &s )
	:CNodeObj(s)
{
	
	 m_bError = 	s.m_bError;
	 m_bOffHook = 	s.m_bOffHook;
	 m_bHangUp = 	s.m_bHangUp;
	 m_bHookFlash = 	s.m_bHookFlash;
	 m_bRecDTMF = 	s.m_bRecDTMF;
	 m_bRecFaxTone = 	s.m_bRecFaxTone;
	 m_bAppExit = 	s.m_bAppExit;
	 m_bFaxSendTone = 	s.m_bFaxSendTone;
	 m_bConfDestroyed = 	s.m_bConfDestroyed;
	 m_bAgentLoginMsg = 	s.m_bAgentLoginMsg;
	 m_bAgentLogOutMsg = 	s.m_bAgentLogOutMsg;
	 m_bAgentBusyMsg = 	s.m_bAgentBusyMsg;
	 m_bAgentInserveMsg = 	s.m_bAgentInserveMsg;
	 m_bAgentAnswerMsg = 	s.m_bAgentAnswerMsg;
	 m_bAgentMakeCallMsg = 	s.m_bAgentMakeCallMsg;
	 m_bAgentTransferMsg = 	s.m_bAgentTransferMsg;
	 m_bAgentConferMsg = 	s.m_bAgentConferMsg;
	 m_bAgentHangUpMsg = 	s.m_bAgentHangUpMsg;
	 m_bAgentSendDataMsg = 	s.m_bAgentSendDataMsg;
	 m_bAgentSendFaxMsg = 	s.m_bAgentSendFaxMsg;
	 m_bRinging		= 	s.m_bRinging;
	 m_bNothing		= 	s.m_bNothing;
	 m_bAgentHold	= s.m_bAgentHold;
	 m_bAgentPickupHold		= 	s.m_bAgentPickupHold;
	 m_bAgentMonitor		= 	s.m_bAgentMonitor;
	 m_bAgentDisMonitor		= 	s.m_bAgentDisMonitor;
	 m_bAgentBuildConfer = 	s.m_bAgentBuildConfer;
	 m_bElapseTime = s.m_bElapseTime;
     m_nElapseTime = s.m_nElapseTime;
	 m_bClearDTMF = s.m_bClearDTMF;
	m_bPlay	= s.m_bPlay;
	m_strPlayFile = s.m_strPlayFile;
	m_nPlayType = s.m_nPlayType;
	m_strPlayDTMF = s.m_strPlayDTMF;
	m_bAgentGetActiveCall = s.m_bAgentGetActiveCall;
}
CEventCheckObj & CEventCheckObj::operator =( const CEventCheckObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	 m_bError = 	s.m_bError;
	 m_bOffHook = 	s.m_bOffHook;
	 m_bHangUp = 	s.m_bHangUp;
	 m_bHookFlash = 	s.m_bHookFlash;
	 m_bRecDTMF = 	s.m_bRecDTMF;
	 m_bRecFaxTone = 	s.m_bRecFaxTone;
	 m_bAppExit = 	s.m_bAppExit;
	 m_bFaxSendTone = 	s.m_bFaxSendTone;
	 m_bConfDestroyed = 	s.m_bConfDestroyed;
	 m_bAgentLoginMsg = 	s.m_bAgentLoginMsg;
	 m_bAgentLogOutMsg = 	s.m_bAgentLogOutMsg;
	 m_bAgentBusyMsg = 	s.m_bAgentBusyMsg;
	 m_bAgentInserveMsg = 	s.m_bAgentInserveMsg;
	 m_bAgentAnswerMsg = 	s.m_bAgentAnswerMsg;
	 m_bAgentMakeCallMsg = 	s.m_bAgentMakeCallMsg;
	 m_bAgentTransferMsg = 	s.m_bAgentTransferMsg;
	 m_bAgentConferMsg = 	s.m_bAgentConferMsg;
	 m_bAgentHangUpMsg = 	s.m_bAgentHangUpMsg;
	 m_bAgentSendDataMsg = 	s.m_bAgentSendDataMsg;
	 m_bAgentSendFaxMsg = 	s.m_bAgentSendFaxMsg;
	 m_bRinging = 	s.m_bRinging;
	 m_bNothing  = 	s.m_bNothing;
	 m_bAgentHold  = s.m_bAgentHold;
	 m_bAgentPickupHold  = 	s.m_bAgentPickupHold;
	 m_bAgentMonitor		= 	s.m_bAgentMonitor;
	 m_bAgentDisMonitor		= 	s.m_bAgentDisMonitor;
	 m_bAgentBuildConfer = 	s.m_bAgentBuildConfer;
	 m_bElapseTime = s.m_bElapseTime;
     m_nElapseTime = s.m_nElapseTime;
	 m_bClearDTMF = s.m_bClearDTMF;
	m_bPlay	= s.m_bPlay;
	m_strPlayFile = s.m_strPlayFile;
	m_nPlayType = s.m_nPlayType;
	m_strPlayDTMF = s.m_strPlayDTMF;
	m_bAgentGetActiveCall = s.m_bAgentGetActiveCall;
	return *this;
	
}

CEventCheckObj::~CEventCheckObj()
{

}
void CEventCheckObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar	<<	 m_bError	;
		ar	<<	 m_bOffHook	;
		ar	<<	 m_bHangUp	;
		ar	<<	 m_bHookFlash	;
		ar	<<	 m_bRecDTMF	;
		ar	<<	 m_bRecFaxTone	;
		ar	<<	 m_bAppExit	;
		ar	<<	 m_bFaxSendTone	;
		ar	<<	 m_bConfDestroyed	;
		ar	<<	 m_bAgentLoginMsg	;
		ar	<<	 m_bAgentLogOutMsg	;
		ar	<<	 m_bAgentBusyMsg	;
		ar	<<	 m_bAgentInserveMsg	;
		ar	<<	 m_bAgentAnswerMsg	;
		ar	<<	 m_bAgentMakeCallMsg	;
		ar	<<	 m_bAgentTransferMsg	;
		ar	<<	 m_bAgentConferMsg	;
		ar	<<	 m_bAgentHangUpMsg	;
		ar	<<	 m_bAgentSendDataMsg	;
		ar	<<	 m_bAgentSendFaxMsg	;
		ar	<<	 m_bRinging	;
		ar	<<	 m_bNothing;
		ar	<<	 m_bAgentHold;
		ar	<<	 m_bAgentPickupHold;
		ar	<<	 m_bAgentMonitor;
		ar	<<	 m_bAgentDisMonitor;
		ar	<<	 m_bAgentBuildConfer;
		ar	<<	 m_bElapseTime;
		ar	<<	 m_nElapseTime;
		ar	<<	 m_bClearDTMF;
		ar	<<	 m_bPlay;
		ar	<<	 m_strPlayFile;
		ar	<<	 m_nPlayType;
		ar	<<	 m_strPlayDTMF;
		ar	<<	 m_bAgentGetActiveCall;
	}
	else//读数据
	{
		ar	>>	 m_bError	;
		ar	>>	 m_bOffHook	;
		ar	>>	 m_bHangUp	;
		ar	>>	 m_bHookFlash	;
		ar	>>	 m_bRecDTMF	;
		ar	>>	 m_bRecFaxTone	;
		ar	>>	 m_bAppExit	;
		ar	>>	 m_bFaxSendTone	;
		ar	>>	 m_bConfDestroyed	;
		ar	>>	 m_bAgentLoginMsg	;
		ar	>>	 m_bAgentLogOutMsg	;
		ar	>>	 m_bAgentBusyMsg	;
		ar	>>	 m_bAgentInserveMsg	;
		ar	>>	 m_bAgentAnswerMsg	;
		ar	>>	 m_bAgentMakeCallMsg	;
		ar	>>	 m_bAgentTransferMsg	;
		ar	>>	 m_bAgentConferMsg	;
		ar	>>	 m_bAgentHangUpMsg	;
		ar	>>	 m_bAgentSendDataMsg	;
		ar	>>	 m_bAgentSendFaxMsg	;
		ar	>>	 m_bRinging	;
		ar	>>	 m_bNothing;
		ar	>>	 m_bAgentHold;
		ar	>>	 m_bAgentPickupHold;
		ar	>>	 m_bAgentMonitor;
		ar	>>	 m_bAgentDisMonitor;
		ar	>>	 m_bAgentBuildConfer;
		ar	>>	 m_bElapseTime;
		ar	>>	 m_nElapseTime;
		ar	>>	 m_bClearDTMF;
		ar	>>	 m_bPlay;
		ar	>>	 m_strPlayFile;
		ar	>>	 m_nPlayType;
		ar	>>	 m_strPlayDTMF;
		ar	>>	 m_bAgentGetActiveCall;
	}


}

//////////////////////////////////////////////////////////////////////
// CSwitchObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CSwitchObj, CNodeObj, 0)

CSwitchObj::CSwitchObj()
{
	m_nSwitchType = 0;
	m_nAimType = 0;
	m_nWaitOption = 0;
	m_strPlayFile = "";
	m_strAimChnl = "";

	m_nGetFreeChWay = 0;
	m_strBeginCH = "0";
	m_strEndCH = "0";
	m_nTimeOut = 30;
}

CSwitchObj::CSwitchObj( CPoint point )
	: CNodeObj(point)
{
	m_nSwitchType = 0;
	m_nAimType = 0;
	m_nWaitOption = 0;
	m_strPlayFile = "";
	m_strAimChnl = "";

	m_nGetFreeChWay = 0;
	m_strBeginCH = "0";
	m_strEndCH = "0";
	m_nTimeOut = 30;

}
CSwitchObj::CSwitchObj( const CSwitchObj &s )
	:CNodeObj(s)
{
	m_nSwitchType = s.m_nSwitchType;
	m_strAimChnl = s.m_strAimChnl;
	m_nAimType = s.m_nAimType;
	m_nWaitOption = s.m_nWaitOption;
	m_strPlayFile = s.m_strPlayFile;


	m_nGetFreeChWay = s.m_nGetFreeChWay;
	m_strBeginCH = s.m_strBeginCH;
	m_strEndCH = s.m_strEndCH;
	m_nTimeOut = s.m_nTimeOut;

}
CSwitchObj & CSwitchObj::operator =( const CSwitchObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nSwitchType = s.m_nSwitchType;
	m_strAimChnl = s.m_strAimChnl;
	m_nAimType = s.m_nAimType;
	m_nWaitOption = s.m_nWaitOption;
	m_strPlayFile = s.m_strPlayFile;


	m_nGetFreeChWay = s.m_nGetFreeChWay;
	m_strBeginCH = s.m_strBeginCH;
	m_strEndCH = s.m_strEndCH;
	m_nTimeOut = s.m_nTimeOut;

	return *this;
	
}

CSwitchObj::~CSwitchObj()
{

}
void CSwitchObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nSwitchType;
		ar << m_strAimChnl;
		ar << m_nAimType;
		ar << m_nWaitOption;
		ar << m_strPlayFile;


		ar << m_nGetFreeChWay;
		ar << m_strBeginCH;
		ar << m_strEndCH;
		ar << m_nTimeOut;

	}
	else//读数据
	{
		ar >> m_nSwitchType;
		ar >> m_strAimChnl;
		ar >> m_nAimType;
		ar >> m_nWaitOption;
		ar >> m_strPlayFile;


		ar >> m_nGetFreeChWay;
		ar >> m_strBeginCH;
		ar >> m_strEndCH;
		ar >> m_nTimeOut;
	}


}



//////////////////////////////////////////////////////////////////////
// CRouteCallObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CRouteCallObj, CNodeObj, 0)

CRouteCallObj::CRouteCallObj()
{
	m_strBeginCh = "";
	m_strEndCh = "";
	m_strTel = "";
	m_nWaitOption = 0;
	m_strWaitFile = "";
	m_nConnOption = 0;
	m_strConnFile = "";
}

CRouteCallObj::CRouteCallObj( CPoint point )
	: CNodeObj(point)
{
	m_strBeginCh = "";
	m_strEndCh = "";
	m_strTel = "";
	m_nWaitOption = 0;
	m_strWaitFile = "";
	m_nConnOption = 0;
	m_strConnFile = "";
}
CRouteCallObj::CRouteCallObj( const CRouteCallObj &s )
	:CNodeObj(s)
{
	m_strBeginCh = s.m_strBeginCh;
	m_strEndCh = s.m_strEndCh;
	m_strTel = s.m_strTel;
	m_nWaitOption = s.m_nWaitOption;
	m_strWaitFile = s.m_strWaitFile;
	m_nConnOption = s.m_nConnOption;
	m_strConnFile = s.m_strConnFile;

}
CRouteCallObj & CRouteCallObj::operator =( const CRouteCallObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strBeginCh = s.m_strBeginCh;
	m_strEndCh = s.m_strEndCh;
	m_strTel = s.m_strTel;
	m_nWaitOption = s.m_nWaitOption;
	m_strWaitFile = s.m_strWaitFile;
	m_nConnOption = s.m_nConnOption;
	m_strConnFile = s.m_strConnFile;

	return *this;
	
}

CRouteCallObj::~CRouteCallObj()
{

}
void CRouteCallObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strBeginCh;
		ar << m_strEndCh;
		ar << m_strTel;
		ar << m_nWaitOption ;
		ar << m_strWaitFile ;
		ar << m_nConnOption ;
		ar << m_strConnFile ;
	}
	else//读数据
	{
		ar >> m_strBeginCh;
		ar >> m_strEndCh;
		ar >> m_strTel;
		ar >> m_nWaitOption ;
		ar >> m_strWaitFile ;
		ar >> m_nConnOption ;
		ar >> m_strConnFile ;
	}


}



//////////////////////////////////////////////////////////////////////
// CCompareObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCompareObj, CNodeObj, 0)

CCompareObj::CCompareObj()
{
	m_nVarKey = -1;
	m_strVarName = "";
}

CCompareObj::CCompareObj( CPoint point )
	: CNodeObj(point)
{
	m_nVarKey = -1;
	m_strVarName = "";
}
CCompareObj::CCompareObj( const CCompareObj &s )
	:CNodeObj(s)
{
	
	m_nVarKey = s.m_nVarKey;
	m_strVarName = s.m_strVarName;

}
CCompareObj & CCompareObj::operator =( const CCompareObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nVarKey		= s.m_nVarKey;
	m_strVarName	= s.m_strVarName;

	return *this;
	
}

CCompareObj::~CCompareObj()
{

}
void CCompareObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nVarKey;
		ar << m_strVarName;

	}
	else//读数据
	{
		ar >> m_nVarKey;
		ar >> m_strVarName;
	}


}


//////////////////////////////////////////////////////////////////////
// CDTMFMenuObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDTMFMenuObj, CNodeObj, 0)

CDTMFMenuObj::CDTMFMenuObj()
{
	m_bClearDTMF = FALSE;
	m_nTimeOut = 30;
}

CDTMFMenuObj::CDTMFMenuObj( CPoint point )
	: CNodeObj(point)
{
	m_bClearDTMF = FALSE;
	m_nTimeOut = 30;
}
CDTMFMenuObj::CDTMFMenuObj( const CDTMFMenuObj &s )
	:CNodeObj(s)
{
	
	m_bClearDTMF = s.m_bClearDTMF;
	m_nTimeOut = s.m_nTimeOut;

}
CDTMFMenuObj & CDTMFMenuObj::operator =( const CDTMFMenuObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_bClearDTMF		= s.m_bClearDTMF;
	m_nTimeOut = s.m_nTimeOut;

	return *this;
	
}

CDTMFMenuObj::~CDTMFMenuObj()
{

}
void CDTMFMenuObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_bClearDTMF;
		ar << m_nTimeOut;
	}
	else//读数据
	{
		ar >> m_bClearDTMF;
		ar >> m_nTimeOut;
	}


}


//////////////////////////////////////////////////////////////////////
// CRecordObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CRecordObj, CNodeObj, 0)

CRecordObj::CRecordObj()
{
	m_strFileName = "";
	m_strFolder   = "";
	m_nFileLength = 1;
	m_nRecordType = 0;
	m_strConferNO = "";
	m_strEndDTMF = "#";
}

CRecordObj::CRecordObj( CPoint point )
	: CNodeObj(point)
{
	m_strFileName = "";
	m_strFolder   = "";
	m_nFileLength = 1;
	m_nRecordType = 0;
	m_strConferNO = "";
	m_strEndDTMF = "#";
}
CRecordObj::CRecordObj( const CRecordObj &s )
	:CNodeObj(s)
{
	m_strFileName = s.m_strFileName;
	m_strFolder   = s.m_strFolder;
	m_nFileLength = s.m_nFileLength;
	m_nRecordType = s.m_nRecordType;
	m_strConferNO = s.m_strConferNO;
	m_strEndDTMF = s.m_strEndDTMF;
}
CRecordObj & CRecordObj::operator =( const CRecordObj &s )
{
	if(this == &s)
		return *this;
	CNodeObj::operator = (s);
	m_strFileName = s.m_strFileName;
	m_strFolder   = s.m_strFolder;
	m_nFileLength = s.m_nFileLength;
	m_nRecordType = s.m_nRecordType;
	m_strConferNO = s.m_strConferNO;
	m_strEndDTMF = s.m_strEndDTMF;
	return *this;
	
}

CRecordObj::~CRecordObj()
{

}
void CRecordObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strFileName;
		ar << m_strFolder;
		ar << m_nFileLength;
		ar << m_nRecordType;
		ar << m_strConferNO;
		ar << m_strEndDTMF;

	}
	else//读数据
	{
		ar >> m_strFileName;
		ar >> m_strFolder;
		ar >> m_nFileLength;
		ar >> m_nRecordType;
		ar >> m_strConferNO;
		ar >> m_strEndDTMF;
	}


}



//////////////////////////////////////////////////////////////////////
// CFaxReciveObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFaxReciveObj, CNodeObj, 0)

CFaxReciveObj::CFaxReciveObj()
{
	m_strFileName = "";
	m_strFolder   = "";
	m_strPage	  = "";
}

CFaxReciveObj::CFaxReciveObj( CPoint point )
	: CNodeObj(point)
{
	m_strFileName = "";
	m_strFolder   = "";
	m_strPage	  = "";
}
CFaxReciveObj::CFaxReciveObj( const CFaxReciveObj &s )
	:CNodeObj(s)
{
	m_strFileName = s.m_strFileName;
	m_strFolder   = s.m_strFolder;
	m_strPage		  = s.m_strPage;
}
CFaxReciveObj & CFaxReciveObj::operator =( const CFaxReciveObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strFileName = s.m_strFileName;
	m_strFolder   = s.m_strFolder;
	m_strPage		  = s.m_strPage;

	return *this;
	
}

CFaxReciveObj::~CFaxReciveObj()
{

}
void CFaxReciveObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strFileName;
		ar << m_strFolder;
		ar << m_strPage;
	}
	else//读数据
	{
		ar >> m_strFileName;
		ar >> m_strFolder;
		ar >> m_strPage;
	}


}

//////////////////////////////////////////////////////////////////////
// CFaxSendObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFaxSendObj, CNodeObj, 0)

CFaxSendObj::CFaxSendObj()
{
	m_strFileName = "";
	m_strFolder = "";
}

CFaxSendObj::CFaxSendObj( CPoint point )
	: CNodeObj(point)
{
	m_strFileName = "";
	m_strFolder = "";
}
CFaxSendObj::CFaxSendObj( const CFaxSendObj &s )
	:CNodeObj(s)
{
	m_strFileName = s.m_strFileName;
	m_strFolder  = s.m_strFolder;
}
CFaxSendObj & CFaxSendObj::operator =( const CFaxSendObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strFileName = s.m_strFileName;
	m_strFolder  = s.m_strFolder;

	return *this;
	
}

CFaxSendObj::~CFaxSendObj()
{

}
void CFaxSendObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strFileName;
		ar << m_strFolder;
	}
	else//读数据
	{
		ar >> m_strFileName;
		ar >> m_strFolder;
	}


}


//////////////////////////////////////////////////////////////////////
// CConferenceBulidObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CConferenceBulidObj, CNodeObj, 0)

CConferenceBulidObj::CConferenceBulidObj()
{
	m_nBuildType = FALSE;
	m_strConferNo = "";
	m_strMaxMember = "";
}

CConferenceBulidObj::CConferenceBulidObj( CPoint point )
	: CNodeObj(point)
{
	m_nBuildType = FALSE;
	m_strConferNo = "";
	m_strMaxMember = "";
}
CConferenceBulidObj::CConferenceBulidObj( const CConferenceBulidObj &s )
	:CNodeObj(s)
{
	m_nBuildType = s.m_nBuildType;
	m_strConferNo = s.m_strConferNo;
	m_strMaxMember = s.m_strMaxMember;

}
CConferenceBulidObj & CConferenceBulidObj::operator =( const CConferenceBulidObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nBuildType = s.m_nBuildType;
	m_strConferNo = s.m_strConferNo;
	m_strMaxMember = s.m_strMaxMember;

	return *this;
	
}

CConferenceBulidObj::~CConferenceBulidObj()
{

}
void CConferenceBulidObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nBuildType;
		ar << m_strConferNo;
		ar << m_strMaxMember;

	}
	else//读数据
	{
		ar >> m_nBuildType;
		ar >> m_strConferNo;
		ar >> m_strMaxMember;
		
	}


}

//////////////////////////////////////////////////////////////////////
// CConferenceDeleteObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CConferenceDeleteObj, CNodeObj, 0)

CConferenceDeleteObj::CConferenceDeleteObj()
{
	m_nDeleteType = FALSE;
	m_strConferNo = "";
}

CConferenceDeleteObj::CConferenceDeleteObj( CPoint point )
	: CNodeObj(point)
{
	m_nDeleteType = FALSE;
	m_strConferNo = "";
}
CConferenceDeleteObj::CConferenceDeleteObj( const CConferenceDeleteObj &s )
	:CNodeObj(s)
{
	m_nDeleteType = s.m_nDeleteType;
	m_strConferNo = s.m_strConferNo;

}
CConferenceDeleteObj & CConferenceDeleteObj::operator =( const CConferenceDeleteObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nDeleteType = s.m_nDeleteType;
	m_strConferNo = s.m_strConferNo;

	return *this;
	
}

CConferenceDeleteObj::~CConferenceDeleteObj()
{

}
void CConferenceDeleteObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nDeleteType;
		ar << m_strConferNo;

	}
	else//读数据
	{
		ar >> m_nDeleteType;
		ar >> m_strConferNo;
		
	}


}

//////////////////////////////////////////////////////////////////////
// CConferencePlayObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CConferencePlayObj, CNodeObj, 0)

CConferencePlayObj::CConferencePlayObj()
{
	m_strConferNO = "";
	m_strFileExpress = "";
}

CConferencePlayObj::CConferencePlayObj( CPoint point )
	: CNodeObj(point)
{
	m_strConferNO = "";
	m_strFileExpress = "";
}
CConferencePlayObj::CConferencePlayObj( const CConferencePlayObj &s )
	:CNodeObj(s)
{
	m_strConferNO = s.m_strConferNO;
	m_strFileExpress = s.m_strFileExpress;

}
CConferencePlayObj & CConferencePlayObj::operator =( const CConferencePlayObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);
	m_strConferNO = s.m_strConferNO;
	m_strFileExpress = s.m_strFileExpress;

	return *this;
	
}

CConferencePlayObj::~CConferencePlayObj()
{

}
void CConferencePlayObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strConferNO;
		ar << m_strFileExpress;

	}
	else//读数据
	{
		ar >> m_strConferNO;
		ar >> m_strFileExpress;
		
	}


}

//////////////////////////////////////////////////////////////////////
// CConferenceRecordObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CConferenceRecordObj, CNodeObj, 0)

CConferenceRecordObj::CConferenceRecordObj()
{
	m_strConferNO = "";
	m_strFileExpress="";
	m_nFileLength=1;
	m_strFolder="";
}

CConferenceRecordObj::CConferenceRecordObj( CPoint point )
	: CNodeObj(point)
{
	m_strConferNO = "";
	m_strFileExpress="";
	m_nFileLength=1;
	m_strFolder="";
}
CConferenceRecordObj::CConferenceRecordObj( const CConferenceRecordObj &s )
	:CNodeObj(s)
{
	m_strConferNO = s.m_strConferNO;
	m_strFileExpress = s.m_strFileExpress;
	m_nFileLength = s.m_nFileLength;
	m_strFolder = s.m_strFolder;
}
CConferenceRecordObj & CConferenceRecordObj::operator =( const CConferenceRecordObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strConferNO = s.m_strConferNO;
	m_strFileExpress = s.m_strFileExpress;
	m_nFileLength = s.m_nFileLength;
	m_strFolder = s.m_strFolder;

	return *this;
	
}

CConferenceRecordObj::~CConferenceRecordObj()
{

}
void CConferenceRecordObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strConferNO;
		ar << m_strFileExpress;
		ar << m_nFileLength;
		ar << m_strFolder;
	}
	else//读数据
	{
		ar >> m_strConferNO;
		ar >> m_strFileExpress;
		ar >> m_nFileLength;
		ar >> m_strFolder;
		
	}


}


//////////////////////////////////////////////////////////////////////
// CGetConferMemberObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CGetConferMemberObj, CNodeObj, 0)

CGetConferMemberObj::CGetConferMemberObj()
{
	m_strConferNo		= "";
	m_strMemberCount	= "";
	m_strMaxMemberNum	= "";
	m_strMaxSpeakerNum	= "";
	m_strMaxSpeakingNum = "";
	m_strListenNum		= "";
	m_strSpeakerNum		= "";
	m_strSpeakingNum	= "";

}

CGetConferMemberObj::CGetConferMemberObj( CPoint point )
	: CNodeObj(point)
{
	m_strConferNo		= "";
	m_strMemberCount	= "";
	m_strMaxMemberNum	= "";
	m_strMaxSpeakerNum	= "";
	m_strMaxSpeakingNum = "";
	m_strListenNum		= "";
	m_strSpeakerNum		= "";
	m_strSpeakingNum	= "";
}
CGetConferMemberObj::CGetConferMemberObj( const CGetConferMemberObj &s )
	:CNodeObj(s)
{
	m_strConferNo		= s.m_strConferNo;
	m_strMemberCount	= s.m_strMemberCount;
	m_strMaxMemberNum	= s.m_strMaxMemberNum;
	m_strMaxSpeakerNum	= s.m_strMaxSpeakerNum;
	m_strMaxSpeakingNum = s.m_strMaxSpeakingNum;
	m_strListenNum		= s.m_strListenNum;
	m_strSpeakerNum		= s.m_strSpeakerNum;
	m_strSpeakingNum	= s.m_strSpeakingNum;

}
CGetConferMemberObj & CGetConferMemberObj::operator =( const CGetConferMemberObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strConferNo		= s.m_strConferNo;
	m_strMemberCount	= s.m_strMemberCount;
	m_strMaxMemberNum	= s.m_strMaxMemberNum;
	m_strMaxSpeakerNum	= s.m_strMaxSpeakerNum;
	m_strMaxSpeakingNum = s.m_strMaxSpeakingNum;
	m_strListenNum		= s.m_strListenNum;
	m_strSpeakerNum		= s.m_strSpeakerNum;
	m_strSpeakingNum	= s.m_strSpeakingNum;

	return *this;
	
}

CGetConferMemberObj::~CGetConferMemberObj()
{

}
void CGetConferMemberObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strConferNo;
		ar << m_strMemberCount;
		ar << m_strMaxMemberNum;
		ar << m_strMaxSpeakerNum;
		ar << m_strMaxSpeakingNum;
		ar << m_strListenNum;
		ar << m_strSpeakerNum;
		ar << m_strSpeakingNum;
	}
	else//读数据
	{
		ar >> m_strConferNo;
		ar >> m_strMemberCount;
		ar >> m_strMaxMemberNum;
		ar >> m_strMaxSpeakerNum;
		ar >> m_strMaxSpeakingNum;
		ar >> m_strListenNum;
		ar >> m_strSpeakerNum;
		ar >> m_strSpeakingNum;	
	}


}


//////////////////////////////////////////////////////////////////////
// CEnterConferObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CEnterConferObj, CNodeObj, 0)

CEnterConferObj::CEnterConferObj()
{
	m_strConferNO = "$8:ConferIndex";
	m_nAction = 0;
	m_strChnlIndex = "-1";

}

CEnterConferObj::CEnterConferObj( CPoint point )
	: CNodeObj(point)
{
	m_strConferNO = "$8:ConferIndex";
	m_nAction = 0;
	m_strChnlIndex = "-1";
}
CEnterConferObj::CEnterConferObj( const CEnterConferObj &s )
	:CNodeObj(s)
{
	m_strConferNO = s.m_strConferNO;
	m_nAction = s.m_nAction;
	m_strChnlIndex = s.m_strChnlIndex;

}
CEnterConferObj & CEnterConferObj::operator =( const CEnterConferObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strConferNO = s.m_strConferNO;
	m_nAction = s.m_nAction;
	m_strChnlIndex = s.m_strChnlIndex;

	return *this;
	
}

CEnterConferObj::~CEnterConferObj()
{

}
void CEnterConferObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strConferNO;
		ar << m_nAction;
		ar << m_strChnlIndex;

	}
	else//读数据
	{
		ar >> m_strConferNO;
		ar >> m_nAction;
		ar >> m_strChnlIndex;

		
	}


}



//////////////////////////////////////////////////////////////////////
// CLeaveConferObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLeaveConferObj, CNodeObj, 0)

CLeaveConferObj::CLeaveConferObj()
{
	m_strConferNO = "";
	m_strChnlIndex = "-1";
}

CLeaveConferObj::CLeaveConferObj( CPoint point )
	: CNodeObj(point)
{
	m_strConferNO = "";
	m_strChnlIndex = "-1";

}
CLeaveConferObj::CLeaveConferObj( const CLeaveConferObj &s )
	:CNodeObj(s)
{
	m_strConferNO = s.m_strConferNO;
	m_strChnlIndex = s.m_strChnlIndex;
}
CLeaveConferObj & CLeaveConferObj::operator =( const CLeaveConferObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strConferNO = s.m_strConferNO;
	m_strChnlIndex = s.m_strChnlIndex;

	return *this;
	
}

CLeaveConferObj::~CLeaveConferObj()
{

}
void CLeaveConferObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strConferNO;
		ar << m_strChnlIndex;

	}
	else//读数据
	{
		ar >> m_strConferNO;
		ar >> m_strChnlIndex;
		
	}


}

//////////////////////////////////////////////////////////////////////
// CDTMFSendObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDTMFSendObj, CNodeObj, 0)

CDTMFSendObj::CDTMFSendObj()
{
	m_strDTMF = "";
}

CDTMFSendObj::CDTMFSendObj( CPoint point )
	: CNodeObj(point)
{
	m_strDTMF = "";
}
CDTMFSendObj::CDTMFSendObj( const CDTMFSendObj &s )
	:CNodeObj(s)
{
	m_strDTMF = s.m_strDTMF;

}
CDTMFSendObj & CDTMFSendObj::operator =( const CDTMFSendObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strDTMF = s.m_strDTMF;


	return *this;
	
}

CDTMFSendObj::~CDTMFSendObj()
{

}
void CDTMFSendObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strDTMF;
	}
	else//读数据
	{
		ar >> m_strDTMF;
		
	}


}


//////////////////////////////////////////////////////////////////////
// CConferRequestObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CConferRequestObj, CNodeObj, 0)

CConferRequestObj::CConferRequestObj()
{
	m_nDialType			= 0;
	m_strDialTel			= "";
	m_nAction			= 0;
}

CConferRequestObj::CConferRequestObj( CPoint point )
	: CNodeObj(point)
{
	m_nDialType			= 0;
	m_strDialTel			= "";
	m_nAction			= 0;
}
CConferRequestObj::CConferRequestObj( const CConferRequestObj &s )
	:CNodeObj(s)
{
	m_nDialType		= s.m_nDialType;
	m_strDialTel		= s.m_strDialTel;
	m_nAction		= s.m_nAction;

}
CConferRequestObj & CConferRequestObj::operator =( const CConferRequestObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nDialType		= s.m_nDialType;
	m_strDialTel		= s.m_strDialTel;
	m_nAction		= s.m_nAction;


	return *this;
	
}

CConferRequestObj::~CConferRequestObj()
{

}
void CConferRequestObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nDialType;
		ar << m_strDialTel;
		ar << m_nAction;
	}
	else//读数据
	{
		ar >> m_nDialType;
		ar >> m_strDialTel;
		ar >> m_nAction;
		
	}
	
	return;

}



//////////////////////////////////////////////////////////////////////
// CBlockObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CBlockObj, CNodeObj, 0)

CBlockObj::CBlockObj()
{

}

CBlockObj::CBlockObj( CPoint point )
	: CNodeObj(point)
{

}
CBlockObj::CBlockObj( const CBlockObj &s )
	:CNodeObj(s)
{
	AssignsObjList( s.m_ChildObjLst,&m_ChildObjLst,0);
}

int CBlockObj::PasteNodeObj(CDBConnVarList* pDBLst,CFDLLVariableList *pDLLlst,CRecordsetVarList *pRSlst, CBlockObj *pBlock, int nBaseKey )
{

	POSITION pos = pBlock->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj =  pBlock->m_ChildObjLst.GetNext(pos);
		if( pObj == NULL ) continue;
		int nType = pObj->m_nType;
		
		nBaseKey = nBaseKey + 1;
		PasteReplaceNextKey( pBlock, pObj->m_nKey ,nBaseKey );
		if( NODETYPE_RETURN == nType )
		{
			//改变Block与return的对应关系
			//NODETYPE_RETURN节点的m_nBlockKey必须等于所在Block的m_nKey值
			//NODETYPE_RETURN节点根据m_nBlockKey去寻找到他属于哪个Block
			//该Block的Next节点的的m_nType必须等于该NODETYPE_RETURN节点
			//的m_nKey值，以此决定NODETYPE_RETURN节点，Block节点与Block节点的
			//Next节点的关系
			pObj->m_nBlockKey = pBlock->m_nKey;
			POSITION pos2 = pBlock->m_NextNodeList.GetHeadPosition();
			while( pos2 )
			{
				CNextNode *pNObj =  pBlock->m_NextNodeList.GetNext(pos2);
				if( pNObj == NULL ) continue;
				if( pObj->m_nKey == pNObj->m_nType )
				{
					pNObj->m_nType = nBaseKey;
				}
			}
		}
		if( NODETYPE_ENTER == nType )
		{
			pObj->m_nBlockKey = pBlock->m_nKey;
		}

		pObj->m_nKey = nBaseKey;
		
		
		if( NODETYPE_EXCUTE_SQL == nType )
		{
			
			CSQLExcuteObj *pNewSQL = (CSQLExcuteObj*)pObj;
			pNewSQL->m_strRecordset.Format("Recordset%d",nBaseKey);
			
			CRecordsetVar *pOldRset = pObj->FindRecordsetVar( pRSlst, pNewSQL->m_nRecordsetKey );
			if( pOldRset != NULL )
			{
				CRecordsetVar *pRset =  new CRecordsetVar;
				
				*pRset = *pOldRset;
				pRset->m_nKey = nBaseKey;
				pRset->m_strName.Format("Recordset%d",nBaseKey);
				pRSlst->AddTail( pRset );//将其保存
				pNewSQL->m_nRecordsetKey = nBaseKey;
				pNewSQL->m_strRecordset.Format("Recordset%d",nBaseKey);
				CString str = "";
				str.Format("@%d:%s", pRset->m_nKey, pRset->m_strName );
				PasteReplcaeDBBrowse( pBlock,pOldRset->m_nKey, nBaseKey,str);

			}
			else
			{
				AfxMessageBox("Paste Node Error");
				return nBaseKey;
			}
			

		}
		else if( NODETYPE_OPEN_DATABASE == nType )
		{
			CDBOpenObj *pDB = (CDBOpenObj*)pObj;
			if( 0 == pDB->m_nDBType )//打开普通数据库连接
			{
				CDBConnVar *pOldDBConVar = pObj->FindDBConnectVariable( pDBLst, pDB->m_nDBKey );
				if( NULL != pOldDBConVar )
				{
					CDBConnVar *pDBConVar =  new CDBConnVar;
					*pDBConVar = *pOldDBConVar;
					pDBConVar->m_nKey = nBaseKey;
					pDBConVar->m_nVarDataType = DATATYPE_DBCONNECT;
					pDBConVar->m_strName.Format("DataBase%d",nBaseKey);
					pDBLst->AddTail( pDBConVar );
					pDB->m_nDBKey = nBaseKey;
					pDB->m_strDBName.Format("DataBase%d",nBaseKey);
				}
				else
				{
					AfxMessageBox("Paste Node Error");
					return nBaseKey;
				}
			}
			
		}
		else if( NODETYPE_STOREPRO == nType )
		{
			CStoreProObj * pNewSproObj = ( CStoreProObj *)pObj;
			pNewSproObj->m_strRecordset.Format("Recordset%d",nBaseKey);
			CRecordsetVar *pOldRset = pObj->FindRecordsetVar( pRSlst, pNewSproObj->m_nRecordsetKey );
			if( pOldRset != NULL )
			{
				CRecordsetVar *pRset =  new CRecordsetVar;
				
				*pRset = *pOldRset;
				pRset->m_nKey = nBaseKey;
				pRset->m_strName.Format("Recordset%d",nBaseKey);
				pRSlst->AddTail( pRset );//将其保存
				pNewSproObj->m_strRecordset.Format("Recordset%d",nBaseKey);
				pNewSproObj->m_nRecordsetKey = nBaseKey;
				CString str = "";
				str.Format("@%d:%s", pRset->m_nKey, pRset->m_strName );
				PasteReplcaeDBBrowse( pBlock,pOldRset->m_nKey, nBaseKey,str);

			}
			else
			{
				AfxMessageBox("Paste Node Error");
				return nBaseKey;
			}

			
		}
		else if( NODETYPE_DLL_LOAD == nType )
		{
			CDLLLoadObj * pNewDLLObj =  ( CDLLLoadObj *)pObj;
			if( 0 == pNewDLLObj->m_nHandleType )//普通DLL
			{
				CDLLVariable *pOldDLLVar = pObj->FindDLLVariable( pDLLlst, pNewDLLObj->m_nDLLKey );
				if( pOldDLLVar != NULL )
				{
					CDLLVariable *pDLLVar =  new CDLLVariable;
					*pDLLVar = *pOldDLLVar;

					pDLLVar->m_nKey = nBaseKey;
					pDLLVar->m_nVarDataType = DATATYPE_DLL;
					pDLLVar->m_strName.Format("DLL%d",nBaseKey);
					pDLLlst->AddTail( pDLLVar );//将其保存
					pNewDLLObj->m_nDLLKey = nBaseKey;
					pNewDLLObj->m_strDescription.Format("DLL%d",nBaseKey);
				}
				else
				{
					AfxMessageBox("Paste Node Error");
					return nBaseKey;
				}
				
			}
		}
		else if( NODETYPE_BLOCK == nType )
		{
			
			CBlockObj *pNewBlock = (CBlockObj *)pObj;
			nBaseKey = PasteNodeObj(pDBLst,  pDLLlst, pRSlst, pNewBlock, nBaseKey );
		}
	}
	return nBaseKey;
}

 void CBlockObj::PasteReplaceNextKey( CBlockObj *pBlock,int nNodeKey , int nNewNodeKey )
{

	POSITION pos = pBlock->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj =  pBlock->m_ChildObjLst.GetNext(pos);
		if( pObj == NULL ) continue;

		POSITION pos2 = pObj->m_NextNodeList.GetHeadPosition();
		while( pos2 )
		{
			CNextNode *pNObj =  pObj->m_NextNodeList.GetNext(pos2);
			if( pNObj == NULL ) continue;
			
			pNObj->m_LineObj.m_BeginPoint = pNObj->GetLinePoint();
			if( nNodeKey == pNObj->m_nNodeObjKey )
			{
				pNObj->m_nNodeObjKey = nNewNodeKey;
			}
		}
        
		int nType = pObj->m_nType;
		if( NODETYPE_BLOCK == nType )
		{
			CBlockObj *pNewBlock = (CBlockObj *)pObj;
			PasteReplaceNextKey( pNewBlock, nNodeKey ,nNewNodeKey );
		}
	}
	return ;

}

CBlockObj & CBlockObj::operator =( const CBlockObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	AssignsObjList( s.m_ChildObjLst,&m_ChildObjLst,0);

	return *this;
	
}

CBlockObj::~CBlockObj()
{
	while ( m_ChildObjLst.GetHeadPosition() )
	{
		delete m_ChildObjLst.RemoveHead();
	}
}
void CBlockObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		m_ChildObjLst.Serialize(ar);
	}
	else//读数据
	{

		m_ChildObjLst.Serialize(ar);
	}
	
	return;

}
CNodeObj * CBlockObj::FindNodeObj(int nKey, int &nParentKey)
{
	CNodeObjList *pObjLst = &(m_ChildObjLst);
	POSITION pos = pObjLst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = pObjLst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( nKey == pObj->m_nKey )
		{
			nParentKey = m_nKey;
			return pObj;
		}
		else
		{
			if(  NODETYPE_BLOCK == pObj->m_nType )
			{
				CBlockObj *pBlockObj = (CBlockObj *)pObj;
				CNodeObj *pObj1 = pBlockObj->FindNodeObj( nKey,nParentKey);//递归调用
				if( NULL != pObj1 )
				{
					//nParentKey = pBlockObj->m_nKey;
					return pObj1;
				}
			}
		}

	}

	return NULL;
}
/////////////////////////////////////
//查找属于Block的节点
//
//////////////////////////////////////
CNodeObj * CBlockObj::FindObj(int nKey)
{
	CNodeObjList *pObjLst = &(m_ChildObjLst);
	POSITION pos = pObjLst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = pObjLst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( nKey == pObj->m_nKey )
		{
			return pObj;
		}
		else
		{
			if(  NODETYPE_BLOCK == pObj->m_nType )
			{
				CBlockObj *pBlockObj = (CBlockObj *)pObj;
				CNodeObj *pObj1 = pBlockObj->FindObj( nKey);//递归调用
				if( NULL != pObj1 )
					return pObj1;
			}
		}

	}

	return NULL;
}



//////////////////////////////////////////////////////////////////////
// CStoreProObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CStoreProObj, CNodeObj, 0)

CStoreProObj::CStoreProObj()
{
	m_strStoreProName	= "";
	m_strParameterVar = "";
	m_strRecordset		= "";

	m_nRecordsetKey = -1;
	m_nDBKey = -1;
	m_nDBType = 0;
	m_strDB = "";
}

CStoreProObj::CStoreProObj( CPoint point )
	: CNodeObj(point)
{
	m_strStoreProName	= "";
	m_strParameterVar = "";
	m_strRecordset		= "";

	m_strReturn	= "";
	m_bReturnRecordSet = FALSE;

	m_nRecordsetKey = -1;
	m_nDBKey = -1;
	m_nDBType = 0;
	m_strDB = "";

}
CStoreProObj::CStoreProObj( const CStoreProObj &s )
	:CNodeObj(s)
{
	m_strStoreProName	= s.m_strStoreProName;
	m_strParameterVar = s.m_strParameterVar;
	m_strRecordset		= s.m_strRecordset;
	m_strReturn	 = s.m_strReturn;
	m_bReturnRecordSet  = s.m_bReturnRecordSet;
	m_nRecordsetKey = s.m_nRecordsetKey;
	m_nDBKey = s.m_nDBKey;
	m_nDBType = s.m_nDBType;
	m_strDB = s.m_strDB;
	
}
CStoreProObj & CStoreProObj::operator =( const CStoreProObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strStoreProName	= s.m_strStoreProName;
	m_strParameterVar = s.m_strParameterVar;
	m_strRecordset		= s.m_strRecordset;
	m_strReturn	 = s.m_strReturn;
	m_bReturnRecordSet  = s.m_bReturnRecordSet;
	m_nRecordsetKey = s.m_nRecordsetKey;
	m_nDBKey = s.m_nDBKey;
	m_nDBType = s.m_nDBType;
	m_strDB = s.m_strDB;

	return *this;
	
}

CStoreProObj::~CStoreProObj()
{

}
void CStoreProObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strStoreProName;
		ar << m_strParameterVar;
		ar << m_strRecordset;
		ar << m_strReturn;
		ar << m_bReturnRecordSet;

		ar << m_nRecordsetKey;
		ar << m_nDBKey;
		ar << m_nDBType;
		ar << m_strDB;

	}
	else//读数据
	{

		ar >> m_strStoreProName;
		ar >> m_strParameterVar;
		ar >> m_strRecordset;
		ar >> m_strReturn;
		ar >> m_bReturnRecordSet;
		ar >> m_nRecordsetKey;
		ar >> m_nDBKey;
		ar >> m_nDBType;
		ar >> m_strDB;
	}


}



//////////////////////////////////////////////////////////////////////
// CChangeVoiceObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CChangeVoiceObj, CNodeObj, 0)
CChangeVoiceObj::CChangeVoiceObj()
{
	m_nChangeVoiceType = 0;
}

CChangeVoiceObj::CChangeVoiceObj( CPoint point )
	: CNodeObj(point)
{
	m_nChangeVoiceType = 0;

}
CChangeVoiceObj::CChangeVoiceObj( const CChangeVoiceObj &s )
	:CNodeObj(s)
{
	m_nChangeVoiceType	= s.m_nChangeVoiceType;
	
}
CChangeVoiceObj & CChangeVoiceObj::operator =( const CChangeVoiceObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nChangeVoiceType	= s.m_nChangeVoiceType;

	return *this;
	
}

CChangeVoiceObj::~CChangeVoiceObj()
{

}
void CChangeVoiceObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nChangeVoiceType;

	}
	else//读数据
	{

		ar >> m_nChangeVoiceType;
		
	}


}


//////////////////////////////////////////////////////////////////////
// CEndCallBlockObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CEndCallBlockObj, CNodeObj, 0)

CEndCallBlockObj::CEndCallBlockObj()
{
	m_nType = NODETYPE_ENDCALL_BLOCK;
	m_strCaption = "EndCallBlock";
}

CEndCallBlockObj::CEndCallBlockObj( CPoint point )
	: CNodeObj(point)
{
	m_nType = NODETYPE_ENDCALL_BLOCK;
	m_strCaption = "EndCallBlock";
}
CEndCallBlockObj::CEndCallBlockObj( const CEndCallBlockObj &s )
	:CNodeObj(s)
{
	
	POSITION pos = s.m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = s.m_ChildObjLst.GetNext(pos);
		if( pObj == NULL ) continue;
		int nType = pObj->m_nType;
		if( NODETYPE_OPEN_DATABASE == nType )
		{
			CDBOpenObj * pNewObj = new CDBOpenObj;
			*pNewObj = *(CDBOpenObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_CLOSE_DATABASE == nType )
		{
			CDBCloseObj * pNewObj = new CDBCloseObj;
			*pNewObj = *(CDBCloseObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_EXCUTE_SQL == nType )
		{
			CSQLExcuteObj * pNewObj = new CSQLExcuteObj;
			*pNewObj = *(CSQLExcuteObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_LOAD == nType )
		{
			CDLLLoadObj * pNewObj = new CDLLLoadObj;
			*pNewObj = *(CDLLLoadObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_UNLOAD == nType )
		{
			CDLLUnLoadObj * pNewObj = new CDLLUnLoadObj;
			*pNewObj = *(CDLLUnLoadObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_CALL == nType )
		{
			CDLLCallObj * pNewObj = new CDLLCallObj;
			*pNewObj = *(CDLLCallObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_DATABASE_BROWSE == nType )
		{
			CDBBrowseObj * pNewObj = new CDBBrowseObj;
			*pNewObj = *(CDBBrowseObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_COMPARE  == nType )
		{
			CCompareObj * pNewObj = new CCompareObj;
			*pNewObj = *(CCompareObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_STOREPRO == pObj->m_nType )
		{
			CStoreProObj * pNewObj =  new CStoreProObj;
			*pNewObj = *(CStoreProObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if(  NODETYPE_ENDCALLEVENT_CHECK == pObj->m_nType )
		{
			CNodeObj * pNewObj = new CNodeObj;
			*pNewObj = *pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if(  NODETYPE_ENDCALLEVENT_END == pObj->m_nType )
		{
			CNodeObj * pNewObj = new CNodeObj;
			*pNewObj = *pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_EXPRESS == pObj->m_nType )
		{
			CExpressObj * pNewObj =  new CExpressObj;
			*pNewObj = *(CExpressObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_FUNCTION == pObj->m_nType )
		{
			CFunctionObj * pNewObj =  new CFunctionObj;
			*pNewObj = *(CFunctionObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_GOTO == pObj->m_nType )
		{
			CGoToObj * pNewObj =  new CGoToObj;
			*pNewObj = *(CGoToObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_TRACE_INFO == pObj->m_nType )
		{
			CTraceObj * pNewObj =  new CTraceObj;
			*pNewObj = *(CTraceObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_MOUDLE == pObj->m_nType )
		{
			CModuleObj * pNewObj =  new CModuleObj;
			*pNewObj = *(CModuleObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_CUSTOM == pObj->m_nType )
		{
			CCustomObj * pNewObj =  new CCustomObj;
			*pNewObj = *(CCustomObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else 
		{
			CNodeObj * pNewObj =  new CNodeObj;
			*pNewObj = *(CNodeObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
	}

}
CEndCallBlockObj & CEndCallBlockObj::operator =( const CEndCallBlockObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);


	POSITION pos = s.m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = s.m_ChildObjLst.GetNext(pos);
		if( pObj == NULL ) continue;
		int nType = pObj->m_nType;
		if( NODETYPE_OPEN_DATABASE == nType )
		{
			CDBOpenObj * pNewObj = new CDBOpenObj;
			*pNewObj = *(CDBOpenObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_CLOSE_DATABASE == nType )
		{
			CDBCloseObj * pNewObj = new CDBCloseObj;
			*pNewObj = *(CDBCloseObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_EXCUTE_SQL == nType )
		{
			CSQLExcuteObj * pNewObj = new CSQLExcuteObj;
			*pNewObj = *(CSQLExcuteObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_LOAD == nType )
		{
			CDLLLoadObj * pNewObj = new CDLLLoadObj;
			*pNewObj = *(CDLLLoadObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_UNLOAD == nType )
		{
			CDLLUnLoadObj * pNewObj = new CDLLUnLoadObj;
			*pNewObj = *(CDLLUnLoadObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_CALL == nType )
		{
			CDLLCallObj * pNewObj = new CDLLCallObj;
			*pNewObj = *(CDLLCallObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_DATABASE_BROWSE == nType )
		{
			CDBBrowseObj * pNewObj = new CDBBrowseObj;
			*pNewObj = *(CDBBrowseObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_COMPARE  == nType )
		{
			CCompareObj * pNewObj = new CCompareObj;
			*pNewObj = *(CCompareObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_STOREPRO == pObj->m_nType )
		{
			CStoreProObj * pNewObj =  new CStoreProObj;
			*pNewObj = *(CStoreProObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if(  NODETYPE_ENDCALLEVENT_CHECK == pObj->m_nType )
		{
			CNodeObj * pNewObj = new CNodeObj;
			*pNewObj = *pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if(  NODETYPE_ENDCALLEVENT_END == pObj->m_nType )
		{
			CNodeObj * pNewObj = new CNodeObj;
			*pNewObj = *pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_EXPRESS == pObj->m_nType )
		{
			CExpressObj * pNewObj =  new CExpressObj;
			*pNewObj = *(CExpressObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_FUNCTION == pObj->m_nType )
		{
			CFunctionObj * pNewObj =  new CFunctionObj;
			*pNewObj = *(CFunctionObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_GOTO == pObj->m_nType )
		{
			CGoToObj * pNewObj =  new CGoToObj;
			*pNewObj = *(CGoToObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_TRACE_INFO == pObj->m_nType )
		{
			CTraceObj * pNewObj =  new CTraceObj;
			*pNewObj = *(CTraceObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_MOUDLE == pObj->m_nType )
		{
			CModuleObj * pNewObj =  new CModuleObj;
			*pNewObj = *(CModuleObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_CUSTOM == pObj->m_nType )
		{
			CCustomObj * pNewObj =  new CCustomObj;
			*pNewObj = *(CCustomObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		else if( NODETYPE_EVENT_CHECK == pObj->m_nType )
		{
			CEventCheckObj * pNewObj =  new CEventCheckObj;
			*pNewObj = *(CEventCheckObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
		
		else 
		{
			CNodeObj * pNewObj =  new CNodeObj;
			*pNewObj = *(CNodeObj *)pObj;
			m_ChildObjLst.AddTail(pNewObj);
		}
	}

	return *this;
	
}

CEndCallBlockObj::~CEndCallBlockObj()
{
	while ( m_ChildObjLst.GetHeadPosition() )
	{
		delete m_ChildObjLst.RemoveHead();
	}
}
void CEndCallBlockObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		m_ChildObjLst.Serialize(ar);
	}
	else//读数据
	{

		m_ChildObjLst.Serialize(ar);
	}
	
	return;

}
CNodeObj * CEndCallBlockObj::FindNodeObj(int nKey, int &nParentKey)
{
	CNodeObjList *pObjLst = &(m_ChildObjLst);
	POSITION pos = pObjLst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = pObjLst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( nKey == pObj->m_nKey )
		{
			nParentKey = m_nKey;
			return pObj;
		}

	}

	return NULL;
}
/////////////////////////////////////
//查找属于Block的节点
//
//////////////////////////////////////
CNodeObj * CEndCallBlockObj::FindObj(int nKey)
{
	CNodeObjList *pObjLst = &(m_ChildObjLst);
	POSITION pos = pObjLst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = pObjLst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( nKey == pObj->m_nKey )
		{
			return pObj;
		}

	}

	return NULL;
}

CNodeObj * CEndCallBlockObj::GetEndCallEventCheckObj()
{
	CNodeObjList *pObjLst = &(m_ChildObjLst);
	POSITION pos = pObjLst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = pObjLst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( NODETYPE_ENDCALLEVENT_CHECK == pObj->m_nType )
		{
			return pObj;
		}

	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CFunctionObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFunctionObj, CNodeObj, 0)

CFunctionObj::CFunctionObj()
{
	m_strFunction = "";
	m_strReturnValues = "";
}

CFunctionObj::CFunctionObj( CPoint point )
	: CNodeObj(point)
{
	m_strFunction = "";
	m_strReturnValues = "";
}
CFunctionObj::CFunctionObj( const CFunctionObj &s )
	:CNodeObj(s)
{
	m_strFunction = s.m_strFunction;
	m_strReturnValues = s.m_strReturnValues;
}
CFunctionObj & CFunctionObj::operator =( const CFunctionObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);
	m_strFunction = s.m_strFunction;
	m_strReturnValues = s.m_strReturnValues;

	return *this;
	
}

CFunctionObj::~CFunctionObj()
{

}
void CFunctionObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strFunction;
		ar << m_strReturnValues;
	}
	else//读数据
	{
		ar >> m_strFunction;
		ar >> m_strReturnValues;
	}


}


//////////////////////////////////////////////////////////////////////
// CExpressObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CExpressObj, CNodeObj, 0)

CExpressObj::CExpressObj()
{
	m_strExpressValues = "";
	m_strExpress = "";
}

CExpressObj::CExpressObj( CPoint point )
	: CNodeObj(point)
{
	m_strExpressValues = "";
	m_strExpress = "";

}
CExpressObj::CExpressObj( const CExpressObj &s )
	:CNodeObj(s)
{
	m_strExpressValues = s.m_strExpressValues;
	m_strExpress = s.m_strExpress;
}
CExpressObj & CExpressObj::operator =( const CExpressObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strExpressValues = s.m_strExpressValues;
	m_strExpress = s.m_strExpress;

	return *this;
	
}

CExpressObj::~CExpressObj()
{

}
void CExpressObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strExpressValues;
		ar << m_strExpress;

	}
	else//读数据
	{
		ar >> m_strExpressValues;
		ar >> m_strExpress;
		
	}


}

//////////////////////////////////////////////////////////////////////
// CGoToObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CGoToObj, CNodeObj, 0)
CGoToObj::CGoToObj()
{
	m_nNextObjKey = -1;
	m_strConnPath = "";
}

CGoToObj::CGoToObj( CPoint point )
	: CNodeObj(point)
{
	m_nNextObjKey = -1;
	m_strConnPath = "";
}
CGoToObj::CGoToObj( const CGoToObj &s )
	:CNodeObj(s)
{
	m_nNextObjKey = s.m_nNextObjKey;
	m_strConnPath = s.m_strConnPath;
}
CGoToObj & CGoToObj::operator =( const CGoToObj &s )
{
	if(this == &s)
		return *this;
	CNodeObj::operator = (s);
	m_nNextObjKey = s.m_nNextObjKey;
	m_strConnPath = s.m_strConnPath;
	return *this;
	
}

CGoToObj::~CGoToObj()
{

}
void CGoToObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nNextObjKey;
		ar << m_strConnPath;
	}
	else//读数据
	{
		ar >> m_nNextObjKey;
		ar >> m_strConnPath;
	}
}



void CGoToObj::DisConnectToNode(int nKey)
{
	if( nKey == m_nNextObjKey )
	{
		m_nNextObjKey = -1;;
		m_strConnPath = m_strCaption + "->";
	}

	return;
}

CRecordsetVar * CNodeObj::FindRecordsetVar(CRecordsetVarList *pRSlst, int nKey)
{
	if( NULL == pRSlst ) return NULL;
	POSITION pos = pRSlst->GetHeadPosition();
	while(pos)
	{
		CRecordsetVar * pVar = pRSlst->GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey ) return pVar;
		}
	}

	return NULL;
}

CDLLVariable * CNodeObj::FindDLLVariable(CFDLLVariableList *pDLLlst, int nKey)
{
	if( NULL == pDLLlst ) return NULL;
	POSITION pos = pDLLlst->GetHeadPosition();
	while(pos)
	{
		CDLLVariable * pVar = pDLLlst->GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey ) return pVar;
		}
	}

	return NULL;
}

CDBConnVar * CNodeObj::FindDBConnectVariable(CDBConnVarList *pDBLst, int nKey)
{
	if( NULL == pDBLst ) return NULL;
	POSITION pos = pDBLst->GetHeadPosition();
	while(pos)
	{
		CDBConnVar * pVar = pDBLst->GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey ) return pVar;
		}
	}

	return NULL;
}

void CBlockObj::PasteReplcaeDBBrowse(CBlockObj *pBlock, int nRSKey, int nNewRSKey,CString strNewRecordset )
{
	POSITION pos = pBlock->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj =  pBlock->m_ChildObjLst.GetNext(pos);
		if( pObj == NULL ) continue;
		if( NODETYPE_DATABASE_BROWSE == pObj->m_nType )
		{
			CDBBrowseObj *pDBObj = (CDBBrowseObj *)pObj;
			if( nRSKey == pDBObj->m_nRecordsetKey )
			{
				pDBObj->m_nRecordsetKey = nNewRSKey;
				pDBObj->m_strRecordset= strNewRecordset;
				
				return;
			}

		}

	}
	return;
}
//////////////////////////////////////////////////////////////////////
// CTraceObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CTraceObj, CNodeObj, 0)

CTraceObj::CTraceObj()
{
	m_strTrace = "";
}

CTraceObj::CTraceObj( CPoint point )
	: CNodeObj(point)
{
	m_strTrace = "";

}
CTraceObj::CTraceObj( const CTraceObj &s )
	:CNodeObj(s)
{
	m_strTrace = s.m_strTrace;

}
CTraceObj & CTraceObj::operator =( const CTraceObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_strTrace = s.m_strTrace;

	return *this;
	
}

CTraceObj::~CTraceObj()
{

}
void CTraceObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strTrace;

	}
	else//读数据
	{
		ar >> m_strTrace;

	}
}

//////////////////////////////////////////////////////////////////////
// CModuleObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CModuleObj, CNodeObj, 0)

CModuleObj::CModuleObj()
{
	m_strFile = "";
	m_pCurObj= NULL;
}

CModuleObj::CModuleObj( CPoint point )
	: CNodeObj(point)
{
	m_strFile = "";
	m_pCurObj= NULL;
}
CModuleObj::CModuleObj( const CModuleObj &s )
	:CNodeObj(s)
{
	m_strFile = s.m_strFile;
	m_pCurObj = s.m_pCurObj;
	POSITION pos = s.m_ParameterLst.GetHeadPosition();
	while( pos )
	{
		CParameter *pObj = s.m_ParameterLst.GetNext(pos);
		if( pObj != NULL )
		{
			CParameter *pNewObj = new CParameter;
			*pNewObj = *pObj;
			m_ParameterLst.AddTail( pNewObj );
		}
	}
	pos = s.m_DBConnVarList.GetHeadPosition();
	while (pos)
	{
		CDBConnVar * pNewObj = new CDBConnVar;
		*pNewObj = *( s.m_DBConnVarList.GetNext(pos) );
		m_DBConnVarList.AddTail(pNewObj);
	}

	pos = s.m_DLLVariableList.GetHeadPosition();
	while (pos)
	{
		CDLLVariable * pNewObj = new CDLLVariable;
		*pNewObj = *( s.m_DLLVariableList.GetNext(pos) );
		m_DLLVariableList.AddTail(pNewObj);
	}

	pos = s.m_NormalVarList.GetHeadPosition();
	while (pos)
	{
		CNormalVar * pNewObj = new CNormalVar;
		*pNewObj = *( s.m_NormalVarList.GetNext(pos) );
		m_NormalVarList.AddTail(pNewObj);
	}

	pos = s.m_RecordsetVariList.GetHeadPosition();
	while (pos)
	{
		CRecordsetVar * pNewObj = new CRecordsetVar;
		*pNewObj = *( s.m_RecordsetVariList.GetNext(pos) );
		m_RecordsetVariList.AddTail(pNewObj);
	}
	
	AssignsObjList( s.m_objects,&m_objects,0);
	
}
CModuleObj & CModuleObj::operator =( const CModuleObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);
	m_pCurObj = s.m_pCurObj;
	m_strFile = s.m_strFile;

	POSITION pos = s.m_ParameterLst.GetHeadPosition();
	while( pos )
	{
		CParameter *pObj = s.m_ParameterLst.GetNext(pos);
		if( pObj != NULL )
		{
			CParameter *pNewObj = new CParameter;
			*pNewObj = *pObj;
			m_ParameterLst.AddTail( pNewObj );
		}
	}

	pos = s.m_DBConnVarList.GetHeadPosition();
	while (pos)
	{
		CDBConnVar * pNewObj = new CDBConnVar;
		*pNewObj = *( s.m_DBConnVarList.GetNext(pos) );
		m_DBConnVarList.AddTail(pNewObj);
	}

	pos = s.m_DLLVariableList.GetHeadPosition();
	while (pos)
	{
		CDLLVariable * pNewObj = new CDLLVariable;
		*pNewObj = *( s.m_DLLVariableList.GetNext(pos) );
		m_DLLVariableList.AddTail(pNewObj);
	}

	pos = s.m_NormalVarList.GetHeadPosition();
	while (pos)
	{
		CNormalVar * pNewObj = new CNormalVar;
		*pNewObj = *( s.m_NormalVarList.GetNext(pos) );
		m_NormalVarList.AddTail(pNewObj);
	}

	pos = s.m_RecordsetVariList.GetHeadPosition();
	while (pos)
	{
		CRecordsetVar * pNewObj = new CRecordsetVar;
		*pNewObj = *( s.m_RecordsetVariList.GetNext(pos) );
		m_RecordsetVariList.AddTail(pNewObj);
	}

	AssignsObjList( s.m_objects, &m_objects,0);


	return *this;
	
}

CModuleObj::~CModuleObj()
{

	while( m_ParameterLst.GetHeadPosition() )
	{
		delete m_ParameterLst.RemoveHead();
	}
	while( m_objects.GetHeadPosition() )
	{
		delete m_objects.RemoveHead();
	}

	while( m_DBConnVarList.GetHeadPosition() )
	{
		delete m_DBConnVarList.RemoveHead();
	}

	while( m_RecordsetVariList.GetHeadPosition() )
	{
		delete m_RecordsetVariList.RemoveHead();
	}

	while( m_NormalVarList.GetHeadPosition() )
	{
		delete m_NormalVarList.RemoveHead();
	}

	while( m_DLLVariableList.GetHeadPosition() )
	{
		delete m_DLLVariableList.RemoveHead();
	}

}
void CModuleObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_strFile;
		m_objects.Serialize(ar);
		m_RecordsetVariList.Serialize(ar);
		m_DBConnVarList.Serialize(ar);
		m_NormalVarList.Serialize(ar);
		m_DLLVariableList.Serialize(ar);
		m_ParameterLst.Serialize(ar);
	}
	else//读数据
	{
		ar >> m_strFile;
		m_objects.Serialize(ar);
		m_RecordsetVariList.Serialize(ar);
		m_DBConnVarList.Serialize(ar);
		m_NormalVarList.Serialize(ar);
		m_DLLVariableList.Serialize(ar);
		m_ParameterLst.Serialize(ar);
	}
}

void CModuleObj::RemoveParameterList()
{
	while( m_ParameterLst.GetHeadPosition() )
	{
		delete m_ParameterLst.RemoveHead();
	}
	return;
}

void CModuleObj::UpdateParameterList(CParameterList *p)
{
	POSITION pos = p->GetHeadPosition();
	while( pos )
	{
		CParameter *pObj = p->GetNext(pos);
		if( pObj != NULL )
		{
			CParameter *pNewObj = new CParameter;
			*pNewObj = *pObj;
			m_ParameterLst.AddTail( pNewObj );
		}
	}

	return;
}

BOOL CCFDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (IsModified())
		TRACE0("Warning: Open Document replaces an unsaved document.\n");
	CString strError("");
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		strError.Format("it is error that open the %s",lpszPathName);
		AfxMessageBox(strError);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		//DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CModuleDocument

IMPLEMENT_DYNCREATE(CModuleDocument, CDocument)

CModuleDocument::CModuleDocument()
{
}

BOOL CModuleDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CModuleDocument::~CModuleDocument()
{
}


BEGIN_MESSAGE_MAP(CModuleDocument, CDocument)
	//{{AFX_MSG_MAP(CModuleDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleDocument diagnostics

#ifdef _DEBUG
void CModuleDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CModuleDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModuleDocument serialization

void CModuleDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_ModuleObj.Serialize(ar);
	}
	else
	{
		m_ModuleObj.Serialize(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CModuleDocument commands

BOOL CModuleDocument::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (IsModified())
		TRACE0("Warning: Open Document replaces an unsaved document.\n");
	CString strError("");
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		strError.Format("it is error that open the %s",lpszPathName);
		AfxMessageBox(strError);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		//DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}

//nAssigns 0:普通 1编译
extern "C" _declspec(dllexport) void AssignsObjList( const CNodeObjList &s ,CNodeObjList *d,int nAssigns )
{

	POSITION  pos = s.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = s.GetNext(pos);
		if( pObj == NULL ) continue;

		if( 1 ==nAssigns )//编译
		{
			pObj->m_strControlMsg = "";
			pObj->m_strDescription = "";
		}
		int nType = pObj->m_nType;
		if( NODETYPE_WAITCALL == nType )
		{
			CWaitCallObj * pNewObj = new CWaitCallObj;
			*pNewObj = *(CWaitCallObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_START_PLAY == nType )
		{
			CPlayStartObj * pNewObj = new CPlayStartObj;
			*pNewObj = *(CPlayStartObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CHECK_DIAL == nType )
		{
			CDialCheckObj * pNewObj = new CDialCheckObj;
			*pNewObj = *(CDialCheckObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_RELEASECALL == nType )
		{
			CReleaseCallObj * pNewObj = new CReleaseCallObj;
			*pNewObj = *(CReleaseCallObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CALLHOLD == nType )
		{
			CCallHoldObj * pNewObj = new CCallHoldObj;
			*pNewObj = *(CCallHoldObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_START_RECORD == pObj->m_nType )
		{
			CRecordObj * pNewObj = new CRecordObj;
			*pNewObj = *(CRecordObj *)pObj;
			d->AddTail(pNewObj);

		}
		else if( NODETYPE_CHECK_TRANSFER == nType )
		{
			CTransferCheckObj * pNewObj = new CTransferCheckObj;
			*pNewObj = *(CTransferCheckObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_OPEN_DATABASE == nType )
		{
			CDBOpenObj * pNewObj = new CDBOpenObj;
			*pNewObj = *(CDBOpenObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CLOSE_DATABASE == nType )
		{
			CDBCloseObj * pNewObj = new CDBCloseObj;
			*pNewObj = *(CDBCloseObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_EXCUTE_SQL == nType )
		{
			CSQLExcuteObj * pNewObj = new CSQLExcuteObj;
			*pNewObj = *(CSQLExcuteObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_LOAD == nType )
		{
			CDLLLoadObj * pNewObj = new CDLLLoadObj;
			*pNewObj = *(CDLLLoadObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_UNLOAD == nType )
		{
			CDLLUnLoadObj * pNewObj = new CDLLUnLoadObj;
			*pNewObj = *(CDLLUnLoadObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_CALL == nType )
		{
			CDLLCallObj * pNewObj = new CDLLCallObj;
			*pNewObj = *(CDLLCallObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_RECIVE_DTMF == nType )
		{
			CDTMFReciveObj * pNewObj = new CDTMFReciveObj;
			*pNewObj = *(CDTMFReciveObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_SEND_DTMFCODE == pObj->m_nType )
		{
			CDTMFSendObj * pNewObj = new CDTMFSendObj;
			*pNewObj = *(CDTMFSendObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DTMF_MENU == nType )
		{
			CDTMFMenuObj * pNewObj = new CDTMFMenuObj;
			*pNewObj = *(CDTMFMenuObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_EVENT_CHECK == nType )
		{
			CEventCheckObj * pNewObj = new CEventCheckObj;
			*pNewObj = *(CEventCheckObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DATABASE_BROWSE == nType )
		{
			CDBBrowseObj * pNewObj = new CDBBrowseObj;
			*pNewObj = *(CDBBrowseObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CALLLOGREQUEST == nType )
		{
			CCallLogRequestObj * pNewObj = new CCallLogRequestObj;
			*pNewObj = *(CCallLogRequestObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_SWITCH  == nType )
		{
			CSwitchObj * pNewObj = new CSwitchObj;
			*pNewObj = *(CSwitchObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_COMPARE  == nType )
		{
			CCompareObj * pNewObj = new CCompareObj;
			*pNewObj = *(CCompareObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_ENTER_CONFERENCE == pObj->m_nType )
		{
			CEnterConferObj * pNewObj = new CEnterConferObj;
			*pNewObj = *(CEnterConferObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_LEAVE_CONFERENCE == pObj->m_nType )
		{
			CLeaveConferObj * pNewObj = new CLeaveConferObj;
			*pNewObj = *(CLeaveConferObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_FAXRECIVE == pObj->m_nType )
		{
			CFaxReciveObj * pNewObj =  new CFaxReciveObj;
			*pNewObj = *(CFaxReciveObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_FAXSEND == pObj->m_nType )
		{
			CFaxSendObj * pNewObj =  new CFaxSendObj;
			*pNewObj = *(CFaxSendObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_CONFERENCE_BUILD == pObj->m_nType )
		{
			CConferenceBulidObj * pNewObj =  new CConferenceBulidObj;
			*pNewObj = *(CConferenceBulidObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_CONFERENCE_DELETE == pObj->m_nType )
		{
			CConferenceDeleteObj * pNewObj =  new CConferenceDeleteObj;
			*pNewObj = *(CConferenceDeleteObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_GET_CONFERENCE_MEMBER == pObj->m_nType )
		{
			CGetConferMemberObj * pNewObj =  new CGetConferMemberObj;
			*pNewObj = *(CGetConferMemberObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_CONFERENCE_REQUEST == pObj->m_nType )
		{
			CConferRequestObj * pNewObj =  new CConferRequestObj;
			*pNewObj = *(CConferRequestObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_BLOCK == pObj->m_nType )
		{
			CBlockObj * pNewObj =  new CBlockObj;
			*pNewObj = *(CBlockObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_STOREPRO == pObj->m_nType )
		{
			CStoreProObj * pNewObj =  new CStoreProObj;
			*pNewObj = *(CStoreProObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CHANGEVOICE == pObj->m_nType )
		{
			CChangeVoiceObj * pNewObj =  new CChangeVoiceObj;
			*pNewObj = *(CChangeVoiceObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
		{
			CEndCallBlockObj * pNewObj =  new CEndCallBlockObj;
			*pNewObj = *(CEndCallBlockObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_EXPRESS == pObj->m_nType )
		{
			CExpressObj * pNewObj =  new CExpressObj;
			*pNewObj = *(CExpressObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_FUNCTION == pObj->m_nType )
		{
			CFunctionObj * pNewObj =  new CFunctionObj;
			*pNewObj = *(CFunctionObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_GOTO == pObj->m_nType )
		{
			CGoToObj * pNewObj =  new CGoToObj;
			*pNewObj = *(CGoToObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_TRACE_INFO == pObj->m_nType )
		{
			CTraceObj * pNewObj =  new CTraceObj;
			*pNewObj = *(CTraceObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_MOUDLE == pObj->m_nType )
		{
			if( 0 == nAssigns )//0:普通 
			{
				CModuleObj * pNewObj =  new CModuleObj;
				*pNewObj = *(CModuleObj *)pObj;
				d->AddTail(pNewObj);
			}
			else//1编译
			{
				CModuleDocument doc;
				CModuleObj *pMdl = new CModuleObj;
				*pMdl = *(CModuleObj *)pObj;
				if( !doc.OnOpenDocument( pMdl->m_strFile ) )
				{
					CString strError("");
					strError.Format("It is error that open %s",pMdl->m_strFile);
					AfxMessageBox(strError);
					return;
				}
				else
				{
					CModuleObj s = doc.m_ModuleObj;
					AssignsObjList( s.m_objects, &(pMdl->m_objects),0 );


					POSITION  pos = s.m_DBConnVarList.GetHeadPosition();
					while (pos)
					{
						CDBConnVar * pNewObj = new CDBConnVar;
						*pNewObj = *( s.m_DBConnVarList.GetNext(pos) );
						pMdl->m_DBConnVarList.AddTail(pNewObj);
					}

					pos = s.m_DLLVariableList.GetHeadPosition();
					while (pos)
					{
						CDLLVariable * pNewObj = new CDLLVariable;
						*pNewObj = *( s.m_DLLVariableList.GetNext(pos) );
						pMdl->m_DLLVariableList.AddTail(pNewObj);
					}

					pos = s.m_NormalVarList.GetHeadPosition();
					while (pos)
					{
						CNormalVar * pNewObj = new CNormalVar;
						*pNewObj = *( s.m_NormalVarList.GetNext(pos) );
						pMdl->m_NormalVarList.AddTail(pNewObj);
					}

					pos = s.m_RecordsetVariList.GetHeadPosition();
					while (pos)
					{
						CRecordsetVar * pNewObj = new CRecordsetVar;
						*pNewObj = *( s.m_RecordsetVariList.GetNext(pos) );
						pMdl->m_RecordsetVariList.AddTail(pNewObj);
					}

					pMdl->m_strControlMsg = "";
					pMdl->m_strDescription = "";
					d->AddTail(pMdl);
					
				}
			}
		}
		else if( NODETYPE_CUSTOM == pObj->m_nType )
		{
			CCustomObj * pNewObj =  new CCustomObj;
			*pNewObj = *(CCustomObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CC_CALLAGENT== pObj->m_nType )
		{
			CCCCallAgentObj * pNewObj =  new CCCCallAgentObj;
			*pNewObj = *(CCCCallAgentObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CC_TRANSFER== pObj->m_nType )
		{
			CCCTransferObj * pNewObj =  new CCCTransferObj;
			*pNewObj = *(CCCTransferObj *)pObj;
			d->AddTail(pNewObj);

		}
		else
		{
			CNodeObj * pNewObj = new CNodeObj;
			*pNewObj = *pObj;
			d->AddTail(pNewObj);
		}

	}
	
	return;
}

void CNodeObj::RemoveAllNextNode()
{
	CNextNodeList *pNextObj = &(m_NextNodeList);
	POSITION pos = pNextObj->GetHeadPosition();
	while( pos )
	{
		CNextNode * pObj =pNextObj->GetNext(pos);
		if( NULL == pObj  )
		{
			continue;
		}
		else
		{
			RemoveNextNodeObj(pObj);
		}
	}

	return ;
}


//////////////////////////////////////////////////////////////////////
// CCustomObj Class
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCustomObj, CNodeObj, 0)
CCustomObj::CCustomObj()
{
	m_pos		= NULL;
	m_strFunName	= "";
	m_nFunType		= -1;
	m_bEnd	= TRUE;
}

CCustomObj::CCustomObj( CPoint point )
	: CNodeObj(point)
{
	m_pos		= NULL;
	m_strFunName	= "";
	m_nFunType		= -1;
	m_bEnd	= TRUE;
}
CCustomObj::CCustomObj( const CCustomObj &s )
	:CNodeObj(s)
{
	
	m_pos		= s.m_pos;
	m_nFunType		= s.m_nFunType;
	m_strFunName	= s.m_strFunName;
	m_bEnd	=  s.m_bEnd;
}
CCustomObj & CCustomObj::operator =( const CCustomObj &s )
{
	if(this == &s)
		return *this;
	CNodeObj::operator = (s);

	m_pos		= s.m_pos;
	m_nFunType		= s.m_nFunType;
	m_strFunName	= s.m_strFunName;
	m_bEnd	=  s.m_bEnd;

	return *this;
	
}

CCustomObj::~CCustomObj()
{

}
void CCustomObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

	}
	else//读数据
	{

	}


}

POSITION CCustomObj::MoveNextFun()
{
	
	if( m_pos )
	{
		CNodeData * pVar = m_NodeDataList.GetNext(m_pos);
		if( pVar != NULL )
		{
			m_strFunName = pVar->m_strName;
			m_nFunType = pVar->m_nDataType;
			m_bEnd = FALSE;
		}
		else
		{
			m_bEnd = TRUE;
		}
	}
	else
	{
		m_bEnd = TRUE;
	}

	return m_pos;
}

void CCustomObj::MoveFirst()
{
	m_pos = m_NodeDataList.GetHeadPosition();
	if( NULL == m_pos )
	{
		m_bEnd = TRUE;
	}
	else
	{

		CNodeData * pVar = m_NodeDataList.GetNext(m_pos);
		if( pVar != NULL )
		{
			m_strFunName = pVar->m_strName;
			m_nFunType = pVar->m_nDataType;
			m_bEnd = FALSE;
		}
		else
		{
			m_bEnd = TRUE;
		}
	}

	return;
}


//////////////////////////////////////////////////////////////////////
// CCCCallAgentObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCCCallAgentObj, CNodeObj, 0)

CCCCallAgentObj::CCCCallAgentObj()
{
	m_nTimeOut = 30;
	m_nWaitType = 1;
	m_strWaitPlayFile = "";
	m_nPlayType = 0;
	m_strAimDN = "";
	m_bIsPlayBeforeTalk = TRUE;	

}

CCCCallAgentObj::CCCCallAgentObj( CPoint point )
	: CNodeObj(point)
{
	m_nTimeOut = 30;
	m_nWaitType = 0;
	m_strWaitPlayFile = "";
	m_nPlayType = 1;
	m_strAimDN = "";
	m_bIsPlayBeforeTalk = TRUE;	



}
CCCCallAgentObj::CCCCallAgentObj( const CCCCallAgentObj &s )
	:CNodeObj(s)
{
	

	m_nTimeOut = s.m_nTimeOut;
	m_nWaitType = s.m_nWaitType;
	m_strWaitPlayFile = s.m_strWaitPlayFile;
	m_nPlayType = s.m_nPlayType;
	m_strAimDN = s.m_strAimDN;
	m_bIsPlayBeforeTalk = s.m_bIsPlayBeforeTalk;

	POSITION pos = s.m_PlayList.GetHeadPosition();
	while( pos )
	{
		CNodeData *pObj = s.m_PlayList.GetNext(pos);
		if( pObj != NULL )
		{
			CNodeData *pNewObj = new CNodeData;
			*pNewObj = *pObj;
			m_PlayList.AddTail( pNewObj );
		}
	}



}
CCCCallAgentObj & CCCCallAgentObj::operator =( const CCCCallAgentObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nTimeOut = s.m_nTimeOut;
	m_nWaitType = s.m_nWaitType;
	m_strWaitPlayFile = s.m_strWaitPlayFile;
	m_nPlayType = s.m_nPlayType;
	m_strAimDN = s.m_strAimDN;
	m_bIsPlayBeforeTalk = s.m_bIsPlayBeforeTalk;

	POSITION pos = s.m_PlayList.GetHeadPosition();
	while( pos )
	{
		CNodeData *pObj = s.m_PlayList.GetNext(pos);
		if( pObj != NULL )
		{
			CNodeData *pNewObj = new CNodeData;
			*pNewObj = *pObj;
			m_PlayList.AddTail( pNewObj );
		}
	}
	return *this;
	
}

CCCCallAgentObj::~CCCCallAgentObj()
{
	while ( m_PlayList.GetHeadPosition() )
	{
		delete m_PlayList.RemoveHead();
	}
}
void CCCCallAgentObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nTimeOut;
		ar << m_nWaitType;
		ar << m_strWaitPlayFile;
		ar << m_nPlayType;
		ar << m_strAimDN;
		ar << m_bIsPlayBeforeTalk;

		m_PlayList.Serialize(ar);

	}
	else//读数据
	{
		ar >> m_nTimeOut;
		ar >> m_nWaitType;
		ar >> m_strWaitPlayFile;
		ar >> m_nPlayType;
		ar >> m_strAimDN;
		ar >> m_bIsPlayBeforeTalk;
		m_PlayList.Serialize(ar);

	}


}

//////////////////////////////////////////////////////////////////////
// CCCTransferObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CCCTransferObj, CNodeObj, 0)

CCCTransferObj::CCCTransferObj()
{
	m_nTransferType = 0;
	m_nTimeOut = 30;
	m_nWaitType = 0;
	m_strPlayFile = "";
	m_strCompleteDTMF ="#";
	m_strCancelDTMF = "#";
	m_strAimDN = "";
	m_bTimeOut = TRUE;
	m_bTalkWitDN = TRUE;
}

CCCTransferObj::CCCTransferObj( CPoint point )
	: CNodeObj(point)
{
	m_nTransferType = 0;
	m_nTimeOut = 30;
	m_nWaitType = 0;
	m_strPlayFile = "";
	m_strCompleteDTMF ="#";
	m_strCancelDTMF = "#";
	m_strAimDN = "";
	m_bTimeOut = TRUE;
	m_bTalkWitDN = TRUE;
}
CCCTransferObj::CCCTransferObj( const CCCTransferObj &s )
	:CNodeObj(s)
{
	m_nTransferType = s.m_nTransferType;
	m_nTimeOut = s.m_nTimeOut;
	m_nWaitType = s.m_nWaitType;
	m_strPlayFile = s.m_strPlayFile;
	m_strCompleteDTMF = s.m_strCompleteDTMF;
	m_strCancelDTMF = s.m_strCancelDTMF;
	m_strAimDN = s.m_strAimDN;
	m_bTimeOut = s.m_bTimeOut;
	m_bTalkWitDN = s.m_bTalkWitDN;

}
CCCTransferObj & CCCTransferObj::operator =( const CCCTransferObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	m_nTransferType = s.m_nTransferType;
	m_nTimeOut = s.m_nTimeOut;
	m_nWaitType = s.m_nWaitType;
	m_strPlayFile = s.m_strPlayFile;
	m_strCompleteDTMF = s.m_strCompleteDTMF;
	m_strCancelDTMF = s.m_strCancelDTMF;
	m_strAimDN = s.m_strAimDN;
	m_bTimeOut = s.m_bTimeOut;
	m_bTalkWitDN = s.m_bTalkWitDN;

	return *this;
	
}

CCCTransferObj::~CCCTransferObj()
{

}
void CCCTransferObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		ar << m_nTransferType;
		ar << m_nTimeOut;
		ar << m_nWaitType;
		ar << m_strPlayFile;
		ar << m_strCompleteDTMF;
		ar << m_strCancelDTMF;
		ar << m_strAimDN;
		ar << m_bTimeOut;
		ar << m_bTalkWitDN;
	}
	else//读数据
	{
		ar >> m_nTransferType;
		ar >> m_nTimeOut;
		ar >> m_nWaitType;
		ar >> m_strPlayFile;
		ar >> m_strCompleteDTMF;
		ar >> m_strCancelDTMF;
		ar >> m_strAimDN;
		ar >> m_bTimeOut;
		ar >> m_bTalkWitDN;
	}


}
