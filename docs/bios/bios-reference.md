

## BIOS Video Services (INT 10h)

The **INT 10h** interrupt provides a set of video-related services that allow you to control the display, manage the cursor, and write text or graphics directly to the screen. This section details the individual functions available under **INT 10h**, their parameters, and usage examples in **assembly**.

---

### **Set Video Mode** - `AH=00h`

This function sets the display mode (text or graphics) and initializes the video display. Each video mode determines the number of characters or pixels that can be displayed on the screen, as well as the colors supported.

#### Parameters:
- **AH** = `00h` (Function identifier)
- **AL** = Video mode number

#### Video Mode Values (AL):
| **Mode** | **Description**                  |
|----------|----------------------------------|
| `00h`    | 40x25 text mode, 16 colors       |
| `01h`    | 40x25 text mode, monochrome      |
| `02h`    | 80x25 text mode, 16 colors       |
| `03h`    | 80x25 text mode, monochrome      |
| `04h`    | 320x200 graphics mode, 4 colors  |
| `05h`    | 320x200 graphics mode, monochrome|
| `06h`    | 640x200 graphics mode, monochrome|
| `07h`    | 80x25 text mode, monochrome (CGA-compatible) |

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x00, %ah        # Set function: Set video mode
movb $0x03, %al        # Mode 03h: 80x25 text mode, monochrome
int $0x10              # Call BIOS video interrupt
```

This example sets the display to **80x25 text mode**, using monochrome display settings.

---

### **Set Cursor Shape** - `AH=01h`

This function sets the shape of the cursor by defining its start and end scan lines. This is particularly useful for customizing the appearance of the text cursor.

#### Parameters:
- **AH** = `01h` (Function identifier)
- **CH** = Cursor start line (0-31, topmost scan line of the cursor)
- **CL** = Cursor end line (0-31, bottommost scan line of the cursor)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x01, %ah        # Set function: Set cursor shape
movb $0x06, %ch        # Cursor start at scan line 6
movb $0x07, %cl        # Cursor end at scan line 7
int $0x10              # Call BIOS video interrupt
```

In this example, the cursor is set to a block that covers scan lines 6 to 7, giving it a custom size.

---

### **Set Cursor Position** - `AH=02h`

This function sets the position of the text cursor on the screen.

#### Parameters:
- **AH** = `02h` (Function identifier)
- **BH** = Page number (0 for most single-page modes)
- **DH** = Row (Y-position)
- **DL** = Column (X-position)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x02, %ah        # Set function: Set cursor position
movb $0x00, %bh        # Page number 0
movb $0x05, %dh        # Set cursor row to 5
movb $0x10, %dl        # Set cursor column to 16
int $0x10              # Call BIOS video interrupt
```

This sets the cursor to row 5, column 16 on the screen.

---

### **Get Cursor Position** - `AH=03h`

This function retrieves the current position and shape of the text cursor.

#### Parameters:
- **AH** = `03h` (Function identifier)
- **BH** = Page number (0 for most single-page modes)

#### Returns:
- **CH** = Cursor start line (cursor shape start)
- **CL** = Cursor end line (cursor shape end)
- **DH** = Row (current cursor Y-position)
- **DL** = Column (current cursor X-position)

#### Example (AT&T Syntax):
```asm
movb $0x03, %ah        # Set function: Get cursor position and shape
movb $0x00, %bh        # Page number 0
int $0x10              # Call BIOS video interrupt

# Cursor position is now in %dh (row) and %dl (column)
```

This example retrieves the current cursor position and stores the row in `DH` and the column in `DL`.

---

### **Write Character and Attribute at Cursor** - `AH=09h`

This function writes a character to the screen at the current cursor position, using the specified color attribute.

#### Parameters:
- **AH** = `09h` (Function identifier)
- **AL** = ASCII value of the character to display
- **BH** = Page number (0 for most single-page modes)
- **BL** = Attribute (color for text modes)
- **CX** = Number of times to write the character

#### Color Attribute (BL) for Text Modes:
- **High nibble** = Background color (0-7, 8-15 for blinking)
- **Low nibble** = Foreground color (0-15)

| **Foreground Color** | **Value (Low Nibble)** |
|----------------------|-----------------------|
| Black                | 0                     |
| Blue                 | 1                     |
| Green                | 2                     |
| Cyan                 | 3                     |
| Red                  | 4                     |
| Magenta              | 5                     |
| Brown                | 6                     |
| Light Gray           | 7                     |

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x09, %ah        # Set function: Write character at cursor
movb $'A', %al         # ASCII value for 'A'
movb $0x00, %bh        # Page number 0
movb $0x07, %bl        # Attribute: Light gray text on black background
movw $0x01, %cx        # Write the character once
int $0x10              # Call BIOS video interrupt
```

This example writes the letter 'A' at the current cursor position with light gray text on a black background.

---

### **Write Character in TTY Mode** - `AH=0Eh`

This function writes a character to the screen at the current cursor position and automatically advances the cursor. It is commonly used for TTY-style output.

#### Parameters:
- **AH** = `0Eh` (Function identifier)
- **AL** = ASCII value of the character to display
- **BH** = Page number (0 for most single-page modes)
- **BL** = Current text attribute (in text modes)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x0E, %ah        # Set function: Write character in TTY mode
movb $'H', %al         # ASCII value for 'H'
int $0x10              # Call BIOS video interrupt

