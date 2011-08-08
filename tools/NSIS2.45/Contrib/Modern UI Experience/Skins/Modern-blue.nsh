!ifndef MUI_EXPERIENCE_SKIN
!verbose push
!verbose 3
!define MUI_EXPERIENCE_SKIN


!insertmacro MUI_UNSET MUI_SKIN_DIR
!define MUI_SKIN_DIR "${NSISDIR}\Contrib\Modern UI Experience\Skins\Modern-blue"




!insertmacro MUI_DEFAULT MUI_ICON "${MUI_SKIN_DIR}\Icon.ico"
!insertmacro MUI_DEFAULT MUI_UNICON "${MUI_SKIN_DIR}\UnIcon.ico"
	
!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "${MUI_SKIN_DIR}\LeftBranding.bmp"
!insertmacro MUI_DEFAULT MUI_HEADERIMAGE_BITMAP "${MUI_SKIN_DIR}\Header.bmp"
!insertmacro MUI_DEFAULT MUI_BOTTOMIMAGE_BITMAP "${MUI_SKIN_DIR}\Bottom.bmp"

!insertmacro MUI_DEFAULT MUI_TEXT_COLOR 4000FF
!insertmacro MUI_DEFAULT MUI_TEXT_BGCOLOR 9FC0E1




!insertmacro MUI_UNSET MUI_SKIN_DIR
!verbose pop
!endif
