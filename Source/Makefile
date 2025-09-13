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

include Make.Tools.mk
include Make.Bootloader.mk
include Make.Libs.mk
include Make.Modules.mk
include Make.Kernel.mk


FLOPPY_BUILD_PATH = Build

FLOPPY_IMAGE_FILE = $(FLOPPY_BUILD_PATH)/Kickstart.img


.PHONY: build-floppy clean-floppy


build-floppy: clean-floppy $(FLOPPY_IMAGE_FILE)


clean-floppy:
	rm -f $(FLOPPY_IMAGE_FILE)


$(FLOPPY_IMAGE_FILE): $(KERNEL_BINARY_FILE) $(BOOTLOADER_BINARY_FILE) | $(FLOPPY_BUILD_PATH)
	dd if=/dev/zero of=$(FLOPPY_IMAGE_FILE) bs=512 count=2880 conv=notrunc
	dd if=$(BOOTLOADER_BINARY_FILE) of=$(FLOPPY_IMAGE_FILE) bs=512 count=1 seek=0 conv=notrunc
	dd if=$(KERNEL_BINARY_FILE) of=$(FLOPPY_IMAGE_FILE) bs=512 count=15 seek=1 conv=notrunc

$(FLOPPY_BUILD_PATH):
	mkdir -p $(FLOPPY_BUILD_PATH)


build-all: build-bootloader build-kernel build-floppy

clean-all: clean-floppy clean-kernel clean-bootloader


run-qemu: $(FLOPPY_IMAGE_FILE)
	qemu-system-i386 -drive if=floppy,index=0,media=disk,file=$(FLOPPY_IMAGE_FILE),format=raw -m 5M -serial stdio -parallel none -rtc base=localtime -no-fd-bootchk


