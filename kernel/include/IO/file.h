#ifndef FILE_H
#define FILE_H

#include <io/file_operations.h>

#define MAX_FILE_OPEN 1024

struct file {
    uint32_t ftype;
    struct file_operations *file_op;
};

int32_t open(int8_t* fpath);
int32_t read(uint32_t fd,int8_t* buffer,uint32_t rsize);
int32_t write(uint32_t fd,int8_t* buffer,uint32_t wsize);
int32_t seek(uint32_t fd,uint32_t off);
int32_t filesetup(uint32_t fd,uint32_t opt,int8_t* buffer);
void close(uint32_t fd);

#endif
