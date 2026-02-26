#include "core/paging.h"

#include "utils/types.h"

#define PAGE_SIZE 4096
#define ENTRIES_NUM 1024

static __attribute__((aligned(4096))) u32 page_dir[ENTRIES_NUM];
static __attribute__((aligned(4096))) u32 page_table_first[ENTRIES_NUM];

/* todo:
 * Detect the ram and assign the correct
 * page table size
 */

static void paging_setup(void) 
{
    u32 current_address = 0;
    for(int i = 0; i < ENTRIES_NUM; i++){
        page_table_first[i] = current_address | 0x3;
        current_address += PAGE_SIZE;
    }

     page_dir[0] = (uint32_t)page_table_first | 0x3;
}

static inline void paging_load_dir(void)
{
    u32 cr3_value = (u32)page_dir;
    __asm__ volatile ("mov %0, %%cr3" : : "r"(cr3_value));
}

static void paging_enable(void)
{
    __asm__ (
            "mov %%cr0, %%eax;"
            "or $0x80000000, %%eax;"
            "mov %%eax, %%cr0;"
            :
            :
            : "%eax"
            );
}

void paging_init(void)
{
    paging_setup();
    paging_load_dir();
    paging_enable();
}
