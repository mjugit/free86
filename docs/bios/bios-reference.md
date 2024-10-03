# BIOS function reference

## Introduction

The BIOS (Basic Input/Output System) Interrupt Services provide a critical interface for system developers to access and control hardware components at a low level. These services allow software to interact with hardware devices such as disks, video displays, keyboards, and real-time clocks, directly through BIOS-provided interrupts.

BIOS interrupt functions are a key part of legacy system programming, especially in Real Mode, where they offer standardized ways to perform essential tasks. While modern operating systems largely abstract hardware control through device drivers and high-level APIs, BIOS interrupts remain vital in specific cases such as bootloaders, embedded systems, firmware, and operating system development. For example, BIOS services are often the only available methods to access hardware in the early boot stages before an operating system or custom drivers are loaded.

This document serves as a comprehensive reference for developers working on low-level system programming, especially on older PC systems or in environments where Real Mode access is required. It covers the most important BIOS interrupt services, providing detailed descriptions of each function, their parameters, return values, and potential caveats.

### What are BIOS Interrupt Services?

BIOS Interrupt Services are a set of functions provided by the system’s BIOS that can be invoked using software interrupts. By issuing an interrupt call, software can request specific hardware-related tasks such as reading from a disk, displaying graphics, or querying the system time. Each service is accessed through a specific interrupt vector (for example, INT 0x13 for disk operations or INT 0x10 for video services).

These interrupts are especially useful in Real Mode, where software has direct access to hardware without the abstraction layer provided by modern operating systems. Real Mode operates with 16-bit addresses and limited memory access, and it’s in this environment that BIOS interrupts are most frequently used, particularly in system startup code or bootloaders.

### Important Information for Developers

When working with BIOS interrupts, developers must understand the constraints and limitations of Real Mode, where all of these services operate. Here are some key points to keep in mind:

- **Real Mode vs. Protected Mode:** BIOS interrupts are designed for use in Real Mode, which operates with 16-bit memory addressing and limited access to system memory. Once a system enters Protected Mode (used by modern operating systems), BIOS interrupts are typically no longer accessible unless explicitly supported by virtualization or emulation.
  
- **Register Usage:** BIOS interrupt calls rely on the use of CPU registers to pass parameters and return values. For each function, specific registers such as `AX`, `BX`, `CX`, and others must be set before invoking the interrupt. The result is also returned through registers, so careful management of these is essential for correct function usage.

- **Risk of System Instability:** Direct hardware access through BIOS interrupts can be risky. Improper use of these services can lead to system instability, data corruption, or crashes. Developers should ensure that the hardware state is correctly managed and that any interrupt calls are compatible with the underlying hardware.

- **Legacy Hardware:** Many BIOS interrupt services were designed for legacy hardware such as floppy drives, parallel ports, and early video adapters. While these services are still relevant for bootloaders or older systems, modern hardware may not fully support these functions. Developers should consider compatibility and test their software on the intended hardware environment.

- **Use in Bootloaders and OS Development:** BIOS interrupts are crucial during the early stages of system boot, before the operating system or custom drivers take control. Bootloaders like GRUB and other system-level software use these interrupts to load the operating system from disk, query the system configuration, or display information on the screen.

- **Modern Alternatives:** While BIOS interrupts are essential for low-level hardware access, modern systems often use ACPI (Advanced Configuration and Power Interface) for tasks like power management, and UEFI (Unified Extensible Firmware Interface) has replaced BIOS in many contemporary computers. Developers targeting modern systems should be aware of these alternatives but may still need to rely on BIOS services for backward compatibility or during the system boot process.

### Best Practices for Using BIOS Interrupts

- **Understand the Hardware Environment:** Ensure that the target system supports the BIOS interrupts being used, especially when working with modern hardware or emulated environments.
  
- **Handle Errors Gracefully:** Always check the return values and status codes provided by BIOS interrupts to handle errors gracefully, such as disk read failures or device timeouts.
  
- **Preserve Registers:** Since BIOS interrupts alter register values, it's crucial to save and restore any registers not involved in the interrupt call to avoid corrupting the program state.

- **Optimize for Real Mode:** BIOS interrupts are often used in memory-constrained environments. Efficient use of registers, stack memory, and inline assembly is important for performance and reliability, especially when working within a bootloader or early-stage OS code.


## INT 0x15 - System Services

| Function | Description |
| --- | --- |
| `0x88` | Get Extended Memory Size |
| `0x87` | Move Block in Extended Memory (286+) |
| `0xC0` | Get System Configuration Parameters |
| `0x41` | Wait on External Event |
| `0x90` | Device Busy/Device Post |
| `0x83` | Keyboard Intercept |
| `0x86` | Wait or Sleep |
| `0x4F` | System Shutdown |
| `0x53` | APM Installation Check |
| `0x5300` | APM BIOS Connect |
| `0x5301` | APM Disable |
| `0x5302` | APM Enable |
| `0x5303` | APM Set Power State |

### Get Extended Memory Size

This function retrieves the size of extended memory available on the system, which is memory above the first 1 MB in Real Mode. It is commonly used to detect available extended memory (above 1 MB) for applications or operating systems that require it.

#### Remarks
This function returns the size of extended memory in kilobytes. It can be called on systems with at least 286 CPUs. Be aware that this function does not consider memory-mapped devices, so the actual available memory may vary slightly from what is reported.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x88` to get extended memory size. |

#### Returns
| Register | Description |
| --- | --- |
| `%ax` | Extended memory size in kilobytes (KB). |

#### Example
This example shows how to retrieve the size of extended memory and store the result in `%ax`.

```asm
movb $0x88, %ah     # Set function code for 'Get Extended Memory Size'
int $0x15           # Call BIOS system services
# Result: %ax contains the extended memory size in KB
```

---

### Move Block in Extended Memory (286+)

This function moves a block of data in extended memory, primarily used on 286+ processors. It provides a way to manipulate large data structures or buffer memory in systems with extended memory.

#### Remarks
Be sure to set up the source and destination addresses properly to avoid memory corruption. It is also crucial that the memory regions do not overlap, as this function does not handle overlapping memory blocks gracefully.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x87` for moving a block in extended memory. |
| `%cx` | Number of words (16-bit) to move. |
| `%ds:%si` | Source address of the block in extended memory. |
| `%es:%di` | Destination address in extended memory. |

#### Returns
There are no specific return values for this function, but ensure proper memory handling to avoid unexpected behavior.

#### Example
This example moves a block of 64 words from the source address stored in `%ds:%si` to the destination `%es:%di` in extended memory.

```asm
movb $0x87, %ah     # Set function code for 'Move Block in Extended Memory'
movw $0x40, %cx     # Move 64 words
movw $src_seg, %ds  # Set source segment in extended memory
movw $src_off, %si  # Set source offset in extended memory
movw $dst_seg, %es  # Set destination segment in extended memory
movw $dst_off, %di  # Set destination offset in extended memory
int $0x15           # Call BIOS system services
```

---

### Get System Configuration Parameters

This function retrieves system configuration parameters such as memory sizes, DMA availability, and other hardware configurations. It is useful for system diagnostics or configuration checks.

