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


/*
  ICW1 - Initialization control word 1
  Bit 4 = 'Init sequence required'
  Bit 1 = '8086/88 mode'

  = Initialize both PICs in cascade mode for 8086/88 CPUs.
*/
#define ICW1 0b00010001


/*
  ICW2 - Initialization control word 2

  = Interrupt vector base (no magic number)
 */



/*
  ICW3 - Initialization control word 3

  ICW3_PIC1: 0x04
  Bit 2 = 'Slave connected to IRQ 2'

  ICW3_PIC2: 0x02
  Bit 1 = 'Connected via port 2'

  = Master receives interrupts from slave on IRQ2
 */
#define ICW3_PIC1 0b00000100
#define ICW3_PIC2 0b00000010


/*
  ICW4 - Initialization control word 4
  Bit 0 = '8086/88 mode (no auto-EOI, etc.)'

  = Typical protected mode interrupt handling
 */

#define ICW4 0b00000001



void _Pic_RemapImplementation(U8 masterOffset, U8 slaveOffset) {
  U8 masterMask = PortReadByte(PIC1_DATA);
  U8 slaveMask = PortReadByte(PIC2_DATA);

  // Begin init sequence
  PortWriteByte(PIC1_COMMAND, ICW1);
  PortWriteByte(PIC2_COMMAND, ICW1);

  // Define vector offsets
  PortWriteByte(PIC1_DATA, masterOffset);
  PortWriteByte(PIC2_DATA, slaveOffset);

  // Setup cascade wiring
  PortWriteByte(PIC1_DATA, ICW3_PIC1);
  PortWriteByte(PIC2_DATA, ICW3_PIC2);

  // Set mode
  PortWriteByte(PIC1_DATA, ICW4);
  PortWriteByte(PIC2_DATA, ICW4);

  // Restore mask
  PortWriteByte(PIC1_DATA, masterMask);
  PortWriteByte(PIC2_DATA, slaveMask);
}
