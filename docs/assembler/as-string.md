# String Instructions

## 1. MOVSB/MOVSW/MOVSD (Move String Data)
The `MOVSB`, `MOVSW`, and `MOVSD` instructions copy a byte (`MOVSB`), word (`MOVSW`), or doubleword (`MOVSD`) from the memory location pointed to by `ESI` to the memory location pointed to by `EDI`. After each move, both `ESI` and `EDI` are incremented or decremented based on the `DF` (Direction Flag).

### Syntax:
```
MOVSB
MOVSW
MOVSD
```

### Example (AT&T Syntax):
```asm
movsb              # Move byte from [ESI] to [EDI], adjust ESI and EDI
movsw              # Move word from [ESI] to [EDI], adjust ESI and EDI
movsd              # Move doubleword from [ESI] to [EDI], adjust ESI and EDI
```

## 2. CMPSB/CMPSW/CMPSD (Compare String Data)
The `CMPSB`, `CMPSW`, and `CMPSD` instructions compare a byte (`CMPSB`), word (`CMPSW`), or doubleword (`CMPSD`) from the memory location pointed to by `ESI` with the memory location pointed to by `EDI`. The comparison sets the flags based on the result (but does not store the result).

### Syntax:
```
CMPSB
CMPSW
CMPSD
```

### Example (AT&T Syntax):
```asm
cmpsb              # Compare byte at [ESI] with byte at [EDI], adjust ESI and EDI
cmpsw              # Compare word at [ESI] with word at [EDI], adjust ESI and EDI
cmpsd              # Compare doubleword at [ESI] with doubleword at [EDI], adjust ESI and EDI
```

## 3. SCASB/SCASW/SCASD (Scan String Data)
The `SCASB`, `SCASW`, and `SCASD` instructions compare the value in `AL` (for `SCASB`), `AX` (for `SCASW`), or `EAX` (for `SCASD`) with the value at the memory location pointed to by `EDI`. The comparison sets the flags based on the result.

### Syntax:
```
SCASB
SCASW
SCASD
```

### Example (AT&T Syntax):
```asm
scasb              # Compare AL with byte at [EDI], adjust EDI
scasw              # Compare AX with word at [EDI], adjust EDI
scasd              # Compare EAX with doubleword at [EDI], adjust EDI
```

## 4. LODSB/LODSW/LODSD (Load String Data)
The `LODSB`, `LODSW`, and `LODSD` instructions load a byte (`LODSB`), word (`LODSW`), or doubleword (`LODSD`) from the memory location pointed to by `ESI` into `AL`, `AX`, or `EAX`, respectively. After the load, `ESI` is adjusted based on the `DF`.

### Syntax:
```
LODSB
LODSW
LODSD
```

### Example (AT&T Syntax):
```asm
lodsb              # Load byte from [ESI] into AL, adjust ESI
lodsw              # Load word from [ESI] into AX, adjust ESI
lodsd              # Load doubleword from [ESI] into EAX, adjust ESI
```

## 5. STOSB/STOSW/STOSD (Store String Data)
The `STOSB`, `STOSW`, and `STOSD` instructions store the value in `AL` (for `STOSB`), `AX` (for `STOSW`), or `EAX` (for `STOSD`) at the memory location pointed to by `EDI`. After storing the value, `EDI` is adjusted based on the `DF`.

### Syntax:
```
STOSB
STOSW
STOSD
```

### Example (AT&T Syntax):
```asm
stosb              # Store AL at [EDI], adjust EDI
stosw              # Store AX at [EDI], adjust EDI
stosd              # Store EAX at [EDI], adjust EDI
```

## 6. REP (Repeat String Operation)
The `REP` prefix is used to repeat a string instruction (`MOVS`, `CMPS`, `SCAS`, `LODS`, `STOS`) a number of times, determined by the `ECX` register. `REP` decrements `ECX` after each iteration and repeats the string operation until `ECX` is zero.

### Syntax:
```
REP string_instruction
```

### Example (AT&T Syntax):
```asm
movl $5, %ecx      # Set counter to 5
rep movsb          # Repeat MOVSB 5 times
```

## 7. REPE/REPZ (Repeat While Equal/Zero)
The `REPE` (or `REPZ`) prefix repeats a string operation as long as the zero flag (`ZF`) is set and `ECX` is not zero. It is commonly used with comparison instructions like `CMPS` or `SCAS`.

### Syntax:
```
REPE string_instruction
```

### Example (AT&T Syntax):
```asm
movl $5, %ecx      # Set counter to 5
repe cmpsb         # Repeat CMPSB while ZF is set and ECX != 0
```

## 8. REPNE/REPNZ (Repeat While Not Equal/Not Zero)
The `REPNE` (or `REPNZ`) prefix repeats a string operation as long as the zero flag (`ZF`) is clear and `ECX` is not zero. It is commonly used with comparison instructions like `CMPS` or `SCAS`.

### Syntax:
```
REPNE string_instruction
```

### Example (AT&T Syntax):
```asm
movl $5, %ecx      # Set counter to 5
repne scasb        # Repeat SCASB while ZF is clear and ECX != 0
```
