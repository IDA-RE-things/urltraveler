!ifdef MUI_INCLUDED
	!error `请取消包含 MUI.nsh`
!endif

!ifdef ISUI_INCLUDED
	!error `包含了重复定义的头文件`
!endif

!echo `模仿 InstallShield 的界面`

!verbose push

!ifndef MUI_VERBOSE
	!define MUI_VERBOSE 3
!endif

!verbose ${MUI_VERBOSE}

!ifndef MUIXP_INCLUDED
!define MUIXP_INCLUDED

!verbose push
!verbose ${MUI_VERBOSE}
!include "WinMessages.nsh"
!verbose pop

Var MUI_TEMP1
Var MUI_TEMP2

;--------------------------------
;INSERT CODE

!macro MUI_INSERT

	!ifdef MUI_PRODUCT | MUI_VERSION
		!warning "The MUI_PRODUCT and MUI_VERSION defines have been removed. Use a normal Name command now."
	!endif
	
	!ifndef MUI_INSERT_INTERFACE
		!insertmacro MUI_INTERFACE
		!define MUI_INSERT_INTERFACE
	!endif
	
	!insertmacro MUI_FUNCTION_GUIINIT
	!insertmacro MUI_FUNCTION_ABORTWARNING
	
	!ifdef MUI_UNINSTALLER
		!insertmacro MUI_UNFUNCTION_GUIINIT
		!insertmacro MUI_FUNCTION_UNABORTWARNING
	!endif

!macroend

;--------------------------------
;GENERAL

!macro MUI_DEFAULT SYMBOL CONTENT

	!ifndef "${SYMBOL}"
		!define "${SYMBOL}" "${CONTENT}"
	!endif

!macroend

!macro MUI_SET SYMBOL

	!ifndef "${SYMBOL}"
		!define "${SYMBOL}"
	!endif

!macroend

!macro MUI_UNSET SYMBOL

	!ifdef "${SYMBOL}"
		!undef "${SYMBOL}"
	!endif

!macroend

!insertmacro MUI_UNSET MUI_XP_DIR
!define MUI_XP_DIR "${NSISDIR}\Contrib\Modern UI Experience"

;--------------------------------
;INTERFACE - COMPILE TIME SETTINGS

!macro MUI_INTERFACE

	!ifdef MUI_INSERT_NSISCONF
		!insertmacro MUI_NSISCONF
	!endif
	
	!ifdef MUI_SKIN
		!include  "${MUI_XP_DIR}\Skins\${MUI_SKIN}.nsh" 
	!endif 

	!insertmacro MUI_DEFAULT MUI_UI "${MUI_XP_DIR}\Skins\Default\installshield.exe"
	!insertmacro MUI_DEFAULT MUI_UI_COMPONENTSPAGE_NODESC "${MUI_XP_DIR}\Skins\Default\installshield_nodesc.exe"
	!insertmacro MUI_DEFAULT MUI_UI_HEADERIMAGE "${MUI_XP_DIR}\Skins\Default\installshield_headerbmp.exe"
	!insertmacro MUI_DEFAULT MUI_ICON "${MUI_XP_DIR}\Skins\Default\Icon.ico"
	!insertmacro MUI_DEFAULT MUI_UNICON "${MUI_XP_DIR}\Skins\Default\UnIcon.ico"
	!insertmacro MUI_DEFAULT MUI_COMPONENTSPAGE_CHECKBITMAP "${NSISDIR}\Contrib\Graphics\Checks\modern.bmp"
	!insertmacro MUI_DEFAULT MUI_LICENSEPAGE_BGCOLOR "/windows"
	!insertmacro MUI_DEFAULT MUI_INSTFILESPAGE_COLORS "/windows"
	!insertmacro MUI_DEFAULT MUI_INSTFILESPAGE_PROGRESSBAR "smooth"
	!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_INI "${MUI_XP_DIR}\ioSpecial.ini"
	!insertmacro MUI_DEFAULT MUI_UNWELCOMEFINISHPAGE_INI "${MUI_XP_DIR}\ioSpecial.ini"
	!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "${MUI_XP_DIR}\Skins\Default\LeftBranding.bmp"
	!insertmacro MUI_DEFAULT MUI_UNWELCOMEFINISHPAGE_BITMAP "${MUI_WELCOMEFINISHPAGE_BITMAP}"
	!insertmacro MUI_DEFAULT MUI_HEADERIMAGE_BITMAP "${MUI_XP_DIR}\Skins\Default\Header.bmp"
	!insertmacro MUI_DEFAULT MUI_HEADERIMAGE_UNBITMAP "${MUI_HEADERIMAGE_BITMAP}"
	!insertmacro MUI_DEFAULT MUI_BOTTOMIMAGE_BITMAP "${MUI_XP_DIR}\Skins\Default\BtmImg.bmp"
	!insertmacro MUI_DEFAULT MUI_BOTTOMIMAGE_UNBITMAP "${MUI_BOTTOMIMAGE_BITMAP}"

	!insertmacro MUI_DEFAULT MUI_TEXT_COLOR FFFFFF
	!insertmacro MUI_DEFAULT MUI_TEXT_BGCOLOR 4E6FD6
	!insertmacro MUI_DEFAULT MUI_TEXT_LIGHTCOLOR FFFFFF
	
	!ifdef MUI_BGCOLOR
		!undef MUI_BGCOLOR
		!define MUI_BGCOLOR ${MUI_TEXT_BGCOLOR}
	!endif

	XPStyle on

	ChangeUI IDD_INST "${MUI_UI_HEADERIMAGE}"
	!ifdef MUI_COMPONENTSPAGE_NODESC
		ChangeUI all "${MUI_UI_COMPONENTSPAGE_NODESC}"
	!else 
		ChangeUI all "${MUI_UI}"
	!endif 

	!ifdef MUI_BGGRADIENT
		BGGradient ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR} ${MUI_TEXT_LIGHTCOLOR}
	!endif
	
	Icon "${MUI_ICON}"
	UninstallIcon "${MUI_UNICON}"
	
	CheckBitmap "${MUI_COMPONENTSPAGE_CHECKBITMAP}"
	LicenseBkColor "${MUI_LICENSEPAGE_BGCOLOR}"
	InstallColors "${MUI_TEXT_COLOR}" "${MUI_TEXT_BGCOLOR}"
	InstProgressFlags ${MUI_INSTFILESPAGE_PROGRESSBAR}
	
	SubCaption 4 " "
	UninstallSubCaption 2 " "
	
	!insertmacro MUI_DEFAULT MUI_ABORTWARNING_TEXT "$(MUI_TEXT_ABORTWARNING)"
	!insertmacro MUI_DEFAULT MUI_UNABORTWARNING_TEXT "$(MUI_UNTEXT_ABORTWARNING)"

!macroend

;--------------------------------
;INTERFACE - RUN-TIME

!macro MUI_INNERDIALOG_TEXT CONTROL TEXT

	!verbose push
	!verbose ${MUI_VERBOSE}
	
	FindWindow $MUI_TEMP1 "#32770" "" $HWNDPARENT
	GetDlgItem $MUI_TEMP1 $MUI_TEMP1 ${CONTROL}
	SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:${TEXT}"
	
	!verbose pop

!macroend

!macro MUI_HEADER_TEXT TEXT SUBTEXT

	!verbose push
	!verbose ${MUI_VERBOSE}

	LockWindow on
	!ifdef MUI_HEADERIMAGE_BITMAP_NOSTRETCH
		SetBrandingImage /IMGID=1046 "$PLUGINSDIR\Header.bmp"
	!else
		SetBrandingImage /IMGID=1046 /RESIZETOFIT "$PLUGINSDIR\Header.bmp"
	!endif
	LockWindow off
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1037
	SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:${TEXT}"
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1038
	SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:${SUBTEXT}"
	
	!verbose pop

!macroend

!macro MUI_HEADER_TEXT_PAGE TEXT SUBTEXT

	!ifdef MUI_PAGE_HEADER_TEXT & MUI_PAGE_HEADER_SUBTEXT
		!insertmacro MUI_HEADER_TEXT "${MUI_PAGE_HEADER_TEXT}" "${MUI_PAGE_HEADER_SUBTEXT}"
	!else ifdef MUI_PAGE_HEADER_TEXT
		!insertmacro MUI_HEADER_TEXT "${MUI_PAGE_HEADER_TEXT}" "${SUBTEXT}"
	!else ifdef MUI_PAGE_HEADER_SUBTEXT
		!insertmacro MUI_HEADER_TEXT "${TEXT}" "${MUI_PAGE_HEADER_SUBTEXT}"
	!else
		!insertmacro MUI_HEADER_TEXT "${TEXT}" "${SUBTEXT}"
	!endif
	
	!insertmacro MUI_UNSET MUI_PAGE_HEADER_TEXT
	!insertmacro MUI_UNSET MUI_PAGE_HEADER_SUBTEXT

!macroend

!macro MUI_DESCRIPTION_BEGIN

  FindWindow $MUI_TEMP1 "#32770" "" $HWNDPARENT
  GetDlgItem $MUI_TEMP1 $MUI_TEMP1 1043

  StrCmp $0 -1 0 mui.description_begin_done
    SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:"
    EnableWindow $MUI_TEMP1 0
    !insertmacro MUI_INNERDIALOG_TEXT 1043 $MUI_TEXT
    Goto mui.description_done
  mui.description_begin_done:

!macroend

!macro MUI_DESCRIPTION_TEXT VAR TEXT

  !verbose push
  !verbose ${MUI_VERBOSE}

  StrCmp $0 ${VAR} 0 mui.description_${VAR}_done
    SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:"
    EnableWindow $MUI_TEMP1 1
    SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:${TEXT}"
    Goto mui.description_done
  mui.description_${VAR}_done:

  !verbose pop

!macroend

!macro MUI_DESCRIPTION_END

  !verbose push
  !verbose ${MUI_VERBOSE}

  mui.description_done:

  !verbose pop

!macroend

