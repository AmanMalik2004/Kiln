# MiniC Compiler(ctrl+shift+v to format this file)

A compiler built from scratch in C++ that compiles a custom C-like language (MiniC)
down to real x86 assembly language, which is then assembled and linked into a
native Windows executable.

---

## What is NASM?

NASM (Netwide Assembler) is a program that converts assembly language into machine
code that your CPU can directly execute. 

When our compiler runs, it produces a human-readable `.asm` file full of instructions
like `mov eax, 5` and `add eax, ecx`. Your CPU cannot read that text directly — it
only understands binary machine code (0s and 1s). NASM's job is to translate that
text assembly into actual binary machine code stored in a `.obj` file. Then gcc links
that `.obj` into a final `.exe`.

So the full chain is:
```
MiniC code → (minic.exe) → output.asm → (NASM) → output.obj → (gcc) → output.exe
```

Think of it like this:
- **minic.exe** = your compiler (translates MiniC to assembly text)
- **NASM** = assembler (translates assembly text to machine code)
- **gcc** = linker (connects machine code with system libraries like printf)

---

## MiniC vs C — What is the Difference?

MiniC is inspired by C but is a much simpler language. Here is a comparison:

| Feature | C | MiniC |
|---|---|---|
| Data types | int, float, char, double, struct... | int only |
| String support | Full string manipulation | print only |
| Functions | Full support with any return type | int return only |
| Pointers | Full pointer arithmetic | Not supported |
| Standard library | Huge (stdio, stdlib, string...) | Only print built-in |
| Header files | Yes (#include) | Not needed |
| Compiler | GCC, Clang, MSVC | minic.exe (ours) |
| Output | Machine code via compiler | x86 ASM via our codegen |
| Comments | // and /* */ | // only |
| For loop | Full (any init/update) | int init only |
| Arrays | Multi-dimensional | 1D only |

MiniC is intentionally limited — the goal was to build every phase of a real compiler
from scratch, not to replace C. But MiniC compiles to the same x86 assembly that C
compilers produce, so the output is just as real.

---

## Requirements

- Windows OS
- g++ (MinGW) — for compiling the compiler itself
- NASM — for assembling the generated x86 assembly
- gcc (MinGW) — for linking the final executable

### Check if tools are installed
Open CMD and run:
```
g++ --version
nasm --version
gcc --version
```

### Install NASM (if not installed)
Download: https://www.nasm.us/pub/nasm/releasebuilds/2.16.01/win64/nasm-2.16.01-installer-x64.exe
During install, check "Add to PATH". Then close and reopen CMD.

---

## Project Structure

```
CC Project/
├── compiler.cpp   ← Source code of the MiniC compiler
├── test.mc        ← Your MiniC source file (edit this to test)
├── run.bat        ← One-click build and run script
├── minic.exe      ← Compiler executable (generated)
├── output.asm     ← Generated assembly (generated)
├── output.obj     ← Object file (generated)
└── output.exe     ← Final executable (generated)
```

---

## How to Run

### Step 1 — Navigate to project folder
```cmd
cd "C:\Users\YourName\Desktop\CC\CC Project"
```

### Step 2 — Run everything with one command
```cmd
run.bat, run2.bat, run3.bat, run4.bat(all are same with just different test.mc file being executing)
```

This will automatically:
1. Compile `compiler.cpp` into `minic.exe`
2. Run `minic.exe` on `test.mc` to produce `output.asm`
3. Assemble `output.asm` into `output.obj` using NASM
4. Link `output.obj` into `output.exe` using gcc
5. Run `output.exe` and print results

### Or run manually step by step
```cmd
g++ -o minic.exe compiler.cpp
minic.exe test.mc
nasm -f win32 output.asm -o output.obj
gcc output.obj -o output.exe
output.exe
```

### Contents of run.bat
```bat
@echo off
g++ -o minic.exe compiler.cpp
minic.exe test.mc
nasm -f win32 output.asm -o output.obj
gcc output.obj -o output.exe
output.exe
pause
```

---

## MiniC Language Guide

### Variable Declaration
```c
int x = 5;
int y = x + 3;
```

### Arithmetic
```c
int a = 10 + 5;
int b = a * 2;
int c = b / 4;
int d = c - 1;
```

### If / Else
```c
if (x > 5) {
    print(x);
} else {
    print(y);
}
```

### While Loop
```c
while (x > 0) {
    x = x - 1;
}
```

### For Loop
```c
for (int i = 0; i < 5; i = i + 1) {
    print(i);
}
```

### Functions
```c
int add(int a, int b) {
    int result = a + b;
    return result;
}
int x = add(3, 4);
print(x);
```

### Recursive Functions
```c
int factorial(int n) {
    if (n < 2) {
        return 1;
    }
    int prev = factorial(n - 1);
    int result = n * prev;
    return result;
}
print(factorial(5));
```

### Arrays
```c
int arr[5];
arr[0] = 10;
arr[1] = 20;
print(arr[0]);
```

### String Printing
```c
print("Hello World");
```

### Comments
```c
// This is a comment
int x = 5; // inline comment
```

---

## Supported Operators

| Operator | Meaning |
|----------|---------|
| + | Addition |
| - | Subtraction |
| * | Multiplication |
| / | Division |
| == | Equal to |
| != | Not equal to |
| < | Less than |
| > | Greater than |
| <= | Less than or equal |
| >= | Greater than or equal |

---

## Error Handling

The compiler reports errors for:
- Unknown characters in source
- Unexpected tokens during parsing
- Undefined variables
- Undefined arrays

### Example error messages
```
Parse error: unexpected token '='
Undefined variable: z
Unknown character: @
```

---

## Example Test Cases

### Valid — Factorial
```c
int factorial(int n) {
    if (n < 2) {
        return 1;
    }
    int prev = factorial(n - 1);
    int result = n * prev;
    return result;
}
int x = factorial(5);
print(x);
```
Output: `120`

### Valid — Array with loop
```c
int arr[5];
for (int i = 0; i < 5; i = i + 1) {
    arr[i] = i * 10;
}
for (int j = 0; j < 5; j = j + 1) {
    print(arr[j]);
}
```
Output:
```
0
10
20
30
40
```

### Invalid — Undefined variable
```c
int x = 5;
print(z);
```
Output: `Undefined variable: z`

### Invalid — Bad syntax
```c
int = 5;
```
Output: `Parse error: unexpected token '='`