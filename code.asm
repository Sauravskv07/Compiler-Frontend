global  main
extern printf,scanf
section .text
main:
	mov	RBP,	RSP
	sub	RSP,	24
Label10:
	mov	CL,	1
Label11:
	mov	rbx,	rbp
	add	rbx,	-13
	mov	[RBX],	CL
Label12:
	mov	R8W,	5
Label13:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	[RBX],	R8W
Label14:
	mov	R9W,	9
Label15:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	[RBX],	R9W
Label16:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	rdi,	IntInputString
	xor	rax,	rax
	call printf
	mov	rdi,	IntInput
	mov	rsi,	intvar	
	xor	rax,	rax
	call	scanf
	mov	ax,	[intvar]
	mov	[RBX],	ax
Label17:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	rdi,	IntInputString
	xor	rax,	rax
	call printf
	mov	rdi,	IntInput
	mov	rsi,	intvar	
	xor	rax,	rax
	call	scanf
	mov	ax,	[intvar]
	mov	[RBX],	ax
Label18:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R10W,	[rbx]
Label19:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R11W,	[rbx]
Label20:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R12W,	[rbx]
Label21:
	IMUL	R11W,	R12W
Label22:
	ADD	R10W,	R11W
Label23:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	R13W,	[rbx]
Label24:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R14W,	[rbx]
Label25:
	SUB	R13W,	R14W
Label26:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R15W,	[rbx]
Label27:
	IMUL	R13W,	R15W
Label28:
	ADD	R10W,	R13W
Label29:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	CX,	[rbx]
Label30:
	mov	R8W,	2
Label31:
	IMUL	CX,	R8W
Label32:
	ADD	R10W,	CX
Label33:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R9W,	[rbx]
Label34:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R10W,	[rbx]
Label35:
	IMUL	R9W,	R10W
Label36:
	SUB	R10W,	R9W
Label37:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	[RBX],	R10W
Label38:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R11W,	[rbx]
Label39:
	mov	R12W,	10
Label40:
	CMP	R11W,	R12W
	JLE	JRP40
	mov	R13W,	1
	JRP40:
Label41:
Label42:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	R14W,	[rbx]
Label43:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R15W,	[rbx]
Label44:
	CMP	R14W,	R15W
	JG	JRP44
	mov	CX,	1
	JRP44:
	JMP	Label47
Label46:
Label47:
	OR	R13W,	R13W
Label48:
Label49:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R8W,	[rbx]
Label50:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R9W,	[rbx]
Label51:
	CMP	R8W,	R9W
	JGE	JRP51
	mov	R10W,	1
	JRP51:
	JMP	Label54
Label53:
Label54:
	AND	R13W,	R13W
Label55:
Label56:
	mov	rbx,	rbp
	add	rbx,	-13
	mov	R11B,	[rbx]
	JMP	Label59
Label58:
Label59:
	AND	R13W,	R13W
Label60:
	mov	rbx,	rbp
	add	rbx,	-14
	mov	[RBX],	R13W
Label61:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R12W,	[rbx]
	mov	rdi,	Output
	xor	rax,	rax
	call	printf
	mov	rdi,	IntOutput
	movsx	rsi,	R12W
	xor	rax,	rax
	call	printf
Label62:
	mov	rbx,	rbp
	add	rbx,	-13
	mov	R13B,	[rbx]
	mov	rdi,	Output
	xor	rax,	rax
	call	printf
	CMP	R13B,	0
	JE PLF62
	mov	rdi,	BoolTrue
	JMP	PLT62
PLF62:
	mov	rdi,	BoolFalse
PLT62:
	xor	rax,	rax
	call	printf
Label63:
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
