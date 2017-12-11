; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSysMsgForm
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "cifcommserver.h"
LastPage=0

ClassCount=8
Class1=CActiveMsgForm
Class2=CCifCommServerApp
Class3=CAboutDlg
Class4=CCifCommServerDoc
Class5=CCifCommServerView
Class6=CMainFrame
Class7=CSelectTreeForm
Class8=CSysMsgForm

ResourceCount=6
Resource1=IDR_TRAYICON
Resource2=IDD_ACTIVEMSG_FORM
Resource3=IDD_ABOUTBOX
Resource4=IDD_SYSMSG_FORM
Resource5=IDR_MAINFRAME
Resource6=IDD_TREE_FORM

[CLS:CActiveMsgForm]
Type=0
BaseClass=CFormView
HeaderFile=ActiveMsgForm.h
ImplementationFile=ActiveMsgForm.cpp

[CLS:CCifCommServerApp]
Type=0
BaseClass=CWinApp
HeaderFile=CifCommServer.h
ImplementationFile=CifCommServer.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=CifCommServer.cpp
ImplementationFile=CifCommServer.cpp

[CLS:CCifCommServerDoc]
Type=0
BaseClass=CDocument
HeaderFile=CifCommServerDoc.h
ImplementationFile=CifCommServerDoc.cpp

[CLS:CCifCommServerView]
Type=0
BaseClass=CView
HeaderFile=CifCommServerView.h
ImplementationFile=CifCommServerView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=CMainFrame

[CLS:CSelectTreeForm]
Type=0
BaseClass=CFormView
HeaderFile=SelectTreeForm.h
ImplementationFile=SelectTreeForm.cpp
LastObject=ID_APP_OPEN
Filter=D
VirtualFilter=VWC

[CLS:CSysMsgForm]
Type=0
BaseClass=CFormView
HeaderFile=SysMsgForm.h
ImplementationFile=SysMsgForm.cpp
Filter=D
VirtualFilter=VWC
LastObject=CSysMsgForm

[DLG:IDD_ACTIVEMSG_FORM]
Type=1
Class=CActiveMsgForm
ControlCount=6
Control1=IDC_STATIC_TITLE,static,1342308352
Control2=IDC_ACD_LIST,listbox,1353777153
Control3=IDC_AGENT_LIST,listbox,1353777409
Control4=IDC_IVR_LIST,listbox,1353777153
Control5=IDC_CTI_LIST,listbox,1353777153
Control6=IDC_SOCKET_LIST,listbox,1353777409

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=0

[DLG:IDD_TREE_FORM]
Type=1
Class=CSelectTreeForm
ControlCount=1
Control1=IDC_TREE_SELECT,SysTreeView32,1350631463

[DLG:IDD_SYSMSG_FORM]
Type=1
Class=CSysMsgForm
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_SYSMSG_LIST,listbox,1353777409

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_RUN
Command2=ID_FILE_STOP
Command3=ID_APP_EXIT
Command4=ID_APP_ABOUT
CommandCount=4

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[MNU:IDR_TRAYICON]
Type=1
Class=?
Command1=ID_APP_OPEN
Command2=IDR_MENU_RUN
Command3=IDR_MENU_STOP
Command4=IDR_MENU_EXIT
CommandCount=4

