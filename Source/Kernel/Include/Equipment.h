/*
	
  Copyright © 2025 Maximilian Jung

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the “Software”), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
*/


#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

#include "../../Modules/Include/SystemCore.h"


module(Equipment) {

  // Get the total amount of usable bytes in the low memory area (in KiB)
  U16 (*GetLowMemorySize)(void);

  // Get the total amount of usable bytes in the high memory area (in KiB)
  U16 (*GetHighMemorySize)(void);

  // Get the amount of installed floppy drives
  U16 (*GetFloppyDriveCount)(void);

  // Get the amount of installed RS232 interfaces
  U16 (*GetRs232InterfaceCount)(void);

  // Get whether a FPU (floating point unit) is installed
  bool (*GetFpuInstalled)(void);
  
};

#endif
