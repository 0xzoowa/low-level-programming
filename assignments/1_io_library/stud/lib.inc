section .data newline: db 0xA section .text exit: mov rax, 60 xor rdi, rdi
syscall string_length: xor rax, rax ;initializes rax to 0 .loop: cmp byte [rdi +
rax], 0 je .end inc rax jmp .loop .end: ret ;return rax print_string: ;Accepts a
pointer to a null-terminated string and prints it to stdout. mov rsi, rdi xor
rcx, rcx ;0 .loop: cmp [rsi + rcx], 0 ;hello\0e je .end inc rcx jmp .loop .end:
jmp .write_to_screen ret .write_to_screen: ;print string to screen mov rdx, rcx
;count mov rax, 1 ;write mov rdi, 1 ;stdout syscall ret print_char: xor rax, rax
ret print_newline: xor rax, rax ret print_uint: xor rax, rax ret print_int: xor
rax, rax ret string_equals: xor rax, rax ret read_char: xor rax, rax ret
read_word: ret ; rdi points to a string ; returns rax: number, rdx : length
parse_uint: xor rax, rax ret ; rdi points to a string ; returns rax: number, rdx
: length parse_int: xor rax, rax ret string_copy: ret
