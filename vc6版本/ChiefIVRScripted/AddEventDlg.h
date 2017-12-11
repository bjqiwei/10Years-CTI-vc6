#if !defined(AFX_ADDEVENTDLG_H__5E5C22E0_9F31_4414_815D_DAD342A9030C__INCLUDED_)
#define AFX_ADDEVENTDLG_H__5E5C22E0_9F31_4414_815D_DAD342A9030C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddEventDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddEventDlg dialog

class CAddEventDlg : public CDialog
{
// Construction
public:
	void InitEventList( void );
	CString m_strEventName;
	int m_nEventType;
	CAddEventDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddEventDlg)
	enum { IDD = IDD_DIALOG_ADDEVENT };
	CListCtrl	m_lstEvent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddEventDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddEventDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickListEvent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListEvent(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDEVENTDLG_H__5E5C22E0_9F31_4414_815D_DAD342A9030C__INCLUDED_)