#### Remarks
This function is considered obsolete on modern systems. It may return useful data on older legacy systems, but it has largely been replaced by more advanced hardware configuration mechanisms like ACPI.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0xC0` to get system configuration parameters. |

#### Returns
| Register | Description |
| --- | --- |
| `%bx` | Model information of the system. |
| `%cx` | Sub-model or hardware feature information. |
| `%dx` | BIOS revision number. |

#### Example
This example retrieves system configuration parameters and stores them in `%bx`, `%cx`, and `%dx`.

```asm
movb $0xC0, %ah     # Set function code for 'Get System Configuration Parameters'
int $0x15           # Call BIOS system services
# Result: %bx contains the model, %cx contains sub-model, and %dx contains BIOS revision
```

---

### Wait on External Event

This function waits for an external event such as hardware signals, DMA completion, or other external triggers. It can be useful in synchronous systems or when direct hardware communication is necessary.

#### Remarks
Use this function with caution, as it will halt the CPU until the external event occurs. This can lead to poor performance if used excessively or incorrectly. Ensure that the event you are waiting for will occur, or the system may hang indefinitely.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x41` to wait on an external event. |

#### Returns
There are no specific return values, but the function returns once the event occurs.

#### Example
This example waits for an external event to complete.

```asm
movb $0x41, %ah     # Set function code for 'Wait on External Event'
int $0x15           # Call BIOS system services
# Waits for the external event to occur
```

---

### Device Busy/Device Post

This function signals to the BIOS that a device is busy or has finished its operation (device post). It is part of the BIOS device activity tracking system.

#### Remarks
This function is often used in coordination with hardware devices like disk controllers or network cards. Be mindful of the device status to prevent race conditions.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x90` to indicate device busy or post. |
| `%al` | Device code (specific to the hardware). |

#### Example
This example signals that a device is busy.

```asm
movb $0x90, %ah     # Set function code for 'Device Busy/Device Post'
movb $0x01, %al     # Device code (e.g., floppy drive)
int $0x15           # Call BIOS system services
# Floppy drive is marked as busy
```

### Keyboard Intercept (INT 0x15 - Function 0x83)

This function intercepts the keyboard input and provides a way to manipulate or control keystrokes before they are processed by the system. It can be used to capture specific key combinations or to modify keyboard behavior.

#### Remarks
This function is used in some older systems for key remapping or handling special key combinations. It provides low-level access to the keyboard, allowing you to intercept keypresses before they are passed to the operating system.

#### Parameters
There are no input parameters.

#### Returns
There are no direct return values, but the function allows interception of the keyboard stream.

#### Example
This function is system-dependent, and specific code examples for its usage depend on the context in which it is implemented.

---

### Wait or Sleep (INT 0x15 - Function 0x86)

This function provides a way to suspend execution for a specified period of time. It can be used to introduce delays or pauses in program execution.

#### Remarks
The delay is specified in microseconds, but the accuracy depends on the system's timer resolution. It is commonly used in situations where precise timing is required.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%cx:%dx` | Time to wait (in microseconds). |

#### Returns
There are no return values.

#### Example
This example suspends execution for 100,000 microseconds (0.1 seconds).

```asm
movb $0x86, %ah     # Set function code for 'Wait or Sleep'
movw $0x0001, %cx   # Set high word of delay to 1
movw $0x86A0, %dx   # Set low word of delay to 100,000 microseconds (0.1 seconds)
int $0x15           # Call BIOS system service
# Execution is delayed by 0.1 seconds
```

---

### System Shutdown (INT 0x15 - Function 0x4F)

This function attempts to shut down the system. It is part of the Advanced Power Management (APM) specification, and it instructs the BIOS to turn off the power to the system.

#### Remarks
Not all systems support this function. On older systems that do, it works by signaling the BIOS to power down the machine. Modern systems typically use ACPI for power management, and this function may not work as expected.

#### Parameters
There are no input parameters.

#### Returns
There are no return values.

#### Example
This example attempts to shut down the system.

```asm
movb $0x4F, %ah     # Set function code for 'System Shutdown'
int $0x15           # Call BIOS system service
# If supported, the system shuts down
```

---

### APM Installation Check (INT 0x15 - Function 0x53)

This function checks if Advanced Power Management (APM) is installed on the system. It returns a signature and version number if APM is present.

#### Remarks
APM is an older power management standard, and while it is supported on legacy systems, modern systems typically use ACPI. If APM is installed, this function will return a signature indicating its presence.

#### Parameters
There are no input parameters.

#### Returns
| Register | Description |
| --- | --- |
| `%ax` | APM signature (`0x504D` for "PM"). |
| `%bx` | APM version number. |

#### Example
This example checks if APM is installed on the system.

```asm
movb $0x53, %ah     # Set function code for 'APM Installation Check'
int $0x15           # Call BIOS system service
cmpw $0x504D, %ax   # Check if APM signature is present
je apm_installed    # Jump if APM is installed
apm_installed:
# APM is installed
```

---

### APM BIOS Connect (INT 0x15 - Function 0x5300)

This function connects the operating system to the APM BIOS, enabling APM features such as power state management and system standby.

#### Remarks
This function must be called before any other APM functions. It initializes communication between the operating system and the APM BIOS.

#### Parameters
There are no input parameters.

#### Returns
There are no return values.

#### Example
This example connects the system to the APM BIOS.

```asm
movb $0x53, %ah     # Set function code for 'APM BIOS Connect'
movb $0x00, %al     # Subfunction for 'Connect'
int $0x15           # Call BIOS system service
# APM BIOS is now connected
```

---

### APM Disable (INT 0x15 - Function 0x5301)

This function disables APM on the system, effectively turning off all APM features.

#### Remarks
Once APM is disabled, the system will no longer manage power states such as standby or suspend. Use this function if power management needs to be controlled manually or via a different method.

#### Parameters
There are no input parameters.

#### Returns
There are no return values.

#### Example
This example disables APM.

```asm
movb $0x53, %ah     # Set function code for 'APM Disable'
movb $0x01, %al     # Subfunction for 'Disable'
int $0x15           # Call BIOS system service
# APM is now disabled
```

---

### APM Enable (INT 0x15 - Function 0x5302)

This function enables APM on the system, allowing the BIOS to manage power states such as standby and suspend.

#### Remarks
Once APM is enabled, the BIOS will automatically manage power states. This function is used to re-enable APM after it has been disabled.

#### Parameters
There are no input parameters.

#### Returns
There are no return values.

#### Example
This example enables APM.

```asm
movb $0x53, %ah     # Set function code for 'APM Enable'
movb $0x02, %al     # Subfunction for 'Enable'
int $0x15           # Call BIOS system service
# APM is now enabled
```

---

### APM Set Power State (INT 0x15 - Function 0x5303)

This function sets the power state of the system or a device (such as standby, suspend, or off).

#### Remarks
APM power states allow the system to enter low-power modes such as standby or suspend. Use this function to manage power consumption on systems that support APM.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%bx` | Device ID (0 for system). |
| `%cx` | Power state (see below). |

##### Power States
| Value | Description |
| --- | --- |
| `0x0000` | Standby |
| `0x0001` | Suspend |
| `0x0002` | Off |

#### Returns
There are no return values.

#### Example
This example puts the system into standby mode.

```asm
movb $0x53, %ah     # Set function code for 'APM Set Power State'
movw $0x0000, %bx   # Set device ID to 0 (system)
movw $0x0000, %cx   # Set power state to Standby
int $0x15           # Call BIOS system service
# The system enters standby mode
```

## INT 0x13 - Disk Services

| Function | Description |
| --- | --- |
| `0x00` | Reset Disk System |
| `0x01` | Read Disk Status |
| `0x02` | Read Sectors from Drive |
| `0x03` | Write Sectors to Drive |
| `0x04` | Verify Sectors |
| `0x05` | Format Track |
| `0x08` | Get Drive Parameters |
| `0x15` | Get Drive Type |
| `0x41` | Check Extensions Installation |
| `0x42` | Extended Read Sectors |
| `0x43` | Extended Write Sectors |
| `0x44` | Verify Extended Sectors |

---

### Reset Disk System

This function resets the disk system, reinitializing the disk controller and drive. It is commonly used to recover from errors or prepare the system for new disk I/O operations.

#### Remarks
This function is essential for error handling in disk I/O. After a failed read or write operation, a disk reset may resolve the issue. However, use it cautiously as it might clear the current state of the drive or controller, requiring reinitialization of operations.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x00` to reset the disk system. |
| `%dl` | Drive number (`0x00` for floppy drive A, `0x80` for the first hard drive). |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Status code (0 if successful, non-zero indicates an error). |

