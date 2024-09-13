# free86: Documentation

Welcome to the **free86** documentation!


## Guides
- [**Memory Segmentation**](./guide/memory-segmentation.md):  
  Memory segmentation is a fundamental part of how the Intel 80386SX organizes and accesses memory.
- [**Working with physical Floppy Disks**](./guide/prepare-floppy.md):  
  This guide provides an overview of how to work with floppy disks using modern USB floppy drives on contemporary systems. You will learn how to identify and format floppy disks at a low level, prepare them for use without a file system, and write raw data directly to specific sectors.

## BIOS
- [**BIOS Function Reference**](./bios/bios-reference.md):

## CPU

- [**Intel 80386SX**](./hardware/386sx.md):  
  A technical overview and register reference of the Intel 80386SX processor describing its features, flags, limits and capabilities.

### Assembler Instructions

Understanding x86 assembler instructions is crucial for low-level programming, such as operating system development or embedded system projects. This section categorizes instructions into various types, each serving a different purpose in the CPU's operation. Follow the links to learn more about each instruction set.

- [**Data Transfer**](./assembler/as-data.md):  
  Data transfer instructions move data between registers, memory, and I/O ports. Commonly used commands include `MOV`, `PUSH`, and `POP`. These instructions are essential for almost every assembly program, as they allow manipulation of the CPU’s data flow.

- [**Arithmetic**](./assembler/as-arithmetic.md):  
  Arithmetic instructions perform mathematical operations such as addition, subtraction, multiplication, and division. These commands, like `ADD`, `SUB`, and `IMUL`, are fundamental in manipulating numeric values and performing calculations at the assembly level.

- [**Logical**](./assembler/as-logic.md):  
  Logical instructions handle bitwise operations such as AND, OR, XOR, and NOT. These instructions are used to manipulate bits within registers or memory and are often combined with other instructions to implement more complex decision-making structures.

- [**Shift**](./assembler/as-shift.md):  
  Shift and rotate instructions, such as `SHL`, `SHR`, and `ROL`, move bits within registers left or right. These operations are commonly used for efficient arithmetic or bitwise manipulations, especially when dealing with binary data.

- [**Control Transfer**](./assembler/as-control-transfer.md):  
  Control transfer instructions manage the flow of execution by jumping to different sections of code or calling subroutines. Key instructions include `JMP`, `CALL`, `RET`, and conditional jumps like `JE` and `JNE`, which are essential for implementing loops and branching logic.

- [**String**](./assembler/as-string.md):  
  String manipulation instructions, such as `MOVSB`, `CMPSB`, and `SCASB`, allow for efficient handling of arrays of data (often bytes or words). These commands are useful for operations on buffers, such as copying, scanning, or comparing blocks of data.

- [**Flag Control**](./assembler/as-flag-control.md):  
  Flag control instructions manipulate specific flags in the CPU’s EFLAGS register, which affects how the processor behaves during operations. Instructions like `STC`, `CLC`, and `STD` allow control over flags such as the carry and direction flags, which are vital for certain arithmetic and string operations.

- [**System**](./assembler/as-system.md):  
  System instructions interact with the CPU at a low level, enabling advanced control over system states. Commands like `HLT`, `CLI`, and `STI` handle operations such as halting the CPU, enabling/disabling interrupts, or managing processor modes like paging.

- [**Coprocessor and FPU**](./assembler/as-coprocessor.md):  
  Coprocessor instructions manage the interaction with the floating-point unit (FPU) or other specialized hardware. Instructions like `FADD`, `FMUL`, and `FINIT` allow arithmetic operations on floating-point numbers, making them crucial for mathematical computations.
