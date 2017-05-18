[bits 32]

call next ; instead of next: could do
next: ; ...
;call $+5

pop eax ; eax points to next: address

;mov ebp, eax
;add ebp, data-next ; push "hello world" from data
; or
lea ebp, [eax+(data-next)]

push ebp
call [ebx+3*4] ; printf
add esp, 4

push 0
call [ebx] ; exit(0)

data:
db "hello world", 0xa, 0

