#ifndef INTR_INIT_H
#define INTR_INIT_H

//部分中断栈里会有errcode导致栈指针发生变化，所以手动压入0使栈指针保持一致
#define INTR_ERRCODE "nop\n"
#define INTR_ZERO    "pushl 0\n"

#define INTR(vect,func,code) void intr_init_#vect (void) {   \
                            if(!func) func();           \
                            asm volatile( code \
				    "movb $0x20,%%al\n"     \
                                    "outb %%al,$0x20\n"     \
                                    "outb %%al,$0x20\n"     \
                                    "addl $4,%%esp\n "      \
                                    "iret\n "); 		    \
                        }

#define INTR_ADDR(vect) intr_init_#vect

#endif
