#ifndef TYPE_H
#define TYPE_H

#ifdef X86_64 
#define int64_t int
#define uint64_t unsigned int
#define ADDR uint64_t
#else
#define int32_t int
#define uint32_t unsigned int
#define ADDR uint32_t
#endif

#define uint8_t unsigned char
#define int8_t char
#define uint16_t unsigned short
#define int16_t short

#endif
