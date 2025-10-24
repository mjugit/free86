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


#include "../Include/Hash.h"

use(MurmurHash);


U32 _Murmur3_HashBlockImplementation(const void* data, U32 length, U32 seed) {
  const U8* bytes = (const U8*)data;
  const U32 numberOfBlocks = length / 4;

  U32 hash = seed;

  // Full 4-byte blocks
  const U32* blocks = (const U32*)(bytes);
  for (U32 blockIndex = 0; blockIndex < numberOfBlocks; blockIndex++) {
    U32 nextBlock = blocks[blockIndex];

    nextBlock *= _HASH_MURMUR3_BLOCKMIX_C1;
    nextBlock = (nextBlock << 15) | (nextBlock >> (32 - 15));
    nextBlock *= _HASH_MURMUR3_BLOCKMIX_C2;

    hash ^= nextBlock;
    hash = (hash << 13) | (hash >> (32 - 13));
    hash = hash * 5 + _HASH_MURMUR3_MIXCONSTANT;
  }

  // Rest
  const U8* tail = (const U8*)(bytes + (numberOfBlocks * 4));
  U32 restBlock = 0;

  switch (length & 3) {
  case 3:
    restBlock ^= tail[2] << 16;
    // Intentional fall-through
  case 2:
    restBlock ^= tail[1] << 8;
    // Intentional fall-through
  case 1:
    restBlock ^= tail[0];
    restBlock *= _HASH_MURMUR3_BLOCKMIX_C1;
    restBlock = (restBlock << 15) | (restBlock >> (32 - 15));
    restBlock *= _HASH_MURMUR3_BLOCKMIX_C2;
    hash ^= restBlock;
    break;
  }

  // Finalize
  return MurmurHash.HashNumber(hash);  
}