#### Example
This example resets the first hard drive (`0x80`).

```asm
movb $0x00, %ah     # Set function code for 'Reset Disk System'
movb $0x80, %dl     # Select first hard drive
int $0x13           # Call BIOS disk services
# If successful, the disk system is reset
```

---

### Read Disk Status

This function reads the status of the last disk operation. It is typically called after a failed disk I/O operation to diagnose the issue.

#### Remarks
This function is often used after a disk error to retrieve detailed status information. The status codes can indicate whether there was a failure with the drive, controller, or media.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x01` to read disk status. |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Status code of the last disk operation. |

#### Example
This example reads the status of the last disk operation.

```asm
movb $0x01, %ah     # Set function code for 'Read Disk Status'
int $0x13           # Call BIOS disk services
# %ah contains the status code of the last disk operation
```

---

### Read Sectors from Drive

This function reads one or more sectors from a specified disk drive into memory. It is essential for any program that needs to directly read data from a disk, such as an OS loader or file manager.

#### Remarks
When reading sectors, ensure the parameters (cylinder, head, and sector) are within valid ranges for the drive. If reading from a floppy disk, be cautious of the sector size and track layout.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x02` to read sectors. |
| `%al` | Number of sectors to read (must be between 1 and the maximum sectors per track). |
| `%ch` | Cylinder number (high 8 bits of the cylinder). |
| `%cl` | Sector number (lower 6 bits of the sector, high 2 bits of the cylinder). |
| `%dh` | Head number (0-based). |
| `%dl` | Drive number (`0x00` for floppy A, `0x80` for the first hard drive). |
| `%es:%bx` | Memory segment:offset to store the data. |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Status code (0 if successful, non-zero indicates an error). |

#### Example
This example reads one sector from the first hard drive into memory at segment `0x0000` and offset `0x7C00` (typically used by bootloaders).

```asm
movb $0x02, %ah     # Set function code for 'Read Sectors from Drive'
movb $0x01, %al     # Read 1 sector
movb $0x00, %ch     # Cylinder 0
movb $0x01, %cl     # Sector 1
movb $0x00, %dh     # Head 0
movb $0x80, %dl     # First hard drive
movw $0x0000, %es   # Segment for data
movw $0x7C00, %bx   # Offset for data (standard bootloader location)
int $0x13           # Call BIOS disk services
# Reads the first sector of the first hard drive into memory
```

---

### Write Sectors to Drive

This function writes one or more sectors from memory to the specified disk drive. It is often used in disk imaging, creating filesystems, or other low-level disk operations.

#### Remarks
Ensure that the destination on the disk is valid and does not overwrite important data unless intended. Improper use of this function can lead to data corruption or system instability.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x03` to write sectors. |
| `%al` | Number of sectors to write. |
| `%ch` | Cylinder number. |
| `%cl` | Sector number. |
| `%dh` | Head number. |
| `%dl` | Drive number. |
| `%es:%bx` | Memory segment:offset from where the data will be written. |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Status code (0 if successful, non-zero indicates an error). |

#### Example
This example writes one sector from memory at `0x7C00` to the first sector of the first hard drive.

```asm
movb $0x03, %ah     # Set function code for 'Write Sectors to Drive'
movb $0x01, %al     # Write 1 sector
movb $0x00, %ch     # Cylinder 0
movb $0x01, %cl     # Sector 1
movb $0x00, %dh     # Head 0
movb $0x80, %dl     # First hard drive
movw $0x0000, %es   # Segment where data is stored
movw $0x7C00, %bx   # Offset where data is stored
int $0x13           # Call BIOS disk services
# Writes the data from memory into the first sector of the hard drive
```

---

### Verify Sectors

This function verifies the integrity of one or more sectors on the specified disk. It reads the sectors and checks if they are readable without transferring the data to memory.

#### Remarks
This function is useful for verifying that the media is readable without actually reading data into memory. It can be used as part of a disk integrity check.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x04` to verify sectors. |
| `%al` | Number of sectors to verify. |
| `%ch` | Cylinder number. |
| `%cl` | Sector number. |
| `%dh` | Head number. |
| `%dl` | Drive number. |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Status code (0 if successful, non-zero indicates an error). |

#### Example
This example verifies one sector on the first hard drive.

```asm
movb $0x04, %ah     # Set function code for 'Verify Sectors'
movb $0x01, %al     # Verify 1 sector
movb $0x00, %ch     # Cylinder 0
movb $0x01, %cl     # Sector 1
movb $0x00, %dh     # Head 0
movb $0x80, %dl     # First hard drive
int $0x13           # Call BIOS disk services
# Verifies the first sector of the hard drive
```


## INT 0x10 - Video Services

| Function | Description |
| --- | --- |
| `0x00` | Set Video Mode |
| `0x01` | Set Cursor Type |
| `0x02` | Set Cursor Position |
| `0x03` | Get Cursor Position and Size |
| `0x04` | Read Light Pen Position |
| `0x05` | Set Active Display Page |
| `0x06` | Scroll Up Window |
| `0x07` | Scroll Down Window |
| `0x08` | Read Character and Attribute at Cursor Position |
| `0x09` | Write Character and Attribute at Cursor Position |
| `0x0A` | Write Character Only at Cursor Position |
| `0x0B` | Set Background/Border Color |
| `0x0E` | Write Character in Teletype Mode |
| `0x13` | Write String |

---

### Set Video Mode

This function sets the video mode of the display. It is used to switch between different text and graphics modes supported by the video hardware. Common modes include text mode (e.g., 80x25) and various graphics modes.

#### Remarks
Switching video modes will clear the screen, so all currently displayed data will be lost. Make sure to save any important screen data before calling this function. Also, certain video modes may not be supported by all hardware, so ensure compatibility with the display adapter.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x00` for setting video mode. |
| `%al` | Video mode number, specifying the mode to be set. |

##### Video Mode Values
Common video modes supported by the BIOS:
| Value | Description |
| --- | --- |
| `0x03` | 80x25 text mode, 16 colors. |
| `0x13` | 320x200 graphics mode, 256 colors. |

#### Returns
There are no return values for this function.

#### Example
This example sets the video mode to 80x25 text mode.

```asm
movb $0x00, %ah     # Set video mode function
movb $0x03, %al     # 80x25 text mode
int $0x10           # Call BIOS video service
# The display is now in 80x25 text mode
```

---

### Set Cursor Type

This function sets the shape of the text cursor by defining the starting and ending scan lines. It is useful for making the cursor more or less visible depending on the application.

#### Remarks
Ensure that the cursor type matches the current display mode. Some cursor shapes may not be appropriate for graphics modes, and improperly setting the cursor could result in display artifacts.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x01` for setting cursor type. |
| `%ch` | Cursor start line (lower part of the cursor). |
| `%cl` | Cursor end line (upper part of the cursor). |

