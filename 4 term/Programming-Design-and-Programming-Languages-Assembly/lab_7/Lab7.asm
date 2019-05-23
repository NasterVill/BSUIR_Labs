; �������� ���������, ��������������� ����������� ���������, ������� ����������� � �������� ��������. 

;**************************************************************************************************************************
;�������� 1: ���� � ��������� �����       ;
.model small                              ;
;.286                                      ;
                                          ;
.data                                     ;
                                          ;
cmd_size            db ?                  ;
                                          ;
maxCMDSize          equ 127               ;
cmd_text            db maxCMDSize + 2 dup(0)
folderPath          db maxCMDSize + 2 dup(0)
                                          ;
;Disk Transfer Area                       ;
DTAsize             equ 2Ch               ;
DTAblock            db DTAsize dup(0)     ;
                                          ;
maxWordSize         equ 50                ;
buffer              db maxWordSize + 2 dup(0)
                                          ;
PathToRequredEXE    db maxCMDSize + 15 dup(0);
newProgramCMD       db 0                  ;
                                          ;
spaceSymbol         equ ' '               ;
newLineSymbol       equ 13                ;
returnSymbol        equ 10                ;
tabulation          equ 9                 ;
                                          ;
ASCIIZendl          equ 0                 ;
                                          ;
startText           db "Program has started", '$'    ;
badCMDArgsMessage   db "Bad command-line args. 1 argument is required: folder path", '$'
endText             db "Program has ended", '$'
initToRunErrorText  db "Bad attempt to run other programs. Error code: ", '$'
runEXEErrorText     db "Error while running the other program. Error code: ", '$'
badFolderErrorText  db "Bad program container folder ", '$'
                                          ;
;EXEC Parameter Block                     ;
EPBstruct dw 0  	                      ;
	      dw offset line,0                ;
	      dw 005Ch,0,006Ch,0    	      ;������ FCB (File control block) ���������
line                db 125                ;
	                db " /?"              ;
line_text           db 122 dup(?)         ;
                                          ;
EPBlen              dw $ - EPBstruct      ;
                                          ;
findSuffix          db "*.exe", 0         ;
                                          ;
DataSize=$-cmd_size                       ;
                                          ;
.stack 100h                               ;
.code                                     ;
;**************************************************************************************************************************

;**************************************************************************************************************************
println MACRO info          ;
	push ax                 ;
	push dx                 ;
                            ;
	mov ah, 09h             ; ������� ������ 
	lea dx, info            ; �������� � dx �������� ���������� ���������
	int 21h                 ; ����� ����������� ��� ���������� ������
	                        ;
	mov dl, 0Ah            ; ������ �������� �� ����� ������
	mov ah, 02h             ; ������� ������ �������
	int 21h                 ; ����� ����������
                            ;
	mov dl, 0Dh             ; ������ �������� � ������ ������   
	mov ah, 02h             ;
	int 21h                 ;            ==//==                        
                            ;
	pop dx                  ;
	pop ax                  ;
ENDM 
;**************************************************************************************************************************

;**************************************************************************************************************************                                                                  
; ax - error code                     ;
printErrorCode MACRO nothing          ;
	add al, '0'                       ;
	mov dl, al                        ;
	mov ah, 06h                       ; ������� ��� ������
	int 21h                           ;
                                      ;
	mov dl, 0Ah                       ; ������ �������� �� ����� ������
	mov ah, 02h                       ; ������� ������ �������
	int 21h                           ; ����� ����������
                                      ;
	mov dl, 0Dh                       ; ������ �������� � ������ ������   
	mov ah, 02h                       ;
	int 21h                           ;            ==//==                                            
ENDM                                  ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                         
main:                                 ;
	;reallocating memory              ; ��������� ���������� ��������� ������ �� ��������
	mov ah, 4Ah                       ;
	mov bx, ((CodeSize/16)+1)+((DataSize/16)+1)+32	;��������� ������ � ����������
	int 21h                           ;
                                      ;
	jnc initToRunAllGood              ; ���� ��� ������������� ������ ������ �� ���������
                                      ;
	println initToRunErrorText        ;
                                      ;
	printErrorCode                    ;
                                      ;
	mov ax, 1                         ; 
                                      ;
	jmp endMain                       ;
                                      ;
