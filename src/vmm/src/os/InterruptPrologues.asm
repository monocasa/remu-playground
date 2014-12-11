[BITS 64]
[EXTERN _ZN2os4intm13fault_handlerEPNS0_7ContextE]

[GLOBAL breakpoint_prologue]
breakpoint_prologue:
	cli
	push	0
	push	3
	jmp	isr_common_stub

[GLOBAL pagefault_prologue]
pagefault_prologue:
	cli
	push	14
	jmp	isr_common_stub

%macro pushaq 0
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popaq 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

isr_common_stub:
	pushaq

	mov	rdi, rsp

	call	_ZN2os4intm13fault_handlerEPNS0_7ContextE

	popaq

	add	rsp, 16 ; clean error code and interrupt num

	iretq

