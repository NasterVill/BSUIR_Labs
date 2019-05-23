name "Reverse"
 
.model tiny 

.code   
   
org 100h 

output macro output_string, offset_space
    mov dx, offset output_string
    add dx, offset_space
    mov ah, 09h
    int 21h
    endm

start:      
        output message, 0                 ; Welcoming message
                    
        mov bx, offset string             ; input of the string
        mov [bx], SIZE                    ; setting buffer size (so we can input all 200 symboles if needed)
        mov dx, bx  
        mov ah, 0Ah  
        int 21h
        
        add bl, [string + DOLLAR_OFFSET] ; getting string length + '$' offset
        add bx, SYMBOL_OFFSET            ; and adding 0Ah and 0Dh to it (means incrementing by two) 
        
        mov ax, dx                       ; checking if the string is not empty
        add ax, SYMBOL_OFFSET           
        cmp ax, bx
        je exit_failure
        
        mov [bx], '$'
        
        xor dx,dx                    ; set dx to null
        xor ax,ax                    ; set ax to null

        lea bx, string               ; repeating the operation
        add bx, SYMBOL_OFFSET        ; after setting '$'

        mov si, bx                   ; saving the begining position
        mov di, bx                   ; of a string
        dec si
 
next_symbol:
            inc si                       ; going through the string
            mov al, [si]                 ; reading a symbol
            cmp al, ' '
            je end_found 
            cmp al, '$'
            je end_found
            jmp next_symbol
 
end_found:
            mov dx, si                   ; found space or '$'
            dec si                       ; going one symbol back
            mov bx, di                   ; in order to know the word's last symbol
 
reverse_word: 
            cmp bx, si                   ; inverse the word
            jae done
            
            mov al, [bx]
            mov ah, [si]
            
            mov [si], al
            mov [bx], ah
            
            inc bx
            dec si
jmp reverse_word
 
done:       
            mov si, dx                   ; restoring the position of the symbol after the reversed word
            inc dx                       ; getting the position of
            mov bx, dx                   ; possible first symbol of the next word
            mov di, bx                   ; saving it

            mov ah, [si] 
            cmp ah, '$'
            jne next_symbol   

    output result_message, 0
    
    output new_string, 0
    
    output string, SYMBOL_OFFSET
    
    mov ah, 01h                              ; waiting for any symbol
    int 21h                                  ; to exit the programm 
    
    ret

exit_failure:
    output error_message, 0
    
    mov ah, 01h                              ; waiting for any symbol
    int 21h                                  ; to exit the programm 
    
    ret
 
string db 200 dup('$')   
new_string db 0Ah, 0Dh, '$'
message db "Please, enter the string:", 0Dh, 0Ah, '$'
result_message db 0Dh, 0Ah, "Result : ", '$'
error_message db 0Dh, 0Ah, "Error : the string is empty!", 0Dh, 0Ah, '$'
DOLLAR_OFFSET equ 1
SYMBOL_OFFSET equ 2
SIZE equ 200

end startR_OFFSET equ 1
SYMBOL_OFFSET equ 2
SIZE equ 200

end startSET equ 2
SIZE equ 200

end startT equ 2
SIZE equ 200

end startequ 1
SYMBOL_OFFSET equ 2
SIZE equ 200

end startt