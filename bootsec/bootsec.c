#include "desctable.h"
#include "page.h"

void start(){
	//初始化gdt
	lgdt_t gdt = {(unsigned int)&seg,24};
	//初始化选择子
	selector_t selector_cs={0,0,1};
	selector_t selector_ds={0,0,2};

	switchon_a20();
	switchon_protection_mode();
	flush_csseg(selector_cs);
    init_page_entry();
    page_switch_on();


	load_gdt(gdt);
	
}
