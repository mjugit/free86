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



# Build path
KERNEL_BUILD_PATH = Kernel/Build

# Kernel sources directory
KERNEL_SOURCES_PATH = Kernel

# Kernel linker file
KERNEL_LINKER_FILE = $(KERNEL_SOURCES_PATH)/Kernel.Linker.ld

# Kernel binary file
KERNEL_BINARY_FILE = $(KERNEL_BUILD_PATH)/Kernel.bin


# Kernel assembler source files
KERNEL_ASSEMBLER_SOURCES = $(wildcard $(KERNEL_SOURCES_PATH)/*.s)
KERNEL_C_SOURCES = $(wildcard $(KERNEL_SOURCES_PATH)/*.c)

# Kernel object files
KERNEL_OBJECTS = \
	$(KERNEL_ASSEMBLER_SOURCES:$(KERNEL_SOURCES_PATH)/%.s=$(KERNEL_BUILD_PATH)/%.o) \
	$(KERNEL_C_SOURCES:$(KERNEL_SOURCES_PATH)/%.c=$(KERNEL_BUILD_PATH)/%.o)

KERNEL_LIBS = \
	$(LIB_MEMORY_FILE) \
	$(LIB_BITMAP_FILE) \
	$(LIB_HEAP_FILE) \
	$(LIB_GFX_FILE)


.PHONY: build-kernel

build-kernel: clean-kernel $(KERNEL_BINARY_FILE)

clean-kernel:
	rm -vfr $(KERNEL_BUILD_PATH)


$(KERNEL_BINARY_FILE): $(KERNEL_OBJECTS) $(KERNEL_LIBS)| $(KERNEL_BUILD_PATH)
	$(LD) -o $(KERNEL_BINARY_FILE) --oformat binary -T $(KERNEL_LINKER_FILE) $(KERNEL_OBJECTS) \
		-L $(LIBS_BUILD_PATH) \
		-l:$(LIB_GFX_NAME) \
		-l:$(LIB_MEMORY_NAME) \
		-l:$(LIB_HEAP_NAME) \
		-l:$(LIB_BITMAP_NAME)

$(KERNEL_BUILD_PATH)/%.o: $(KERNEL_SOURCES_PATH)/%.s | $(KERNEL_BUILD_PATH)
	$(AS) -o $@ $<

$(KERNEL_BUILD_PATH)/%.o: $(KERNEL_SOURCES_PATH)/%.c | $(KERNEL_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<


$(KERNEL_BUILD_PATH):
	mkdir -p $(KERNEL_BUILD_PATH)
