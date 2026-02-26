#ifndef KERNEL_CONTEXT_H
#define KERNEL_CONTEXT_H

#include "core/boot_info.h"

struct kernel_context {
    struct boot_info kernel_boot_info;
};

#endif
