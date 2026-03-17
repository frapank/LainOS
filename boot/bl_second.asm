; ----------------------
; |    Second Stage    |
; ----------------------

[bits 16]
org 0x1000

second_stage:
    mov dword [BOOT_MODE], 0
    mov dword [BOOT_KERNEL_START], KERNEL_LOCATION

    call clear_screen
    mov si, ascii_screen
    call print_string
    mov si, msg_boot_options
    call print_string

    call select_boot_mode       ; Save in BOOT_MODE
    
    mov dl, [0x7E00]            ; Get the saved boot_sector
    call read_disk
    call load_smap
    call start_protected

; ----------------------
; Data
msg_disk_error          db "[-] Error while reading the disk",0Dh,0Ah,0
msg_smem_error          db "[-] Can't get the memory map",0Dh,0Ah,0
msg_test                db "[*] Test reached!",0Dh,0Ah,0
msg_boot_options        db "Press a number to select an option:",0Dh,0Ah
                        db "[1] Normal mode",0Dh,0Ah
                        db "[2] Debug mode",0Dh,0Ah,0

ascii_screen            db "+----------------------------+",0Dh,0Ah
                        db "|                            |",0Dh,0Ah
                        db "|       L A I N   O S        |",0Dh,0Ah
                        db "|                            |",0Dh,0Ah
                        db "|   experimental bootloader  |",0Dh,0Ah
                        db "|                            |",0Dh,0Ah
                        db "+----------------------------+",0Dh,0Ah,0Dh,0Ah,0

SMAP_BUFFER             db 20

CODE_SEG                equ gdt_code - gdt_start   ; Offset of code segment in GDT
DATA_SEG                equ gdt_data - gdt_start   ; Offset of data segment in GDT
KERNEL_LOCATION         equ 0x2000                 ; Load address of kernel
KERNEL_SECTORS          equ 18                     ; Number of sectors to read
KERNEL_START_SECTOR     equ 4                      ; Start sector on disk

OPTION_COUNT            equ 2
BOOT_INFO_ADDR          equ 0x9000

BOOT_MODE               equ BOOT_INFO_ADDR+0
BOOT_KERNEL_START       equ BOOT_INFO_ADDR+4
BOOT_MEMORY_MAP_COUNT   equ BOOT_INFO_ADDR+8
                        ; Padding 4 byte
BOOT_MAP_ENTRIES        equ BOOT_INFO_ADDR+16

; ----------------------
; Includes
%include "boot/graphics.asm"
%include "boot/disk.asm"
%include "boot/io.asm"
%include "boot/sys_info.asm"

; ----------------------
; Protected mode setup
start_protected:
    cli                                 ; Disable interrupts
    xor ax, ax
    mov ds, ax                          ; DS = 0
    mov es, ax                          ; ES = 0
    lgdt [gdt_descriptor]               ; Load GDT
    mov eax, cr0
    or eax, 1                           ; Set PE bit to enable protected mode
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode   ; Far jump to flush prefetch queue

; ----------------------
; Global Descriptor Table
gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff               ; Limit low
    dw 0x0                  ; Base low
    db 0x0                  ; Base middle
    db 0b10011010           ; Access byte (code segment)
    db 0b11001111           ; Flags
    db 0x0                  ; Base high

gdt_data:
    dw 0xffff               ; Limit low
    dw 0x0                  ; Base low
    db 0x0                  ; Base middle
    db 0b10010010           ; Access byte (data segment)
    db 0b11001111           ; Flags
    db 0x0                  ; Base high

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of GDT
    dd gdt_start                ; Address of GDT

; ----------------------
; Protected mode entry
[bits 32]
start_protected_mode:
    mov ax, DATA_SEG        ; Load data segment selector
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; Setup stack base
    mov esp, ebp            ; Initialize stack pointer

    mov eax, 0x1BADB002     ; Magic number
    mov ebx, BOOT_INFO_ADDR
    jmp KERNEL_LOCATION     ; Jump to loaded kernel

times 2048 - ($ - $$) db 0
