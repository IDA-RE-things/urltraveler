!ifndef MUI_EXPERIENCE_SKIN
!verbose push
!verbose 3
!define MUI_EXPERIENCE_SKIN


!insertmacro MUI_UNSET MUI_SKIN_DIR
!define MUI_SKIN_DIR "${NSISDIR}\Contrib\Modern UI Experience\Skins\Delphi 8"




!insertmacro MUI_DEFAULT MUI_UI "${MUI_SKIN_DIR}\installshield.exe"
!insertmacro MUI_DEFAULT MUI_UI_COMPONENTSPAGE_NODESC "${MUI_SKIN_DIR}\installshield_nodesc.exe"

!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "${MUI_SKIN_DIR}\LeftBranding.bmp"
!insertmacro MUI_DEFAULT MUI_HEADERIMAGE_BITMAP "${MUI_SKIN_DIR}\Header.bmp"
!insertmacro MUI_DEFAULT MUI_BOTTOMIMAGE_BITMAP "${MUI_SKIN_DIR}\Bottom.bmp"

!insertmacro MUI_DEFAULT MUI_TEXT_COLOR 000000
!insertmacro MUI_DEFAULT MUI_TEXT_BGCOLOR EBE8D7




!insertmacro MUI_UNSET MUI_SKIN_DIR
!verbose pop
!endif
