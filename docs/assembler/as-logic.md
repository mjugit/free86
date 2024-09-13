# Logical Instructions

## 1. AND (Logical AND)
The `AND` instruction performs a bitwise logical AND operation between the source and destination operands. The result is stored in the destination operand. It is often used to clear specific bits in a register or memory location.

### Syntax:
```
AND destination, source
```

### Example (AT&T Syntax):
```asm
andl $0xFF, %eax      # Perform AND between 0xFF and EAX
andw %bx, %ax         # Perform AND between AX and BX
```

## 2. OR (Logical OR)
The `OR` instruction performs a bitwise logical OR operation between the source and destination operands. The result is stored in the destination operand. It is used to set specific bits in a register or memory location.

### Syntax:
```
OR destination, source
```

### Example (AT&T Syntax):
```asm
orl $0x01, %eax       # Perform OR between 0x01 and EAX
orw %bx, %ax          # Perform OR between AX and BX
```

## 3. XOR (Exclusive OR)
The `XOR` instruction performs a bitwise exclusive OR operation between the source and destination operands. The result is stored in the destination operand. It is often used for toggling bits or clearing a register (e.g., `XOR`ing a register with itself clears it).

### Syntax:
```
XOR destination, source
```

### Example (AT&T Syntax):
```asm
xorl %eax, %eax       # Clear the EAX register (XOR with itself)
xorw %bx, %ax         # Perform XOR between AX and BX
```

## 4. NOT (Logical NOT)
The `NOT` instruction performs a bitwise logical NOT operation on the destination operand, inverting all bits. It does not affect the processor flags.

### Syntax:
```
NOT destination
```

### Example (AT&T Syntax):
```asm
notl %eax             # Invert all bits in EAX
notw %bx              # Invert all bits in BX
```

## 5. TEST (Logical Compare)
The `TEST` instruction performs a bitwise AND operation between the source and destination operands, but it does not store the result. Instead, it sets the flags based on the result of the operation. This instruction is typically used to check whether specific bits are set.

### Syntax:
```
TEST destination, source
```

### Example (AT&T Syntax):
```asm
testl $0x01, %eax     # Test if the least significant bit of EAX is set
testw %bx, %ax        # Perform bitwise AND between AX and BX and set flags
```
