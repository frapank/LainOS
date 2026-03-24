print_string:
    mov ah, 0x0e            ; BIOS display function
    .print_loop:
        lodsb               ; Increment SI to point to next character and
                            ; load character in al
        cmp al, 0           ; Check for null terminator (end of string)
        je .print_done      ; Jump if end of string
        int 0x10            ; Call BIOS video interrupt
        jmp .print_loop     ; Repeat loop for next character
    .print_done:
        ret

clear_screen:
    mov ah, 0x06            ; Scroll window function
    xor al, al              ; Clear lines to scroll
    mov bh, 0x07            ; Attribute for blank lines
    xor cx, cx              ; Upper left corner (row/col)
    mov dh, 24-1            ; Lower right row
    mov dl, 80-1            ; Lower right column
    int 0x10                ; BIOS video interrupt to scroll

    mov ah, 0x02            ; Set cursor position
    xor bh, bh              ; Page number
    xor dh, dh              ; Row = 0
    xor dl, dl              ; Column = 0
    int 0x10                ; BIOS interrupt to set cursor
    ret
