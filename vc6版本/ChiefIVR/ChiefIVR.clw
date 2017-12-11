; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "chiefivr.h"
LastPage=0

ClassCount=11
Class1=CAddSysScriptFileDlg
Class2=CChannelForm
Class3=CChiefIVRApp
Class4=CAboutDlg
Class5=CChiefIVRView
Class6=CMainFrame
Class7=CProgressDlg
Class8=CSelectChannelDlg
Class9=CSelectTreeForm
Class10=CSysmsgForm
Class11=CUpdateChannelDlg

ResourceCount=11
Resource1=IDD_DIALOG_SELCETCHANNEL
Resource2=IDR_TRAYICON
Resource3=IDR_POPTOOL
Resource4=IDR_MAINFRAME
Resource5=IDD_CHANNEL_FORM
Resource6=IDD_ABOUTBOX
Resource7=IDD_SYSSCRIPT_DLG
Resource8=IDD_SYSMSG_FORM
Resource9=IDD_DIALOG_UPDATECHNL
Resource10=IDD_TREE_FORM
Resource11=IDD_DIALOG_PROGRESS

[CLS:CAddSysScriptFileDlg]
Type=0
BaseClass=CDialog
HeaderFile=AddSysScriptFileDlg.h
ImplementationFile=AddSysScriptFileDlg.cpp

[CLS:CChannelForm]
Type=0
BaseClass=CFormView
HeaderFile=ChannelForm.h
ImplementationFile=ChannelForm.cpp

[CLS:CChiefIVRApp]
Type=0
BaseClass=CWinApp
HeaderFile=ChiefIVR.h
ImplementationFile=ChiefIVR.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ChiefIVR.cpp
ImplementationFile=ChiefIVR.cpp
LastObject=ID_SYS_HANGUP
Filter=D
VirtualFilter=dWC

[CLS:CChiefIVRView]
Type=0
BaseClass=CView
HeaderFile=ChiefIVRView.h
ImplementationFile=ChiefIVRView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=CMainFrame

[CLS:CProgressDlg]
Type=0
BaseClass=CDialog
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp

[CLS:CSelectChannelDlg]
Type=0
BaseClass=CDialog
HeaderFile=SelectChannelDlg.h
ImplementationFile=SelectChannelDlg.cpp

[CLS:CSelectTreeForm]
Type=0
BaseClass=CFormView
HeaderFile=SelectTreeForm.h
ImplementationFile=SelectTreeForm.cpp

[CLS:CSysmsgForm]
Type=0
BaseClass=CFormView
HeaderFile=SysmsgForm.h
ImplementationFile=SysmsgForm.cpp

[CLS:CUpdateChannelDlg]
Type=0
BaseClass=CDialog
HeaderFile=UpdateChannelDlg.h
ImplementationFile=UpdateChannelDlg.cpp

[DLG:IDD_SYSSCRIPT_DLG]
Type=1
Class=CAddSysScriptFileDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_REMARK,edit,1350631552
Control6=IDC_EDIT_FILE,edit,1350631552
Control7=IDC_BUTTON_FILE,button,1342242816
Control8=IDC_STATIC,button,1342177287

[DLG:IDD_CHANNEL_FORM]
Type=1
Class=CChannelForm
ControlCount=8
Control1=IDC_STATIC_TITLE,static,1342308352
Control2=IDC_LIST_CHANNEL,SysListView32,1350631429
Control3=IDC_LIST_TCPIP,SysListView32,1350631429
Control4=IDC_LIST_SCRIPT,SysListView32,1350631429
Control5=IDC_LIST_COMMLIST,listbox,1352663040
Control6=IDC_LIST_FAX,SysListView32,1350631429
Control7=IDC_LIST_CONFER,SysListView32,1350631429
Control8=IDC_LIST_NORMALSCRIPT,SysListView32,1350631429

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=10
Control1=IDC_STATIC_WWW,static,1342177550
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,button,1342177287
Control5=IDC_EDIT_LICENCE,edit,1352730692
Control6=IDOK,button,1342242817
Control7=IDC_STATIC,button,1342177287
Control8=IDC_EDIT_ADDATION,edit,1352734788
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_WARNING,edit,1352734788

