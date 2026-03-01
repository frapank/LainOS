#include "utils/kshell/kshell.h"

/*
* kshell.c - Minimal kernel shell implementation
*
* This is a simple, built-in kernel shell used for debugging
* and executing a small set of commands via keyboard input.
* 
* Architecture and logic:
*  - The shell reads characters from the keyboard using getch().
*  - Characters are bufered until the Enter key is pressed.
*  - Input is compared against a static list of commands.
*  - Each command has an identifier, a help string, and a function pointer.
*  - Matching command executes its corresponding function.
*
* Features:
*  - Comand execution supports no arguments (single word commands).
*  - Builtin commands: clear, exit, help, about, binfo.
*  - Feedback is printed to the VGA console using printk_color().
*  - Handles backspace to edit the command buffer.
*  - Input buffer is limited to COMMAND_MAX_INPUT characters.
*
* Notes:
*  - The shell runs in the main kernel context; no threads or multitasking.
*  - It uses only low-level kernel facilities (VGA output, keyboard driver).
*  - Designed for simplicity and clarity, mainly for debugging purposes.
*/

#include <stddef.h>
#include "core/print_vga_text.h"
#include "drivers/keyboard.h"
#include "drivers/vga/vga_text.h"
#include "utils/string.h"
#include "utils/kshell/kshell_commands.h"

static void cmd_help_local(struct kernel_context* ctx);

struct command {
    const char* identifier; 
    const char* help;
    void (*function)(struct kernel_context* ctx);
};

static const struct command commands_list[] = {
    { "clear", "Clear the console",         cmd_clear      },
    { "exit",  "Exit shell",                cmd_exit       },
    { "help",  "Print help",                cmd_help_local },
    { "about", "Print start screen",        cmd_about      },
    { "binfo", "All build info",            cmd_binfo      },
    { "ismem", "Display init memory map",   cmd_ismem      },
    { "echo",  "Print a message",           cmd_echo       },
};

#define COMMAND_LIST_SIZE sizeof(commands_list)/sizeof(commands_list[0])

static inline void check_command(char* cmd_name, struct kernel_context* ctx)
{
    for(size_t i = 0; i < COMMAND_LIST_SIZE; i++){
        struct command cmd = commands_list[i];
        if(kstreql(cmd_name, cmd.identifier)){
            cmd.function(ctx);
            return;
        }
    }

    printk_color("Command %s not found \n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, cmd_name);
}

static void cmd_help_local(struct kernel_context* ctx)
{
    printk_color("Commands list:\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);

    for(size_t i = 0; i < COMMAND_LIST_SIZE; i++){
        struct command cmd = commands_list[i];
        printk_color(" > %s: ", VGA_COLOR_YELLOW, VGA_COLOR_BLACK, cmd.identifier);
        printk_color("%s\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK, cmd.help);
    }
}

#define COMMAND_MAX_INPUT   10
#define PRINT_PREFIX        printk("$ ")

static volatile int running = 0;

void kshell_start(struct kernel_context* ctx)
{
    clear_screenk();
    cmd_about(&ctx); // Welcome message

    running = 1;

    char command_buffer[COMMAND_MAX_INPUT] = {0};
    size_t index = 0;
    PRINT_PREFIX;
    while(running == 1){
        for (;;){
            char c = getch();

            // Enter: Process comands
            if (c == '\n') {
                if(command_buffer[0] == '\0') {
                    new_linek();
                    break;
                }
                new_linek();
                command_buffer[index] = '\0';
                check_command(command_buffer, ctx);
                index = 0;
                kmemset(command_buffer, 0, sizeof(command_buffer));
                break;
            }

            // Backspace: Delete char
            if (c == 0x08 || c == 0x7f) {
                if (index > 0) {
                    index--;
                    command_buffer[index] = '\0';
                    printk("\b \b");
                }
                continue;
            }

            // Normal: Add a char 
            if (index < sizeof(command_buffer) - 1) {
                if (c >= 32 && c <= 126) {
                    command_buffer[index++] = c;
                    printk("%c", c);
                }
            }
        }
        PRINT_PREFIX;
    }
}

void kshell_stop(void)
{
    running = 0;
}
