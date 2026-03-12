#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "utils/safety.h"

void keyboard_interrupt_handler(void);
WARN_UNUSED_RESULT char getch(void);

#endif
