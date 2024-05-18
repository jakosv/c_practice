global _start

global _errno
global _read
global _write

extern _main

section .bss
_errno resd 1

section .text

_generic_syscall_3:
	int 80h
	jnc .ok
	mov [_errno], eax 
	mov eax, -1
.ok:	ret

_read:
	mov eax, 3
	jmp _generic_syscall_3

_write:
	mov eax, 4
	jmp _generic_syscall_3 

_start:
	mov ecx, [esp]
	mov ebx, esp
	add ebx, 4

	push ebx
	push ecx

	call _main

	; exit
	push eax
	push eax
	mov eax, 1
	int 80h
