#if !defined(AFX_RENAMEDLG_H__87ACF906_BB3E_443A_B8FB_6E925B94EB13__INCLUDED_)
#define AFX_RENAMEDLG_H__87ACF906_BB3E_443A_B8FB_6E925B94EB13__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReNameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReNameDlg dialog

class CReNameDlg : public CDialog
{
// Construction
public:
	void Init( CNodeObj *pObj );
	
	CReNameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReNameDlg)
	enum { IDD = IDD_DIALOG_RENAME };
	CString	m_strNewName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReNameDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CNodeObj * m_pObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENAMEDLG_H__87ACF906_BB3E_443A_B8FB_6E925B94EB13__INCLUDED_)
