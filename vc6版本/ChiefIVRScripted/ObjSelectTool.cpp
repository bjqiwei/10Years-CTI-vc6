// ObjSelectTool.cpp: implementation of the CObjSelectTool class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "chiefivrscripted.h"
#include "ObjSelectTool.h"


#include "DrawDoc.h"
#include "DrawView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjSelectTool::CObjSelectTool()
{
	m_bDown = FALSE;//鼠标左键按下标志
	m_last.x = 0;//鼠标移动最时的坐标
	m_last.y =0;
	m_down= m_last;//鼠标左键按下时坐标
}

CObjSelectTool::~CObjSelectTool()
{

}

void CObjSelectTool::OnLButtonDblClk(CDrawView *pView, UINT nFlags, const CPoint &point)
{

}

void CObjSelectTool::OnLButtonDown(CDrawView *pView, UINT nFlags, const CPoint &point)
{
	CPoint local = point;
	pView->ClientToDoc(local);

	BOOL bSelect = FALSE;
	
	bSelect = pView->HitTestNodeObj( local, pView->m_bNextObjSelected );

	if( !bSelect )
	{
		pView->m_selection.RemoveAll();
		CClientDC dc( pView );
		CRect rect(point.x, point.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);

		m_bDown = TRUE;
		m_down = point;
		m_last = point;
		pView->m_bObjSelected = FALSE;
	}
	else
	{
		pView->m_bObjSelected = TRUE;
	}
	return;
}

void CObjSelectTool::OnLButtonUp(CDrawView *pView, UINT nFlags, const CPoint &point)
{
	if (pView->GetCapture() == pView)
	{
		m_bDown = FALSE;
		CClientDC dc(pView);
		CRect rect(m_down.x, m_down.y, m_last.x, m_last.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		pView->SelectWithinRect(rect, TRUE);
	}
	return;
}

void CObjSelectTool::OnMouseMove(CDrawView *pView, UINT nFlags, const CPoint &point)
{
	if ( pView->GetCapture() == pView )
	{
		if( m_bDown)
		{
			CClientDC dc(pView);
			CRect rect(m_down.x, m_down.y, m_last.x, m_last.y);
			rect.NormalizeRect();
			dc.DrawFocusRect(rect);
			rect.SetRect(m_down.x, m_down.y, point.x, point.y);
			rect.NormalizeRect();
			dc.DrawFocusRect(rect);
			m_last = point;

		}
		return;
	}




	return;
}


void CObjSelectTool::OnMouseMove( CDrawView *pView , CPoint point, CDC *pDc)
{
	if ( pView->GetCapture() == pView )
	{
		if( m_bDown)
		{

			CRect rect(m_down.x, m_down.y, m_last.x, m_last.y);
			rect.NormalizeRect();
			pDc->DrawFocusRect(rect);
			rect.SetRect(m_down.x, m_down.y, point.x, point.y);
			rect.NormalizeRect();
			pDc->DrawFocusRect(rect);
			m_last = point;

		}
	}


	return;
}
