#if !defined(AFX_CFBUTTON_H__3D460D81_386D_43EA_9D13_692F2879F1B5__INCLUDED_)
#define AFX_CFBUTTON_H__3D460D81_386D_43EA_9D13_692F2879F1B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CFButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCFButton window

class AFX_EXT_CLASS CCFButton : public CButton
{
	DECLARE_DYNAMIC(CCFButton)

// Construction
public:
	CCFButton();

// Attributes
protected:
	CSize       m_cSize;
	CRect		m_rcItem;
	CRect		m_rcIcon;
	UINT		m_nState;
	bool		m_bLBtnDown;
	bool		m_bFlatLook;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void SetIcon(HICON hIcon,CSize cSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DisableFlatLook() { m_bFlatLook = false; }
	void SetIconRect();
	virtual ~CCFButton();

// Generated message map functions
protected:
	//{{AFX_MSG(CCFButton)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFBUTTON_H__3D460D81_386D_43EA_9D13_692F2879F1B5__INCLUDED_)