movb $'i', %al         # ASCII value for 'i'
int $0x10              # Write 'i'
```

This example writes "Hi" to the screen, with each character automatically advancing the cursor.

---

### **Write String to Display** - `AH=13h`

This function writes a string of characters to the screen starting at the current cursor position.

#### Parameters:
- **AH** = `13h` (Function identifier)
- **AL** = Write mode (bit 0: update cursor, bit 1: write string in XOR mode)
- **BH** = Page number (0 for most single-page modes)
- **BL** = Attribute (color for text modes)
- **CX** = Length of the string
- **DL:SI** = Pointer to string in memory (segment:offset)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x13, %ah        # Set function: Write string to display
movb $0x01, %al        # Write mode: update cursor after writing
movb $0x00, %bh        # Page number 0
movb $0x07, %bl        # Attribute: Light gray text on black background
movw $5, %cx           # Length of string to write

lea msg, %si           # Load address of the string into SI
int $0x10              # Call BIOS video interrupt

msg:
    .ascii "Hello"
```

This example writes the string "Hello" to the screen in text mode.



Hier ist der nächste Abschnitt der Handbuchseite zu **INT 13h** – Disk Services. Diese Funktionen ermöglichen den Zugriff auf das Diskettenlaufwerk und die Festplatte, einschließlich Lesen, Schreiben und Abrufen von Disk-Informationen.


## BIOS Disk Services (INT 13h)

The **INT 13h** interrupt provides low-level disk services for reading, writing, and managing floppy disks and hard drives. These functions allow direct access to the disk hardware and are critical for bootloaders, operating systems, and system-level utilities.

---

### **Reset Disk System** - `AH=00h`

This function resets the specified disk drive and its controller. It is typically called before performing any disk operations to ensure that the drive is in a known state.

#### Parameters:
- **AH** = `00h` (Function identifier)
- **DL** = Drive number  
  - `00h-7Fh`: Floppy disk (drive A: is `00h`, drive B: is `01h`)  
  - `80h-FFh`: Hard disk (drive C: is `80h`, drive D: is `81h`)

#### Returns:
- **AH** = Status code (0 on success, non-zero on error)

#### Example (AT&T Syntax):
```asm
movb $0x00, %ah        # Set function: Reset disk system
movb $0x00, %dl        # Drive A:
int $0x13              # Call BIOS disk interrupt

jc error               # Jump to error handling if the carry flag is set
```

In this example, the floppy drive (drive A:) is reset, and any errors can be handled by checking the carry flag (`CF`).

---

### **Read Sectors from Disk** - `AH=02h`

This function reads one or more sectors from a disk into memory.

#### Parameters:
- **AH** = `02h` (Function identifier)
- **AL** = Number of sectors to read (1-255)
- **CH** = Track number (cylinder number, bits 0-7)
- **CL** = Sector number (bits 0-5), and bits 6-7 of the cylinder number (in bits 6-7 of CL)
- **DH** = Head number
- **DL** = Drive number (`00h` = Floppy, `80h` = Hard drive)
- **ES:BX** = Pointer to the memory buffer where data is stored (segment:offset)

#### Returns:
- **AH** = Status code (0 on success, non-zero on error)
- **CF** = Set on error

#### Example (AT&T Syntax):
```asm
movb $0x02, %ah        # Set function: Read sectors from disk
movb $0x01, %al        # Read 1 sector
movb $0x00, %ch        # Cylinder 0 (track 0)
movb $0x01, %cl        # Sector 1
movb $0x00, %dh        # Head 0
movb $0x00, %dl        # Drive A:
movw $0x1000, %bx      # Load address 0x1000 for buffer
movw $0x0000, %es      # Set ES segment to 0
int $0x13              # Call BIOS disk interrupt

jc error               # Jump to error handling if the carry flag is set
```

In this example, one sector is read from **cylinder 0, head 0, sector 1** of the floppy disk into memory at `0x1000:0x0000`.

---

### **Write Sectors to Disk** - `AH=03h`

This function writes one or more sectors from memory to a disk.

#### Parameters:
- **AH** = `03h` (Function identifier)
- **AL** = Number of sectors to write (1-255)
- **CH** = Track number (cylinder number, bits 0-7)
- **CL** = Sector number (bits 0-5), and bits 6-7 of the cylinder number (in bits 6-7 of CL)
- **DH** = Head number
- **DL** = Drive number (`00h` = Floppy, `80h` = Hard drive)
- **ES:BX** = Pointer to the memory buffer containing the data to write (segment:offset)

#### Returns:
- **AH** = Status code (0 on success, non-zero on error)
- **CF** = Set on error

#### Example (AT&T Syntax):
```asm
movb $0x03, %ah        # Set function: Write sectors to disk
movb $0x01, %al        # Write 1 sector
movb $0x00, %ch        # Cylinder 0 (track 0)
movb $0x01, %cl        # Sector 1
movb $0x00, %dh        # Head 0
movb $0x00, %dl        # Drive A:
movw $0x1000, %bx      # Load address 0x1000 for buffer
movw $0x0000, %es      # Set ES segment to 0
int $0x13              # Call BIOS disk interrupt

jc error               # Jump to error handling if the carry flag is set
```

This example writes one sector of data from memory (starting at `0x1000:0x0000`) to the disk at **cylinder 0, head 0, sector 1**.

---

### **Get Drive Parameters** - `AH=08h`

This function retrieves the parameters of the specified drive, including the number of cylinders, heads, and sectors per track. These parameters define the disk geometry.

#### Parameters:
- **AH** = `08h` (Function identifier)
- **DL** = Drive number (`00h` = Floppy, `80h` = Hard drive)