!macro MUI_ENDHEADER

  IfAbort mui.endheader_abort

    !ifdef MUI_INSTFILESPAGE_FINISHHEADER_TEXT & MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT
      !insertmacro MUI_HEADER_TEXT "${MUI_INSTFILESPAGE_FINISHHEADER_TEXT}" "${MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT}"
    !else ifdef MUI_INSTFILESPAGE_FINISHHEADER_TEXT
      !insertmacro MUI_HEADER_TEXT "${MUI_INSTFILESPAGE_FINISHHEADER_TEXT}" "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_SUBTITLE)"
    !else ifdef MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT
      !insertmacro MUI_HEADER_TEXT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_TITLE)" "${MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT}"
    !else
      !insertmacro MUI_HEADER_TEXT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_TITLE)" "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_SUBTITLE)"
    !endif

  Goto mui.endheader_done

  mui.endheader_abort:

    !ifdef MUI_INSTFILESPAGE_ABORTHEADER_TEXT & MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT
      !insertmacro MUI_HEADER_TEXT "${MUI_INSTFILESPAGE_ABORTHEADER_TEXT}" "${MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT}"
    !else ifdef MUI_INSTFILESPAGE_ABORTHEADER_TEXT
      !insertmacro MUI_HEADER_TEXT "${MUI_INSTFILESPAGE_ABORTHEADER_TEXT}" "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_ABORT_SUBTITLE)"
    !else ifdef MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT
      !insertmacro MUI_HEADER_TEXT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_ABORT_TITLE)" "${MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT}"
    !else
      !insertmacro MUI_HEADER_TEXT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_ABORT_TITLE)" "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_ABORT_SUBTITLE)"
    !endif

  mui.endheader_done:

!macroend

!macro MUI_ABORTWARNING

  !ifdef MUI_FINISHPAGE_ABORTWARNINGCHECK
    StrCmp $MUI_NOABORTWARNING "1" mui.quit
  !endif

  MessageBox MB_YESNO|MB_ICONEXCLAMATION "${MUI_ABORTWARNING_TEXT}" IDYES mui.quit

  Abort
  mui.quit:

!macroend

!macro MUI_UNABORTWARNING

  MessageBox MB_YESNO|MB_ICONEXCLAMATION "${MUI_UNABORTWARNING_TEXT}" IDYES mui.quit

  Abort
  mui.quit:

!macroend

!macro MUI_GUIINIT

  !insertmacro MUI_WELCOMEFINISHPAGE_INIT ""

  !insertmacro MUI_GUIINIT_BASIC ""

!macroend

!macro MUI_UNGUIINIT

  !insertmacro MUI_WELCOMEFINISHPAGE_INIT "UN"

  !insertmacro MUI_GUIINIT_BASIC "UN"

  !ifdef MUI_UNFINISHPAGE
    !ifndef MUI_UNFINISHPAGE_NOAUTOCLOSE
      SetAutoClose true
    !endif
  !endif

!macroend

!macro MUI_PAGEBG_INIT
	!ifmacrodef MUI_PAGEBG_INIT_MACRO
		!insertmacro MUI_PAGEBG_INIT_MACRO
	!endif 
	FindWindow $MUI_TEMP1 "#32770" "" $HWNDPARENT
	GetDlgItem $MUI_TEMP1 $MUI_TEMP1 1005
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_BGCOLOR} ${MUI_TEXT_BGCOLOR}
!macroend

!macro MUI_PAGECOLOR_INIT ID
	FindWindow $MUI_TEMP1 "#32770" "" $HWNDPARENT
	GetDlgItem $MUI_TEMP1 $MUI_TEMP1 ${ID}
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
!macroend

!macro MUI_GUIINIT_BASIC UNPREFIX

	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1037
	CreateFont $MUI_TEMP2 "$(^Font)" "$(^FontSize)" 700
	SendMessage $MUI_TEMP1 ${WM_SETFONT} $MUI_TEMP2 0
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_LIGHTCOLOR} transparent
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1038
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_LIGHTCOLOR} transparent
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1034
	SetCtlColors $MUI_TEMP1 "" ${MUI_TEXT_BGCOLOR}
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1039
	SetCtlColors $MUI_TEMP1 "" ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 10391
	SetCtlColors $MUI_TEMP1 "" ${MUI_TEXT_BGCOLOR}
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1028
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1256
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:$(^Branding) "
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1100
	CreateFont $MUI_TEMP2 Arial 8 700
	SendMessage $MUI_TEMP1 ${WM_SETFONT} $MUI_TEMP2 $MUI_TEMP2
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_LIGHTCOLOR} ${MUI_TEXT_BGCOLOR}
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1101
	CreateFont $MUI_TEMP2 Arial 8 350
	SendMessage $MUI_TEMP1 ${WM_SETFONT} $MUI_TEMP2 $MUI_TEMP2
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1102
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1103
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1104
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1304
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	
	IntCmp $LANGUAGE 2052 +2
	IntCmp $LANGUAGE 1028 0 eng eng
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1039
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	Goto ch
	eng:
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 10391
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 10181
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	ch:
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 2
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 3
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1300
	SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:"
	SetCtlColors $MUI_TEMP1 0xFFFFFF ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1301
	SendMessage $MUI_TEMP1 ${WM_SETTEXT} 0 "STR:"
	SetCtlColors $MUI_TEMP1 0xFFFFFF ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1044
	SetCtlColors $MUI_TEMP1 0xFFFFFF ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1018
	SetCtlColors $MUI_TEMP1 0xFFFFFF ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 10181
	SetCtlColors $MUI_TEMP1 0xFFFFFF ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1046
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_BGCOLOR} transparent
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1034
	SetCtlColors $MUI_TEMP1 "" transparent
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1044
	SetCtlColors $MUI_TEMP1 "" transparent
	
	File "/oname=$PLUGINSDIR\LeftImg.bmp" "${MUI_${UNPREFIX}WELCOMEFINISHPAGE_BITMAP}"
	!ifdef MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH
		SetBrandingImage /IMGID=1302 "$PLUGINSDIR\LeftImg.bmp"
	!else
		SetBrandingImage /IMGID=1302 /RESIZETOFIT "$PLUGINSDIR\LeftImg.bmp"
	!endif
	
	File "/oname=$PLUGINSDIR\Header.bmp" "${MUI_HEADERIMAGE_${UNPREFIX}BITMAP}"
	!ifdef MUI_HEADERIMAGE_BITMAP_NOSTRETCH
		SetBrandingImage /IMGID=1046 "$PLUGINSDIR\Header.bmp"
	!else
		SetBrandingImage /IMGID=1046 /RESIZETOFIT "$PLUGINSDIR\Header.bmp"
	!endif
	
	File "/oname=$PLUGINSDIR\BtmImg.bmp" "${MUI_BOTTOMIMAGE_${UNPREFIX}BITMAP}"
	!ifdef MUI_HEADERIMAGE_BITMAP_NOSTRETCH
		SetBrandingImage /IMGID=1039 "$PLUGINSDIR\BtmImg.bmp"
		SetBrandingImage /IMGID=10391 "$PLUGINSDIR\BtmImg.bmp"
	!else
		SetBrandingImage /IMGID=1039 /RESIZETOFIT "$PLUGINSDIR\BtmImg.bmp"
		SetBrandingImage /IMGID=10391 /RESIZETOFIT "$PLUGINSDIR\BtmImg.bmp"
	!endif
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1028
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1256
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1045
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1035
	ShowWindow $MUI_TEMP1 ${SW_HIDE}
	
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1018
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 10181
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1039
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 10391
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1028
	SetCtlColors $MUI_TEMP1 0x000000 ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1035
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	GetDlgItem $MUI_TEMP1 $HWNDPARENT 1045
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	
	!ifmacrodef MUI_GUIINIT_MACRO
		!insertmacro MUI_GUIINIT_MACRO
	!endif 
	
!macroend

!macro MUI_WELCOMEFINISHPAGE_INIT UNINSTALLER

  !ifdef MUI_${UNINSTALLER}WELCOMEPAGE | MUI_${UNINSTALLER}FINISHPAGE

    !insertmacro MUI_INSTALLOPTIONS_EXTRACT_AS "${MUI_${UNINSTALLER}WELCOMEFINISHPAGE_INI}" "ioSpecial.ini"
    File "/oname=$PLUGINSDIR\modern-wizard.bmp" "${MUI_${UNINSTALLER}WELCOMEFINISHPAGE_BITMAP}"

  !endif

!macroend

;--------------------------------
;INTERFACE - FUNCTIONS

!macro MUI_FUNCTION_GUIINIT

  Function .onGUIInit
	
	InitPluginsDir
    !insertmacro MUI_GUIINIT

    !ifdef MUI_CUSTOMFUNCTION_GUIINIT
      Call "${MUI_CUSTOMFUNCTION_GUIINIT}"
    !endif

  FunctionEnd

!macroend

!macro MUI_FUNCTION_DESCRIPTION_BEGIN

  !verbose push
  !verbose ${MUI_VERBOSE}

  !ifndef MUI_VAR_TEXT
    Var MUI_TEXT
    !define MUI_VAR_TEXT
  !endif

  Function .onMouseOverSection
    !insertmacro MUI_DESCRIPTION_BEGIN

  !verbose pop

!macroend

!macro MUI_FUNCTION_DESCRIPTION_END

  !verbose push
  !verbose ${MUI_VERBOSE}

    !insertmacro MUI_DESCRIPTION_END
  FunctionEnd

  !verbose pop

!macroend

!macro MUI_UNFUNCTION_DESCRIPTION_BEGIN

  !verbose push
  !verbose ${MUI_VERBOSE}

  Function un.onMouseOverSection
    !insertmacro MUI_DESCRIPTION_BEGIN

  !verbose pop

!macroend

!macro MUI_UNFUNCTION_DESCRIPTION_END

  !verbose push
  !verbose ${MUI_VERBOSE}

    !insertmacro MUI_DESCRIPTION_END
  FunctionEnd

  !verbose pop

!macroend

!macro MUI_FUNCTION_ABORTWARNING

  Function .onUserAbort
    !ifdef MUI_ABORTWARNING
      !insertmacro MUI_ABORTWARNING
    !endif
    !ifdef MUI_CUSTOMFUNCTION_ABORT
      Call "${MUI_CUSTOMFUNCTION_ABORT}"
    !endif
  FunctionEnd

!macroend

!macro MUI_FUNCTION_UNABORTWARNING

  Function un.onUserAbort
    !ifdef MUI_UNABORTWARNING
      !insertmacro MUI_UNABORTWARNING
    !endif
    !ifdef MUI_CUSTOMFUNCTION_UNABORT
      Call "${MUI_CUSTOMFUNCTION_UNABORT}"
    !endif
  FunctionEnd

!macroend

