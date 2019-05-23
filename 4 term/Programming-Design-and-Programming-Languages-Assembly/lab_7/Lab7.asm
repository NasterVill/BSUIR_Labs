; Написать программу, последовательно запускающую программы, которые расположены в заданном каталоге. 

;**************************************************************************************************************************
;Параметр 1: путь к требуемой папке       ;
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
	      dw 005Ch,0,006Ch,0    	      ;Адреса FCB (File control block) программы
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
; ax - error code                     ;
printErrorCode MACRO nothing          ;
	add al, '0'                       ;
	mov dl, al                        ;
	mov ah, 06h                       ; Выводит код ошибки
	int 21h                           ;
                                      ;
	mov dl, 0Ah                       ; Символ перехода на новую строку
	mov ah, 02h                       ; Команда вывода символа
	int 21h                           ; Вызов прерывания
                                      ;
	mov dl, 0Dh                       ; Символ перехода в начало строки   
	mov ah, 02h                       ;
	int 21h                           ;            ==//==                                            
ENDM                                  ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                         
main:                                 ;
	;reallocating memory              ; Сокращаем выделенную программе память до минимума
	mov ah, 4Ah                       ;
	mov bx, ((CodeSize/16)+1)+((DataSize/16)+1)+32	;требуемый размер в параграфах
	int 21h                           ;
                                      ;
	jnc initToRunAllGood              ; Если при перевыделении памяти ошибок не произошло
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
	mov ax, @data                     ; Загружаем сегмент данных
	mov es, ax                        ;
                                      ;
	xor ch, ch                        ;
	mov cl, ds:[80h]		          ; Загружаем в cl длину введенной командной строки
	mov cmd_size, cl 		          ; Сохраняем размер строки
	mov si, 81h                       ; Устанавливаем si на первый символ командной строки
	mov di, offset cmd_text           ; Устанавливаем di на начало cmd_text
	rep movsb                         ; Переписываем строки из командной строки в cmd_text
	                                  ; теперь в cmd_text то, что было записано в командную строку
	                                  ; а в cmd_size - размер
                                      ;
	mov ds, ax                        ;
                                      ;
	println startText                 ; Выводим вводный текст
                                      ;
	call parseCMD                     ; Парсим командную строку
	cmp ax, 0                         ;
	jne endMain                       ; Возникла ошибка - выходим
	                                  ;
	                                  ; Меняем директорию на указанную в командной строке
	mov ah, 3Bh                       ; Загружаем код операции изменения директории
    mov dx, offset folderPath         ; В dx загружаем название искомой директории
    int 21h                           ; Прерывание для выполнения функции
    jc badWay                         ; Директория не найдена - выдаем ошибку
                                      ;
	call findFirstFile                ; Находим первый .exe файл в указанной директории
	cmp ax, 0                         ; 
	jne endMain			              ; Какая-то ошибка - выход
                                      ;
	call runEXE                       ; Пытаемся запустить найденный файл
	cmp ax, 0                         ; 
	jne endMain			              ; Какая-то ошибка - выход
                                      ;
runFile:                              ;
	call findNextFile                 ; Находим следующий файл
	cmp ax, 0                         ;
	jne endMain			              ; Какая-то ошибка - выход
                                      ;
	call runEXE                       ; Пытаемся запустить найденный файл
	cmp ax, 0                         ;
	jne endMain			              ; Какая-то ошибка - выход
                                      ;
	jmp runFile                       ;
                                      ;
badWay:                               ;
	println badFolderErrorText        ; Выводим сообщение о некорректной директории
endMain:                              ;                                      
	println endText                   ; Выводим заключительный текст
                                      ;
	mov ah, 4Ch                       ; Выходим из программы
	int 21h                           ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                      
cmpWordLenWith0 MACRO textline, is0Marker ; Если длина textline == 0, прыгаем в is0Marker
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
	push dx                               ; Сохраняем регистры
                                          ;
	mov cl, cmd_size                      ; В cl загружаем длину командной строки
	xor ch, ch                            ;
                                          ; Парсим из командной строки
	mov si, offset cmd_text               ; В буффер
	mov di, offset buffer                 ;
	call rewriteAsASCIIZWord              ;
                                          ;
	mov di, offset folderPath             ; Парсим название директории
	call rewriteAsASCIIZWord              ;
	cmpWordLenWith0 folderPath, badCMDArgs; Если название нулевой длинны - ошибка
                                          ;
	mov di, offset buffer                 ; Проверяем, указали ли мы лишние аругменты
	call rewriteAsASCIIZWord              ;
                                          ;
	cmpWordLenWith0 buffer, argsIsGood    ; Если указали - badCMDArgs
                                          ;
badCMDArgs:                               ;
	println badCMDArgsMessage             ; Выводим сообщение об ошибке
	mov ax, 1                             ; Загружаем код 1 в ax ( код ошибки)
                                          ;
	jmp endproc                           ;
                                          ;
argsIsGood:                               ;
	mov ax, 0                             ; Записываем 0 в ax (обработка прошла успешно)
                                          ;
endproc:                                  ;
	pop dx                                ;  
	pop cx                                ;
	pop bx                                ; Восстанавливаем регистры
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
	mov ax, 4B00h			              ; 4B00h - Загрузить и выполнить (4B01h - загрузить и НЕ выполнять)
	mov bx, offset EPBstruct              ; Адрес структуры LoadExec
	mov dx, offset DTAblock + 1Eh	      ; Информация о запускаемом файле
	int 21h                               ;
	                                      ;
	jnc runEXEAllGood                     ;
                                          ;
	println runEXEErrorText               ; Вывыодим сообщения об ошибках
	printErrorCode                        ;
                                          ;
	mov ax, 1                             ; Записываем код ошибки
                                          ;
	jmp runEXEEnd                         ; Заканчиваем функцию
                                          ;
runEXEAllGood:                            ;
	mov ax, 0                             ; Записываем код успешного выполнения
                                          ;
runEXEEnd:                                ;
	pop dx                                ;
	pop bx                                ;
	ret                                   ;
ENDP                                      ;
;**************************************************************************************************************************

;**************************************************************************************************************************                                          
installDTA PROC                           ;
	mov ah,1Ah                            ; Ф-ция DOS - установить область DTA
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
    mov ah,4Eh                            ; 4Eh - найти первый совпадающий файл
    xor cx,cx               		      ; 
    mov dx, offset findSuffix             ; Адрес строки с именем файла
    int 21h                               ;
                                          ;
	jnc findFirstFileAllGood              ; Если файл найден - переходим в findFirstFileAllGood
                                          ;
	mov ax, 1                             ; Помещаем в ax код ошибки
                                          ;
	jmp findFirstFileEnd                  ;
                                          ;
findFirstFileAllGood:                     ;
	mov ax, 0                             ; Помещаем в ax код успешного выполнения
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
	mov ah,4Fh                            ; 4Fh - найти следующий файл     
	                                      ; При этом DTA содержит данные предыдущиего вызова функций 4E и 4F
    mov dx, offset DTAblock               ; адрес строки с именем файла
    int 21h                               ; После выполнения DTA содержит данные о найденном файле
                                          ;
	jnc findNextFileAllGood               ;
                                          ;
	mov ax, 1                             ; Записываем код ошибки
                                          ;
	jmp findNextFileEnd                   ;  
                                          ;
findNextFileAllGood:                      ;
	mov ax, 0                             ; Записываем успешную обработку
                                          ;
findNextFileEnd:                          ;
                                          ;
	ret                                   ;
ENDP                                      ;
                                          ;
CodeSize = $ - main                       ;
;**************************************************************************************************************************
                                          
end main                                  