#### Returns
There are no return values.

#### Example
This example sets a block cursor in 80x25 text mode by setting the start line to 0 and the end line to 15.

```asm
movb $0x01, %ah     # Set cursor type function
movb $0x00, %ch     # Start at scan line 0 (top of the block)
movb $0x0F, %cl     # End at scan line 15 (bottom of the block)
int $0x10           # Call BIOS video service
# The cursor is now a solid block
```

---

### Set Cursor Position

This function sets the position of the text-mode cursor on the screen. It is commonly used to move the cursor to a specific location before writing characters.

#### Remarks
Cursor positions are 0-based, so the top-left corner of the screen corresponds to position (0, 0). Be sure that the cursor is set within the bounds of the display area to avoid display errors.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x02` for setting cursor position. |
| `%bh` | Page number (used for multi-page displays, usually `0x00` for single-page displays). |
| `%dh` | Row (Y position). |
| `%dl` | Column (X position). |

#### Returns
There are no return values.

#### Example
This example moves the cursor to row 10, column 20 on the screen.

```asm
movb $0x02, %ah     # Set cursor position function
movb $0x00, %bh     # Page 0
movb $0x0A, %dh     # Row 10
movb $0x14, %dl     # Column 20
int $0x10           # Call BIOS video service
# The cursor is now at row 10, column 20
```

---

### Get Cursor Position and Size

This function retrieves the current cursor position and size. It is useful when you need to store the current cursor location before moving it or changing the display.

#### Remarks
This function returns both the cursor's location (row and column) and its shape (start and end scan lines), which are useful for restoring the display later.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x03` for getting cursor position and size. |
| `%bh` | Page number (usually `0x00` for single-page displays). |

#### Returns
| Register | Description |
| --- | --- |
| `%ch` | Cursor start scan line. |
| `%cl` | Cursor end scan line. |
| `%dh` | Row (Y position of the cursor). |
| `%dl` | Column (X position of the cursor). |

#### Example
This example retrieves the current cursor position and size for page 0.

```asm
movb $0x03, %ah     # Get cursor position and size function
movb $0x00, %bh     # Page 0
int $0x10           # Call BIOS video service
# The current cursor start scan line is in %ch, end scan line in %cl
# The cursor's position is stored in %dh (row) and %dl (column)
```

---

### Read Light Pen Position

This function returns the light pen position on the screen. It was used with older input devices like light pens, which are mostly obsolete today.

#### Remarks
This function is mostly obsolete, as light pens are no longer common input devices. It may not return valid data on modern hardware.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x04` for reading light pen position. |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Status code (`0x00` if light pen trigger was pressed, `0x01` if not). |
| `%cx` | Column (X position of light pen). |
| `%dx` | Row (Y position of light pen). |

#### Example
This example retrieves the light pen position.

```asm
movb $0x04, %ah     # Read light pen position function
int $0x10           # Call BIOS video service
# The position is stored in %cx (column) and %dx (row)
# %ah contains the status of the light pen trigger
```

---

### Set Active Display Page

This function sets the active display page in text mode. In some modes, multiple display pages can be used to store different screen contents, and this function allows switching between them.

#### Remarks
Switching display pages is useful for double-buffering in text mode or for quickly displaying different content without rewriting the entire screen.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x05` for setting active display page. |
| `%al` | Page number (0-based). |

#### Returns
There are no return values.

#### Example
This example switches to display page 1.

```asm
movb $0x05, %ah     # Set active display page function
movb $0x01, %al     # Switch to page 1
int $0x10           # Call BIOS video service
# The display switches to page 1
```

## INT 0x16 - Keyboard Services

| Function | Description |
| --- | --- |
| `0x00` | Read Character from Keyboard Buffer |
| `0x01` | Check for Keystroke (Non-blocking) |
| `0x02` | Get Keyboard Shift Flags |
| `0x05` | Store Keystroke in Keyboard Buffer |
| `0x10` | Get Extended Keystroke (for AT/PS2 keyboards) |
| `0x11` | Check for Extended Keystroke (for AT/PS2 keyboards) |

---

### Read Character from Keyboard Buffer

This function reads a character from the keyboard buffer. If no keystroke is available, it waits until a key is pressed, making it a blocking function.

#### Remarks
This function returns both the ASCII code and the scan code of the key that was pressed. The function will block (wait) until a key is pressed, so use it carefully if you need non-blocking behavior.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x00` to read a character from the keyboard buffer. |

#### Returns
| Register | Description |
| --- | --- |
| `%al` | ASCII code of the key pressed (0 if a special key, like function keys). |
| `%ah` | Scan code of the key pressed. |

#### Example
This example reads a key from the keyboard and stores the ASCII code in `%al` and the scan code in `%ah`.

```asm
movb $0x00, %ah     # Set function code for 'Read Character from Keyboard Buffer'
int $0x16           # Call BIOS keyboard service
# The ASCII code is stored in %al, scan code in %ah
```

---

### Check for Keystroke (Non-blocking)

This function checks whether a keystroke is available in the keyboard buffer. Unlike function `0x00`, this function does not block if no keystroke is available. If a key has been pressed, it returns the key without waiting.

#### Remarks
This is a non-blocking function, making it useful when you need to check for key input without halting the program. If no key has been pressed, the Zero Flag (ZF) will be set.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x01` to check for keystroke. |

#### Returns
| Register | Description |
| --- | --- |
| `%al` | ASCII code of the key (if available). |
| `%ah` | Scan code of the key (if available). |

##### Zero Flag
| Value | Description |
| --- | --- |
| `ZF=1` | No keystroke available. |
| `ZF=0` | Keystroke is available. |

#### Example
This example checks if a key has been pressed and retrieves its value if available.

```asm
movb $0x01, %ah     # Set function code for 'Check for Keystroke'
int $0x16           # Call BIOS keyboard service
jz no_key           # If ZF is set, no key is pressed
# The ASCII code is stored in %al, scan code in %ah
no_key:
# No key was pressed
```

---

### Get Keyboard Shift Flags

This function retrieves the current status of the keyboard shift flags (e.g., Shift, Ctrl, Alt, Caps Lock, Num Lock). It can be used to check the current state of modifier keys and lock keys.

#### Remarks
This function does not consume a keystroke from the keyboard buffer; it simply returns the status of the keyboard shift flags.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x02` to get keyboard shift flags. |

#### Returns
| Register | Description |
| --- | --- |
| `%al` | Keyboard shift flag status (bitmask). |

##### Shift Flag Bitmask
| Bit | Flag Description |
| --- | --- |
| `0` | Right Shift pressed |
| `1` | Left Shift pressed |
| `2` | Ctrl pressed |
| `3` | Alt pressed |
| `4` | Scroll Lock active |
| `5` | Num Lock active |
| `6` | Caps Lock active |
| `7` | Insert mode active |

#### Example
This example retrieves the keyboard shift flags and checks if Caps Lock is active.

```asm
movb $0x02, %ah     # Set function code for 'Get Keyboard Shift Flags'
int $0x16           # Call BIOS keyboard service
testb $0x40, %al    # Test if the Caps Lock bit (bit 6) is set
jnz caps_on         # If set, Caps Lock is active
caps_on:
# Caps Lock is active
```

---

### Store Keystroke in Keyboard Buffer

This function manually stores a keystroke in the keyboard buffer. It is useful for simulating keypresses programmatically, such as for automated testing or key macros.

#### Remarks
Ensure that the keyboard buffer has space for the new keystroke. If the buffer is full, the function may fail without reporting an error.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x05` to store a keystroke. |
| `%ch` | Scan code of the key to be stored. |
| `%cl` | ASCII code of the key to be stored. |

