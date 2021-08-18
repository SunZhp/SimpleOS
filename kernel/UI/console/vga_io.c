#include <UI/console_vga_io.h>

#define VGA_RAM_START   0xb8000
#define VGA_RAM_END     0xbffff
#define VGA_RAM_BUFFER  (32*1024)

#define VGA_IO_EACHLINE 80

struct console_vga {
    uint8_t bg_color:4;
    uint8_t pg_color:4;
    uint8_t ascii;
};

//用于保存全局配置
struct console_vga_config {
    uint8_t bg_color:4;
    uint8_t pg_color:4;
    uint32_t curpos;
} vga_config = {COLOR_BLACK,COLOR_WHITE|COLOR_I,0};

void vga_write_sigle(uint8_t ascii){
    struct console_vga *cvga = (struct console_vga*)VGA_RAM_START;
    if(cvga + vga_config.curpos >= VGA_RAM_END){
        vga_config.curpos = 0;
    }

    cvga[vga_config.curpos].bg_color = vga_config.bg_color;
    cvga[vga_config.curpos].pg_color = vga_config.pg_color;
    cvga[vga_config.curpos].ascii = ascii;
    vga_config.curpos++;
}

int32_t vga_write(uint8_t *buffer, uint32_t nsize){
    uint32_t i = 0;

    for(; i < nsize; i++){
        if(buffer[i] == '\n'){ //换行
            uint8_t nblank = vga_config.curpos%VGA_IO_EACHLINE;
            for(;nblank < VGA_IO_EACHLINE;nblank++){
                vga_write_sigle(' ');
            }
        }
        else if(buffer[i] == '\t'){ //隔开4个字符
            uint8_t nblank = 
                (vga_config.curpos%VGA_IO_EACHLINE)>4?4:vga_config.curpos%VGA_IO_EACHLINE;
            uint8_t p = 0;
            for(;p < nblank;p++){
                vga_write_sigle(' ');
            }
        }
        else if(buffer[i]>=32 && buffer[i]<=126){ //可显字符
            vga_write_sigle(buffer[i]);
        }
        else {  //按空格处理
            vga_write_sigle(' ');
        }
    }
}

int32_t vga_seek(uint32_t off){
    if(vga_config.curpos+off >= VGA_RAM_BUFFER/sizeof(struct console_vga))
        return -1;
    vga_config.curpos = off;
    return 0;
}

int32_t vga_setup(uint32_opt,uint8_t* buffer){
    switch(opt){
    case FILEOPT_VGA_SETUPCOLOR:
        *(uint8_t*)&vga_config = *buffer;
        break;
    default:
        break;
    }
    return 0;
}

struct file_operations vga_foperation = {
    .open = NULL,
    .close = NULL,
    .read = NULL,
    .write = vga_write,
    .seek = vga_seek,
    .setup = vga_setup
};

void vga_init(void){
    register_fileoperations(FILE_VGA,&vga_foperation);
}
