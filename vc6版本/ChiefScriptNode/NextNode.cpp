// NextNode.cpp: implementation of the CNextNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodeObj.h"
//#include <afxwin.h>



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNextNode, CObject, 0)

CNextNode::CNextNode()
{
	m_width			= 72;
	m_high			= 13;
	m_rect.left		= 0;
	m_rect.top		= 0;
	m_rect.right	= m_width;
	m_rect.bottom   = m_high;
	m_nIndex        = 0;

	m_strData = "";//保存自己数据
	m_strName = "";//名称
	m_nDataType = 0;//初始化自己数据类型为整型
	m_nType = 0;

	m_strNodeObjName = "";//与之相关的结点对象名称，如果为""表示没有相关对象
	m_nNodeObjType = 0 ;//与之相关的结点对象类型，如果等于0表示没有相关对象
	m_LineObj.m_BeginPoint = GetLinePoint();
	m_nNodeObjKey = 0;//如果等于0，表示该NextNode没有下关系NodeObj对象，当编译时需要检查该相

	m_NodeObjRect = CRect(0,0,0,0);
}

CNextNode::CNextNode( const CNextNode &s )
{
	m_width			= s.m_width;
	m_high			= s.m_high;
	m_rect			= s.m_rect;
	m_nIndex        = s.m_nIndex;
	m_strData		= s.m_strData;
	m_strName		= s.m_strName;
	m_nDataType		= s.m_nDataType;
	m_nType			= s.m_nType;
	m_strNodeObjName	= s.m_strNodeObjName;
	m_nNodeObjType		= s.m_nNodeObjType;
	m_nNodeObjKey		= s.m_nNodeObjKey;
	m_LineObj			= s.m_LineObj;
	m_NodeObjRect		= s.m_NodeObjRect;

}

CNextNode & CNextNode::operator =( const CNextNode &s )
{
	if(this == &s)
		return *this;
	m_width			= s.m_width;
	m_high			= s.m_high;
	m_rect			= s.m_rect;
	m_nIndex        = s.m_nIndex;
	m_strData		= s.m_strData;
	m_strName		= s.m_strName;
	m_nDataType		= s.m_nDataType;
	m_nType			= s.m_nType;
	m_strNodeObjName	= s.m_strNodeObjName;
	m_nNodeObjType		= s.m_nNodeObjType;
	m_nNodeObjKey		= s.m_nNodeObjKey;
	m_LineObj			= s.m_LineObj;
	m_NodeObjRect		= s.m_NodeObjRect;

	return *this;
}


CNextNode::~CNextNode()
{

}

inline void CNextNode::OnDraw(CDC *pDC )
{
	CRect rect;
	rect = m_rect;

	LOGFONT lf; 
	memset(&lf,0,sizeof(LOGFONT)); 
	lf.lfHeight			= 14; //字体大小
	lf.lfWeight			= FW_NORMAL; 
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

	pDC->Draw3dRect( &rect, RGB(128,128,128), RGB(255, 255, 255) );
	pDC->SetTextColor( RGB(10,36,106) );
	pDC->SetBkMode( TRANSPARENT); 
	pDC->DrawText( m_strName,   &rect , DT_CENTER|DT_END_ELLIPSIS);

	
	pDC->SelectObject(pOldFont); // 回复旧字体-这是必须的
	pOldFont = NULL;
	
	if( m_nNodeObjKey > 100 )
		m_LineObj.OnDraw(pDC);//画线

	return;
}

inline void CNextNode::MoveTo( CPoint point , CRect BeginRect )
{
	m_rect.left   = 21 + point.x ;
	m_rect.top     = 8 + point.y  + (m_high + 2) * m_nIndex ;
	m_rect.right  =  21 +  point.x + m_width;
	m_rect.bottom  = 8 + point.y  +  (m_high + 2) * (m_nIndex + 1);

	m_LineObj.m_BeginPoint = GetLinePoint();
	m_LineObj.m_BeginRect = BeginRect;
	m_NodeObjRect = BeginRect;
}

