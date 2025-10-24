# Address Ranges
For better understanding, this page lists the standardized address ranges of x86 systems. None of this has been specified by the free86 project, but should be interpreted as a given.

## Lower memory
The address ranges described below were defined in the original IBM PC specification and ensure compatibility with the Intel 8086/8088. Since every x86 PC starts in real mode after reset, they should be considered a given and shall not be used for other purposes. In Real Mode, the CPU can use an address space of 1 MiB total called _lower memory_. 

Although it is possible to read from these ranges, it is generally not a good idea to write to them - if this is even possible. An exception to this is, of course, the address ranges for video RAM and conventional memory (the range from 0x00500 to 0x9ffff which is the 640 KiB base memory).

### RAM address ranges

| Address           | Area name              | Purpose                                        |
|-------------------|------------------------|------------------------------------------------|
| 0x00000 - 0x003ff | Interrupt vector table | 256 * 4 bytes = 1 KiB BIOS interrupts          |
| 0x00400 - 0x004ff | BIOS data area         | Drive counter, keyboard and timer status, etc. |
| 0x00500 - 0x9ffff | Conventional RAM       | **Free**                                       |

### ROM / IO mappings

| Address           | Area name               | Purpose                                        |
|-------------------|-------------------------|------------------------------------------------|
| 0xa0000 - 0xaffff | Planar video memory     | 64 KiB VRAM for planar graphics modes          |
| 0xb0000 - 0xb7fff | Monochrome video memory | VRAM for MDA or hercules cards                 |
| 0xb8000 - 0xbffff | Text mode video memory  | VRAM for colored VGA/EGA 80x25 text mode       |
| 0xc0000 - 0xc7fff | VGA ROM                 | VGA BIOS code                                  |
| 0xc8000 - 0xeffff | Option ROMs             | BIOS code for SCSI, network, etc.              |
| 0xf0000 - 0xfffff | System BIOS / ROMs      | 64 KiB BIOS code / shadow area                 |


## Upper memory
The memory area above 1 MiB is referred to as _extended memory_ and can be used from the Intel 80286 onwards. How it is actually used depends heavily on the operating system design and the kernel. Newer processors map PCI devices and similar components to the upper address range of extended memory, but these are not yet available on the 80386SX. Note, that the upper memory is only available in protected mode and requires the A20 gate to be enabled.

| Address                 | Area name              | Purpose         |
|-------------------------|------------------------|-----------------|
| 0x00100000 - 0xffffff   | Extended memory        | **Free**        |
| 0xc0000000 - 0xdfffffff | Memory mapped IO / PCI | **Unreachable** |
| 0xe0000000 - 0xffffffff | ROM, BIOS shadow       | **Unreachable** |



