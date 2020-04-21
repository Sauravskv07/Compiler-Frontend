global  main
extern printf,scanf
section .text
main:
	mov	RBP,	RSP
	sub	RSP,	56
Label11:
	mov	rbx,	rbp
	add	rbx,	-16
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
Label12:
	mov	rbx,	rbp
	add	rbx,	-18
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
;dynamic array declaration
	mov	rbx,	rbp
	add	rbx,	-16
	mov	CX,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-18
	mov	R8W,	[rbx]
	xor	R9,	R9
	mov	R9W,	R8W
	sub	R9W,	CX
	inc	R9W
	add	R9W,	R9W
	sub	rsp,	R9
	push	R8W
	push	CX
	mov	[rbp-26],	rsp
;dynamic array declaration
	mov	rbx,	rbp
	add	rbx,	-16
	mov	R10W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-18
	mov	R11W,	[rbx]
	xor	R12,	R12
	mov	R12W,	R11W
	sub	R12W,	R10W
	inc	R12W
	add	R12W,	R12W
	sub	rsp,	R12
	push	R11W
	push	R10W
	mov	[rbp-34],	rsp
;dynamic array declaration
	mov	rbx,	rbp
	add	rbx,	-16
	mov	R13W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-18
	mov	R14W,	[rbx]
	xor	R15,	R15
	mov	R15W,	R14W
	sub	R15W,	R13W
	inc	R15W
	add	R15W,	R15W
	sub	rsp,	R15
	push	R14W
	push	R13W
	mov	[rbp-42],	rsp
Label16:
	mov	rbx,	rbp
	add	rbx,	-26
	mov	rbx,	[rbx]
	mov	CX,	[RBX]
	mov	R8W,	[RBX+2]
	sub	R8W,	CX
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
	ILA16:
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
	jnz ILA16
Label17:
	mov	rbx,	rbp
	add	rbx,	-34
	mov	rbx,	[rbx]
	mov	R9W,	[RBX]
	mov	R10W,	[RBX+2]
	sub	R10W,	R9W
	inc	R10W
	add	rbx,	4
	mov	rdi,	IntArrInputString
	movsx	rsi,	R10W
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
PUSH	R10W
	call printf
POP	R10W
PUSH	RBP
PUSH	RBX
	ILA17:
	mov	rdi,	IntInput
	mov	rsi,intvar	
	xor	rax,	rax
PUSH	RBX
PUSH	RBP
PUSH	R10W
	call scanf
POP	R10W
PUSH	RBP
PUSH	RBX
	mov	ax,	[intvar]
	mov	[RBX],	ax
	add	rbx,2
	dec	R10W
	jnz ILA17
Label18:
	mov	R11W,	7
Label19:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	[RBX],	R11W
Label20:
	mov	R12W,	1
	mov	rbx,	rbp
	add	rbx,	-8
	mov	[RBX],	R12W
Label21:
	mov	rbx,	rbp
	add	rbx,	-8
	mov	R13W,	[rbx]
	mov	R14W,	3
	xor	R15B,	R15B
	CMP	R13W,	R14W
	JG	JRP21
	mov	R15B,	1
JRP21:
Label22:
	CMP	R15B,	0
	JE Label43
	JMP Label23
Label23:
Label24:
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
Label25:
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
Label26:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	CX,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-26
	mov	rbx,	[rbx]
	cmp	CX,	[RBX]
	JGE TESTH26
	mov	rdi,	ErrorLow
	xor	rax,	rax
	PUSH	CX
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	CX
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTH26:
	cmp	CX,	[RBX+2]
	JLE TESTC26
	mov	rdi,	ErrorHigh
	xor	rax,	rax
	PUSH	CX
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	CX
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTC26:
	sub	CX,	[RBX]
	add	rbx,	4
	movsx	RSI,	CX
	mov	R8W,	[RBX+2*RSI]
Label27:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R9W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-34
	mov	rbx,	[rbx]
	cmp	R9W,	[RBX]
	JGE TESTH27
	mov	rdi,	ErrorLow
	xor	rax,	rax
	PUSH	R9W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R9W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTH27:
	cmp	R9W,	[RBX+2]
	JLE TESTC27
	mov	rdi,	ErrorHigh
	xor	rax,	rax
	PUSH	R9W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R9W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTC27:
	sub	R9W,	[RBX]
	add	rbx,	4
	movsx	RSI,	R9W
	mov	R10W,	[RBX+2*RSI]
