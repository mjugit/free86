# Module definition macros

One of the main goals in free86 is to keep the codebase modular and easy to navigate. To achieve this, we use a small set of macros that establish a simple but powerful pattern for building namespaces. These macros take away much of the repetitive boilerplate normally involved in defining and wiring up `structs` with function pointers. Instead, you get a consistent interface for each subsystem that looks and feels like a self-contained module.

The idea is straightforward:

1. You declare the _interface_ of a module in a header file.
2. You provide its _implementation_ in the corresponding source file.
3. Other code can then either _use_ the module or even _embed_ it into a larger one.

This creates a uniform style across the project and makes modules easy to swap or extend. In addition, a central location for configuring the actual implementation is created. If a function is replaced, only a single pointer needs to be adjusted.


## Declaring a module

A new module begins with the `module(...)` macro. It defines the shape of the interface – essentially a namespace containing function pointers or submodules. From the outside, it behaves like a global constant struct that you can call into.

For example, a library might be declared as follows:

```c
// MyLib.h

module(MyLib) {
  void (*ExampleFunction1)(void* obj, unsigned count);
  int  (*ExampleFunction2)(const void* obj);
};
```

This tells the compiler: “There will be a module called `MyLib` with these two functions.” Nothing is implemented yet, but the interface is fixed.


## Providing the implementation

The actual function bodies are written in a `.c` file as usual. Once they exist, the `members(...)` macro is used to connect them to the declared interface.

```c
// MyLib.Module.c

#include "MyLib.h"

void MyLib_ImplFunction1(void* obj, U16 count) { /* ... */ }
I32  MyLib_ImplFunction2(const void* obj) { /* ... */ }

members(MyLib) {
  .ExampleFunction1 = MyLib_ImplFunction1,
  .ExampleFunction2 = MyLib_ImplFunction2
};
```

This step “fills in” the namespace. After importing the module (as described below), you can now write:

```c
MyLib.ExampleFunction1(myObj, 42);
```


## Using a module
When another part of the project needs access to a module, there is no need to manually declare `extern` symbols. Instead, the `use(...)` macro provides a consistent way to import the module.

```c
// AnotherLib.c

use(MyLib);

void AnotherLib_DoSomething(void* obj) {
  if (MyLib.ExampleFunction2(obj)) {
    MyLib.ExampleFunction1(obj, 10);
  }
}
```

This makes it clear that `AnotherLib` depends on `MyLib`, and it keeps the code free of redundant declarations.


## Composing modules

Sometimes a larger system should be built from smaller building blocks. The `embed(...)` macro allows one module to include another directly, making the relationship explicit.

```c
// Graphics.h

module(Graphics, {
  embed(MyLib);
  void (*DrawLine)(int x0, int y0, int x1, int y1);
});
```

```c
// Graphics.Module.c
#include "Graphics.h"

use(MyLib);

void Graphics_DrawLineImpl(int x0, int y0, int x1, int y1) { /* ... */ }

members(Graphics, {
  .MyLib    = MyLib,
  .DrawLine = Graphics_DrawLineImpl
});
```

With this design, any code using `Graphics` automatically has access to both `Graphics.DrawLine(...)` and the functions from `Graphics.MyLib`. Please note that this is only an illustrative example and not advice for meaningful system design.

