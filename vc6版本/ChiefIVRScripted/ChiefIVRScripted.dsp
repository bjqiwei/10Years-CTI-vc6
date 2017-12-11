# Microsoft Developer Studio Project File - Name="ChiefIVRScripted" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ChiefIVRScripted - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ChiefIVRScripted.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ChiefIVRScripted.mak" CFG="ChiefIVRScripted - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ChiefIVRScripted - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ChiefIVRScripted - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ChiefIVRScripted - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../bin/ChiefScriptNode.lib ../bin/ChiefTabCtrlBar.lib htmlhelp.lib /nologo /subsystem:windows /machine:I386 /out:"../bin/ChiefIVRScripted.exe"

!ELSEIF  "$(CFG)" == "ChiefIVRScripted - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../bin/ChiefScriptNode.lib ../bin/ChiefTabCtrlBar.lib htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../bin/ChiefIVRScripted.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ChiefIVRScripted - Win32 Release"
# Name "ChiefIVRScripted - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddEventDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddNewVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\AddNormalVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\AddToTemplateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BBBrowseSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\CallHoldProPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CallLogRequestPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\CallRecordProPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CallTransferCheckPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CcCallAgentPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CcTransferPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeVoicePage.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckBuildFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChiefIVRScripted.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\ChiefIVRScripted.hpj
# End Source File
# Begin Source File

SOURCE=.\ChiefIVRScripted.rc
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CompareProPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConferBuildPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConferDeletePage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConferenceEnterPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\ConferLeavePageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\ConferPlayPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConferRecordPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConferRequestPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectNodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomManageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\DBClosePageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\DLLCallPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\DLLLoadPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\DLLUnloadPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawView.cpp
# End Source File
# Begin Source File

SOURCE=.\DTMFMenuPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\DTMFRecivePageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\DTMFSendPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\EditCompareListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EventCheckPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpressionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpressPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FaxRecivePageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\FaxSendPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\FileListFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\FindFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\FunctionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GetConferMemberPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBoxExBuddy.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeCallCheckPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleProPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDocManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMultiDocTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\NewScriptTypeSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeRemarkFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeTreeFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjSelectTool.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenDBPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayProPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ReleaseCallPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ReNameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RouteCallProPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectConstantVarDld.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectVariablesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfDefinesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SetParameterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetVariableProPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLExcutePageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StoreProcPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SwitchPageSpc.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceProPage.cpp
# End Source File
# Begin Source File

SOURCE=.\VoiceFileFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitCallSpecificProPage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddEventDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddNewVariable.h
# End Source File
# Begin Source File

SOURCE=.\AddNormalVariable.h
# End Source File
# Begin Source File

SOURCE=.\AddToTemplateDlg.h
# End Source File
# Begin Source File

SOURCE=.\BBBrowseSpc.h
# End Source File
# Begin Source File

SOURCE=.\CallHoldProPage.h
# End Source File
# Begin Source File

SOURCE=.\CallLogRequestPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\CallRecordProPage.h
# End Source File
# Begin Source File

SOURCE=.\CallTransferCheckPage.h
# End Source File
# Begin Source File

SOURCE=.\CcCallAgentPage.h
# End Source File
# Begin Source File

SOURCE=.\CcTransferPage.h
# End Source File
# Begin Source File

SOURCE=.\CFString.h
# End Source File
# Begin Source File

SOURCE=.\ChangeVoicePage.h
# End Source File
# Begin Source File

SOURCE=.\CheckBuildFormView.h
# End Source File
# Begin Source File

SOURCE=.\ChiefIVRScripted.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CntrItem.h
# End Source File
# Begin Source File

SOURCE=.\CompareProPage.h
# End Source File
# Begin Source File

SOURCE=.\ConferBuildPage.h
# End Source File
# Begin Source File

SOURCE=.\ConferDeletePage.h
# End Source File
# Begin Source File

SOURCE=.\ConferenceEnterPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\ConferLeavePageSpc.h
# End Source File
# Begin Source File

SOURCE=.\ConferPlayPage.h
# End Source File
# Begin Source File

SOURCE=.\ConferRecordPage.h
# End Source File
# Begin Source File

SOURCE=.\ConferRequestPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectNodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\CustomManageDlg.h
# End Source File
# Begin Source File

SOURCE=.\CustomTemplate.h
# End Source File
# Begin Source File

SOURCE=.\DBClosePageSpc.h
# End Source File
# Begin Source File

