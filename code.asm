global  main
extern printf,scanf
section .text
Label1:
	mov	RBP,	RSP
	sub	RSP,	16
Label6:
	mov	rbx,	rbp
	add	rbx,	-6
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
Label7:
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
Label8:
	mov	rbx,	rbp
	add	rbx,	16
	mov	CX,	[rbx]
Label9:
	mov	rbx,	rbp
	add	rbx,	18
	mov	R8W,	[rbx]
Label10:
	IMUL	CX,	R8W
Label11:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R9W,	[rbx]
Label12:
	mov	R10W,	2
Label13:
	IMUL	R9W,	R10W
Label14:
	ADD	CX,	R9W
Label15:
	mov	R11W,	3
Label16:
	SUB	CX,	R11W
Label17:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	[RBX],	CX
Label18:
	mov	rbx,	rbp
	add	rbx,	20
	mov	R12B,	[rbx]
Label19:
	CMP	R12B,	0
	JE Label24
	JMP Label20
Label20:
	mov	rbx,	rbp
	add	rbx,	16
	mov	R13W,	[rbx]
Label21:
	mov	rbx,	rbp
	add	rbx,	18
	mov	R14W,	[rbx]
Label22:
	xor	R15B,	R15B
	CMP	R13W,	R14W
	JG	JRP22
	mov	R15B,	1
JRP22:
	JMP	Label25
Label24:
	mov	R15B,	R12B
Label25:
	AND	R12B,	R15B
Label26:
	mov	rbx,	rbp
	add	rbx,	-7
	mov	[RBX],	R12B
Label27:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	CX,	[rbx]
Label28:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R8W,	[rbx]
Label29:
	ADD	CX,	R8W
Label30:
	mov	rbx,	rbp
	add	rbx,	21
	mov	[RBX],	CX
Label31:
	mov	rbx,	rbp
	add	rbx,	-7
	mov	R9B,	[rbx]
Label32:
	CMP	R9B,	0
	JE Label33
	JMP Label37
Label33:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R10W,	[rbx]
Label34:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R11W,	[rbx]
Label35:
	xor	R12B,	R12B
	CMP	R10W,	R11W
	JG	JRP35
	mov	R12B,	1
JRP35:
	JMP	Label38
Label37:
	mov	R12B,	R9B
Label38:
	OR	R9B,	R12B
Label39:
	mov	rbx,	rbp
	add	rbx,	23
	mov	[RBX],	R9B
Label40:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	16
	mov	R13W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R13W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label41:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	18
	mov	R14W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R14W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label42:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	20
	mov	R15B,	[rbx]
	CMP	R15B,	0
	JE PLF42
	mov	rdi,	BoolTrue
	JMP	PLT42
PLF42:
	mov	rdi,	BoolFalse
PLT42:
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label43:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-6
	mov	CX,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	CX
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label44:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R8W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R8W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label45:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R9W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R9W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label46:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	21
	mov	R10W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R10W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label47:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	23
	mov	R11B,	[rbx]
	CMP	R11B,	0
	JE PLF47
	mov	rdi,	BoolTrue
	JMP	PLT47
PLF47:
	mov	rdi,	BoolFalse
PLT47:
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label48:
	add	RSP,	16
	ret
main:
	mov	RBP,	RSP
	sub	RSP,	16
Label53:
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
Label54:
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
Label57:
	mov	R12B,	0
Label58:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	[RBX],	R12B
Label59:
	mov	rbx,	rbp
	add	rbx,	-7
Label60:
	mov	rbx,	rbp
	add	rbx,	-6
	PUSH	word [rbx]
Label61:
	mov	rbx,	rbp
	add	rbx,	-8
Label62:
	mov	rbx,	rbp
	add	rbx,	-4
	PUSH	word [rbx]
Label63:
	mov	rbx,	rbp
	add	rbx,	-2
	PUSH	word [rbx]
Label64:
	PUSH	RBP
	call	Label1
	POP	RBP
Label65:
	add	rsp,	2
Label66:
	add	rsp,	2
Label67:
inc	rsp
Label68:
	mov	rbx,	rbp
	add	rbx,	-6
	POP	word [rbx]
Label69:
	mov	rbx,	rsp
	mov	AL,	byte [rbx]
	mov	rbx,	rbp
	add	rbx,	-7
	mov	[rbx],	Al
inc	rsp
Label70:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R13W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R13W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label71:
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
Label72:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R15W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R15W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label73:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-7
	mov	CL,	[rbx]
	CMP	CL,	0
	JE PLF73
	mov	rdi,	BoolTrue
	JMP	PLT73
PLF73:
	mov	rdi,	BoolFalse
PLT73:
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label74:
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