Label28:
	ADD	R8W,	R10W
Label29:
	mov	rbx,	rbp
	add	rbx,	-10
	mov	[RBX],	R8W
Label30:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R11W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-26
	mov	rbx,	[rbx]
	cmp	R11W,	[RBX]
	JGE TESTH30
	mov	rdi,	ErrorLow
	xor	rax,	rax
	PUSH	R11W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R11W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTH30:
	cmp	R11W,	[RBX+2]
	JLE TESTC30
	mov	rdi,	ErrorHigh
	xor	rax,	rax
	PUSH	R11W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R11W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTC30:
	sub	R11W,	[RBX]
	add	rbx,	4
	movsx	RSI,	R11W
	mov	R12W,	[RBX+2*RSI]
Label31:
	mov	rbx,	rbp
	add	rbx,	-4
	mov	R13W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-34
	mov	rbx,	[rbx]
	cmp	R13W,	[RBX]
	JGE TESTH31
	mov	rdi,	ErrorLow
	xor	rax,	rax
	PUSH	R13W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R13W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTH31:
	cmp	R13W,	[RBX+2]
	JLE TESTC31
	mov	rdi,	ErrorHigh
	xor	rax,	rax
	PUSH	R13W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R13W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTC31:
	sub	R13W,	[RBX]
	add	rbx,	4
	movsx	RSI,	R13W
	mov	R14W,	[RBX+2*RSI]
Label32:
	ADD	R12W,	R14W
Label33:
	mov	rbx,	rbp
	add	rbx,	-12
	mov	[RBX],	R12W
Label34:
	mov	rbx,	rbp
	add	rbx,	-2
	mov	R15W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-26
	mov	rbx,	[rbx]
	cmp	R15W,	[RBX]
	JGE TESTH34
	mov	rdi,	ErrorLow
	xor	rax,	rax
	PUSH	R15W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R15W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTH34:
	cmp	R15W,	[RBX+2]
	JLE TESTC34
	mov	rdi,	ErrorHigh
	xor	rax,	rax
	PUSH	R15W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R15W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTC34:
	sub	R15W,	[RBX]
	add	rbx,	4
	movsx	RSI,	R15W
	mov	CX,	[RBX+2*RSI]
Label35:
	mov	rbx,	rbp
	add	rbx,	-6
	mov	R8W,	[rbx]
	mov	rbx,	rbp
	add	rbx,	-34
	mov	rbx,	[rbx]
	cmp	R8W,	[RBX]
	JGE TESTH35
	mov	rdi,	ErrorLow
	xor	rax,	rax
	PUSH	R8W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R8W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTH35:
	cmp	R8W,	[RBX+2]
	JLE TESTC35
	mov	rdi,	ErrorHigh
	xor	rax,	rax
	PUSH	R8W
	PUSH	RBX
	PUSH	RBP
	call	printf
	POP	RBP
	POP	RBX
	POP	R8W
	mov	rax,	60		; system call for exit
	xor	rdi,	rdi		; exit code 0
	syscall		; invoke operating system to exit
TESTC35:
	sub	R8W,	[RBX]
	add	rbx,	4
	movsx	RSI,	R8W
	mov	R9W,	[RBX+2*RSI]
Label36:
	ADD	CX,	R9W
Label37:
	mov	rbx,	rbp
	add	rbx,	-14
	mov	[RBX],	CX
Label38:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-10
	mov	R10W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R10W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label39:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-12
	mov	R11W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R11W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label40:
	mov	rdi,	Output
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
	mov	rbx,	rbp
	add	rbx,	-14
	mov	R12W,	[rbx]
	mov	rdi,	IntOutput
	movsx	rsi,	R12W
	xor	rax,	rax
PUSH	RBP
	call	printf
POP	RBP
Label41:
	mov	rbx,	rbp
	add	rbx,	-8
	inc	word [RBX]
	JMP	Label21
Label43:
Label44:
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