!macro MUI_UNFUNCTION_GUIINIT

  Function un.onGUIInit

  InitPluginsDir
  !insertmacro MUI_UNGUIINIT

  !ifdef MUI_CUSTOMFUNCTION_UNGUIINIT
    Call "${MUI_CUSTOMFUNCTION_UNGUIINIT}"
  !endif

  FunctionEnd

!macroend

!macro MUI_FUNCTIONS_DESCRIPTION_BEGIN

  ;1.65 compatibility

  !warning "Modern UI macro name has changed. Please change MUI_FUNCTIONS_DESCRIPTION_BEGIN to MUI_FUNCTION_DESCRIPTION_BEGIN."

  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN

!macroend

!macro MUI_FUNCTIONS_DESCRIPTION_END

  ;1.65 compatibility

  !warning "Modern UI macro name has changed. Please change MUI_FUNCTIONS_DESCRIPTION_END to MUI_FUNCTION_DESCRIPTION_END."

  !insertmacro MUI_FUNCTION_DESCRIPTION_END

!macroend

;--------------------------------
;START MENU FOLDER
/*
!macro MUI_STARTMENU_GETFOLDER ID VAR

  !ifdef MUI_STARTMENUPAGE_${ID}_REGISTRY_ROOT & MUI_STARTMENUPAGE_${ID}_REGISTRY_KEY & MUI_STARTMENUPAGE_${ID}_REGISTRY_VALUENAME

    ReadRegStr $MUI_TEMP1 "${MUI_STARTMENUPAGE_${ID}_REGISTRY_ROOT}" "${MUI_STARTMENUPAGE_${ID}_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_${ID}_REGISTRY_VALUENAME}"
      StrCmp $MUI_TEMP1 "" +3
        StrCpy "${VAR}" $MUI_TEMP1
        Goto +2

        StrCpy "${VAR}" "${MUI_STARTMENUPAGE_${ID}_DEFAULTFOLDER}"

   !else

     StrCpy "${VAR}" "${MUI_STARTMENUPAGE_${ID}_DEFAULTFOLDER}"

   !endif

!macroend

!macro MUI_STARTMENU_WRITE_BEGIN ID

  !verbose push
  !verbose ${MUI_VERBOSE}

  !define MUI_STARTMENUPAGE_CURRENT_ID "${ID}"

  StrCpy $MUI_TEMP1 "${MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_VARIABLE}" 1
  StrCmp $MUI_TEMP1 ">" mui.startmenu_write_${MUI_STARTMENUPAGE_CURRENT_ID}_done

  StrCmp "${MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_VARIABLE}" "" 0 mui.startmenu_writebegin_${MUI_STARTMENUPAGE_CURRENT_ID}_notempty

    !insertmacro MUI_STARTMENU_GETFOLDER "${MUI_STARTMENUPAGE_CURRENT_ID}" "${MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_VARIABLE}"

  mui.startmenu_writebegin_${MUI_STARTMENUPAGE_CURRENT_ID}_notempty:

  !verbose pop

!macroend

!macro MUI_STARTMENU_WRITE_END

  !verbose push
  !verbose ${MUI_VERBOSE}

  !ifdef MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_REGISTRY_ROOT & MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_REGISTRY_KEY & MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_REGISTRY_VALUENAME
    WriteRegStr "${MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_REGISTRY_ROOT}" "${MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_REGISTRY_VALUENAME}" "${MUI_STARTMENUPAGE_${MUI_STARTMENUPAGE_CURRENT_ID}_VARIABLE}"
  !endif

  mui.startmenu_write_${MUI_STARTMENUPAGE_CURRENT_ID}_done:

  !undef MUI_STARTMENUPAGE_CURRENT_ID

  !verbose pop

!macroend
*/
;--------------------------------
;PAGES

!macro MUI_PAGE_INIT

  !ifndef MUI_INSERT_INTERFACE
    !insertmacro MUI_INTERFACE
    !define MUI_INSERT_INTERFACE
  !endif

  !insertmacro MUI_DEFAULT MUI_PAGE_UNINSTALLER_PREFIX ""
  !insertmacro MUI_DEFAULT MUI_PAGE_UNINSTALLER_FUNCPREFIX ""

  !insertmacro MUI_UNSET MUI_UNIQUEID

  !define MUI_UNIQUEID ${__LINE__}

!macroend

!macro MUI_UNPAGE_INIT

  !ifndef MUI_UNINSTALLER
    !define MUI_UNINSTALLER
  !endif

  !define MUI_PAGE_UNINSTALLER

  !insertmacro MUI_UNSET MUI_PAGE_UNINSTALLER_PREFIX
  !insertmacro MUI_UNSET MUI_PAGE_UNINSTALLER_FUNCPREFIX

  !define MUI_PAGE_UNINSTALLER_PREFIX "UN"
  !define MUI_PAGE_UNINSTALLER_FUNCPREFIX "un."

!macroend

!macro MUI_UNPAGE_END

  !undef MUI_PAGE_UNINSTALLER
  !undef MUI_PAGE_UNINSTALLER_PREFIX
  !undef MUI_PAGE_UNINSTALLER_FUNCPREFIX

!macroend

!macro MUI_PAGE_WELCOME

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_PAGE_INIT

  !insertmacro MUI_SET MUI_${MUI_PAGE_UNINSTALLER_PREFIX}WELCOMEPAGE

  !insertmacro MUI_DEFAULT MUI_WELCOMEPAGE_TITLE "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_WELCOME_INFO_TITLE)"
  !insertmacro MUI_DEFAULT MUI_WELCOMEPAGE_TEXT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_WELCOME_INFO_TEXT)"

  !ifndef MUI_VAR_HWND
    Var MUI_HWND
    !define MUI_VAR_HWND
  !endif

  PageEx ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}custom

    PageCallbacks ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.WelcomePre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.WelcomeLeave_${MUI_UNIQUEID}

  PageExEnd

  !insertmacro MUI_FUNCTION_WELCOMEPAGE ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.WelcomePre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.WelcomeLeave_${MUI_UNIQUEID}

  !insertmacro MUI_UNSET MUI_WELCOMEPAGE_TITLE
  !insertmacro MUI_UNSET MUI_WELCOMEPAGE_TITLE_3LINES
  !insertmacro MUI_UNSET MUI_WELCOMEPAGE_TEXT

  !verbose pop

!macroend

!macro MUI_PAGE_LICENSE LICENSEDATA

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_PAGE_INIT

  !insertmacro MUI_SET MUI_${MUI_PAGE_UNINSTALLER_PREFIX}LICENSEPAGE

  !insertmacro MUI_DEFAULT MUI_LICENSEPAGE_TEXT_TOP "$(MUI_INNERTEXT_LICENSE_TOP)"
  !insertmacro MUI_DEFAULT MUI_LICENSEPAGE_BUTTON ""
  !insertmacro MUI_DEFAULT MUI_LICENSEPAGE_CHECKBOX_TEXT ""
  !insertmacro MUI_DEFAULT MUI_LICENSEPAGE_RADIOBUTTONS_TEXT_ACCEPT ""
  !insertmacro MUI_DEFAULT MUI_LICENSEPAGE_RADIOBUTTONS_TEXT_DECLINE ""

  PageEx ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}license

    PageCallbacks ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.LicensePre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.LicenseShow_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.LicenseLeave_${MUI_UNIQUEID}

    Caption " "

    LicenseData "${LICENSEDATA}"

    !ifndef MUI_LICENSEPAGE_TEXT_BOTTOM
      !ifndef MUI_LICENSEPAGE_CHECKBOX & MUI_LICENSEPAGE_RADIOBUTTONS
        LicenseText "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}INNERTEXT_LICENSE_BOTTOM)" "${MUI_LICENSEPAGE_BUTTON}"
      !else ifdef MUI_LICENSEPAGE_CHECKBOX
        LicenseText "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}INNERTEXT_LICENSE_BOTTOM_CHECKBOX)" "${MUI_LICENSEPAGE_BUTTON}"
      !else
        LicenseText "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}INNERTEXT_LICENSE_BOTTOM_RADIOBUTTONS)" "${MUI_LICENSEPAGE_BUTTON}"
      !endif
    !else
      LicenseText "${MUI_LICENSEPAGE_TEXT_BOTTOM}" "${MUI_LICENSEPAGE_BUTTON}"
    !endif

    !ifdef MUI_LICENSEPAGE_CHECKBOX
      LicenseForceSelection checkbox "${MUI_LICENSEPAGE_CHECKBOX_TEXT}"
    !else ifdef MUI_LICENSEPAGE_RADIOBUTTONS
      LicenseForceSelection radiobuttons "${MUI_LICENSEPAGE_RADIOBUTTONS_TEXT_ACCEPT}" "${MUI_LICENSEPAGE_RADIOBUTTONS_TEXT_DECLINE}"
    !endif

  PageExEnd

  !insertmacro MUI_FUNCTION_LICENSEPAGE ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.LicensePre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.LicenseShow_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.LicenseLeave_${MUI_UNIQUEID}

  !insertmacro MUI_UNSET MUI_LICENSEPAGE_TEXT_TOP
  !insertmacro MUI_UNSET MUI_LICENSEPAGE_TEXT_BOTTOM
  !insertmacro MUI_UNSET MUI_LICENSEPAGE_BUTTON
  !insertmacro MUI_UNSET MUI_LICENSEPAGE_CHECKBOX
    !insertmacro MUI_UNSET MUI_LICENSEPAGE_CHECKBOX_TEXT
  !insertmacro MUI_UNSET MUI_LICENSEPAGE_RADIOBUTTONS
    !insertmacro MUI_UNSET MUI_LICENSEPAGE_CHECKBOX_TEXT_ACCEPT
    !insertmacro MUI_UNSET MUI_LICENSEPAGE_CHECKBOX_TEXT_DECLINE

  !verbose pop

!macroend

