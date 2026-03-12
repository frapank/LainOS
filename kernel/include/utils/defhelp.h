#ifndef DEFHELP_H
#define DEFHELP_H

#define likely(x)           __builtin_expect(!!(x), 1)
#define unlikely(x)         __builtin_expect(!!(x), 0)
#define ALWAYS_INLINE       __always_inline 

#endif
