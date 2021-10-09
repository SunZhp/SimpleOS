#ifndef KERNEL_GDT_H
#define KERNEL_GDT_H

#include <type.h>

//gdtr寄存器结构定义
struct gdt_reg{
	uint16_t limit; 	//gdt界限
	uint32_t gdt_addr;	//gdt表地址
} __attribute__ ((packed)) ;

//详见网上选择子结构
struct dt_selector{
	uint16_t rpl:2;
	uint16_t ti:1;
	uint16_t index:13;
} __attribute__ ((packed)) ;

//详见网上描述符表结构
struct desc_table {
	uint16_t seg_limit;
	uint16_t seg_base_l;
	uint8_t seg_base_m_1;
	uint8_t type:4;
	uint8_t S:1;
	uint8_t dpl:2;
	uint8_t P:1;
	uint8_t seg_base_m_0:4;
	uint8_t avl:1;
	uint8_t L:1;
	uint8_t D_B:1;
	uint8_t G:1;
	uint8_t seg_base_h;
} __attribute__ ((packed)) ;

//初始化全局描述符表项
struct desc_table cs_gdt = {
	.seg_limit 		= 0xffff;
	.seg_base_l 	= 0;
	.seg_base_m_l	= 0;
	.type			= 0xf;
	.S				= 1;
	.dpl			= 0;
	.P				= 1;
	.seg_base_m_0	= 0;
	.avl			= 0;
	.L				= 0;
	.D_B			= 1;
	.G				= 1;
	.seg_base_h 	= 0;
};

//初始化全局描述符表项
struct desc_table ds_gdt = {
	.seg_limit 		= 0xffff;
	.seg_base_l 	= 0;
	.seg_base_m_l	= 0;
	.type			= 0xf;
	.S				= 1;
	.dpl			= 0;
	.P				= 1;
	.seg_base_m_0	= 0;
	.avl			= 0;
	.L				= 0;
	.D_B			= 0;
	.G				= 1;
	.seg_base_h 	= 0;
};

struct dt_selector cs_sel = {
	.rpl 	= 0;
	.ti  	= 0;
	.index 	= 1;
};

struct dt_selector ds_sel = {
	.rpl 	= 0;
	.ti  	= 0;
	.index 	= 2;
};

//初始化全局描述符表
struct desc_table gdt[] = {
	{0}, //第0项为空
	cs_gdt, //代码段
	ds_gdt  //数据段
};

void init_gdt(void);
void flush_executionflow(void);

#endif
