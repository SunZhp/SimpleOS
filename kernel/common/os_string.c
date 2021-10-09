#include<os_string.h>

int32_t strlen(int8_t* string){
	uint32_t slen = 0;

	if(!string) return -1;

	while(*(string++)){
		slen++;
	}

	return slen;
}


uint32_t memcpy(int8_t* dst,int8_t* src,uint32_t len){
	int32_t i = 0;

	while((i++) < len){
		dst[i] = src[i];
	}

	return i;
}


void inb(int8_t port,int8_t var){
	asm volatile ("inb bl,al" ::"a"(port),"b"(var));
}


int8_t outb(int8_t port){
	int8_t var;
	asm volatile ("outb al,bl" :"b"(var):"a"(port));
	return var;
}


void inw(int8_t port,int16_t var){
	asm volatile ("inw bx,al" ::"a"(port),"b"(var));
}


int16_t outw(int8_t port){
	int16_t var;
	asm volatile ("outb al,bx" :"b"(var):"a"(port));
	return var;
}


void inl(int8_t port,int32_t var){
	asm volatile ("inl ebx,al" ::"a"(port),"b"(var));
}


int32_t outl(int8_t port){
	int32_t var;
	asm volatile ("outl al,ebx" :"b"(var):"a"(port));
	return var;
}
