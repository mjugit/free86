# The free86 Operating System

This repository contains the source code of the **free86** operating system and its development environment.  
You can build a bootable floppy image of the system targeting x86 processors.  

The system has been tested on a 30386SX (1988, @16 MHz, 16 MiB address range) and a 80686 virtual machine, so most x86 CPUs should be compatible.  
Note that this project does not aim to run binaries from existing operating systems.

## Getting Started

To build the bootable floppy (kickstart image), you first need to compile the custom toolchain.  
A Makefile in the `Tools` directory automates this process. Increasing the number of parallel build jobs can reduce build time, but the default configuration should work on any system.

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

For further details, please refer to the [handbook](./Docs/Manual/README.md).

