#include <stdarg.h>
#include <os_string.h>
#include <IO/file.h>
#include <UI/console_vga_io.h>

static int32_t _printf(const int8_t* fmt,va_list ap);
static int8_t get_slash_char(int8_t ch);
static int8_t get_percent_char(va_list ap);
static int32_t get_percent_sign_num(va_list ap);
static int32_t get_percent_unsign_num(va_list ap);
static int8_t* get_percent_string(va_list ap);
static int32_t print_percent_varg(int8_t ch,va_list ap);

int32_t printf(const int8_t* fmt, ...){
	va_list ap;
	int32_t wtlen = 0;

	va_start(ap,fmt);
	wtlen = _printf(fmt,ap);
	va_end(ap);
	return wtlen;
}

#define VGA_PRINTF_PERCENT 	0
#define VGA_PRINTF_SLASH	1
#define VGA_PRINTF_NORMAL 	2
static int32_t _printf(const int8_t* fmt,va_list ap);{
	int8_t *step = fmt;
	int8_t prevmark = VGA_PRINTF_NORMAL;
	int8_t out;

	while(*step){
		if(*step == '\'){
			if(prevmark==VGA_PRINTF_NORMAL){
				prevmark = VGA_PRINTF_SLASH;
			}
			else if(prevmark==VGA_PRINTF_SLASH){
				out = '\';
				prevmark = VGA_PRINTF_NORMAL;
			}
			else {
				return -1;
			}
		}
		else if(*step == '%'){
			if(prevmark == VGA_PRINTF_NORMAL){
				prevmark = VGA_PRINTF_PERCENT;
			}
			else if(prevmark == VGA_PRINTF_PERCENT){
				out = '%';
				prevmark = VGA_PRINTF_NORMAL;
			}
			else {
				return -1;
			}
		}
		else {
			if(prevmark == VGA_PRINTF_NORMAL){
				out = *step;
			}
			else if(prevmark == VGA_PRINTF_SLASH){
				out = get_slash_char(*step);
				prevmark = VGA_PRINTF_NORMAL;
			}
			else {
				print_percent_varg(*step,ap);
				prevmark = VGA_PRINTF_NORMAL;
				goto PRINTF_STEP;
			}
		}

		write(stdout,&out,1);
PRINTF_STEP:
		step++;
	}
}

static void print_num(uint32_t u){
	int32_t u1,u2;
	int8_t c;

	do{
		u1 = u % 10;
		u2 = u / 10;

		if(u2 > 10){
			u = u2 + u1;
			c = u2 + 0x30;
			write(stdout,&c,1);
		}
		else {
			u = u1;
			c = u1 + 0x30;
			write(stdout,&c,1);
		}
	}while(u2);
}

static int32_t print_percent_varg(int8_t ch,va_list ap){
	uint32_t u;
	int32_t n;
	int8_t* s;
	int8_t c;

	switch(ch){
	case 'u':
		u = get_percent_unsign_num(ap);
		print_num(u);
		break;
	case 'd':
		n = get_percent_sign_num(ap);
		if(n > 0){
			print_num(n);
		}
		else {
			c = '-';
			write(stdout,&c,1);
			n = n * (-1);
			print_num(n);
		}
		break;
	case 's':
		s = get_percent_string(ap);
		u = strlen(s);
		write(stdout,s,u);
		break;
	case 'c':
		c = get_percent_char(ap);
		write(stdout,&c,1);
		break;
	default:
		break;
	}
}

static int8_t get_slash_char(int8_t ch){
	int8_t finchar = ch;

	switch(ch){
		case 'n':
			finchar = '\n'; break;
		case 't':
			finchar = '\t'; break;
	}
	return finchar;
}

static int8_t get_percent_char(va_list ap){
	return va_arg(ap,int8_t);
}

static int32_t get_percent_sign_num(va_list ap){
	return va_arg(ap,int32_t);
}

static uint32_t get_percent_unsign_num(va_list ap){
	return va_arg(ap,uint32_t);
}

static int8_t* get_percent_string(va_list ap){
	return va_arg(ap,int8_t*);
}
