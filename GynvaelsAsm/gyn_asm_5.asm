[bits 32]

int3 ; debugger bp

mov eax, 0x12345678
xor eax, 0x78563412

; print("%.8x\n", eax)
push eax
call print
db "%.8x", 0xa, 0
print:
call [ebx+3*4] ; printf
add esp, 8

push 0
call [ebx] ; exit
