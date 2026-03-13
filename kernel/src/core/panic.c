#include "core/panic.h"

#include "arch/io.h"
#include "core/print_vga_text.h"

void panick(char *msg)
{
    cli();
    clear_screenk();

    printk("%t\n]== KERNEL PANIC ==-\n\n", 
            VGA_COLOR_LIGHT_RED);
    printk(" \t%t %s\n\n", VGA_COLOR_WHITE, msg);

    for (;;) hlt();
}
