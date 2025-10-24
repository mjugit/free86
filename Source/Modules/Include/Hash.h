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


#ifndef __HASH_H__
#define __HASH_H__

#include "SystemCore.h"


/*
  Murmur hash algorithm
  - non-cryptographic
  - good entropy
  - fast

  IMPORTANT: This implementation is for little endian machines
 */

#define _HASH_MURMUR3_BLOCKMIX_C1 0xcc9e2d51
#define _HASH_MURMUR3_BLOCKMIX_C2 0x1b873593
#define _HASH_MURMUR3_MIXCONSTANT 0xe6546b64

#define _HASH_MURMUR3_FINALIZER_C1 0x85ebca6b
#define _HASH_MURMUR3_FINALIZER_C2 0xc2b2ae35


module (MurmurHash) {
  U32 (*HashNumber)(U32 input);
  U32 (*HashBlock)(const void* data, U32 length, U32 seed);
};




module(Hash) {
  embed(MurmurHash, Murmur);
};


#endif
