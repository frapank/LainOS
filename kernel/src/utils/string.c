#include "utils/string.h"

s32 kstrncmp(const c8* str1, const c8* str2, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (str1[i] == 0 || str2[i] == 0)
            return (s32)(str1[i] - str2[i]);
        if (str1[i] != str2[i])
            return (s32)(str1[i] - str2[i]);
    }
    return 0;
}

size_t kstrncpy(c8* dest, const c8* src, size_t n)
{
    size_t src_len = 0;
    const c8 *s;

    for (s = src; *s; ++s)
        ++src_len;

    if (n == 0)
        return src_len;

    size_t to_copy = (src_len >= n) ? (n - 1) : src_len;

    for (size_t i = 0; i < to_copy; ++i)
        dest[i] = src[i];

    dest[to_copy] = (u8)'\0';

    return src_len;
}

size_t strlen_t(const c8* ch)
{
    size_t i = 0;
    while (ch[i] != 0)
        i++;
    return i;
}

s32 kstreql(const c8* ch1, const c8* ch2)
{
    size_t size = strlen_t(ch1);

    if (size != strlen_t(ch2)) return 0;

    for (size_t i = 0; i <= size; i++)
        if (ch1[i] != ch2[i]) return 0;

    return 1;
}
