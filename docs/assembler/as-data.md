# Data Transfer Instructions

## 1. MOV (Move)
The `MOV` instruction transfers data from a source to a destination. This is the most fundamental instruction for moving data between registers, memory, or immediate values.

### Syntax:
```
MOV destination, source
```

### Example (AT&T Syntax):
```asm
movl $5, %eax       # Move the value 5 into EAX
movl %eax, %ebx     # Move the value in EAX into EBX
movw %ax, (%bx)     # Move the word in AX into the memory address pointed to by BX
```

## 2. PUSH (Push to Stack)
The `PUSH` instruction decreases the stack pointer (`ESP`) and stores a value onto the stack.

### Syntax:
```
PUSH source
```

### Example (AT&T Syntax):
```asm
pushl %eax          # Push the value of EAX onto the stack
pushw $10           # Push the immediate value 10 onto the stack
```

## 3. POP (Pop from Stack)
The `POP` instruction retrieves the value from the top of the stack and places it in the specified register or memory location, then increases the stack pointer (`ESP`).

### Syntax:
```
POP destination
```

### Example (AT&T Syntax):
```asm
popl %eax           # Pop the top value from the stack into EAX
popw %bx            # Pop the top value from the stack into BX
```

## 4. PUSHA (Push All General Registers - 16-bit)
The `PUSHA` instruction pushes all general-purpose registers (AX, CX, DX, BX, SP, BP, SI, DI) onto the stack in a specific order. This is a 16-bit instruction.

### Syntax:
```
PUSHA
```

### Example (AT&T Syntax):
```asm
pusha              # Push AX, CX, DX, BX, SP, BP, SI, DI onto the stack
```

## 5. POPA (Pop All General Registers - 16-bit)
The `POPA` instruction pops values from the stack back into the general-purpose registers (AX, CX, DX, BX, SP, BP, SI, DI). This is the complement to `PUSHA`.

### Syntax:
```
POPA
```

### Example (AT&T Syntax):
```asm
popa               # Pop values from the stack into AX, CX, DX, BX, SP, BP, SI, DI
```

## 6. PUSHAD (Push All General Registers - 32-bit)
The `PUSHAD` instruction is similar to `PUSHA`, but it operates on 32-bit registers (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI).

### Syntax:
```
PUSHAD
```

### Example (AT&T Syntax):
```asm
pushad             # Push EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI onto the stack
```

## 7. POPAD (Pop All General Registers - 32-bit)
The `POPAD` instruction is the complement to `PUSHAD` and pops values from the stack back into the 32-bit general-purpose registers (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI).

### Syntax:
```
POPAD
```

### Example (AT&T Syntax):
```asm
popad              # Pop values from the stack into EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
```

## 8. XCHG (Exchange)
The `XCHG` instruction exchanges the contents of two operands.

### Syntax:
```
XCHG destination, source
```

### Example (AT&T Syntax):
```asm
xchgl %eax, %ebx    # Exchange the values of EAX and EBX
xchg %al, %ah       # Exchange the low and high byte of AX
```

## 9. IN (Input from Port)
The `IN` instruction reads a byte or word from an I/O port into a register.

### Syntax:
```
IN accumulator, port
```

### Example (AT&T Syntax):
```asm
inb $0x60, %al      # Read a byte from port 0x60 into AL
inw $0x60, %ax      # Read a word from port 0x60 into AX
```

## 10. OUT (Output to Port)
The `OUT` instruction writes a byte or word from a register to an I/O port.

### Syntax:
```
OUT port, accumulator
```

### Example (AT&T Syntax):
```asm
outb %al, $0x60     # Write a byte from AL to port 0x60
outw %ax, $0x60     # Write a word from AX to port 0x60
```

## 11. XLAT (Table Look-up Translation)
The `XLAT` instruction translates a byte in `AL` using a lookup table. The effective address of the table is determined by adding `AL` to the base address pointed to by `EBX`.

### Syntax:
```
XLAT
```

### Example (AT&T Syntax):
```asm
xlat                # Translate the byte in AL using the lookup table at EBX + AL
```

## 12. LEA (Load Effective Address)
The `LEA` instruction loads the address of the source operand (effective address) into the destination register, rather than loading the value at that address.

### Syntax:
```
LEA destination, source
```

### Example (AT&T Syntax):
```asm
leal (%ebx, %esi, 4), %eax   # Load the effective address of EBX + 4 * ESI into EAX
```

## 13. LDS (Load Pointer to DS: Register)
The `LDS` instruction loads a pointer into a general-purpose register and loads the segment part into the `DS` register.

### Syntax:
```
LDS register, source
```

### Example (AT&T Syntax):
```asm
lds %bx, ptr               # Load pointer into BX and DS
```

## 14. LES (Load Pointer to ES: Register)
The `LES` instruction works similarly to `LDS`, but it loads the pointer into `ES`.

### Syntax:
```
LES register, source
```

### Example (AT&T Syntax):
```asm
les %bx, ptr               # Load pointer into BX and ES
```

## 15. LFS (Load Pointer to FS: Register)
The `LFS` instruction loads a pointer into a general-purpose register and the `FS` segment register.

### Syntax:
```
LFS register, source
```

### Example (AT&T Syntax):
```asm
lfs %eax, ptr              # Load pointer into EAX and FS
```

## 16. LGS (Load Pointer to GS: Register)
The `LGS` instruction loads a pointer into a general-purpose register and the `GS` segment register.

### Syntax:
```
LGS register, source
```

### Example (AT&T Syntax):
```asm
lgs %ebx, ptr              # Load pointer into EBX and GS
```

## 17. LSS (Load Pointer to SS: Register)
The `LSS` instruction loads a pointer into a general-purpose register and the `SS` segment register.

### Syntax:
```
LSS register, source
```

### Example (AT&T Syntax):
```asm
lss %esp, ptr              # Load pointer into ESP and SS
```

## 18. MOVSX (Move with Sign Extension)
The `MOVSX` instruction moves a value from a source operand to a destination register and extends the sign of the source to fill the destination.

### Syntax:
```
MOVSX destination, source
```

### Example (AT&T Syntax):
```asm
movsxw %al, %eax           # Move byte AL to EAX and sign extend it to 32 bits
```

## 19. MOVZX (Move with Zero Extension)
The `MOVZX` instruction moves a value from a source operand to a destination register and extends it with zeroes.

### Syntax:
```
MOVZX destination, source
```

### Example (AT&T Syntax):
```asm
movzxw %al, %eax           # Move byte AL to EAX and zero-extend it to 32 bits
```
