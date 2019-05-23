.model small

.stack 100h

.data    
wordLen               dw  0
wordOffset            dw  0

buffer                db  0  

maxCMDSize equ 127
cmd_size              db  ?
cmd_text              db  maxCMDSize + 2 dup(0)
sourcePath            db  maxCMDSize + 2 dup(0) 
       
sourceID              dw  0
                      
maxWordSize           equ 50
spaceBuffer           db  maxWordSize + 2 dup(0)
cmd_buffer            db  maxWordSize + 2 dup(0) 
exodusWord            db  maxWordSize + 2 dup(0)
                                                  
spaceSymbol           equ ' '
dotSymbol             equ '.'
commaSymbol           equ '.'
exclamationSymbol     equ '!'
questionSymbol        equ '?'
newLineSymbol         equ 0Dh
returnSymbol          equ 0Ah
tabulation            equ 9
endl                  equ 0
                      
startText             db  "Program has started",                                                                    '$'
badCMDArgsMessage     db  "Bad command-line args. Only 2 arguments are required: source path and exodus word", '$'
badSourceText         db  "Cannot open source file",                                                                '$'
fileNotFoundText      db  "File is not found",                                                                         '$'
errorClosingSource    db  "Cannot close source file",                                                               '$'
errorWritingText      db  "Error while writing to file",                                                            '$'
endText               db  "Program has ended",                                                                      '$'
errorReadSourceText   db  "Error while reading from source file",                                                   '$'
EOF                   db  0

.code  
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
cmpWordLenWith0 MACRO textline, is0Marker       ; ��������� ������ � �����, 1 �������� - ������, 2 ������� �������� ��� 0
	push si                                     ; ���������� �������� �������� si
	                                            ;
	lea si, textline                            ; ��������� � si �������� ������, � ������� �������� ������
	call    strlen                              ; �������� �-��� ���������� ����� ������ textline, ��������� -> ax
	                                            ;
	pop si                                      ; ��������������� �������� si
	cmp ax, 0                                   ; ���������� ��������� ������ � �����
	je is0Marker                                ; ���� ����� ����� ���� -> ��������� �� ���������� ����� is0Marker
ENDM                                            ;                               
;**************************************************************************************************************************
 
;******************************************************************************************************************************       
setPosInFileTo MACRO offsetSymbolsDX, from
	push ax                     ;
	push bx                     ;
	push cx                     ;
	push dx                     ; ��������� �������� ���������    
    
    mov al, from                ;
	mov ah, 42h                 ; ���������� � ah ��� 42h - �-��� DOS ��������� ��������� �����
	mov cx, 0
	mov dx, offsetSymbolsDX		; �������� dx, �.� ��������� ��������� �� 0 �������� �� ������ ����� (cx*2^16)+dx 
	int 21h                     ; �������� ���������� DOS ��� ���������� ��������   
                                ;
	pop dx                      ; ��������������� �������� ��������� � ������� �� ���������
	pop cx                      ;
	pop bx                      ;
	pop ax                      ;
ENDM 
;******************************************************************************************************************************
 
;**************************************************************************************************************************
main:
	mov ax, @data           ; ��������� ������
	mov es, ax              ;
                            ;
	xor ch, ch              ; �������� ch
	mov cl, ds:[80h]		; ������� ��� ���������� ������ � �������� �������
	mov cmd_size, cl 		; � cmd_size ��������� ����� ��������� ������
	mov si, 81h             ;
	lea di, cmd_text        ; ��������� � di �������� ������ ����������� ����� ��������� ������
	rep movsb               ; �������� � ������ ������� ES:DI ���� �� ������ DS:SI
                            ;
	mov ds, ax              ; ��������� � ds ������
                            ;
	println startText       ; ����� ������ � ������ ������ ���������
                            ;
	call parseCMD           ; ����� ��������� �������� ��������� ������
	cmp ax, 0               ;
	jne endMain				; ���� ax != 0, �.�. ��� ��������� ��������� ��������� ������ - ��������� � ����� ���������, �.�. ������� � endMain
                            ;
	call openFiles          ; �������� ���������, ������� ��������� ��� ����� ��� ������/������
	cmp ax, 0               ;
	jne endMain				;  ==//==
                            ;
    call processingFile     ;
                            ;
	call closeFiles         ; �������� ��������� ����������, �������� ��������� �������� ������
	cmp ax, 0               ;
	jne endMain				;  ==//==
                            ;
