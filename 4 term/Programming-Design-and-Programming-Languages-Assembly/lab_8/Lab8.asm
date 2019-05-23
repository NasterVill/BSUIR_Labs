.model tiny                ;
.code  
.386                    ;
org 100h               ; Размер PSP
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
handler PROC                          ; Новый обработчик прерывания
	pushf  
	call    cs:intOldHandler                           ;                              
	push ds                           ;
    push es                           ;
	push ax                           ;
	push bx                           ; Сохраняем регистры
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
        
    pop     di                       ; восстановление модифицируемых регистров
    pop     dx
    pop     cx
    pop     bx
    pop     ax
    pop     es
    pop     ds
    jmp dword ptr cs:intOldHandler   ;на старый вектор    
     
upRoutine:
    mov ah, 3Dh			        ; Функция 3Dh - открыть существующий файл
	mov al, 0			        ; Режим открытия файла
	lea dx, sourcePath          ; Загружаем в dx название исходного файла
	mov cx, 0			        ; 
	int 21h                     ;    
	
	jc noFileEnd              
	                
	mov sourceId, ax
    
    mov al, 0                ; 
    mov bx, sourceId
	mov ah, 42h              ; Записываем в ah код 42h - ф-ция DOS уставноки указателя файла
	mov cx, 0
	mov dx, filePosition		 
	int 21h                  ; Вызываем прерывания DOS для исполнения кодманды                            
                                   
    mov ah, 3Fh                     ; Загружаем в ah код 3Fh - код ф-ции чтения из файла
	mov bx, sourceID                ; В bx загружаем ID файла, из которого собираемся считывать
	mov cx, maxBuffSize             ; В cx загружаем максимальный размер слова, т.е. считываем максимальное кол-во символов (maxWordSize символов)
	lea dx, buffer                  ; В dx загружаем смещения буффера, в который будет считывать данные из файла
	int 21h                         ; Вызываем прерывание для выполнения ф-ции    
    
	cmp ax, 0
    jbe endHandler     
     
    call printToVM 
    
    add ax, filePosition
    mov filePosition, ax       
    
    jmp endHandler
    
downRoutine:
    mov ah, 3Dh			        ; Функция 3Dh - открыть существующий файл
	mov al, 0			        ; Режим открытия файла
	lea dx, sourcePath          ; Загружаем в dx название исходного файла
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
	mov ah, 42h              ; Записываем в ah код 42h - ф-ция DOS уставноки указателя файла
	mov cx, 0
	mov dx, filePosition		 
	int 21h                  ; Вызываем прерывания DOS для исполнения кодманды                               
                                   
    mov ah, 3Fh                     ; Загружаем в ah код 3Fh - код ф-ции чтения из файла
	mov bx, sourceID                ; В bx загружаем ID файла, из которого собираемся считывать
	mov cx, maxBuffSize             ; В cx загружаем максимальный размер слова, т.е. считываем максимальное кол-во символов (maxWordSize символов)
	lea dx, buffer                  ; В dx загружаем смещения буффера, в который будет считывать данные из файла
	int 21h                         ; Вызываем прерывание для выполнения ф-ции    
    
	cmp ax, 0
    jbe endHandler     
     
    call printToVM 
    
    add ax, filePosition
    mov filePosition, ax       
    
endHandler:
    mov ah, 3Eh                 ; Загружаем в ah код 3Eh - код закрытия файла
	mov bx, sourceID            ; В bx загружаем ID файла, подлежащего закрытию
	int 21h  
	                     ; Выпоняем прерывание для выполнения     
nofileEnd:
    in al,61h ;Введем содержимое порта В
    or al,80h ;Подтвердим прием кода, добавив
    out 61h,al ;бит 80h к содержимому порта В
    and al,7Fh ;Снова разрешим работу клавиатуры.
    out 61h,al ;сбросив в порте В бит 80h .
                           ;
    mov al, 20h                       ;
    out 20h, al                       ; 
    
	pop di                            ;
	pop dx                            ;
	pop cx                            ;
	pop bx                            ; Восстанавливаем регистры
	pop ax                            ;
	pop es                            ;
	pop ds	                          
	jmp intEnd    
       
escRoutine:    
    ;восстанавливаем вектор 09h
	
	in al,61h ;Введем содержимое порта В
    or al,80h ;Подтвердим прием кода, добавив
    out 61h,al ;бит 80h к содержимому порта В
    and al,7Fh ;Снова разрешим работу клавиатуры.
    out 61h,al ;сбросив в порте В бит 80h .
                           ;
    mov al, 20h                       ;
    out 20h, al                       ; 
    
	pop di                            ;
	pop dx                            ;
	pop cx                            ;
	pop bx                            ; Восстанавливаем регистры
	pop ax                            ;
	pop es                            ;
	pop ds	                          
	
    mov ax,2509h
    mov dx,word ptr cs:[intOldHandler]
    mov ds,word ptr cs:[intOldHandler+2]
    int 21h 
    
    mov es,cs:2ch ; получим из PSP адрес собственного 
    mov ah,49h ; окружения резидента и выгрузим его 
     int 21h ;  
    
    push cs ; выгрузим теперь саму программу 
    pop es ; 
    mov ah,49h ; 
    int 21h ;                         
    
