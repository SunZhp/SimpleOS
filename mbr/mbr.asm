;从硬盘控制器中获取硬盘数据
section .code vstart=0x7c00

mov ax,0x900
call .readdisk
call ax

.readdisk
    push eax
    push edx
    push ecx
    push ebx
    mov bx,ax
    ;设置读取块的数量为10
    call .setlba
    call .setcount
    ;写入读命令
    mov dx,0x1f7
    mov al,0x20
    out dx,al
    .not_ready
    call .getstatus
    cmp al,0x8
    jnz .not_ready
    mov ax,0x0a
    mov cx,256
    mul cx
    .ready_read:
    mov dx,0x1f0
    in ax,dx
    mov [bx],ax
    add bx,2
    loop .ready_read
    .end:
    pop ebx
    pop ecx
    pop edx
    pop eax
    ret


;设置LBA地址，加载第二个块,low,mid,high对应于0x1f3,0x1f4,0x1f5,0x1f6
.setlba:
    push eax
    mov al,0x2
    out 0x1f3,al
    mov al,0x0
    out 0x1f4,al
    mov al,0x0
    out 0x1f5,al
    mov al,0x07
    out 0x1f6,al
    pop eax

;设置读取磁盘块的数量,默认10
.setcount:
    push eax
    mov al,0x0a
    out 0x1f2,al
    pop eax
    ret

;获取读写磁盘时的状态标志
.getstatus:
    push eax
    in al,0x1f7
    and al,0x88
    pop eax
    ret

times 510-($-$$) db 0
db 0x55,0xaa
