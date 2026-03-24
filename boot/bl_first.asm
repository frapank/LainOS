[bits 16]
org 0x7c00

; Entry
start:
    cli

    ; set Stack
    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF
    mov bp, sp

    ; set Data Segment
    mov ax, 0x0000
    mov ds, ax
    mov es, ax

    ; store boot_disk (for stage2)
    mov [boot_disk], dl
    mov byte [0x7E00], dl

    ; print log
    mov si, msg_disk_log
    call print_string

    ; load second stage
    call read_disk
    call start_second

; Data
msg_disk_log            db "[*] Searching second stage...",0Dh,0Ah,0
msg_disk_error          db "[-] Can't find second stage, stopped" ,0Dh,0Ah,0

boot_disk               db  0

STAGE_LOCATION          equ 0x1000
STAGE_SECTORS           equ 4
STAGE_START_LBA         equ 2048

dap:
    db 0x10                             ; packet size
    db 0                                ; reserved
    dw STAGE_SECTORS                    ; number of sectors
    dw STAGE_LOCATION                   ; offset
    dw 0x0000                           ; segment
    dq STAGE_START_LBA                  ; LBA start


; Includes
%include "boot/graphics.asm"

; Setup Protected
read_disk:
    lea si, [dap]               ; DS:SI = disk packet 
    mov ah, 0x42                ; extended read 
    mov dl, [boot_disk] 
    int 0x13 
    jc disk_error 
    ret

disk_error:
    mov si, msg_disk_error
    call print_string
    hlt
    jmp $

start_second:
    jmp STAGE_LOCATION

times 446-($-$$) db 0           ; Fill up to 510 bytes
