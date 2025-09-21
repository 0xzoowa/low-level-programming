section .data BUFFER_SIZE EQU 256 newline: db 0xA char_buffer: db 0
uncleaned_input: resb BUFFER_SIZE temp: resb 1 section .text exit: mov rax, 60
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
integer in decimal format. mov rax, rdi ;move dividend (arg) into rax xor rdx,
rdx ;zero out high 64 bit mov rcx, 10 ;divisor mov rbx, rsp ;save stack pointer
.loop: div rcx add rdx, '0' ; transform to ASCII push rdx ;push processed digit
onto stack xor rdx, rdx ;clear reminder for new iteration test rax, rax jnz
.loop ;if rax is 0, done processing digits .print_loop: cmp rbx, rsp je .end pop
rdi call print_char ;routine to print a char jmp .print_loop .end: ret
print_int: ;Output a signed 8-byte integer in decimal format. mov rax, rdi cmp
rax, 0 jge .positive mov rdi, '-' call print_char neg rax .positive: mov rcx, 10
mov rbx, rsp .loop: xor rdx, rdx div rcx add rdx, '0' push rdx test rax, rax jnz
.loop .print_loop: cmp rbx, rsp je .end pop rdi call print_char jmp .print_loop
.end: ret string_equals: ;Accepts two pointers to strings and compares them.
Returns 1 if they are equal, otherwise 0. .loop: cmp byte [rdi], 0 je
.check_pointer2 cmp byte [rsi], 0 je .false mov al, [rdi] cmp al, [rsi] jne
.false inc rdi inc rsi jmp .loop .check_pointer2: cmp byte [rsi], 0 je .true jmp
.false .true: mov rax, 1 ret .false: xor rax, rax ret read_word: ;Accepts a
buffer address and size as arguments. ;Reads next word from stdin (skipping
whitespaces into buffer). ;Stops and returns 0 if word is too big for the buffer
specified; otherwise returns a buffer address. ;This function should
null-terminate the accepted string. mov rcx, 0 mov rbx, rdi ;buffer address mov
r8, rsi ;buffer size ;reads stdin into a temporary buffer call .read cmp rax, 0
je .done xor rdx, rdx mov [uncleaned_input + rax], 0 ;null terminate the
temporary buffer .next_char_from_buffer: mov al, [uncleaned_input + rdx] ;check
for null terminator cmp al, 0 je .done ;skip/ignore whitespaces cmp al, ' ' je
.continue cmp al, 9 je .continue cmp al, 10 je .continue ;check capacity cmp
rcx, r8 jae .too_big ;save to buffer mov [rbx + rcx], al inc rcx jmp
.next_char_from_buffer .read: mov rax, 0 mov rdi, 0 mov rsi, uncleaned_input mov
rdx, r8 ;count -> buffer size syscall ret .too_big: xor rax, rax ret .continue:
inc rdx jmp .next_char_from_buffer .done: mov byte [rbx + rcx], 0 mov rax, rbx
ret read_char: xor rax, rax ret parse_uint: xor rax, rax ret ; rdi points to a
string ; returns rax: number, rdx : length parse_int: xor rax, rax ret
string_copy: ret
