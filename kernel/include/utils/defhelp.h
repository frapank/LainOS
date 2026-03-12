#ifndef DEFHELP_H
#define DEFHELP_H

#if defined(__clang__) || defined(__GNUC__)
#define likely(x)           __builtin_expect(!!(x), 1)
#define unlikely(x)         __builtin_expect(!!(x), 0)
#define ALWAYS_INLINE       __always_inline 
#else
#define likely(x)
#define unlikely(x)
#define ALWAYS_INLINE
#endif


#endif
