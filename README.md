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

To build the bootable floppy (kickstart image), you first need to compile the custom toolchain.  
A Makefile in the `Tools` directory automates this process. Increasing the number of parallel build jobs can drastically reduce the time needed to build the toolchain, so I highly recommend it. The GNU toolchain is not particularly small, and this tiny tweak can mean a difference between a cigarette break and a leisurely dinner. However, it is not strictly necessary.

```makefile
# File: Tools/Makefile

# Configure the amount of parallel jobs here
export MAKEFLAGS += -j4

# For example:
# export MAKEFLAGS += -j12
```

To start building, fire up a terminal, switch to the `Tools` directory and run `make`.

```bash
cd Tools
make
```

This will take a while, so feel free to grab a coffee.
After the toolchain is built, you can create and run the floppy image with QEMU from within the Source directory.

```bash
cd Source
make run-qemu
```

## Documentation
If you want to tinker with system yourself, I strongly recommended you give the [handbook](./Docs/Manual/README.md) a try. It's not always 100% complete, but I do my best to keep it up to date. 


## Contributors
- Special thanks to Damien Guard who generously allowed me to use some of his handmade bitmap fonts. Check out [his website](https://damieng.com/), where he provides lots of cool resources.
- David Siñuela Pastor and the other contributors of the [MinUnit](https://github.com/siu/minunit) project, a simple C unit testing framework in a single header file.

