global _start

global errno

%ifdef OS_DARWIN
global _read
global _write
global _exit
global _mmap 
global _munmap 

extern _main

%else
global read
global write
global exit
global mmap 
global munmap 

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
%else
exit:
%endif
%ifdef OS_LINUX
	mov eax, 1
	mov ebx, [esp+4]
	int 80h
%else
	mov eax, 1
	int 80h
%endif

; void mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
%ifdef OS_DARWIN
_mmap:
%else
mmap:
%endif
%ifdef OS_LINUX
	push ebp
	mov ebp, esp

	push ebx
	push esi
	push edi
	push ebp

	mov eax, 192
	mov ebx, [ebp+8]
	mov ecx, [ebp+12]
	mov edx, [ebp+16]
	mov esi, [ebp+20]
	mov edi, [ebp+24]
	mov ebp, [ebp+28]
	int 80h
	mov edx, eax
	and edx, 0fffff000h
	cmp edx, 0fffff000h
	jnz .ok
	mov [errno], eax 
	mov eax, -1
.ok:
	pop ebp
	pop edi
	pop esi
	pop ebx

	mov esp, ebp
	pop ebp
	ret
%else
	mov eax, 197
	int 80h
	jnc .ok
	mov [errno], eax 
	mov eax, -1
.ok:	ret
%endif

; void munmap(void *addr, size_t len);
%ifdef OS_DARWIN
_munmap:
%else
munmap:
%endif
%ifdef OS_LINUX
	push ebp
	mov ebp, esp

	push ebx

	mov eax, 91
	mov ebx, [ebp+8]
	mov ecx, [ebp+12]
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
	mov eax, 73
	int 80h
	jnc .ok
	mov [errno], eax 
	mov eax, -1
.ok:	ret
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
