Name                 "Example"
OutFile              "Example.exe"
SetCompressor        lzma
ShowInstDetails      show

Var                  HWND

;!include LangFile.nsh
;!include             MUI.nsh
!include             MUIXP.nsh
!include             LogicLib.nsh

	;要使用不同风格的界面，取消定义的注释 
	;!define MUI_SKIN "InstallShield"
	;!define MUI_SKIN "Windows XP"
	;!define MUI_SKIN "Modern-blue"
	;!define MUI_SKIN "Orange"
	;!define MUI_SKIN "Delphi 8"
	;!define MUI_SKIN "InstallWizard"
	;!define MUI_SKIN "InstallWizard Modern"
	
	;!define MUI_COMPONENTSPAGE_NODESC
	
	;插入页面 
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_COMPONENTS
	Page custom SetCustomA
	
	!insertmacro MUI_PAGE_INSTFILES
	!insertmacro MUI_PAGE_FINISH
	
	;插入语言 
	;!insertmacro MUI_LANGUAGE English
	;!insertmacro MUI_LANGUAGE SimpChinese
	;!insertmacro MUI_LANGUAGE TradChinese

!macro Section SECTION
	Section "区段 ${SECTION}" sec${SECTION}
	    DetailPrint "Modern UI Experience example, do nothing."
	    Sleep 100
	SectionEnd
!macroend

;SectionGroup Test
!insertmacro Section 1
!insertmacro Section 2
!insertmacro Section 3
!insertmacro Section 4
!insertmacro Section 5
!insertmacro Section 6
!insertmacro Section 7
!insertmacro Section 8
!insertmacro Section 9
;SectionGroupEnd

Function .onInit
	InitPluginsDir
	File "/oname=$PLUGINSDIR\ioA.ini" ".\ioA.ini"
FunctionEnd

;演示自定义页面中如何配合主题
Function SetCustomA
	InstallOptions::initDialog /NOUNLOAD "$PLUGINSDIR\ioA.ini"
	Pop $HWND
	SetCtlColors $HWND ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	${For} $1 0 5 ;把所有控件的背景都设为 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
		GetDlgItem $0 $HWND "120$1"
		SetCtlColors $0 ${MUI_TEXT_COLOR} ${MUI_TEXT_BGCOLOR}
	${Next}
	!insertmacro MUI_HEADER_TEXT "标题" "子标题"
	InstallOptions::show
FunctionEnd

Function .onMouseOverSection
	!insertmacro MUI_DESCRIPTION_BEGIN
	
	!insertmacro MUI_DESCRIPTION_TEXT ${sec1} "区段描述 1"
	!insertmacro MUI_DESCRIPTION_TEXT ${sec2} "区段描述 2"
	!insertmacro MUI_DESCRIPTION_TEXT ${sec3} "区段描述 3"
	!insertmacro MUI_DESCRIPTION_TEXT ${sec4} "区段描述 4"
	!insertmacro MUI_DESCRIPTION_TEXT ${sec5} "区段描述 5"
	!insertmacro MUI_DESCRIPTION_TEXT ${sec6} "区段描述 6"
	!insertmacro MUI_DESCRIPTION_TEXT ${sec7} "区段描述 7"
	!insertmacro MUI_DESCRIPTION_TEXT ${sec8} "区段描述 8"
	!insertmacro MUI_DESCRIPTION_TEXT ${sec9} "区段描述 9"

	!insertmacro MUI_DESCRIPTION_END
FunctionEnd 

