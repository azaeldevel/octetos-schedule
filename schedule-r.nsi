!include "MUI.nsh"

!define NAME "sche"
!define VERSION "0.46.1-alpha"
!define PUBLISHER "Azael Reyes Inc."
!define WEBSITE "https://github.com/azaeldevel/octetos-schedule.git"


!include "FileFunc.nsh"
!insertmacro GetFileVersion
!insertmacro GetParameters
!include "WordFunc.nsh"
!insertmacro VersionCompare

Name "Schedule (Octetos)"
OutFile "bin\Release\Installer-${NAME}-${VERSION}.exe"
RequestExecutionLevel admin
SetCompressor bzip2
InstallDir $PROGRAMFILES64\${NAME}

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Spanish"


LangString Uninstall ${LANG_ENGLISH} "Uninstall"
LangString Uninstall ${LANG_SPANISH} "Desintalador"


Section
  SetOutPath $INSTDIR\
    
  File bin\Release\ec-schedule.exe
  File bin\Release\schedule.exe
  File bin\Release\libgcc_s_seh-1.dll
  File bin\Release\octetos-core.dll
  File bin\Release\octetos-EC.dll
  File bin\Release\octetos-schedule.dll
  File src\ec-schedule.bat
  
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  CreateDirectory "$SMPROGRAMS\${NAME}"
  CreateShortCut "$SMPROGRAMS\${NAME}\$(Uninstall).lnk" "$INSTDIR\Uninstall.exe" 
  CreateShortCut "$SMPROGRAMS\${NAME}\Schedule.lnk" "$INSTDIR\schedule.exe" " $INSTDIR\schedule.ico"
  CreateShortCut "$SMPROGRAMS\${NAME}\ec-schedule.lnk" "$INSTDIR\ec-schedule.bat"
  

SectionEnd

Section "Uninstall"
  
  Delete "$INSTDIR\ec-schedule.exe"
  Delete "$INSTDIR\schedule.exe"
  Delete "$INSTDIR\libgcc_s_seh-1.dll"
  Delete "$INSTDIR\octetos-core.dll"
  Delete "$INSTDIR\octetos-schedule.dll"
  Delete "$INSTDIR\octetos-EC.dll"
  Delete "$INSTDIR\schedule.ico"
  Delete "$INSTDIR\schedule.ui"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\ec-schedule.bat"
  RMDir "$INSTDIR"
  Delete "$SMPROGRAMS\${NAME}\$(Uninstall).lnk"
  Delete "$SMPROGRAMS\${NAME}\Schedule.lnk"
  Delete "$SMPROGRAMS\${NAME}\ec-schedule.lnk"
  RMDir "$SMPROGRAMS\${NAME}"
  
SectionEnd
