#if !defined(AFX_PLAYPROPAGE_H__DA6CAAB3_519F_4632_B750_526FF4A77933__INCLUDED_)
#define AFX_PLAYPROPAGE_H__DA6CAAB3_519F_4632_B750_526FF4A77933__INCLUDED_

#include "ListBoxExBuddy.h"	// Added by ClassView
#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayProPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlayProPage dialog
class CDrawDoc;
class CPlayProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPlayProPage)

// Construction
public:
	CListBoxExBuddy m_ListBoxExBuddy;
	void InitPage( CPlayStartObj * pNodeObj, CDrawDoc * pDoc   );
	CPlayProPage();
	~CPlayProPage();
protected:
// Dialog Data
	//{{AFX_DATA(CPlayProPage)
	enum { IDD = IDD_PAGE_PLAY_SPC };
	CEdit	m_edtFileExpress;
	CListCtrl	m_lstPlay;
	BOOL	m_bClearDTMF;
	BOOL	m_bReciveDTMF;
	CString	m_strDTMF;
	BOOL	m_bPlayStop;
	CString	m_strConfNO;
	int		m_nPlayType;
	int		m_nPlayWay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlayProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPlayProPage)
	afx_msg void OnButtonDtmf();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCleardtmf();
	afx_msg void OnCheckReciveDtmf();
	afx_msg void OnButtonExpress();
	afx_msg void OnClickListPlay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonConfno();
	afx_msg void OnSelchangeComboPlayway();
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CPlayStartObj * m_pNodeObj;
	CDrawDoc * m_pDoc;
public:
	int GetPlayType( CString strPlayType );
	CString GetPlayStr( int nPlayType );
	void EnableConferenceWindow( void );
	CNodeDataList m_PlayList;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYPROPAGE_H__DA6CAAB3_519F_4632_B750_526FF4A77933__INCLUDED_)
