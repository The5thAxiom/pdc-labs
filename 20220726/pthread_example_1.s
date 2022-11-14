	.file	"pthread_example_1.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"Thread %d: top of stack near %p; argv_string=%s\n"
.LC1:
	.string	"strdup"
	.text
	.type	thread_start, @function
thread_start:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-40(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	16(%rax), %rcx
	movq	-24(%rbp), %rax
	movl	8(%rax), %eax
	leaq	-32(%rbp), %rdx
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L2
	leaq	.LC1(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L2:
	movq	-16(%rbp), %rax
	movq	%rax, -32(%rbp)
	jmp	.L3
.L4:
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	toupper@PLT
	movl	%eax, %edx
	movq	-32(%rbp), %rax
	movb	%dl, (%rax)
	movq	-32(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -32(%rbp)
.L3:
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L4
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L6
	call	__stack_chk_fail@PLT
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	thread_start, .-thread_start
	.section	.rodata
	.align 8
.LC2:
	.string	"Usage: %s [-s stack-size] arg...\n"
.LC3:
	.string	"s:"
.LC4:
	.string	"pthread_attr_init"
.LC5:
	.string	"pthread_attr_setstacksize"
.LC6:
	.string	"calloc"
.LC7:
	.string	"pthread_create"
.LC8:
	.string	"pthread_attr_destroy"
.LC9:
	.string	"pthread_join"
	.align 8
.LC10:
	.string	"Joined with thread %d; returned value was %s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movl	%edi, -116(%rbp)
	movq	%rsi, -128(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$-1, -96(%rbp)
	jmp	.L8
.L10:
	cmpl	$115, -92(%rbp)
	jne	.L9
	movq	optarg(%rip), %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	strtoul@PLT
	movl	%eax, -96(%rbp)
	jmp	.L8
.L9:
	movq	-128(%rbp), %rax
	movq	(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC2(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L8:
	movq	-128(%rbp), %rcx
	movl	-116(%rbp), %eax
	leaq	.LC3(%rip), %rdx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	getopt@PLT
	movl	%eax, -92(%rbp)
	cmpl	$-1, -92(%rbp)
	jne	.L10
	movl	optind(%rip), %eax
	movl	-116(%rbp), %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -88(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_attr_init@PLT
	movl	%eax, -84(%rbp)
	cmpl	$0, -84(%rbp)
	je	.L11
	call	__errno_location@PLT
	movl	-84(%rbp), %edx
	movl	%edx, (%rax)
	leaq	.LC4(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L11:
	cmpl	$0, -96(%rbp)
	jle	.L12
	movl	-96(%rbp), %eax
	movslq	%eax, %rdx
	leaq	-64(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pthread_attr_setstacksize@PLT
	movl	%eax, -84(%rbp)
	cmpl	$0, -84(%rbp)
	je	.L12
	call	__errno_location@PLT
	movl	-84(%rbp), %edx
	movl	%edx, (%rax)
	leaq	.LC5(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L12:
	movl	-88(%rbp), %eax
	cltq
	movl	$24, %esi
	movq	%rax, %rdi
	call	calloc@PLT
	movq	%rax, -72(%rbp)
	cmpq	$0, -72(%rbp)
	jne	.L13
	leaq	.LC6(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L13:
	movl	$0, -100(%rbp)
	jmp	.L14
.L16:
	movl	-100(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movl	-100(%rbp), %edx
	addl	$1, %edx
	movl	%edx, 8(%rax)
	movl	optind(%rip), %edx
	movl	-100(%rbp), %eax
	addl	%edx, %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-128(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movl	-100(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rax, %rdx
	movq	(%rcx), %rax
	movq	%rax, 16(%rdx)
	movl	-100(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-72(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movl	-100(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	leaq	-64(%rbp), %rax
	leaq	thread_start(%rip), %rdx
	movq	%rax, %rsi
	call	pthread_create@PLT
	movl	%eax, -84(%rbp)
	cmpl	$0, -84(%rbp)
	je	.L15
	call	__errno_location@PLT
	movl	-84(%rbp), %edx
	movl	%edx, (%rax)
	leaq	.LC7(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L15:
	addl	$1, -100(%rbp)
.L14:
	movl	-100(%rbp), %eax
	cmpl	-88(%rbp), %eax
	jl	.L16
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_attr_destroy@PLT
	movl	%eax, -84(%rbp)
	cmpl	$0, -84(%rbp)
	je	.L17
	call	__errno_location@PLT
	movl	-84(%rbp), %edx
	movl	%edx, (%rax)
	leaq	.LC8(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L17:
	movl	$0, -100(%rbp)
	jmp	.L18
.L20:
	movl	-100(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	leaq	-80(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	%eax, -84(%rbp)
	cmpl	$0, -84(%rbp)
	je	.L19
	call	__errno_location@PLT
	movl	-84(%rbp), %edx
	movl	%edx, (%rax)
	leaq	.LC9(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L19:
	movq	-80(%rbp), %rcx
	movl	-100(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movl	8(%rax), %eax
	movq	%rcx, %rdx
	movl	%eax, %esi
	leaq	.LC10(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	addl	$1, -100(%rbp)
.L18:
	movl	-100(%rbp), %eax
	cmpl	-88(%rbp), %eax
	jl	.L20
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
