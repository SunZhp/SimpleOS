#ifndef CONSOLE_VGA_IO_H
#define CONSOLE_VGA_IO_H

//用于控制背景色闪烁
#define COLOR_K     0x8
//用于控制前景色亮度
#define COLOR_I     0x8
#define COLOR_RED       0x4
#define COLOR_GREEN     0x2
#define COLOR_BLUE      0x1
#define COLOR_BLACK     0x0
#define COLOR_WHITE     (COLOR_RED|COLOR_GREEN|COLOR_BLUE)

#define FILEOPT_VGA_SETUPCOLOR 0x01

void vga_init(void);

#endif