!macro MUI_PAGE_COMPONENTS

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_PAGE_INIT

  !insertmacro MUI_SET MUI_${MUI_PAGE_UNINSTALLER_PREFIX}COMPONENTSPAGE

  !insertmacro MUI_DEFAULT MUI_COMPONENTSPAGE_TEXT_TOP ""
  !insertmacro MUI_DEFAULT MUI_COMPONENTSPAGE_TEXT_COMPLIST ""
  !insertmacro MUI_DEFAULT MUI_COMPONENTSPAGE_TEXT_INSTTYPE ""
  !insertmacro MUI_DEFAULT MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_TITLE "$(MUI_INNERTEXT_COMPONENTS_DESCRIPTION_TITLE)"
  !insertmacro MUI_DEFAULT MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_INFO "$(MUI_INNERTEXT_COMPONENTS_DESCRIPTION_INFO)"

  !ifndef MUI_VAR_TEXT
    Var MUI_TEXT
    !define MUI_VAR_TEXT
  !endif

  PageEx ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}components

    PageCallbacks ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.ComponentsPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.ComponentsShow_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.ComponentsLeave_${MUI_UNIQUEID}

    Caption " "

    ComponentText "${MUI_COMPONENTSPAGE_TEXT_TOP}" "${MUI_COMPONENTSPAGE_TEXT_INSTTYPE}" "${MUI_COMPONENTSPAGE_TEXT_COMPLIST}"

  PageExEnd

  !insertmacro MUI_FUNCTION_COMPONENTSPAGE ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.ComponentsPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.ComponentsShow_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.ComponentsLeave_${MUI_UNIQUEID}

  !undef MUI_COMPONENTSPAGE_TEXT_TOP
  !undef MUI_COMPONENTSPAGE_TEXT_COMPLIST
  !undef MUI_COMPONENTSPAGE_TEXT_INSTTYPE
  !insertmacro MUI_UNSET MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_TITLE
  !insertmacro MUI_UNSET MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_INFO

  !verbose pop

!macroend

!macro MUI_PAGE_DIRECTORY

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_PAGE_INIT

  !insertmacro MUI_SET MUI_${MUI_PAGE_UNINSTALLER_PREFIX}DIRECTORYPAGE

  !insertmacro MUI_DEFAULT MUI_DIRECTORYPAGE_TEXT_TOP ""
  !insertmacro MUI_DEFAULT MUI_DIRECTORYPAGE_TEXT_DESTINATION ""

  PageEx ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}directory

    PageCallbacks ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.DirectoryPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.DirectoryShow_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.DirectoryLeave_${MUI_UNIQUEID}

    Caption " "

    DirText "${MUI_DIRECTORYPAGE_TEXT_TOP}" "${MUI_DIRECTORYPAGE_TEXT_DESTINATION}"

    !ifdef MUI_DIRECTORYPAGE_VARIABLE
      DirVar "${MUI_DIRECTORYPAGE_VARIABLE}"
    !endif

    !ifdef MUI_DIRECTORYPAGE_VERIFYONLEAVE
      DirVerify leave
    !endif

  PageExEnd

  !insertmacro MUI_FUNCTION_DIRECTORYPAGE ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.DirectoryPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.DirectoryShow_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.DirectoryLeave_${MUI_UNIQUEID}

  !undef MUI_DIRECTORYPAGE_TEXT_TOP
  !undef MUI_DIRECTORYPAGE_TEXT_DESTINATION
  !insertmacro MUI_UNSET MUI_DIRECTORYPAGE_VARIABLE
  !insertmacro MUI_UNSET MUI_DIRECTORYPAGE_VERIFYONLEAVE

  !verbose pop

!macroend

/*
!macro MUI_PAGE_STARTMENU ID VAR

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_PAGE_INIT

  !insertmacro MUI_SET MUI_${MUI_PAGE_UNINSTALLER_PREFIX}STARTMENUPAGE

  !insertmacro MUI_DEFAULT MUI_STARTMENUPAGE_DEFAULTFOLDER "$(^Name)"
  !insertmacro MUI_DEFAULT MUI_STARTMENUPAGE_TEXT_TOP "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}INNERTEXT_STARTMENU_TOP)"
  !insertmacro MUI_DEFAULT MUI_STARTMENUPAGE_TEXT_CHECKBOX "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}INNERTEXT_STARTMENU_CHECKBOX)"

  !define MUI_STARTMENUPAGE_VARIABLE "${VAR}"
  !define "MUI_STARTMENUPAGE_${ID}_VARIABLE" "${MUI_STARTMENUPAGE_VARIABLE}"
  !define "MUI_STARTMENUPAGE_${ID}_DEFAULTFOLDER" "${MUI_STARTMENUPAGE_DEFAULTFOLDER}"
  !ifdef MUI_STARTMENUPAGE_REGISTRY_ROOT
    !define "MUI_STARTMENUPAGE_${ID}_REGISTRY_ROOT" "${MUI_STARTMENUPAGE_REGISTRY_ROOT}"
  !endif
  !ifdef MUI_STARTMENUPAGE_REGISTRY_KEY
    !define "MUI_STARTMENUPAGE_${ID}_REGISTRY_KEY" "${MUI_STARTMENUPAGE_REGISTRY_KEY}"
  !endif
  !ifdef MUI_STARTMENUPAGE_REGISTRY_VALUENAME
    !define "MUI_STARTMENUPAGE_${ID}_REGISTRY_VALUENAME" "${MUI_STARTMENUPAGE_REGISTRY_VALUENAME}"
  !endif

  PageEx ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}custom

    PageCallbacks ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.StartmenuPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.StartmenuLeave_${MUI_UNIQUEID}

    Caption " "

  PageExEnd

  !insertmacro MUI_FUNCTION_STARTMENUPAGE ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.StartmenuPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.StartmenuLeave_${MUI_UNIQUEID}

  !undef MUI_STARTMENUPAGE_VARIABLE
  !undef MUI_STARTMENUPAGE_TEXT_TOP
  !undef MUI_STARTMENUPAGE_TEXT_CHECKBOX
  !undef MUI_STARTMENUPAGE_DEFAULTFOLDER
  !insertmacro MUI_UNSET MUI_STARTMENUPAGE_NODISABLE
  !insertmacro MUI_UNSET MUI_STARTMENUPAGE_REGISTRY_ROOT
  !insertmacro MUI_UNSET MUI_STARTMENUPAGE_REGISTRY_KEY
  !insertmacro MUI_UNSET MUI_STARTMENUPAGE_REGISTRY_VALUENAME

  !verbose pop

!macroend
*/

!macro MUI_PAGE_INSTFILES

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_PAGE_INIT

  !insertmacro MUI_SET MUI_${MUI_PAGE_UNINSTALLER_PREFIX}INSTFILESPAGE

  PageEx ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}instfiles

    PageCallbacks ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.InstFilesPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.InstFilesShow_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.InstFilesLeave_${MUI_UNIQUEID}

    Caption " "

  PageExEnd

  !insertmacro MUI_FUNCTION_INSTFILESPAGE ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.InstFilesPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.InstFilesShow_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.InstFilesLeave_${MUI_UNIQUEID}

  !insertmacro MUI_UNSET MUI_INSTFILESPAGE_FINISHHEADER_TEXT
  !insertmacro MUI_UNSET MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT
  !insertmacro MUI_UNSET MUI_INSTFILESPAGE_ABORTWARNING_TEXT
  !insertmacro MUI_UNSET MUI_INSTFILESPAGE_ABORTWARNING_SUBTEXT

  !verbose pop

!macroend

!macro MUI_PAGE_FINISH

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_PAGE_INIT

  !insertmacro MUI_SET MUI_${MUI_PAGE_UNINSTALLER_PREFIX}FINISHPAGE

  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_TITLE "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_INFO_TITLE)"
  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_TEXT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_INFO_TEXT)"
  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_BUTTON "$(MUI_BUTTONTEXT_FINISH)"
  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_TEXT_REBOOT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_INFO_REBOOT)"
  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_TEXT_REBOOTNOW "$(MUI_TEXT_FINISH_REBOOTNOW)"
  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_TEXT_REBOOTLATER "$(MUI_TEXT_FINISH_REBOOTLATER)"
  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_RUN_TEXT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_RUN)"
  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_SHOWREADME_TEXT "$(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_FINISH_SHOWREADME)"
  !insertmacro MUI_DEFAULT MUI_FINISHPAGE_LINK_COLOR "000080"

  !ifndef MUI_VAR_HWND
    Var MUI_HWND
    !define MUI_VAR_HWND
  !endif

  !ifndef MUI_PAGE_UNINSTALLER
    !ifndef MUI_FINISHPAGE_NOAUTOCLOSE
      AutoCloseWindow true
    !endif
  !endif

  !ifdef MUI_FINISHPAGE_RUN | MUI_FINISHPAGE_SHOWREADME
    !ifndef MUI_FINISHPAGE_ABORTWARNINGCHECK
      !define MUI_FINISHPAGE_ABORTWARNINGCHECK
      Var MUI_NOABORTWARNING
    !endif
  !endif

  PageEx ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}custom

    PageCallbacks ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.FinishPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.FinishLeave_${MUI_UNIQUEID}

    Caption " "

  PageExEnd

  !insertmacro MUI_FUNCTION_FINISHPAGE ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.FinishPre_${MUI_UNIQUEID} ${MUI_PAGE_UNINSTALLER_FUNCPREFIX}mui.FinishLeave_${MUI_UNIQUEID}

  !insertmacro MUI_UNSET MUI_FINISHPAGE_TITLE
  !insertmacro MUI_UNSET MUI_FINISHPAGE_TITLE_3LINES
  !insertmacro MUI_UNSET MUI_FINISHPAGE_TEXT
  !insertmacro MUI_UNSET MUI_FINISHPAGE_TEXT_LARGE
  !insertmacro MUI_UNSET MUI_FINISHPAGE_BUTTON
  !insertmacro MUI_UNSET MUI_FINISHPAGE_TEXT_REBOOT
  !insertmacro MUI_UNSET MUI_FINISHPAGE_TEXT_REBOOTNOW
  !insertmacro MUI_UNSET MUI_FINISHPAGE_TEXT_REBOOTLATER
  !insertmacro MUI_UNSET MUI_FINISHPAGE_RUN
    !insertmacro MUI_UNSET MUI_FINISHPAGE_RUN_TEXT
    !insertmacro MUI_UNSET MUI_FINISHPAGE_RUN_PARAMETERS
    !insertmacro MUI_UNSET MUI_FINISHPAGE_RUN_NOTCHECKED
    !insertmacro MUI_UNSET MUI_FINISHPAGE_RUN_FUNCTION
  !insertmacro MUI_UNSET MUI_FINISHPAGE_SHOWREADME
    !insertmacro MUI_UNSET MUI_FINISHPAGE_SHOWREADME_TEXT
    !insertmacro MUI_UNSET MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
    !insertmacro MUI_UNSET MUI_FINISHPAGE_SHOWREADME_FUNCTION
  !insertmacro MUI_UNSET MUI_FINISHPAGE_LINK
    !insertmacro MUI_UNSET MUI_FINISHPAGE_LINK_LOCATION
    !insertmacro MUI_UNSET MUI_FINISHPAGE_LINK_COLOR
  !insertmacro MUI_UNSET MUI_FINISHPAGE_NOREBOOTSUPPORT
  
  !insertmacro MUI_UNSET MUI_FINISHPAGE_ABORTWARNINGCHECK

  !verbose pop

