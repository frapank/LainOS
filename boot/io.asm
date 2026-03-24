; Select boot mode
select_boot_mode:
    .redo:
    mov ah, 0x00
    int 0x16

    cmp al, '0'
    jb  .redo       ; < '0'
    cmp al, '9'
    ja  .redo       ; > '9'

    sub al, '0'
    cmp al, OPTION_COUNT
    ja .redo
    mov [BOOT_MODE], al
    ret
