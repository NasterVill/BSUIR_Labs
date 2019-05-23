.model tiny                ;
.code  
.386                    ;
org 100h               ; ������ PSP
                           ;
start:                     ;
	jmp main               ;
;
                           
maxCMDSize equ 127
maxBuffSize equ 25
cmd_size              db  ?
cmd_text              db  maxCMDSize + 2 dup(0) 
buffer                db  maxBuffSize + 2 dup(0)
sourcePath            db  maxCMDSize + 2 dup(0)        
sourceID              dw  0
interval              dw  maxBuffSize

fileOffset dw 0
amount dw 0

spaceSymbol           equ ' '
newLineSymbol         equ 0Dh
returnSymbol          equ 0Ah
endl                  equ 0

badSourceText         db  "Cannot open source file",                                                                '$'   
fileNotFoundText      db  "File is not found",                                                                      '$'
badCMDArgsMessage db "Bad command-line arguments! Only one argument is required", '$'
startText db "The begining of the program", '$' 
                                      ;
intOldHandler dd 0                    ;
                                      
;**************************************************************************************************************************
handler PROC                          ; ����� ���������� ����������
	pushf  
	call    cs:intOldHandler                           ;                              
	push ds                           ;
    push es                           ;
	push ax                           ;
	push bx                           ; ��������� ��������
    push cx                           ;
    push dx                           ;
	push di                           ;
                                      ;
	push cs                           ;
	pop ds                            ;
                                      ;
    in  al, 60H         
    cmp al, 39h                       ; space scan code    
    je escRoutine
    cmp al, 48h                       ; up scan code        
    je upRoutine
    cmp al, 50h                       ; down scan code   
    je downRoutine 
     
    pop     di                       ; �������������� �������������� ���������
    pop     dx
    pop     cx
    pop     bx
    pop     ax
    pop     es
    pop     ds
    jmp dword ptr cs:intOldHandler   ;�� ������ ������    
     
upRoutine: 
    mov ah, 3Fh                     ; ��������� � ah ��� 3Fh - ��� �-��� ������ �� �����
	mov bx, sourceID                ; � bx ��������� ID �����, �� �������� ���������� ���������
	mov cx, maxBuffSize             ; � cx ��������� ������������ ������ �����, �.�. ��������� ������������ ���-�� �������� (maxWordSize ��������)
	lea dx, buffer                  ; � dx ��������� �������� �������, � ������� ����� ��������� ������ �� �����
	int 21h                         ; �������� ���������� ��� ���������� �-���
	
	cmp ax, 0
	jbe endHandler     
     
    call printToVM 
    
    add ax, fileOffset
    mov fileOffset, ax 
    jmp endHandler
    
downRoutine:
    mov ax, fileOffset
    sub ax, maxBuffSize
    
    cmp ax, 0
    ja skip 
    
    mov ax, 0
    mov fileOffset, ax 
skip:      
    mov al, 0                ;
	mov ah, 42h              ; ���������� � ah ��� 42h - �-��� DOS ��������� ��������� �����
	mov cx, 0
	mov dx, fileOffset		 
	int 21h                  ; �������� ���������� DOS ��� ���������� ��������  

	
    mov ah, 3Fh                     ; ��������� � ah ��� 3Fh - ��� �-��� ������ �� �����
	mov bx, sourceID                ; � bx ��������� ID �����, �� �������� ���������� ���������
	mov cx, maxBuffSize             ; � cx ��������� ������������ ������ �����, �.�. ��������� ������������ ���-�� �������� (maxWordSize ��������)
	lea dx, buffer                  ; � dx ��������� �������� �������, � ������� ����� ��������� ������ �� �����
	int 21h                         ; �������� ���������� ��� ���������� �-���
	
	cmp ax, 0
	jbe endHandler     
    
    call printToVM 
    
    add ax, fileOffset
    mov fileOffset, ax 
    jmp endHandler
    