#### Returns
There are no return values.

#### Example
This example stores a simulated keystroke for the 'A' key in the keyboard buffer.

```asm
movb $0x05, %ah     # Set function code for 'Store Keystroke in Buffer'
movb $0x1E, %ch     # Scan code for 'A' key
movb $0x41, %cl     # ASCII code for 'A' key
int $0x16           # Call BIOS keyboard service
# 'A' is now stored in the keyboard buffer
```

---

### Get Extended Keystroke (for AT/PS2 keyboards)

This function reads an extended keystroke from the keyboard buffer. It supports extended keys on AT and PS/2 keyboards, which have more keys than the original IBM PC/XT keyboards.

#### Remarks
This function is similar to `0x00` but supports extended key codes from modern keyboards. It will block until a keystroke is available, making it a blocking call.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x10` to get extended keystroke. |

#### Returns
| Register | Description |
| --- | --- |
| `%al` | ASCII code of the key pressed (if applicable). |
| `%ah` | Scan code of the key pressed (including extended codes). |

#### Example
This example reads an extended keystroke and stores the ASCII and scan codes.

```asm
movb $0x10, %ah     # Set function code for 'Get Extended Keystroke'
int $0x16           # Call BIOS keyboard service
# The ASCII code is stored in %al, scan code (including extended) in %ah
```

---

### Check for Extended Keystroke (Non-blocking, for AT/PS2 keyboards)

This function checks for an extended keystroke without blocking. It is useful for handling input from modern keyboards with extended keysets without waiting for input.

#### Remarks
This is a non-blocking version of the extended keystroke read function. If no key has been pressed, the Zero Flag (ZF) will be set, and no values will be returned.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x11` to check for extended keystroke. |

#### Returns
| Register | Description |
| --- | --- |
| `%al` | ASCII code of the key (if available). |
| `%ah` | Scan code of the key (if available). |

##### Zero Flag
| Value | Description |
| --- | --- |
| `ZF=1` | No keystroke available. |
| `ZF=0` | Keystroke is available. |

#### Example
This example checks for an extended keystroke.

```asm
movb $0x11, %ah     # Set function code for 'Check for Extended Keystroke'
int $0x16           # Call BIOS keyboard service
jz no_key           # If ZF is set, no key is pressed
# The ASCII code is stored in %al, scan code (including extended) in %ah
no_key:
# No key was pressed
```


## INT 0x17 - Parallel Port Services

| Function | Description |
| --- | --- |
| `0x00` | Print a Character to Printer |
| `0x01` | Initialize Printer |
| `0x02` | Check Printer Status |

---

### Print a Character to Printer

This function sends a character to the printer connected to the parallel port. It waits until the printer is ready before sending the character, making it a blocking function.

#### Remarks
Make sure the printer is online and ready to receive data. The function will block until the printer is ready, so if the printer is offline or experiencing an error, it may cause the system to hang. Also, ensure that the parallel port is correctly initialized before using this function.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x00` to print a character to the printer. |
| `%al` | ASCII character to be printed. |
| `%dx` | Printer port number (usually `0x00` for LPT1, `0x01` for LPT2, and `0x02` for LPT3). |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Printer status (0 if successful, otherwise error code). |

##### Printer Status Flags
| Value | Description |
| --- | --- |
| `0x00` | Successful operation, printer is ready. |
| `0x01` | Printer timeout (printer did not respond in time). |
| `0x02` | Printer not selected (offline). |
| `0x08` | I/O error occurred during communication with the printer. |

#### Example
This example prints the character 'A' to the printer connected to LPT1.

```asm
movb $0x00, %ah     # Set function code for 'Print Character to Printer'
movb $0x41, %al     # ASCII code for 'A'
movb $0x00, %dx     # Select LPT1
int $0x17           # Call BIOS parallel port service
# The character 'A' is sent to the printer
```

---

### Initialize Printer

This function initializes the printer connected to the specified parallel port. It prepares the printer for receiving data and resets any error conditions.

#### Remarks
This function is typically called before sending data to the printer. It resets the printer and clears any existing errors, making it ready to receive further data.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x01` to initialize the printer. |
| `%dx` | Printer port number (`0x00` for LPT1, `0x01` for LPT2, `0x02` for LPT3). |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Printer status after initialization. |

##### Printer Status Flags
| Value | Description |
| --- | --- |
| `0x00` | Successful initialization. |
| `0x01` | Printer timeout (printer did not respond in time). |
| `0x02` | Printer not selected (offline). |
| `0x08` | I/O error occurred during initialization. |

#### Example
This example initializes the printer connected to LPT1.

```asm
movb $0x01, %ah     # Set function code for 'Initialize Printer'
movb $0x00, %dx     # Select LPT1
int $0x17           # Call BIOS parallel port service
# The printer connected to LPT1 is now initialized
```

---

### Check Printer Status

This function checks the status of the printer connected to the specified parallel port. It returns flags that indicate whether the printer is ready, offline, or experiencing an error.

#### Remarks
Use this function to check the printer's status before sending data or to diagnose communication issues. The status flags provide valuable information about the state of the printer, such as whether it's online, busy, or in an error state.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x02` to check the printer status. |
| `%dx` | Printer port number (`0x00` for LPT1, `0x01` for LPT2, `0x02` for LPT3). |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Printer status flags. |

##### Printer Status Flags
| Bit | Description |
| --- | --- |
| `0` | Time-out occurred. |
| `1` | Printer not selected (offline). |
| `2` | Out of paper. |
| `3` | Printer error. |
| `4` | Printer is busy (cannot receive data). |
| `5` | Printer is ready (able to receive data). |

#### Example
This example checks the status of the printer connected to LPT1 and verifies if the printer is ready.

```asm
movb $0x02, %ah     # Set function code for 'Check Printer Status'
movb $0x00, %dx     # Select LPT1
int $0x17           # Call BIOS parallel port service
testb $0x20, %ah    # Test if the printer is ready (bit 5)
jnz printer_ready   # Jump if the printer is ready to receive data
printer_ready:
# The printer is ready to receive data
```

## INT 0x14 - Serial Port Services

| Function | Description |
| --- | --- |
| `0x00` | Initialize Serial Port |
| `0x01` | Transmit Character via Serial Port |
| `0x02` | Receive Character via Serial Port |
| `0x03` | Get Serial Port Status |

---

### Initialize Serial Port

This function initializes the specified serial port by setting the baud rate, parity, stop bits, and data length. It prepares the serial port for communication.

#### Remarks
Before transmitting or receiving data, it is important to initialize the serial port with the desired communication parameters. If the serial port is not initialized properly, data transfer may result in incorrect or corrupted data.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x00` to initialize the serial port. |
| `%al` | Communication parameters, which include baud rate, parity, stop bits, and data length (see below). |
| `%dx` | Serial port number (`0x00` for COM1, `0x01` for COM2, etc.). |

##### Communication Parameters (stored in `%al`)
| Bit(s) | Description |
| --- | --- |
| `0-3` | Baud rate (see table below). |
| `4`   | Parity (0 = none, 1 = even). |
| `5`   | Stop bits (0 = 1 stop bit, 1 = 2 stop bits). |
| `6-7` | Data length (0 = 8 bits, 1 = 7 bits). |

