# Microsoft Developer Studio Project File - Name="ChiefIVR" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ChiefIVR - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ChiefIVR.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ChiefIVR.mak" CFG="ChiefIVR - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ChiefIVR - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ChiefIVR - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ChiefIVR - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../bin/CFCH.lib ../bin/ChiefScriptNode.lib ../bin/CFCTI.lib /nologo /subsystem:windows /machine:I386 /out:"../bin/ChiefIVR.exe"

!ELSEIF  "$(CFG)" == "ChiefIVR - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../bin/CFCH.lib ../bin/ChiefScriptNode.lib ../bin/CFCTI.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../bin/ChiefIVR.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ChiefIVR - Win32 Release"
# Name "ChiefIVR - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddSysScriptFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalculateExpress.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelForm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChiefIVR.cpp
# End Source File
# Begin Source File

SOURCE=.\ChiefIVR.rc
# End Source File
# Begin Source File

SOURCE=.\ChiefIVRView.cpp
# End Source File
# Begin Source File

SOURCE=.\D08A.cpp
# End Source File
# Begin Source File

SOURCE=.\FChannelInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NewDBConnVar.cpp
# End Source File
# Begin Source File

SOURCE=.\NewEndCallBlockObj.cpp
# End Source File
# Begin Source File

SOURCE=.\NewModuleObj.cpp
# End Source File
# Begin Source File

SOURCE=.\NewRecordsetVar.cpp
# End Source File
# Begin Source File

SOURCE=.\NewSQLExcuteObj.cpp
# End Source File
# Begin Source File

SOURCE=.\NewStoreProObj.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalScriptInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\NorMarch32.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PublicCardObj.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectChannelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectTreeForm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysmsgForm.cpp
# End Source File
# Begin Source File

SOURCE=.\Trayicon.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateChannelDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddSysScriptFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalculateExpress.h
# End Source File
# Begin Source File

SOURCE=.\ChannelForm.h
# End Source File
# Begin Source File

SOURCE=.\ChiefIVR.h
# End Source File
# Begin Source File

SOURCE=.\ChiefIVRView.h
# End Source File
# Begin Source File

SOURCE=.\CIF5188.h
# End Source File
# Begin Source File

SOURCE=..\include\CtiDef.h
# End Source File
# Begin Source File

SOURCE=.\D08A.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NorMarch32.h
# End Source File
# Begin Source File

SOURCE=..\include\PhonicDll.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\PublicCardObj.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectChannelDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectTreeForm.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StructDefine.h
# End Source File
# Begin Source File

SOURCE=.\SysmsgForm.h
# End Source File
# Begin Source File

SOURCE=.\Trayicon.h
# End Source File
# Begin Source File

SOURCE=.\UpdateChannelDlg.h
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

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ChiefIVR.ico
# End Source File
# Begin Source File

SOURCE=.\res\ChiefIVR.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ChiefIVRDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sense4.lib
# End Source File
# End Group
# Begin Group "NorLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\lib\Phonic.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
