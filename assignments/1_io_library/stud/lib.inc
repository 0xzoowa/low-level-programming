section .data newline: db 0xA char_buffer: db 0 section .text exit: mov rax, 60
xor rdi, rdi syscall string_length: xor rax, rax ;initializes rax to 0 .loop:
cmp byte [rdi + rax], 0 je .end inc rax jmp .loop .end: ret ;return rax
print_string: ;Accepts a pointer to a null-terminated string and prints it to
stdout. mov rsi, rdi xor rcx, rcx ;0 .loop: cmp [rsi + rcx], 0 ;hello\0e je .end
inc rcx jmp .loop .end: jmp .write_to_screen ret .write_to_screen: ;print string
to screen mov rdx, rcx ;count mov rax, 1 ;write mov rdi, 1 ;stdout syscall ret
print_char: ;Accepts a character code directly as its first argument and prints
it to stdout. mov [char_buffer], rdi; mov rsi, char_buffer mov rdx, 1 ;count mov
rax, 1 ;write mov rdi, 1 ;stdout syscall ret print_newline: mov rsi, newline mov
rax, 1 mov rdi, 1 mov rdx, 1 syscall ret print_uint: ;Outputs an unsigned 8-byte
integer in decimal format. ;q - rax -> unprocessed digits ;r - rdx -> processed
digit ;divisor any 64-bit register or memory operand (e.g. rcx) ;decimal 112
;12345/10 -> r 5 q 1234 ;1234/10 -> r 4 q 123 ;123/10 -> r 3 q 12 ;12/10 -> r 2
q 1 ;1/10 -> r 1 q 0 mov rax, rdi ;move dividend (arg) into rax xor rdx, rdx
;zero out high 64 bit mov rcx, 10 ;divisor mov rbx, rsp ;save stack pointer
.loop: div rcx add rdx, '0' ; transform to ASCII push rdx ;push processed digit
onto stack xor rdx, rdx ;clear reminder for new iteration test rax, rax jnz
.loop .print_loop: cmp rbx, rsp je .end pop rdi call print_char ;routine to
print a char jmp .print_loop .end: ret print_int: xor rax, rax ret
string_equals: xor rax, rax ret read_char: xor rax, rax ret read_word: ret ; rdi
points to a string ; returns rax: number, rdx : length parse_uint: xor rax, rax
ret ; rdi points to a string ; returns rax: number, rdx : length parse_int: xor
rax, rax ret string_copy: ret
