!include "MUI.nsh"

!define NAME "schedule"
!define VERSION "0.1.0"
!define PUBLISHER "Octetos"
!define WEBSITE "https://github.com/azaeldevel/octetos-schedule.git"


!include "FileFunc.nsh"
!insertmacro GetFileVersion
!insertmacro GetParameters
!include "WordFunc.nsh"
!insertmacro VersionCompare

Name "Schedule (Octetos)"
OutFile "bin\Debug\Installer-${NAME}-${VERSION}.exe"
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
    
  File bin\Debug\ec-schedule.exe
  File bin\Debug\schedule.exe
  File bin\Debug\libgcc_s_seh-1.dll
  File bin\Debug\octetos-core.dll
  File bin\Debug\octetos-EC.dll
  File schedule.ico
  File schedule.ui
  
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  CreateDirectory "$SMPROGRAMS\${NAME}"
  CreateShortCut "$SMPROGRAMS\${NAME}\$(Uninstall).lnk" "$INSTDIR\Uninstall.exe" 
  CreateShortCut "$SMPROGRAMS\${NAME}\Schedule.lnk" "$INSTDIR\schedule.exe" " $INSTDIR\schedule.ico"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayName" "${NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayIcon" "$INSTDIR\${NAME}.ico"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "Publisher" "${PUBLISHER}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "HelpLink" "${WEBSITE}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "URLInfoAbout" "${WEBSITE}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "NoRepair" 1
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "InstallLocation" "$INSTDIR"
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
  IntFmt $0 "0x%08X" $0
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "EstimatedSize" $0
SectionEnd

Section "Uninstall"
  
  Delete "$INSTDIR\ec-schedule.exe"
  Delete "$INSTDIR\schedule.exe"
  Delete "$INSTDIR\libgcc_s_seh-1.dll"
  Delete "$INSTDIR\octetos-core.dll"
  Delete "$INSTDIR\server.xsd"
  Delete "$INSTDIR\octetos-EC.dll"
  Delete "$INSTDIR\schedule.ico"
  Delete "$INSTDIR\schedule.ui"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR"
  Delete "$SMPROGRAMS\${NAME}\$(Uninstall).lnk"
  Delete "$SMPROGRAMS\${NAME}\Schedule.lnk"
  RMDir "$SMPROGRAMS\${NAME}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}"
SectionEnd