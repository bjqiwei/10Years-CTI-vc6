#if !defined(AFX_SELECTVARIABLESDLG_H__503A0107_9D72_4566_A1F0_0609C966F7A2__INCLUDED_)
#define AFX_SELECTVARIABLESDLG_H__503A0107_9D72_4566_A1F0_0609C966F7A2__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectVariablesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectVariablesDlg dialog
class CDrawDoc;
class CSelectVariablesDlg : public CDialog
{
// Construction
public:
	void InitParameterList( void );
	void InitSystemVarialeList();
	CString m_strVarName;
	void InitNormalVarialeList( void );
	void InitPage(CDrawDoc *pDoc );
	CSelectVariablesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectVariablesDlg)
	enum { IDD = IDD_DIALOG_SELSECT_VAR };
	CListCtrl	m_varList;
	int		m_nShowType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectVariablesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectVariablesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioShow1();
	afx_msg void OnRadioShow2();
	afx_msg void OnRadioShow3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTVARIABLESDLG_H__503A0107_9D72_4566_A1F0_0609C966F7A2__INCLUDED_)
