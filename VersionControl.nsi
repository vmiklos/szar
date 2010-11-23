Name "SQL Version Control"
InstallDir "$PROGRAMFILES\SQL Version Control"
SetCompressor /SOLID lzma
OutFile "sql-version-control-setup.exe"

!include "MUI2.nsh"
!define MUI_ABORTWARNING

RequestExecutionLevel admin ; Windows Vista UAC
XPStyle on

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

; The stuff to install ======================================================
Section ""

; Set output path to the installation directory.
SetOutPath $INSTDIR
File "winbin\*.exe"
File "winbin\*.dll"
File "VersionControl.ico"

CreateShortCut "$SMPROGRAMS\SQL Version Control.lnk" "$INSTDIR\gui.exe" "" "$INSTDIR\VersionControl.ico"

SectionEnd