!macroend

!macro MUI_UNPAGE_WELCOME

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_UNPAGE_INIT

    !insertmacro MUI_PAGE_WELCOME

  !insertmacro MUI_UNPAGE_END

  !verbose pop

!macroend

!macro MUI_UNPAGE_CONFIRM

  !verbose push
  !verbose ${MUI_VERBOSE}

  !ifndef MUI_UNINSTALLER
    !define MUI_UNINSTALLER
  !endif

  !insertmacro MUI_PAGE_INIT

  !insertmacro MUI_SET MUI_UNCONFIRMPAGE

  !insertmacro MUI_DEFAULT MUI_UNCONFIRMPAGE_TEXT_TOP ""
  !insertmacro MUI_DEFAULT MUI_UNCONFIRMPAGE_TEXT_LOCATION ""

  PageEx un.uninstConfirm

    PageCallbacks un.mui.ConfirmPre_${MUI_UNIQUEID} un.mui.ConfirmShow_${MUI_UNIQUEID} un.mui.ConfirmLeave_${MUI_UNIQUEID}

    Caption " "

    UninstallText "${MUI_UNCONFIRMPAGE_TEXT_TOP}" "${MUI_UNCONFIRMPAGE_TEXT_LOCATION}"

  PageExEnd

  !insertmacro MUI_UNFUNCTION_CONFIRMPAGE un.mui.ConfirmPre_${MUI_UNIQUEID} un.mui.ConfirmShow_${MUI_UNIQUEID} un.mui.ConfirmLeave_${MUI_UNIQUEID}

  !insertmacro MUI_UNSET MUI_UNCONFIRMPAGE_TEXT_TOP
  !insertmacro MUI_UNSET MUI_UNCONFIRMPAGE_TEXT_LOCATION

  !verbose pop

!macroend

!macro MUI_UNPAGE_LICENSE LICENSEDATA

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_UNPAGE_INIT

    !insertmacro MUI_PAGE_LICENSE "${LICENSEDATA}"

  !insertmacro MUI_UNPAGE_END

  !verbose pop

!macroend

!macro MUI_UNPAGE_COMPONENTS

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_UNPAGE_INIT

    !insertmacro MUI_PAGE_COMPONENTS

  !insertmacro MUI_UNPAGE_END

  !verbose pop

!macroend

!macro MUI_UNPAGE_DIRECTORY

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_UNPAGE_INIT

    !insertmacro MUI_PAGE_DIRECTORY

  !insertmacro MUI_UNPAGE_END

  !verbose pop

!macroend

!macro MUI_UNPAGE_INSTFILES

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_UNPAGE_INIT

    !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_END

  !verbose pop

!macroend

!macro MUI_UNPAGE_FINISH

  !verbose push
  !verbose ${MUI_VERBOSE}

  !insertmacro MUI_UNPAGE_INIT

    !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_END

  !verbose pop

!macroend

;--------------------------------
;PAGE FUNCTIONS

!macro MUI_PAGE_FUNCTION_CUSTOM TYPE

  !ifdef MUI_PAGE_CUSTOMFUNCTION_${TYPE}
    Call "${MUI_PAGE_CUSTOMFUNCTION_${TYPE}}"
    !undef MUI_PAGE_CUSTOMFUNCTION_${TYPE}
  !endif

!macroend

!macro MUI_WELCOMEFINISHPAGE_FUNCTION_CUSTOM

  !ifdef MUI_WELCOMEFINISHPAGE_CUSTOMFUNCTION_INIT
    Call "${MUI_WELCOMEFINISHPAGE_CUSTOMFUNCTION_INIT}"
    !undef MUI_WELCOMEFINISHPAGE_CUSTOMFUNCTION_INIT
  !endif

!macroend

;欢迎页面
!macro MUI_FUNCTION_WELCOMEPAGE PRE LEAVE

  Function "${PRE}"
	
	SetBrandingImage /IMGID=1046 /RESIZETOFIT "$PLUGINSDIR\Header.bmp"
	!insertmacro MUI_PAGEBG_INIT
	
    !insertmacro MUI_WELCOMEFINISHPAGE_FUNCTION_CUSTOM

    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "NumFields" "3"
    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "NextButtonText" ""
    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "CancelEnabled" ""

    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 2" "Text" "${MUI_WELCOMEPAGE_TITLE}"

    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 2" Top 60
    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 2" Bottom 80

    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" Top 92
    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Text" "${MUI_WELCOMEPAGE_TEXT}"
    
    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 2" "Right" 290
    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Right" 290

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE

    LockWindow on
    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1256
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1035
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1037
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1038
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1045
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}
    LockWindow off

    !insertmacro MUI_INSTALLOPTIONS_INITDIALOG "ioSpecial.ini"
    Pop $MUI_HWND
    SetCtlColors $MUI_HWND ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}

	GetDlgItem $MUI_TEMP1 $MUI_HWND 1200
	SetCtlColors $MUI_TEMP1 ${MUI_TEXT_BGCOLOR} ${MUI_TEXT_BGCOLOR}
	
    GetDlgItem $MUI_TEMP1 $MUI_HWND 1201
    SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}

    CreateFont $MUI_TEMP2 Tahoma 12 700
    SendMessage $MUI_TEMP1 ${WM_SETFONT} $MUI_TEMP2 0

    GetDlgItem $MUI_TEMP1 $MUI_HWND 1202
    SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM SHOW

    !insertmacro MUI_INSTALLOPTIONS_SHOW

    LockWindow on

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1256
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1035
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1037
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1038
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1045
    ShowWindow $MUI_TEMP1 ${SW_HIDE}
    
    LockWindow off

  FunctionEnd

  Function "${LEAVE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE

  FunctionEnd

!macroend

;许可协议
!macro MUI_FUNCTION_LICENSEPAGE PRE SHOW LEAVE

  Function "${PRE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE
    !insertmacro MUI_HEADER_TEXT_PAGE $(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_LICENSE_TITLE) $(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_LICENSE_SUBTITLE)

  FunctionEnd

  Function "${SHOW}"

	LockWindow on
	!insertmacro MUI_PAGEBG_INIT
	!insertmacro MUI_PAGECOLOR_INIT 1040
	!insertmacro MUI_PAGECOLOR_INIT 1035
	!insertmacro MUI_PAGECOLOR_INIT 1034
	!insertmacro MUI_PAGECOLOR_INIT 1036
	!insertmacro MUI_PAGECOLOR_INIT 1006
	LockWindow off
	
    !insertmacro MUI_INNERDIALOG_TEXT 1040 "${MUI_LICENSEPAGE_TEXT_TOP}"
    !undef MUI_LICENSEPAGE_TEXT_TOP
    !insertmacro MUI_PAGE_FUNCTION_CUSTOM SHOW

  FunctionEnd

  Function "${LEAVE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE

  FunctionEnd

!macroend

;组件选择
!macro MUI_FUNCTION_COMPONENTSPAGE PRE SHOW LEAVE

  Function "${PRE}"
    !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE
    !insertmacro MUI_HEADER_TEXT_PAGE $(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_COMPONENTS_TITLE) $(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_COMPONENTS_SUBTITLE)
  FunctionEnd

  Function "${SHOW}"

	!insertmacro MUI_PAGEBG_INIT
	!insertmacro MUI_PAGECOLOR_INIT 10051
	!insertmacro MUI_PAGECOLOR_INIT 10052
	!insertmacro MUI_PAGECOLOR_INIT 1017
	!insertmacro MUI_PAGECOLOR_INIT 1022
	!insertmacro MUI_PAGECOLOR_INIT 1021
	!insertmacro MUI_PAGECOLOR_INIT 1023
	!insertmacro MUI_PAGECOLOR_INIT 1006
	!insertmacro MUI_PAGECOLOR_INIT 1042
	!insertmacro MUI_PAGECOLOR_INIT 1043
	
    !insertmacro MUI_INNERDIALOG_TEXT 1042 "${MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_TITLE}"

    FindWindow $MUI_TEMP1 "#32770" "" $HWNDPARENT
    GetDlgItem $MUI_TEMP1 $MUI_TEMP1 1043
    EnableWindow $MUI_TEMP1 0

    !insertmacro MUI_INNERDIALOG_TEXT 1043 "${MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_INFO}"
    StrCpy $MUI_TEXT "${MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_INFO}"
    !insertmacro MUI_PAGE_FUNCTION_CUSTOM SHOW

  FunctionEnd

  Function "${LEAVE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE

  FunctionEnd

!macroend

;目录选择
!macro MUI_FUNCTION_DIRECTORYPAGE PRE SHOW LEAVE

  Function "${PRE}"
    !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE
    !insertmacro MUI_HEADER_TEXT_PAGE $(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_DIRECTORY_TITLE) $(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_DIRECTORY_SUBTITLE)
  FunctionEnd

  Function "${SHOW}"
	!insertmacro MUI_PAGEBG_INIT
	!insertmacro MUI_PAGECOLOR_INIT 1001
	!insertmacro MUI_PAGECOLOR_INIT 1008
	!insertmacro MUI_PAGECOLOR_INIT 1006
	!insertmacro MUI_PAGECOLOR_INIT 1020
	!insertmacro MUI_PAGECOLOR_INIT 1023
	!insertmacro MUI_PAGECOLOR_INIT 1024
	
    !insertmacro MUI_PAGE_FUNCTION_CUSTOM SHOW
  FunctionEnd

  Function "${LEAVE}"
    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE
  FunctionEnd

!macroend

