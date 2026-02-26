#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include "utils/types.h"

struct memory_map {
    u64 base;
    u64 length;
    u32 type;
};

struct boot_info {
    u8 boot_mode;
    u32 mem_map_entries_count;
    struct memory_map mem_map_entries[32];
};

struct boot_info* boot_info_init(u32 boot_info);

#endif
