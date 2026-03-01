#include "arch/interrupts/idt.h"
#include "arch/io.h"
#include "arch/pic.h"
#include "core/boot_info.h"
#include "core/print_vga_text.h"
#include "core/paging.h"
#include "core/panic.h"
#include "core/kernel_context.h"
#include "utils/kshell/kshell.h"

#define BOOT_IDENTIFIER 0x1BADB002

void kernel_main(u32 magic, u32 boot_info_addr)
{
    struct kernel_context k_context;
    k_context.kernel_boot_info = *boot_info_init(boot_info_addr);

    u32 buffer = 0;
    for(int i = 0; i != k_context.kernel_boot_info.mem_map_entries_count; i++) {
        buffer += k_context.kernel_boot_info.mem_map_entries[i].length;
    }
    k_context.system_ram_kb = buffer / 1024;

    if(magic != BOOT_IDENTIFIER)
        panick("Wrong bootloader identifier");
    paging_init();
    idt_init();
    pic_init();
    sti();

    kshell_start(&k_context);

    panick("No programs running");
}
