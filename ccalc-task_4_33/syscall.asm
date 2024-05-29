global _start

global errno

%ifdef OS_DARWIN
global _read
global _write
global _exit

extern _main

%else
global read
global write
global exit

extern main
%endif

section .bss
errno resd 1

section .text

_generic_syscall_3:
%ifdef OS_LINUX
	push ebp
	mov ebp, esp
	push ebx

	mov ebx, [ebp+8]
	mov ecx, [ebp+12]
	mov edx, [ebp+16]
	int 80h
	mov edx, eax
	and edx, 0fffff000h
	cmp edx, 0fffff000h
	jnz .ok
	mov [errno], eax 
	mov eax, -1
.ok:	
	pop ebx
	mov esp, ebp
	pop ebp
	ret
%else
	int 80h
	jnc .ok
	mov [errno], eax 
	mov eax, -1
.ok:	ret
%endif


; int read(int fd, char *buf, int count);
%ifdef OS_DARWIN
_read:
%else
read:
%endif
	mov eax, 3
	jmp _generic_syscall_3


; int write(int fd, const char *buf, int count);
%ifdef OS_DARWIN
_write:
%else
write:
%endif
	mov eax, 4
	jmp _generic_syscall_3 

; void exit(int status);
%ifdef OS_DARWIN
_exit:
	mov eax, 1
	int 80h
%else
exit:
	mov eax, 1
	mov ebx, [esp+4]
	int 80h
%endif


_start:
	mov ecx, [esp]
	mov ebx, esp
	add ebx, 4

	push ebx
	push ecx

%ifdef OS_DARWIN
	call _main
%else
	call main
%endif

	push eax
%ifdef OS_DARWIN
	call _exit
%else
	call exit
%endif
