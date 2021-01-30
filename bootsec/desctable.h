#ifndef __DESC_TABLE_H__
#define __DESC_TABLE_H__

#define SEG_DESC_SYS_TYPE_286_TSS_AVLI 0x1
#define SEG_DESC_SYS_TYPE_286_TSS_BUSY 0x2
#define SEG_DESC_SYS_TYPE_286_GATE_CALL 0x4
#define SEG_DESC_SYS_TYPE_GATE_TASK 0x5
#define SEG_DESC_SYS_TYPE_286_GATE_INTR 0x6
#define SEG_DESC_SYS_TYPE_286_GATE_TRAP 0x7
#define SEG_DESC_SYS_TYPE_386_TSS_AVLI 0x9
#define SEG_DESC_SYS_TYPE_386_TSS_BUSY 0xb
#define SEG_DESC_SYS_TYPE_386_GATE_CALL 0xc
#define SEG_DESC_SYS_TYPE_INTR 0xe
#define SEG_DESC_SYS_TYPE_TRAP 0xf
#define SEG_DESC_NOSYS_TYPE_CODE_X 0x8
#define SEG_DESC_NOSYS_TYPE_CODE_R 0x4
#define SEG_DESC_NOSYS_TYPE_CODE_C 0x2
#define SEG_DESC_NOSYS_TYPE_CODE_A 0x1
#define SEG_DESC_NOSYS_TYPE_DATA_X 0x8
#define SEG_DESC_NOSYS_TYPE_DATA_W 0x4
#define SEG_DESC_NOSYS_TYPE_DATA_E 0x2
#define SEG_DESC_NOSYS_TYPE_DATA_A 0x1

#define LGDT_MAX_SEGMENT_SZ 8192

//段描述符的格式
typedef struct segment_desc{
	short limit_l16;
	short base_l16;
	unsigned int base_m8:8;
	unsigned int type:4;
	unsigned int s:1;
	unsigned int dpl:2;
	unsigned int p:1;
	unsigned int limit_h20:4;
	unsigned int avl:1;
	unsigned int l:1;
	unsigned int db:1;
	unsigned int g:1;
	unsigned int base_h8:8;
}segment_desc_t;

typedef struct lgdt{
	unsigned int base;
	unsigned short limit;
} lgdt_t;

typedef struct selector{
	short rpl:2;
	short ti:1;
	short index:13;
}__attribute__((packed)) selector_t;

//lgdt第一个元素为空,不使用
//定义一个代码段一个数据段
segment_desc_t seg[LGDT_MAX_SEGMENT_SZ] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0x0fff,0,0,SEG_DESC_NOSYS_TYPE_CODE_X,1,0,1,0xf,1,0,1,1,0},
	{0x0fff,0,0,SEG_DESC_NOSYS_TYPE_DATA_W,1,0,1,0xf,1,0,1,1,0}
};

void switchon_a20(); //打开A20总线(开启32位寻址)
void switchon_protection_mode(); //打开保护模式(cr0寄存器的保护模式位置位)
void load_gdt(lgdt_t gdt); //加载gdt
void flush_csseg(selector_t selector); //刷新段寄存器


//0x92端口第1位置位
void switchon_a20(){
	asm volatile (" \
			inb $0x92,%%al;	\
			or $0x2,%%al;	\
			out %%al,$0x92;":::"ax");
}

//cr0寄存器第0位PE位置位
void switchon_protection_mode(){
	asm volatile ("\
			movl %%cr0,%%eax; 		\
			or $0x00000001,%%eax;	\
			movl %%eax,%%cr0;":::"eax");
}

void load_gdt(lgdt_t gdt){
	asm volatile ("lgdt %0"::"m"(gdt):);
}

void flush_csseg(selector_t selector){
	asm volatile (" \
        movw %%cs,%%ax; \
		jmp flush_CS_segment; \
		flush_CS_segment:"::"a"(selector):);
}

#endif
