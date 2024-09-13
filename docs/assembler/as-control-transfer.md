# Control Transfer Instructions

## 1. JMP (Unconditional Jump)
The `JMP` instruction performs an unconditional jump to the specified address or label. The instruction pointer (`EIP`) is set to the new address.

### Syntax:
```
JMP destination
```

### Example (AT&T Syntax):
```asm
jmp label           # Jump to the label
jmp *%eax           # Jump to the address stored in EAX
```

## 2. Jcc (Conditional Jump)
The `Jcc` instructions perform a conditional jump based on the state of the flags in the **EFLAGS** register. Common examples include:
- **JE/JZ** (Jump if Equal/Zero)
- **JNE/JNZ** (Jump if Not Equal/Not Zero)
- **JG/JNLE** (Jump if Greater/Not Less or Equal)
- **JL/JNGE** (Jump if Less/Not Greater or Equal)

### Syntax:
```
Jcc destination
```

### Example (AT&T Syntax):
```asm
cmp $5, %eax        # Compare EAX with 5
je equal_label      # Jump to equal_label if EAX is equal to 5
jne not_equal_label # Jump to not_equal_label if EAX is not equal to 5
```

## 3. CALL (Call Procedure)
The `CALL` instruction saves the address of the next instruction on the stack and then jumps to the specified procedure or subroutine. It is used to transfer control to a function or subroutine.

### Syntax:
```
CALL destination
```

### Example (AT&T Syntax):
```asm
call my_function    # Call the function at my_function
call *%eax          # Call the function at the address in EAX
```

## 4. RET (Return from Procedure)
The `RET` instruction returns from a procedure by popping the return address from the stack and jumping to it. It is used to return control to the calling function after a `CALL`.

### Syntax:
```
RET
```

### Example (AT&T Syntax):
```asm
ret                 # Return from the current procedure
```

## 5. INT (Software Interrupt)
The `INT` instruction triggers a software interrupt. The interrupt handler for the specified interrupt number is called. This is commonly used for system calls and hardware interaction.

### Syntax:
```
INT interrupt_number
```

### Example (AT&T Syntax):
```asm
int $0x80           # Trigger software interrupt 0x80 (common for Linux system calls)
```

## 6. IRET (Interrupt Return)
The `IRET` instruction is used to return from an interrupt handler. It pops the saved `EIP`, `CS`, and `EFLAGS` from the stack, restoring the state before the interrupt occurred.

### Syntax:
```
IRET
```

### Example (AT&T Syntax):
```asm
iret                # Return from interrupt handler
```

## 7. LOOP (Loop with Counter)
The `LOOP` instruction decrements the `ECX` register and jumps to the specified label if `ECX` is not zero. It is used for loop constructs.

### Syntax:
```
LOOP destination
```

### Example (AT&T Syntax):
```asm
movl $5, %ecx       # Set loop counter to 5
loop start_label    # Jump to start_label until ECX is zero
```

## 8. LOOPE/LOOPZ (Loop while Equal/Zero)
The `LOOPE` (or `LOOPZ`) instruction decrements `ECX` and jumps to the specified label if `ECX` is not zero and the zero flag (`ZF`) is set.

### Syntax:
```
LOOPE destination
```

### Example (AT&T Syntax):
```asm
cmp $5, %eax        # Compare EAX with 5
movl $3, %ecx       # Set loop counter to 3
loope loop_label    # Jump to loop_label while ECX != 0 and ZF is set
```

## 9. LOOPNE/LOOPNZ (Loop while Not Equal/Not Zero)
The `LOOPNE` (or `LOOPNZ`) instruction decrements `ECX` and jumps to the specified label if `ECX` is not zero and the zero flag (`ZF`) is clear.

### Syntax:
```
LOOPNE destination
```

### Example (AT&T Syntax):
```asm
cmp $0, %eax        # Compare EAX with 0
movl $4, %ecx       # Set loop counter to 4
loopne loop_label   # Jump to loop_label while ECX != 0 and ZF is clear
```
