#ifndef STRING_H
#define STRING_H

#include "utils/types.h"

s32 kstrncmp(const u8* str1, const u8* str2, size_t n);
s32 kstreql(const u8* ch1, const u8* ch2);
size_t kstrncpy(u8* dest, const u8* src, size_t n);
size_t strlen_t(const u8* ch);

#endif

