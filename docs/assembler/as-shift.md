# Shift and Rotate Instructions

## 1. SHL (Shift Left)
The `SHL` instruction shifts the bits of the destination operand to the left by a specified number of positions, filling the emptied bits with zeros. Each left shift is equivalent to multiplying the operand by 2.

### Syntax:
```
SHL destination, count
```

### Example (AT&T Syntax):
```asm
shll $2, %eax        # Shift EAX left by 2 bits
shlw $1, %bx         # Shift BX left by 1 bit
```

## 2. SAL (Shift Arithmetic Left)
The `SAL` instruction is identical to `SHL` and shifts the bits of the destination operand to the left, filling the emptied bits with zeros. It is often used interchangeably with `SHL` for arithmetic shifts.

### Syntax:
```
SAL destination, count
```

### Example (AT&T Syntax):
```asm
sall $1, %eax        # Shift EAX left by 1 bit
salw $2, %bx         # Shift BX left by 2 bits
```

## 3. SHR (Shift Right)
The `SHR` instruction shifts the bits of the destination operand to the right by a specified number of positions, filling the emptied bits with zeros. This operation is used for unsigned division by powers of 2.

### Syntax:
```
SHR destination, count
```

### Example (AT&T Syntax):
```asm
shrl $1, %eax        # Shift EAX right by 1 bit (unsigned)
shrw $2, %bx         # Shift BX right by 2 bits (unsigned)
```

## 4. SAR (Shift Arithmetic Right)
The `SAR` instruction shifts the bits of the destination operand to the right by a specified number of positions. The sign bit (most significant bit) is preserved to maintain the number's sign, making it suitable for signed division by powers of 2.

### Syntax:
```
SAR destination, count
```

### Example (AT&T Syntax):
```asm
sarl $1, %eax        # Shift EAX right by 1 bit (signed)
sarw $2, %bx         # Shift BX right by 2 bits (signed)
```

## 5. ROL (Rotate Left)
The `ROL` instruction rotates the bits of the destination operand to the left by the specified count. The bits that are shifted out of the most significant position are reintroduced at the least significant position.

### Syntax:
```
ROL destination, count
```

### Example (AT&T Syntax):
```asm
roll $1, %eax        # Rotate EAX left by 1 bit
rolw $3, %bx         # Rotate BX left by 3 bits
```

## 6. ROR (Rotate Right)
The `ROR` instruction rotates the bits of the destination operand to the right by the specified count. The bits that are shifted out of the least significant position are reintroduced at the most significant position.

### Syntax:
```
ROR destination, count
```

### Example (AT&T Syntax):
```asm
rorl $1, %eax        # Rotate EAX right by 1 bit
rorw $2, %bx         # Rotate BX right by 2 bits
```

## 7. RCL (Rotate Left through Carry)
The `RCL` instruction rotates the bits of the destination operand to the left through the carry flag (`CF`). The bit shifted out of the most significant position is moved into the carry flag, and the previous value of the carry flag is shifted into the least significant bit.

### Syntax:
```
RCL destination, count
```

### Example (AT&T Syntax):
```asm
rcll $1, %eax        # Rotate EAX left through the carry flag by 1 bit
rclw $2, %bx         # Rotate BX left through the carry flag by 2 bits
```

## 8. RCR (Rotate Right through Carry)
The `RCR` instruction rotates the bits of the destination operand to the right through the carry flag (`CF`). The bit shifted out of the least significant position is moved into the carry flag, and the previous value of the carry flag is shifted into the most significant bit.

### Syntax:
```
RCR destination, count
```

### Example (AT&T Syntax):
```asm
rcrl $1, %eax        # Rotate EAX right through the carry flag by 1 bit
rcrw $2, %bx         # Rotate BX right through the carry flag by 2 bits
```
