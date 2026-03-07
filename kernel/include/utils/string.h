#ifndef STRING_H
#define STRING_H

#include "utils/types.h"

s32 kstrncmp(const c8* str1, const c8* str2, size_t n);
s32 kstreql(const c8* ch1, const c8* ch2);
size_t kstrncpy(c8* dest, const c8* src, size_t n);
size_t strlen_t(const c8* ch);

#endif

