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

