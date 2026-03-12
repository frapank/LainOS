#include "arch/interrupts/irq.h"
#include "core/print_vga_text.h"
#include "drivers/keyboard.h"
#include "utils/defhelp.h"
#include "utils/types.h"

static void default_irq_handler(struct regs* regs)
{
    (void)regs;
}

#define UNDECLARED_IRQ(n) \
void irq##n##_man(struct regs *r) __attribute__((alias("default_irq_handler")))

UNDECLARED_IRQ(0);
UNDECLARED_IRQ(2);
UNDECLARED_IRQ(3);
UNDECLARED_IRQ(4);
UNDECLARED_IRQ(5);
UNDECLARED_IRQ(6);
UNDECLARED_IRQ(7);
UNDECLARED_IRQ(8);
UNDECLARED_IRQ(9);
UNDECLARED_IRQ(10);
UNDECLARED_IRQ(11);
UNDECLARED_IRQ(12);
UNDECLARED_IRQ(13);
UNDECLARED_IRQ(14);
UNDECLARED_IRQ(15);

static void irq1_man(struct regs *r)
{
    (void)r;
    keyboard_interrupt_handler();
}

static void (* const irq_handlers[16])(struct regs*) = {
    irq0_man,  irq1_man,  irq2_man,  irq3_man,
    irq4_man,  irq5_man,  irq6_man,  irq7_man,
    irq8_man,  irq9_man,  irq10_man, irq11_man,
    irq12_man, irq13_man, irq14_man, irq15_man
};

void __attribute__((cdecl)) i686_IRQ_Handler(struct regs* regs)
{
    u8 irq = (u8) regs->int_no;
    if (likely(irq >= 0x20)) 
        irq -= 0x20;
    
    if (unlikely(irq >= 16)) {
        printk("%t%bUnknown irq number: %d\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, irq);
        return;
    }

    irq_handlers[irq](regs);
}

