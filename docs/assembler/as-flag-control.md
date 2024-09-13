# Flag Control Instructions

## 1. STC (Set Carry Flag)
The `STC` instruction sets the carry flag (`CF`) to 1. It is typically used to force a carry in arithmetic operations.

### Syntax:
```
STC
```

### Example (AT&T Syntax):
```asm
stc                 # Set the carry flag
```

## 2. CLC (Clear Carry Flag)
The `CLC` instruction clears the carry flag (`CF`), setting it to 0. It is often used before arithmetic operations to ensure that there is no carry.

### Syntax:
```
CLC
```

### Example (AT&T Syntax):
```asm
clc                 # Clear the carry flag
```

## 3. CMC (Complement Carry Flag)
The `CMC` instruction inverts the carry flag (`CF`): if `CF` is 1, it becomes 0, and vice versa. This is useful for adjusting the state of the carry flag.

### Syntax:
```
CMC
```

### Example (AT&T Syntax):
```asm
cmc                 # Complement the carry flag
```

## 4. STD (Set Direction Flag)
The `STD` instruction sets the direction flag (`DF`) to 1. This causes string instructions to auto-decrement pointers (move backwards through memory).

### Syntax:
```
STD
```

### Example (AT&T Syntax):
```asm
std                 # Set the direction flag (for reverse string operations)
```

## 5. CLD (Clear Direction Flag)
The `CLD` instruction clears the direction flag (`DF`), setting it to 0. This causes string instructions to auto-increment pointers (move forwards through memory).

### Syntax:
```
CLD
```

### Example (AT&T Syntax):
```asm
cld                 # Clear the direction flag (for forward string operations)
```

## 6. STI (Set Interrupt Flag)
The `STI` instruction sets the interrupt flag (`IF`), enabling maskable hardware interrupts. This allows the processor to respond to external interrupt requests.

### Syntax:
```
STI
```

### Example (AT&T Syntax):
```asm
sti                 # Enable interrupts
```

## 7. CLI (Clear Interrupt Flag)
The `CLI` instruction clears the interrupt flag (`IF`), disabling maskable hardware interrupts. This is often used in critical sections of code where interrupts must not occur.

### Syntax:
```
CLI
```

### Example (AT&T Syntax):
```asm
cli                 # Disable interrupts
```
