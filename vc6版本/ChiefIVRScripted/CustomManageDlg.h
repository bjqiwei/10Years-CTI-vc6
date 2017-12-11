#if !defined(AFX_CUSTOMMANAGEDLG_H__CC16A36D_D2A6_4BDC_B05B_5126F885B21A__INCLUDED_)
#define AFX_CUSTOMMANAGEDLG_H__CC16A36D_D2A6_4BDC_B05B_5126F885B21A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomManageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomManageDlg dialog

class CCustomManageDlg : public CDialog
{
// Construction
public:
	int FindItem( int nKey );
	CCustomManageDlg(CWnd* pParent = NULL);   // standard constructor
	CImageList m_ImageList;
// Dialog Data
	//{{AFX_DATA(CCustomManageDlg)
	enum { IDD = IDD_DIALOG_CUSTOM_MANAGE };
	CListCtrl	m_lstManage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomManageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCustomManageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomUpdate();
	afx_msg void OnCustomDelete();
	virtual void OnOK();
	afx_msg void OnClickListManage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListManage(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMMANAGEDLG_H__CC16A36D_D2A6_4BDC_B05B_5126F885B21A__INCLUDED_)
