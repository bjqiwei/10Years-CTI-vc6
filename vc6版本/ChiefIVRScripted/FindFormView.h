#if !defined(AFX_FINDFORMVIEW_H__61C85197_8193_4511_82D0_55CE47B8D55D__INCLUDED_)
#define AFX_FINDFORMVIEW_H__61C85197_8193_4511_82D0_55CE47B8D55D__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
class CDrawDoc;
class CDrawView;
class CFindFormView : public CFormView
{
protected:
	CFindFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFindFormView)

// Form Data
public:
	//{{AFX_DATA(CFindFormView)
	enum { IDD = IDD_FIND_FORMVIEW };
	CListCtrl	m_ctrFindList;
	int		m_nFindType;
	CString	m_strFindStr;
	CString	m_strMatchesCount;
	//}}AFX_DATA
protected:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
// Attributes
public:

// Operations
public:
	void FindObjFromBlock( CBlockObj*pObj, CString strFind, int nFindType, CString &strNodeTree,int *pnIndex );
	void FindObjFromEndCallBlock( CEndCallBlockObj *pObj, CString strFind, int nFindType, CString &strNodeTree,int *pnIndex );
	CDrawView * m_pDrawView;
	void InsertMyColumn( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFindFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFindFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonFindnode();
	afx_msg void OnDblclkFindList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedFindList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList m_ImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDFORMVIEW_H__61C85197_8193_4511_82D0_55CE47B8D55D__INCLUDED_)
