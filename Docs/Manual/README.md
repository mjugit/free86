# Free86 development handbook

This manual is intended to help other programmers who want to participate in the free86 project or pursue their own development goals get started. Even if you are an experienced C programmer and have Assembly -language skills, there are some pitfalls to be aware of that are not so obvious. To avoid frustration and long debugging sessions, here are some explanations that may help clarify things. Especially when it comes to system programming, you may need to take a different approach than you are used to. Please take the time to read up a little before you start your project.

## General stuff
- [Project organization](./General/ProjectOrganization.md) A few important notes about code structuring, the use of statically linked libraries, and naming things.
- [Module definition macros](./General/ModuleDefinition.md) A guide on how to define and use code modules (or namespaces). There are a few macros that help you to keep your code structured.


## Code documentation

### Essentials
- [SystemCore](./CodeDocs/SystemCore.md) The core module of the system, which provides the most important types and low-level operations.
- [Bitmap Module](./CodeDocs/BitmapModule.md) Provides functions for editing and evaluating bitmaps.
- [Memory Module](./CodeDocs/MemoryModule.md) Provides functions for low level memory manipulation and evaluation.

