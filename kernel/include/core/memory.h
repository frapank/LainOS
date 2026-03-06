#ifndef MEMORY_H
#define MEMORY_H

#include "utils/types.h"

void* kmemset(void* ptr, u8 value, u32 count);
void* kmemset16(void* ptr, u16 value, u32 count);
void* kmemset32(void* ptr, u32 value, u32 count);

void *kmemmove(void *dest, const void *src, phys_addr_t n);

#endif