[DLG:IDD_DIALOG_PROGRESS]
Type=1
Class=CProgressDlg
ControlCount=1
Control1=IDC_PROGRESS1,msctls_progress32,1350565888

[DLG:IDD_DIALOG_SELCETCHANNEL]
Type=1
Class=CSelectChannelDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_BEGINNO,edit,1350631552
Control7=IDC_COMBO_CHNLTYPE,combobox,1344339971
Control8=IDC_EDIT_ENDNO,edit,1350631552

[DLG:IDD_TREE_FORM]
Type=1
Class=CSelectTreeForm
ControlCount=1
Control1=IDC_TREE_SELECT,SysTreeView32,1350631463

[DLG:IDD_SYSMSG_FORM]
Type=1
Class=CSysmsgForm
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_LIST_SYSTEM,listbox,1353777409

[DLG:IDD_DIALOG_UPDATECHNL]
Type=1
Class=CUpdateChannelDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHECK_UPDATE_TYPE,button,1342242819
Control4=IDC_CHECK_UPDATE_CALLEDID,button,1342242819
Control5=IDC_CHECK_UPDATE_SCRIPTFILE,button,1342242819
Control6=IDC_COMBO_CHNLTYPE,combobox,1344339971
Control7=IDC_EDIT_CALLEDID,edit,1350631552
Control8=IDC_EDIT_SCRIPTFILE,edit,1350631552
Control9=IDC_BUTTON_FILE,button,1342242816

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_SYS_RUN
Command2=ID_SYS_HANGUP
Command3=ID_SYS_STOP
Command4=ID_APP_ABOUT
CommandCount=4

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_SYS_RUN
Command2=ID_SYS_HANGUP
Command3=ID_SYS_STOP
Command4=ID_APP_EXIT
Command5=ID_ADD_SYSSCRIPT
Command6=ID_DEL_SYSSCRIPT
Command7=ID_SAVE_SYSSCRIPT
Command8=ID_ADD_NORSCRIPT
Command9=ID_DEL_NORSCRIPT
Command10=ID_SAVE_NORSCRIPT
Command11=ID_SELECT_ALL_CHANNEL
Command12=ID_CANCEL_SELECT_CHANNEL
Command13=ID_SELECT_CHANNEL
Command14=ID_REVERSE_SELECT_CHANNEL
Command15=ID_UPDATE_SELECT_CHANNEL
Command16=ID_RELOAD_CHANNEL_SCRIPT
Command17=ID_AUTO_CHECK
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_APP_ABOUT
CommandCount=20

[MNU:IDR_TRAYICON]
Type=1
Class=?
Command1=ID_APP_OPEN
Command2=IDR_MENU_RUN
Command3=IDR_MENU_HANGUP
Command4=IDR_MENU_STOP
Command5=IDR_MENU_EXIT
CommandCount=5

[MNU:IDR_POPTOOL]
Type=1
Class=?
Command1=ID_ADD_NORSCRIPT
Command2=ID_DEL_NORSCRIPT
Command3=ID_SAVE_NORSCRIPT
Command4=ID_ADD_SYSSCRIPT
Command5=ID_DEL_SYSSCRIPT
Command6=ID_SAVE_SYSSCRIPT
Command7=ID_SELECT_ALL_CHANNEL
Command8=ID_CANCEL_SELECT_CHANNEL
Command9=ID_SELECT_CHANNEL
Command10=ID_REVERSE_SELECT_CHANNEL
Command11=ID_UPDATE_SELECT_CHANNEL
Command12=ID_RELOAD_CHANNEL_SCRIPT
Command13=ID_SAVE_CHANNEL_CONFIG
CommandCount=13

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

