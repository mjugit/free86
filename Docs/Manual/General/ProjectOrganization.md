# Project organization

When working on a system-level project like free86, it quickly becomes apparent that organization is not just a matter of convenience but of necessity. The complexity of an operating system grows fast, and small decisions in naming, file structure, or build process can have a big impact later on. The following notes are based on practical experience and may help you avoid some of the pitfalls.


## Function naming and file structure

A common mistake in the beginning is to rely on long prefixes and large compilation units to keep things organized (e.g., `MyLib_FunctionName(...)`). In practice, this leads to very large source files that become difficult to navigate and maintain.

A more effective approach is to place each function in its own source file together with its internal helpers. This makes it clear where to look for an implementation and avoids endless scrolling.

Instead of prefixing, it is also beneficial to use namespace-style interfaces: constant structs with function pointers that are defined once and configured centrally. This makes it easy to swap implementations, keeps the code modular, and allows access in an object-oriented style such as `MyLib.ExampleFunction1(...)`. Most IDEs will also provide proper auto-completion support for such constructs.

_Example:_

```c
MyType* MyLib_ExampleFunction1(void *location, U32 size);
void*   MyLib_ExampleFunction2(MyType* obj, U32 count);
void    MyLib_ExampleFunction3(MyType* obj, void* ptr);

const struct MyLibNamespace MyLib = {
  .ExampleFunction1 = MyLib_ExampleFunction1,
  .ExampleFunction2 = MyLib_ExampleFunction2,
  .ExampleFunction3 = MyLib_ExampleFunction3
};
```


## Static libraries vs. module compilation

It is tempting to rely on statically linked libraries for coordination of the build process, especially if you come from application development. However, when writing an operating system, this approach often causes problems: some functions or variables may not be linked, and the resulting errors can be extremely hard to track down.

A more robust solution is to collect all related function objects into a single _module file_. Each module explicitly includes its component implementations and can also integrate assembler objects at the same stage. Since the compiler and linker work at the object level, this guarantees that the module contains everything that belongs to it.

_Example:_

```c
// MyLib.Module.c
#include "MyLib_ExampleFunction1.c"
#include "MyLib_ExampleFunction2.c"
#include "MyLib_ExampleFunction3.c"
```

```makefile
# Makefile
MyLib.Module.o: MyLib.Module.c MyLib_LowLevel.asm
    $(CC) -c MyLib.Module.c -o MyLib.Module.o
    $(AS) MyLib_LowLevel.s -o MyLib_LowLevel.o
    $(LD) -r -o MyLib.o MyLib.Module.o MyLib_LowLevel.o
```


## Folder layout

A clean folder structure further reduces friction. It has proven practical to place all source files of a given module or library in one directory while keeping all headers in a separate `Include` folder. This makes it easier to review and include headers from a central place without having to navigate through multiple subdirectories.

_Example:_

```
Include/
 └── MyLib.h
MyLib/
 ├── MyLib.Module.c
 ├── MyLib_ExampleFunction1.c
 ├── MyLib_ExampleFunction2.c
 └── MyLib_ExampleFunction3.c
 ```
 
 
