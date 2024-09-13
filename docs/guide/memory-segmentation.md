# Memory Segmentation on the Intel 80386SX

Memory segmentation is a fundamental part of how the Intel 80386SX organizes and accesses memory. It divides memory into distinct sections, called **segments**, which can represent code, data, or the stack. Understanding segmentation is critical for programming this processor, especially when working in **real mode** and **protected mode**. In this guide, we’ll explore how segmentation works, how addresses are calculated, and provide practical examples of working with segments in assembly language.

## Overview of Segmentation

On the 80386SX, memory segmentation is managed through six **segment registers**:

- **CS**: Code Segment
- **DS**: Data Segment
- **SS**: Stack Segment
- **ES**, **FS**, **GS**: Extra Segments, typically used for additional data.

Each segment register holds a **segment base address**, which points to the beginning of a memory segment. To access memory within a segment, this base address is combined with an **offset**, forming the **effective memory address**.

### Segment:Offset Addressing

In the segment:offset model, the effective memory address is calculated by combining the base address in the segment register with an offset. This offset can be a constant value or stored in a general-purpose register like `EAX`.

```
Effective Address = Segment Base Address + Offset
```

- **Segment register** holds the base address.
- **Offset** specifies the location within the segment.

### Example in Real Mode

In **real mode**, the base address of a segment is calculated by multiplying the 16-bit segment value by 16 (or shifting it left by 4 bits). This gives a maximum segment size of 64 KiB.

For example:
- **CS (Code Segment)** = `0x1000`
- **IP (Instruction Pointer)** = `0x0010`

The effective address would be:
```
Effective Address = (0x1000 * 16) + 0x0010 = 0x10000 + 0x0010 = 0x10010
```

In real mode, the processor can address up to **1 MiB** of memory using 20-bit addresses.

## Segmentation in Protected Mode

In **protected mode**, segmentation becomes more powerful and flexible. Instead of just storing a base address in the segment registers, the registers contain **selectors** that point to **descriptors** in the **Global Descriptor Table (GDT)** or **Local Descriptor Table (LDT)**. Each descriptor provides:

- **Base address**: Starting address of the segment.
- **Limit**: The size of the segment, up to 4 GiB.
- **Access rights**: Defines the type of segment (code, data, stack) and privilege levels.

In protected mode, you can work with 32-bit base addresses, allowing segments to be much larger than in real mode.

### Example in Protected Mode

For example:
- **CS (Code Segment)** = `0x0001` (a selector pointing to a descriptor in the GDT).
- The descriptor in the GDT for `CS = 0x0001` has a base address of `0x00400000`.
- **EIP (Instruction Pointer)** = `0x00000010`

The effective address is:
```
Effective Address = 0x00400000 + 0x00000010 = 0x00400010
```

Protected mode uses 32-bit addressing, which allows access to a much larger memory space than real mode.

## Examples of Segmentation in Assembly

Here are practical examples of using segmentation in assembly language with **GNU Assembler (AT&T syntax)**.

### Setting Up Segments in Real Mode

In real mode, segment registers are directly set with values, and addresses are calculated using the `segment:offset` model.

```asm
movw $0x1000, %ax      # Load 0x1000 into AX
movw %ax, %ds          # Move AX into DS (set DS = 0x1000)
movw $0x0010, %bx      # Load 0x0010 into BX (offset)

movw %bx, %ax          # Access memory at DS:BX (0x10000 + 0x0010)
movw %ds:(%bx), %ax    # This accesses the memory address 0x10010
```

In this example, the memory at address `0x10010` is accessed by combining the base address in `DS` with the offset in `BX`.

### Working with Segments in Protected Mode

In protected mode, segment registers use selectors that reference descriptors in the GDT. The segment base address comes from the descriptor, and the offset is added to calculate the effective address.

```asm
movw $0x0010, %ax      # Load a selector (GDT entry 2) into AX
movw %ax, %ds          # Load the selector into DS (Data Segment)
movl $0x10, %ebx       # Set offset to 0x10
movl %ds:(%ebx), %eax  # Access memory at the base of DS + offset 0x10
```

In this case, the memory address is calculated using the base address from the GDT descriptor and the offset stored in `EBX`.

### Using Extra Segments (ES, FS, GS)

The extra segment registers (`ES`, `FS`, `GS`) are useful for handling additional data segments. Here’s how you can access memory using the `FS` segment register:

```asm
movw $0x0018, %ax      # Load a selector for FS
movw %ax, %fs          # Load FS with the selector value

movl %fs:(%ebx), %eax  # Access memory using FS as the segment base
```

In this example, `FS` points to a different data segment, allowing you to manage memory beyond the standard `DS` segment.

## Key Concepts and Considerations

### Segment Limits and Privilege Levels

Each segment in protected mode has a **limit** that determines its size. Accessing memory beyond this limit causes a **general protection fault**. Segments also have privilege levels, with **Ring 0** (kernel mode) being the most privileged, and **Ring 3** (user mode) being the least.

### Combining Segmentation with Paging

In protected mode, segmentation is often combined with paging. After calculating the **linear address** through segmentation, the paging unit translates it into a **physical address**. Paging allows dynamic memory mapping and virtual memory, adding flexibility to memory management.

### Descriptor Caching

Once a segment selector is loaded into a segment register, the descriptor information is cached by the CPU. This avoids repeated lookups in the GDT or LDT, improving performance when accessing frequently used segments.
