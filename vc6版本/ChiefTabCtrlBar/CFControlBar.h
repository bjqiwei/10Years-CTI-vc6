// CFControlBar.h: interface for the CCFControlBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFCONTROLBAR_H__60D37BA3_ACE3_46ED_9E37_C4E8C8837749__INCLUDED_)
#define AFX_CFCONTROLBAR_H__60D37BA3_ACE3_46ED_9E37_C4E8C8837749__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CFButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCFControlBar class

class  AFX_EXT_CLASS CCFControlBar : public CControlBar
{
	DECLARE_DYNAMIC(CCFControlBar)

// Construction
public:
	CCFControlBar();

// Attributes
protected:
	BOOL		 m_bInRecalcNC;
    BOOL         m_bTracking;
	BOOL		 m_bGripper;
	BOOL		 m_bButtons;
    UINT         m_cxEdge;
	UINT		 m_menuID;
    UINT         m_nDockBarID;
    CSize        m_sizeMin;
    CSize        m_sizeHorz;
    CSize        m_sizeVert;
    CSize        m_sizeFloat;
    CRect        m_rectBorder;
    CRect        m_rectTracker;
    CPoint       m_ptOld;
	CCFButton    m_btnClose;
	CCFButton    m_btnMinim;
	CImageList*  m_pImageList;
	CToolTipCtrl m_ToolTip;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFControlBar)
	public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL Create(CWnd* pParentWnd, UINT nID, LPCTSTR lpszWindowName = NULL, CSize sizeDefault = CSize(200,200), DWORD dwStyle = WS_CHILD|WS_VISIBLE|CBRS_TOP);
    virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
    virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
	virtual BOOL IsFloating();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsHorzDocked() const;
	BOOL IsVertDocked() const;
	
	void ShowFrameStuff(BOOL bGripper = TRUE, BOOL bButtons = TRUE) { 
		m_bGripper = bGripper;
		m_bButtons = bButtons;
	}

	void DrawGripper(CDC* pDC);
	CRect GetGripperRect();
	CRect GetButtonRect();
	UINT GetMenuID();
	void SetMenuID(UINT nID);
	void SetBtnImageList(CImageList *pImageList);
	void OnInvertTracker(const CRect& rect);
	void StopTracking(BOOL bAccept);
	void StartTracking();
	CPoint& ClientToWnd(CPoint& point);
	virtual ~CCFControlBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CCFControlBar)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnNcPaint();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonClose();
	afx_msg void OnUpdateButtonClose(CCmdUI* pCmdUI);
	afx_msg void OnButtonMinimize();
	afx_msg void OnUpdateButtonMinimize(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CFCONTROLBAR_H__60D37BA3_ACE3_46ED_9E37_C4E8C8837749__INCLUDED_)
