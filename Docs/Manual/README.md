# Free86 development handbook

This manual is intended to help other programmers who want to participate in the free86 project or pursue their own development goals get started. Even if you are an experienced C programmer and have Assembly -language skills, there are some pitfalls to be aware of that are not so obvious. To avoid frustration and long debugging sessions, here are some explanations that may help clarify things. Especially when it comes to system programming, you may need to take a different approach than you are used to. Please take the time to read up a little before you start your project.

## General stuff

### Project related things
- [Project organization](./General/ProjectOrganization.md) A few important notes about code structuring, the use of statically linked libraries, and naming things.
- [Module definition macros](./General/ModuleDefinition.md) A guide on how to define and use code modules (or namespaces). There are a few macros that help you to keep your code structured.

### Information on x86 systems
- [Address ranges](./AddressRanges.md) A summary of the standard addresses on x86 systems. This page contains important information about the address ranges for RAM, ROM, and memory-mapped I/O.


## Code documentation

### Essentials
- [SystemCore](./CodeDocs/SystemCore.md) The core module of the system, which provides the most important types and low-level operations.

### General modules
- [Bitmap module](./CodeDocs/BitmapModule.md) Provides functions for editing and evaluating bitmaps.
- [Heap module](./CodeDocs/HeapModule.md) Provides function for creating and managing dynamic memory areas.
- [Memory module](./CodeDocs/MemoryModule.md) Provides functions for low level memory manipulation and evaluation.
- [Stream module](./CodeDocs/StreamModule.md) Provides functions and types for working with data streams.
- [String module](./CodeDocs/StringModule.md) Simple string operations.


### Kernel modules

#### Low-Level kernel operations
- [Equipment](./CodeDocs/EquipmentModule.md) Provides information about the detected hardware.
- [Hardware IO](./CodeDocs/HardwareIO.md) Inline kernel functions for interacting with the hardware (base operations).
- [IDT module](./CodeDocs/IdtModule.md) Management functions for the Interrupt Descriptor Table (IDT).
- [PIC module](./CodeDocs/PicModule.md) Function module for interacting with the Programmable Interrupt Controller (PIC).


#### Driver modules
- [Keyboard module](#) TBD
- [Graphics module](#) TBD
- [Timer module](#) TBD




## Other files and folders
- All fonts in the kernel include directory have been generously provided by Damien Guard (the `Kernel/Include/Font_*.h` files). Check out [his website](https://damieng.com/), where he provides lots of cool resources.
