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



# Source code
BOOTLOADER_SOURCE_FILE = Bootloader/Bootloader.s
BOOTLOADER_LINKER_FILE = Bootloader/Bootloader.Linker.ld

# Build path
BOOTLOADER_BUILD_PATH = Bootloader/Build

# Path to object file
BOOTLOADER_OBJECT_FILE = $(BOOTLOADER_BUILD_PATH)/Bootloader.o

# Path to binary
BOOTLOADER_BINARY_FILE = $(BOOTLOADER_BUILD_PATH)/Bootloader.bin


.PHONY: build-bootloader clean-bootloader


# (Re)build the bootloader
build-bootloader: clean-bootloader $(BOOTLOADER_BINARY_FILE)


# Build the bootloader binary
$(BOOTLOADER_BINARY_FILE): $(BOOTLOADER_SOURCE_FILE) $(BOOTLOADER_LINKER_FILE) | $(BOOTLOADER_BUILD_PATH)
	$(AS) -o $(BOOTLOADER_OBJECT_FILE) -c $(BOOTLOADER_SOURCE_FILE)
	$(LD) -o $(BOOTLOADER_BINARY_FILE) --oformat binary -T $(BOOTLOADER_LINKER_FILE) $(BOOTLOADER_OBJECT_FILE)


# Create the bootloader build directory
$(BOOTLOADER_BUILD_PATH):
	mkdir -p $(BOOTLOADER_BUILD_PATH)


# Remove the bootloader build artifacts
clean-bootloader:
	rm -vfr $(BOOTLOADER_BUILD_PATH)

