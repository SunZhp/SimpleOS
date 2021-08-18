CROSS_COMPILER=/usr/local/i386elfgcc/bin/i386-elf
CC=$(CROSS_COMPILER)-gcc
LD=$(CROSS_COMPILER)-ld
AR=$(CROSS_COMPILER)-ar
CFLAGS=-m32 -I./include/kernel/ -I./include/bootsec/

mbr_file=./mbr/mbr.asm
mbr_target=mbr.bin
kernel_target=kernel.bin
kernel_obj=./kernel/bootinit/bootsec.o \
		   ./kernel/common/print.o \
		   ./kernel/interrupt/interrupt.o

.PHONY: mbr kernel clean

$(mbr_target):$(mbr_file)
	nasm -f bin -o $@ $^
mbr:$(mbr_target)
	dd if=$^ of=hd.img bs=512 conv=notrunc

$(kernel_target):$(kernel_obj)
	$(LD) -o $@ $(CFLAGS) -Ttext 0x900 --oformat binary $<
kernel:$(kernel_target)
	dd if=$^ of=hd.img bs=512 seek=2 conv=notrunc

all:mbr kernel

clean:
	rm $(mbr_target) $(kernel_target) $(kernel_obj)

