#ifndef VGA_BASIC_IO_H
#define VGA_BASIC_IO_H

#include "type.h"

#define stdin 1
#define stdout 0

/*
 *功能类似于标准库提供的printf
 *返回值: <0 调用失败 >=0 打印的字符串长度
 */
int32_t printf(const int8_t* format, ...);

#endif
