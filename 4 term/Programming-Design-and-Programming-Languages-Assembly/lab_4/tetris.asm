;asm Tetris - just for fun

CSEG    segment word public 'CODE'
assume  CS:CSEG, DS:CSEG
        org  100h
start:  mov  ax, 03h
        int  10h  
        
newGame:call initGame
	    call initFigure
	    call put
	    jmp Main

newFig: call initFigure
        call tryPut
        cmp ax, 0 
        jne exit
        call put
        ;---- Main game cycle
Main:   call drawField
        call getTime
        add  ax, delayValue
        mov  timer,ax
cycle:  mov  ax, 0100h
        int  16h
        jz   nokeys  
        
        xor  ax,ax 
        int  16h

        mov  si,offset figure
        cmp  ah,4bh ;left
        jnz  other1
leftMark:
        call clearCurrFig
        
        mov ax, FigX
        cmp ax, 0
        je tryLeft
        
        sub ax, 1 
        mov FigX, ax
        
        call tryPut
        
        cmp ax, 0
        jne noLeft
        
        call put
        
        jmp short Main
tryLeft:       
        lea si, figure
        lea di, oldFigure
        
        push cx  
        mov cx, 9
        call copy 
        pop cx  
        
        call moveLeft
        call tryPut
        
        cmp ax, 0
        je moveLeftAtOnce
        
        lea si, oldFigure
        lea di, figure
        
        push cx  
        mov cx, 9
        call copy 
        pop cx  
moveLeftAtOnce:
        call put        
        jmp short Main   
       
noLeft:
        mov ax, FigX
        add ax, 1
        mov FigX, ax
              
        call put                  
        jmp short Main
      
other1: cmp  ah,4dh ;right
        jnz  other2
rightMark:
        call clearCurrFig
        
        mov ax, coloumns
        sub ax, 3
        mov bx, ax 
        mov cx, ax
        add cx, 1
        mov ax, FigX
        add ax, 1 
        mov FigX, ax
        cmp ax, cx
        je tryRight
        cmp ax, bx
        ja noRight
        
        call tryPut
        cmp ax, 0
        jne noRight
        
        call put
        
        jmp short Main  
tryRight:
        mov ax, FigX
        sub ax, 1
        mov FigX, ax
        
        lea si, figure
        lea di, oldFigure
        
        push cx  
        mov cx, 9
        call copy 
        pop cx  
        
        call moveRight
        call tryPut
        
        cmp ax, 0
        je moveRightAtOnce
        
        lea si, oldFigure
        lea di, figure
        
        push cx  
        mov cx, 9
        call copy 
        pop cx  
moveRightAtOnce:
        call put        
        jmp short Main  
noRight:         
        mov ax, FigX
        sub ax, 1
        mov FigX, ax
        
        call put 
         
        jmp short Main   ;think about newFig
    
other2: cmp  ah,48h ;up / rotate
        jnz  other3
rotateMark: 
        call clearCurrFig
        
        lea si, figure
        lea di, oldFigure
        
        push cx 
        
        mov cx, 9
        call copy 
        
        pop cx
        
        call rotateFig
        
        call tryPut
        
        cmp ax, 0
        jne noRotate
         
        call put 
         
        jmp short Main
        
noRotate: 
        lea si, oldFigure
        lea di, figure
        
        push cx
        mov cx, 9
        call copy
        pop cx
        
        call put
        
        jmp short Main
        
other3: cmp  ah,1 ;Escape
        jz   exit   

                
        cmp  ah,50h ;down
        jnz nokeys
downMark:
        call clearCurrFig
        
        mov ax, rows
        sub ax, 3
        mov bx, ax
        mov cx, ax
        add cx, 1
        mov ax, FigY
        add ax, 1 
        mov FigY, ax 
        cmp ax, cx
        je tryDown
        cmp ax, bx
        ja noDown
        
        call tryPut
        cmp ax, 0
        jne noDown
        
        call put
        
        jmp short Main   
tryDown:
        mov ax, FigY
        sub ax, 1
        mov FigY, ax
        
        lea si, figure
        lea di, oldFigure
        
        push cx  
        mov cx, 9
        call copy 
        pop cx  
        
        call moveDown
        call tryPut
        
        cmp ax, 0
        je moveDownAtOnce
        
        lea si, oldFigure
        lea di, figure
        
        push cx  
        mov cx, 9
        call copy 
        pop cx  
moveDownAtOnce:
        call put 
        call checkLines       
        jmp short newFig          
        
noDown:         
        mov ax, FigY
        sub ax, 1
        mov FigY, ax
        
        call put 
        call checkLines
         
        jmp short newFig   ;think about newFig
        
nokeys: call getTime
        cmp  ax, timer
        jl   cycle
       
        jmp downMark      

exit:   mov  ax,3
        int  10h
        int  20h

getTime:
        push ds
        xor  ax,ax
        mov  ds,ax
        mov  ax,word ptr ds:[46ch]
        pop  ds
        ret


