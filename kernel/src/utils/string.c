#include "utils/string.h"

s32 kstrncmp(const u8* str1, const u8* str2, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (str1[i] == 0 || str2[i] == 0)
            return (s32)(str1[i] - str2[i]);
        if (str1[i] != str2[i])
            return (s32)(str1[i] - str2[i]);
    }
    return 0;
}

size_t kstrncpy(u8* dest, const u8* src, size_t n)
{
    size_t src_len = 0;
    const u8 *s;

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

size_t strlen_t(const u8* ch)
{
    size_t i = 0;
    while (ch[i] != 0)
        i++;
    return i;
}

s32 kstreql(const u8* ch1, const u8* ch2)
{
    size_t size = strlen_t(ch1);

    if (size != strlen_t(ch2)) return 0;

    for (size_t i = 0; i <= size; i++)
        if (ch1[i] != ch2[i]) return 0;

    return 1;
}
