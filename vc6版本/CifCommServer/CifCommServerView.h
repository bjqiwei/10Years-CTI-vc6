// CifCommServerView.h : interface of the CCifCommServerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIFCOMMSERVERVIEW_H__C274FFA2_9F97_477A_9D4A_CED982F4795D__INCLUDED_)
#define AFX_CIFCOMMSERVERVIEW_H__C274FFA2_9F97_477A_9D4A_CED982F4795D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCifCommServerView : public CView
{
protected: // create from serialization only
	CCifCommServerView();
	DECLARE_DYNCREATE(CCifCommServerView)

// Attributes
public:
	CCifCommServerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCifCommServerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCifCommServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCifCommServerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CifCommServerView.cpp
inline CCifCommServerDoc* CCifCommServerView::GetDocument()
   { return (CCifCommServerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIFCOMMSERVERVIEW_H__C274FFA2_9F97_477A_9D4A_CED982F4795D__INCLUDED_)