endMain:                    ;
	println endText         ; ������� ��������� � ���������� ������ ���������
                            ;
	mov ah, 4Ch             ; ��������� � ah ��� ������� ���������� ������
	int 21h                 ; ����� ���������� DOS ��� �� ����������
;**************************************************************************************************************************

;**************************************************************************************************************************
parseCMD PROC                                   ;
	push bx                                     ;
	push cx                                     ;
	push dx                                     ; ���������
                                                ;
	mov cl, cmd_size                            ; ��������� � cl ������ �������� ������
	xor ch, ch                                  ; �������� ch
                                                ;
	lea si, cmd_text                            ; � si ��������� �������� ������ �� �������� ������
	                                            ;                                                          
	lea di, cmd_buffer                          ; � di ��������� �������� ������� ��� ��������� ������      
	call rewriteAsWord                          ;                                                           
                                                ;                                                           
	lea di, sourcePath                          ; ��������� � di �������� sourcePath �.�. ��������� ������ ��� ���������
	call rewriteAsWord                          ;
                                                ;
	cmpWordLenWith0 sourcePath, badCMDArgs      ; ���� ������, ���������� �������� ��x������ ����� �����, ������� � badCMDArgs                                                           
                                                ;                                                           
	lea di, exodusWord                          ; ��������� � di �������� exodusWord �.�. ����� ��� �������� �� ������
	call rewriteAsWord                          ;
                                                ;
	cmpWordLenWith0 exodusWord, badCMDArgs      ; ���� ������, ���������� ����� �����, ������� � badCMDArgs  	                                                                                                                          
                                                ; 
	lea di, cmd_buffer                          ; 
	call rewriteAsWord                          ;
                                                ;
	cmpWordLenWith0 buffer, goodCMDArgs         ; ���� ������ ������ ���, �.�. ����� �������� ����� � ����� ��������� ������ ������ �� ��������
                                                ; �� ����� ��������� ���������� - ������� � argsIsGood
badCMDArgs:                                     ;
	println badCMDArgsMessage                   ; ������� ��������� � �������� ���������� ��������� ������
	mov ax, 1                                   ; ���������� � ax ��� ������
                                                ;
	jmp endproc                                 ; ������� � endproc � ��������� ���������
                                                ;
goodCMDArgs:                                    ;
	mov ax, 0                                   ; ���������� � ax ��� ��������� ���������� ���������
endproc:                                        ;
	pop dx                                      ;
	pop cx                                      ;
	pop bx                                      ; ��������������� �������� ��������� � ������� �� ���������
	ret	                                        ;
ENDP
;*************************************************************************************************************************  

;*************************************************************************************************************************
setSpaceBuffer PROC
	push cx                     ;
	push si                     ;
	push di                     ; ��������� ��������  
	
	lea si, exodusWord
    call strlen	
    mov cx, ax 
    cld                                       ;����� ���� DF
    lea di, spaceBuffer 
    xor ax, ax                                ;�������� ������������ ���� 
    mov al, ' '
    rep stosb                                 ;�������� ������ ������
		       
    pop di
    pop si
    pop cx
    ret
ENDP
;*************************************************************************************************************************

;*************************************************************************************************************************
;cx - ����� ��������� ������, si - ��������, di - ��������
;��������� - ������������ �������� �� ���������� ������ 
rewriteAsWord PROC              ;
	push ax                     ;
	push cx                     ;
	push di                     ; ��������� ��������
	                            ;
