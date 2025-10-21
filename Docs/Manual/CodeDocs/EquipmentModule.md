# Equipment module
The information provided by this module reflects the system state as detected during boot.

## Introduction
The equipment module provides access to basic hardware information reported by the BIOS. It is mainly used during system initialization to determine available memory, peripheral devices, and processor features.
The gathered data allows higher-level modules such as the memory manager or device drivers to adapt their behavior to the actual hardware configuration.


## Using the module
To access the hardware information in your code, include the header and import the module instance:

```c
#include "Modules/Include/Equipment.h"
use(Equipment);
```

Once imported, the module functions can be used directly:

```c
U16 lowMem = Equipment.GetLowMemorySize();
U16 floppies = Equipment.GetFloppyDriveCount();
bool hasFpu = Equipment.GetFpuInstalled();
```

The values returned by these functions correspond to the BIOS-reported configuration and are typically gathered using interrupt `0x11` (Equipment List) and related low-level calls.
This module is read-only; it does not alter any system state.


## Function parameters
All functions in the equipment module provide system-wide information and do not require parameters.


### `GetLowMemorySize`
Retrieve the total size of usable conventional memory below 1 MiB, as reported by the BIOS.

```c
U16 GetLowMemorySize(void);
```

| Returns    | Description                 |
| ---------- | --------------------------- |
| Size (KiB) | Usable low memory area size |



### `GetHighMemorySize`
Retrieve the total amount of extended memory above 1 MiB, as reported by the BIOS.

```c
U16 GetHighMemorySize(void);
```

| Returns    | Description                      |
| ---------- | -------------------------------- |
| Size (KiB) | Usable extended memory area size |



### `GetFloppyDriveCount`
Return the number of installed floppy drives.

```c
U16 GetFloppyDriveCount(void);
```

| Returns | Description                      |
| ------- | -------------------------------- |
| Count   | Number of floppy drives detected |


### `GetRs232InterfaceCount`
Return the number of RS-232 serial interfaces available in the system.

```c
U16 GetRs232InterfaceCount(void);
```

| Returns | Description                          |
| ------- | ------------------------------------ |
| Count   | Number of RS-232 interfaces detected |



### `GetFpuInstalled`
Check whether a floating-point unit (FPU) is present.

```c
bool GetFpuInstalled(void);
```

| Returns | Description      |
| ------- | ---------------- |
| `true`  | FPU detected     |
| `false` | No FPU installed |

