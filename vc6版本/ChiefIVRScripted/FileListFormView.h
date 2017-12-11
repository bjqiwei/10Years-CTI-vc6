#if !defined(AFX_FILELISTFORMVIEW_H__07C87631_B8A8_4AF0_9414_E3D0C1B87BE5__INCLUDED_)
#define AFX_FILELISTFORMVIEW_H__07C87631_B8A8_4AF0_9414_E3D0C1B87BE5__INCLUDED_

#include "DrawDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileListFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileListFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "DrawView.h"	// Added by ClassView
class CFileListFormView : public CFormView
{
protected:
	CFileListFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFileListFormView)

// Form Data
public:
	//{{AFX_DATA(CFileListFormView)
	enum { IDD = IDD_FILE_FORMVIEW };
	CTreeCtrl	m_ctrFileTree;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	HTREEITEM FindItemFromItemData(DWORD ID, HTREEITEM TreeItem);
	void DeleteItem(DWORD dwData );
	void InsertItem( DWORD dwData ,int nDocType,CString  strTitle  );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileListFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFileListFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFileListFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedTreeFile(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
private:
	CImageList m_ImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILELISTFORMVIEW_H__07C87631_B8A8_4AF0_9414_E3D0C1B87BE5__INCLUDED_)
