; ========================================================
; int strlen(char *str)
; returns the length of the string pointed to by str
strlen:
    push  ebx           ; save the base pointer
    mov   ebx, eax

loop_start:
    cmp   byte [eax], 0 ; check for null terminator
    jz    loop_end      ; if null terminator, jump to loop_end
    inc   eax           ; increment the pointer
    jmp   loop_start    ; jump to loop_start

loop_end:
    sub   eax, ebx      ; calculate the length of the string
                        ; eax = eax - ebx
    pop   ebx           ; pop the value from the stack to ebx
    ret

; ========================================================
; void print(eax = char *msg)
; prints the string pointed to by msg
print:
    push  ebx           ; save the base pointer
    push  ecx           ; save the counter
    push  edx           ; save the length
    push  eax           ; save the pointer
    call  strlen

    mov   edx, eax      ; message length
    pop   eax           ; restore the pointer

    mov   ecx, eax      ; message to write 
    mov   eax, 4        ; system call for sys_write
    mov   ebx, 1        ; file descriptor 1 is stdout
    int   0x80

    pop   edx           ; restore the length
    pop   ecx           ; restore the counter
    pop   ebx           ; restore the base pointer
    ret

; ========================================================
; void println(eax = char *msg)
; prints the string pointed to by msg and a newline
println:
    push  ebx
    call  print          ; print the normal string

    push  eax
    mov   eax, 0x0a
    push  eax            ; push the newline
    mov   eax, esp
    call  print          ; print the newline
    pop   eax
    pop   eax
    pop   ebx
    ret

exit:
    mov   eax, 1        ; system call for sys_exit
    mov   ebx, 0        ; exit code 0
    int   0x80          ; call kernel
    ret