random  proc  
loopRand:  
        in ax, 40h 
        inc al 
        and al, 05h 
        cmp al, 05h 
    je  loopRand 
    mov randValue, al
    xor ax, ax
    mov al, randValue 
    ret 
endp


initFigure proc
        call random
        mov  bx,9
        mul  bx
        add  ax, offset TetrisFigures
        mov  si,ax
        mov  di, offset figure
        mov  cx, 9
        call copy ;---- copy figure
        ;xor  ax, ax
        mov  ax, 0
        mov  FigY, ax
        mov  ax, 0
        mov  FigX, ax
        ret
        endp

initGame proc
        xor  ax,ax
        mov  cx, fieldSize
        call printBorder
        ret
endp

copy    proc
; si -- source
; di -- destination
        cld
        rep movsb
        ret
endp



printBorder proc
    push es 
    push bx 
    
    mov ax, 0b800h
    mov es, ax
    mov di, 0
    
    mov cx, rows
loopVertical: 
    mov es:[di], '|'
    mov ax, coloumns
    add ax, 1
    add ax, ax
    mov bx, ax
    add di, bx
    mov es:[di], '|'
    mov ax, 160
    sub ax, bx
    add di, ax
    loop loopVertical 
    
    mov cx, coloumns
    add cx, 2
loopHorizontal: 
    mov es:[di], '-'
    add di, 2
    loop loopHorizontal 
     
    pop bx 
    pop es
    ret
endp  


drawField proc 
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es 
    
    mov ax, 0b800h
    mov es, ax
    mov di, vmPrintOffset
    lea si, field
        
    
    mov cx, rows
coloumnLoopDraw: 
    push cx
    mov cx, coloumns
    
    rowLoopDraw:    
        mov al, [si]
        cmp al, 0
        jne isOne                
    isZero:
        mov al, ' '
        mov es:[di], al
        jmp nextDraw 
    isOne:
        mov al, '*'
        mov es:[di], al
        jmp nextDraw   
        
nextDraw:            
        add di, 2
        inc si
        
        loop rowLoopDraw
        
    pop cx
    mov ax, coloumns
    
    mov bx, 2
    mul bx
    mov bx, ax
    ; computimg: coloumns*2 and moving to bx    
    mov ax, 160
    sub ax, bx
    ;add ax, vmPrintOffset 
    ;computing: 160 - coloumns * 2 + vmPrintOffset
    
    add di, ax   
    
    loop coloumnLoopDraw
    
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp

moveRight proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    add si, 2
    mov cx, 3
checkRightLoop:
    mov al, [si]
    cmp al, 0
    jne noMoveRight
    add si, 3
    loop checkRightLoop  
    
    lea di, figure
    mov cx, 3
    mov bx, 0
moveRightLoop:
    mov al, [di+bx+1]
    mov [di+bx+2], al
    
    mov al, [di+bx]
    mov [di+bx+1], al 
    
    mov al, 0
    mov [di+bx], al 
    
    add bx, 3
    loop moveRightLoop     
    
noMoveRight:    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp     

moveLeft proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    add si, 0
    mov cx, 3
checkLeftLoop:
    mov al, [si]
    cmp al, 0
    jne noMoveLeft
    add si, 3
    loop checkLeftLoop  
    
    lea di, figure
    mov cx, 3
    mov bx, 0
moveLeftLoop:
    mov al, [di+bx+1]
    mov [di+bx], al
    
    mov al, [di+bx+2]
    mov [di+bx+1], al 
    
    mov al, 0
    mov [di+bx+2], al 
    
    add bx, 3
    loop moveLeftLoop     
    
noMoveLeft:    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp

moveDown proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    add si, 6
    mov cx, 3
checkDownLoop:
    mov al, [si]
    cmp al, 0
    jne noMoveDown
    inc si
    loop checkDownLoop  
    
    lea di, figure
    mov cx, 3
    mov bx, 0
moveDownLoop:
    mov al, [di+3+bx]
    mov [di+6+bx], al
    
    mov al, [di+bx]
    mov [di+3+bx], al 
    
    mov al, 0
    mov [di+bx], al 
    
    add bx, 1
    loop moveDownLoop     
    
noMoveDown:    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp




tryPut proc
; ax == 0 -- good , ax == 1 -- can not put
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    lea di, field 
    
    mov ax, FigY
    mov bx, coloumns
    mul bx
    add ax, FigX
    add di, ax
    
    mov cx, 3
coloumnLoopTryPut: 
    push cx
    mov cx, 3
    
    rowLoopTryPut:    
        mov al, [si]
        mov ah, [di]
        cmp al, 0
        je notOne    
        
        mov al, [di]
        cmp al, 1
        je cannotPut
           
    notOne:    
        inc di
        inc si
        
        loop rowLoopTryPut
        
    pop cx
    mov ax, coloumns
    sub ax, 3
    add di, ax   
    
    loop coloumnLoopTryPut
    
    xor ax, ax
    
    jmp endTryPut

