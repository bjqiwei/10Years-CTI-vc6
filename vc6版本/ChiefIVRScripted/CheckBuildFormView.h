#if !defined(AFX_CHECKBUILDFORMVIEW_H__35CFD265_8970_47C2_82B9_5A727A87D59E__INCLUDED_)
#define AFX_CHECKBUILDFORMVIEW_H__35CFD265_8970_47C2_82B9_5A727A87D59E__INCLUDED_

#include "DrawDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckBuildFormView.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CCheckBuildFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DrawView.h"
class CCheckBuildFormView : public CFormView
{
protected:
	CCheckBuildFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCheckBuildFormView)

// Form Data
public:
	//{{AFX_DATA(CCheckBuildFormView)
	enum { IDD = IDD_CHECK_FORMVIEW };
	CTreeCtrl	m_checkTree;
	//}}AFX_DATA
protected:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
// Attributes
public:

// Operations
public:
	CDrawView * m_pDrawView;
	void InserRootItem();
	void InsertNotifyItem(LPCTSTR lpszItem);
	HTREEITEM InsertNodeItem(LPCTSTR lpszItem,int nItemKey ,HTREEITEM &hParent );
	void InsertErrorItem(LPCTSTR lpszItem,HTREEITEM hParent );
	void InsertWarningItem(LPCTSTR lpszItem, HTREEITEM hParent);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckBuildFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCheckBuildFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCheckBuildFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickCheckTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCheckTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedCheckTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	CImageList m_ImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKBUILDFORMVIEW_H__35CFD265_8970_47C2_82B9_5A727A87D59E__INCLUDED_)
