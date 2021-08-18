#include <IO/file_operations.h>

#define MAX_REG_FILEOPT 128

static uint8_t reg_top = 0;

struct reg_file_operations{
    uint32_t reg_type;
    struct file_operations* opt;
};

struct reg_file_operations reg_filetype[MAX_REG_FILEOPT];

void register_fileoperations(uint32_t filetype,struct file_operations* fop){
    uint8_t i = 0, index = reg_top;
    if(reg_top >= MAX_REG_FILEOPT) return;

    for(;i<reg_top;i++){
        if(reg_filetype[i].reg_type == FILE_NONE){
            index = i;
            break;
        }
    }

    reg_filetype[index].reg_type = filetype;
    reg_filetype[index].fop = fop;

    if(index == reg_top) reg_top++;
}

void unregister_fileoperations(uint32_t filetype){
    uint8_t i = 0,index = 0;
    for(;i<reg_top;i++){
        if(reg_filetype[i].reg_type == filetype)
            reg_filetype[i].filetype = FILE_NONE;
    }
}

struct file_operations* find_fileoperations(uint32_t filetype){
    uint8_t i = 0,index = MAX_REG_FILEOPT;
    for(;i<reg_top;i++){
        if(reg_filetype[i].reg_type == filetype)
            index = i;
    }

    if(index < MAX_REG_FILEOPT){
        return reg_filetype[index].opt;
    }
    else {
        return NULL;
    }
}

void fileoperations_init(void){
    vga_init();
}
