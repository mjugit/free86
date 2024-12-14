# free86

Welcome to the **free86** repository! 
**free86** is a custom operating system for 80386 vintage systems. As the focus is on learning and finding your own solutions, the system is not based on any existing standards and does not aim to be compatible with other systems. 

## Getting started
If you want to try it out, you can build the floppies yourself and run them with QEMU. Simply follow the steps below.

1. To create a build for the target architecture, a **custom toolchain for 80836** is required. There is a Makefile that automates the download and build process. Switch to the `Tools` directory and run `make`.
2. Once you've obtained your toolchain, switch to the `Source` directory. The Makefile in this directory does the actual build. Run `make qemu` to create the floppies and load them with the emulator.


## Files & folders
A quick look at the most important places within the repository:

- `Build` contains all files created during the build process, including the `Kickstart.img` (system startup floppy).
- `Source` contains the acutal source files and the `Makefile` for the system floppies.
- `Tools` contains the toolchain and its build scripts.


## Target hardware
Builds are regularly tested on a 1990s PC besides QEMU (emulator, supports GDB remote debugging).

- **CPU**: Intel 80386SX @ 16MHz  
  Released at the 16. June 1988. The SX is a 32-bit chip internally, but only has a 16-bit path to the system memory. With 24 pins connected to the address bus, the CPU can address 16MiB in total.
- **Graphics**: Trident TVGA9000B VGA ISA card  
This VGA card is based on the Trident TVGA9000B chipset, supports standard VGA resolutions and color depths, such as 320x200 with 256 colors (Mode 13h), as well as higher resolutions like 640x480 in 16 colors.
- **Memory**: 1MB onboard
- **Storage**: IDE HDD with 128 MB and a 3.5" 1.44 MB floppy disk drive 
- **I/O**: Floppy, IDE, parallel and serial support


## License
This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
