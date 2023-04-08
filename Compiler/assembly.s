
.globl boo
boo:
pushq %rbp
movq %rsp, %rbp
subq $24, %rsp
movq %rdi, -8(%rbp)
movq %rsi, -16(%rbp)
movq -8(%rbp), %rax
addq $12, %rax
movq %rax, -24(%rbp)
movq -24(%rbp), %rax
movq %rbp, %rsp
popq  %rbp
retq

.globl foo
foo:
pushq %rbp
movq %rsp, %rbp
subq $48, %rsp
movq %rdi, -32(%rbp)
movq %rsi, -40(%rbp)
movq -32(%rbp), %rax
addq $12, %rax
movq %rax, -48(%rbp)
movq $24, %rdi
movq -48(%rbp), %rsi
call boo
movq %rax, -56(%rbp)
movq -56(%rbp), %rax
movq -48(%rbp), %rcx
subq %rcx, %rax
movq %rax, -64(%rbp)
movq -64(%rbp), %rax
movq %rbp, %rsp
popq  %rbp
retq
