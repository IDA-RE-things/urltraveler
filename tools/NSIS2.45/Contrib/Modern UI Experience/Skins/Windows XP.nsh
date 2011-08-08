!ifndef MUI_EXPERIENCE_SKIN
!verbose push
!verbose 3
!define MUI_EXPERIENCE_SKIN


!insertmacro MUI_UNSET MUI_SKIN_DIR
!define MUI_SKIN_DIR "${NSISDIR}\Contrib\Modern UI Experience\Skins\Windows XP"



!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "${MUI_SKIN_DIR}\LeftBranding.bmp"
!insertmacro MUI_DEFAULT MUI_HEADERIMAGE_BITMAP "${MUI_SKIN_DIR}\Header.bmp"
!insertmacro MUI_DEFAULT MUI_BOTTOMIMAGE_BITMAP "${MUI_SKIN_DIR}\BtmImg.bmp"

!insertmacro MUI_DEFAULT MUI_TEXT_COLOR FFFFFF
!insertmacro MUI_DEFAULT MUI_TEXT_BGCOLOR 4E6FD6

!macro MUI_GUIINIT_MACRO
	!verbose push
	!verbose 3
	!insertmacro MUI_UNSET MUI_SKIN_DIR
	!define MUI_SKIN_DIR "${NSISDIR}\Contrib\Modern UI Experience\Skins\Windows XP"
	Push $1
	File "/oname=$PLUGINSDIR\Spotlight.bmp" "${MUI_SKIN_DIR}\Spotlight.bmp"
	File "/oname=$PLUGINSDIR\Key.bmp" "${MUI_SKIN_DIR}\Key.bmp"
	File "/oname=$PLUGINSDIR\BG_Header.bmp" "${MUI_SKIN_DIR}\BG_Header.bmp"
	File "/oname=$PLUGINSDIR\BG_BtmImg.bmp" "${MUI_SKIN_DIR}\BG_BtmImg.bmp"
	BgImage::SetBg /NOUNLOAD /GRADIENT 78 111 214 78 111 214
	BgImage::AddImage /NOUNLOAD "$PLUGINSDIR\Spotlight.bmp" 0 57
	BgImage::AddImage /NOUNLOAD "$PLUGINSDIR\BG_Header.bmp" 0 0
	BgImage::AddImage /NOUNLOAD "$PLUGINSDIR\BG_BtmImg.bmp" 0 -70
	BgImage::AddImage /NOUNLOAD "$PLUGINSDIR\Key.bmp" -248 -351
	CreateFont $1 Tahoma 30 700
	BgImage::AddText /NOUNLOAD "$(^Name)" $1 8 51 155 15 70 -1 -1
	BgImage::AddText /NOUNLOAD "$(^Name)" $1 255 255 255 10 65 -1 -1
	BgImage::Redraw /NOUNLOAD
	Pop $1
	!insertmacro MUI_UNSET MUI_SKIN_DIR
	!verbose pop
!macroend


Function .onGUIEnd
	# Destroy must not have /NOUNLOAD so NSIS will be able to unload
	# and delete BgImage before it exits
	BgImage::Destroy
	# Destroy doesn't return any value
FunctionEnd


!insertmacro MUI_UNSET MUI_SKIN_DIR
!verbose pop
!endif
