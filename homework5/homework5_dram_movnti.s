	.file	"cache.c"
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB499:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movnti	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movnti	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movnti	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movnti	%eax, -4(%rbp)
	movl	-8(%rbp), %eax
	addl	$4, %eax
	movnti	%eax, -8(%rbp)
.L2:
	movl	-8(%rbp), %eax
	cmpl	$9999999, %eax
	jle	.L3
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE499:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
