#ifndef TYPES_H
#define TYPES_H

/* Base types */
typedef unsigned char      u8;
typedef signed char        s8;
typedef unsigned short     u16;
typedef signed short       s16;
typedef unsigned int       u32;
typedef signed int         s32;
typedef unsigned long long u64;
typedef signed long long   s64;

/* Bool types */
typedef _Bool bool;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

/* Null type */
#define NULL ((void*)0)

/* Size type */
#if defined(__SIZE_TYPE__)
typedef __SIZE_TYPE__ size_t;
#else 
typedef u32 size_t;
#endif

/* Kernel types */
typedef u32 phys_addr_t;
typedef u32 addr_t;

/* Control */
_Static_assert(sizeof(u8)  == 1, "u8 should be 1 byte");
_Static_assert(sizeof(u16) == 2, "u16 should be 2 byte");
_Static_assert(sizeof(u32) == 4, "u32 should be 4 byte");
_Static_assert(sizeof(u64) == 8, "u64 should be 8 byte");
_Static_assert(sizeof(s64) == 8, "s64 should be 8 byte");

#endif
