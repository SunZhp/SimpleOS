;从硬盘控制器中获取硬盘数据
section .code vstart=0x7c00

SELECTOR_CODE equ 0x0008

call .set_protect
mov eax,0x9000
call .readdisk
jmp dword SELECTOR_CODE:0x9000

.readdisk:
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
    .not_ready:
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
    push edx
    mov al,0x2
    mov dx,0x1f3
    out dx,al
    mov al,0x0
    mov dx,0x1f4
    out dx,al
    mov al,0x0
    mov dx,0x1f5
    out dx,al
    mov al,0x07
    mov dx,0x1f6
    out dx,al
    pop edx
    pop eax
    ret

;设置读取磁盘块的数量,默认10
.setcount:
    push eax
    push edx
    mov al,0x100
    mov dx,0x1f2
    out dx,al
    pop edx
    pop eax
    ret

;获取读写磁盘时的状态标志
.getstatus:
    push eax
    push edx
    mov dx,0x1f7
    in al,dx
    and al,0x88
    pop edx
    pop eax
    ret

.set_protect:
   ;打开A20
   push eax
   in al,0x92
   or al,0000_0010B
   out 0x92,al

   ;cr0第0位置1
   mov eax, cr0
   or eax, 0x00000001
   mov cr0, eax
   pop eax
   ret

times 510-($-$$) db 0
db 0x55,0xaa
