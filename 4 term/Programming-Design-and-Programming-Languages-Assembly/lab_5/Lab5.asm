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
cmpWordLenWith0 MACRO textline, is0Marker       ; Сравнение строки с нулем, 1 параметр - строка, 2 условие перехода при 0
	push si                                     ; Сохранение значение регистра si
	                                            ;
	lea si, textline                            ; Загружаем в si смещение строки, в которой измеряем длинну
	call    strlen                              ; Вызываем ф-цию нахождения длины строки textline, результат -> ax
	                                            ;
	pop si                                      ; Восстанавливаем значение si
	cmp ax, 0                                   ; Сравниваем найденную длинну с нулем
	je is0Marker                                ; Если длина равна нулю -> переходим по переданной метке is0Marker
ENDM                                            ;                               
;**************************************************************************************************************************
 
;******************************************************************************************************************************       
setPosInFileTo MACRO offsetSymbolsDX, from
	push ax                     ;
	push bx                     ;
	push cx                     ;
	push dx                     ; Сохраняем значения регистров    
    
    mov al, from                ;
	mov ah, 42h                 ; Записываем в ah код 42h - ф-ция DOS уставноки указателя файла
	mov cx, 0
	mov dx, offsetSymbolsDX		; Обнуляем dx, т.е премещаем указатель на 0 символов от начала файла (cx*2^16)+dx 
	int 21h                     ; Вызываем прерывания DOS для исполнения кодманды   
                                ;
	pop dx                      ; Восстанавливаем значения регистров и выходим из процедуры
	pop cx                      ;
	pop bx                      ;
	pop ax                      ;
ENDM 
;******************************************************************************************************************************
 
;**************************************************************************************************************************
main:
	mov ax, @data           ; Загружаем данные
	mov es, ax              ;
                            ;
	xor ch, ch              ; Обнуляем ch
	mov cl, ds:[80h]		; Смещеие для дальнейшей работы с командой строкой
	mov cmd_size, cl 		; В cmd_size загружаем длину командной строки
	mov si, 81h             ;
	lea di, cmd_text        ; Загружаем в di смещение текста переданного через командную строку
	rep movsb               ; Записать в ячейку адресом ES:DI байт из ячейки DS:SI
                            ;
	mov ds, ax              ; Загружаем в ds данные
                            ;
	println startText       ; Вывод строки о начале работы программы
                            ;
	call parseCMD           ; Вызов процедуры парсинга командной строки
	cmp ax, 0               ;
	jne endMain				; Если ax != 0, т.е. при выполении процедуры произошла ошибка - переходим к конце программы, т.е. прыгаем в endMain
                            ;
	call openFiles          ; Вызываем процедуру, которая открывает оба файла для чтения/записи
	cmp ax, 0               ;
	jne endMain				;  ==//==
                            ;
    call processingFile     ;
                            ;
	call closeFiles         ; Завершив обработку информации, вызываем процедуру закрытия файлов
	cmp ax, 0               ;
	jne endMain				;  ==//==
                            ;
endMain:                    ;
	println endText         ; Выводим сообщение о завершении работы программы
                            ;
	mov ah, 4Ch             ; Загружаем в ah код команды завершения работы
	int 21h                 ; Вызов прерывания DOS для ее исполнения
;**************************************************************************************************************************

;**************************************************************************************************************************
parseCMD PROC                                   ;
	push bx                                     ;
	push cx                                     ;
	push dx                                     ; Сохраняем
                                                ;
	mov cl, cmd_size                            ; Загружаем в cl размер командой строки
	xor ch, ch                                  ; Обнуляем ch
                                                ;
	lea si, cmd_text                            ; В si загружаем смещение данных из командой строки
	                                            ;                                                          
	lea di, cmd_buffer                          ; В di загружаем смещение буффера для обработки данных      
	call rewriteAsWord                          ;                                                           
                                                ;                                                           
	lea di, sourcePath                          ; Загружаем в di смещение sourcePath т.е. источника текста для обработки
	call rewriteAsWord                          ;
                                                ;
	cmpWordLenWith0 sourcePath, badCMDArgs      ; Если строка, содержащая название исxодного файла пуста, прыгаем в badCMDArgs                                                           
                                                ;                                                           
	lea di, exodusWord                          ; Загружаем в di смещение exodusWord т.е. слово для удаления из текста
	call rewriteAsWord                          ;
                                                ;
	cmpWordLenWith0 exodusWord, badCMDArgs      ; Если строка, содержащая слово пуста, прыгаем в badCMDArgs  	                                                                                                                          
                                                ; 
	lea di, cmd_buffer                          ; 
	call rewriteAsWord                          ;
                                                ;
	cmpWordLenWith0 buffer, goodCMDArgs         ; Если больше данных нет, т.е. кроме названия файла и слова командная строка ничего не содержит
                                                ; то вызов программы корректный - прыгаем в argsIsGood
