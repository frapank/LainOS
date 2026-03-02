#include "core/paging.h"

/*
 * -- INFORMATION MAY BE REMOVED LATER --
 * Page Table Entry (PTE):
 * 31-12: 20 bits = Physical address (frame address)
 * 11-9: 3 bits = Reserved (unused)
 * 8-2: 7 bits = Flags
 *        - P (Present)      : 1 bit  --> Page is in memory
 *        - R/W (Read/Write) : 1 bit  --> Page is readable/writable
 *        - U/S (User/Root)  : 1 bit  --> Access in user mode (1) or kernel mode (0)
 *        - A (Accessed)     : 1 bit  --> Page was recently accessed
 *        - D (Dirty)        : 1 bit  --> Page was written to
 *        - G (Global)       : 1 bit  --> Global page (not removed on context switch)
 * 1-0: 2 bits = Reserved

 * Page Directory Entry (PDE):
 * 31-22: 20 bits = Physical address of the Page Table
 * 21-12: 10 bits = Reserved
 * 11-9: 3 bits = Flags
 *        - P (Present)      : 1 bit  --> Page table is in memory
 *        - R/W (Read/Write) : 1 bit  --> Page table is readable/writable
 *        - U/S (User/Sup) : 1 bit  --> Page table accessible in user mode (1) or kernel mode (0)
 *        - A (Accessed)     : 1 bit  --> Page table was recently used
 *        - D (Dirty)        : 1 bit  --> Usually unused in PDEs
 *        - PS (Page Size)   : 1 bit  --> If 1, maps a 4 MB page (large page)
 * 8-2: 7 bits = Reserved
 * 1-0: 2 bits = Cache and write flags (PWT, PCD)
 */

#include "utils/types.h"

#define page_size 4096
#define entries_num 1024

static __attribute__((aligned(4096))) u32 page_dir[entries_num];
static __attribute__((aligned(4096))) u32 page_table_first[entries_num];

static void paging_setup(void) 
{
    u32 current_address = 0; 
    for(int i = 0; i < entries_num; i++){
        page_table_first[i] = current_address | 0x3;
        current_address += page_size;
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

void paging_init(struct kernel_context* ctx)
{
    paging_setup();
    paging_load_dir();
    paging_enable();
}
