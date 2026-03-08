#ifndef FORMAT_H
#define FORMAT_H

#include "utils/types.h"

void uint_to_hex(u32 n, c8* buffer);
void int_to_ascii(u32 n, c8* buffer);
u32 asciihex_to_uint(c8* buffer);

#endif
