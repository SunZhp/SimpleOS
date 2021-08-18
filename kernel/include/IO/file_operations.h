#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <type.h>

#define FILE_NONE 0x0000
#define FILE_VGA 0x0001

#define FILE_DEV(x) FILE_##x


typedef uint32_t (*f_open)(int8_t* fpath);
typedef void (*f_close)(uint32_t );
typedef uint32_t (*f_read)(int8_t* buffer,int rsize);
typedef uint32_t (*f_write)(int8_t* buffer,int wsize);
typedef uint32_t (*f_seek)(uint32_t off);
typedef uint32_t (*f_setup)(uint32_t opt,uint8_t* buffer);

/*
 * 注册文件基本操作
 * open: 打开文件
 * close: 关闭文件
 * read: 读取rsize个字符到buffer中
 * write: 写入wsize个字符到文件中
 * fseek: 从文件起始处偏移off个字节
 * f_setup: 设置基本属性
 */
struct file_operations{
    f_open open;
    f_close close;
    f_read read;
    f_write write;
    f_seek seek;
    f_setup setup;
};

//注册新的文件类型
void register_fileoperations(uint32_t filetype,struct file_operations* fop);
//卸载文件类型
void unregister_fileoperations(uint32_t filetype);
//根据文件类型查找对应的file_operations
struct file_operations* find_fileoperations(uint32_t filetype);

void fileoperations_init(void);

#endif