loopParseWord:                  ;
	mov al, ds:[si]             ; ��������� � al ������� ������
	
	cmp al, spaceSymbol         ; -------------------
    je isStoppedSymbol          ;                                 ;
	cmp al, newLineSymbol       ;
	je isStoppedSymbol          ;         ���� ���� ������ �����            ;
	cmp al, tabulation          ;   �������, ���������, 0Ah, 0Dh ��� \0
	je isStoppedSymbol          ;     ������ �� ����� ��  ����� �����
	cmp al, returnSymbol        ;
	je isStoppedSymbol          ;                                        ;
	cmp al, endl                ;
	je isStoppedSymbol          ;
                                ;
	mov es:[di], al             ; ���� ������ ������ ������ � �����, ��������� ��� � �������������� ������
                                ;
	inc di                      ; ����������� di,si �.�. ��������� �� ��������� ������
	inc si                      ;
                                ;
	loop loopParseWord          ; ���� �� ��������� ������������ ����� �����, ������
isStoppedSymbol:                ;
	mov al, endl                ;
	mov es:[di], al             ; ��������� ������ ����� ������ � �������������� ������
	inc si                      ; ����������� si ��� �������� �� ��������� ������ ��������� ������
                                ;
	pop di                      ; ��������������� ��������
	pop cx                      ;
	pop ax                      ;
	ret                         ;
ENDP   
;**************************************************************************************************************************  
  
;*************************************************************************************************************************
;ds:si - ��������, � ������� ��������� ������ ������
;��������� - � ax ���������� ����� ������ 
strlen PROC                     ;
	push bx                     ;
	push si                     ;  ��������� ������������ ����� ��������
	                            ;
	xor ax, ax                  ;  �������� ax
                                ;
    startCalc:                  ;
	    mov bl, ds:[si]         ;  ��������� ��������� ������ ������ �� ds �� ��������� si
	    cmp bl, endl            ;  ���������� ���� ������ � �������� ����� ������
	    je endCalc              ;  ���� ��� ������ ����� ������ - ������� � endCalc � ����������� ����������
                                ;
	    inc si                  ;  ����������� si, �.�. ��������� � ���������� �������
	    inc ax                  ;  ����������� ax, �.�. ����� ������                                                     
	    jmp startCalc           ;  ����������
	                            ;
    endCalc:                    ;
	pop si                      ;
	pop bx                      ;  ��������������� ��������
	ret                         ;
ENDP                            ;
;*************************************************************************************************************************

;****************************************************************************************************************************** 
 ;ax equals 1 if all is good, or 0 if the EOF is reached, or there was an error
 readSymbol PROC
    
    push bx                         ;
	push cx                         ;
	push dx                         ; ��������� �������� ���������
                                    ;
	mov ah, 3Fh                     ; ��������� � ah ��� 3Fh - ��� �-��� ������ �� �����
	mov bx, sourceID                ; � bx ��������� ID �����, �� �������� ���������� ���������
	mov cx, 1                       ; � cx ��������� 1, ����� ������� ������
	lea dx, buffer                  ; � dx ��������� �������� �������, � ������� ����� ��������� ������ �� �����
	int 21h                         ; �������� ���������� ��� ���������� �-���
                                    ;
	jnb goodSymbol					; ���� ������ �� ����� ������� �� ��������� - ������� � goodRead
                                    ;
	println errorReadSourceText     ; ����� ������� ��������� �� ������ ������ �� �����
	mov ax, 0                       ; ���������� � ax 0                               
goodSymbol:                         ;   
	pop dx                          ; ��������������� �������� ���������
	pop cx                          ;
	pop bx                          ;
	ret                             ;
 
 ENDP
;****************************************************************************************************************************** 

