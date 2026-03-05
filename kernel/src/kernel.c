#include "arch/interrupts/idt.h"
#include "arch/io.h"
#include "arch/pic.h"
#include "core/boot_info.h"
#include "core/print_vga_text.h"
#include "core/paging.h"
#include "core/physical_memory_manager.h"
#include "core/panic.h"
#include "core/kernel_context.h"
#include "utils/kshell/kshell.h"

#define BOOT_IDENTIFIER 0x1BADB002
#define KERNEL_END_ADDRESS 0x00100000 // May change

static u32 calculate_total_memory(struct boot_info* boot_info)
{
    u32 total_mem_size = 0;
    for(u32 i = 0; i != boot_info->mem_map_entries_count; i++) {
        total_mem_size += boot_info->mem_map_entries[i].length;
    }
    return total_mem_size;
}

static void system_init(struct kernel_context* ctx, u32 magic)
{
    if(magic != BOOT_IDENTIFIER)
        panick("Wrong bootloader identifier");

    idt_init();
    pic_init();

    u32 alloc_bytes = (ctx->system_ram_kb * 1024);
    u32 bitmap_size = phmm_init(KERNEL_END_ADDRESS, alloc_bytes);
    if (bitmap_size == 0 || bitmap_size > alloc_bytes) 
        panick("phmm_init failed");

    u32 start_region = 0x00000000;
    u32 bytes_to_reserve = (KERNEL_END_ADDRESS - start_region) + bitmap_size;
    phmm_mark_region_used(start_region, bytes_to_reserve);

    //paging_init(ctx);

    sti();   
}

void kernel_main(u32 magic, u32 boot_info_addr)
{
    struct kernel_context ctx;
    ctx.kernel_boot_info = *boot_info_init(boot_info_addr);

    u32 total_mem_size = calculate_total_memory(&ctx.kernel_boot_info);
    ctx.system_ram_kb = total_mem_size / 1024;

    system_init(&ctx, magic);

    kshell_start(&ctx);
    panick("No programs running");
}
