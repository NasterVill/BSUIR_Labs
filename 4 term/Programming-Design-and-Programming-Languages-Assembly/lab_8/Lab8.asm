.model tiny                ;
.code  
.386                    ;
org 100h               ; ������ PSP
                           ;
start:                     ;
	jmp main               ;
;
                           
maxCMDSize            equ 127
maxBuffSize           equ 60
clearValue            equ 80       
endl                  equ 0
cmd_size              db  ?
cmd_text              db  maxCMDSize + 2 dup(0) 
buffer                db  maxBuffSize + 2 dup(0)
sourcePath            db  maxCMDSize + 2 dup(0) 
       
sourceID              dw  0
filePosition          dw 0
amount dw 0

badCMDArgsMessage     db "Bad command-line arguments! Only one argument is required", '$'
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
    
    in  al, 60H         
    cmp al, 01h                       ; esc scan code    
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
    mov ah, 3Dh			        ; ������� 3Dh - ������� ������������ ����
	mov al, 0			        ; ����� �������� �����
	lea dx, sourcePath          ; ��������� � dx �������� ��������� �����
	mov cx, 0			        ; 
	int 21h                     ;    
	
	jc noFileEnd              
	                
	mov sourceId, ax
    
    mov al, 0                ; 
    mov bx, sourceId
	mov ah, 42h              ; ���������� � ah ��� 42h - �-��� DOS ��������� ��������� �����
	mov cx, 0
	mov dx, filePosition		 
	int 21h                  ; �������� ���������� DOS ��� ���������� ��������                            
                                   
    mov ah, 3Fh                     ; ��������� � ah ��� 3Fh - ��� �-��� ������ �� �����
	mov bx, sourceID                ; � bx ��������� ID �����, �� �������� ���������� ���������
	mov cx, maxBuffSize             ; � cx ��������� ������������ ������ �����, �.�. ��������� ������������ ���-�� �������� (maxWordSize ��������)
	lea dx, buffer                  ; � dx ��������� �������� �������, � ������� ����� ��������� ������ �� �����
	int 21h                         ; �������� ���������� ��� ���������� �-���    
    
	cmp ax, 0
    jbe endHandler     
     
    call printToVM 
    
    add ax, filePosition
    mov filePosition, ax       
    
    jmp endHandler
    
downRoutine:
    mov ah, 3Dh			        ; ������� 3Dh - ������� ������������ ����
	mov al, 0			        ; ����� �������� �����
	lea dx, sourcePath          ; ��������� � dx �������� ��������� �����
	mov cx, 0			        ; 
	int 21h                     ;  
	
	jc nofileEnd              
	                
	mov sourceId, ax          
	
	mov ax, filePosition
	mov cx, maxBuffSize
    add cx, maxBuffSize	
	cmp ax, cx
	ja subtract

	xor ax, ax
	jmp OK
subtract:	
	sub ax, cx  
OK:     
    mov filePosition, ax
    
    mov al, 0                ; 
    mov bx, sourceId
	mov ah, 42h              ; ���������� � ah ��� 42h - �-��� DOS ��������� ��������� �����
	mov cx, 0
	mov dx, filePosition		 
	int 21h                  ; �������� ���������� DOS ��� ���������� ��������                               
                                   
    mov ah, 3Fh                     ; ��������� � ah ��� 3Fh - ��� �-��� ������ �� �����
	mov bx, sourceID                ; � bx ��������� ID �����, �� �������� ���������� ���������
	mov cx, maxBuffSize             ; � cx ��������� ������������ ������ �����, �.�. ��������� ������������ ���-�� �������� (maxWordSize ��������)
	lea dx, buffer                  ; � dx ��������� �������� �������, � ������� ����� ��������� ������ �� �����
	int 21h                         ; �������� ���������� ��� ���������� �-���    
    
	cmp ax, 0
    jbe endHandler     
     
    call printToVM 
    
    add ax, filePosition
    mov filePosition, ax       
    
endHandler:
    mov ah, 3Eh                 ; ��������� � ah ��� 3Eh - ��� �������� �����
	mov bx, sourceID            ; � bx ��������� ID �����, ����������� ��������
	int 21h  
	                     ; �������� ���������� ��� ����������     
nofileEnd:
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
    
    mov amount, ax 
    
    mov ax, 0b800h
    mov es, ax
    mov di, 0 
    
    mov cx, clearValue
loopClear:
    mov al, ' ' 
    mov es:[di], al
    add di, 2
    loop loopClear
    
    lea si, buffer
    mov di, 0 
     
    mov cx, amount
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
;*************************************************************************************************************************

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