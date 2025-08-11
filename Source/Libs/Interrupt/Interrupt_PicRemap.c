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


#include "../Include/Interrupt.h"

// Initialization and control words
#define PicIcw1Init  0x10    // Initialize
#define PicIcw1Icw4  0x01    // Expect ICW4
#define PicIcw4_8086 0x01    // 8086/88 mode


// Remaps the 8259A PICs to the given vector bases
void Interrupt_PicRemap(U8 masterOffset, U8 slaveOffset) {
  // Save current masks
  U8 masterMask = PortReadByte(Interrupt.Pic.MasterDataPort);
  U8 slaveMask = PortReadByte(Interrupt.Pic.SlaveDataPort);

  // Start init sequence (cascade mode, expect ICW4)
  PortWriteByte(Interrupt.Pic.MasterCommandPort, PicIcw1Init | PicIcw1Icw4);
  PortWriteByte(Interrupt.Pic.SlaveCommandPort, PicIcw1Init | PicIcw1Icw4);

  // ICW2: Set new vector offsets
  PortWriteByte(Interrupt.Pic.MasterDataPort, masterOffset);
  PortWriteByte(Interrupt.Pic.SlaveDataPort, slaveOffset);

  // ICW3: Wiring information
  // Master has a Slave on IRQ2 -> bitmask 0b00000100
  // Slave identity/cascade input line is 2
  PortWriteByte(Interrupt.Pic.MasterDataPort, 0x04);
  PortWriteByte(Interrupt.Pic.SlaveDataPort,  0x02);

  // ICW4: 8086 mode
  PortWriteByte(Interrupt.Pic.MasterDataPort, PicIcw4_8086);
  PortWriteByte(Interrupt.Pic.SlaveDataPort,  PicIcw4_8086);

  // Restore masks
  PortWriteByte(Interrupt.Pic.MasterDataPort, masterMask);
  PortWriteByte(Interrupt.Pic.SlaveDataPort, slaveMask);
}
