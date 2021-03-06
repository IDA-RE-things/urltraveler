
Function MyDirHandler
	# R0 button
	# R1 text box
	# R2 current path
	# R3 suffix
	# R4 title

	Pop $R0

	# Recover the DriRequest associated with this button
	nsDialogs::GetUserData /NOUNLOAD $R0
	Pop $R1
	
	# Recover the current path
	${NSD_GetText} $R1 $R2 
	
	# Recover the suffix
	ReadINIStr $R3 '$EXEDIR\tmp.ini' STR:$R0 'suffix'
	ReadINIStr $R4 '$EXEDIR\tmp.ini' STR:$R0 'title'
	
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

!macro MyCreateDirRequest y title dir suffix
	${NSD_CreateButton} 95% ${y} 5% 12u ...
	Pop $R1
	
	${NSD_CreateDirRequest} 0 ${y} 93% 12u '${dir}'
	Pop $R0
	
	nsDialogs::SetUserData /NOUNLOAD $R1 $R0
	WriteINIStr '$EXEDIR\tmp.ini' STR:$R1 'title' '${title}'
	WriteINIStr '$EXEDIR\tmp.ini' STR:$R1 'suffix' '${suffix}'
	
	${NSD_OnClick} $R1 MyDirHandler
	
	Push $0 ; Return the DirRequest
!macroend

!define MyCreateDirRequest '!insertmacro MyCreateDirRequest'