initToRunAllGood:                     ;
	mov ax, @data                     ; ��������� ������� ������
	mov es, ax                        ;
                                      ;
	xor ch, ch                        ;
	mov cl, ds:[80h]		          ; ��������� � cl ����� ��������� ��������� ������
	mov cmd_size, cl 		          ; ��������� ������ ������
	mov si, 81h                       ; ������������� si �� ������ ������ ��������� ������
	mov di, offset cmd_text           ; ������������� di �� ������ cmd_text
	rep movsb                         ; ������������ ������ �� ��������� ������ � cmd_text
	                                  ; ������ � cmd_text ��, ��� ���� �������� � ��������� ������
	                                  ; � � cmd_size - ������
                                      ;
	mov ds, ax                        ;
                                      ;
	println startText                 ; ������� ������� �����
                                      ;
	call parseCMD                     ; ������ ��������� ������
	cmp ax, 0                         ;
	jne endMain                       ; �������� ������ - �������
	                                  ;
	                                  ; ������ ���������� �� ��������� � ��������� ������
	mov ah, 3Bh                       ; ��������� ��� �������� ��������� ����������
    mov dx, offset folderPath         ; � dx ��������� �������� ������� ����������
    int 21h                           ; ���������� ��� ���������� �������
    jc badWay                         ; ���������� �� ������� - ������ ������
                                      ;
	call findFirstFile                ; ������� ������ .exe ���� � ��������� ����������
	cmp ax, 0                         ; 
	jne endMain			              ; �����-�� ������ - �����
                                      ;
	call runEXE                       ; �������� ��������� ��������� ����
	cmp ax, 0                         ; 
	jne endMain			              ; �����-�� ������ - �����
                                      ;
runFile:                              ;
	call findNextFile                 ; ������� ��������� ����
	cmp ax, 0                         ;
	jne endMain			              ; �����-�� ������ - �����
                                      ;
	call runEXE                       ; �������� ��������� ��������� ����
	cmp ax, 0                         ;
	jne endMain			              ; �����-�� ������ - �����
                                      ;
	jmp runFile                       ;
                                      ;
badWay:                               ;
	println badFolderErrorText        ; ������� ��������� � ������������ ����������
endMain:                              ;                                      
	println endText                   ; ������� �������������� �����
                                      ;
	mov ah, 4Ch                       ; ������� �� ���������
	int 21h                           ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                      
cmpWordLenWith0 MACRO textline, is0Marker ; ���� ����� textline == 0, ������� � is0Marker
	push si                               ;
	mov si, offset textline               ;
	call strlen                           ;
	pop si                                ;
	cmp ax, 0                             ;
	je is0Marker                          ;
ENDM                                      ;
;**************************************************************************************************************************                                          
 
;**************************************************************************************************************************                                         
;Result in ax: 0 if all is good, else not ;
parseCMD PROC                             ;
	push bx			                      ;
	push cx                               ;
	push dx                               ; ��������� ��������
                                          ;
	mov cl, cmd_size                      ; � cl ��������� ����� ��������� ������
	xor ch, ch                            ;
                                          ; ������ �� ��������� ������
	mov si, offset cmd_text               ; � ������
	mov di, offset buffer                 ;
	call rewriteAsASCIIZWord              ;
                                          ;
	mov di, offset folderPath             ; ������ �������� ����������
	call rewriteAsASCIIZWord              ;
	cmpWordLenWith0 folderPath, badCMDArgs; ���� �������� ������� ������ - ������
                                          ;
	mov di, offset buffer                 ; ���������, ������� �� �� ������ ���������
	call rewriteAsASCIIZWord              ;
                                          ;
	cmpWordLenWith0 buffer, argsIsGood    ; ���� ������� - badCMDArgs
                                          ;
badCMDArgs:                               ;
	println badCMDArgsMessage             ; ������� ��������� �� ������
	mov ax, 1                             ; ��������� ��� 1 � ax ( ��� ������)
                                          ;
	jmp endproc                           ;
                                          ;
argsIsGood:                               ;
	mov ax, 0                             ; ���������� 0 � ax (��������� ������ �������)
                                          ;
endproc:                                  ;
	pop dx                                ;  
	pop cx                                ;
	pop bx                                ; ��������������� ��������
	ret	                                  ;
ENDP                                      ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                      
;ds:si - offset, where needed ASCIIZ string is located
;RES: ax - length                     ;
strlen PROC                           ;
	push bx si                        ;
	xor ax, ax                        ;
                                      ;
startCalc:                            ;
	mov bl, ds:[si]                   ;
	cmp bl, ASCIIZendl                ;
	je endCalc                        ;
                                      ;
	inc si                            ;
	inc ax                            ;
	jmp startCalc                     ;
	                                  ;
