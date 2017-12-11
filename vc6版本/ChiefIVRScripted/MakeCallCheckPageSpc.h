#if !defined(AFX_MAKECALLCHECKPAGESPC_H__58785D87_9925_419D_8C94_F7D089DA2D19__INCLUDED_)
#define AFX_MAKECALLCHECKPAGESPC_H__58785D87_9925_419D_8C94_F7D089DA2D19__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeCallCheckPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeCallCheckPageSpc dialog
class CDrawDoc;
class CMakeCallCheckPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CMakeCallCheckPageSpc)

// Construction
public:
	void InitPage( CDialCheckObj * pObj ,CDrawDoc *pDoc );
	CMakeCallCheckPageSpc();
	~CMakeCallCheckPageSpc();

// Dialog Data
	//{{AFX_DATA(CMakeCallCheckPageSpc)
	enum { IDD = IDD_PAGE_MAKECALLCHECK_SPC };
	CListCtrl	m_listEvent;
	BOOL	m_bAfterRings;
	BOOL	m_bNotAnswer;
	UINT	m_nRings;
	UINT	m_nTimes;
	CString	m_strPhone;
	CString	m_strPrefix;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMakeCallCheckPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMakeCallCheckPageSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPhone();
	afx_msg void OnCheckNotanswer();
	afx_msg void OnCheckAfterrings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void EditNextNode(int nType, BOOL bEditType);
	CDrawDoc * m_pDoc;
	CDialCheckObj * m_pNodeObj;
public:
	void EnableMyWindow( void );
	BOOL m_bConnect;
	BOOL m_bBusy;
	BOOL m_bNoDialTone;
	BOOL m_bNoAnswer;
	BOOL m_bNoRingBack;
	BOOL m_bError;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKECALLCHECKPAGESPC_H__58785D87_9925_419D_8C94_F7D089DA2D19__INCLUDED_)