##### Baud Rate Values
| Value | Baud Rate |
| --- | --- |
| `0x00` | 110 baud |
| `0x01` | 150 baud |
| `0x02` | 300 baud |
| `0x03` | 600 baud |
| `0x04` | 1200 baud |
| `0x05` | 2400 baud |
| `0x06` | 4800 baud |
| `0x07` | 9600 baud |

#### Returns
There are no return values.

#### Example
This example initializes COM1 with 9600 baud, no parity, 1 stop bit, and 8 data bits.

```asm
movb $0x00, %ah     # Set function code for 'Initialize Serial Port'
movb $0x07, %al     # 9600 baud, no parity, 1 stop bit, 8 data bits
movb $0x00, %dx     # Select COM1
int $0x14           # Call BIOS serial port service
# COM1 is now initialized
```

---

### Transmit Character via Serial Port

This function transmits a character through the specified serial port. It waits until the serial port is ready to send data, making it a blocking function.

#### Remarks
Make sure the serial port is initialized before attempting to transmit data. The function will block until the serial port is ready to send the character, so avoid using it in time-critical loops without proper consideration.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x01` to transmit a character via serial port. |
| `%al` | ASCII character to be transmitted. |
| `%dx` | Serial port number (`0x00` for COM1, `0x01` for COM2, etc.). |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Status code (0 if successful, otherwise error code). |

##### Serial Port Status Flags
| Value | Description |
| --- | --- |
| `0x00` | Successful transmission. |
| `0x01` | Timeout (serial port did not respond). |

#### Example
This example transmits the character 'A' via COM1.

```asm
movb $0x01, %ah     # Set function code for 'Transmit Character via Serial Port'
movb $0x41, %al     # ASCII code for 'A'
movb $0x00, %dx     # Select COM1
int $0x14           # Call BIOS serial port service
# The character 'A' is sent through COM1
```

---

### Receive Character via Serial Port

This function receives a character from the specified serial port. It waits until a character is received, making it a blocking function.

#### Remarks
Make sure the serial port is initialized before attempting to receive data. The function will block until a character is available, so use it cautiously in time-sensitive applications.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x02` to receive a character via serial port. |
| `%dx` | Serial port number (`0x00` for COM1, `0x01` for COM2, etc.). |

#### Returns
| Register | Description |
| --- | --- |
| `%al` | ASCII code of the received character. |
| `%ah` | Status code (0 if successful, otherwise error code). |

##### Serial Port Status Flags
| Value | Description |
| --- | --- |
| `0x00` | Successful reception. |
| `0x01` | Timeout (no data received). |

#### Example
This example receives a character from COM1 and stores it in `%al`.

```asm
movb $0x02, %ah     # Set function code for 'Receive Character via Serial Port'
movb $0x00, %dx     # Select COM1
int $0x14           # Call BIOS serial port service
# The received character is stored in %al
```

---

### Get Serial Port Status

This function checks the status of the specified serial port. It returns flags that indicate whether the serial port is ready to transmit or receive data.

#### Remarks
This function is useful for checking the state of the serial port without blocking the program. It provides information on whether the port is ready to send or receive data, allowing you to manage serial communications more efficiently.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x03` to get serial port status. |
| `%dx` | Serial port number (`0x00` for COM1, `0x01` for COM2, etc.). |

#### Returns
| Register | Description |
| --- | --- |
| `%ah` | Status flags of the serial port. |

##### Serial Port Status Flags
| Bit | Description |
| --- | --- |
| `0` | Data Ready (1 if data is available to be read). |
| `1` | Overrun Error (1 if the input buffer has overrun). |
| `2` | Parity Error (1 if a parity error occurred). |
| `3` | Framing Error (1 if a framing error occurred). |
| `4` | Break Interrupt (1 if a break signal is detected). |
| `5` | Transmit Holding Register Empty (1 if ready to transmit). |
| `6` | Transmit Shift Register Empty (1 if all data has been sent). |
| `7` | Timeout Error (1 if a timeout occurred). |

#### Example
This example checks the status of COM1 to see if the port is ready to transmit data.

```asm
movb $0x03, %ah     # Set function code for 'Get Serial Port Status'
movb $0x00, %dx     # Select COM1
int $0x14           # Call BIOS serial port service
testb $0x20, %ah    # Test if the Transmit Holding Register is empty (bit 5)
jnz ready_to_send   # Jump if the serial port is ready to send data
ready_to_send:
# COM1 is ready to transmit data
```

## INT 0x1A - Real-Time Clock Services

| Function | Description |
| --- | --- |
| `0x00` | Get System Time |
| `0x01` | Set System Time |
| `0x02` | Get Real-Time Clock Time |
| `0x03` | Set Real-Time Clock Time |
| `0x04` | Get Real-Time Clock Date |
| `0x05` | Set Real-Time Clock Date |
| `0x06` | Set RTC Alarm |
| `0x07` | Reset RTC Alarm |
| `0x08` | Read RTC Status |
| `0x09` | Set RTC Interrupt Mask |
| `0x0B` | Read Extended CMOS Data |
| `0x0C` | Write Extended CMOS Data |

---

### Get System Time

This function retrieves the system time in the format of hours, minutes, and seconds since midnight. The time is stored in the BIOS Data Area (BDA), and this function reads it.

#### Remarks
The system time is updated every 18.2 ticks per second. If you need higher precision timing, use timer-related functions. Be aware that this function retrieves the time since the last system boot.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x00` to get system time. |

#### Returns
| Register | Description |
| --- | --- |
| `%cx` | Hours (in BCD format). |
| `%dx` | Minutes and seconds (BCD format, high byte = minutes, low byte = seconds). |

#### Example
This example retrieves the current system time in hours, minutes, and seconds.

```asm
movb $0x00, %ah     # Set function code for 'Get System Time'
int $0x1A           # Call BIOS real-time clock service
# %cx contains hours in BCD, %dx contains minutes (high byte) and seconds (low byte) in BCD
```

---

### Set System Time

This function sets the system time in hours, minutes, and seconds since midnight. The time is stored in the BIOS Data Area (BDA).

#### Remarks
Make sure to provide valid values for hours, minutes, and seconds in Binary-Coded Decimal (BCD) format. This function only updates the system time stored in the BDA and does not modify the Real-Time Clock (RTC).

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x01` to set system time. |
| `%cx` | Hours (BCD format). |
| `%dx` | Minutes and seconds (BCD format, high byte = minutes, low byte = seconds). |

#### Returns
There are no return values.

#### Example
This example sets the system time to 12:30:45.

```asm
movb $0x01, %ah     # Set function code for 'Set System Time'
movw $0x1230, %cx   # 12 hours (BCD format)
movw $0x3045, %dx   # 30 minutes and 45 seconds (BCD format)
int $0x1A           # Call BIOS real-time clock service
# System time is now set to 12:30:45
```

---

### Get Real-Time Clock Time

This function retrieves the time from the Real-Time Clock (RTC) in Binary-Coded Decimal (BCD) format. The RTC continues to run even when the system is powered off, so this function provides the actual time of day.

#### Remarks
The RTC provides the actual time as maintained by the system's battery-backed clock, so it persists across reboots. Make sure to convert BCD values if needed.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x02` to get RTC time. |

#### Returns
| Register | Description |
| --- | --- |
| `%ch` | Hours (BCD format). |
| `%cl` | Minutes (BCD format). |
| `%dh` | Seconds (BCD format). |

#### Example
This example retrieves the current RTC time.