#### Returns:
- **AH** = Status code (0 on success, non-zero on error)
- **CH** = Number of cylinders (bits 0-7)
- **CL** = Number of sectors per track (bits 0-5), upper bits of the cylinder (bits 6-7 of CL)
- **DH** = Number of heads
- **DL** = Number of drives
- **ES:DI** = Pointer to disk type and size information (for hard drives)

#### Example (AT&T Syntax):
```asm
movb $0x08, %ah        # Set function: Get drive parameters
movb $0x00, %dl        # Drive A:
int $0x13              # Call BIOS disk interrupt

jc error               # Jump to error handling if the carry flag is set

# Now CH contains the number of cylinders, CL the number of sectors,
# and DH the number of heads for the floppy disk.
```

This example retrieves the drive geometry for **Drive A:** (floppy disk).

---

### **Extended Read (LBA)** - `AH=42h`

This function reads sectors from a hard drive using **Logical Block Addressing (LBA)**, which allows access to sectors on large drives that exceed the traditional CHS (Cylinder-Head-Sector) addressing limits.

#### Parameters:
- **AH** = `42h` (Function identifier)
- **DL** = Drive number (`80h` = Hard drive)
- **DS:SI** = Pointer to an **Extended Disk Address Packet (EDAP)** in memory, which specifies the LBA, number of sectors, and buffer location.

#### Extended Disk Address Packet (EDAP) Structure:
| **Offset** | **Description**                              |
|------------|----------------------------------------------|
| `00h`      | Size of the packet (10h or 18h bytes)        |
| `01h`      | Reserved (should be 0)                       |
| `02h`      | Number of sectors to read (16-bit)           |
| `04h`      | Pointer to buffer (32-bit address in real mode) |
| `08h`      | Starting LBA (64-bit value)                  |
| `10h`      | Reserved (used in newer packet sizes)        |

#### Returns:
- **AH** = Status code (0 on success, non-zero on error)
- **CF** = Set on error

#### Example (AT&T Syntax):
```asm
movb $0x42, %ah        # Set function: Extended read (LBA)
movb $0x80, %dl        # Hard drive (C:)
lea edap, %si          # Load pointer to Extended Disk Address Packet
movw $0x0000, %ds      # Set DS segment to 0
int $0x13              # Call BIOS disk interrupt

jc error               # Jump to error handling if the carry flag is set

edap:
    .byte 0x10         # Size of the packet (16 bytes)
    .byte 0x00         # Reserved
    .word 1            # Read 1 sector
    .long 0x1000       # Buffer address at 0x1000
    .quad 0            # Starting LBA (e.g., 0 for first sector)
```

In this example, **LBA 0** (the first sector) is read from the hard drive into the memory buffer at `0x1000:0x0000`.


## BIOS Keyboard Services (INT 16h)

The **INT 16h** interrupt provides services for interacting with the keyboard. These functions allow you to read characters, check for keypresses, and retrieve the status of modifier keys (e.g., Shift, Ctrl, Alt). **INT 16h** is essential for handling user input in low-level programs.

---

### **Read Character from Keyboard (Blocking)** - `AH=00h`

This function reads a character from the keyboard buffer. It waits (blocks) until a key is pressed and returns the ASCII value of the key (if applicable) and the scan code.

#### Parameters:
- **AH** = `00h` (Function identifier)

#### Returns:
- **AH** = Scan code of the key
- **AL** = ASCII character code (if applicable, otherwise 0)

#### Example (AT&T Syntax):
```asm
movb $0x00, %ah        # Set function: Read character from keyboard
int $0x16              # Call BIOS keyboard interrupt

# Now AH contains the scan code, and AL contains the ASCII code (if any)
```

In this example, the system waits for a keypress. Once a key is pressed, the ASCII value of the key (if it has one) is returned in `AL`, and the scan code is returned in `AH`.

---

### **Check for Keypress (Non-blocking)** - `AH=01h`

This function checks if a key has been pressed but does not block the program. If a key is available, it returns the key's ASCII value and scan code. If no key is available, it clears the zero flag.

#### Parameters:
- **AH** = `01h` (Function identifier)

#### Returns:
- **ZF** = Set if no key is available
- **AH** = Scan code of the key (if pressed)
- **AL** = ASCII character code (if applicable, otherwise 0)

#### Example (AT&T Syntax):
```asm
movb $0x01, %ah        # Set function: Check for keypress
int $0x16              # Call BIOS keyboard interrupt

jz no_key              # Jump if no key is pressed (ZF is set)

# If a key was pressed, AH contains the scan code and AL contains the ASCII code
no_key:
# Handle no key pressed
```

In this example, the system checks for a keypress without blocking. If no key is available, the zero flag is set, and the program jumps to the `no_key` label.

---

### **Get Keyboard Shift Status** - `AH=02h`

This function retrieves the current status of the keyboard's shift, control, alt, and toggle keys (e.g., Caps Lock, Num Lock). The result is returned in the **AL** register, where each bit represents the state of a specific key.

#### Parameters:
- **AH** = `02h` (Function identifier)

#### Returns:
- **AL** = Shift status byte  
  | **Bit** | **Description**                     |
  |---------|-------------------------------------|
  | `0`     | Right Shift pressed                 |
  | `1`     | Left Shift pressed                  |
  | `2`     | Ctrl pressed                        |
  | `3`     | Alt pressed                         |
  | `4`     | Scroll Lock active                  |
  | `5`     | Num Lock active                     |
  | `6`     | Caps Lock active                    |
  | `7`     | Insert active                       |

