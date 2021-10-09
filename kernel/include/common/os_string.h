#ifndef OS_STRING_H
#define OS_STRING_H

#include <type.h>

/*
 *以下函数功能类似于标准库中提供的同名函数，不做具体标注
 */

int32_t strlen(int8_t* string);
uint32_t memcpy(int8_t* dst,int8_t* src,uint32_t len);

//端口读写
void inb(int8_t port,int8_t var);
int8_t outb(int8_t port);

void inw(int8_t port,int16_t var);
int16_t outw(int8_t port);

void inl(int8_t port,int32_t var);
int32_t outl(int8_t port);

#endif
