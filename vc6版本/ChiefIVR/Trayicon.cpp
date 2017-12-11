
#include "stdafx.h"
#include "trayicon.h"
#include <afxpriv.h>		

IMPLEMENT_DYNAMIC(CTrayIcon, CCmdTarget)

CTrayIcon::CTrayIcon(UINT uID)
{
	//uID：当用户在托盘图标上单击右键，CTrayIcon显示这个菜单
	memset(&m_nid, 0 , sizeof(m_nid));
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uID = uID;
	AfxLoadString(uID, m_nid.szTip, sizeof(m_nid.szTip));
}

CTrayIcon::~CTrayIcon()
{
	SetIcon(0); //从系统删除托盘
}


void CTrayIcon::SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg)
{

	ASSERT(pNotifyWnd==NULL || ::IsWindow(pNotifyWnd->GetSafeHwnd()));
	m_nid.hWnd = pNotifyWnd->GetSafeHwnd();

	ASSERT(uCbMsg==0 || uCbMsg>=WM_USER);
	m_nid.uCallbackMessage = uCbMsg;
}


BOOL CTrayIcon::SetIcon(UINT uID)
{ 
	HICON hicon=NULL;
	if (uID) 
	{
		AfxLoadString(uID, m_nid.szTip, sizeof(m_nid.szTip));
		hicon = AfxGetApp()->LoadIcon(uID);
	}
	return SetIcon(hicon, NULL);
}


BOOL CTrayIcon::SetIcon(HICON hicon, LPCSTR lpTip) 
{
	UINT msg;
	m_nid.uFlags = 0;


	if (hicon)
	{
	
		msg = m_nid.hIcon ? NIM_MODIFY : NIM_ADD;
		m_nid.hIcon = hicon;
		m_nid.uFlags |= NIF_ICON;
	} 
	else
	{ 
		if (m_nid.hIcon==NULL)
		{
			return TRUE;	
		}
		msg = NIM_DELETE;
	}

	if (lpTip)
	{
		strncpy(m_nid.szTip, lpTip, sizeof(m_nid.szTip));
	}
	if (m_nid.szTip[0])
	{
		m_nid.uFlags |= NIF_TIP;
	}

	if (m_nid.uCallbackMessage && m_nid.hWnd)
	{
		m_nid.uFlags |= NIF_MESSAGE;
	}

	BOOL bRet = Shell_NotifyIcon(msg, &m_nid);
	if (msg==NIM_DELETE || !bRet)
	{
		m_nid.hIcon = NULL;	// failed
	}
	return bRet;
}


LRESULT CTrayIcon::OnTrayNotification(WPARAM wID, LPARAM lEvent)
{
	if (wID!=m_nid.uID ||
		(lEvent!=WM_RBUTTONUP && lEvent!=WM_LBUTTONDBLCLK))
	{
		return 0;
	}

	CMenu menu;
	if (!menu.LoadMenu(m_nid.uID))
	{
		return 0;
	}

	CMenu* pSubMenu = menu.GetSubMenu(0);

	if (!pSubMenu) 
	{
		return 0;
	}

	if (lEvent==WM_RBUTTONUP) 
	{
		::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

		CPoint mouse;
		GetCursorPos(&mouse);
		::SetForegroundWindow(m_nid.hWnd);	
		::TrackPopupMenu(pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0,
			m_nid.hWnd, NULL);

	} 
	else 
	{
		::SendMessage(m_nid.hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0);
	}

	return 1;
}
