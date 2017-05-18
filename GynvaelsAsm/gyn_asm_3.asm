[bits 32]

; int x = getchar()
call [ebx+2*4] ; getchar, load to eax

cmp eax, 'A'
jne else

call print_hello
db 'hello world', 0xa, 0 ; helloworld\n\00

print_hello:
call [ebx+3*4] ; printf
add esp, 4
push 0
call [ebx] ; exit

else:
call print_xyz
db 'xyz', 0xa, 0

print_xyz:
call [ebx+3*4] ; printf
add esp, 4

push 0
call [ebx] ; exit
