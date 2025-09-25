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
.false .true: mov rax, 1 ret .false: xor rax, rax ret read_char: ;Read one
character from stdin and return it. If the end of input stream occurs, return 0.
mov rax, 0 ;read syscall number mov rdi, 0 ;stdin mov rsi, temp ;buffer mov rdx,
1 ;count syscall ;execute cmp rax, 0 je .eof movzx rax, byte [temp] ret .eof:
xor rax, rax ret read_word: ;Accepts a buffer address and size as arguments.
;Reads next word from stdin (skipping whitespaces into buffer). ;Stops and
returns 0 if word is too big for the buffer specified; otherwise returns a
buffer address. ;This function should null-terminate the accepted string. xor
rcx, rcx ; counter = 0 mov rbx, rdi ;save buffer mov r8, rsi ;save size .loop:
call read_char cmp rax, 0 je .done cmp al, ' ' je .continue cmp al, 9 je
.continue cmp al, 10 je .continue ;check capacity cmp rcx, r8 jae .too_big mov
[rbx + rcx], al inc rcx jmp .loop .too_big: xor rax, rax ret .continue: cmp rcx,
0 je .loop jmp .done .done: mov byte [rbx + rcx], 0 mov rax, rbx ret
string_copy: ;Accepts a pointer to a string, a pointer to a buffer, and buffer’s
length. ;Copies string to the destination. The destination address is returned
if the string fits the buffer; otherwise zero is returned. ;rdi -> pointer to
string ;rsi -> pointer to buffer ;rdx -> buffer's length xor rcx, rcx ;counter
-> incremented everytime char is added to buffer mov rbx, rsi ;save dest pointer
.loop: cmp byte [rdi], 0 je .end cmp rcx, rdx jae .too_small mov r8b, [rdi] mov
[rsi], r8b inc rcx inc rdi inc rsi jmp .loop .end: mov byte [rsi], 0 ;null
terminate src string mov rax, rbx ret .too_small: xor rax, rax ret parse_uint:
;Accepts a null-terminated string and tries to parse an unsigned number from its
start. ;Returns the number parsed in rax, its characters count in rdx. xor rcx,
rcx ;power counter xor rdx, rdx ;character counter xor rax, rax ;accumulator mov
r8, 1 ;initialize multiplier to 1 mov rbx, rsp ;save stack pointer .loop: mov
r11b, [rdi] cmp r11b, 0 je .accumulator cmp r11b, '0' jb .end cmp r11b, '9' ja
.end sub r11b, '0' ;get number representation movzx r11, r11b push r11 inc rdx
inc rdi jmp .loop .accumulator: cmp rsp, rbx je .end cmp rcx, 0 je
.handle_zero_power .handle_power: pop r10 ;3 call .power ;r8 = 10 imul r10, r8
add rax, r10 inc rcx jmp .accumulator .handle_zero_power: pop r10 add rax, r10
inc rcx jmp .accumulator .power: imul r8, 10 ret .end: ret parse_int: ;Accepts a
null-terminated string and tries to parse a signed number from its start.
;Returns the number parsed in rax; its characters count in rdx (including sign
if any). ;No spaces between sign and digits are allowed. ;'-123\0' xor rax, rax
xor rcx, rcx xor rdx, rdx xor r9, r9 mov r8, 1 mov rbx, rsp .loop: mov r11b,
[rdi] cmp r11b, 0 je .accumulator ;handle sign cmp r11b, '-' je .handle_negative
cmp r11b, '+' je .handle_positive cmp r11b, '0' jb .end cmp r11b, '9' ja .end
sub r11b, '0' movzx r11, r11b push r11 inc rdx inc rdi jmp .loop .accumulator:
cmp rsp, rbx je .end cmp rcx, 0 je .handle_zero_power .handle_power: pop r10
call .power imul r10, r8 add rax, r10 inc rcx jmp .accumulator
.handle_zero_power: pop r10 add rax, r10 inc rcx jmp .accumulator .power: imul
r8, 10 ret .handle_negative: cmp rdx, 0 jne .end ;reject if first char isnt sign
mov r9, 1 ;set sign flag inc rdx inc rdi jmp .loop .handle_positive: cmp rdx, 0
jne .end inc rdx inc rdi jmp .loop .negate_result: neg rax ret .end: cmp r9, 1
je .negate_result ret
