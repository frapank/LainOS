#include "core/panic.h"

#include "arch/io.h"
#include "core/print_vga_text.h"

void panick(char* msg)
{
    cli();
    clear_screenk();
    printk("%t%bKERNEL PANIC:\n%s", VGA_COLOR_RED, VGA_COLOR_BLACK, msg);
    for(;;) {
        hlt();
    }
}
