asm(".code16gcc\n");
#include <type.h>
#include <init/gdt.h>
#include <init/page.h>
#include <UI/vga_basic_io.h>
#include <interupt/intr_init.h>


//开启a20总线开关
static void a20_open(void);
//打开CPU PE位
static void pe_setup(void);
//设置保护模式
static void protectmode_setup(void);

//内存分布结构
struct ADRS{
	uint16_t 15m_below;
	uint16_t 15m_under;
};
//获取内存分布
static void get_ADRS(struct ADRS* adrs);

struct kernel_mem{
	uint32_t totalkb;
} kernel_mem_t;

//保存内存大小
kernel_mem_t kmem_all = {0};

//主函数
void start(void){
	struct ADRS adrs;
	protectmode_setup();

	init_gdt();
	//刷新执行流同时进入到32位保护模式
	flush_executionflow();

	//初始化vga接口，同时创建stdout(0)文件描述符
	fileoperations_init();

	get_ADRS(&adrs);
	kmem_all.totalkb = adrs.15m_below + adrs.15m_under*64;
	printf("kernel.totalkb = %dKb \n",kmem_all.totalkb);

    init_page_entry();

    //开启分页后需要重新加载gdt
	init_gdt();

	//idt初始化
	init_idt();
}

static void a20_open(void){
	asm ( \
			"inb $0x92,%%al \n" \
			"or $0x02,%%al \n"	\
			"outb %%al,$0x92 \n"\
			);
}

static void pe_setup(void){
	asm (	\
			"movq %%cr0, %%eax\n"	\
			"or $0x01,%%eax\n"	\
			"movq %%eax, %%cr0\n"	\
			);
}

static void protectmode_setup(void){
	a20_open();
	pe_setup();
}

static void get_ADRS(struct ADRS* adrs){
	asm( \
			"adrs_tryagin:\n"		\
			"movw $0xe801,%%ax\n"	\
			"int 0x15\n"			\
			"jc adrs_tryagin\n"		\
			"movw %%ax,%0\n"		\
			"movw %%bx,%1\n"		\
			:"=m"(adrs->15m_below),"=m"(adrs->15m_under)	\
			::"ax","bx"				\
			);
}
