[bits 32]

push 'H'
call [ebx+4] ; putchar
add esp, 4

push 0
call [ebx] ; exit

