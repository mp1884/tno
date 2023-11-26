#ifndef __linux__
#error bullshit
#endif

volatile char inp, ans_buf[30] = {[29] = ' '};

signed main() {
    __asm("jmp start\n"
          // Num to print in rax
          /* "       movq $1234567890, %rax\n" */
          "print_num:\n"
          "       leaq ans_buf+28(%rip), %rsi\n"
          "       movq $10, %rbx\n"
          "print_num_loop:\n"
          "       xor %rdx, %rdx\n"
          "       divq %rbx\n"
          "       addq $48, %rdx\n"
          "       movb %dl, (%rsi)\n"
          "       decq %rsi\n"
          "       test %rax, %rax\n"
          "       jnz print_num_loop\n"
          "print_num_loop_end:\n"
          "       leaq ans_buf+29(%rip), %rdx\n"
          "       subq %rsi, %rdx\n"
          "       incq %rsi\n"
          "       movq $1, %rax\n"
          "       movq $1, %rdi\n"
          "       syscall\n"
          "       ret\n"

          "next:\n"
          "       movq $0, %rax\n"
          "       movq $0, %rdi\n"
          "       leaq inp(%rip), %rsi\n"
          "       movq $1, %rdx\n"
          "       syscall\n"
          "       testq %rax, %rax\n"
          "       jz end\n"
          "       movb inp(%rip), %al\n"
          "       ret\n"

          // %r12 for non-empty lines count.
          // %r13 for maximal num count in lines.
          // %r14 for num count in current line.
          "start:\n"
          "       xorq %r12, %r12\n"
          "       xorq %r13, %r13\n"
          "       xorq %r14, %r14\n"
          "loop1:\n"
          "       call next\n"
          "loop2:\n"
          "       cmpb $32, %al\n"
          "       je loop1\n"
          "       cmpb $9, %al\n"
          "       je loop1\n"
          "       cmpb $13, %al\n"
          "       je loop1\n"

          "maybe_newline:\n"
          "       cmpb $10, %al\n"
          "       jne maybe_num\n"
          "       call next\n"
          "       testq %r14, %r14\n"
          "       jz loop2\n"
          "line_not_empty:\n"
          "       incq %r12\n"
          "       cmpq %r13, %r14\n"
          "       jbe r14_upd\n"
          "       movq %r14, %r13\n"
          "r14_upd:\n"
          "       xor %r14, %r14\n"
          "       jmp loop2\n"

          "maybe_num:\n"
          "       cmpb $45, %al\n"
          "       je num_loop\n"
          "       cmpb $48, %al\n"
          "       jb haram\n"
          "       cmpb $57, %al\n"
          "       ja haram\n"
          "num_loop:\n"
          "       call next\n"
          "       cmpb $45, %al\n"
          "       je num_loop\n"
          "       cmpb $48, %al\n"
          "       jb num_loop_end\n"
          "       cmpb $57, %al\n"
          "       jbe num_loop\n"
          "num_loop_end:\n"
          "       incq %r14\n"
          "       jmp loop2\n"

          "haram:\n"
          "       movq $0, 0\n"
          "       jmp haram\n"

          "end:\n"
          "       popq %rax\n"
          "       test %r14, %r14\n"
          "       jz no_extra_line\n"
          "       incq %r12\n"
          "no_extra_line:\n"
          "       movq %r12, %rax\n"
          "       call print_num\n"
          "       movq %r13, %rax\n"
          "       call print_num"
          "");
    return 0x5d0 - 02720;
}
