;这是用nsDialogs产生目录选择框的宏。
;使用方法：
;在脚本开头要先引用如下插件
;;;;;;;;;;;;;;;;;;;;;;;;;
;!include LogicLib.nsh
;!include nsDialogs.nsh
;!include dirrequest.nsh
;;;;;;;;;;;;;;;;;;;;;;;;;
;宏使用格式：${MyCreateDirRequest} "_Dirx" "_Diry" "_Dirwidth" "_Buttonx" "_Buttonwidth" "_Height" "_Dirtext" "_Buttontext" "_Title" "_Suffix"
;_Dirx是路径名显示框的x坐标，_Diry 显示框y坐标；_Dirwidth宽度；
;_Buttonx目录选择按钮x坐标，_Buttonwidth目录按钮宽度；_height目录显示框高度；
;_Dirtext默认显示路径；_Buttontext 按钮显示文字；
;_Title目录选择窗口标题;_suffix是选择目录后自动添加的后缀（子目录）,后缀名可以不加，这样选择目录是什么就是什么。
;共10个参数；
;By liangzi@ustc 200905

Function MyDirHandler
	# R0 button
	# R1 text box
	# R2 current path
	# R3 suffix
	# R4 title
	
	# Recover the current path
		Pop $R0
	# Recover the DriRequest associated with this button
	nsDialogs::GetUserData /NOUNLOAD $R0
	Pop $R1
	${NSD_GetText} $R1 $R2
		# Recover the suffix
	ReadINIStr $R3 '$TEMP\$R1.ini' STR:$R0 'suffix'
	ReadINIStr $R4 '$TEMP\$R1.ini' STR:$R0 'title'
	# Since the suffix directory may not exists, remove it from the path to be
	# passed to the SelectFolderDialog
	StrLen $R5 $R2 ; length of all path
	StrLen $R6 $R3 ; length of suffix
	IntOp  $R5 $R5 - $R6
	StrCpy $R6 $R2 $R6 $R5 ; Recover the last n chars of the path, where n is strlen(suffix)
	${If} $R6 == $R3 ; if the suffix is the last component of the path, remove it
		StrCpy $R2 $R2 $R5 0 
	${EndIf}
	nsDialogs::SelectFolderDialog /NOUNLOAD $R4 $R2
	Pop $R5
	${If} $R5 != error
		; When C: is selected, for example, a \ will be present at the end of the
		; path. When a directory is selected this won't happen. Verify if there is 
		; a \ at the end of the path
		StrLen $R6 $R5
		IntOp $R6 $R6 - 1
		StrCpy $R6 $R5 1 $R6 ; Get the last char
		${If} $R6 == '\'
			StrCpy $R5 '$R5$R3'
		${Else}
			StrCpy $R5 '$R5\$R3'
		${EndIf}
		; set the path in the text box
		SendMessage $R1 ${WM_SETTEXT} 0 STR:$R5

	${EndIf}

FunctionEnd

;_Dirx是路径名显示框的x坐标，_Diry 显示框y坐标；_Dirwidth宽度；
;_Buttonx目录选择按钮x坐标，_Buttonwidth目录按钮宽度；_height目录显示框高度；
;_Dirtext默认显示路径；_Buttontext 按钮显示文字；_Title目录选择窗口标题；_suffix是选择目录自动添加的后缀；
!macro MyCreateDirRequest _Dirx _Diry _Dirwidth _Buttonx _Buttonwidth _Height _Dirtext _Buttontext _Title _Suffix
  ${NSD_CreateButton} "${_Buttonx}" "${_Diry}" "${_Buttonwidth}" "${_Height}" "${_Buttontext}"
	Pop $R0
	${If} "${_Dirtext}" == ""
    ;StrCpy $1 "请选择一个目录"
    StrCpy $1 ""
  ${Else}
    StrCpy $1 "${_Dirtext}"
  ${EndIf}

	${NSD_CreateDirRequest} "${_Dirx}" "${_Diry}" "${_Dirwidth}" "${_Height}" $1
	Pop $R1
	
	nsDialogs::SetUserData /NOUNLOAD $R0 $R1
	Delete "$TEMP\$R1.ini"
	WriteINIStr '$TEMP\$R1.ini' STR:$R0 'title' '${_Title}'
	WriteINIStr '$TEMP\$R1.ini' STR:$R0 'suffix' '${_Suffix}'

;调用MyDirHandler函数时会返回$R0到堆栈
	${NSD_OnClick} $R0 MyDirHandler 

  Push $R0 ; Return the DirRequest Button
  Push $R1 ; Return the DirRequest 
!macroend
!define MyCreateDirRequest '!insertmacro MyCreateDirRequest'
