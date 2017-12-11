// LineObj.cpp: implementation of the CLineObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodeObj.h"


IMPLEMENT_SERIAL(CLineObj, CObject, 0)
CLineObj::CLineObj()
{
	
	m_BeginPoint = CPoint(0,0);
	m_strCaption = "";
	m_crLineColor = RGB(0, 0, 255);
	m_crOldLineColor = RGB(0, 0, 255);
	m_OldPoint = CPoint(0,0);
	m_nOffset = 2;
	m_EndRect.SetRect(0,0,0,0);
	m_BeginRect.SetRect(0,0,0,0);

	return;
}
CLineObj::CLineObj( const CLineObj &s )
{
	m_BeginRect = s.m_BeginRect;
	m_EndRect = s.m_EndRect;
	
	m_nOffset = s.m_nOffset;
	m_strCaption = s.m_strCaption;
	m_crLineColor = s.m_crLineColor;
	m_crOldLineColor = s.m_crOldLineColor;
	m_OldPoint.Offset( s.m_OldPoint );
	m_BeginPoint.Offset( s.m_BeginPoint );

}
CLineObj & CLineObj::operator =( const CLineObj &s )
{
	if(this == &s)
		return *this;

	m_BeginRect = s.m_BeginRect;
	m_EndRect = s.m_EndRect;
	
	m_nOffset = s.m_nOffset;
	m_strCaption = s.m_strCaption;
	m_crLineColor = s.m_crLineColor;
	m_crOldLineColor = s.m_crOldLineColor;
	m_OldPoint.Offset( s.m_OldPoint );
	m_BeginPoint.Offset( s.m_BeginPoint );

	return *this;
}
CLineObj::~CLineObj()
{

}

