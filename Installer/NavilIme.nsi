; NavilIME Installation Script
; Written by Manwoo Yi

!define RELVERSION      "2.3.0"
!define APPNAME         "NavilIME ${RELVERSION}"

!define DLLNAME "NavilIME.dll"
!define X64SRC "..\x64\Release\${DLLNAME}"
!define X86SRC "..\Win32\Release\${DLLNAME}"
!define DATSRC "..\NavilIME\Keyboard.dat"
!define INSTDIR "NavilIME"

!define NAVILIME_ICON "..\NavilIME\Images\Navilime.ico"
!define NAVILIME_CTRL_TOOL "..\navilIME_control\navilIME_control\bin\Release\navilIME_control.exe"

;!include "x64.nsh"
;Include Modern UI
!include "MUI2.nsh"
;Include Radio buttons
!include "Sections.nsh"

SetCompressor /SOLID lzma

BrandingText "NavilIME Installer"

;--------------------------------
;Configuration

  ;General
  Name "${APPNAME}"
  OutFile "NavilIme-${RELVERSION}.exe"
  !define MUI_ICON "${NAVILIME_ICON}"
  !define MUI_UNICON "${NAVILIME_ICON}"

;--------------------------------
;Pages

  !insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "setup.bmp"

  !insertmacro MUI_PAGE_WELCOME 
  !insertmacro MUI_PAGE_LICENSE "LICENSE.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH 
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "NavilIME" SecBody
  SetOutPath "$PROGRAMFILES64\${INSTDIR}"
  SetOverwrite try
  File "${X64SRC}"
  File "${DATSRC}"
  
  File "${NAVILIME_CTRL_TOOL}"
  File "${NAVILIME_ICON}"
  
  # Start Menu
  CreateDirectory "$SMPROGRAMS\${INSTDIR}"
  CreateShortCut "$SMPROGRAMS\${INSTDIR}\Setting.lnk" "$PROGRAMFILES64\${INSTDIR}\navilIME_control.exe" "" "$PROGRAMFILES64\${INSTDIR}\Navilime.ico" 0

  SetOutPath "$PROGRAMFILES32\${INSTDIR}"
  SetOverwrite try
  File "${X86SRC}"
  File "${DATSRC}"
  
  Exec 'regsvr32 "$PROGRAMFILES64\${INSTDIR}\${DLLNAME}"'
  Exec 'regsvr32 "$PROGRAMFILES32\${INSTDIR}\${DLLNAME}"'
SectionEnd