#### Example (AT&T Syntax):
```asm
movb $0x02, %ah        # Set function: Get keyboard shift status
int $0x16              # Call BIOS keyboard interrupt

# Now AL contains the shift status
testb $0x01, %al       # Check if Right Shift is pressed
jz no_shift            # Jump if Right Shift is not pressed
```

In this example, the keyboard shift status is retrieved, and the right Shift key status (bit 0) is checked. If the right Shift key is pressed, the code continues; otherwise, it jumps to `no_shift`.

---

### **Get Extended Keyboard Keystroke (Enhanced Keyboards)** - `AH=10h`

This function is used for enhanced keyboards (such as the 101/102-key keyboards). It reads a keystroke from the buffer and returns the scan code and ASCII code for the key, similar to `AH=00h`, but it also handles extended keys.

#### Parameters:
- **AH** = `10h` (Function identifier)

#### Returns:
- **AH** = Scan code of the key (including extended keys)
- **AL** = ASCII character code (if applicable, otherwise 0)

#### Example (AT&T Syntax):
```asm
movb $0x10, %ah        # Set function: Get extended keystroke
int $0x16              # Call BIOS keyboard interrupt

# Now AH contains the scan code (including extended keys), AL contains the ASCII code (if any)
```

This example reads a keypress, including the extended keys, from an enhanced keyboard.

---

### **Get Extended Keyboard Shift Status (Enhanced Keyboards)** - `AH=11h`

This function retrieves the status of shift keys, control keys, and toggle keys on enhanced keyboards. It works similarly to `AH=02h` but provides additional information for extended keyboards.

#### Parameters:
- **AH** = `11h` (Function identifier)

#### Returns:
- **AL** = Shift status byte (same as `AH=02h`)
- **AH** = Extended shift status byte  
  | **Bit** | **Description**                     |
  |---------|-------------------------------------|
  | `0`     | Insert key is pressed               |
  | `1`     | Alt Gr pressed (Right Alt)          |
  | `2`     | Ctrl pressed                        |
  | `3`     | Alt pressed                         |
  | `4`     | Scroll Lock active                  |
  | `5`     | Num Lock active                     |
  | `6`     | Caps Lock active                    |
  | `7`     | SysReq key pressed                  |

#### Example (AT&T Syntax):
```asm
movb $0x11, %ah        # Set function: Get extended shift status
int $0x16              # Call BIOS keyboard interrupt

# Now AL contains the basic shift status, and AH contains the extended shift status
testb $0x02, %ah       # Check if Alt Gr is pressed
jz no_altgr            # Jump if Alt Gr is not pressed
```

In this example, the extended shift status is retrieved, and the Alt Gr key status (bit 1 of AH) is checked.




## BIOS Printer Services (INT 17h)

The **INT 17h** interrupt provides services for sending data to a printer connected via a parallel port (LPT1, LPT2, etc.). It allows low-level control of printing operations, including sending characters, initializing the printer, and retrieving printer status information.

---

### **Send Character to Printer** - `AH=00h`

This function sends a single character to the specified printer. The printer must be connected to a parallel port (typically LPT1).

#### Parameters:
- **AH** = `00h` (Function identifier)
- **AL** = ASCII value of the character to send
- **DX** = Printer port number  
  - `00h`: LPT1  
  - `01h`: LPT2  
  - `02h`: LPT3  

#### Returns:
- **AH** = Printer status  
  | **Bit** | **Description**                        |
  |---------|----------------------------------------|
  | `0`     | 0 = No error, 1 = Printer busy         |
  | `1`     | 0 = No error, 1 = Printer selected     |
  | `2`     | 0 = No error, 1 = Paper out            |
  | `3`     | 0 = No error, 1 = Printer acknowledges |
  | `4`     | 0 = No error, 1 = Printer error        |
  | `7`     | 0 = Operation successful, 1 = Timeout  |

#### Example (AT&T Syntax):
```asm
movb $0x00, %ah        # Set function: Send character to printer
movb $'H', %al         # ASCII value for 'H'
movb $0x00, %dx        # Printer port LPT1
int $0x17              # Call BIOS printer interrupt

# Now AH contains the printer status
testb $0x80, %ah       # Check if timeout occurred (bit 7)
jnz error              # Jump if there was a timeout
```

In this example, the character 'H' is sent to the printer connected to LPT1. The status of the printer is returned in `AH`, and the program checks if there was a timeout (bit 7 of `AH`).

---

### **Initialize Printer** - `AH=01h`

This function initializes the specified printer by sending the necessary control signals to reset the printer and make it ready to receive data.

#### Parameters:
- **AH** = `01h` (Function identifier)
- **DX** = Printer port number  
  - `00h`: LPT1  
  - `01h`: LPT2  
  - `02h`: LPT3  

#### Returns:
- **AH** = Printer status (same format as `AH=00h`)

#### Example (AT&T Syntax):
```asm
movb $0x01, %ah        # Set function: Initialize printer
movb $0x00, %dx        # Printer port LPT1
int $0x17              # Call BIOS printer interrupt

# Now AH contains the printer status
```

In this example, the printer connected to LPT1 is initialized. This ensures that the printer is ready to receive data after being powered on or reset.

---

### **Get Printer Status** - `AH=02h`

This function retrieves the current status of the specified printer. The status can indicate whether the printer is ready, if it is out of paper, or if there is a printer error.