```asm
movb $0x02, %ah     # Set function code for 'Get RTC Time'
int $0x1A           # Call BIOS real-time clock service
# %ch contains hours, %cl contains minutes, %dh contains seconds (all in BCD format)
```

---

### Set Real-Time Clock Time

This function sets the time in the Real-Time Clock (RTC) using Binary-Coded Decimal (BCD) values. This time is retained even when the system is powered off, as the RTC is battery-backed.

#### Remarks
The RTC time will persist across system reboots and power-offs, as it is powered by a battery. Make sure the values are valid and in BCD format before setting the time.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x03` to set RTC time. |
| `%ch` | Hours (BCD format). |
| `%cl` | Minutes (BCD format). |
| `%dh` | Seconds (BCD format). |

#### Returns
There are no return values.

#### Example
This example sets the RTC time to 14:45:30.

```asm
movb $0x03, %ah     # Set function code for 'Set RTC Time'
movb $0x14, %ch     # 14 hours (BCD format)
movb $0x45, %cl     # 45 minutes (BCD format)
movb $0x30, %dh     # 30 seconds (BCD format)
int $0x1A           # Call BIOS real-time clock service
# RTC time is now set to 14:45:30
```

---

### Get Real-Time Clock Date

This function retrieves the current date from the Real-Time Clock (RTC), including the year, month, and day. The date is returned in Binary-Coded Decimal (BCD) format.

#### Remarks
The RTC provides the actual date maintained by the system's battery-backed clock, so this function gives the persistent system date. The date includes the century, so be sure to handle 4-digit years.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x04` to get RTC date. |

#### Returns
| Register | Description |
| --- | --- |
| `%ch` | Year (BCD format). |
| `%cl` | Month (BCD format). |
| `%dh` | Day (BCD format). |

#### Example
This example retrieves the current RTC date.

```asm
movb $0x04, %ah     # Set function code for 'Get RTC Date'
int $0x1A           # Call BIOS real-time clock service
# %ch contains the year, %cl contains the month, %dh contains the day (all in BCD format)
```

---

### Set Real-Time Clock Date

This function sets the date in the Real-Time Clock (RTC), using Binary-Coded Decimal (BCD) values. The date includes the year, month, and day, and is stored in the battery-backed RTC.

#### Remarks
The date is stored in the RTC, which is persistent across reboots and power-offs. Ensure the date is in valid BCD format and includes the full year (including century).

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x05` to set RTC date. |
| `%ch` | Year (BCD format). |
| `%cl` | Month (BCD format). |
| `%dh` | Day (BCD format). |

#### Returns
There are no return values.

#### Example
This example sets the RTC date to August 15, 2023.

```asm
movb $0x05, %ah     # Set function code for 'Set RTC Date'
movb $0x20, %ch     # Year 2023 (BCD format for 23)
movb $0x08, %cl     # Month 8 (August, BCD format)
movb $0x15, %dh     # Day 15 (BCD format)
int $0x1A           # Call BIOS real-time clock service
# RTC date is now set to August 15, 2023
```

### Read Extended CMOS Data (INT 0x1A - Function 0x0B)

This function reads data from the extended CMOS (Complementary Metal-Oxide Semiconductor) area. The CMOS contains system hardware configuration data, and this function allows access to extended areas beyond the standard 128 bytes.

#### Remarks
The standard CMOS area is often limited to 128 bytes (addresses 0x00 to 0x7F), but some systems extend this area to allow more configuration data storage. This function allows reading from the extended CMOS, useful for systems with more complex configurations.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%dx` | Address in extended CMOS to read. |

#### Returns
| Register | Description |
| --- | --- |
| `%al` | Data read from the extended CMOS address. |

#### Example
This example reads a byte from extended CMOS address `0x80`.

```asm
movb $0x0B, %ah     # Set function code for 'Read Extended CMOS Data'
movw $0x80, %dx     # Set extended CMOS address to 0x80
int $0x1A           # Call BIOS real-time clock service
# The byte at extended CMOS address 0x80 is stored in %al
```

---

### Write Extended CMOS Data (INT 0x1A - Function 0x0C)

This function writes data to the extended CMOS area. The extended CMOS is used for storing additional system configuration information.

#### Remarks
When writing to the extended CMOS, ensure that the correct address is being accessed, as incorrect values can lead to system misconfiguration. Be careful when modifying CMOS data, as it affects system hardware settings.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%dx` | Address in extended CMOS to write. |
| `%al` | Data byte to be written to the specified CMOS address. |

#### Returns
There are no return values.

#### Example
This example writes a byte (`0xAA`) to extended CMOS address `0x80`.

```asm
movb $0x0C, %ah     # Set function code for 'Write Extended CMOS Data'
movw $0x80, %dx     # Set extended CMOS address to 0x80
movb $0xAA, %al     # Set data byte to 0xAA
int $0x1A           # Call BIOS real-time clock service
# The byte 0xAA is written to extended CMOS address 0x80
```


## INT 0x19 - Boot Services

| Function | Description |
| --- | --- |
| `0x00` | Boot the System (Cold Boot) |

---

### Boot the System (Cold Boot)

This function initiates a system boot by jumping to the BIOS bootstrap loader. The bootstrap loader will attempt to boot from the configured boot devices in the order specified in the BIOS settings (typically floppy, hard disk, CD-ROM, or network).

#### Remarks
This function performs a cold boot (full system reset), effectively restarting the entire system. It is equivalent to pressing the reset button or powering the system off and back on. Any unsaved data or program state will be lost. Use this function with caution, as it will not return control to the calling program.

#### Parameters
There are no input parameters for this function.

#### Returns
This function does not return, as the system is restarted immediately.

#### Example
This example triggers a cold boot, restarting the entire system.

```asm
int $0x19           # Call BIOS boot service
# The system reboots, and control is transferred to the BIOS bootstrap loader
```


## Miscellaneous Services

| Function | Description |
| --- | --- |
| `INT 0x1A - Function 0x06` | Set RTC Alarm |
| `INT 0x1A - Function 0x07` | Reset RTC Alarm |
| `INT 0x1A - Function 0x08` | Read RTC Status |
| `INT 0x1A - Function 0x09` | Set RTC Interrupt Mask |
| `INT 0x05` | Print Screen |
| `INT 0x18` | Execute ROM BASIC (Cassette Services) |
| `INT 0x19` | Bootstrap Loader |

---

### Set RTC Alarm (INT 0x1A - Function 0x06)

This function sets the Real-Time Clock (RTC) alarm, allowing the system to wake up or trigger an interrupt at a specified time.

#### Remarks
This function allows setting a wake-up time or alert based on the RTC. It uses Binary-Coded Decimal (BCD) values for hours, minutes, and seconds. Ensure that the RTC interrupt system is properly configured for the alarm to trigger correctly.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x06` to set RTC alarm. |
| `%ch` | Alarm hour (BCD format). |
| `%cl` | Alarm minute (BCD format). |
| `%dh` | Alarm second (BCD format). |

#### Returns
There are no return values.

#### Example
This example sets an alarm for 08:30:00.

```asm
movb $0x06, %ah     # Set function code for 'Set RTC Alarm'
movb $0x08, %ch     # 08 hours (BCD format)
movb $0x30, %cl     # 30 minutes (BCD format)
movb $0x00, %dh     # 00 seconds (BCD format)
int $0x1A           # Call BIOS real-time clock service
# RTC alarm is set for 08:30:00
```

---

### Reset RTC Alarm (INT 0x1A - Function 0x07)