badCMDArgs:                                     ;
	println badCMDArgsMessage                   ; Выводим сообщение о неверных параметрах командной строки
	mov ax, 1                                   ; Записываем в ax код ошибки
                                                ;
	jmp endproc                                 ; Прыгаем в endproc и завершаем процедуру
                                                ;
goodCMDArgs:                                    ;
	mov ax, 0                                   ; Записываем в ax код успешного завершения процедуры
endproc:                                        ;
	pop dx                                      ;
	pop cx                                      ;
	pop bx                                      ; Восстанавливаем значения регистров и выходим из процедуры
	ret	                                        ;
ENDP
;*************************************************************************************************************************  

;*************************************************************************************************************************
setSpaceBuffer PROC
	push cx                     ;
	push si                     ;
	push di                     ; Сохраняеи регистры  
	
	lea si, exodusWord
    call strlen	
    mov cx, ax 
    cld                                       ;снять флаг DF
    lea di, spaceBuffer 
    xor ax, ax                                ;обнуляем записываемый байт 
    mov al, ' '
    rep stosb                                 ;обнуляем массив байтов
		       
    pop di
    pop si
    pop cx
    ret
ENDP
;*************************************************************************************************************************

;*************************************************************************************************************************
;cx - Длина командной строки, si - источник, di - приемник
;Результат - переписывает параметр из коммандной строки 
rewriteAsWord PROC              ;
	push ax                     ;
	push cx                     ;
	push di                     ; Сохраняеи регистры
	                            ;
loopParseWord:                  ;
	mov al, ds:[si]             ; Загружаем в al текущий символ
	
	cmp al, spaceSymbol         ; -------------------
    je isStoppedSymbol          ;                                 ;
	cmp al, newLineSymbol       ;
	je isStoppedSymbol          ;         Если этот символ равен            ;
	cmp al, tabulation          ;   пробелу, табуляции, 0Ah, 0Dh или \0
	je isStoppedSymbol          ;     Значит мы дошли до  конца слова
	cmp al, returnSymbol        ;
	je isStoppedSymbol          ;                                        ;
	cmp al, endl                ;
	je isStoppedSymbol          ;
                                ;
	mov es:[di], al             ; Если данный символ входит в слово, добавляем его в результирующую строку
                                ;
	inc di                      ; Увеличиваем di,si т.е. переходим на следующий символ
	inc si                      ;
                                ;
	loop loopParseWord          ; Пока не превышена максимальная длина слова, парсим
isStoppedSymbol:                ;
	mov al, endl                ;
	mov es:[di], al             ; Загружаем символ конца строки в результирующую строку
	inc si                      ; Увеличиваем si для перезода на следующий символ командной строки
                                ;
	pop di                      ; восстанавливаем регистры
	pop cx                      ;
	pop ax                      ;
	ret                         ;
ENDP   
;**************************************************************************************************************************  
  
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

;****************************************************************************************************************************** 
 ;ax equals 1 if all is good, or 0 if the EOF is reached, or there was an error
 readSymbol PROC
    
    push bx                         ;
	push cx                         ;
	push dx                         ; Сохраняем значения регистров
                                    ;
	mov ah, 3Fh                     ; Загружаем в ah код 3Fh - код ф-ции чтения из файла
	mov bx, sourceID                ; В bx загружаем ID файла, из которого собираемся считывать
	mov cx, 1                       ; В cx загружаем 1, чтобы считать символ
	lea dx, buffer                  ; В dx загружаем смещения буффера, в который будет считывать данные из файла
	int 21h                         ; Вызываем прерывание для выполнения ф-ции
                                    ;
	jnb goodSymbol					; Если ошибок во время счтения не произошло - прыгаем в goodRead
                                    ;
	println errorReadSourceText     ; Иначе выводим сообщение об ошибке чтения из файла
	mov ax, 0                       ; Записываем в ax 0                               
