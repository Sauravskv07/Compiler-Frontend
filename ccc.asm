global  main
extern printf,scanf
section .text
main:
	mov	RBP,	RSP
	sub	RSP,	48
	mov	rbx,	rbp
	add	rbx,	-32
	mov	word [RBX],10
	mov	word [RBX+2],15
Label12:
	mov	CL,	1
Label13:
	mov	rbx,	rbp
	add	rbx,	-15
	mov	[RBX],	CL
Label14:
	mov	R8W,	5
Label15:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	[RBX],	R8W
Label16:
	mov	R9W,	9
Label17:
	mov	rbx,	rbp
	add	rbx,	-12
	mov	[RBX],	R9W
Label18:
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
Label19:
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
Label20:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R10W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R10W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label21:
	mov	R11W,	10
	mov	rbx,	rbp
	add	rbx,	-8
	mov	[RBX],	R11W
Label22:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	R12W,	[rbx]
	mov	R13W,	15
	xor	R14B,	R14B
	CMP	R12W,	R13W
	JG	JRP22
	mov	R14B,	1
JRP22:
Label23:
	CMP	R14B,	0
	JE Label46
	JMP Label24
Label24:
Label27:
	mov	R15W,	7
Label28:
	mov	rbx,	rbp
	add	rbx,	-34
	mov	[RBX],	R15W
Label29:
	mov	rbx,	rbp
	add	rbx,	-36
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
Label30:
	mov	rbx,	rbp
	add	rbx,	-34
	mov	CX,	[rbx]
Label31:
	mov	rbx,	rbp
	add	rbx,	-36
	mov	R8W,	[rbx]
Label32:
	ADD	CX,	R8W
Label33:
	mov	rbx,	rbp
	add	rbx,	-34
	mov	[RBX],	CX
Label34:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-34
	mov	R9W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R9W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label35:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R10W,	[rbx]
Label36:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	R11W,	[rbx]
Label37:
	ADD	R10W,	R11W
Label38:
	mov	rbx,	rbp
	add	rbx,	-34
	mov	R12W,	[rbx]
Label39:
	mov	R13W,	2
Label40:
	IMUL	R12W,	R13W
Label41:
	ADD	R10W,	R12W
Label42:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	[RBX],	R10W
Label43:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R14W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R14W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label44:
	mov	rbx,	rbp
	add	rbx,	-8
	inc	word [RBX]
	JMP	Label22
Label46:
Label47:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R15W,	[rbx]
Label48:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	CX,	[rbx]
Label49:
	mov	rbx,	rbp
	add	rbx,	-12
	mov	R8W,	[rbx]
Label50:
	IMUL	CX,	R8W
Label51:
	ADD	R15W,	CX
Label52:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R9W,	[rbx]
Label53:
	mov	rbx,	rbp
	add	rbx,	-12
	mov	R10W,	[rbx]
Label54:
	SUB	R9W,	R10W
Label55:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R11W,	[rbx]
Label56:
	IMUL	R9W,	R11W
Label57:
	ADD	R15W,	R9W
Label58:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R12W,	[rbx]
Label59:
	mov	R13W,	2
Label60:
	IMUL	R12W,	R13W
Label61:
	ADD	R15W,	R12W
Label62:
	mov	rbx,	rbp
	add	rbx,	-12
	mov	R14W,	[rbx]
Label63:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	CX,	[rbx]
Label64:
	IMUL	R14W,	CX
Label65:
	SUB	R15W,	R14W
Label66:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	[RBX],	R15W
Label67:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R8W,	[rbx]
Label68:
	mov	R9W,	10
Label69:
	xor	R10B,	R10B
	CMP	R8W,	R9W
	JLE	JRP69
	mov	R10B,	1
JRP69:
Label70:
	CMP	R10B,	0
	JE Label71
	JMP Label75
Label71:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R11W,	[rbx]
Label72:
	mov	rbx,	rbp
	add	rbx,	-12
	mov	R12W,	[rbx]