#### Parameters:
- **AH** = `02h` (Function identifier)
- **DX** = Printer port number  
  - `00h`: LPT1  
  - `01h`: LPT2  
  - `02h`: LPT3  

#### Returns:
- **AH** = Printer status  
  | **Bit** | **Description**                        |
  |---------|----------------------------------------|
  | `0`     | 0 = No error, 1 = Printer busy         |
  | `1`     | 0 = No error, 1 = Printer selected     |
  | `2`     | 0 = No error, 1 = Paper out            |
  | `3`     | 0 = No error, 1 = Printer acknowledges |
  | `4`     | 0 = No error, 1 = Printer error        |
  | `7`     | 0 = Operation successful, 1 = Timeout  |

#### Example (AT&T Syntax):
```asm
movb $0x02, %ah        # Set function: Get printer status
movb $0x00, %dx        # Printer port LPT1
int $0x17              # Call BIOS printer interrupt

# Now AH contains the printer status
testb $0x04, %ah       # Check if the printer is out of paper (bit 2)
jnz out_of_paper       # Jump if printer is out of paper
```

In this example, the status of the printer connected to LPT1 is retrieved. The program checks if the printer is out of paper by testing bit 2 of the status byte.



## BIOS System Timer Services (INT 1Ah)

The **INT 1Ah** interrupt provides services for accessing the system timer and the real-time clock (RTC). These functions are used to retrieve and set the system time, as well as to interact with the hardware clock. They are critical for system-level programs that need to manage time-based operations.

---

### **Get System Time (Ticks Since Midnight)** - `AH=00h`

This function retrieves the current system time in the form of ticks since midnight. The system timer ticks 18.2 times per second, so there are approximately 65,536 ticks per hour and 1,180,000 ticks per day.

#### Parameters:
- **AH** = `00h` (Function identifier)

#### Returns:
- **CX:DX** = Number of ticks since midnight
- **AL** = Number of overflow days (if the system has been running for more than 24 hours)

#### Example (AT&T Syntax):
```asm
movb $0x00, %ah        # Set function: Get system time (ticks)
int $0x1A              # Call BIOS system timer interrupt

# Now CX:DX contains the number of ticks since midnight
# AL contains the overflow days (if applicable)
```

In this example, the current system time (in ticks since midnight) is retrieved, and the overflow days are returned in `AL` if the system has been running for more than 24 hours.

---

### **Set System Time** - `AH=01h`

This function sets the system time by specifying the number of ticks since midnight. The ticks should be passed in the **CX:DX** registers.

#### Parameters:
- **AH** = `01h` (Function identifier)
- **CX:DX** = Number of ticks since midnight

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x01, %ah        # Set function: Set system time (ticks)
movw $0x1234, %cx      # Set the high word of the tick count
movw $0x5678, %dx      # Set the low word of the tick count
int $0x1A              # Call BIOS system timer interrupt
```

In this example, the system time is set to the specified tick count (`0x12345678` ticks since midnight).

---

### **Get Real-Time Clock Time** - `AH=02h`

This function retrieves the current time from the real-time clock (RTC), which is a battery-powered clock that keeps track of the time even when the system is powered off.

#### Parameters:
- **AH** = `02h` (Function identifier)

#### Returns:
- **CH** = Hours (BCD format)
- **CL** = Minutes (BCD format)
- **DH** = Seconds (BCD format)

#### Example (AT&T Syntax):
```asm
movb $0x02, %ah        # Set function: Get RTC time
int $0x1A              # Call BIOS system timer interrupt

# Now CH contains the hours, CL contains the minutes, and DH contains the seconds (all in BCD format)
```

In this example, the current time is retrieved from the RTC. The values are returned in **BCD format**, meaning each digit is stored in a separate nibble (e.g., 12:34:56 is returned as `12h:34h:56h`).

---

### **Set Real-Time Clock Time** - `AH=03h`

This function sets the time in the real-time clock (RTC) using the provided hours, minutes, and seconds in BCD format.

#### Parameters:
- **AH** = `03h` (Function identifier)
- **CH** = Hours (BCD format)
- **CL** = Minutes (BCD format)
- **DH** = Seconds (BCD format)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x03, %ah        # Set function: Set RTC time
movb $0x12, %ch        # Set hours to 12 (BCD format)
movb $0x34, %cl        # Set minutes to 34 (BCD format)
movb $0x56, %dh        # Set seconds to 56 (BCD format)
int $0x1A              # Call BIOS system timer interrupt
```

In this example, the time in the RTC is set to 12:34:56 in **BCD format**.

---

### **Get Real-Time Clock Date** - `AH=04h`

This function retrieves the current date from the real-time clock (RTC), including the day, month, and year.

#### Parameters:
- **AH** = `04h` (Function identifier)

#### Returns:
- **CH** = Century (BCD format)
- **CL** = Year (BCD format)
- **DH** = Month (BCD format)
- **DL** = Day (BCD format)

#### Example (AT&T Syntax):
```asm
movb $0x04, %ah        # Set function: Get RTC date
int $0x1A              # Call BIOS system timer interrupt

# Now CH contains the century, CL contains the year, DH contains the month, and DL contains the day (all in BCD format)
```

In this example, the current date is retrieved from the RTC, and the values are returned in **BCD format**.

---

### **Set Real-Time Clock Date** - `AH=05h`

This function sets the date in the real-time clock (RTC) using the provided day, month, and year in BCD format.