This function disables the currently set RTC alarm.

#### Remarks
Once the RTC alarm has been reset, no further interrupts will be triggered by the alarm until it is set again.

#### Parameters
There are no input parameters.

#### Returns
There are no return values.

#### Example
This example disables any currently set RTC alarm.

```asm
movb $0x07, %ah     # Set function code for 'Reset RTC Alarm'
int $0x1A           # Call BIOS real-time clock service
# RTC alarm is now disabled
```

---

### Read RTC Status (INT 0x1A - Function 0x08)

This function reads the status of the Real-Time Clock (RTC), including whether the RTC is operational and if there is an alarm set.

#### Remarks
The status provides information on whether the RTC is functioning correctly, and whether the alarm system is active. It is useful for diagnosing issues with the RTC or confirming that the alarm was set correctly.

#### Parameters
There are no input parameters.

#### Returns
| Register | Description |
| --- | --- |
| `%al` | RTC status flags (bitmask). |

##### RTC Status Flags
| Bit | Description |
| --- | --- |
| `0` | RTC operational (1 = running, 0 = stopped). |
| `1` | RTC alarm is set. |

#### Example
This example checks if the RTC is operational and if an alarm is set.

```asm
movb $0x08, %ah     # Set function code for 'Read RTC Status'
int $0x1A           # Call BIOS real-time clock service
testb $0x01, %al    # Test if bit 0 is set (RTC operational)
jnz rtc_running     # Jump if RTC is running
rtc_running:
# RTC is operational
```

---

### Set RTC Interrupt Mask (INT 0x1A - Function 0x09)

This function sets the mask for RTC interrupts, enabling or disabling specific interrupt events such as periodic interrupts or alarm interrupts.

#### Remarks
This function allows enabling or disabling specific types of RTC-generated interrupts. Proper use of this function is essential for managing power and alarm events.

#### Parameters
| Register | Parameter description |
| --- | --- |
| `%ah` | Function number, `0x09` to set RTC interrupt mask. |
| `%al` | RTC interrupt mask (bitmask). |

##### Interrupt Mask Bits
| Bit | Description |
| --- | --- |
| `0` | Enable periodic interrupts. |
| `1` | Enable alarm interrupts. |
| `2` | Enable update-ended interrupts. |

#### Returns
There are no return values.

#### Example
This example enables alarm interrupts on the RTC.

```asm
movb $0x09, %ah     # Set function code for 'Set RTC Interrupt Mask'
movb $0x02, %al     # Enable alarm interrupts (bit 1)
int $0x1A           # Call BIOS real-time clock service
# RTC alarm interrupts are now enabled
```

---

### Print Screen (INT 0x05)

This function captures the current contents of the screen (in text mode) and sends it to the printer connected to the first parallel port (LPT1).

#### Remarks
This function only works in text mode and prints the contents of the entire screen. It does not work in graphics modes. Make sure a printer is connected and online, or the function may hang.

#### Parameters
There are no input parameters.

#### Returns
There are no return values.

#### Example
This example captures the screen and sends it to the printer.

```asm
int $0x05           # Call BIOS print screen service
# The current screen content is sent to the printer
```

---

### Execute ROM BASIC (Cassette Services) (INT 0x18)

This function transfers control to the ROM BASIC interpreter. It was used in older systems to load BASIC directly from ROM, typically used when no bootable media was found.

#### Remarks
This function is mostly obsolete and is unlikely to be found on modern systems. It was used in early PCs that had a built-in BASIC interpreter stored in ROM.

#### Parameters
There are no input parameters.

#### Returns
There are no return values, as control is passed to the ROM BASIC interpreter.

#### Example
This example transfers control to the ROM BASIC interpreter.

```asm
int $0x18           # Call BIOS ROM BASIC service
# Control is transferred to the ROM BASIC interpreter
```

---

### Bootstrap Loader (INT 0x19)

This function executes the bootstrap loader, similar to the cold boot function. It attempts to boot the system from the configured boot devices.

#### Remarks
This function initiates a boot process and transfers control to the system's bootloader (typically located on the first bootable device). It is functionally equivalent to triggering a reboot through INT 0x19.

#### Parameters
There are no input parameters.

#### Returns
This function does not return, as control is transferred to the bootstrap loader.

#### Example
This example calls the bootstrap loader.

```asm
int $0x19           # Call BIOS bootstrap loader service
# The system boots from the configured boot device
```

## INT 0x1E - Diskette Parameter Table Services

| Function | Description |
| --- | --- |
| `0x00` | Get Diskette Drive Parameters (using Diskette Parameter Table) |

---

### Get Diskette Drive Parameters (using Diskette Parameter Table)

This function retrieves the diskette drive parameters stored in the BIOS Diskette Parameter Table (DPT). It provides information such as the number of sectors per track, gap lengths, and data rates. These parameters are essential for proper communication with diskette drives.

#### Remarks
The Diskette Parameter Table is typically located at memory address `0x0000:0x0078` in the BIOS Data Area (BDA). It holds critical information for diskette operations, and this function provides a way to read those values.

The parameters returned by this function are specific to the type of diskette drive and media being used (e.g., 3.5" 1.44MB, 5.25" 1.2MB).

#### Parameters
There are no input parameters.

#### Returns
| Register | Description |
| --- | --- |
| `%es:%bx` | Pointer to the Diskette Parameter Table (DPT). |

##### Diskette Parameter Table (DPT) Layout
| Offset | Description |
| --- | --- |
| `+0x00` | Step rate (for stepping between tracks). |
| `+0x01` | Head unload time. |
| `+0x02` | Head load time. |
| `+0x03` | Motor on delay time (in clock ticks). |
| `+0x04` | Bytes per sector (typically `512`). |
| `+0x05` | Sectors per track (number of sectors per disk track). |
| `+0x06` | Gap length for read/write operations. |
| `+0x07` | Data length (for variable-length sectors). |
| `+0x08` | Format gap length (gap between sectors when formatting). |
| `+0x09` | Fill byte used when formatting. |
| `+0x0A` | Head settle time. |
| `+0x0B` | Motor start delay time. |

#### Example
This example retrieves the Diskette Parameter Table and accesses its contents.

```asm
int $0x1E           # Call BIOS diskette parameter table service
movw %es, %ax       # Store the segment of the Diskette Parameter Table
movw %bx, %di       # Store the offset of the Diskette Parameter Table
# The Diskette Parameter Table is now accessible at %es:%di
```

## INT 0x18 - Cassette Services

| Function | Description |
| --- | --- |
| `0x00` | Load and Execute ROM BASIC |

---

### Load and Execute ROM BASIC (Cassette Services)

This function attempts to load and execute the ROM BASIC interpreter, a feature found in early IBM PC systems. When the BIOS is unable to find a bootable device (such as a floppy or hard drive), it would fall back on this function to execute BASIC from ROM.

#### Remarks
ROM BASIC is obsolete and is not present in modern systems. Early IBM PCs had BASIC embedded in ROM for users who did not have any bootable media. Calling this function effectively hands control over to the ROM BASIC interpreter, and the system will no longer return control to the original program.

Modern systems no longer include ROM BASIC, so invoking this function will have no effect on newer machines.

#### Parameters
There are no input parameters.

#### Returns
This function does not return, as control is transferred to the ROM BASIC interpreter.

#### Example
This example demonstrates invoking ROM BASIC.

```asm
int $0x18           # Call BIOS cassette services to load ROM BASIC
# Control is transferred to the ROM BASIC interpreter (on early systems)
```