;开始菜单
/*
!macro MUI_FUNCTION_STARTMENUPAGE PRE LEAVE

  Function "${PRE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE

     !ifdef MUI_STARTMENUPAGE_REGISTRY_ROOT & MUI_STARTMENUPAGE_REGISTRY_KEY & MUI_STARTMENUPAGE_REGISTRY_VALUENAME

      StrCmp "${MUI_STARTMENUPAGE_VARIABLE}" "" 0 +4

      ReadRegStr $MUI_TEMP1 "${MUI_STARTMENUPAGE_REGISTRY_ROOT}" "${MUI_STARTMENUPAGE_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_REGISTRY_VALUENAME}"
        StrCmp $MUI_TEMP1 "" +2
          StrCpy "${MUI_STARTMENUPAGE_VARIABLE}" $MUI_TEMP1

    !endif

    !insertmacro MUI_HEADER_TEXT_PAGE $(MUI_TEXT_STARTMENU_TITLE) $(MUI_TEXT_STARTMENU_SUBTITLE)

    StrCmp $(^RTL) 0 mui.startmenu_nortl
      !ifndef MUI_STARTMENUPAGE_NODISABLE
        StartMenu::Select /rtl /noicon /autoadd /text "${MUI_STARTMENUPAGE_TEXT_TOP}" /lastused "${MUI_STARTMENUPAGE_VARIABLE}" /checknoshortcuts "${MUI_STARTMENUPAGE_TEXT_CHECKBOX}" "${MUI_STARTMENUPAGE_DEFAULTFOLDER}"
      !else
        StartMenu::Select /rtl /noicon /autoadd /text "${MUI_STARTMENUPAGE_TEXT_TOP}" /lastused "${MUI_STARTMENUPAGE_VARIABLE}" "${MUI_STARTMENUPAGE_DEFAULTFOLDER}"
      !endif
      Goto mui.startmenu_calldone
    mui.startmenu_nortl:
      !ifndef MUI_STARTMENUPAGE_NODISABLE
        StartMenu::Select /noicon /autoadd /text "${MUI_STARTMENUPAGE_TEXT_TOP}" /lastused "${MUI_STARTMENUPAGE_VARIABLE}" /checknoshortcuts "${MUI_STARTMENUPAGE_TEXT_CHECKBOX}" "${MUI_STARTMENUPAGE_DEFAULTFOLDER}"
      !else
        StartMenu::Select /noicon /autoadd /text "${MUI_STARTMENUPAGE_TEXT_TOP}" /lastused "${MUI_STARTMENUPAGE_VARIABLE}" "${MUI_STARTMENUPAGE_DEFAULTFOLDER}"
      !endif
    mui.startmenu_calldone:

    Pop $MUI_TEMP1
    StrCmp $MUI_TEMP1 "success" 0 +2
      Pop "${MUI_STARTMENUPAGE_VARIABLE}"

  FunctionEnd

  Function "${LEAVE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE

  FunctionEnd

!macroend
*/

;文件安装页面
!macro MUI_FUNCTION_INSTFILESPAGE PRE SHOW LEAVE

  Function "${PRE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE
    !insertmacro MUI_HEADER_TEXT_PAGE $(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_${MUI_PAGE_UNINSTALLER_PREFIX}INSTALLING_TITLE) $(MUI_${MUI_PAGE_UNINSTALLER_PREFIX}TEXT_${MUI_PAGE_UNINSTALLER_PREFIX}INSTALLING_SUBTITLE)

  FunctionEnd

  Function "${SHOW}"

	!insertmacro MUI_PAGEBG_INIT
	!insertmacro MUI_PAGECOLOR_INIT 1027
	!insertmacro MUI_PAGECOLOR_INIT 1004
	!insertmacro MUI_PAGECOLOR_INIT 1006
	!insertmacro MUI_PAGECOLOR_INIT 1016
	
    !insertmacro MUI_PAGE_FUNCTION_CUSTOM SHOW

  FunctionEnd

  Function "${LEAVE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE

    !insertmacro MUI_ENDHEADER
    !insertmacro MUI_LANGDLL_SAVELANGUAGE

  FunctionEnd

!macroend

;完成页面
!macro MUI_FUNCTION_FINISHPAGE PRE LEAVE

  Function "${PRE}"

    !insertmacro MUI_WELCOMEFINISHPAGE_FUNCTION_CUSTOM

    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "NextButtonText" "${MUI_FINISHPAGE_BUTTON}"

    !ifdef MUI_FINISHPAGE_ABORTWARNINGCHECK
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "CancelEnabled" "1"
    !endif

  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 2" Top 30
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 2" "Bottom" 58
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Top" 65

    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 2" "Text" "${MUI_FINISHPAGE_TITLE}"

    !ifdef MUI_FINISHPAGE_RUN | MUI_FINISHPAGE_SHOWREADME
       !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Bottom" 105
    !else
      !ifndef MUI_FINISHPAGE_LINK
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Bottom" 205
      !else
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Bottom" 195
      !endif
    !endif

    !ifndef MUI_FINISHPAGE_NOREBOOTSUPPORT

      IfRebootFlag 0 mui.finish_noreboot_init

		!insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Bottom" 105

        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Text" "${MUI_FINISHPAGE_TEXT_REBOOT}"

        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "Numfields" "5"

        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Type" "RadioButton"
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Text" "${MUI_FINISHPAGE_TEXT_REBOOTNOW}"
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Left" 47
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Right" "321"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Top" 110
		!insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Bottom" 121
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "State" "1"
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 5" "Type" "RadioButton"
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 5" "Text" "${MUI_FINISHPAGE_TEXT_REBOOTLATER}"
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 5" "Left" 47
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 5" "Right" "321"
	    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 5" "Top" 137
	    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 5" "Bottom" 147

        Goto mui.finish_load

      mui.finish_noreboot_init:

    !endif

    !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Text" "${MUI_FINISHPAGE_TEXT}"

    !ifdef MUI_FINISHPAGE_RUN

      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Type" "CheckBox"
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Text" "${MUI_FINISHPAGE_RUN_TEXT}"
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Left" 47
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Right" 321
	  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Top" 110
	  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Bottom" 121
      !ifndef MUI_FINISHPAGE_RUN_NOTCHECKED
        !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "State" "1"
      !endif

    !endif

    !ifdef MUI_FINISHPAGE_SHOWREADME

      !ifdef MUI_FINISHPAGE_CURFIELD_NO
        !undef MUI_FINISHPAGE_CURFIELD_NO
      !endif

      !ifndef MUI_FINISHPAGE_RUN
        !define MUI_FINISHPAGE_CURFIELD_NO 4
            !define MUI_FINISHPAGE_CURFIELD_TOP 110
            !define MUI_FINISHPAGE_CURFIELD_BOTTOM 121
      !else
        !define MUI_FINISHPAGE_CURFIELD_NO 5
            !define MUI_FINISHPAGE_CURFIELD_TOP 137
            !define MUI_FINISHPAGE_CURFIELD_BOTTOM 147
      !endif

      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Type" "CheckBox"
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Text" "${MUI_FINISHPAGE_SHOWREADME_TEXT}"
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Left" 47
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Right" 321
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Top" "${MUI_FINISHPAGE_CURFIELD_TOP}"
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Bottom" "${MUI_FINISHPAGE_CURFIELD_BOTTOM}"
      !ifndef MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
         !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "State" "1"
      !endif

    !endif

    !ifdef MUI_FINISHPAGE_LINK

      !ifdef MUI_FINISHPAGE_CURFIELD_NO
        !undef MUI_FINISHPAGE_CURFIELD_NO
      !endif

      !ifdef MUI_FINISHPAGE_RUN & MUI_FINISHPAGE_SHOWREADME
        !define MUI_FINISHPAGE_CURFIELD_NO 6
      !else ifdef MUI_FINISHPAGE_RUN | MUI_FINISHPAGE_SHOWREADME
        !define MUI_FINISHPAGE_CURFIELD_NO 5
      !else
        !define MUI_FINISHPAGE_CURFIELD_NO 4
      !endif

      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Type" "Link"
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Text" "${MUI_FINISHPAGE_LINK}"
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Left" 36
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Right" "315"
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Top" 225
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "Bottom" 235
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field ${MUI_FINISHPAGE_CURFIELD_NO}" "State" "${MUI_FINISHPAGE_LINK_LOCATION}"

    !endif

    !ifdef MUI_FINISHPAGE_RUN & MUI_FINISHPAGE_SHOWREADME & MUI_FINISHPAGE_LINK
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "Numfields" "6"
    !else ifdef MUI_FINISHPAGE_RUN & MUI_FINISHPAGE_SHOWREADME
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "Numfields" "5"
    !else ifdef MUI_FINISHPAGE_RUN & MUI_FINISHPAGE_LINK
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "Numfields" "5"
    !else ifdef MUI_FINISHPAGE_SHOWREADME & MUI_FINISHPAGE_LINK
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "Numfields" "5"
    !else ifdef MUI_FINISHPAGE_RUN | MUI_FINISHPAGE_SHOWREADME | MUI_FINISHPAGE_LINK
      !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "Numfields" "4"
    !endif

    !ifndef MUI_FINISHPAGE_NOREBOOTSUPPORT
       mui.finish_load:
    !endif

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE

    LockWindow on
    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1028
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1256
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1035
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1037
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1038
    ShowWindow $MUI_TEMP1 ${SW_HIDE}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1045
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}
    LockWindow off

    !insertmacro MUI_INSTALLOPTIONS_INITDIALOG "ioSpecial.ini"
    Pop $MUI_HWND
    SetCtlColors $MUI_HWND ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}

    GetDlgItem $MUI_TEMP1 $MUI_HWND 1200
    ShowWindow $MUI_TEMP1 0

    GetDlgItem $MUI_TEMP1 $MUI_HWND 1201
    SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}

    CreateFont $MUI_TEMP2 Tahoma "12" "700"
    SendMessage $MUI_TEMP1 ${WM_SETFONT} $MUI_TEMP2 0

    GetDlgItem $MUI_TEMP1 $MUI_HWND 1202
    SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}

    !ifndef MUI_FINISHPAGE_NOREBOOTSUPPORT

      IfRebootFlag 0 mui.finish_noreboot_show

        GetDlgItem $MUI_TEMP1 $MUI_HWND 1203
        SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}

        GetDlgItem $MUI_TEMP1 $MUI_HWND 1204
        SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}

        Goto mui.finish_show

      mui.finish_noreboot_show:

    !endif

    !ifdef MUI_FINISHPAGE_RUN
      GetDlgItem $MUI_TEMP1 $MUI_HWND 1203
      SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
    !endif

    !ifdef MUI_FINISHPAGE_SHOWREADME
      !ifndef MUI_FINISHPAGE_RUN
        GetDlgItem $MUI_TEMP1 $MUI_HWND 1203
      !else
        GetDlgItem $MUI_TEMP1 $MUI_HWND 1204
      !endif
      SetCtlColors $MUI_TEMP1 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
    !endif

    !ifdef MUI_FINISHPAGE_LINK
      !ifdef MUI_FINISHPAGE_RUN & MUI_FINISHPAGE_SHOWREADME
        GetDlgItem $MUI_TEMP1 $MUI_HWND 1205
      !else ifdef MUI_FINISHPAGE_RUN | MUI_FINISHPAGE_SHOWREADME
        GetDlgItem $MUI_TEMP1 $MUI_HWND 1204
      !else
        GetDlgItem $MUI_TEMP1 $MUI_HWND 1203
      !endif
      SetCtlColors $MUI_TEMP1 E1F0FF ${MUI_TEXT_BGCOLOR}
    !endif

    !ifndef MUI_FINISHPAGE_NOREBOOTSUPPORT
      mui.finish_show:
    !endif

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM SHOW

    !ifdef MUI_FINISHPAGE_ABORTWARNINGCHECK
      StrCpy $MUI_NOABORTWARNING "1"
    !endif

    !insertmacro MUI_INSTALLOPTIONS_SHOW

    !ifdef MUI_FINISHPAGE_ABORTWARNINGCHECK
      StrCpy $MUI_NOABORTWARNING ""
    !endif

    LockWindow on
    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1028
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1256
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1035
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1037
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1038
    ShowWindow $MUI_TEMP1 ${SW_NORMAL}

    GetDlgItem $MUI_TEMP1 $HWNDPARENT 1045
    ShowWindow $MUI_TEMP1 ${SW_HIDE}
    LockWindow off

  FunctionEnd

  Function "${LEAVE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE

    !ifndef MUI_FINISHPAGE_NOREBOOTSUPPORT

      IfRebootFlag "" mui.finish_noreboot_end

        !insertmacro MUI_INSTALLOPTIONS_READ $MUI_TEMP1 "ioSpecial.ini" "Field 4" "State"

          StrCmp $MUI_TEMP1 "1" 0 +2
            Reboot

          Return

      mui.finish_noreboot_end:

    !endif

    !ifdef MUI_FINISHPAGE_RUN

      !insertmacro MUI_INSTALLOPTIONS_READ $MUI_TEMP1 "ioSpecial.ini" "Field 4" "State"

      StrCmp $MUI_TEMP1 "1" 0 mui.finish_norun
        !ifndef MUI_FINISHPAGE_RUN_FUNCTION
          !ifndef MUI_FINISHPAGE_RUN_PARAMETERS
            Exec `"${MUI_FINISHPAGE_RUN}"`
          !else
            Exec `"${MUI_FINISHPAGE_RUN}" "${MUI_FINISHPAGE_RUN_PARAMETERS}"`
          !endif
        !else
          Call "${MUI_FINISHPAGE_RUN_FUNCTION}"
        !endif

        mui.finish_norun:

    !endif

    !ifdef MUI_FINISHPAGE_SHOWREADME

      !ifndef MUI_FINISHPAGE_RUN
        !insertmacro MUI_INSTALLOPTIONS_READ $MUI_TEMP1 "ioSpecial.ini" "Field 4" "State"
      !else
        !insertmacro MUI_INSTALLOPTIONS_READ $MUI_TEMP1 "ioSpecial.ini" "Field 5" "State"
      !endif

      StrCmp $MUI_TEMP1 "1" 0 mui.finish_noshowreadme
        !ifndef MUI_FINISHPAGE_SHOWREADME_FUNCTION
           ExecShell "open" "${MUI_FINISHPAGE_SHOWREADME}"
        !else
          Call "${MUI_FINISHPAGE_SHOWREADME_FUNCTION}"
        !endif

        mui.finish_noshowreadme:

    !endif

  FunctionEnd