inline void CNextNode::SetNodeInfo(CPoint point, LPCTSTR lpszName, LPCTSTR lpszData, int nDataType, int nType , int nIndex )
{
	m_nIndex = nIndex;
	m_strName = lpszName;
	m_strData = lpszData;

	m_nDataType = nDataType;

	m_rect.left   = 21 + point.x ;
	m_rect.top     = 8 + point.y  + (m_high + 2) * m_nIndex ;
	m_rect.right  =  21 +  point.x + m_width;
	m_rect.bottom  = 8 + point.y  +  (m_high + 2) * (m_nIndex + 1);

	m_LineObj.m_BeginPoint = GetLinePoint();
	m_nType = nType;
	return;
}


inline BOOL CNextNode::Intersects(const CRect &rect)
{
	ASSERT_VALID(this);

	CRect fixed = m_rect;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & fixed).IsRectEmpty();
}

inline void CNextNode::SetNodeObj(LPCTSTR lpszName, int nType)
{
	m_strNodeObjName = lpszName;
	
	m_nNodeObjType = nType;

}

void CNextNode::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if (ar.IsStoring())//保存数据
	{
		ar << m_nIndex;
		ar << m_width;
		ar << m_high;
		ar << m_rect;
		ar << m_nType;
		ar << m_strData;
		ar << m_nDataType;
		ar << m_strName;
		ar << m_strNodeObjName;
		ar << m_nNodeObjType;
		ar << m_nNodeObjKey;
		ar << m_NodeObjRect;


		m_LineObj.Serialize(ar);


	}
	else//读数据
	{
		ar >> m_nIndex;
		ar >> m_width;
		ar >> m_high;
		ar >> m_rect;
		ar >> m_nType;
		ar >> m_strData;
		ar >> m_nDataType;
		ar >> m_strName;
		ar >> m_strNodeObjName;
		ar >> m_nNodeObjType;
		ar >> m_nNodeObjKey;
		ar >> m_NodeObjRect;

		m_LineObj.Serialize(ar);
	}


}

long CNextNode::GetHigh()
{
	return m_high;
}

inline  BOOL CNextNode::HitTest(CPoint point)
{
	ASSERT_VALID(this);

	if (point.x >= m_rect.left && point.x < m_rect.right &&
		point.y >= m_rect.top && point.y < m_rect.bottom)
		return TRUE;
	else
		return FALSE;
}

inline CPoint CNextNode::GetLinePoint()
{
	CPoint point;
	point.x = m_rect.right ;
	point.y = m_rect.top + m_high/2;

	return point;

}

inline void CNextNode::OnDrawRubberLine(CWnd *pWnd, CPoint point)
{
	m_LineObj.OnDrawRubberLine( pWnd,point);
	return;
}

inline void CNextNode::ConnectToNodeObj(const char *pszNodeObjName, int nNodeObjType, int nNodeObjKey, CRect endRect)
{
	m_LineObj.m_EndRect = endRect;
	m_strNodeObjName = pszNodeObjName;
	m_nNodeObjKey = nNodeObjKey;
	m_nNodeObjType = nNodeObjType;

	return;
}

inline void CNextNode::DisConnectToNodeObj()
{
	m_strNodeObjName = "";
	m_nNodeObjKey = 0;
	m_nNodeObjType = 0;
	m_LineObj.m_EndRect.SetRect(0,0,0,0);

	return;
}

inline void CNextNode::UpdateNodeInfo(CPoint point, int nIndex)
{
	m_nIndex = nIndex;

	m_rect.left   = 21 + point.x ;
	m_rect.top     = 8 + point.y  + (m_high + 2) * m_nIndex ;
	m_rect.right  =  21 +  point.x + m_width;
	m_rect.bottom  = 8 + point.y  +  (m_high + 2) * (m_nIndex + 1);

	m_LineObj.m_BeginPoint = GetLinePoint();

	return;
}