#### Parameters:
- **AH** = `05h` (Function identifier)
- **CH** = Century (BCD format)
- **CL** = Year (BCD format)
- **DH** = Month (BCD format)
- **DL** = Day (BCD format)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x05, %ah        # Set function: Set RTC date
movb $0x20, %ch        # Set century to 20 (BCD format)
movb $0x21, %cl        # Set year to 21 (BCD format)
movb $0x09, %dh        # Set month to September (09h in BCD format)
movb $0x15, %dl        # Set day to 15 (BCD format)
int $0x1A              # Call BIOS system timer interrupt
```

In this example, the RTC date is set to September 15, 2021, using **BCD format**.



## BIOS Memory and Bootstrap Services

The **INT 12h** and **INT 19h** interrupts provide essential services for querying the system's memory size and triggering the boot process. These interrupts are fundamental in system initialization and are often used during the boot phase of an operating system.

---

## **INT 12h - Get Memory Size**

This interrupt is used to determine the amount of conventional memory available in the system. Conventional memory is the first 640KB of RAM that is accessible in real mode.

### **Get Memory Size** - `INT 12h`

This function returns the amount of conventional memory in kilobytes (up to 640KB). The value returned represents the amount of memory available below the 1MB mark, and it does not include extended or expanded memory.

#### Parameters:
- None. **INT 12h** does not require any parameters.

#### Returns:
- **AX** = Size of conventional memory in kilobytes (up to 640KB)

#### Example (AT&T Syntax):
```asm
int $0x12              # Call BIOS memory size interrupt

# Now AX contains the size of conventional memory in KB
```

In this example, the system's conventional memory size is returned in **AX**. If the system has 640KB of RAM, **AX** will contain `0x0280` (640 in decimal).

---

## **INT 19h - Bootstrap Loader**

The **INT 19h** interrupt is used to initiate the system's boot process. It loads the first sector (the boot sector) from the boot device (typically a floppy disk or hard drive) and transfers control to the code loaded from that sector.

This interrupt is called by the BIOS during the startup sequence to load the operating system. However, it can also be invoked manually to restart the boot process, especially when developing bootloaders.

### **Bootstrap Loader** - `INT 19h`

This function attempts to boot the system from the current boot device. It loads the first sector of the boot device (such as a floppy disk or hard drive) and then jumps to the code loaded into memory.

#### Parameters:
- None. **INT 19h** does not require any parameters.

#### Returns:
- None. **INT 19h** does not return any values since control is transferred to the boot sector.

#### Example (AT&T Syntax):
```asm
int $0x19              # Call BIOS bootstrap loader interrupt

# Control is transferred to the boot sector of the boot device
```

In this example, the **INT 19h** interrupt is called to trigger the boot process, loading the boot sector and transferring control to the code loaded from it.



## DOS Services (INT 21h)

The **INT 21h** interrupt provides access to a wide range of DOS services, including file handling, memory management, and system control. These services are essential for low-level DOS programs and utilities, allowing you to interact with the operating system and manage system resources.

---

### **Write String to STDOUT** - `AH=09h`

This function writes a string of characters to the standard output (usually the screen). The string must be terminated by a dollar sign (`$`), which is how DOS recognizes the end of the string.

#### Parameters:
- **AH** = `09h` (Function identifier)
- **DX** = Offset of the string in memory (the segment is assumed to be in `DS`)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x09, %ah        # Set function: Write string to STDOUT
movw $msg, %dx         # Load offset of the string into DX
int $0x21              # Call DOS interrupt

msg:
    .ascii "Hello, World!$"  # Null-terminated string for DOS
```

In this example, the string `"Hello, World!"` is printed to the screen. The string must end with a `$` to signal the end of the string to DOS.

---

### **Open File** - `AH=3Dh`

This function opens a file and returns a file handle for subsequent read, write, or close operations.

#### Parameters:
- **AH** = `3Dh` (Function identifier)
- **AL** = Access mode  
  - `00h`: Read-only  
  - `01h`: Write-only  
  - `02h`: Read/Write
- **DS:DX** = Pointer to the file name (ASCIIZ string, terminated by a null byte)

#### Returns:
- **AX** = File handle (if successful)
- **CF** = Set on error
- **AX** = Error code (if `CF` is set)

#### Example (AT&T Syntax):
```asm
movb $0x3D, %ah        # Set function: Open file
movb $0x00, %al        # Access mode: Read-only
movw $filename, %dx    # Load offset of the filename into DX
int $0x21              # Call DOS interrupt

jc error               # Jump to error handling if the carry flag is set
# Now AX contains the file handle

filename:
    .ascii "example.txt\0"  # ASCIIZ filename
```

In this example, the file `"example.txt"` is opened in read-only mode, and the file handle is returned in **AX**. If there is an error (such as the file not existing), the carry flag will be set, and **AX** will contain an error code.

---

### **Close File** - `AH=3Eh`

This function closes an open file using the provided file handle.

#### Parameters:
- **AH** = `3Eh` (Function identifier)
- **BX** = File handle to close

#### Returns:
- **AX** = `00h` if successful
- **CF** = Set on error
- **AX** = Error code (if `CF` is set)

#### Example (AT&T Syntax):
```asm
movb $0x3E, %ah        # Set function: Close file
movw %ax, %bx          # File handle (from a previous open)
int $0x21              # Call DOS interrupt

jc error               # Jump to error handling if the carry flag is set
```

In this example, a previously opened file is closed using the file handle in **BX**. If the operation fails, the carry flag will be set.

---

### **Read from File** - `AH=3Fh`

This function reads data from an open file into a buffer in memory.

