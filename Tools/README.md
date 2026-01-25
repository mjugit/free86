# free86 development tools


## Toolchain requirements
The project uses a custom toolchain, cross-compiled for i386 from the GNU **gcc** and **binutils** source code. More details in the respective manual page (see the `Docs` directory). In the list below you can see the package dependencies for the build.

**IMPORTANT NOTE:** Even if you are missing a package or two, the build might start and run for a while until it encounters an error. **Make sure they are installed before you attempt to build the toolchain!** This might save you a lot of time.

To check the dependencies in advance, Arch Linux users can utilize the `check-deps-arch-linux.sh` script. Others will have to do it manually.

#### Package list

- **make** GNU make utility to maintain groups of programs. [[Arch Linux Package](https://archlinux.org/packages/core/x86_64/make/)] [[Upstream URL](https://www.gnu.org/software/make)]
- **bison** The GNU general-purpose parser generator. [[Arch Linux Package](https://archlinux.org/packages/core/x86_64/bison/)] [[Upstream URL](https://www.gnu.org/software/bison/bison.html)]
- **flex** A tool for generating text-scanning programs. [[Arch Linux Package](https://archlinux.org/packages/core/x86_64/flex/)] [[Upstream URL](https://github.com/westes/flex)]
- **mtools** A collection of utilities to access MS-DOS disks. [[Arch Linux Package](https://archlinux.org/packages/extra/x86_64/mtools/)] [[Upstream URL](https://www.gnu.org/software/mtools/)]
- **gmp** A free library for arbitrary precision arithmetic. [[Arch Linux Package](https://archlinux.org/packages/core/x86_64/gmp/)] [[Upstream URL](https://gmplib.org/)]
- **libmpc** Library for the arithmetic of complex numbers with arbitrarily high precision. [[Arch Linux Package](https://archlinux.org/packages/core/x86_64/libmpc/)] [[Upstream URL](http://www.multiprecision.org/)]
- **mpfr** Multiple-precision floating-point library. [[Arch Linux Package](https://archlinux.org/packages/core/x86_64/mpfr/)] [[Upstream URL](https://www.mpfr.org/)]
- **texinfo** GNU documentation system for on-line information and printed output. [[Arch Linux Package](https://archlinux.org/packages/core/x86_64/texinfo/)] [[Upstream URL](https://www.gnu.org/software/texinfo/)]
- **libisoburn** Frontend for libraries libburn & libisofs. __Might not longer be required! Needs review.__ [[Arch Linux Package](https://archlinux.org/packages/extra/x86_64/libisoburn/)] [[Upstream URL](https://dev.lovelyhq.com/libburnia/libisoburn)]


## Testing
Debugging and inspecting an operating system during operation is no easy task. On original hardware, it is virtually impossible to gain insight into the internals—at least not without custom code. So it makes sense to test the system in an emulator during development. 

I mainly develop with Emacs and other GNU tools, so I value good integration between my tools. When it comes to testing, I have had particularly good experiences with the combination of QEMU and GDB. A command line parameter can be used to instruct QEMU to open a debug port and wait for a GDB connection. Needless to say, GDB integrates seamlessly with Emacs.

I personally get along great with this setup, but of course your experience may be completely different. If you have other suggestions, please feel free to add them.

#### Debugging tools
- **GDB** The well known (and de facto standard) debugger of the GNU project. Although the first release lies more than 40 years back, it is widely supported and the basis for many IDEs. [[Project page](https://sourceware.org/gdb/download/)]
- **QEMU** QEMU is a generic and open source machine emulator and virtualizer. When using KVM, QEMU can virtualize x86, server and embedded PowerPC, 64-bit POWER, S390, 32-bit and 64-bit ARM, and MIPS guests. [[Project page](https://www.qemu.org/)]
- **Emacs** At its core is an interpreter for Emacs Lisp, a dialect of the Lisp programming language with extensions to support text editing. It is much more than a simple text editor or IDE. Imagine it as a whole ecosystem, that can be customized to your needs. [[Project page](https://www.gnu.org/software/emacs/)]

