#ifndef PHYSICAL_MEMORY_MANAGMENT_H
#define PHYSICAL_MEMORY_MANAGMENT_H

#include "utils/types.h"

bool phmm_is_address_free(u32 address);
void phmm_mark_region_used(u32 init_address, u32 size);
void phmm_mark_region_free(u32 init_address, u32 size);
u32* phmm_alloc_blocks(u32 num);
void phmm_free_blocks(u32* addr, u32 num_block);
u32 phmm_init(u32 init_address, u32 size);

#endif
