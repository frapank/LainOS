#include "core/print_vga_text.h"

#include "core/format.h"
#include "core/memory.h"

#define DEFAULT_BACKGROUND_COLOR 0
#define DEFAULT_TEXT_COLOR 15

static struct line_data cursor = {0, 0};

/*
 * scroll_if_needed - scroll screen up if cursor exceeds VGA_HEIGHT
 *
 * Moves existing lines up and clears the new blank rows.
 * Updates cursor.y to stay within visible area.
 */
static void scroll_if_needed(void) 
{
    if (cursor.y < VGA_HEIGHT) 
        return;

    u16 *vga = (u16 *) VIDEO_MEMORY;

    int lines = cursor.y - (VGA_HEIGHT - 1);
    if (lines <= 0) 
        return;
    if (lines > VGA_HEIGHT) 
        lines = VGA_HEIGHT;

    u32 bytes_per_row = VGA_WIDTH * sizeof(u16);
    u32 src_offset = lines * bytes_per_row;
    u32 move_bytes = (VGA_HEIGHT * bytes_per_row) - src_offset;

    kmemmove((void *)vga, (void *)((u8*)vga + src_offset), move_bytes);

    u8 att = ((DEFAULT_BACKGROUND_COLOR & 0x0F) << 4) | (DEFAULT_TEXT_COLOR & 0x0F);
    u16 blank = (' ' | (att << 8));

    u16 *last_row = vga + (VGA_HEIGHT - lines) * VGA_WIDTH;
    for (int r = 0; r < lines; r++)
        kmemset16(last_row + r * VGA_WIDTH, blank, VGA_WIDTH);

    cursor.y = VGA_HEIGHT - 1;
}

/*
 * printk & printk_color - variadic wrapper for vprint_color
 *
 * vprint_color expects a va_list already initialized.
 * We cannot pass the variadic parameters ('...') directly
 * without first initializing a va_list with va_start.
 * This wrapper does exactly that: it creates and initializes
 * args before passing them to vprint_color.
 *
 * Note: without this wrapper, the variadic list would be
 * invalid and behavior would be undefined.
 */
void printk_color(char *msg, enum vga_color text, enum vga_color background,
                 ...) 
{
    __builtin_va_list args;
    __builtin_va_start(args, background);
    vprint_color(msg, text, background, args);
    __builtin_va_end(args);
}

void printk(char *msg, ...) 
{
    __builtin_va_list args;
    __builtin_va_start(args, msg);
    vprint_color(msg, DEFAULT_TEXT_COLOR, DEFAULT_BACKGROUND_COLOR, args);
    __builtin_va_end(args);
}

void vprint_color(char *msg, enum vga_color text, enum vga_color background,
                  __builtin_va_list args) 
{
    for (size_t i = 0; msg[i] != '\0'; i++) {

        char ch = msg[i];

        switch (ch) {
            case '\n':
                cursor.x = 0;
                cursor.y++;
                scroll_if_needed();
                break;

            case '\r':
                vga_put_char(' ', text, background, cursor.x, cursor.y);
                cursor.x = 0;
                scroll_if_needed();
                break;

            case '\b':
                if (cursor.x > 0) {
                    cursor.x--;
                    vga_put_char(' ', text, background, cursor.x, cursor.y);
                }
                break;

            case '%':
                if (msg[i + 1] == 'd') {
                    int val = __builtin_va_arg(args, int);
                    char ascii_buffer[21];
                    int_to_ascii(val, ascii_buffer);
                    printk_color(ascii_buffer, text, background);
                    i++;
                    break;
                }

                if (msg[i + 1] == 'u') {
                    int val = __builtin_va_arg(args, unsigned int);
                    char ascii_buffer[21];
                    int_to_ascii(val, ascii_buffer);
                    printk_color(ascii_buffer, text, background);
                    i++;
                    break;
                }

                if (msg[i + 1] == 'h') {
                    unsigned int uval = __builtin_va_arg(args, unsigned int);
                    char hex_buffer[12];
                    uint_to_hex(uval, hex_buffer);
                    printk_color(hex_buffer, text, background);
                    i++;
                    break;
                }

                if (msg[i + 1] == 'c') {
                    int c = __builtin_va_arg(args, int);
                    char cb[2];
                    cb[0] = (char)c;
                    cb[1] = '\0';
                    printk_color(cb, text, background);
                    i++;
                    break;
                }

                if (msg[i + 1] == 's') {
                    char *s = __builtin_va_arg(args, char *);
                    for (int j = 0; s[j] != '\0'; j++) {
                        if (cursor.x >= VGA_WIDTH) {
                            cursor.x = 0;
                            cursor.y++;
                            scroll_if_needed();
                        }
                        vga_put_char((unsigned char)s[j], text, background,
                                     cursor.x, cursor.y);
                        cursor.x++;
                    }
                    i++;
                    break;
                }

                break;

            default:
                if (cursor.x >= VGA_WIDTH) {
                    cursor.x = 0;
                    cursor.y++;
                    scroll_if_needed();
                }
                vga_put_char((unsigned char)ch, text, background, cursor.x,
                             cursor.y);
                cursor.x++;
        }
    }
}

void new_linek(void)
{
    cursor.x = 0;
    cursor.y++;
    scroll_if_needed();
}

void clear_screenk(void)
{
    u16* vga = (u16*) VIDEO_MEMORY;
    u16 blank = (' ' | (DEFAULT_ATT << 8));

    kmemset16(vga, blank, VGA_WIDTH * VGA_HEIGHT);

    cursor.x = 0;
    cursor.y = 0;
}
