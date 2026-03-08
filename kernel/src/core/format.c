#include "core/format.h"

void int_to_ascii(u32 n, c8* buffer)
{

    char temp[10];
    int i = 0;

    do {
        int ret = n % 10;
        temp[i++] = ret + '0';
        n /= 10;
    } while(n > 0);

    for(int j = 0; j < i; j++)
        buffer[j] = temp[i - j - 1];

    buffer[i] = 0;
}

u32 asciihex_to_uint(c8* buffer)
{
    u32 result = 0;

    for (int i = 0; buffer[i] != '\0'; i++) {
        char c = buffer[i];

        if (c >= '0' && c <= '9')
            result = result * 16 + (c - '0');

        else if (c >= 'A' && c <= 'F')
            result = result * 16 + (c - 'A' + 10);

        else if (c >= 'a' && c <= 'f')
            result = result * 16 + (c - 'a' + 10);

        else return 0;
    }

    return result;
}

void uint_to_hex(u32 n, c8* buffer)
{
    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < 8; i++) {
        u8 nibble = (n >> (28 - i * 4)) & 0xF;
        if (nibble < 10)
            buffer[2 + i] = '0' + nibble;
        else
            buffer[2 + i] = 'A' + (nibble - 10);
    }

    buffer[10] = '\0';
}
