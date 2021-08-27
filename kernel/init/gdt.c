#include <init/gdt.h>

void init_gdt(void){
	struct gdt_reg gdtr = {0xff,(uint32_t)&gdt};
	asm volatile ( "lgdt %0"::"=m"(gdtr):);

	printf("init global descripte-table done...\n");
}


void flush_executionflow(void){
	printf("entering 32bits mode!\n")
	asm volatile (								\
			"pushw ds_sel\n" 					\
			"popw  %%ds\n" 						\
			"jmpl cs_sel:flush_do_nothing \n" 	\
			".code32gcc\n"					\
			"flush_do_nothing:\n"			\
			"nop\n");
}