void CLineObj::OnDraw(CDC *pDC)
{

	LOGPEN logpen;
	CPen pen;
	CPoint TempPoint=CPoint(0,0);
	CPen* pOldPen = NULL;
	logpen.lopnStyle = PS_SOLID;//实心的线
	logpen.lopnWidth.x = 1;
	logpen.lopnWidth.y = 1;
	logpen.lopnColor = m_crLineColor;

	
	if (!pen.CreatePenIndirect(&logpen))
		return;
	pOldPen = pDC->SelectObject(&pen);


	//画连接线

	CPoint Point1;
	CPoint Point2;
	CPoint Point3;
	CPoint Point4;
	CPoint Point5;
	CPoint Point6;
	CPoint Point7;
	CPoint Point8;

	CPoint BeginPoint = m_BeginPoint;
	CPoint endPoint(0,0) ;

	endPoint.x = m_EndRect.left  ;
	endPoint.y = m_EndRect.top + 8;

	if(  BeginPoint.y > endPoint.y )//结束点在开始点的下端
	{
		if( BeginPoint.x > endPoint.x )//结束点在开始点的左端
		{
				if( m_BeginRect.top - m_EndRect.bottom > 10  )
				{
					Point1.x = BeginPoint.x + 10;
					Point1.y = BeginPoint.y ;
					Point2.x = Point1.x + m_nOffset;
					Point2.y = Point1.y - m_nOffset;

					Point3.x = Point2.x;
					Point3.y = m_EndRect.bottom + 7;

					Point4.x = Point1.x;
					Point4.y = Point3.y  - m_nOffset;

					Point5.x = m_EndRect.left - 12;
					Point5.y = Point4.y ;

					Point6.x = Point5.x  - m_nOffset;
					Point6.y = Point5.y - m_nOffset ;

					Point7.x = Point6.x;
					Point7.y = endPoint.y + m_nOffset ;

					Point8.x = Point7.x + m_nOffset ;
					Point8.y = endPoint.y ;

				}
				else
				{
					Point1.x = BeginPoint.x + 10;
					Point1.y = BeginPoint.y ;
					Point2.x = Point1.x + m_nOffset;
					Point2.y = Point1.y - m_nOffset;

					Point3.x = Point2.x;
					Point3.y = m_EndRect.top  - 5;

					Point4.x = Point1.x;
					Point4.y = Point3.y  - m_nOffset;

					Point5.x = m_EndRect.left - 12;
					Point5.y = Point4.y ;

					Point6.x = Point5.x  - m_nOffset;
					Point6.y = Point5.y + m_nOffset ;

					Point7.x = Point6.x;
					Point7.y = endPoint.y - m_nOffset ;

					Point8.x = Point7.x + m_nOffset ;
					Point8.y = endPoint.y ;
				}

		}
		else//结束点在开始点的右端
		{

			Point1.x = BeginPoint.x + 10;
			Point1.y = BeginPoint.y ;
			Point2.x = Point1.x + m_nOffset;
			Point2.y = Point1.y - m_nOffset;

			Point3 = Point2;
			Point4 = Point2;
			Point5 = Point2;
			Point6 = Point2;

			Point7.x = Point6.x;
			Point7.y = endPoint.y + m_nOffset ;

			Point8.x = Point7.x + m_nOffset ;
			Point8.y = endPoint.y ;



		}



	}
	else//在开始结点下方
	{
		if( BeginPoint.x > endPoint.x )//结束点在开始点的左端
		{
			if( m_EndRect.top - m_BeginRect.bottom > 10  )
			{
				Point1.x = BeginPoint.x + 10;
				Point1.y = BeginPoint.y ;
				Point2.x = Point1.x + m_nOffset;
				Point2.y = Point1.y + m_nOffset;

				Point3.x = Point2.x;
				Point3.y = m_EndRect.top  - 7;

				Point4.x = Point1.x;
				Point4.y = Point3.y  + m_nOffset;

				Point5.x = m_EndRect.left - 12;
				Point5.y = Point4.y ;

				Point6.x = Point5.x  - m_nOffset;
				Point6.y = Point5.y + m_nOffset ;

				Point7.x = Point6.x;
				Point7.y = endPoint.y - m_nOffset ;

				Point8.x = Point7.x + m_nOffset ;
				Point8.y = endPoint.y ;

			}
			else
			{
				
				Point1.x = BeginPoint.x + 10;
				Point1.y = BeginPoint.y ;
				Point2.x = Point1.x + m_nOffset;
				Point2.y = Point1.y - m_nOffset;

				Point3.x = Point2.x;
				Point3.y = m_BeginRect.top  - 5;

				Point4.x = Point1.x;
				Point4.y = Point3.y  - m_nOffset;

				Point5.x = m_EndRect.left - 12;
				Point5.y = Point4.y ;

				Point6.x = Point5.x  - m_nOffset;
				Point6.y = Point5.y + m_nOffset ;

				Point7.x = Point6.x;
				Point7.y = endPoint.y - m_nOffset ;

				Point8.x = Point7.x + m_nOffset ;
				Point8.y = endPoint.y ;
				
			}
		}
		else
		{
			Point1.x = BeginPoint.x + 10;
			Point1.y = BeginPoint.y ;
			Point2.x = Point1.x + m_nOffset;
			Point2.y = Point1.y + m_nOffset;

			Point3 = Point2;
			Point4 = Point2;
			Point5 = Point2;
			Point6 = Point2;

			Point7.x = Point6.x;
			Point7.y = endPoint.y - m_nOffset ;

			Point8.x = Point7.x + m_nOffset ;
			Point8.y = endPoint.y ;


		}

	}

	pDC->MoveTo(BeginPoint);
	pDC->LineTo(Point1);
	pDC->MoveTo(Point1);
	pDC->LineTo(Point2);
	pDC->MoveTo(Point2);
	pDC->LineTo(Point3);
	pDC->MoveTo(Point3);
	pDC->LineTo(Point4);
	pDC->MoveTo(Point4);
	pDC->LineTo(Point5);
	pDC->MoveTo(Point5);
	pDC->LineTo(Point6);
	pDC->MoveTo(Point6);
	pDC->LineTo(Point7);
	pDC->MoveTo(Point7);
	pDC->LineTo(Point8);
	pDC->MoveTo(Point8);
	pDC->LineTo(endPoint);

	//画箭头

	CPoint ptop,pbottom;
	ptop.x = endPoint.x - 4;
	ptop.y = endPoint.y - 4;

	pbottom.x = endPoint.x - 4;
	pbottom.y = endPoint.y + 4;

	pDC->MoveTo(endPoint);
	pDC->LineTo(ptop);
	pDC->MoveTo(ptop);
	pDC->LineTo(pbottom);
	pDC->MoveTo(pbottom);
	pDC->LineTo(endPoint);

	return;

}

//画橡皮线，在画之前，需要把原来的连线去掉，
//只需要用背景色重画即可
inline void CLineObj::OnDrawRubberLine( CWnd* pWnd , CPoint point )
{
	ASSERT_VALID(pWnd);
	CClientDC dc(pWnd);
	
	LOGPEN logpen;
	CPen pen;
	CPen* pOldPen = NULL;
	logpen.lopnStyle = PS_SOLID;//实心的线
	logpen.lopnWidth.x = 1;
	logpen.lopnWidth.y = 1;
	logpen.lopnColor = RGB(255,0,0);

	

	if (!pen.CreatePenIndirect(&logpen))
		return;
	pOldPen = dc.SelectObject(&pen);
	//画连接线
	dc.SetROP2(R2_NOTXORPEN );
	dc.MoveTo(m_BeginPoint);
	dc.LineTo(m_OldPoint);
	dc.MoveTo(m_BeginPoint);
	dc.LineTo(point);
	m_OldPoint = point;


	dc.SelectObject(pOldPen);

	return;
}



void CLineObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);


	if (ar.IsStoring())//保存数据
	{

		ar << m_BeginPoint;
		ar << m_EndRect;
		ar << m_BeginRect;

		ar << m_strCaption;
		ar << m_crLineColor;
		ar <<m_OldPoint;

	}
	else//读数据
	{


		ar >> m_BeginPoint;
		ar >> m_EndRect;
		ar >> m_BeginRect;

		ar >> m_strCaption;
		ar >> m_crLineColor;
		ar >>m_OldPoint;
	}
	
	return;
}
