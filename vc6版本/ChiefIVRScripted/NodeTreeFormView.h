#if !defined(AFX_NODETREEFORMVIEW_H__C2E17BE7_B8EB_4C9A_A728_51D95521F2C2__INCLUDED_)
#define AFX_NODETREEFORMVIEW_H__C2E17BE7_B8EB_4C9A_A728_51D95521F2C2__INCLUDED_

#include "DrawDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeTreeFormView.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CNodeTreeFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "DrawView.h"	// Added by ClassView

class CNodeTreeFormView : public CFormView
{
protected:
	CNodeTreeFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CNodeTreeFormView)

// Form Data
public:
	//{{AFX_DATA(CNodeTreeFormView)
	enum { IDD = IDD_NODETREE_FORMVIEW };
	CTreeCtrl	m_nodeTree;
	//}}AFX_DATA
protected:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;

// Attributes
public:
	CDrawView * m_pDrawView;
// Operations
public:
	void DeleteTreeNode( void );
	void InsertEndCallBlockItem( int nItemKey, LPCTSTR lpszItem, UINT nImage, UINT nSelectedImage);
	void InsertBlockItem( int nBlockKey ,int nItemKey,LPCTSTR lpszItem, UINT nImage, UINT nSelectedImage);


	void UpdateTreeItem(LPCTSTR lpszNewText ,int nItemKey);
	void InserRootItem( int nDocType );
	void DeleteAllItems( void );
	void DeleteItem(int nItemKey );
	void InsertItem( int nparent ,int nItemKey,LPCTSTR lpszItem, UINT nImage, UINT nSelectedImage);
	HTREEITEM FindItemFromItemData(DWORD ID, HTREEITEM TreeItem);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeTreeFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CNodeTreeFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CNodeTreeFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickNodeTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkNodeTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickNodeTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkNodeTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingNodeTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNormalVariable();
	afx_msg void OnDatabaseVariable();
	afx_msg void OnDllHadle();
	afx_msg void OnSelchangedNodeTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	CImageList m_ImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODETREEFORMVIEW_H__C2E17BE7_B8EB_4C9A_A728_51D95521F2C2__INCLUDED_)
