#include <init/page.h>

struct page_entry g_pde[1024] = {0};
struct page_entry g_pte[1024] = {0};

//加载pde,同时开启cr0 pg位
static void load_page_setup(ADDR);

//页面初始化暂时留到最后完善
void init_page_entry(void){
    //设计页面布局
    //XXX
    
   load_page_setup(); 
}

static void load_page_setup(void){
    asm volatile ("movl %%eax,%%cr3" \
            "movl %%cr0,%%ebx"       \
            "or $0x80000000,%%ebx"   \
            "movl %%ebx,%%cr0"       \
            ::"a"(g_pde));
}
