#ifndef FORMAT_H
#define FORMAT_H

#include "utils/types.h"
#include "utils/safety.h"

ARGS_NONNULL(2) void uint_to_hex(u32 n, c8* buffer);
ARGS_NONNULL(2) void int_to_ascii(u32 n, c8* buffer);
WARN_UNUSED_RESULT ARGS_NONNULL(1) u32 asciihex_to_uint(c8* buffer);

#endif
