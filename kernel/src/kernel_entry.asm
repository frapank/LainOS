; ----------------------
; |    Kernel Entry    |
; ----------------------

[bits 32]

global _start
extern kernel_main

section .text
_start:
    cli

    push ebx            ; Pass boot_info
    push eax            ; Pass magic_number
    call kernel_main

.halt:
    hlt
    jmp .halt

