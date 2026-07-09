section .data
    fmt    db "%d", 10, 0
    fmtstr db "%s", 10, 0
    str0 db "z is ", 0

section .text
    global _main
    extern _printf

_main:
    push ebp
    mov ebp, esp

    sub esp, 20
    mov eax, 10
    push eax
    mov eax, 0
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    pop ecx
    mov [eax], ecx
    mov eax, 20
    push eax
    mov eax, 1
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    pop ecx
    mov [eax], ecx
    mov eax, 30
    push eax
    mov eax, 2
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    pop ecx
    mov [eax], ecx
    mov eax, 40
    push eax
    mov eax, 3
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    pop ecx
    mov [eax], ecx
    mov eax, 50
    push eax
    mov eax, 4
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    pop ecx
    mov [eax], ecx
    mov eax, 0
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    mov eax, [eax]
    push eax
    push fmt
    call _printf
    add esp, 8
    mov eax, 1
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    mov eax, [eax]
    push eax
    push fmt
    call _printf
    add esp, 8
    mov eax, 2
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    mov eax, [eax]
    push eax
    push fmt
    call _printf
    add esp, 8
    mov eax, 3
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    mov eax, [eax]
    push eax
    push fmt
    call _printf
    add esp, 8
    mov eax, 4
    imul eax, 4
    mov ecx, ebp
    sub ecx, 4
    add ecx, eax
    mov eax, ecx
    mov eax, [eax]
    push eax
    push fmt
    call _printf
    add esp, 8
    sub esp, 20
    sub esp, 4
    mov eax, 0
    mov [ebp - 44], eax
.for0:
    mov eax, [ebp - 44]
    push eax
    mov eax, 5
    mov ecx, eax
    pop eax
    cmp eax, ecx
    mov eax, 0
    setl al
    cmp eax, 0
    je .endfor0
    mov eax, [ebp - 44]
    push eax
    mov eax, 10
    mov ecx, eax
    pop eax
    imul eax, ecx
    push eax
    mov eax, [ebp - 44]
    imul eax, 4
    mov ecx, ebp
    sub ecx, 24
    add ecx, eax
    mov eax, ecx
    pop ecx
    mov [eax], ecx
    mov eax, [ebp - 44]
    push eax
    mov eax, 1
    mov ecx, eax
    pop eax
    add eax, ecx
    mov [ebp - 44], eax
    jmp .for0
.endfor0:
    sub esp, 4
    mov eax, 0
    mov [ebp - 48], eax
.for1:
    mov eax, [ebp - 48]
    push eax
    mov eax, 5
    mov ecx, eax
    pop eax
    cmp eax, ecx
    mov eax, 0
    setl al
    cmp eax, 0
    je .endfor1
    mov eax, [ebp - 48]
    imul eax, 4
    mov ecx, ebp
    sub ecx, 24
    add ecx, eax
    mov eax, ecx
    mov eax, [eax]
    push eax
    push fmt
    call _printf
    add esp, 8
    mov eax, [ebp - 48]
    push eax
    mov eax, 1
    mov ecx, eax
    pop eax
    add eax, ecx
    mov [ebp - 48], eax
    jmp .for1
.endfor1:
    sub esp, 4
    mov eax, 100
    mov [ebp - 52], eax
    push str0
    push fmtstr
    call _printf
    add esp, 8
    mov eax, [ebp - 52]
    push eax
    push fmt
    call _printf
    add esp, 8

    mov eax, 0
    mov esp, ebp
    pop ebp
    ret
