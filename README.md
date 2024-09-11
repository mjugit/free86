# free86

**free86** is an operating system built from scratch, specifically designed to run on systems based on the Intel 386 architecture. The name "free86" is a nod to both the freedom of open-source software and the 386 processor series (3-86, you know - haha).

## Project Overview

The goal of free86 is to develop a simple and educational operating system that can run on 386-based PCs. This project is primarily focused on low-level programming, understanding hardware interaction, and creating a fully functional operating system from the ground up.

At the moment, we are in the very early stages of development. As the project progresses, we plan to implement a fully functioning kernel, drivers, file system, and a custom object-oriented scripting language, with the system running entirely in **Protected Mode**.

## Target Hardware

The main idea of this project is to learn. I (the maintainer) use a vintage 1990 PC with the hardware specifications below to test **free86** on a real machine. Since the x86 architecture is backwards compatible, it should also run on newer models, including contemporary hardware which allows legacy BIOS boots.

**Test machine specifications:**

- **CPU**: Intel 386SX @ 16 MHz  
This processor was initially released at the 16. June 1988. The SX is a 32-bit chip internally, but only has a 16-bit path to the system memory. With 24 pins connected to the address bus, the CPU can address 16MiB in total. More information about [the CPU can be found here](https://en.wikipedia.org/wiki/I386#80386SX).
- **RAM**: 1 MB onboard  
The machine includes 1 MB of onboard RAM, which can be expanded through standard SIMM (Single In-line Memory Module) slots. This limited amount of memory requires careful management. Memory allocation and paging will play a crucial role in optimizing the use of available resources.
- **Graphics**: Trident TVGA9000B VGA ISA card  
This VGA card is based on the Trident TVGA9000B chipset, a popular graphics card from the early 90s known for its compatibility and basic graphics capabilities. It supports standard VGA resolutions and color depths, such as 320x200 with 256 colors (Mode 13h), as well as higher resolutions like 640x480 in 16 colors. The card communicates with the system via the ISA (Industry Standard Architecture) bus, offering reasonable performance for text-based or basic graphical user interfaces. 
- **Storage**: IDE HDD with 128 MB and a 3.5" 1.44 MB floppy disk drive  
The system is equipped with a CompactFlash card IDE adapter acting as a hard drive, providing 128 MB of storage. This storage medium, combined with a 3.5-inch 1.44 MB floppy disk drive, reflects the limitations of vintage hardware. The IDE (Integrated Drive Electronics) interface is the primary method of communication between the storage devices and the system, while the floppy drive serves as a secondary, bootable medium. This setup will require custom drivers for both the IDE and floppy interfaces.
- **I/O**: Floppy, IDE, parallel and serial support  
The test machine includes standard I/O ports commonly found in vintage PCs. These ports include floppy and IDE interfaces for storage, a parallel port (often used for printers), and serial ports for communication devices like modems or mice. Developing drivers for these I/O interfaces will be crucial for enabling system functionality, such as reading from disks or interacting with external peripherals.


### Key Features (Planned)

- **Protected Mode kernel** designed for Intel 386SX processors
- **Custom bootloader** written from scratch
- **Basic drivers** for VGA, floppy, and IDE devices
- **UFS-like file system** to manage files and directories
- **Object-oriented scripting language** for system interaction
- **Open Source** and fully transparent development process

## Contributing

We welcome contributions from anyone who is interested in low-level operating system development, especially for legacy hardware. As this project is in its infancy, contributions could range from writing documentation and tutorials to implementing kernel modules or drivers.

Please see our [CONTRIBUTING.md](CONTRIBUTING.md) (coming soon) for details on how to get involved.

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.

## Contact

For any questions, suggestions, or contributions, feel free to open an issue or create a pull request.

---

*free86 is an educational project and is in no way intended for production use.*
