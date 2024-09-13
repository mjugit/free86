# Arithmetic Instructions

## 1. ADD (Addition)
The `ADD` instruction adds the value of the source operand to the destination operand, and the result is stored in the destination.

### Syntax:
```
ADD destination, source
```

### Example (AT&T Syntax):
```asm
addl $5, %eax        # Add 5 to EAX
addw %bx, %ax        # Add BX to AX
```

## 2. ADC (Add with Carry)
The `ADC` instruction adds the source operand and the carry flag (`CF`) to the destination operand.

### Syntax:
```
ADC destination, source
```

### Example (AT&T Syntax):
```asm
adcl $1, %eax        # Add 1 and the carry flag to EAX
adcw %bx, %ax        # Add BX and the carry flag to AX
```

## 3. SUB (Subtraction)
The `SUB` instruction subtracts the source operand from the destination operand, and the result is stored in the destination.

### Syntax:
```
SUB destination, source
```

### Example (AT&T Syntax):
```asm
subl $10, %eax       # Subtract 10 from EAX
subw %bx, %ax        # Subtract BX from AX
```

## 4. SBB (Subtract with Borrow)
The `SBB` instruction subtracts the source operand and the carry flag (`CF`) from the destination operand.

### Syntax:
```
SBB destination, source
```

### Example (AT&T Syntax):
```asm
sbbl $3, %eax        # Subtract 3 and the carry flag from EAX
sbbw %bx, %ax        # Subtract BX and the carry flag from AX
```

## 5. IMUL (Signed Multiplication)
The `IMUL` instruction multiplies signed integers. It supports both one-operand and two-operand formats.

### Syntax:
```
IMUL source
IMUL destination, source
```

### Example (AT&T Syntax):
```asm
imull $5, %eax       # Multiply EAX by 5
imulw %bx, %ax       # Multiply AX by BX (signed)
```

## 6. MUL (Unsigned Multiplication)
The `MUL` instruction multiplies unsigned integers. The result is placed in `AX`, `DX:AX`, or `EDX:EAX`, depending on the operand size.

### Syntax:
```
MUL source
```

### Example (AT&T Syntax):
```asm
mull %ebx            # Multiply EAX by EBX (unsigned)
mulw %bx             # Multiply AX by BX (unsigned)
```

## 7. IDIV (Signed Division)
The `IDIV` instruction performs signed division. The dividend is stored in `AX`, `DX:AX`, or `EDX:EAX`, and the quotient is stored in `AX` or `EAX`.

### Syntax:
```
IDIV source
```

### Example (AT&T Syntax):
```asm
idivl %ebx           # Divide EDX:EAX by EBX (signed)
idivw %bx            # Divide DX:AX by BX (signed)
```

## 8. DIV (Unsigned Division)
The `DIV` instruction performs unsigned division. The dividend is stored in `AX`, `DX:AX`, or `EDX:EAX`, and the quotient is stored in `AX` or `EAX`.

### Syntax:
```
DIV source
```

### Example (AT&T Syntax):
```asm
divl %ebx            # Divide EDX:EAX by EBX (unsigned)
divw %bx             # Divide DX:AX by BX (unsigned)
```

## 9. INC (Increment)
The `INC` instruction increases the value of the destination operand by 1.

### Syntax:
```
INC destination
```

### Example (AT&T Syntax):
```asm
incl %eax            # Increment EAX by 1
incw %bx             # Increment BX by 1
```

## 10. DEC (Decrement)
The `DEC` instruction decreases the value of the destination operand by 1.

### Syntax:
```
DEC destination
```

### Example (AT&T Syntax):
```asm
decl %eax            # Decrement EAX by 1
decw %bx             # Decrement BX by 1
```

## 11. NEG (Negate)
The `NEG` instruction negates the value of the destination operand (i.e., performs two's complement negation).

### Syntax:
```
NEG destination
```

### Example (AT&T Syntax):
```asm
negl %eax            # Negate the value in EAX
negw %bx             # Negate the value in BX
```

## 12. CMP (Compare)
The `CMP` instruction subtracts the source operand from the destination operand but does not store the result. It is used to set flags based on the comparison.

### Syntax:
```
CMP destination, source
```

### Example (AT&T Syntax):
```asm
cmpl $5, %eax        # Compare EAX with 5
cmpw %bx, %ax        # Compare AX with BX
```

## 13. DAA (Decimal Adjust for Addition)
The `DAA` instruction adjusts the result of a binary-coded decimal (BCD) addition to ensure a valid BCD result.

### Syntax:
```
DAA
```

### Example (AT&T Syntax):
```asm
addb $9, %al         # Add 9 to AL (BCD addition)
daa                  # Adjust AL for BCD result
```

## 14. DAS (Decimal Adjust for Subtraction)
The `DAS` instruction adjusts the result of a binary-coded decimal (BCD) subtraction to ensure a valid BCD result.

### Syntax:
```
DAS
```

### Example (AT&T Syntax):
```asm
subb $5, %al         # Subtract 5 from AL (BCD subtraction)
das                  # Adjust AL for BCD result
```

## 15. AAA (ASCII Adjust for Addition)
The `AAA` instruction adjusts the result of an ASCII addition to ensure the result is in valid unpacked BCD form.

### Syntax:
```
AAA
```

### Example (AT&T Syntax):
```asm
addb $1, %al         # Add 1 to AL
aaa                  # Adjust AL for ASCII addition result
```

## 16. AAS (ASCII Adjust for Subtraction)
The `AAS` instruction adjusts the result of an ASCII subtraction to ensure the result is in valid unpacked BCD form.

### Syntax:
```
AAS
```

### Example (AT&T Syntax):
```asm
subb $1, %al         # Subtract 1 from AL
aas                  # Adjust AL for ASCII subtraction result
```
