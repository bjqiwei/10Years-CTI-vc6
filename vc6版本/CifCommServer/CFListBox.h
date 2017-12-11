#if !defined(AFX_PSGLISTBOX_H__F2999E37_72A4_4E41_8304_E5D05D9ECDC6__INCLUDED_)
#define AFX_PSGLISTBOX_H__F2999E37_72A4_4E41_8304_E5D05D9ECDC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CMyListBox window

class CMyListBox : public CListBox
{
// Construction
public:
	CMyListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddStringToList(LPCTSTR lpszItem);
	virtual ~CMyListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListBox)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_clrBack;
	COLORREF m_clrFor;
	CBrush m_brush;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSGLISTBOX_H__F2999E37_72A4_4E41_8304_E5D05D9ECDC6__INCLUDED_)
