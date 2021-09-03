#ifndef KERNEL_PAGE_H
#define LERNEL_PAGE_H

#include <type.h>

//页表相和页目录项结构
struct page_entry{
    uint32_t present:1; //存在位
    uint32_t rw:1;      //读写位
    uint32_t us:1;      //普通用户or超级用户
    uint32_t pwt:1;     //页级通写位
    uint32_t pcd:1;     //页级高速缓存禁止位
    uint32_t a:1;       //访问位
    uint32_t d:1;       //脏页位
    uint32_t pat:1;     //页属性表位
    uint32_t g:1;       //全局位
    uint32_t avl:1;     //avalibale位
    uint32_t base:20;   //[页表]物理地址12-31位
};

extern struct page_entry g_pde[1024];//页目录项
extern struct page_entry g_pte[1024];//页表项

void init_page_entry(void);

#endif