!macroend

!macro MUI_UNFUNCTION_CONFIRMPAGE PRE SHOW LEAVE

  Function "${PRE}"

   !insertmacro MUI_PAGE_FUNCTION_CUSTOM PRE
   !insertmacro MUI_HEADER_TEXT_PAGE $(MUI_UNTEXT_CONFIRM_TITLE) $(MUI_UNTEXT_CONFIRM_SUBTITLE)

  FunctionEnd

  Function "${SHOW}"

	!insertmacro MUI_PAGEBG_INIT
	FindWindow $MUI_TEMP1 "#32770" "" $HWNDPARENT
	GetDlgItem $MUI_TEMP1 $MUI_TEMP1 1019
	SetCtlColors $MUI_TEMP1 0x000000 ${ISUI_TEXT_COLOR}
	
	!insertmacro MUI_PAGECOLOR_INIT 1029
	!insertmacro MUI_PAGECOLOR_INIT 1000
	!insertmacro MUI_PAGECOLOR_INIT 1006

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM SHOW

  FunctionEnd

  Function "${LEAVE}"

    !insertmacro MUI_PAGE_FUNCTION_CUSTOM LEAVE

  FunctionEnd

!macroend

;--------------------------------
;INSTALL OPTIONS (CUSTOM PAGES)

!macro MUI_INSTALLOPTIONS_EXTRACT FILE

  !verbose push
  !verbose ${MUI_VERBOSE}

  InitPluginsDir

  File "/oname=$PLUGINSDIR\${FILE}" "${FILE}"

  !insertmacro MUI_INSTALLOPTIONS_WRITE "${FILE}" "Settings" "RTL" "$(^RTL)"

  !verbose pop

!macroend

!macro MUI_INSTALLOPTIONS_EXTRACT_AS FILE FILENAME

  !verbose push
  !verbose ${MUI_VERBOSE}

  InitPluginsDir

  File "/oname=$PLUGINSDIR\${FILENAME}" "${FILE}"

  !insertmacro MUI_INSTALLOPTIONS_WRITE "${FILENAME}" "Settings" "RTL" "$(^RTL)"

  !verbose pop

!macroend

!macro MUI_INSTALLOPTIONS_DISPLAY FILE

  !verbose push
  !verbose ${MUI_VERBOSE}

  InstallOptions::dialog "$PLUGINSDIR\${FILE}"
  Pop $MUI_TEMP1

  !verbose pop

!macroend

!macro MUI_INSTALLOPTIONS_DISPLAY_RETURN FILE

  !verbose push
  !verbose ${MUI_VERBOSE}

  InstallOptions::dialog "$PLUGINSDIR\${FILE}"

  !verbose pop

!macroend

!macro MUI_INSTALLOPTIONS_INITDIALOG FILE

  !verbose push
  !verbose ${MUI_VERBOSE}

  InstallOptions::initDialog /NOUNLOAD "$PLUGINSDIR\${FILE}"

  !verbose pop

!macroend

!macro MUI_INSTALLOPTIONS_SHOW

  !verbose push
  !verbose ${MUI_VERBOSE}

  InstallOptions::show
  Pop $MUI_TEMP1

  !verbose pop

!macroend

!macro MUI_INSTALLOPTIONS_SHOW_RETURN

  !verbose push
  !verbose ${MUI_VERBOSE}

  InstallOptions::show

  !verbose pop

!macroend

!macro MUI_INSTALLOPTIONS_READ VAR FILE SECTION KEY

  !verbose push
  !verbose ${MUI_VERBOSE}

  ReadINIStr ${VAR} "$PLUGINSDIR\${FILE}" "${SECTION}" "${KEY}"

  !verbose pop

!macroend

!macro MUI_INSTALLOPTIONS_WRITE FILE SECTION KEY VALUE

  !verbose push
  !verbose ${MUI_VERBOSE}

  WriteINIStr "$PLUGINSDIR\${FILE}" "${SECTION}" "${KEY}" "${VALUE}"

  !verbose pop

!macroend

;--------------------------------
;RESERVE FILES

!macro MUI_RESERVEFILE_INSTALLOPTIONS

  !verbose push
  !verbose ${MUI_VERBOSE}

  ReserveFile "${NSISDIR}\Plugins\InstallOptions.dll"

  !verbose pop

!macroend

!macro MUI_RESERVEFILE_LANGDLL

  !verbose push
  !verbose ${MUI_VERBOSE}

  ReserveFile "${NSISDIR}\Plugins\LangDLL.dll"

  !verbose pop

!macroend

;--------------------------------
;LANGUAGES

!macro MUI_LANGUAGE LANGUAGE

  !verbose push
  !verbose ${MUI_VERBOSE}

  ;!include "${NSISDIR}\Contrib\Modern UI\Language files\${LANGUAGE}.nsh"
  !include "${NSISDIR}\Contrib\Language files\${LANGUAGE}.nsh"

  !verbose pop

!macroend

;--------------------------------
;LANGUAGE SELECTION DIALOG

!macro MUI_LANGDLL_DISPLAY

  !verbose push
  !verbose ${MUI_VERBOSE}

  !ifdef NSIS_CONFIG_SILENT_SUPPORT
    IfSilent mui.langdll_done
  !endif

  !insertmacro MUI_DEFAULT MUI_LANGDLL_WINDOWTITLE "Installer Language"
  !insertmacro MUI_DEFAULT MUI_LANGDLL_INFO "Please select a language."

  !ifdef MUI_LANGDLL_REGISTRY_ROOT & MUI_LANGDLL_REGISTRY_KEY & MUI_LANGDLL_REGISTRY_VALUENAME

    ReadRegStr $MUI_TEMP1 "${MUI_LANGDLL_REGISTRY_ROOT}" "${MUI_LANGDLL_REGISTRY_KEY}" "${MUI_LANGDLL_REGISTRY_VALUENAME}"
    StrCmp $MUI_TEMP1 "" mui.langdll_show
      StrCpy $LANGUAGE $MUI_TEMP1
      !ifndef MUI_LANGDLL_ALWAYSSHOW
        Goto mui.langdll_done
      !endif
    mui.langdll_show:

  !endif

  LangDLL::LangDialog "${MUI_LANGDLL_WINDOWTITLE}" "${MUI_LANGDLL_INFO}" A ${MUI_LANGDLL_PUSHLIST} ""

  Pop $LANGUAGE
  StrCmp $LANGUAGE "cancel" 0 +2
    Abort

  !ifdef NSIS_CONFIG_SILENT_SUPPORT
    mui.langdll_done:
  !else ifdef MUI_LANGDLL_REGISTRY_ROOT & MUI_LANGDLL_REGISTRY_KEY & MUI_LANGDLL_REGISTRY_VALUENAME
    mui.langdll_done:
  !endif

  !verbose pop

!macroend

