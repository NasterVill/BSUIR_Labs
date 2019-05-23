.model tiny                ;
.code  
.386                    ;
org 100h               ; Размер PSP
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
                                      ;
    in  al, 60H         
    cmp al, 39h                       ; space scan code    
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
    mov ah, 3Fh                     ; Загружаем в ah код 3Fh - код ф-ции чтения из файла
	mov bx, sourceID                ; В bx загружаем ID файла, из которого собираемся считывать
	mov cx, maxBuffSize             ; В cx загружаем максимальный размер слова, т.е. считываем максимальное кол-во символов (maxWordSize символов)
	lea dx, buffer                  ; В dx загружаем смещения буффера, в который будет считывать данные из файла
	int 21h                         ; Вызываем прерывание для выполнения ф-ции
	
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
	mov ah, 42h              ; Записываем в ah код 42h - ф-ция DOS уставноки указателя файла
	mov cx, 0
	mov dx, fileOffset		 
	int 21h                  ; Вызываем прерывания DOS для исполнения кодманды  

	
    mov ah, 3Fh                     ; Загружаем в ah код 3Fh - код ф-ции чтения из файла
	mov bx, sourceID                ; В bx загружаем ID файла, из которого собираемся считывать
	mov cx, maxBuffSize             ; В cx загружаем максимальный размер слова, т.е. считываем максимальное кол-во символов (maxWordSize символов)
	lea dx, buffer                  ; В dx загружаем смещения буффера, в который будет считывать данные из файла
	int 21h                         ; Вызываем прерывание для выполнения ф-ции
	
	cmp ax, 0
	jbe endHandler     
    
    call printToVM 
    
    add ax, fileOffset
    mov fileOffset, ax 
    jmp endHandler
    
endHandler:
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
	
	mov ah, 3Eh                 ; Загружаем в ah код 3Eh - код закрытия файла
    mov bx, sourceID            ; В bx загружаем ID файла, подлежащего закрытию
	int 21h                     ; Выпоняем прерывание для выполнения     
	
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
    
    jmp intEnd                    
    
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

;**************************************************************************************************************************
;Результат в ax - 0 если все хорошо
openFile PROC                  ;
	push bx                     ;
	push dx                     ; Сохраняем значения регистров          
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
	mov ah, 3Dh			        ; Функция 3Dh - открыть существующий файл
	mov al, 02h			        ; Режим открытия файла
	lea dx, sourcePath          ; Загружаем в dx название исходного файла
	mov cx, 00h			        ; 
	int 21h                     ;
                                ;
	jb badOpenSource	        ; Если файл не открылся, то прыгаем в badOpenSource
                                ;
	mov sourceID, ax	        ; Загружаем в sourceId значение из ax, полученное при открытии файла
                                ;
	mov ax, 0			        ; Загружаем в ax 0, т.е. ошибок во время выполнения процедуры не произшло 
	jmp endOpenProc		        ; Прыгаем в endOpenProc и корректно выходим из процедуры
                                ;
badOpenSource:                  ;
	println badSourceText       ; Выводим соответсвующее сообщение
	cmp ax, 02h                 ; Сравниваем ax с 02h
	jne errorFound              ; Если ax != 02h - файл найден, прыгаем в errorFound
                                ;
	println fileNotFoundText    ; Выводим сообщение о том, что файл не найден 
                                ;
	jmp errorFound              ; Прыгаем в errorFound
                                ;       ;       ==//==                               ;
errorFound:                     ;
	mov ax, 1                   ; Загружаем в ax 1, т.е. произошла ошибка
endOpenProc:                    ;
	pop dx                      ;
	pop bx                      ; Восстанавливаем значения регистров и выходим из процедуры
	ret                         ;
ENDP                            ;
;******************************************************************************************************************************        
 
 ;*************************************************************************************************************************
;ds:si - смещение, в котором находится начало строки
;Результат - в ax помещается длина строки 
strlen PROC                     ;
	push bx                     ;
	push si                     ;  Сохраняем используемые далее регистры
	                            ;
	xor ax, ax                  ;  Зануляем ax
                                ;
    startCalc:                  ;
	    mov bl, ds:[si]         ;  Загружаем очередной символ строки из ds со смещением si
	    cmp bl, endl            ;  Сравниваем этот символ с символом конца строки
	    je endCalc              ;  Если это символ конца строки - прыгаем в endCalc и заканчиваем вычисления
                                ;
	    inc si                  ;  Увеличиваем si, т.е. переходим к следующему символу
	    inc ax                  ;  Увеличиваем ax, т.е. длину строки                                                     
	    jmp startCalc           ;  Продолжаем
	                            ;
    endCalc:                    ;
	pop si                      ;
	pop bx                      ;  Восстанавливаем значения
	ret                         ;
ENDP                            ;
;*************************************************************************************************************************
                       
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
    
    call openFile          ; Вызываем процедуру, которая открывает оба файла для чтения/записи
	cmp ax, 0               ;
	jne endMain				;  ==//==
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