intEnd:     
	iret                              ;
ENDP                                  ;
;**************************************************************************************************************************   


;**************************************************************************************************************************
;ax -- число элементов для вывода на экран
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
	mov ah, 09h             ; Команда вывода 
	lea dx, info            ; Загрузка в dx смещения выводимого сообщения
	int 21h                 ; Вызов прервывания для выполнения вывода
	                        ;
	 mov dl, 0Ah            ; Символ перехода на новую строку
	mov ah, 02h             ; Команда вывода символа
	int 21h                 ; Вызов прерывания
                            ;
	mov dl, 0Dh             ; Символ перехода в начало строки   
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
	push dx                           ; Сохраняем значения регистров
	push si                           ;
	push di                           ;
                                      ;
	cld                               ;
	mov bx, 80h                       ;
	mov cl, cs:[bx]                   ; Переходим в смещение, где расположен текст командной строки
	xor ch, ch                        ; В cl загружаем длину командной строки
                                      ;
	xor dx, dx                        ;
	mov di, 81h                       ;
                                      ;
	mov al, ' '                       ; Пропускаем все пробелы
	repne scasb	                      ; Найти байт, не равный al в блоке из cx байт по адресу es:di 
	
	mov al, ' '
    repe scasb     
    dec di     
    inc cx
	
	xor ax, ax                        ;                                                   
                                      ;
	mov si, di                        ; Загружаем в si смещение, с которого начинаются аргументы
	mov di, offset sourcePath          ; Начинаем парсинг с sourcePath  
	
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
    mov [di], al                ; Загружаем символ конца строки в результирующую строки 
    mov ax, 0
    
endParse:                       ;
	pop di
	pop si
	pop dx                      ; восстанавливаем регистры
	pop cx                      ;
	pop bx                      ;
	ret                         ;  
endp
;********************************************************************************************************************   	
	
	
                                      ;                        
 ;********************************************************************************************************************   
setHandler PROC                       ; Установка нового обработчика прерываний. Результат ax=0 если нет ошибок, иначе ax!=0 
	push bx                           ;
	push dx                           ; Сохраняем значения регистров
                                      ;
	cli                               ; Запрещаем прерывания (запрет/разрешение необходимо для корректной установки нового обработчика )
                                      ;
	mov ah, 35h                       ; Функция получения адреса обработчика прерывания
	mov al, 09h                       ; прерывание, обработчик которого необходимо получить (09h - прерывание клавиатуры)
	int 21h                           ; Вызываем прерывание для выполения функции 
                                      ; В результате выполнения функции в es:bx помещается адрес текущего обработчика прерывания                                                 
                                      ;
	                                  ; Сохраняем старый обработчик
	mov word ptr [offset intOldHandler], bx     ; смещение
	mov word ptr [offset intOldHandler + 2], es ; сегмент
                                      ;
	push ds			                  ; Сохраняем значение ds
	pop es                            ; Восстанавливаем значение es
                                      ;
	mov ah, 25h                       ; Функция замены обработчика прерывания
	mov al, 09h                       ; прерывание, обработчк которого будет заменен
	mov dx, offset handler            ; загружаем в dx смещение нового обработчика прерывания, который будет установлен на место старого обработчика 
	int 21h                           ; Вызываем прерывание для выполнения функции
                                      ;
	sti                               ; Разрешаем прерывания
                                      ;
	mov ax, 0                         ; Загружаем в ax - 0, т.е. ошибок не произошло
                                      ;
	pop dx                            ; Восстанавливаем значения регистров и переходим выходим из процедуры
	pop bx                            ;
	ret                               ;
ENDP                                  ;
                                      ;
newline PROC                          ;
	push ax                           ; Сохраняем значения регистров
	push dx                           ;
                                      ;
	mov dl, 10                        ;	Загружаем в dx последовательно коды возврата каретки 0Ah(10) и 0Dh(13) для перехода на новую строку
	mov ah, 02h                       ; Загружаем в ax код 02h - код операции вывода символа
	int 21h                           ; Вызываем прерывание для вывода символа
                                      ;
	mov dl, 13                        ;
	mov ah, 02h                       ; ==//==
	int 21h                           ;
                                      ;
	pop dx                            ; Восстанавливаем значния регистров
	pop ax                            ;
	ret                               ;
ENDP                                  ;

main:
	call parseCMD                     ; Парсим командную строку
	cmp ax, 0                         ; Если возникла ошибка - выходим
	jne endMain                       ; 
                                      ;
	call setHandler                   ; Устанавливаем новый обработчик прерывания
	cmp ax, 0                         ; Если возникла ошибка - выходим
	jne endMain				          ; 
                                      ;
	mov ah, 31h                       ; Оставляем программу резидентной
	mov al, 0                         ;    
	                                  ;
	mov dx, (programLength - start + 10Fh) / 16 + 1 ; Заносим в dx размер программы + PSP,
	                                  ; делим на  16, т.к. в dx необходимо занести размер в 16 байтных параграфах
	int 21h                           ; 
                                      ;
endMain:                              ;
	ret                               ;                               ;
end start                             ;                    