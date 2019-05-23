.model small
.stack  256
.data
 
Rows            equ    2                
Columns         equ    3 
Two             equ    2
MAX_VALUE       equ    32767              
matrixSize      equ    Rows*Columns    

m               dw     Rows             
n               dw     Columns        
number          dw     0
step            dw     0
 
Matrix          dw     matrixSize dup(?)     
 
newString       db      0dh, 0ah, '$'   
welcomeMes      db      'Input matrix:', '$'
currMatrixMes   db      'Current matrix:', '$'
resultVal       db      'Result value:', '$'
resultNumbers   db      'Result number(s):', '$'
elementBegin    db      'matrix[', '$'     
comma     	    db      ',', '$'
elementEnd      db      ']= ', '$'
 
buffLen        equ     5             
inputBuff      db      buffLen, 0, buffLen dup(0)
 
.code
 
aToi proc
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
 
        mov     cl, [si]
        xor     ch, ch
 
        inc     si
 
        mov     bx, 10
        xor     ax, ax
 
        cmp     byte ptr [si], '-'
        jne     Loop
        inc     si
        dec     cx
Loop:
        mul     bx         
        mov     [di], ax   
        cmp     dx, 0      
        jnz     Error
 
        mov     al, [si]   
        cmp     al, '0'
        jb      Error
        cmp     al, '9'
        ja      Error
        sub     al, '0'
        xor     ah, ah
        add     ax, [di]
        jc      Error    
        cmp     ax, 8000h
        ja      Error
        inc     si
 
        loop    Loop
 
        pop     si         
        push    si
        inc     si
        cmp     byte ptr [si], '-'
        jne     CheckOverflow    
        neg     ax         
        jmp     SaveResult
CheckOverflow:                   
       or       ax, ax     
       js       Error
SaveResult:                
        mov     [di], ax
        clc
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        ret
Error:
        xor     ax, ax
        mov     [di], ax
        stc                  ; setting Carry Flag (CF) to 1
        
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        ret
aToi endp
 

PrintRegister_AX proc
        push    ax
        push    bx
        push    cx
        push    dx
        push    di
 
        mov     cx, 10
        xor     di, di          
 
        or      ax, ax
        jns     Convert
        push    ax
        mov     dx, '-'
        mov     ah, Two           
        int     21h
        pop     ax
 
        neg     ax
 
Convert:
        xor     dx, dx
        div     cx              
        add     dl, '0'         
        inc     di
        push    dx             
        or      ax, ax
        jnz     Convert
        
Print:
        pop     dx              
        mov     ah, Two           
        int     21h
        dec     di              
        jnz     Print
 
        pop     di
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        ret
PrintRegister_AX endp
 
PrintMatrix proc
        pusha
        mov     si, 0  
        mov     di, 0  
        mov     bx, dx
 
PrintRow:
        mov     ax, [bx]
        call    PrintRegister_AX
 
        mov     ah, 02h
        mov     dl, ' '
        int     21h
 
        add     bx, Two
 
        inc     di
 
        cmp     di, n
        jb      PrintRow
 
        mov     dx, OFFSET newString
        mov     ah, 09h
        int     21h
 
        mov     di, 0
 
        inc     si
 
        cmp     si, m
        jb      PrintRow
 
        popa
        ret
PrintMatrix endp
 
InputMatrix proc
        pusha
       
        mov     bx, dx
        
        mov     ah, 09h
        mov     dx, OFFSET welcomeMes
        int     21h
 
        mov     ah, 09h
        mov     dx, OFFSET newString
        int     21h
 
        xor     si, si  
        xor     di, di  
InputInterrupt:        
        lea     dx, elementBegin
        mov     ah, 09h
        int     21h
        mov     ax, si
        call    PrintRegister_AX
        lea     dx, comma
        mov     ah, 09h
        int     21h
        mov     ax, di
        call    PrintRegister_AX
        lea     dx, elementEnd
        mov     ah, 09h
        int     21h
       
        mov     ah, 0ah
        mov     dx, OFFSET inputBuff
        int     21h
 
        push    di
        push    si
        mov     si, OFFSET inputBuff+1
        mov     di, bx
        call    aToi
        pop     si
        pop     di
        jc      InputInterrupt  
        
UpperBorder:        
        cmp     word ptr [bx],  1000
        jle     LowerBorder
        jmp     InputInterrupt
LowerBorder:
        cmp     word ptr [bx],  -999
        jge     NumberSuits
        jmp     InputInterrupt
NumberSuits:      
        mov     dx, OFFSET newString
        mov     ah, 09h
        int     21h
        
        add     bx, Two
 
        inc     di
 
        cmp     di, n
        jnge    InputInterrupt
 
        mov     di, 0
 
        inc     si
 
        cmp     si, m
        jnge    InputInterrupt
 
        popa
        ret
InputMatrix endp
 
Main    proc
        mov     dx, @data
        mov     ds, dx
 
        mov     dx, OFFSET Matrix
        call    InputMatrix
 
        mov     ah, 09h
        mov     dx, OFFSET currMatrixMes
        int     21h
 
        mov     ah, 09h
        mov     dx, OFFSET newString
        int     21h
 
        mov     dx, OFFSET Matrix
        call    PrintMatrix
        
        xor     ax, ax
        mov     ax, Columns
        mov     bx, Two
        mul     bx 
        mov     step, ax
        
        add     number, MAX_VALUE 
        mov     di, 0 
        mov     cx, n    
        lea     si, Matrix    
       
ColumnsCycle:    
        inc     di                    
        xor     ax, ax   
        push    cx
        mov     cx, m       
RowsCycle:   
        mov     dx, [si]  
        add     ax, dx
        add     si, step
        
loop    RowsCycle
        pop     cx
            
        push    ax
        push    dx 
        
        xor     ax, ax
        xor     dx, dx
        mov     ax, m
        mov     dx, step
        mul     dx
        
        sub     si, ax
        
        pop     dx
        pop     ax   
        
        cmp     number, ax
jl     Next
        mov     number, ax      
Next:
        add     si, Two
loop    ColumnsCycle          

        mov     ah, 09h
        mov     dx, OFFSET resultVal
        int     21h
        
        mov     ah, 09h
        mov     dx, OFFSET newString
        int     21h 
        
        mov     ax, number
        call    PrintRegister_AX
        
        mov     ah, 09h
        mov     dx, OFFSET newString
        int     21h
        
        mov     ah, 09h
        mov     dx, OFFSET resultNumbers
        int     21h
        
        mov     ah, 09h
        mov     dx, OFFSET newString
        int     21h 
                
        
FindColumnNumbers: 
        mov     di, 0 
        mov     cx, n    
        lea     si, Matrix    
ColumnsCycleFind:    
        inc     di                    
        xor     ax, ax   
        push    cx
        mov     cx, m       
RowsCycleFind:   
        mov     dx, [si]  
        add     ax, dx
        add     si, step
        
loop    RowsCycleFind
        pop     cx
            
        push    ax
        push    dx 
        
        xor     ax, ax
        xor     dx, dx
        mov     ax, m
        mov     dx, step
        mul     dx
        
        sub     si, ax
        
        pop     dx
        pop     ax
       
        cmp     number, ax
jne     NextFind
        mov     ax, di
        call    PrintRegister_AX
        
        mov     ah, 09h
        mov     dx, OFFSET newString
        int     21h      
NextFind:
        add     si, Two
loop    ColumnsCycleFind
           
                                           
        mov     ax, 4c00h
        int     21h
Main    endp
 
END     Main