endCalc:                              ;
	pop si bx                         ;
	ret                               ;
ENDP                                  ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                      
;ds:si - offset, where we will find (result stop will be in si too)
;es:di - offset, where word will be
;cx - maximum size of word (input)
;result will be ASCIIZ
rewriteAsASCIIZWord PROC              ;
	push ax                           ;
	push cx                           ;
	push di                           ;
	                                  ;
loopParseWord:                        ;
	mov al, ds:[si]                   ;
	cmp al, spaceSymbol               ;
	je isStoppedSymbol                ;
                                      ;
	cmp al, newLineSymbol             ;
	je isStoppedSymbol                ;
                                      ;
	cmp al, tabulation                ;
	je isStoppedSymbol                ;
                                      ;
	cmp al, returnSymbol              ;
	je isStoppedSymbol                ;
                                      ;
	cmp al, ASCIIZendl                ;
	je isStoppedSymbol                ;
                                      ;
	mov es:[di], al                   ;
                                      ;
	inc di                            ;
	inc si                            ;
                                      ;
	loop loopParseWord                ;
                                      ;
isStoppedSymbol:                      ;
	mov al, ASCIIZendl                ;
	mov es:[di], al                   ;
	inc si                            ;
                                      ;
	pop di                            ;
	pop cx                            ;
	pop ax                            ;
	ret                               ;
ENDP                                  ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                      
runEXE PROC                               ;
	push bx                               ;
	push dx                               ;   
                                          ;
	mov ax, 4B00h			              ; 4B00h - ��������� � ��������� (4B01h - ��������� � �� ���������)
	mov bx, offset EPBstruct              ; ����� ��������� LoadExec
	mov dx, offset DTAblock + 1Eh	      ; ���������� � ����������� �����
	int 21h                               ;
	                                      ;
	jnc runEXEAllGood                     ;
                                          ;
	println runEXEErrorText               ; �������� ��������� �� �������
	printErrorCode                        ;
                                          ;
	mov ax, 1                             ; ���������� ��� ������
                                          ;
	jmp runEXEEnd                         ; ����������� �������
                                          ;
runEXEAllGood:                            ;
	mov ax, 0                             ; ���������� ��� ��������� ����������
                                          ;
runEXEEnd:                                ;
	pop dx                                ;
	pop bx                                ;
	ret                                   ;
ENDP                                      ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                          
installDTA PROC                           ;
	mov ah,1Ah                            ; �-��� DOS - ���������� ������� DTA
    mov dx, offset DTAblock               ;
    int 21h                               ;
    ret                                   ;
ENDP                                      ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                         
findFirstFile PROC                        ;
	call installDTA                       ;
                                          ;
                                          ;
    mov ah,4Eh                            ; 4Eh - ����� ������ ����������� ����
    xor cx,cx               		      ; 
    mov dx, offset findSuffix             ; ����� ������ � ������ �����
    int 21h                               ;
                                          ;
	jnc findFirstFileAllGood              ; ���� ���� ������ - ��������� � findFirstFileAllGood
                                          ;
	mov ax, 1                             ; �������� � ax ��� ������
                                          ;
	jmp findFirstFileEnd                  ;
                                          ;
findFirstFileAllGood:                     ;
	mov ax, 0                             ; �������� � ax ��� ��������� ����������
                                          ;
findFirstFileEnd:                         ;
                                          ;
	ret                                   ;
ENDP                                      ;
;**************************************************************************************************************************
 
;**************************************************************************************************************************                                         
;Result in ax: 0 if all is good, else not ;
findNextFile PROC                         ;
	call installDTA                       ;
                                          ;
	mov ah,4Fh                            ; 4Fh - ����� ��������� ����     
	                                      ; ��� ���� DTA �������� ������ ������������ ������ ������� 4E � 4F
    mov dx, offset DTAblock               ; ����� ������ � ������ �����
    int 21h                               ; ����� ���������� DTA �������� ������ � ��������� �����
                                          ;
	jnc findNextFileAllGood               ;
                                          ;
	mov ax, 1                             ; ���������� ��� ������
                                          ;
	jmp findNextFileEnd                   ;  
                                          ;
findNextFileAllGood:                      ;
	mov ax, 0                             ; ���������� �������� ���������
                                          ;
findNextFileEnd:                          ;
                                          ;
	ret                                   ;
ENDP                                      ;
                                          ;
CodeSize = $ - main                       ;
;**************************************************************************************************************************
                                          
end main                                  