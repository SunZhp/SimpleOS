#ifndef INTR_INIT_H
#define INTR_INIT_H

#define INTR(vect,func) void intr_init_#vect (void) {   \
                            if(!func) func();           \
                            asm volatile("movb $0x20,%%al\n"    \
                                    "outb %%al,$0x20\n"         \
                                    "outb %%al,$0x20\n");   \
                        }

#define INTR_ADDR(vect) intr_init_#vect

#endif
