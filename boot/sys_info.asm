; ----------------------
; |      Sys Info      |
; ----------------------

; -----------------
; laod memory map 
load_smap:
    push bp
    mov bp, sp
    pushad

    xor ebx, ebx
    lea di, [BOOT_MAP_ENTRIES]
    mov dword [BOOT_MEMORY_MAP_COUNT], 0

.next_entry:
    mov eax, 0xE820
    mov edx, 0x534D4150         ; 'SMAP'
    mov ecx, 24
    int 15h

    jc .done_or_error

    cmp eax, 0x534D4150
    jne .error

    add di, 24
    inc dword [BOOT_MEMORY_MAP_COUNT]

    test ebx, ebx
    jz .done

    jmp .next_entry

.done_or_error:
    cmp dword [BOOT_MEMORY_MAP_COUNT], 0
    je .error

.done:
    popad
    pop bp
    ret

.error:
    mov si, msg_smem_error
    call print_string
    hlt
    jmp $