endHandler:
    in al,61h ;������ ���������� ����� �
    or al,80h ;���������� ����� ����, �������
    out 61h,al ;��� 80h � ����������� ����� �
    and al,7Fh ;����� �������� ������ ����������.
    out 61h,al ;������� � ����� � ��� 80h .
                           ;
    mov al, 20h                       ;
    out 20h, al                       ; 
    
	pop di                            ;
	pop dx                            ;
	pop cx                            ;
	pop bx                            ; ��������������� ��������
	pop ax                            ;
	pop es                            ;
	pop ds	                          
	jmp intEnd    
       
escRoutine:    
    ;��������������� ������ 09h
	
	mov ah, 3Eh                 ; ��������� � ah ��� 3Eh - ��� �������� �����
    mov bx, sourceID            ; � bx ��������� ID �����, ����������� ��������
	int 21h                     ; �������� ���������� ��� ����������     
	
	in al,61h ;������ ���������� ����� �
    or al,80h ;���������� ����� ����, �������
    out 61h,al ;��� 80h � ����������� ����� �
    and al,7Fh ;����� �������� ������ ����������.
    out 61h,al ;������� � ����� � ��� 80h .
                           ;
    mov al, 20h                       ;
    out 20h, al                       ; 
    
	pop di                            ;
	pop dx                            ;
	pop cx                            ;
	pop bx                            ; ��������������� ��������
	pop ax                            ;
	pop es                            ;
	pop ds	                          
	
    mov ax,2509h
    mov dx,word ptr cs:[intOldHandler]
    mov ds,word ptr cs:[intOldHandler+2]
    int 21h 
    
    mov es,cs:2ch ; ������� �� PSP ����� ������������ 
    mov ah,49h ; ��������� ��������� � �������� ��� 
     int 21h ;  
    
    push cs ; �������� ������ ���� ��������� 
    pop es ; 
    mov ah,49h ; 
    int 21h ;      
    
    jmp intEnd                    
    
intEnd:     
	iret                              ;
ENDP                                  ;
;**************************************************************************************************************************   


;**************************************************************************************************************************
;ax -- ����� ��������� ��� ������ �� �����
printToVM proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push ax 
   
    
    mov ax, 0b800h
    mov es, ax
    lea si, buffer
    ;mov di, 0 
    
    ;mov cx, maxBuffSize
    ;xor ax, ax
    ;rep stosw
    
    mov di, 0
     
    pop cx
loop2:
    mov al, [si] 
    mov es:[di], al
    inc si
    add di, 2
    loop loop2
    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax       
    ret
endp 
;**************************************************************************************************************************
programLength:

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
;��������� � ax - 0 ���� ��� ������
openFile PROC                  ;
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
                       
;********************************************************************************************************************    
 parseCMD PROC                         ;
	push bx                           ;   
	push cx                           ;
	push dx                           ; ��������� �������� ���������
	push si                           ;
	push di                           ;
                                      ;
	cld                               ;
	mov bx, 80h                       ;
	mov cl, cs:[bx]                   ; ��������� � ��������, ��� ���������� ����� ��������� ������
	xor ch, ch                        ; � cl ��������� ����� ��������� ������
                                      ;
	xor dx, dx                        ;
	mov di, 81h                       ;
                                      ;
	mov al, ' '                       ; ���������� ��� �������
	repne scasb	                      ; ����� ����, �� ������ al � ����� �� cx ���� �� ������ es:di 
	
	mov al, ' '
    repe scasb     
    dec di     
    inc cx
	
	xor ax, ax                        ;                                                   
                                      ;
	mov si, di                        ; ��������� � si ��������, � �������� ���������� ���������
	mov di, offset sourcePath          ; �������� ������� � sourcePath  
	
	mov al, [si]
	cmp al, ' '
    je badCMDArgs 
    cmp al, 0Dh
    je badCMDArgs
	
loop1:
    mov al, [si]
    cmp al, ' '
    je endFound 
    cmp al, 0Dh
    je endFound
    
    mov [di], al
    inc di
    inc si 
    jmp loop1 
    
