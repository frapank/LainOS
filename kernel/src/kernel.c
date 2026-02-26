#include "arch/interrupts/idt.h"
#include "arch/io.h"
#include "arch/pic.h"
#include "core/boot_info.h"
#include "core/print_vga_text.h"
#include "core/panic.h"
#include "core/kernel_context.h"
#include "utils/kshell/kshell.h"

#define BOOT_IDENTIFIER 0x1BADB002

void kernel_main(u32 magic, u32 boot_info_addr)
{
    struct kernel_context k_context;
    k_context.kernel_boot_info = *boot_info_init(boot_info_addr);

    if(magic != BOOT_IDENTIFIER)
        panick("Wrong bootloader identifier");
    idt_init();
    pic_init();
    sti();

    kshell_start();

    panick("No programs running");
}
