#include "core/boot_info.h"

#include "core/print_vga_text.h"

struct boot_info* boot_info_init(u32 boot_info)
{
    return (struct boot_info*)boot_info;
}
