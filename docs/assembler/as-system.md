# System Instructions

## 1. HLT (Halt)
The `HLT` instruction halts the processor until the next external interrupt is received. This is commonly used to stop the CPU when no further tasks need to be executed or to conserve power.

### Syntax:
```
HLT
```

### Example (AT&T Syntax):
```asm
hlt                 # Halt the processor until the next interrupt
```

## 2. NOP (No Operation)
The `NOP` instruction performs no operation for one clock cycle. It is typically used for padding or timing purposes within code.

### Syntax:
```
NOP
```

### Example (AT&T Syntax):
```asm
nop                 # No operation
```

## 3. ESC (Escape to Coprocessor)
The `ESC` instruction is used to pass control to a coprocessor (such as the floating-point unit, FPU). It provides a way for the main processor to interact with the FPU or other attached coprocessors.

### Syntax:
```
ESC opcode, source
```

### Example (AT&T Syntax):
```asm
esc 0x1, %st        # Escape to FPU with opcode 1
```

## 4. WAIT/FWAIT (Wait)
The `WAIT` (or `FWAIT`) instruction pauses the execution of the CPU until the coprocessor (e.g., FPU) has finished its current operation. It is often used to ensure that floating-point operations are complete before continuing.

### Syntax:
```
WAIT or FWAIT
```

### Example (AT&T Syntax):
```asm
fwait               # Wait for FPU to complete
```

## 5. LOCK (Bus Lock Prefix)
The `LOCK` prefix is used to ensure that the following instruction is executed atomically. This is critical in multiprocessor environments where shared memory operations need to be synchronized across multiple CPUs.

### Syntax:
```
LOCK instruction
```

### Example (AT&T Syntax):
```asm
lock addl $1, %eax  # Add 1 to EAX atomically
```

## 6. BOUND (Check Array Bounds)
The `BOUND` instruction checks whether a value in a register is within the bounds defined by a memory operand. If the value is out of bounds, an interrupt is triggered. It is useful for array bounds checking.

### Syntax:
```
BOUND register, memory
```

### Example (AT&T Syntax):
```asm
bound %eax, array   # Check if EAX is within the bounds defined by 'array'
```

## 7. ENTER (Create Stack Frame)
The `ENTER` instruction creates a stack frame for a procedure, allocating space on the stack for local variables. It is often used in function calls to set up the stack for local storage.

### Syntax:
```
ENTER immediate, nesting_level
```

### Example (AT&T Syntax):
```asm
enter $16, $0       # Create a stack frame with 16 bytes of local storage
```

## 8. LEAVE (Destroy Stack Frame)
The `LEAVE` instruction is used to reverse the effects of `ENTER`. It restores the stack pointer and base pointer to their values before the procedure was called.

### Syntax:
```
LEAVE
```

### Example (AT&T Syntax):
```asm
leave               # Destroy the current stack frame and restore the base pointer
```

## 9. SGDT (Store Global Descriptor Table)
The `SGDT` instruction stores the base and limit of the Global Descriptor Table (GDT) into a memory location.

### Syntax:
```
SGDT memory
```

### Example (AT&T Syntax):
```asm
sgdt gdt_descriptor  # Store the GDT base and limit in gdt_descriptor
```

## 10. SIDT (Store Interrupt Descriptor Table)
The `SIDT` instruction stores the base and limit of the Interrupt Descriptor Table (IDT) into a memory location.

### Syntax:
```
SIDT memory
```

### Example (AT&T Syntax):
```asm
sidt idt_descriptor  # Store the IDT base and limit in idt_descriptor
```

## 11. LGDT (Load Global Descriptor Table)
The `LGDT` instruction loads the base and limit of the Global Descriptor Table (GDT) from a memory location into the GDTR register.

### Syntax:
```
LGDT memory
```

### Example (AT&T Syntax):
```asm
lgdt gdt_descriptor  # Load the GDT base and limit from gdt_descriptor
```

## 12. LIDT (Load Interrupt Descriptor Table)
The `LIDT` instruction loads the base and limit of the Interrupt Descriptor Table (IDT) from a memory location into the IDTR register.

### Syntax:
```
LIDT memory
```

### Example (AT&T Syntax):
```asm
lidt idt_descriptor  # Load the IDT base and limit from idt_descriptor
```

## 13. LTR (Load Task Register)
The `LTR` instruction loads the Task Register (TR) with a selector that points to a Task State Segment (TSS).

### Syntax:
```
LTR register
```

### Example (AT&T Syntax):
```asm
ltr %ax              # Load the Task Register with the selector in AX
```

## 14. LLDT (Load Local Descriptor Table)
The `LLDT` instruction loads the Local Descriptor Table Register (LDTR) with a selector that points to a Local Descriptor Table (LDT).

### Syntax:
```
LLDT register
```

### Example (AT&T Syntax):
```asm
lldt %ax             # Load the Local Descriptor Table Register with the selector in AX
```

## 15. CLTS (Clear Task Switched Flag)
The `CLTS` instruction clears the Task Switched (`TS`) flag in the `CR0` control register. This is typically used by the operating system to reset the state of the processor after a task switch.

### Syntax:
```
CLTS
```

### Example (AT&T Syntax):
```asm
clts                # Clear the Task Switched flag
```

## 16. INVD (Invalidate Cache)
The `INVD` instruction invalidates the contents of the processor's internal caches without writing the modified data back to memory.

### Syntax:
```
INVD
```

### Example (AT&T Syntax):
```asm
invd                # Invalidate the internal caches
```

## 17. WBINVD (Write Back and Invalidate Cache)
The `WBINVD` instruction writes back all modified cache lines to memory and then invalidates the caches.

### Syntax:
```
WBINVD
```

### Example (AT&T Syntax):
```asm
wbinvd              # Write back and invalidate the internal caches
```

## 18. INVLPG (Invalidate TLB Entry)
The `INVLPG` instruction invalidates a specific page in the Translation Lookaside Buffer (TLB), which is used in paging operations.

### Syntax:
```
INVLPG memory
```

### Example (AT&T Syntax):
```asm
invlpg (%eax)       # Invalidate the TLB entry for the page at the address in EAX
```
