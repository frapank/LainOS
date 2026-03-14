#ifndef KSHELL_COMMANDS_H
#define KSHELL_COMMANDS_H

#include "core/kernel_context.h"

/* Physical Memory Tester */
void cmd_phy_alloc(struct kernel_context* ctx);
void cmd_phy_free(struct kernel_context* ctx);

/* General Info Commands */
void cmd_ismem(struct kernel_context* ctx);
void cmd_about(struct kernel_context* ctx);
void cmd_binfo(struct kernel_context* ctx);

/* Functional */
void cmd_echo(struct kernel_context* ctx);
void cmd_clear(struct kernel_context* ctx);
void cmd_exit(struct kernel_context* ctx);

#endif
