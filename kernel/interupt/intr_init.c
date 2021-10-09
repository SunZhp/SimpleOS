#include <interupt/intr_init.h>
#include <common/os_string.h>

struct idt_desc idt[IDT_CNT];

INTR(0,NULL,INTR_ZERO);			//DE Divide Error
INTR(1,NULL,INTR_ZERO);			//DB Debug Exception
INTR(2,NULL,INTR_ZERO);			//NMI Interrupt
INTR(3,NULL,INTR_ZERO);			//BP Breakpoint Except on
INTR(4,NULL,INTR_ZERO);			//OF Overflow Except on
INTR(5,NULL,INTR_ZERO);			//BR BOUND Range Exceeded Exception
INTR(6,NULL,INTR_ZERO);			//UD Invalid Opcode Exception
INTR(7,NULL,INTR_ZERO);			//NM Device Not Available Exception
INTR(8,NULL,INTR_ZERO);			//DF Double Fault Exception
INTR(9,NULL,INTR_ZERO);			//Coprocessor Segment Overrun
INTR(10,NULL,INTR_ZERO);		//TS nvalid TSS Exception
INTR(11,NULL,INTR_ZERO);		//NP Segment Not Present
INTR(12,NULL,INTR_ZERO);		//SS Stack Fault Exception
INTR(13,NULL,INTR_ZERO);		//GP General Protection Exception
INTR(14,NULL,INTR_ZERO);		//PF Page-Fault Exception
INTR(15,NULL,INTR_ZERO);		//INTERL reserved
INTR(16,NULL,INTR_ZERO);		//MF x87 FPU F'loating-Point Error
INTR(17,NULL,INTR_ZERO);		//AC Alignment Check Exception
INTR(18,NULL,INTR_ZERO);		//MC Machine-Check Exception
INTR(19,NULL,INTR_ZERO);		//XF SIMD Floating-Point Exception
INTR(20,NULL,INTR_ZERO);
INTR(21,NULL,INTR_ZERO);
INTR(22,NULL,INTR_ZERO);
INTR(23,NULL,INTR_ZERO);
INTR(24,NULL,INTR_ZERO);
INTR(25,NULL,INTR_ZERO);
INTR(26,NULL,INTR_ZERO);
INTR(27,NULL,INTR_ZERO);
INTR(28,NULL,INTR_ZERO);
INTR(29,NULL,INTR_ZERO);
INTR(30,NULL,INTR_ZERO);
INTR(31,NULL,INTR_ERRCODE);

typedef void (*IDTR_FUN)(void);
IDTR_FUN fidt[IDT_CNT] = {
	INTR_ADDR(0),
	INTR_ADDR(1),
	INTR_ADDR(2),
	INTR_ADDR(3),
	INTR_ADDR(4),
	INTR_ADDR(5),
	INTR_ADDR(6),
	INTR_ADDR(7),
	INTR_ADDR(8),
	INTR_ADDR(9),
	INTR_ADDR(10),
	INTR_ADDR(11),
	INTR_ADDR(12),
	INTR_ADDR(13),
	INTR_ADDR(14),
	INTR_ADDR(15),
	INTR_ADDR(16),
	INTR_ADDR(17),
	INTR_ADDR(18),
	INTR_ADDR(19),
	INTR_ADDR(20),
	INTR_ADDR(21),
	INTR_ADDR(22),
	INTR_ADDR(23),
	INTR_ADDR(24),
	INTR_ADDR(25),
	INTR_ADDR(26),
	INTR_ADDR(27),
	INTR_ADDR(28),
	INTR_ADDR(29),
	INTR_ADDR(30),
	INTR_ADDR(31),
	INTR_ADDR(32)
};

#define PIC M CTRL 0x20 
#define PIC M DATA 0x21 
#define PIC S CTRL 0xa0 
#define P工 S DATA 0xa1 

//初始化可编程中断控制器 8259A */ · 
static void pic_init (void) { 
	/*初始化主片*/
	outb(PIC_M_CTRL, 0x11); 
	outb(PIC_M_DATA, 0x20); 
	
	outb(PIC_M_DATA, 0x04) ; 
	outb(PIC_M_DATA, 0x0l); 
	
	/*初始化从片*/
	outb(PIC_S_CTRL, 0x11);
	outb(PIC_S_DATA, 0x28); 
	
	outb(PIC_S_DATA, 0x02); 
	outb(PIC_S_DATA, 0x0l); 
	
	//打开主片上 IR0 ，也就是目前只接受时钟产生的中断
	outb(PIC_M_DATA, 0xfe); 
	outb(PIC_S_DATA, 0xff); 
}


static void make_idt(void){
	uint8_t i = 0;

	for(;i < IDT_CNT; i++){
		idt[i].offset_low = fidt[i] && 0x0000ffff;
		idt[i].selector = cs_gdt;
		idt[i].reserved = 0;
		idt[i].type = 0xe;
		idt[i].s = 0;
		idt[i].dpl = 0;
		idt[i].p = 1;
		idt[i].offset_high = (fidt[i] && 0xffff0000) >> 16;
	}
}


void init_idt(void){
	make_idt();
	pic_init();

	/*加载 idt */
	uint64_t idt_operand = ( (sizeof (idt) - 1) I ( (uint64_t) ( (uint32_t) idt < 16)));
	asm volatile ("lidt %0"::"m"(idt_operand));

	printf("init_idt done...\n");
}