;****************************************************************************************************************************** 
deleteWord PROC
    push bx                             ;
	push cx                             ;
	push dx                             ;
                                        ; ��������� �������� ���������	
	mov ax, wordLen
    mov cx, ax
	neg ax
	mov wordOffset, ax
	
	mov bx, sourceID
    setPosInFileTo wordOffset, 1        ; �������� �������� �������� ������� �� ���� ������� ����� �����
    
    xor ax, ax
    mov ah, 40h                         ; ��������� � ah ��� 40h - ��� �-��� ������ � ����
	mov bx, sourceID                    ; � bx ��������� ID �����, � ������� ����� ���������� ������
	lea dx, spaceBuffer                 ; � dx ��������� ������, ������� ����� ����������
	int 21h                             ; �������� ���������� ��� ��������� �������
     
 jnb goodWrite					      ; ���� �� ����� ������ ������ �� ���������, ������� � goodWrite
                                      ;
	println errorWritingText          ; ����� ������� ��������� � ������������� ������ ��� ������
	mov ax, 0                         ; �������� ax
                                      ;
goodWrite:                            ;
    
    xor ax, ax
    mov ah, 0Dh
    mov bx, sourceID 
    int 21h
                                      ;
	pop dx                            ; ��������������� �������� � ������� �� ���������
	pop cx                            ;
	pop bx                            ;
	ret                               ;
ENDP
;******************************************************************************************************************************  
 
;******************************************************************************************************************************    
processingFile PROC                     ;
	push ax                             ;
	push bx                             ;
	push cx                             ;
	push dx                             ;
	push si                             ;
	push di                             ; ��������� �������� ���������
                                        ;
	mov bx, sourceID                    ; ��������� � bx ID �����-���������
	setPosInFileTo 0, 0                 ; �������� �������� �������� ������� � ������ �����
                                        ;
    lea si, exodusWord                  ;                                    
    call strlen                         ;
    mov wordLen, ax                     ;
    call setSpaceBuffer 
    
processing:	
	call readSymbol
	cmp ax, 0                           ; ����������� a� � 0 ��� �������� �� ����� �����
	je finishProcessing                 ; ���� a� == 0, �� ������ ���� � �� ����� �� ����� �����  
	cmp buffer, 0                           
	je finishProcessing                
	
	xor ax, ax
	mov al, buffer
	cmp al, [si]
	je same 
	
not_same:
	lea si, exodusWord
	xor ax, ax
	mov wordOffset, ax
	jmp processing
same:    
    inc si
    mov ax, wordOffset
    add ax, 1
    mov wordOffset, ax
   
    cmp ax, wordLen
    jne processing
    
    call readSymbol  
    
    cmp ax, 0                   ; ����������� a� � 0 ��� �������� �� ����� �����
	je toDelete                 ; ���� a� == 0, �� ������ ���� � �� ����� �� ����� �����    
    
    xor ax, ax
    mov al, buffer
  	cmp al, spaceSymbol         ; -------------------
    je toDelete                 ;                                 
	cmp al, dotSymbol           ;
	je toDelete                 ;                   
	cmp al, commaSymbol         ;   
	je toDelete                 ;  
	cmp al, exclamationSymbol   ;
	je toDelete                 ;                                       
	cmp al, questionSymbol      ;
	je toDelete                 ;
	cmp al, newLineSymbol       ;
    je toDelete                 ;                                 
	cmp al, returnSymbol        ;
	je toDelete                 ;                  
	cmp al, tabulation          ;  
	je toDelete                 ;     
	cmp al, endl                ;
	je toDelete                 ; 
	
	mov bx, sourceID
    mov ax, 1
    neg ax
    mov wordOffset, ax
    setPosInFileTo wordOffset, 1
	
	jmp processing 
toDelete:
    mov bx, sourceID
    mov ax, 1
    neg ax
    mov wordOffset, ax
    setPosInFileTo wordOffset, 1
      
	call deleteWord
	jmp  processing
	                                      
