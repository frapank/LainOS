#ifndef boot_info_h
#define boot_info_h

#include "utils/types.h"

/*
 * base: physical start address
 * length: dimension in byte
 * type: 
 * - 1: available 
 * - 2: reserved
 * - 3: acpi reclaimable
 * - 4: acpi nvs
 * - 5: bad memory
 */
struct memory_map {
    u64 base;
    u64 length;
    u32 type; 
    u32 acpi_extended;
} __attribute__((packed));

struct boot_info {
    u32 boot_mode;
    u32 mem_map_entries_count;
    struct memory_map mem_map_entries[12];
} __attribute__((packed));

struct boot_info* boot_info_init(u32 boot_info);

#endif
