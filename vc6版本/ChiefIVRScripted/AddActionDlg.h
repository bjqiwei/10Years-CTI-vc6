#if !defined(AFX_ADDACTIONDLG_H__58F5BA16_66B5_481C_8D76_FA285678CD78__INCLUDED_)
#define AFX_ADDACTIONDLG_H__58F5BA16_66B5_481C_8D76_FA285678CD78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddActionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddActionDlg dialog
class CDrawDoc;
class CAddActionDlg : public CDialog
{
// Construction
public:
	int m_nActionType;
	CDrawDoc * m_pDoc;
	CString m_strExpress;
	void InitActionList( void );
	CAddActionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddActionDlg)
	enum { IDD = IDD_DIALOG_ADDACTION };
	CEdit	m_ctrExpress;
	CListCtrl	m_lstAction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddActionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonMenu();
	afx_msg void OnClickListAction(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bInit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDACTIONDLG_H__58F5BA16_66B5_481C_8D76_FA285678CD78__INCLUDED_)