#### Parameters:
- **AH** = `3Fh` (Function identifier)
- **BX** = File handle
- **CX** = Number of bytes to read
- **DS:DX** = Pointer to the buffer where the data should be stored

#### Returns:
- **AX** = Number of bytes read (if successful)
- **CF** = Set on error
- **AX** = Error code (if `CF` is set)

#### Example (AT&T Syntax):
```asm
movb $0x3F, %ah        # Set function: Read from file
movw %bx, %bx          # File handle (from a previous open)
movw $100, %cx         # Number of bytes to read
movw $buffer, %dx      # Load offset of the buffer into DX
int $0x21              # Call DOS interrupt

jc error               # Jump to error handling if the carry flag is set
# Now AX contains the number of bytes read

buffer:
    .space 100         # Allocate 100 bytes for the buffer
```

In this example, data is read from the open file into the **buffer**. The number of bytes read is returned in **AX**, and the carry flag is set if an error occurs.

---

### **Write to File** - `AH=40h`

This function writes data from a buffer in memory to an open file.

#### Parameters:
- **AH** = `40h` (Function identifier)
- **BX** = File handle
- **CX** = Number of bytes to write
- **DS:DX** = Pointer to the buffer containing the data to write

#### Returns:
- **AX** = Number of bytes written (if successful)
- **CF** = Set on error
- **AX** = Error code (if `CF` is set)

#### Example (AT&T Syntax):
```asm
movb $0x40, %ah        # Set function: Write to file
movw %bx, %bx          # File handle (from a previous open)
movw $12, %cx          # Number of bytes to write
movw $msg, %dx         # Load offset of the message buffer into DX
int $0x21              # Call DOS interrupt

jc error               # Jump to error handling if the carry flag is set

msg:
    .ascii "Hello, DOS!"
```

In this example, the string `"Hello, DOS!"` is written to the open file. The number of bytes written is returned in **AX**, and the carry flag is set if an error occurs.

---

### **Terminate Program with Return Code** - `AH=4Ch`

This function terminates the currently running program and returns control to DOS, optionally passing a return code.

#### Parameters:
- **AH** = `4Ch` (Function identifier)
- **AL** = Return code (0 for success, non-zero for failure)

#### Returns:
- Control is returned to DOS, and the specified return code is passed.

#### Example (AT&T Syntax):
```asm
movb $0x4C, %ah        # Set function: Terminate program
movb $0x00, %al        # Return code 0 (success)
int $0x21              # Call DOS interrupt
```

In this example, the program terminates and returns a success code (`0`) to DOS.



## BIOS Mouse Services (INT 33h)

The **INT 33h** interrupt provides services for interacting with a mouse. These functions include resetting the mouse, showing and hiding the cursor, retrieving the mouse's position and button states, and setting the cursor position.

---

### **Reset Mouse** - `AX=0000h`

This function initializes the mouse driver and determines if a mouse is installed. It also resets the mouse to its default state.

#### Parameters:
- **AX** = `0000h` (Function identifier)

#### Returns:
- **AX** = `FFFFh` if a mouse is installed
- **AX** = `0000h` if no mouse is installed
- **BX** = Number of mouse buttons

#### Example (AT&T Syntax):
```asm
movw $0x0000, %ax      # Set function: Reset mouse
int $0x33              # Call BIOS mouse interrupt

cmpw $0xFFFF, %ax      # Check if mouse is installed
jne no_mouse           # Jump if no mouse is found

# Now BX contains the number of mouse buttons
```

In this example, the system checks if a mouse is installed. If a mouse is present, **BX** will contain the number of mouse buttons.

---

### **Show Mouse Cursor** - `AX=0001h`

This function makes the mouse cursor visible on the screen.

#### Parameters:
- **AX** = `0001h` (Function identifier)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movw $0x0001, %ax      # Set function: Show mouse cursor
int $0x33              # Call BIOS mouse interrupt
```

In this example, the mouse cursor is made visible on the screen.

---

### **Hide Mouse Cursor** - `AX=0002h`

This function hides the mouse cursor, removing it from the screen.

#### Parameters:
- **AX** = `0002h` (Function identifier)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movw $0x0002, %ax      # Set function: Hide mouse cursor
int $0x33              # Call BIOS mouse interrupt
```

In this example, the mouse cursor is hidden from the screen.

---

### **Get Mouse Position and Button Status** - `AX=0003h`

This function retrieves the current mouse position (in pixels) and the state of the mouse buttons.

#### Parameters:
- **AX** = `0003h` (Function identifier)

#### Returns:
- **BX** = Mouse button status  
  | **Bit** | **Description**               |
  |---------|-------------------------------|
  | `0`     | Left button pressed           |
  | `1`     | Right button pressed          |
  | `2`     | Middle button pressed (if available) |
- **CX** = X-coordinate of the mouse position
- **DX** = Y-coordinate of the mouse position

#### Example (AT&T Syntax):
```asm
movw $0x0003, %ax      # Set function: Get mouse position and button status
int $0x33              # Call BIOS mouse interrupt

# Now BX contains the button status, CX contains the X position, and DX contains the Y position
```

In this example, the current mouse position and button status are retrieved. The coordinates are returned in **CX** (X position) and **DX** (Y position), and the mouse button status is returned in **BX**.

---

### **Set Mouse Position** - `AX=0004h`

This function sets the mouse position to the specified X and Y coordinates on the screen.

