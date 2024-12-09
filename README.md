# free86
Welcome to the **free86** repository! This is an operating system development project for 80386 vintage systems, focusing on learning and creating a functional system from scratch.

## Target hardware & testing
Builds are regularly tested on a vintage 1990 PC (details below) - you can also use QEMU.

**Test machine specifications:**
- **CPU**: Intel 386SX @ 16 MHz  
Released at the 16. June 1988. The SX is a 32-bit chip internally, but only has a 16-bit path to the system memory. With 24 pins connected to the address bus, the CPU can address 16MiB in total.
- **RAM**: 1 MiB onboard  
As soon as we switch to protected mode, the first 637 KiB of the system memory are directly usable. Everything above to 1 MiB is reserved for special purposes (screen memory, etc.).
- **Graphics**: Trident TVGA9000B VGA ISA card  
This VGA card is based on the Trident TVGA9000B chipset, supports standard VGA resolutions and color depths, such as 320x200 with 256 colors (Mode 13h), as well as higher resolutions like 640x480 in 16 colors.
- **Storage**: IDE HDD with 128 MB and a 3.5" 1.44 MB floppy disk drive 
- **I/O**: Floppy, IDE, parallel and serial support


## Implementation progress

- ✔ Bootloader from scratch
- ✔ Protected mode
- ✔ Simple VGA video driver
- ✔ 640x480 resolution support
- ✔ Bitmap font support
- ✔ Keyboard interrupts
- ▶ Dynamic memory management
- ▶ C library (non ANSI)
- ❌ Floppy drive support
- ❌ Hard disk drive support
- ❌ System shell
- ❌ Scripting language
- ❌ Task switching
- ❌ File system

## License
This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
