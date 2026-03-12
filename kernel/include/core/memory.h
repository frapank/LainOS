#ifndef MEMORY_H
#define MEMORY_H

#include "utils/types.h"
#include "utils/safety.h"

ARGS_NONNULL(1) void* kmemset(void* ptr, u8 value, u32 count);
ARGS_NONNULL(1) void* kmemset16(void* ptr, u16 value, u32 count);
ARGS_NONNULL(1) void* kmemset32(void* ptr, u32 value, u32 count);
ARGS_NONNULL(1) void* kmemmove(void *dest, const void *src, size_t n);

#endif
