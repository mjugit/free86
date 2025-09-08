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


MODULES_BASE_PATH = Modules
MODULES_INCLUDE_PATH = $(MODULES_BASE_PATH)/Include
MODULES_BUILD_PATH = $(MODULES_BASE_PATH)/Build

$(MODULES_BUILD_PATH):
	mkdir -p $(MODULES_BUILD_PATH)



# BITMAP module
MOD_BITMAP_SOURCE_PATH = $(MODULES_BASE_PATH)/Bitmap
MOD_BITMAP_BUILD_PATH = $(MODULES_BUILD_PATH)/Bitmap
MOD_BITMAP_OUTPUT = $(MOD_BITMAP_BUILD_PATH)/ModBitmap.o

MOD_BITMAP_ASM = $(wildcard $(MOD_BITMAP_SOURCE_PATH)/*.s)
MOD_BITMAP_C = $(wildcard $(MOD_BITMAP_SOURCE_PATH)/*.c)
MOD_BITMAP_SOURCE = $(MOD_BITMAP_ASM) $(MOD_BITMAP_C)

MOD_BITMAP_OBJECTS = $(patsubst $(MOD_BITMAP_SOURCE_PATH)/%.c, \
                                 $(MOD_BITMAP_BUILD_PATH)/%.o, \
                                 $(MOD_BITMAP_C)) \
                     $(patsubst $(MOD_BITMAP_SOURCE_PATH)/%.s, \
                                 $(MOD_BITMAP_BUILD_PATH)/%.o, \
                                 $(MOD_BITMAP_ASM))

$(MOD_BITMAP_BUILD_PATH):
	mkdir -p $(MOD_BITMAP_BUILD_PATH)

$(MOD_BITMAP_BUILD_PATH)/%.o: $(MOD_BITMAP_SOURCE_PATH)/%.s | $(MOD_BITMAP_BUILD_PATH)
	$(AS) -o $@ $<

$(MOD_BITMAP_BUILD_PATH)/%.o: $(MOD_BITMAP_SOURCE_PATH)/%.c | $(MOD_BITMAP_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(MOD_BITMAP_OUTPUT): $(MOD_BITMAP_OBJECTS)
	$(LD) -r -o $@ $^


.PHONY: mod-bitmap clean-mod-bitmap


mod-bitmap: $(MOD_BITMAP_OUTPUT)

clean-mod-bitmap:
	rm -fr $(MOD_BITMAP_BUILD_PATH)



# MEMORY module
MOD_MEMORY_SOURCE_PATH = $(MODULES_BASE_PATH)/Memory
MOD_MEMORY_BUILD_PATH = $(MODULES_BUILD_PATH)/Memory
MOD_MEMORY_OUTPUT = $(MOD_MEMORY_BUILD_PATH)/ModMemory.o

MOD_MEMORY_ASM = $(wildcard $(MOD_MEMORY_SOURCE_PATH)/*.s)
MOD_MEMORY_C = $(wildcard $(MOD_MEMORY_SOURCE_PATH)/*.c)
MOD_MEMORY_SOURCE = $(MOD_MEMORY_ASM) $(MOD_MEMORY_C)

MOD_MEMORY_OBJECTS = $(patsubst $(MOD_MEMORY_SOURCE_PATH)/%.c, \
                                 $(MOD_MEMORY_BUILD_PATH)/%.o, \
                                 $(MOD_MEMORY_C)) \
                     $(patsubst $(MOD_MEMORY_SOURCE_PATH)/%.s, \
                                 $(MOD_MEMORY_BUILD_PATH)/%.o, \
                                 $(MOD_MEMORY_ASM))

$(MOD_MEMORY_BUILD_PATH):
	mkdir -p $(MOD_MEMORY_BUILD_PATH)

$(MOD_MEMORY_BUILD_PATH)/%.o: $(MOD_MEMORY_SOURCE_PATH)/%.s | $(MOD_MEMORY_BUILD_PATH)
	$(AS) -o $@ $<

$(MOD_MEMORY_BUILD_PATH)/%.o: $(MOD_MEMORY_SOURCE_PATH)/%.c | $(MOD_MEMORY_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(MOD_MEMORY_OUTPUT): $(MOD_MEMORY_OBJECTS)
	$(LD) -r -o $@ $^


.PHONY: mod-memory clean-mod-memory

mod-memory: $(MOD_MEMORY_OUTPUT)

clean-mod-memory:
	rm -fr $(MOD_MEMORY_BUILD_PATH)



# HEAP module
MOD_HEAP_SOURCE_PATH = $(MODULES_BASE_PATH)/Heap
MOD_HEAP_BUILD_PATH = $(MODULES_BUILD_PATH)/Heap
MOD_HEAP_OUTPUT = $(MOD_HEAP_BUILD_PATH)/ModHeap.o

MOD_HEAP_ASM = $(wildcard $(MOD_HEAP_SOURCE_PATH)/*.s)
MOD_HEAP_C = $(wildcard $(MOD_HEAP_SOURCE_PATH)/*.c)
MOD_HEAP_SOURCE = $(MOD_HEAP_ASM) $(MOD_HEAP_C)

MOD_HEAP_OBJECTS = $(patsubst $(MOD_HEAP_SOURCE_PATH)/%.c, \
                                 $(MOD_HEAP_BUILD_PATH)/%.o, \
                                 $(MOD_HEAP_C)) \
                     $(patsubst $(MOD_HEAP_SOURCE_PATH)/%.s, \
                                 $(MOD_HEAP_BUILD_PATH)/%.o, \
                                 $(MOD_HEAP_ASM))

$(MOD_HEAP_BUILD_PATH):
	mkdir -p $(MOD_HEAP_BUILD_PATH)

$(MOD_HEAP_BUILD_PATH)/%.o: $(MOD_HEAP_SOURCE_PATH)/%.s | $(MOD_HEAP_BUILD_PATH)
	$(AS) -o $@ $<

$(MOD_HEAP_BUILD_PATH)/%.o: $(MOD_HEAP_SOURCE_PATH)/%.c | $(MOD_HEAP_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(MOD_HEAP_OUTPUT): $(MOD_HEAP_OBJECTS)
	$(LD) -r -o $@ $^


.PHONY: mod-heap clean-mod-heap

mod-heap: $(MOD_HEAP_OUTPUT)

clean-mod-heap:
	rm -fr $(MOD_HEAP_BUILD_PATH)



# STRING module
MOD_STRING_SOURCE_PATH = $(MODULES_BASE_PATH)/String
MOD_STRING_BUILD_PATH = $(MODULES_BUILD_PATH)/String
MOD_STRING_OUTPUT = $(MOD_STRING_BUILD_PATH)/ModString.o

MOD_STRING_ASM = $(wildcard $(MOD_STRING_SOURCE_PATH)/*.s)
MOD_STRING_C = $(wildcard $(MOD_STRING_SOURCE_PATH)/*.c)
MOD_STRING_SOURCE = $(MOD_STRING_ASM) $(MOD_STRING_C)

MOD_STRING_OBJECTS = $(patsubst $(MOD_STRING_SOURCE_PATH)/%.c, \
                                 $(MOD_STRING_BUILD_PATH)/%.o, \
                                 $(MOD_STRING_C)) \
                     $(patsubst $(MOD_STRING_SOURCE_PATH)/%.s, \
                                 $(MOD_STRING_BUILD_PATH)/%.o, \
                                 $(MOD_STRING_ASM))

$(MOD_STRING_BUILD_PATH):
	mkdir -p $(MOD_STRING_BUILD_PATH)

$(MOD_STRING_BUILD_PATH)/%.o: $(MOD_STRING_SOURCE_PATH)/%.s | $(MOD_STRING_BUILD_PATH)
	$(AS) -o $@ $<

$(MOD_STRING_BUILD_PATH)/%.o: $(MOD_STRING_SOURCE_PATH)/%.c | $(MOD_STRING_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(MOD_STRING_OUTPUT): $(MOD_STRING_OBJECTS)
	$(LD) -r -o $@ $^


.PHONY: mod-string clean-mod-string

mod-string: $(MOD_STRING_OUTPUT)

clean-mod-string:
	rm -fr $(MOD_STRING_BUILD_PATH)



# STREAM module
MOD_STREAM_SOURCE_PATH = $(MODULES_BASE_PATH)/Stream
MOD_STREAM_BUILD_PATH = $(MODULES_BUILD_PATH)/Stream
MOD_STREAM_OUTPUT = $(MOD_STREAM_BUILD_PATH)/ModStream.o

MOD_STREAM_ASM = $(wildcard $(MOD_STREAM_SOURCE_PATH)/*.s)
MOD_STREAM_C = $(wildcard $(MOD_STREAM_SOURCE_PATH)/*.c)
MOD_STREAM_SOURCE = $(MOD_STREAM_ASM) $(MOD_STREAM_C)

MOD_STREAM_OBJECTS = $(patsubst $(MOD_STREAM_SOURCE_PATH)/%.c, \
                                 $(MOD_STREAM_BUILD_PATH)/%.o, \
                                 $(MOD_STREAM_C)) \
                     $(patsubst $(MOD_STREAM_SOURCE_PATH)/%.s, \
                                 $(MOD_STREAM_BUILD_PATH)/%.o, \
                                 $(MOD_STREAM_ASM))

$(MOD_STREAM_BUILD_PATH):
	mkdir -p $(MOD_STREAM_BUILD_PATH)

$(MOD_STREAM_BUILD_PATH)/%.o: $(MOD_STREAM_SOURCE_PATH)/%.s | $(MOD_STREAM_BUILD_PATH)
	$(AS) -o $@ $<

$(MOD_STREAM_BUILD_PATH)/%.o: $(MOD_STREAM_SOURCE_PATH)/%.c | $(MOD_STREAM_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(MOD_STREAM_OUTPUT): $(MOD_STREAM_OBJECTS)
	$(LD) -r -o $@ $^


.PHONY: mod-stream clean-mod-stream

mod-stream: $(MOD_STREAM_OUTPUT)

clean-mod-stream:
	rm -fr $(MOD_STREAM_BUILD_PATH)
