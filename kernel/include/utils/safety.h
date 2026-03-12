#ifndef SAFETY_H
#define SAFETY_H

#if defined(__clang__) || defined(__GNUC__)
#define ARGS_NONNULL(...)   __attribute__((nonnull(__VA_ARGS__)))
#define WARN_UNUSED_RESULT  __attribute__((warn_unused_result))
#else
#define ARGS_NONNULL(...)
#define WARN_UNUSED_RESULT
#endif

#endif
