//--------------------------------------------------------------
//CTrayIcon:ÅÌÓ¦ÓÃÀà
//
//
//
//---------------------------------------------------------------
#ifndef _TRAYICON_H
#define _TRAYICON_H


class CTrayIcon : public CCmdTarget 
{
protected:
	DECLARE_DYNAMIC(CTrayIcon)
	//Contains information that the system needs to process taskbar status area messages
	NOTIFYICONDATA m_nid;	

public:
	CTrayIcon(UINT uID);
	~CTrayIcon();

	void SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg);

	BOOL SetIcon( UINT uID ); 
	BOOL SetIcon( HICON hicon, LPCSTR lpTip );
	BOOL SetIcon( LPCTSTR lpResName, LPCSTR lpTip )
		{ 
			return SetIcon( lpResName ? 
			AfxGetApp()->LoadIcon(lpResName) : NULL, lpTip ); 
		}
	BOOL SetStandardIcon(LPCTSTR lpszIconName, LPCSTR lpTip)
		{
			return SetIcon(::LoadIcon(NULL, lpszIconName), lpTip); 
		}

	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
};

#endif