!macro MUI_LANGDLL_SAVELANGUAGE

  !ifndef MUI_PAGE_UNINSTALLER
    
    IfAbort mui.langdllsavelanguage_abort
    
    !ifdef MUI_LANGDLL_REGISTRY_ROOT & MUI_LANGDLL_REGISTRY_KEY & MUI_LANGDLL_REGISTRY_VALUENAME
      WriteRegStr "${MUI_LANGDLL_REGISTRY_ROOT}" "${MUI_LANGDLL_REGISTRY_KEY}" "${MUI_LANGDLL_REGISTRY_VALUENAME}" $LANGUAGE
    !endif
    
    mui.langdllsavelanguage_abort:
    
  !endif

!macroend

!macro MUI_UNGETLANGUAGE

  !verbose pop

  !ifdef MUI_LANGDLL_REGISTRY_ROOT & MUI_LANGDLL_REGISTRY_KEY & MUI_LANGDLL_REGISTRY_VALUENAME

    ReadRegStr $MUI_TEMP1 "${MUI_LANGDLL_REGISTRY_ROOT}" "${MUI_LANGDLL_REGISTRY_KEY}" "${MUI_LANGDLL_REGISTRY_VALUENAME}"
    StrCmp $MUI_TEMP1 "" 0 mui.ungetlanguage_setlang

  !endif

  !insertmacro MUI_LANGDLL_DISPLAY

  !ifdef MUI_LANGDLL_REGISTRY_ROOT & MUI_LANGDLL_REGISTRY_KEY & MUI_LANGDLL_REGISTRY_VALUENAME

    Goto mui.ungetlanguage_done

    mui.ungetlanguage_setlang:
      StrCpy $LANGUAGE $MUI_TEMP1

    mui.ungetlanguage_done:

  !endif

  !verbose pop

!macroend

;--------------------------------
;LANGUAGE FILES

!macro MUI_LANGUAGEFILE_BEGIN LANGUAGE

  !ifndef MUI_INSERT
    !define MUI_INSERT
    !insertmacro MUI_INSERT
  !endif

  !ifndef "MUI_LANGUAGEFILE_${LANGUAGE}_USED"

    !define "MUI_LANGUAGEFILE_${LANGUAGE}_USED"

    LoadLanguageFile "${NSISDIR}\Contrib\Language files\${LANGUAGE}.nlf"

  !else

    !error "Modern UI language file ${LANGUAGE} included twice!"

  !endif

!macroend

!macro MUI_LANGUAGEFILE_DEFAULT NAME VALUE

  !ifndef "${NAME}"
    !define "${NAME}" "${VALUE}"
    !ifndef MUI_LANGUAGEFILE_DEFAULT_USED
      !define MUI_LANGUAGEFILE_DEFAULT_USED
    !endif
  !endif

!macroend

!macro MUI_LANGUAGEFILE_LANGSTRING NAME

  LangString "${NAME}" 0 "${${NAME}}"
  !undef "${NAME}"

!macroend

!macro MUI_LANGUAGEFILE_UNLANGSTRING NAME

  !ifdef MUI_UNINSTALLER
    LangString "${NAME}" 0 "${${NAME}}"
    !undef "${NAME}"
  !else
    !undef "${NAME}"
  !endif

!macroend

!macro MUI_LANGUAGEFILE_LANGSTRING_PAGE PAGE NAME

  !ifdef MUI_${PAGE}PAGE
    LangString "${NAME}" 0 "${${NAME}}"
    !undef "${NAME}"
  !else
    !undef "${NAME}"
  !endif

!macroend

!macro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE PAGE NAME

  !ifdef MUI_UNINSTALLER
    !ifdef MUI_UN${PAGE}PAGE
      LangString "${NAME}" 0 "${${NAME}}"
      !undef "${NAME}"
    !else
      !undef "${NAME}"
    !endif
  !else
    !undef "${NAME}"
  !endif

!macroend

!macro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE PAGE NAME

  !ifdef MUI_${PAGE}PAGE | MUI_UN${PAGE}PAGE
    LangString "${NAME}" 0 "${${NAME}}"
    !undef "${NAME}"
  !else
    !undef "${NAME}"
  !endif

!macroend

!macro MUI_LANGUAGEFILE_LANGSTRING_DEFINE DEFINE NAME

  !ifdef "${DEFINE}"
    LangString "${NAME}" 0 "${${NAME}}"
  !endif
  !undef "${NAME}"

!macroend

!macro MUI_LANGUAGEFILE_DEFINE DEFINE NAME

  !ifndef "${DEFINE}"
    !define "${DEFINE}" "${${NAME}}"
  !endif
  !undef "${NAME}"

!macroend

!macro MUI_LANGUAGEFILE_END

  !include "${NSISDIR}\Contrib\Modern UI\Language files\Default.nsh"
  !ifdef MUI_LANGUAGEFILE_DEFAULT_USED
    !undef MUI_LANGUAGEFILE_DEFAULT_USED
    !warning "${LANGUAGE} Modern UI language file version doesn't match. Using default English texts for missing strings."
  !endif

  !insertmacro MUI_LANGUAGEFILE_DEFINE "MUI_${LANGUAGE}_LANGNAME" "MUI_LANGNAME"

  !ifndef MUI_LANGDLL_PUSHLIST
    !define MUI_LANGDLL_PUSHLIST "'${MUI_${LANGUAGE}_LANGNAME}' ${LANG_${LANGUAGE}} "
  !else
    !ifdef MUI_LANGDLL_PUSHLIST_TEMP
      !undef MUI_LANGDLL_PUSHLIST_TEMP
    !endif
    !define MUI_LANGDLL_PUSHLIST_TEMP "${MUI_LANGDLL_PUSHLIST}"
    !undef MUI_LANGDLL_PUSHLIST
    !define MUI_LANGDLL_PUSHLIST "'${MUI_${LANGUAGE}_LANGNAME}' ${LANG_${LANGUAGE}} ${MUI_LANGDLL_PUSHLIST_TEMP}"
  !endif

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE WELCOME "MUI_TEXT_WELCOME_INFO_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE WELCOME "MUI_TEXT_WELCOME_INFO_TEXT"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE LICENSE "MUI_TEXT_LICENSE_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE LICENSE "MUI_TEXT_LICENSE_SUBTITLE"
  !insertmacro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE LICENSE "MUI_INNERTEXT_LICENSE_TOP"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE LICENSE "MUI_INNERTEXT_LICENSE_BOTTOM"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE LICENSE "MUI_INNERTEXT_LICENSE_BOTTOM_CHECKBOX"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE LICENSE "MUI_INNERTEXT_LICENSE_BOTTOM_RADIOBUTTONS"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE COMPONENTS "MUI_TEXT_COMPONENTS_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE COMPONENTS "MUI_TEXT_COMPONENTS_SUBTITLE"
  !insertmacro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE COMPONENTS "MUI_INNERTEXT_COMPONENTS_DESCRIPTION_TITLE"
  !insertmacro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE COMPONENTS "MUI_INNERTEXT_COMPONENTS_DESCRIPTION_INFO"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE DIRECTORY "MUI_TEXT_DIRECTORY_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE DIRECTORY "MUI_TEXT_DIRECTORY_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE STARTMENU "MUI_TEXT_STARTMENU_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE STARTMENU "MUI_TEXT_STARTMENU_SUBTITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE STARTMENU "MUI_INNERTEXT_STARTMENU_TOP"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE STARTMENU "MUI_INNERTEXT_STARTMENU_CHECKBOX"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE INSTFILES "MUI_TEXT_INSTALLING_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE INSTFILES "MUI_TEXT_INSTALLING_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE INSTFILES "MUI_TEXT_FINISH_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE INSTFILES "MUI_TEXT_FINISH_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE INSTFILES "MUI_TEXT_ABORT_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE INSTFILES "MUI_TEXT_ABORT_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE FINISH "MUI_BUTTONTEXT_FINISH"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE FINISH "MUI_TEXT_FINISH_INFO_TITLE"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE FINISH "MUI_TEXT_FINISH_INFO_TEXT"
  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_PAGE FINISH "MUI_TEXT_FINISH_INFO_REBOOT"
  !insertmacro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE FINISH "MUI_TEXT_FINISH_REBOOTNOW"
  !insertmacro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE FINISH "MUI_TEXT_FINISH_REBOOTLATER"
  !insertmacro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE FINISH "MUI_TEXT_FINISH_RUN"
  !insertmacro MUI_LANGUAGEFILE_MULTILANGSTRING_PAGE FINISH "MUI_TEXT_FINISH_SHOWREADME"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_DEFINE MUI_ABORTWARNING "MUI_TEXT_ABORTWARNING"


  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE WELCOME "MUI_UNTEXT_WELCOME_INFO_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE WELCOME "MUI_UNTEXT_WELCOME_INFO_TEXT"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE CONFIRM "MUI_UNTEXT_CONFIRM_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE CONFIRM "MUI_UNTEXT_CONFIRM_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE LICENSE "MUI_UNTEXT_LICENSE_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE LICENSE "MUI_UNTEXT_LICENSE_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE LICENSE "MUI_UNINNERTEXT_LICENSE_BOTTOM"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE LICENSE "MUI_UNINNERTEXT_LICENSE_BOTTOM_CHECKBOX"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE LICENSE "MUI_UNINNERTEXT_LICENSE_BOTTOM_RADIOBUTTONS"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE COMPONENTS "MUI_UNTEXT_COMPONENTS_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE COMPONENTS "MUI_UNTEXT_COMPONENTS_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE DIRECTORY "MUI_UNTEXT_DIRECTORY_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE DIRECTORY  "MUI_UNTEXT_DIRECTORY_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE INSTFILES "MUI_UNTEXT_UNINSTALLING_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE INSTFILES "MUI_UNTEXT_UNINSTALLING_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE INSTFILES "MUI_UNTEXT_FINISH_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE INSTFILES "MUI_UNTEXT_FINISH_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE INSTFILES "MUI_UNTEXT_ABORT_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE INSTFILES "MUI_UNTEXT_ABORT_SUBTITLE"

  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE FINISH "MUI_UNTEXT_FINISH_INFO_TITLE"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE FINISH "MUI_UNTEXT_FINISH_INFO_TEXT"
  !insertmacro MUI_LANGUAGEFILE_UNLANGSTRING_PAGE FINISH "MUI_UNTEXT_FINISH_INFO_REBOOT"

  !insertmacro MUI_LANGUAGEFILE_LANGSTRING_DEFINE MUI_UNABORTWARNING "MUI_UNTEXT_ABORTWARNING"

!macroend

;--------------------------------
;END

!endif

!verbose pop










