# IDT Module
The interrupt descriptor table (IDT) defines how the processor dispatches interrupts and exceptions to their corresponding handlers. It maps each interrupt vector (0–255) to a 32-bit descriptor that contains information about the handler routine, code segment, and access privileges.

## Introduction
On x86 systems, the IDT is a key data structure for controlling interrupt flow. It allows the kernel to define where each interrupt or exception should jump and under which privilege level it may be invoked. Each entry describes an interrupt or trap gate, its target address, and a segment selector in the global descriptor table (GDT).

During initialization, the kernel allocates a contiguous block of entries, fills them with valid gate descriptors, and finally loads the descriptor into the CPU using the `lidt` instruction. Once active, all hardware and software interrupts are dispatched through this table. 

## Using the module

```c
#include "Modules/Include/Interrupt.h"

use(Idt);
```

You can then call its functions directly on an IDT instance that you manage yourself. The module does not rely on global state, so multiple IDTs can be created and loaded independently if needed.

```c
IdtEntry idtTable[256];
IdtDescriptor idtDescriptor;

Idt.Initialize(idtTable, &idtDescriptor);
Idt.SetEntry(idtTable, 0x21, (U32)KeyboardHandler, 0x08, IdtGateTypeInt32, IdtPrivilegeKernel);
Idt.Load(idtTable, &idtDescriptor);
```

This defines a keyboard interrupt handler at vector 0x21 and activates the table.



## Function reference
Each of these functions directly manipulates the IDT in memory without assuming any global kernel state.

### `Initialize`
Initializes the IDT structure and prepares it for use. All entries are cleared, the descriptor is filled, and the table is loaded into the CPU.

```c
void Initialize(IdtEntry* idt, IdtDescriptor* descriptor);
```

| Parameter    | Description                             |
| ------------ | --------------------------------------- |
| `idt`        | Pointer to the array of IDT entries     |
| `descriptor` | Pointer to the IDT descriptor structure |


### `Load`
Loads the specified IDT and descriptor into the CPU using the lidt instruction. This replaces the currently active IDT.

```c
void Load(IdtEntry* idt, IdtDescriptor* descriptor);
```

| Parameter    | Description                            |
| ------------ | -------------------------------------- |
| `idt`        | Pointer to the IDT entries to activate |
| `descriptor` | Pointer to the descriptor to load      |


### `SetEntry`
Defines an interrupt gate in the IDT. The function automatically encodes the flags and writes the low- and high-address parts to the entry.

```c
void SetEntry(IdtEntry* idt,
              U8 index,
              U32 handlerAddress,
              U16 selector,
              IdtGateType gateType,
              IdtPrivilegeLevel privilegeLevel);
```

| Parameter        | Description                                                   |
| ---------------- | ------------------------------------------------------------- |
| `idt`            | Pointer to the IDT entries                                    |
| `index`          | The interrupt vector number (0–255)                           |
| `handlerAddress` | Linear address of the interrupt handler function              |
| `selector`       | Code segment selector within the GDT                          |
| `gateType`       | Type of gate (e.g. `IdtGateTypeInt32` or `IdtGateTypeTrap32`) |
| `privilegeLevel` | Minimum privilege level required to trigger the gate          |


### `EnableEntry`
Marks an entry as present and thereby activates it.

```c
void EnableEntry(IdtEntry* idt, U8 index);
```

| Parameter | Description                    |
| --------- | ------------------------------ |
| `idt`     | Pointer to the IDT entries     |
| `index`   | The interrupt vector to enable |


### `DisableEntry`
Marks an entry as not present, effectively disabling the interrupt vector.

```c
void DisableEntry(IdtEntry* idt, U8 index);
```

| Parameter | Description                     |
| --------- | ------------------------------- |
| `idt`     | Pointer to the IDT entries      |
| `index`   | The interrupt vector to disable |


