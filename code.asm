global  main
extern printf,scanf
section .text
main:
	mov	RBP,	RSP
	sub	RSP,	64
	mov	rbx,	rbp
	add	rbx,	-28
	mov	word [RBX],6
	mov	word [RBX+2],10
	mov	rbx,	rbp
	add	rbx,	-42
	mov	word [RBX],6
	mov	word [RBX+2],10
	mov	rbx,	rbp
	add	rbx,	-56
	mov	word [RBX],6
	mov	word [RBX+2],10
Label12:
	mov	rbx,	rbp
	add	rbx,	-28
	mov	CX,	4
	inc	CX
	add	rbx,	4
	mov	rdi,	IntArrInputString
	movsx	rsi,	CX
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
PUSH	CX
	call printf
POP	CX
PUSH	RBP
PUSH	RBX
	ILA12:
	mov	rdi,	IntInput
	mov	rsi,intvar	
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
PUSH	CX
	call scanf
POP	CX
PUSH	RBP
PUSH	RBX
	mov	ax,	[intvar]
	mov	[RBX],	ax
	add	rbx,2
	dec	CX
	jnz ILA12
Label13:
	mov	rbx,	rbp
	add	rbx,	-42
	mov	R8W,	4
	inc	R8W
	add	rbx,	4
	mov	rdi,	IntArrInputString
	movsx	rsi,	R8W
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
PUSH	R8W
	call printf
POP	R8W
PUSH	RBP
PUSH	RBX
	ILA13:
	mov	rdi,	IntInput
	mov	rsi,intvar	
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
PUSH	R8W
	call scanf
POP	R8W
PUSH	RBP
PUSH	RBX
	mov	ax,	[intvar]
	mov	[RBX],	ax
	add	rbx,2
	dec	R8W
	jnz ILA13
Label14:
	mov	R9W,	7
Label15:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	[RBX],	R9W
Label16:
	mov	R10W,	1
	mov	rbx,	rbp
	add	rbx,	-8
	mov	[RBX],	R10W
Label17:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	R11W,	[rbx]
	mov	R12W,	3
	xor	R13B,	R13B
	CMP	R11W,	R12W
	JG	JRP17
	mov	R13B,	1
JRP17:
Label18:
	CMP	R13B,	0
	JE Label39
	JMP Label19
Label19:
Label20:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	rdi,	IntInputString
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
	call printf
POP	RBP
POP	RBX
	mov	rdi,	IntInput
	mov	rsi,	intvar	
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
	call	scanf
POP	RBP
POP	RBX
	mov	ax,	[intvar]
	mov	[RBX],	ax
Label21:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	rdi,	IntInputString
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
	call printf
POP	RBP
POP	RBX
	mov	rdi,	IntInput
	mov	rsi,	intvar	
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
	call	scanf
POP	RBP
POP	RBX
	mov	ax,	[intvar]
	mov	[RBX],	ax
Label22:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R14W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-28
	sub	R14W,	6
	add	rbx,	4
	movsx	RSI,	R14W
	mov	R15W,	[RBX+2*RSI]
Label23:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	CX,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-42
	sub	CX,	6
	add	rbx,	4
	movsx	RSI,	CX
	mov	R8W,	[RBX+2*RSI]
Label24:
	ADD	R15W,	R8W
Label25:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	[RBX],	R15W
Label26:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R9W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-28
	sub	R9W,	6
	add	rbx,	4
	movsx	RSI,	R9W
	mov	R10W,	[RBX+2*RSI]
Label27:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R11W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-42
	sub	R11W,	6
	add	rbx,	4
	movsx	RSI,	R11W
	mov	R12W,	[RBX+2*RSI]
Label28:
	ADD	R10W,	R12W
Label29:
	mov	rbx,	rbp
	add	rbx,	-12
	mov	[RBX],	R10W
Label30:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R13W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-28
	sub	R13W,	6
	add	rbx,	4
	movsx	RSI,	R13W
	mov	R14W,	[RBX+2*RSI]
Label31:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R15W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-42
	sub	R15W,	6
	add	rbx,	4
	movsx	RSI,	R15W
	mov	CX,	[RBX+2*RSI]
Label32:
	ADD	R14W,	CX
Label33:
	mov	rbx,	rbp
	add	rbx,	-14
	mov	[RBX],	R14W
Label34:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R8W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R8W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label35:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-12
	mov	R9W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R9W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label36:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-14
	mov	R10W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R10W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label37:
	mov	rbx,	rbp
	add	rbx,	-8
	inc	word [RBX]
	JMP	Label17
Label39:
Label40:
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
section .data
Output:
	db "Output =",10,0
BoolTrue:
	db "true",10,0
BoolFalse:
	db "false",10,0
IntOutput:
	db "%hd",10,0
IntInput:
	db "%hd",0,
IntInputString:
	db "Please Enter an Integer",10,0
BoolInputString:
	db "Please Enter 0 for false and 1 for true",10,0
IntArrInputString:
	db "Please Enter an %d Integers",10,0 
BoolArrInputString:
	db "Please Enter %d 0s or 1s for false and true",10,0
section .bss
intvar: resw 1
