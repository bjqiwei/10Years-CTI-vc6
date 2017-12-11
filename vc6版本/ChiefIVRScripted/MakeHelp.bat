@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CHIEFIVRSCRIPTED.HPJ. >"hlp\ChiefIVRScripted.hm"
echo. >>"hlp\ChiefIVRScripted.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\ChiefIVRScripted.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\ChiefIVRScripted.hm"
echo. >>"hlp\ChiefIVRScripted.hm"
echo // Prompts (IDP_*) >>"hlp\ChiefIVRScripted.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\ChiefIVRScripted.hm"
echo. >>"hlp\ChiefIVRScripted.hm"
echo // Resources (IDR_*) >>"hlp\ChiefIVRScripted.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\ChiefIVRScripted.hm"
echo. >>"hlp\ChiefIVRScripted.hm"
echo // Dialogs (IDD_*) >>"hlp\ChiefIVRScripted.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\ChiefIVRScripted.hm"
echo. >>"hlp\ChiefIVRScripted.hm"
echo // Frame Controls (IDW_*) >>"hlp\ChiefIVRScripted.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\ChiefIVRScripted.hm"
REM -- Make help for Project CHIEFIVRSCRIPTED


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\ChiefIVRScripted.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\ChiefIVRScripted.hlp" goto :Error
if not exist "hlp\ChiefIVRScripted.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\ChiefIVRScripted.hlp" Debug
if exist Debug\nul copy "hlp\ChiefIVRScripted.cnt" Debug
if exist Release\nul copy "hlp\ChiefIVRScripted.hlp" Release
if exist Release\nul copy "hlp\ChiefIVRScripted.cnt" Release
echo.
goto :done

:Error
echo hlp\ChiefIVRScripted.hpj(1) : error: Problem encountered creating help file

:done
echo.