SOURCE=.\DLLCallPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\DLLLoadPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\DLLUnloadPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\DrawDoc.h
# End Source File
# Begin Source File

SOURCE=.\DrawView.h
# End Source File
# Begin Source File

SOURCE=.\DTMFMenuPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\DTMFRecivePageSpc.h
# End Source File
# Begin Source File

SOURCE=.\DTMFSendPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\EditCompareListDlg.h
# End Source File
# Begin Source File

SOURCE=.\EventCheckPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\ExpressionDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExpressPage.h
# End Source File
# Begin Source File

SOURCE=.\FaxRecivePageSpc.h
# End Source File
# Begin Source File

SOURCE=.\FaxSendPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\FileListFormView.h
# End Source File
# Begin Source File

SOURCE=.\FindFormView.h
# End Source File
# Begin Source File

SOURCE=.\FunctionPage.h
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\GetConferMemberPage.h
# End Source File
# Begin Source File

SOURCE=.\GlobalFunction.h
# End Source File
# Begin Source File

SOURCE=.\ListBoxExBuddy.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MakeCallCheckPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\ModuleProPage.h
# End Source File
# Begin Source File

SOURCE=.\MyDocManager.h
# End Source File
# Begin Source File

SOURCE=.\MyMultiDocTemplate.h
# End Source File
# Begin Source File

SOURCE=.\NewScriptTypeSelect.h
# End Source File
# Begin Source File

SOURCE=.\NodeRemarkFormView.h
# End Source File
# Begin Source File

SOURCE=.\NodeTreeFormView.h
# End Source File
# Begin Source File

SOURCE=.\ObjSelectTool.h
# End Source File
# Begin Source File

SOURCE=.\OpenDBPage.h
# End Source File
# Begin Source File

SOURCE=.\PlayProPage.h
# End Source File
# Begin Source File

SOURCE=.\ReleaseCallPage.h
# End Source File
# Begin Source File

SOURCE=.\ReNameDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RouteCallProPage.h
# End Source File
# Begin Source File

SOURCE=.\SelectConstantVarDld.h
# End Source File
# Begin Source File

SOURCE=.\SelectVariablesDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelfDefinesPage.h
# End Source File
# Begin Source File

SOURCE=.\SetParameterDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetVariableProPage.h
# End Source File
# Begin Source File

SOURCE=.\SQLExcutePageSpc.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StoreProcPage.h
# End Source File
# Begin Source File

SOURCE=.\SwitchPageSpc.h
# End Source File
# Begin Source File

SOURCE=.\TraceProPage.h
# End Source File
# Begin Source File

SOURCE=.\VoiceFileFormView.h
# End Source File
# Begin Source File

SOURCE=.\WaitCallSpecificProPage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\about.bmp
# End Source File
# Begin Source File

SOURCE=.\about2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\about2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\begin.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\build_co.ico
# End Source File
# Begin Source File

SOURCE=.\res\cc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ChiefIVRScripted.ico
# End Source File
# Begin Source File

SOURCE=.\res\ChiefIVRScripted.rc2
# End Source File
# Begin Source File

SOURCE=.\res\confer.ico
# End Source File
# Begin Source File

SOURCE=.\res\confer_r.ico
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_d.cur
# End Source File
# Begin Source File

SOURCE=.\res\dbbrowse.ico
# End Source File
# Begin Source File

SOURCE=.\res\DrawDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\dtmf_rec.ico
# End Source File
# Begin Source File

SOURCE=.\res\end.ico
# End Source File
# Begin Source File

SOURCE=.\res\endcall_.ico
# End Source File
# Begin Source File

SOURCE=.\res\express.ico
# End Source File
# Begin Source File

SOURCE=.\res\faxreciv.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00016.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00017.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00018.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00019.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00020.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00021.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00022.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00023.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00024.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00025.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00026.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00027.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00028.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00029.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00030.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00031.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00032.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00033.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00034.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00035.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00036.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00037.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00038.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00039.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00040.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00041.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00042.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00043.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00044.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00045.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00046.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00047.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00048.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00049.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00050.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00051.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00052.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00053.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00054.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00055.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_pla.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_modu.ico
# End Source File
# Begin Source File

SOURCE=.\res\iidi_con.ico
# End Source File
# Begin Source File

SOURCE=.\res\il_tab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\imagelis.bmp
# End Source File
# Begin Source File

SOURCE=.\nodrop.cur
# End Source File
# Begin Source File

SOURCE=.\res\Start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\variable.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChiefIVRScripted.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