goodSymbol:                         ;   
	pop dx                          ; Восстанавливаем значения регистров
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
                                        ; Сохраняем значения регистров	
	mov ax, wordLen
    mov cx, ax
	neg ax
	mov wordOffset, ax
	
	mov bx, sourceID
    setPosInFileTo wordOffset, 1        ; Вызываем процедую смещения курсора на одно искомое слово назад
    
    xor ax, ax
    mov ah, 40h                         ; Загружаем в ah код 40h - код ф-ции записи в файл
	mov bx, sourceID                    ; В bx загружаем ID файла, в который будем записывать данные
	lea dx, spaceBuffer                 ; В dx загружаем данные, которые будет записывать
	int 21h                             ; Вызываем прерывание для выполения команды
     
 jnb goodWrite					      ; Если во время записи ошибок не произошло, прыгаем в goodWrite
                                      ;
	println errorWritingText          ; Иначе выводим сообщение о возникновении ошибки при записи
	mov ax, 0                         ; Обнуляем ax
                                      ;
goodWrite:                            ;
    
    xor ax, ax
    mov ah, 0Dh
    mov bx, sourceID 
    int 21h
                                      ;
	pop dx                            ; Восстанавливаем регистры и выхоидм из процедуры
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
	push di                             ; Сохраняем значения регистров
                                        ;
	mov bx, sourceID                    ; Загружаем в bx ID файла-источника
	setPosInFileTo 0, 0                 ; Вызываем процедую смещения курсора в начало файла
                                        ;
    lea si, exodusWord                  ;                                    
    call strlen                         ;
    mov wordLen, ax                     ;
    call setSpaceBuffer 
    
processing:	
	call readSymbol
	cmp ax, 0                           ; Сравнивание aх с 0 для проверки на конец файла
	je finishProcessing                 ; Если aх == 0, то буффер пуст и мы дошли до конца файла  
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
    
    cmp ax, 0                   ; Сравнивание aх с 0 для проверки на конец файла
	je toDelete                 ; Если aх == 0, то буффер пуст и мы дошли до конца файла    
    
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
	pop di                              ; Восстанавливаем значения 
	pop si                              ;
	pop dx                              ;
	pop cx                              ;
	pop bx                              ;
	pop ax                              ;
	ret                                 ;
ENDP                                    ;
;**************************************************************************************************************************  
 
;**************************************************************************************************************************
;Результат в ax - 0 если все хорошо
openFiles PROC                  ;
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
 
;**************************************************************************************************************************
closeFiles PROC                 ;
	push bx                     ;
	push cx                     ; Сохраняем значения регистров 
                                ;
	xor cx, cx                  ; Обнуляем cx
                                ;
	mov ah, 3Eh                 ; Загружаем в ah код 3Eh - код закрытия файла
	mov bx, sourceID            ; В bx загружаем ID файла, подлежащего закрытию
	int 21h                     ; Выпоняем прерывание для выполнения 
                                ;
	jnb goodCloseOfSource		; Если ошибок при закрытии не произошло, прыгаем в goodCloseOfSource
                                ;
	println errorClosingSource  ; Иначе выводим соответсвующее сообщение об ошибке       
	                            ;
	inc cx 			            ; сх выступает в роли счетчика ошибок
                                ;
goodCloseOfSource:              ;               ;
	mov ax, cx 		            ; Записываем в ax значение из cx, если ошибок не произошло, то это будет 0, иначе 1
	pop cx                      ;
	pop bx                      ; Восстанавливаем значения регистров и выходим из процедуры
	ret                         ;
ENDP                            ;
;****************************************************************************************************************************** 
 
END main

; c:\data.txt the