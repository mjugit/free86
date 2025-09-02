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
