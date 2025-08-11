# Copyright © 2025 Maximilian Jung
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the “Software”), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#


.PHONY: clean-libs

LIBS_BASE_PATH = Libs
LIBS_INCLUDE_PATH = $(LIBS_BASE_PATH)/Include

LIBS_BUILD_PATH = Libs/Build


$(LIBS_BUILD_PATH):
	mkdir -p $(LIBS_BUILD_PATH)

clean-libs:
	rm -vfr $(LIBS_BUILD_PATH)




# Memory library sources
LIB_MEMORY_SOURCE_PATH = $(LIBS_BASE_PATH)/Memory
LIB_MEMORY_SOURCES = $(wildcard $(LIB_MEMORY_SOURCE_PATH)/*.s)
LIB_MEMORY_HEADER = $(LIBS_INCLUDE_PATH)/Memory.h

# Memory library output
LIB_MEMORY_BUILD_PATH = $(LIBS_BUILD_PATH)/Memory
LIB_MEMORY_NAME = LibMemory.a
LIB_MEMORY_FILE = $(LIBS_BUILD_PATH)/$(LIB_MEMORY_NAME)

# Memory library object files
LIB_MEMORY_OBJECTS = $(LIB_MEMORY_SOURCES:$(LIB_MEMORY_SOURCE_PATH)/%.s=$(LIB_MEMORY_BUILD_PATH)/%.o)

.PHONY: build-lib-memory clean-lib-memory

build-lib-memory: $(LIB_MEMORY_FILE) | $(LIBS_BUILD_PATH)

clean-lib-memory:
	rm -vfr $(LIB_MEMORY_BUILD_PATH)
	rm -vfr $(LIB_MEMORY_FILE)


$(LIB_MEMORY_FILE): $(LIB_MEMORY_OBJECTS) $(LIB_MEMORY_HEADER) | $(LIBS_BUILD_PATH)
	$(AR) rcs $(LIB_MEMORY_FILE) $(LIB_MEMORY_OBJECTS)

$(LIB_MEMORY_BUILD_PATH)/%.o: $(LIB_MEMORY_SOURCE_PATH)/%.s | $(LIB_MEMORY_BUILD_PATH)
	$(AS) -o $@ $<

$(LIB_MEMORY_BUILD_PATH):
	mkdir -p $(LIB_MEMORY_BUILD_PATH)






# Bitmap library sources
LIB_BITMAP_SOURCE_PATH = $(LIBS_BASE_PATH)/Bitmap
LIB_BITMAP_ASSEMBLER_SOURCES = $(wildcard $(LIB_BITMAP_SOURCE_PATH)/*.s)
LIB_BITMAP_C_SOURCES = $(wildcard $(LIB_BITMAP_SOURCE_PATH)/*.c)
LIB_BITMAP_HEADER = $(LIBS_INCLUDE_PATH)/Bitmap.h

# Bitmap library output
LIB_BITMAP_BUILD_PATH = $(LIBS_BUILD_PATH)/Bitmap
LIB_BITMAP_NAME = LibBitmap.a
LIB_BITMAP_FILE = $(LIBS_BUILD_PATH)/$(LIB_BITMAP_NAME)

# Bitmap library object files
LIB_BITMAP_OBJECTS = $(LIB_BITMAP_ASSEMBLER_SOURCES:$(LIB_BITMAP_SOURCE_PATH)/%.s=$(LIB_BITMAP_BUILD_PATH)/%.o) \
$(LIB_BITMAP_C_SOURCES:$(LIB_BITMAP_SOURCE_PATH)/%.c=$(LIB_BITMAP_BUILD_PATH)/%.o)

.PHONY: build-lib-bitmap clean-lib-bitmap

build-lib-bitmap: $(LIB_BITMAP_FILE) | $(LIBS_BUILD_PATH)

clean-lib-bitmap:
	rm -vfr $(LIB_BITMAP_BUILD_PATH)
	rm -vfr $(LIB_BITMAP_FILE)


$(LIB_BITMAP_FILE): $(LIB_BITMAP_OBJECTS) $(LIB_BITMAP_HEADER) | $(LIBS_BUILD_PATH)
	$(AR) rcs $(LIB_BITMAP_FILE) $(LIB_BITMAP_OBJECTS)

$(LIB_BITMAP_BUILD_PATH)/%.o: $(LIB_BITMAP_SOURCE_PATH)/%.s | $(LIB_BITMAP_BUILD_PATH)
	$(AS) -o $@ $<

$(LIB_BITMAP_BUILD_PATH)/%.o: $(LIB_BITMAP_SOURCE_PATH)/%.c | $(LIB_BITMAP_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIB_BITMAP_BUILD_PATH):
	mkdir -p $(LIB_BITMAP_BUILD_PATH)





# Heap library sources
LIB_HEAP_SOURCE_PATH = $(LIBS_BASE_PATH)/Heap
LIB_HEAP_C_SOURCES = $(wildcard $(LIB_HEAP_SOURCE_PATH)/*.c)
LIB_HEAP_HEADER = $(LIBS_INCLUDE_PATH)/Heap.h

# Heap library output
LIB_HEAP_BUILD_PATH = $(LIBS_BUILD_PATH)/Heap
LIB_HEAP_NAME = LibHeap.a
LIB_HEAP_FILE = $(LIBS_BUILD_PATH)/$(LIB_HEAP_NAME)

# Heap library object files
LIB_HEAP_OBJECTS = $(LIB_HEAP_C_SOURCES:$(LIB_HEAP_SOURCE_PATH)/%.c=$(LIB_HEAP_BUILD_PATH)/%.o)

.PHONY: build-lib-heap clean-lib-heap

build-lib-heap: $(LIB_HEAP_FILE) | $(LIBS_BUILD_PATH)

clean-lib-heap:
	rm -vfr $(LIB_HEAP_BUILD_PATH)
	rm -vfr $(LIB_HEAP_FILE)


$(LIB_HEAP_FILE): $(LIB_HEAP_OBJECTS) $(LIB_HEAP_HEADER) | $(LIBS_BUILD_PATH)
	$(AR) rcs $(LIB_HEAP_FILE) $(LIB_HEAP_OBJECTS)

$(LIB_HEAP_BUILD_PATH)/%.o: $(LIB_HEAP_SOURCE_PATH)/%.c | $(LIB_HEAP_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIB_HEAP_BUILD_PATH):
	mkdir -p $(LIB_HEAP_BUILD_PATH)





# Gfx library sources
LIB_GFX_SOURCE_PATH = $(LIBS_BASE_PATH)/Gfx
LIB_GFX_C_SOURCES = $(wildcard $(LIB_GFX_SOURCE_PATH)/*.c)
LIB_GFX_HEADER = $(LIBS_INCLUDE_PATH)/Gfx.h

# Gfx library output
LIB_GFX_BUILD_PATH = $(LIBS_BUILD_PATH)/Gfx
LIB_GFX_NAME = LibGfx.a
LIB_GFX_FILE = $(LIBS_BUILD_PATH)/$(LIB_GFX_NAME)

# Gfx library object files
LIB_GFX_OBJECTS = $(LIB_GFX_C_SOURCES:$(LIB_GFX_SOURCE_PATH)/%.c=$(LIB_GFX_BUILD_PATH)/%.o)

.PHONY: build-lib-gfx clean-lib-gfx

build-lib-gfx: $(LIB_GFX_FILE) | $(LIBS_BUILD_PATH)

clean-lib-gfx:
	rm -vfr $(LIB_GFX_BUILD_PATH)
	rm -vfr $(LIB_GFX_FILE)


$(LIB_GFX_FILE): $(LIB_GFX_OBJECTS) $(LIB_GFX_HEADER) | $(LIBS_BUILD_PATH)
	$(AR) rcs $(LIB_GFX_FILE) $(LIB_GFX_OBJECTS)

$(LIB_GFX_BUILD_PATH)/%.o: $(LIB_GFX_SOURCE_PATH)/%.c | $(LIB_GFX_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIB_GFX_BUILD_PATH):
	mkdir -p $(LIB_GFX_BUILD_PATH)





# Interrupt library sources
LIB_INTERRUPT_SOURCE_PATH = $(LIBS_BASE_PATH)/Interrupt
LIB_INTERRUPT_C_SOURCES = $(wildcard $(LIB_INTERRUPT_SOURCE_PATH)/*.c)
LIB_INTERRUPT_HEADER = $(LIBS_INCLUDE_PATH)/Interrupt.h

# Interrupt library output
LIB_INTERRUPT_BUILD_PATH = $(LIBS_BUILD_PATH)/Interrupt
LIB_INTERRUPT_NAME = LibInterrupt.a
LIB_INTERRUPT_FILE = $(LIBS_BUILD_PATH)/$(LIB_INTERRUPT_NAME)

# Interrupt library object files
LIB_INTERRUPT_OBJECTS = $(LIB_INTERRUPT_C_SOURCES:$(LIB_INTERRUPT_SOURCE_PATH)/%.c=$(LIB_INTERRUPT_BUILD_PATH)/%.o)

.PHONY: build-lib-interrupt clean-lib-interrupt

build-lib-interrupt: $(LIB_INTERRUPT_FILE) | $(LIBS_BUILD_PATH)

clean-lib-interrupt:
	rm -vfr $(LIB_INTERRUPT_BUILD_PATH)
	rm -vfr $(LIB_INTERRUPT_FILE)


$(LIB_INTERRUPT_FILE): $(LIB_INTERRUPT_OBJECTS) $(LIB_INTERRUPT_HEADER) | $(LIBS_BUILD_PATH)
	$(AR) rcs $(LIB_INTERRUPT_FILE) $(LIB_INTERRUPT_OBJECTS)

$(LIB_INTERRUPT_BUILD_PATH)/%.o: $(LIB_INTERRUPT_SOURCE_PATH)/%.c | $(LIB_INTERRUPT_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIB_INTERRUPT_BUILD_PATH):
	mkdir -p $(LIB_INTERRUPT_BUILD_PATH)
