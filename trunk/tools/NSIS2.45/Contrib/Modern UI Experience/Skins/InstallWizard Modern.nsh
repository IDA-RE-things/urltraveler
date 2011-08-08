!ifndef MUI_EXPERIENCE_SKIN
!verbose push
!verbose 3
!define MUI_EXPERIENCE_SKIN

!insertmacro MUI_UNSET MUI_SKIN_DIR
!define MUI_SKIN_DIR "${NSISDIR}\Contrib\Modern UI Experience\Skins\InstallWizard Modern"

!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "${MUI_SKIN_DIR}\LeftBranding.bmp"
!insertmacro MUI_DEFAULT MUI_HEADERIMAGE_BITMAP "${MUI_SKIN_DIR}\Header.bmp"
!insertmacro MUI_DEFAULT MUI_BOTTOMIMAGE_BITMAP "${MUI_SKIN_DIR}\Bottom.bmp"

!insertmacro MUI_DEFAULT MUI_TEXT_COLOR FFFFFF
!insertmacro MUI_DEFAULT MUI_TEXT_BGCOLOR 566978

!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH ""


!insertmacro MUI_UNSET MUI_SKIN_DIR
!verbose pop
!endif