badCMDArgs:
    println badCMDArgsMessage 
    xor ax, ax
    jmp endParse   
      
endFound:     
    mov al, endl
    mov [di], al                ; ��������� ������ ����� ������ � �������������� ������ 
    mov ax, 0
    
endParse:                       ;
	pop di
	pop si
	pop dx                      ; ��������������� ��������
	pop cx                      ;
	pop bx                      ;
	ret                         ;  
endp
;********************************************************************************************************************   	
	
	
                                      ;                        
 ;********************************************************************************************************************   
setHandler PROC                       ; ��������� ������ ����������� ����������. ��������� ax=0 ���� ��� ������, ����� ax!=0 
	push bx                           ;
	push dx                           ; ��������� �������� ���������
                                      ;
	cli                               ; ��������� ���������� (������/���������� ���������� ��� ���������� ��������� ������ ����������� )
                                      ;
	mov ah, 35h                       ; ������� ��������� ������ ����������� ����������
	mov al, 09h                       ; ����������, ���������� �������� ���������� �������� (09h - ���������� ����������)
	int 21h                           ; �������� ���������� ��� ��������� ������� 
                                      ; � ���������� ���������� ������� � es:bx ���������� ����� �������� ����������� ����������                                                 
                                      ;
	                                  ; ��������� ������ ����������
	mov word ptr [offset intOldHandler], bx     ; ��������
	mov word ptr [offset intOldHandler + 2], es ; �������
                                      ;
	push ds			                  ; ��������� �������� ds
	pop es                            ; ��������������� �������� es
                                      ;
	mov ah, 25h                       ; ������� ������ ����������� ����������
	mov al, 09h                       ; ����������, ��������� �������� ����� �������
	mov dx, offset handler            ; ��������� � dx �������� ������ ����������� ����������, ������� ����� ���������� �� ����� ������� ����������� 
	int 21h                           ; �������� ���������� ��� ���������� �������
                                      ;
	sti                               ; ��������� ����������
                                      ;
	mov ax, 0                         ; ��������� � ax - 0, �.�. ������ �� ���������
                                      ;
	pop dx                            ; ��������������� �������� ��������� � ��������� ������� �� ���������
	pop bx                            ;
	ret                               ;
ENDP                                  ;
                                      ;
newline PROC                          ;
	push ax                           ; ��������� �������� ���������
	push dx                           ;
                                      ;
	mov dl, 10                        ;	��������� � dx ��������������� ���� �������� ������� 0Ah(10) � 0Dh(13) ��� �������� �� ����� ������
	mov ah, 02h                       ; ��������� � ax ��� 02h - ��� �������� ������ �������
	int 21h                           ; �������� ���������� ��� ������ �������
                                      ;
	mov dl, 13                        ;
	mov ah, 02h                       ; ==//==
	int 21h                           ;
                                      ;
	pop dx                            ; ��������������� ������� ���������
	pop ax                            ;
	ret                               ;
ENDP                                  ;

main:
	call parseCMD                     ; ������ ��������� ������
	cmp ax, 0                         ; ���� �������� ������ - �������
	jne endMain                       ; 
    
    call openFile          ; �������� ���������, ������� ��������� ��� ����� ��� ������/������
	cmp ax, 0               ;
	jne endMain				;  ==//==
                                      ;
	call setHandler                   ; ������������� ����� ���������� ����������
	cmp ax, 0                         ; ���� �������� ������ - �������
	jne endMain				          ; 
                                      ;
	mov ah, 31h                       ; ��������� ��������� �����������
	mov al, 0                         ;    
	                                  ;
	mov dx, (programLength - start + 10Fh) / 16 + 1 ; ������� � dx ������ ��������� + PSP,
	                                  ; ����� ��  16, �.�. � dx ���������� ������� ������ � 16 ������� ����������
	int 21h                           ; 
                                      ;
endMain:                              ;
	ret                               ;                               ;
end start                             ;                 