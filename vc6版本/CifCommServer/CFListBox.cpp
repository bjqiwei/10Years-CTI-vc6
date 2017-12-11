

#include "stdafx.h"
#include "CFListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListBox

CMyListBox::CMyListBox()
{
	m_clrBack = RGB(0,0,0);
	m_clrFor = RGB(0,255,0);
	m_brush.CreateSolidBrush(m_clrBack);
	
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	//{{AFX_MSG_MAP(CMyListBox)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListBox message handlers



HBRUSH CMyListBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
		pDC->SetBkColor( m_clrBack );
		pDC->SetTextColor( m_clrFor );
		pDC->SetBkMode( OPAQUE );
		SetHorizontalExtent(2000);
	return (HBRUSH)m_brush.GetSafeHandle();
}

void CMyListBox::AddStringToList(LPCTSTR lpszItem)
{
	if( NULL == lpszItem ) return;
	int n_line = GetCount();

	if( LB_ERR == n_line )
		return;
	if (n_line > 300)
	{
		InsertString(n_line, lpszItem );
		SetCurSel(n_line);
		DeleteString(0);
		ResetContent();
	}
	else
	{
		InsertString(n_line, lpszItem);
		SetCurSel(n_line);
	}

	return;
}
