# PIC module

The Programmable Interrupt Controller (PIC) coordinates hardware interrupts and forwards them to the CPU. On classic x86 systems, two cascaded 8259A controllers are used (master and slave) which together manage up to sixteen interrupt lines. The PIC module provides low-level access to these controllers, allowing the kernel to remap interrupt vectors, mask or unmask IRQ lines, and signal the completion of interrupt handling.

## Introduction
Each PIC translates hardware IRQs (Interrupt Requests) into interrupt vectors that the CPU dispatches through the IDT. By default, these vectors overlap with CPU exceptions (0x00–0x1F), so remapping is required during system initialization. The master PIC handles IRQ0–IRQ7, while the slave PIC handles IRQ8–IRQ15 and is connected to the master’s IRQ2 line. Each controller has its own mask register, allowing individual interrupt lines to be disabled or enabled dynamically.

This module abstracts the underlying I/O port operations and provides a clean interface for interrupt configuration and control. It does not maintain any global state and can be used in any environment that provides direct access to the I/O ports.


## Using the module
To use the PIC module in your code, include its header and import the module instance with the `use(...)` macro:

```c
#include "Modules/Include/Interrupt.h"
use(Pic);
```

The module exposes simple functions for remapping, masking, and acknowledging interrupts.

```c
// Example: Remap the PICs to start at interrupt vectors 0x20 and 0x28
Pic.Remap(0x20, 0x28);

// Mask all IRQs except for the timer (IRQ0) and keyboard (IRQ1)
Pic.SetFullMask(0xFFFC);

// Later, send EOI after handling a timer interrupt
Pic.SendEoi(0);
```

## Data structures and types
The following types define the layout and configuration of IDT entries as used by the processor. They are shared between the IDT and PIC modules since both operate within the same interrupt subsystem.

### `IdtEntry`
Represents a single entry in the interrupt descriptor table (IDT). Each entry describes one interrupt or exception handler and defines where control should be transferred when the corresponding interrupt occurs.

```c
typedef struct IdtEntry {
  U16 BaseAddressLow;   // Lower 16 bits of the handler address
  U16 Selector;         // Code segment selector in the GDT
  U8  __AlwaysZero;     // Must always be zero (reserved)
  U8  Flags;            // Type and access flags
  U16 BaseAddressHigh;  // Upper 16 bits of the handler address
} __attribute__((packed)) IdtEntry;
```


### `IdtDescriptor`
Defines the base address and size limit of the IDT. This structure is passed to the `lidt` instruction when loading the table.

```c
typedef struct IdtDescriptor {
  U16 Limit;        // Size of the table in bytes minus one
  U32 BaseAddress;  // Linear address of the first IdtEntry
} __attribute__((packed)) IdtDescriptor;
```

The `Limit` field determines how many entries are valid; the processor uses it to prevent out-of-bounds lookups when an interrupt vector is raised.



### `IdtPrivelegeLevel`
Specifies the minimum privilege level (ring) required to invoke a gate. The x86 architecture defines four rings:

```c
typedef enum {
  IdtPrivilegeKernel  = 0,  // Ring 0 — kernel mode
  IdtPrivilegeRing1   = 1,  // Rarely used
  IdtPrivilegeRing2   = 2,  // Rarely used
  IdtPrivilegeUser    = 3   // Ring 3 — user mode
} IdtPrivilegeLevel;
```

An interrupt with privilege level 3 can be triggered from user space, whereas level 0 is restricted to kernel code.



### `IdtGateType`
Defines the type of gate descriptor stored in each IDT entry. It determines how the CPU transfers control to the handler and which state information is preserved.

```c
typedef enum {
  IdtGateTypeTask16 = 0x5,  // 16-bit task gate
  IdtGateTypeInt16  = 0x6,  // 16-bit interrupt gate
  IdtGateTypeTrap16 = 0x7,  // 16-bit trap gate
  IdtGateTypeInt32  = 0xE,  // 32-bit interrupt gate
  IdtGateTypeTrap32 = 0xF   // 32-bit trap gate
} IdtGateType;
```

Interrupt gates automatically clear the interrupt flag (IF) on entry, preventing nested interrupts until explicitly re-enabled, while trap gates leave the flag unchanged. The 32-bit variants are used in protected mode, while the 16-bit ones remain for backward compatibility with real-mode and early 286 systems.



## Function reference

### `Remap`
Reinitializes both PICs and assigns new interrupt vector offsets. This avoids conflicts with CPU exception vectors.

```c
void Remap(U8 masterOffset, U8 slaveOffset);
```

| Parameter      | Description                                    |
| -------------- | ---------------------------------------------- |
| `masterOffset` | New interrupt vector offset for the master PIC |
| `slaveOffset`  | New interrupt vector offset for the slave PIC  |



### `SendEoi`
Signals the end of interrupt handling for a given IRQ number. For IRQs handled by the slave PIC, the function sends an EOI to both controllers in the correct order.

```c
void SendEoi(U8 irqNumber);
```

| Parameter   | Description                            |
| ----------- | -------------------------------------- |
| `irqNumber` | The IRQ number (0–15) that was handled |



### `SetFullMask`
Sets the entire 16-bit interrupt mask in one operation. Each bit corresponds to one IRQ line; setting a bit disables that interrupt.

```c
void SetFullMask(U16 mask);
```

| Parameter | Description                        |
| --------- | ---------------------------------- |
| `mask`    | 16-bit mask (bit set = IRQ masked) |



### `SetMask`
Disables a specific IRQ line by setting its mask bit.

```c
void SetMask(U8 irqLine);
```

| Parameter | Description                |
| --------- | -------------------------- |
| `irqLine` | IRQ line to disable (0–15) |



### `ClearMask`
Enables a specific IRQ line by clearing its mask bit.

```c
void ClearMask(U8 irqLine);
```

| Parameter | Description               |
| --------- | ------------------------- |
| `irqLine` | IRQ line to enable (0–15) |



### `GetMask`
Retrieves the combined 16-bit interrupt mask from both PICs.

```c
U16 GetMask(void);
```

| Returns | Description                                 |
| ------- | ------------------------------------------- |
| numeric | Combined interrupt mask of both controllers |


