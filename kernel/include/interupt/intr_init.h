#ifndef INTR_INIT_H
#define INTR_INIT_H

//部分中断栈里会有errcode导致栈指针发生变化，所以手动压入0使栈指针保持一致
#define INTR_ERRCODE "nop\n"
#define INTR_ZERO    "pushl 0\n"

#define IDT_CNT 32

#define INTR(vect,func,code) void intr_init_#vect (void) {   \
                            asm volatile( code \
				    				"movb $0x20,%%al\n"     \
                                    "outb %%al,$0x20\n"     \
                                    "outb %%al,$0x20\n"     \
                                    "addl $4,%%esp\n ");    \
                           	if(func) func();           		\
							asm volatile ("iret"); 			\
                        }

#define INTR_ADDR(vect) intr_init_#vect

#include <type.h>
#include <init/gdt.h>

//中断描述符表结构
struct idt_desc{
	int16_t offset_low;
	struct dt_selector selector;
	int8_t reserved;
	int8_t type:4;
	int8_t s:1;
	int8_t dpl:2;
	int8_t p:1;
	int16_t offset_high;
} __attribute__ ((packed));

extern struct idt_desc idt[IDT_CNT];

void init_idt(void);

#endif
