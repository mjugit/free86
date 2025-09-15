# The free86 Operating System

Welcome to the _free86_ Project repository! _free86_ is a handcrafted open source operating system for the x86 architecture. This is a hobby project of mine; I primarily target my vintage PC (specs below), but all x86 CPUs after the 80286 should work fine. You can build a bootable floppy image of the system and try it in a VM or on physical hardware.  

### Target machine

The system is tested on a vintage PC (1990) with the following specs:
- **30386SX CPU** (1988, @16 MHz, 16 MiB address range)
- **640KiB** low memory + **4MiB** extended memory
- **1 Floppy Drive** 3.5", 1.44MiB per disk
- **IDE HDD** 128MiB

### Minimum system requirements

- A **80286 CPU**, since it is the first x86 CPU, that supports _protected mode_.
- The **640KiB** onboard memory are enough at the moment.
- **VGA** support; we use the 640x480 planar mode with 16 active colors and a 256 color palette.


## Getting Started

At the moment the system's booting from floppy. To build the kickstart image, you first need to compile the custom toolchain, which is a gcc and binutils build for 32-bit x86 processors. 

A Makefile in the `Tools` directory automates this process. Increasing the number of parallel build jobs (config file is `Tools/build-config.mk`) can reduce build time, but the default configuration should work on any system.

```bash
cd Tools
make
```

After the toolchain is built, you can create and run the floppy image with QEMU from within the Source directory.

```bash
cd Source
make run-qemu
```

## Documentation

If you want to tinker with system yourself, it is strongly recommended to look into the [handbook](./Docs/Manual/README.md).


## Contributors
- Special thanks to **Damien Guard** who generously allowed me to use some of his handmade bitmap fonts. Check out [his website](https://damieng.com), where he provides lots of cool resources.