finishProcessing:                       ;     
	pop di                              ; ��������������� �������� 
	pop si                              ;
	pop dx                              ;
	pop cx                              ;
	pop bx                              ;
	pop ax                              ;
	ret                                 ;
ENDP                                    ;
;**************************************************************************************************************************  
 
;**************************************************************************************************************************
;��������� � ax - 0 ���� ��� ������
openFiles PROC                  ;
	push bx                     ;
	push dx                     ; ��������� �������� ���������          
	push si                                     
	                                            
	lea si, sourcePath                          
	call    strlen                              
	    
	xor si, si         
	mov si, ax 
	sub si, 1                   
	        
	cmp sourcePath[si], 't' 
	jne checkTxt_Error     
	
	sub si, 1
	
	cmp sourcePath[si], 'x' 
	jne checkTxt_Error    
	
	sub si, 1
	
	cmp sourcePath[si], 't' 
	jne checkTxt_Error   
	
	sub si, 1
	
	cmp sourcePath[si], '.' 
	jne checkTxt_Error
	                    
	jmp checkTxt_OK                    
	checkTxt_Error: 
	pop si
	jmp badOpenSource       
	       
	checkTxt_OK:                
	pop si  
	
	;;;;;;;
                                ;
	mov ah, 3Dh			        ; ������� 3Dh - ������� ������������ ����
	mov al, 02h			        ; ����� �������� �����
	lea dx, sourcePath          ; ��������� � dx �������� ��������� �����
	mov cx, 00h			        ; 
	int 21h                     ;
                                ;
	jb badOpenSource	        ; ���� ���� �� ��������, �� ������� � badOpenSource
                                ;
	mov sourceID, ax	        ; ��������� � sourceId �������� �� ax, ���������� ��� �������� �����
                                ;
	mov ax, 0			        ; ��������� � ax 0, �.�. ������ �� ����� ���������� ��������� �� �������� 
	jmp endOpenProc		        ; ������� � endOpenProc � ��������� ������� �� ���������
                                ;
badOpenSource:                  ;
	println badSourceText       ; ������� �������������� ���������
	cmp ax, 02h                 ; ���������� ax � 02h
	jne errorFound              ; ���� ax != 02h - ���� ������, ������� � errorFound
                                ;
	println fileNotFoundText    ; ������� ��������� � ���, ��� ���� �� ������ 
                                ;
	jmp errorFound              ; ������� � errorFound
                                ;       ;       ==//==                               ;
errorFound:                     ;
	mov ax, 1                   ; ��������� � ax 1, �.�. ��������� ������
endOpenProc:                    ;
	pop dx                      ;
	pop bx                      ; ��������������� �������� ��������� � ������� �� ���������
	ret                         ;
ENDP                            ;
;******************************************************************************************************************************        
 
;**************************************************************************************************************************
closeFiles PROC                 ;
	push bx                     ;
	push cx                     ; ��������� �������� ��������� 
                                ;
	xor cx, cx                  ; �������� cx
                                ;
	mov ah, 3Eh                 ; ��������� � ah ��� 3Eh - ��� �������� �����
	mov bx, sourceID            ; � bx ��������� ID �����, ����������� ��������
	int 21h                     ; �������� ���������� ��� ���������� 
                                ;
	jnb goodCloseOfSource		; ���� ������ ��� �������� �� ���������, ������� � goodCloseOfSource
                                ;
	println errorClosingSource  ; ����� ������� �������������� ��������� �� ������       
	                            ;
	inc cx 			            ; �� ��������� � ���� �������� ������
                                ;
goodCloseOfSource:              ;               ;
	mov ax, cx 		            ; ���������� � ax �������� �� cx, ���� ������ �� ���������, �� ��� ����� 0, ����� 1
	pop cx                      ;
	pop bx                      ; ��������������� �������� ��������� � ������� �� ���������
	ret                         ;
ENDP                            ;
;****************************************************************************************************************************** 
 
END main

; c:\data.txt the