#### Parameters:
- **AX** = `0004h` (Function identifier)
- **CX** = New X-coordinate of the mouse position
- **DX** = New Y-coordinate of the mouse position

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movw $0x0004, %ax      # Set function: Set mouse position
movw $100, %cx         # Set X position to 100
movw $150, %dx         # Set Y position to 150
int $0x33              # Call BIOS mouse interrupt
```

In this example, the mouse position is set to coordinates (100, 150).

---

### **Set Mouse Cursor Range** - `AX=0007h`

This function sets the horizontal and vertical ranges for the mouse cursor, restricting its movement to a specific area of the screen.

#### Parameters:
- **AX** = `0007h` (Function identifier)
- **CX** = Minimum X-coordinate (leftmost position)
- **DX** = Maximum X-coordinate (rightmost position)
- **SI** = Minimum Y-coordinate (topmost position)
- **DI** = Maximum Y-coordinate (bottommost position)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movw $0x0007, %ax      # Set function: Set mouse cursor range
movw $50, %cx          # Set minimum X position to 50
movw $300, %dx         # Set maximum X position to 300
movw $50, %si          # Set minimum Y position to 50
movw $200, %di         # Set maximum Y position to 200
int $0x33              # Call BIOS mouse interrupt
```

In this example, the mouse movement is restricted to the rectangle defined by the coordinates (50, 50) and (300, 200).


## BIOS Miscellaneous System Services (INT 15h)

The **INT 15h** interrupt provides a variety of system services that are not covered by other interrupts. These include advanced memory management, A20 gate control, keyboard functions, and system shutdown. These services are crucial for managing system-level operations and extended memory access in low-level programs.

---

### **Get Extended Memory Size** - `AH=88h`

This function returns the size of extended memory (memory above 1MB) available in the system. This memory is used in **protected mode** and is often utilized for extended DOS applications or multitasking operating systems.

#### Parameters:
- **AH** = `88h` (Function identifier)

#### Returns:
- **AX** = Size of extended memory in kilobytes (1KB units, up to 65535KB or 64MB)

#### Example (AT&T Syntax):
```asm
movb $0x88, %ah        # Set function: Get extended memory size
int $0x15              # Call BIOS interrupt

# Now AX contains the size of extended memory in KB
```

In this example, the amount of extended memory available (in kilobytes) is returned in **AX**. This is memory above the first megabyte of system RAM.

---

### **Enable A20 Line** - `AH=2401h`

This function enables the **A20 line**, allowing access to memory addresses above 1MB. The A20 gate is a hardware mechanism that controls whether the 21st address line is enabled, which is required to access memory beyond the 1MB boundary.

#### Parameters:
- **AX** = `2401h` (Function identifier for enabling A20)

#### Returns:
- **AH** = Status (0 if successful)

#### Example (AT&T Syntax):
```asm
movw $0x2401, %ax      # Set function: Enable A20 line
int $0x15              # Call BIOS interrupt

# Now AH contains the status of the operation (0 if successful)
```

In this example, the **A20 line** is enabled, allowing access to extended memory above the 1MB boundary.

---

### **Disable A20 Line** - `AH=2400h`

This function disables the **A20 line**, restricting memory access to below 1MB. Disabling the A20 line reverts the system to a legacy addressing mode, where memory wraps around at the 1MB boundary (useful for emulating older 8086/8088 behavior).

#### Parameters:
- **AX** = `2400h` (Function identifier for disabling A20)

#### Returns:
- **AH** = Status (0 if successful)

#### Example (AT&T Syntax):
```asm
movw $0x2400, %ax      # Set function: Disable A20 line
int $0x15              # Call BIOS interrupt

# Now AH contains the status of the operation (0 if successful)
```

In this example, the **A20 line** is disabled, restricting memory access to below the 1MB boundary.

---

### **System Shutdown** - `AH=4Fh`

This function shuts down the system. It is typically used to turn off the system or initiate a soft power-off if the system supports this feature.

#### Parameters:
- **AH** = `4Fh` (Function identifier)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x4F, %ah        # Set function: System shutdown
int $0x15              # Call BIOS interrupt
```

In this example, the system initiates a shutdown or powers off if supported by the hardware.

---

### **Keyboard Intercept (Keyboard Hook)** - `AH=4Fh`

This function intercepts keyboard input by allowing the BIOS to filter or modify keystrokes. It is often used to modify keyboard behavior or remap keys at a low level.

#### Parameters:
- **AX** = `4Fh` (Function identifier)

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movw $0x4F00, %ax      # Set function: Keyboard intercept
int $0x15              # Call BIOS interrupt
```

In this example, the system sets up a keyboard intercept, allowing the BIOS to monitor or modify keyboard input.

---

### **Copy Extended Memory Block** - `AH=87h`

This function is used to copy blocks of memory in the extended memory area (above 1MB). It is useful for operating systems that need to manage memory blocks for tasks such as moving data between regions of memory.

#### Parameters:
- **AH** = `87h` (Function identifier)
- **ES:SI** = Source address (32-bit physical address)
- **ES:DI** = Destination address (32-bit physical address)
- **CX** = Number of bytes to copy

#### Returns:
- None.

#### Example (AT&T Syntax):
```asm
movb $0x87, %ah        # Set function: Copy extended memory block
movw $0x1000, %si      # Set source address (physical address 0x1000)
movw $0x2000, %di      # Set destination address (physical address 0x2000)
movw $512, %cx         # Set number of bytes to copy
int $0x15              # Call BIOS interrupt
```

In this example, 512 bytes of memory are copied from the source address **0x1000** to the destination address **0x2000** in the extended memory area.

