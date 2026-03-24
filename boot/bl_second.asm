[bits 16]
org 0x1000

; Start
second_stage:
    mov dword [BOOT_MODE], 0
    mov dword [BOOT_KERNEL_START], KERNEL_LOCATION
    
    ; boot disk passed by stage1
    mov dl, [0x7E00]
    mov byte [boot_disk], dl

    ; print UI
    call clear_screen
    mov si, ascii_screen
    call print_string

    ; print and select options
    mov si, msg_boot_options
    call print_string
    call select_boot_mode

    call load_smap

    call start_protected

; Data
ascii_screen            db "+----------------------------+",0Dh,0Ah
                        db "|                            |",0Dh,0Ah
                        db "|       L A I N   O S        |",0Dh,0Ah
                        db "|                            |",0Dh,0Ah
                        db "|   experimental bootloader  |",0Dh,0Ah
                        db "|                            |",0Dh,0Ah
                        db "+----------------------------+",0Dh,0Ah,0Dh,0Ah,0
msg_boot_options        db "Press a number to select an option:",0Dh,0Ah
                        db "[1] Normal mode",0Dh,0Ah
                        db "[2] Debug mode",0Dh,0Ah,0

msg_disk_error          db "[-] Error while reading the disk",0Dh,0Ah,0
msg_smem_error          db "[-] Can't get the memory map",0Dh,0Ah,0
msg_test                db "[*] Test reached!",0Dh,0Ah,0

boot_disk               db 1

; Constants 
CODE_SEG                equ gdt_code - gdt_start
DATA_SEG                equ gdt_data - gdt_start

KERNEL_LOCATION         equ 0x2000
KERNEL_SECTORS          equ 18
KERNEL_START_SECTOR     equ 4

OPTION_COUNT            equ 2
CR0_PE                  equ 1

; Boot Info Layout (0x9000):
;  +0  : BOOT_MODE (dword)
;  +4  : BOOT_KERNEL_START (dword)
;  +8  : BOOT_MEMORY_MAP_COUNT (dword)
;  +12 : padding
;  +16 : BOOT_MAP_ENTRIES
BOOT_INFO_ADDR          equ 0x9000
BOOT_MODE               equ BOOT_INFO_ADDR+0
BOOT_KERNEL_START       equ BOOT_INFO_ADDR+4
BOOT_MEMORY_MAP_COUNT   equ BOOT_INFO_ADDR+8
BOOT_MAP_ENTRIES        equ BOOT_INFO_ADDR+16

; Includes
%include "boot/graphics.asm"
%include "boot/file_system.asm"
%include "boot/io.asm"
%include "boot/sys_info.asm"

; Protected mode setup
start_protected:
    cli

    ; clear segments before mode switch
    xor ax, ax
    mov ds, ax
    mov es, ax

    lgdt [gdt_descriptor]               ; load GDT
    mov eax, cr0
    or eax, CR0_PE                      ; set PE bit
    mov cr0, eax

    jmp CODE_SEG:pm_entry

; Global Descriptor Table
gdt_start:

    gdt_null:
        dd 0x0
        dd 0x0
    
    gdt_code:
        dw 0xffff               ; limit (low)
        dw 0x0                  ; base (low)
        db 0x0                  ; base (mid)
        db 0b10011010           ; access
        db 0b11001111           ; flags
        db 0x0                  ; base (high)
    
    gdt_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size of GDT
    dd gdt_start                ; address of GDT

; Protected mode entry
[bits 32]
pm_entry:
    mov ax, DATA_SEG        ; load data segment selector
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x90000        ; setup stack base

    mov eax, 0x1BADB002     ; magic number
    mov ebx, BOOT_INFO_ADDR ; boot_info pointer (kernel args)
    jmp KERNEL_LOCATION

times 2048 - ($ - $$) db 0
