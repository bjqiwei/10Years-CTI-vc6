#if !defined(AFX_NEWSCRIPTTYPESELECT_H__599F36EB_F361_4203_8E0F_DA8639C551C4__INCLUDED_)
#define AFX_NEWSCRIPTTYPESELECT_H__599F36EB_F361_4203_8E0F_DA8639C551C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewScriptTypeSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewScriptTypeSelect dialog

class CNewScriptTypeSelect : public CDialog
{
// Construction
public:
	CNewScriptTypeSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewScriptTypeSelect)
	enum { IDD = IDD_DIALOG_DOCTYPE };
	CListCtrl	m_lstDocType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewScriptTypeSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewScriptTypeSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDoctype(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemclickListDoctype(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDoctype(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList m_ImageList;
public:
	int m_nDocType ;//0 ief文件 1 nmf文件
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSCRIPTTYPESELECT_H__599F36EB_F361_4203_8E0F_DA8639C551C4__INCLUDED_)
