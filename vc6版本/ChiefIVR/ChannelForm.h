#if !defined(AFX_CHANNELFORM_H__6805B981_09A0_4759_B0C3_F9E1CA3F99FB__INCLUDED_)
#define AFX_CHANNELFORM_H__6805B981_09A0_4759_B0C3_F9E1CA3F99FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChannelForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CChannelForm : public CFormView
{
protected:
	CChannelForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChannelForm)
private:
	CProgressCtrl *m_pwndProg;
// Form Data
public:
	//{{AFX_DATA(CChannelForm)
	enum { IDD = IDD_CHANNEL_FORM };
	CListCtrl	m_lstNormalScript;
	CListCtrl	m_lstConfer;
	CListCtrl	m_lstFax;
	CListCtrl	m_listTCPIP;
	CListBox	m_CommList;
	CListCtrl	m_listScript;
	CListCtrl	m_lstChannel;
	CString	m_strTitle;
	//}}AFX_DATA

// Attributes
public:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
	int m_nShowType;
public:
	int m_nCardType;
	int m_nAnalogTrunk;//是否有模拟中继
	int m_nDigitalTrunk;//是否有数字中继以及类型
	int m_nVIP;
	CString m_strSetFile;
// Operations
public:
	long CountPlay(long lDTMF);
	BOOL CreatSysConfgFile(LPCTSTR lpszPathName);
	BOOL CreatChnlConfgFile(LPCTSTR lpszPathName);
	int Nor32DevChTypeToCifChType( int nChnlType  );
	void Nor32_InitChannelSet( void );
	void AutoCheck(void) ;
	BOOL InitChannelListItem( void );
	void TMS_InitChannelSet(void);
	int DJDevChTypeToCifChType(int nDJChType);
	void DJ_InitChannelSet( void );
	CString GetChannelTypeNew( int nType );
	int  GetChannelTypeNew( CString strType );
	void DelNormalScriptFile( void );
	BOOL SaveNormalScriptFile( void );
	void InitNormalScriptList( void );
	void InitNormalScriptFile();
	void ReadSystemSetFile( void );
	void InitFaxList( void );
	void InitConferenceList( void );
	void ShowConnectToChnl(int nChnl, int nConnectToChnl );
	BOOL SaveSysScriptFile( void );
	void InitSysScriptFile( void );
	void DelSysScriptFile( void );
	void ShowChannelDTMF(int nChnl, CString strDTMF);
	void ShowChannelBlock(int nChnl, CString strBlock);
	void ShowChannelState( int nChnl, CString strState );
	void InitScriptList( void );
	void InitTCPList( void );
	BOOL InitChannelList( void );
	void ShowList(CString strItem );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChannelForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CChannelForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnRclickListNormalscript(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListScript(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectAllChannel();
	afx_msg void OnCancelSelectChannel();
	afx_msg void OnSelectChannel();
	afx_msg void OnReverseSelectChannel();
	afx_msg void OnUpdateSelectChannel();
	afx_msg void OnReloadChannelScript();
	afx_msg void OnRclickListChannel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSaveChannelConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELFORM_H__6805B981_09A0_4759_B0C3_F9E1CA3F99FB__INCLUDED_)
