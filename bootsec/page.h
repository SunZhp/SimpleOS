#ifndef __BOOTSEC_PAGE_H__
#define __BOOTSEC_PAGE_H__

typedef struct page_entry {
    int p:1;
    int rw:1;
    int us:1;
    int pwt:1; 
    int pcd:1; //高速缓存用
    int a:1; //由CPU设置,表示是否被CPU访问过了
    int d:1;
    int pat:1;
    int g:1; //表示是否存在于tlb缓存中,后面会根据情况设置该位
    int avl:3;//后续的操作系统会用该位
    int base:20;
} page_entry_t;

//二级页表页目录项数量为1024
#define PAGE_TABLE_SIZE         1024
//二级页表页表项数量为1024
#define PAGE_TABLE_ENTRY_SIZE   1024

#define PAGE_USR_RATE (3/4)

//使用二级页表
page_entry_t page_table_l1[PAGE_TABLE_SIZE];
page_entry_t page_entry[PAGE_TABLE_SIZE][PAGE_TABLE_ENTRY_SIZE];

void init_page_entry();
int page_switch_on();

//初始化页表和页表项
void init_page_entry(){
    int i,j;
    int ulimit = PAGE_USR_RATE * PAGE_TABLE_SIZE;
    //低3G划分给不同进程，高1G划分给内核
    for(i=0;i<PAGE_TABLE_SIZE;i++){
        page_table_l1[i].p = 0;
        page_table_l1[i].rw = 1;
        page_table_l1[i].us = (i < ulimit);
        page_table_l1[i].pwt = 0;
        page_table_l1[i].pcd = 0;
        page_table_l1[i].a = 0;
        page_table_l1[i].d = 0;
        page_table_l1[i].pat = 0;
        page_table_l1[i].g = 0;
        page_table_l1[i].avl = 0;
        page_table_l1[i].base = (((int)page_entry[i])>>12);
        
        for(j=0;j<PAGE_TABLE_ENTRY_SIZE;j++){
            page_entry[i][j].p = 0;
            page_entry[i][j].rw = 1;
            page_entry[i][j].us = (i < ulimit);
            page_entry[i][j].pwt = 0;
            page_entry[i][j].pcd = 0;
            page_entry[i][j].a = 0;
            page_entry[i][j].d = 0;
            page_entry[i][j].pat = 0;
            page_entry[i][j].g = 0;
            page_entry[i][j].avl = 0;
            page_table_l1[i].base = 0;
        }
    }

    //同时设置物理内存的低4K(中断向量表)映射到内核空间的第一个页表中
    page_entry[ulimit][0].pcd = 1;
    page_entry[ulimit][0].g = 1;
    page_entry[ulimit][0].base = 0;

    //同时设置页表及页表项到第2-1025个页表中
    page_entry[ulimit][1].pcd = 1;
    page_entry[ulimit][1].g = 1;
    page_entry[ulimit][1].base = (((int)page_table_l1)>>12);
    for(i=2;i<1026;i++){
        page_entry[ulimit+i/PAGE_TABLE_SIZE][i%PAGE_TABLE_ENTRY_SIZE].pcd = 1;
        page_entry[ulimit+i/PAGE_TABLE_SIZE][i%PAGE_TABLE_ENTRY_SIZE].g = 1;
        page_entry[ulimit+i/PAGE_TABLE_SIZE][i%PAGE_TABLE_ENTRY_SIZE].base = (((int)page_entry[ulimit+i/PAGE_TABLE_SIZE])>>12);
    }
}

int page_switch_on(){
    asm volatile(" \
            movl %0,%%eax; \
            movl %%eax,%%cr3; \
            \
            movl %%cr0,%%eax; \
            or $80000000,%%eax; \
            movl %%eax,%%cr0; \
            "::"m"(page_table_l1):"eax");
}

#endif
