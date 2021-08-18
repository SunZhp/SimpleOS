#include <file.h>

static struct file files[MAX_FILE_OPEN];

static int32_t find_filedesc_i(){
    int32_t i = 0;
    for(;i<MAX_FILE_OPEN;i++){
        if(files[i].ftype == FILE_NONE){
            return i;
        }
    }
    return -1;
}


int32_t open(int8_t* fpath){
    int32_t fd = -1;
    uint32_t ftype = FILE_NONE;
    if(fpath[0] == ':'){
        ftype = FILE_DEV(fpath+1);
    }
    else {
        //根据挂载点找到对应文件系统
    }

    fd = find_filedesc_i();
    if(fd < 0){
        return -1;
    }

    files[fd].ftype = ftype;
    files[fd].file_op = find_fileoperations(ftype);

    if(files[fd].file_op && files[fd].file_op->open){
        files[fd].file_op->open(fpath);
    }
    return fd;
}

int32_t read(uint32_t fd,int8_t* buffer,uint32_t rsize){
    if(fd >= MAX_FILE_OPEN) return -1;

    struct file rfile=files[fd];

    if(rfile.file_op && rfile.file_op->read){
        return rfile.file_op->read(buffer,rsize);
    }

    return -1;
}


int32_t write(uint32_t fd,int8_t* buffer,uint32_t wsize){
    if(fd >= MAX_FILE_OPEN) return -1;

    struct file wfile=files[fd];

    if(wfile.file_op && wfile.file_op->write){
        return wfile.file_op->write(buffer,rsize);
    }

    return -1;
}

int32_t seek(uint32_t fd,uint32_t off){
    if(fd >= MAX_FILE_OPEN) return -1;

    struct file sfile=files[fd];

    if(sfile.file_op && sfile.file_op->seek){
        return sfile.file_op->seek(off);
    }

    return -1;
}


int32_t filesetup(uint32_t fd,uint32_t opt,int8_t* buffer){
    if(fd >= MAX_FILE_OPEN) return -1;

    struct file sfile=files[fd];

    if(sfile.file_op && sfile.file_op->setup){
        return sfile.file_op->setup(opt,buffer);
    }

    return -1;
}


void close(uint32_t fd){
    if(fd >= MAX_FILE_OPEN) return;

    struct file sfile=files[fd];

    if(sfile.file_op && sfile.file_op->close){
        sfile.file_op->close();
    }

    files[fd].ftype = FILE_NONE;
}
