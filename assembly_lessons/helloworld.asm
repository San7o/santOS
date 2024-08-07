; $ nasm -f elf helloworld.asm
; $ ld -m elf_i386 helloworld.o -o helloworld
; $ ./helloworld

%include      "functions.asm"


SECTION .data
usage_msg    db   "Usage: ./my_program <stirng1> <string2> ...", 0x0
user_message db   "Please enter your name: ", 0x0

SECTION .bss
user_input:    RESB    255


SECTION .text
global _start         ; subroutine, must be declared for linker (ld)

; NOTE ON ARGUMENTS:
; When we run our program, any passed arguments are
; loaded onto the stack in reverse order. The name
; of the program is then loaded onto the stack and
; lastly the total number of arguments is loaded
; onto the stack. The last two stack items for a
; NASM compiled program are always the name of the
; program and the number of passed arguments.
_start:
    pop   ecx          ; pop the number of arguments
    pop   eax          ; pop the name of the program

    cmp   ecx, 1       ; check args
    jz    show_usage

next_arg:              ; print the arguments
    cmp   ecx, 1
    jz    ask_input
    pop   eax
    call  println
    dec   ecx
    jmp   next_arg

ask_input:              ; read the user input
    mov   eax, user_message
    call  print

    mov   eax, 3
    mov   ebx, 1
    mov   ecx, user_input
    mov   edx, 255
    int   0x80
    mov   eax, user_input
    call  println

done:
    call  exit

show_usage:
    mov   eax, usage_msg
    call  println
    call exit