Label73:
	xor	R13B,	R13B
	CMP	R11W,	R12W
	JG	JRP73
	mov	R13B,	1
JRP73:
	JMP	Label76
Label75:
	mov	R13B,	R10B
Label76:
	OR	R10B,	R13B
Label77:
	CMP	R10B,	0
	JE Label82
	JMP Label78
Label78:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R14W,	[rbx]
Label79:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R15W,	[rbx]
Label80:
	xor	CL,	CL
	CMP	R14W,	R15W
	JGE	JRP80
	mov	CL,	1
JRP80:
	JMP	Label83
Label82:
	mov	CL,	R10B
Label83:
	AND	R10B,	CL
Label84:
	CMP	R10B,	0
	JE Label87
	JMP Label85
Label85:
	mov	rbx,	rbp
	add	rbx,	-15
	mov	R8B,	[rbx]
	JMP	Label88
Label87:
	mov	R8B,	R10B
Label88:
	AND	R10B,	R8B
Label89:
	mov	rbx,	rbp
	add	rbx,	-16
	mov	[RBX],	R10B
Label90:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R9W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R9W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label91:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-15
	mov	R10B,	[rbx]
	CMP	R10B,	0
	JE PLF91
	mov	rdi,	BoolTrue
	JMP	PLT91
PLF91:
	mov	rdi,	BoolFalse
PLT91:
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label92:
	mov	rbx,	rbp
	add	rbx,	-32
	mov	R11W,	5
	inc	R11W
	add	rbx,	4
	mov	rdi,	IntArrInputString
	movsx	rsi,	R11W
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
PUSH	R11W
	call printf
POP	R11W
PUSH	RBP
PUSH	RBX
	ILA92:
	mov	rdi,	IntInput
	mov	rsi,intvar	
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
PUSH	R11W
	call scanf
POP	R11W
PUSH	RBP
PUSH	RBX
	mov	ax,	[intvar]
	mov	[RBX],	ax
	add	rbx,2
	dec	R11W
	jnz ILA92
Label93:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R12W,	[rbx]
Label94:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R13W,	[rbx]
Label95:
	ADD	R12W,	R13W
Label96:
	mov	R14W,	13
	mov	rbx,	rbp
	add	rbx,	-32
	sub	R14W,	10
	add	rbx,	4
	movsx	RSI,	R14W
	mov	R15W,	[RBX+2*RSI]
Label97:
	mov	CX,	2
Label98:
	IMUL	R15W,	CX
Label99:
	ADD	R12W,	R15W
Label100:
	mov	R8W,	14
	mov	rbx,	rbp
	add	rbx,	-32
	sub	R8W,	10
	add	rbx,	4
	movsx	RSI,	R8W
	mov	R9W,	[RBX+2*RSI]
Label101:
	mov	R10W,	3
Label102:
	IMUL	R9W,	R10W
Label103:
	ADD	R12W,	R9W
Label104:
	mov	R11W,	15
	mov	rbx,	rbp
	add	rbx,	-32
	sub	R11W,	10
	add	rbx,	4
	movsx	RSI,	R11W
	mov	R13W,	[RBX+2*RSI]
Label105:
	ADD	R12W,	R13W
Label106:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	[RBX],	R12W
Label107:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R14W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R14W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label108:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-32
	mov	R15,	rbx
	mov	rbx,	R15
	mov	CX,	5
	inc	CX
	add	rbx,	4
	PLA108:
	mov	rdi,	IntOutput
	movsx	rsi,	word [RBX]
	add	rbx,2
	xor	rax,	rax
PUSH	CX
PUSH	RBX
PUSH	RBP
	call	printf
POP	RBP
POP	RBX
POP	CX
	dec	CX
	jnz PLA108
Label109:
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
section .data
ErrorHigh:
	db "Index Greater Than Bound! RUNTIME ERROR! ",10,0
ErrorLow:
	db "Index Smaller Than Bound! RUNTIME ERROR! ",10,0
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