cannotPut:
   pop cx
   xor ax, ax
   add ax, 1

endTryPut:    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    ret
endp


put proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    lea di, field 
    
    mov ax, FigY
    mov bx, coloumns
    mul bx
    add ax, FigX
    add di, ax
    
    mov cx, 3
coloumnLoopPut: 
    push cx
    mov cx, 3
    
    rowLoopPut:    
        mov al, [si]
        cmp al, 0
        je continuePut    
        mov [di], al 
           
    continuePut:    
        inc di
        inc si
        
        loop rowLoopPut
        
    pop cx
    mov ax, coloumns
    sub ax, 3
    add di, ax   
    
    loop coloumnLoopPut
    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp 

clearCurrFig proc 
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    lea di, field 
    
    mov ax, FigY
    mov bx, coloumns
    mul bx
    add ax, FigX
    add di, ax
    
    mov cx, 3
coloumnLoopClear: 
    push cx
    mov cx, 3
    
    rowLoopClear:    
        mov al, [si]
        cmp al, 0
        je continueClear    
        mov [di], 0
           
    continueClear:    
        inc di
        inc si
        
        loop rowLoopClear
        
    pop cx
    mov ax, coloumns
    sub ax, 3
    add di, ax   
    
    loop ColoumnLoopClear
    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp

rotateFig proc 
        push cx
        
        lea     si, figure
        lea     di, temp
        mov     cx, 9
        mov     bx, 6 
        ;rows * coloumns - rows
Rot:
        push    si
        add     si, bx
        sub     bx,3
        cmp     bx,0
        jge     Pos
        add     bx, 10
        ;rows * coloumns + 1 == 3 * 3 + 1
Pos:
 
        mov     al,[si]
        mov     [di],al
        inc     di
        pop     si
        loop    Rot
        
        lea si, temp
        lea di, figure 
        
        mov cx, 9
        
        call copy
        
        pop cx
    ret
endp 

checkLines proc   
; bx -- curr row number
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea di, field
    mov bx, 0
    
rowLoopCheck:    
    mov cx, coloumns
coloumnLoopCheck: 
    mov al, [di]
    cmp al, 0
    je nextRow
    inc di
    loop coloumnLoopCheck
    dec di 
moveAll:
    push di
    
    mov al, 1
    mov beepFlag, al
                    
    cmp bx, 0
    je  clearFirstRow               
                    
    mov ax, coloumns
    mul bx
    sub ax, 1
    lea si, field
    add si, ax
    
    push cx
    mov cx, ax     
    ;because ax -- is index of the element
    ;and cx -- is current amount of them
    add cx, 1
        
    std 
    
    rep movsb
    
clearFirstRow:
    lea di, field  
    mov cx, coloumns
    mov al, 0
    
    cld
    
    rep stosb       
    
    pop cx
    pop di
nextRow:
    add bx, 1
    cmp bx, rows
    je beepCheck
    
    mov ax, bx
    mov cx, coloumns
    mul cx
    lea di, field
    add di, ax
   
    jmp rowLoopCheck 
    
beepCheck:
    mov al, beepFlag
    cmp al, 0
    je exitCheck 
    
    call beep
    xor ax, ax
    mov beepFlag, al 
    
exitCheck:        
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp

beep    proc
        pusha  
        
        ;sound on
        mov  cx,6
next:   in   al,61h
        or   al,3
        out  61h,al
        mov  al,10110110b
        mov  dx,43h
        out  dx,al
        dec  dx
        mov  al,cl
        out  dx,al
        out  dx,al
        mov  ax,1
        call delaySound
        ;sound off
        in   al,61h
        and  al,11111100b
        out  61h,al
        loop next
           
        popa
        ret
endp  


delaySound   proc
        mov  bx,ax
        call getTime
        add  bx,ax
tester: call getTime
        cmp  ax,bx
        jl   tester
        ret
endp

TetrisFigures:
        db 0,0,0
        db 1,1,1
        db 0,1,0
        ;--------
        db 0,0,0
        db 1,1,1
        db 0,0,1
        ;--------
        db 0,0,0
        db 1,1,0
        db 1,1,0
        ;--------
        db 0,0,0
        db 1,1,0
        db 0,1,1
        ;--------
        db 0,1,0
        db 0,1,0
        db 0,1,0

fieldSize equ 216
rows equ 18
coloumns equ 12  
;rows equ 6
;coloumns equ 6
vmPrintOffset equ 2

beepFlag db 0

FigX  dw 0
FigY  dw 0

figNumber dw 0

delayValue  dw 20
timer   dw 0
randNumer dw 0

field   db fieldSize dup (0)
randValue db 0         

figure  db 9 dup(0)
oldFigure  db 9 dup(0)

temp    db 9 dup(0)
        ends
        end start