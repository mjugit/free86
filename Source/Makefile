SOURCE_DIRECTORY_PATH = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
REPOSITORY_PATH = $(SOURCE_DIRECTORY_PATH)/..

BUILD_DIRECTORY_PATH = $(REPOSITORY_PATH)/Build


AS = $(REPOSITORY_PATH)/Tools/bin/i386-elf-as
AR = $(REPOSITORY_PATH)/Tools/bin/i386-elf-ar
LD = $(REPOSITORY_PATH)/Tools/bin/i386-elf-ld
CC = $(REPOSITORY_PATH)/Tools/bin/i386-elf-gcc
CFLAGS =  -ffreestanding -O2 -Wall -Wextra -std=gnu99

.PHONY: bootloader clean-bootloader kernel clean-kernel clean all floppy clean-floppy qemu qemu-debug

all: $(STRING_LIB) floppy

clean: clean-bootloader clean-kernel clean-floppy


# BOOTLOADER

BOOTSECTOR_BUILD_PATH = $(BUILD_DIRECTORY_PATH)/Bootloader
BOOTSECTOR_SOURCE_PATH = $(SOURCE_DIRECTORY_PATH)/Bootloader
BOOTSECTOR_BINARY = $(BUILD_DIRECTORY_PATH)/Bootloader.bin
BOOTSECTOR_SOURCE = $(BOOTSECTOR_SOURCE_PATH)/BootSector.s
BOOTSECTOR_OBJECT = $(BOOTSECTOR_BUILD_PATH)/BootSector.o
BOOTSECTOR_LINK = $(BOOTSECTOR_SOURCE_PATH)/BootSector.Link.ld


bootloader: clean-bootloader $(BOOTSECTOR_BINARY)

clean-bootloader:
	rm -f $(BOOTSECTOR_BINARY)
	rm -fr $(BOOTSECTOR_BUILD_PATH)

$(BOOTSECTOR_BINARY): $(BOOTSECTOR_SOURCE) | $(BOOTSECTOR_BUILD_PATH)
	$(AS) -o $(BOOTSECTOR_OBJECT) -c $(BOOTSECTOR_SOURCE)
	$(LD) -o $(BOOTSECTOR_BINARY) --oformat binary -T $(BOOTSECTOR_LINK) $(BOOTSECTOR_OBJECT)

$(BOOTSECTOR_BUILD_PATH):
	mkdir -p $(BOOTSECTOR_BUILD_PATH)


# STANDARDLIB

STANDARDLIB_BUILD_PATH = $(BUILD_DIRECTORY_PATH)/StandardLib
STANDARDLIB_SOURCE_PATH = $(SOURCE_DIRECTORY_PATH)/StandardLib

STRING_BUILD_PATH = $(STANDARDLIB_BUILD_PATH)/String
STRING_SOURCE_PATH = $(STANDARDLIB_SOURCE_PATH)/String
STRING_LIB = $(BUILD_DIRECTORY_PATH)/String.a
STRING_C_SOURCES = $(wildcard $(STANDARDLIB_SOURCE_PATH)/String/*.c)
STRING_OBJECTS = $(STRING_C_SOURCES:$(STANDARDLIB_SOURCE_PATH)/String/%.c=$(STRING_BUILD_PATH)/%.o)

$(STRING_LIB): $(STRING_OBJECTS)
	$(AR) rcs $(STRING_LIB) $(STRING_OBJECTS)

$(STRING_BUILD_PATH)/%.o: $(STRING_SOURCE_PATH)/%.c | $(STRING_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(STRING_BUILD_PATH):
	mkdir -p $(STRING_BUILD_PATH)



# KERNEL

KERNEL_BUILD_PATH = $(BUILD_DIRECTORY_PATH)/Kernel
KERNEL_SOURCE_PATH = $(SOURCE_DIRECTORY_PATH)/Kernel
KERNEL_BINARY = $(BUILD_DIRECTORY_PATH)/Kernel.bin
KERNEL_LINK = $(KERNEL_SOURCE_PATH)/Kernel.Link.ld
KERNEL_C_SOURCES = $(wildcard $(KERNEL_SOURCE_PATH)/*.c)
KERNEL_ASSEMBLER_SOURCES = $(wildcard $(KERNEL_SOURCE_PATH)/*.s)

INT_C_SOURCES = $(wildcard $(KERNEL_SOURCE_PATH)/Interrupt/*.c)
INT_ASSEMBLER_SOURCES = $(wildcard $(KERNEL_SOURCE_PATH)/Interrupt/*.s)

KERNEL_OBJECTS = \
	$(KERNEL_C_SOURCES:$(KERNEL_SOURCE_PATH)/%.c=$(KERNEL_BUILD_PATH)/%.o) \
	$(KERNEL_ASSEMBLER_SOURCES:$(KERNEL_SOURCE_PATH)/%.s=$(KERNEL_BUILD_PATH)/%.o) \
	$(INT_C_SOURCES:$(KERNEL_SOURCE_PATH)/Interrupt/%.c=$(KERNEL_BUILD_PATH)/%.o) \
	$(INT_ASSEMBLER_SOURCES:$(KERNEL_SOURCE_PATH)/Interrupt/%.s=$(KERNEL_BUILD_PATH)/%.o)


kernel: clean-kernel $(KERNEL_BINARY)

clean-kernel:
	rm -f $(KERNEL_BINARY)
	rm -fr $(KERNEL_BUILD_PATH)

$(KERNEL_BINARY): $(KERNEL_OBJECTS) $(STRING_LIB) | $(KERNEL_BUILD_PATH)
	$(LD) -o $(KERNEL_BINARY) --oformat binary -T $(KERNEL_LINK) $(KERNEL_OBJECTS) \
		-L $(BUILD_DIRECTORY_PATH) \
		-l:String.a

$(KERNEL_BUILD_PATH)/%.o: $(KERNEL_SOURCE_PATH)/%.s | $(KERNEL_BUILD_PATH)
	$(AS) -o $@ $<

$(KERNEL_BUILD_PATH)/%.o: $(KERNEL_SOURCE_PATH)/%.c | $(KERNEL_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(KERNEL_BUILD_PATH)/%.o: $(KERNEL_SOURCE_PATH)/Interrupt/%.s | $(KERNEL_BUILD_PATH)
	$(AS) -o $@ $<

$(KERNEL_BUILD_PATH)/%.o: $(KERNEL_SOURCE_PATH)/Interrupt/%.c | $(KERNEL_BUILD_PATH)
	$(CC) -o $@ $(CFLAGS) -c $<

$(KERNEL_BUILD_PATH):
	mkdir -p $(KERNEL_BUILD_PATH)




# OUTPUT

KICKSTART_FLOPPY = $(BUILD_DIRECTORY_PATH)/Kickstart.img

floppy: $(KICKSTART_FLOPPY)


$(KICKSTART_FLOPPY): $(KERNEL_BINARY) $(BOOTSECTOR_BINARY)
	rm -f $(KICKSTART_FLOPPY)
	dd if=/dev/zero of=$(KICKSTART_FLOPPY) bs=512 count=2880
	dd if=$(BOOTSECTOR_BINARY) of=$(KICKSTART_FLOPPY) bs=512 count=1 seek=0
	dd if=$(KERNEL_BINARY) of=$(KICKSTART_FLOPPY) bs=512 seek=1 count=15

clean-floppy:
	rm -f $(KICKSTART_FLOPPY)


# TESTING

qemu: $(KICKSTART_FLOPPY)
	qemu-system-i386 -drive if=floppy,file=$(KICKSTART_FLOPPY),format=raw -m 1M -serial stdio -parallel none -rtc base=localtime -no-fd-bootchk

qemu-debug: $(KICKSTART_FLOPPY)
	qemu-system-i386 -drive if=floppy,file=$(KICKSTART_FLOPPY),format=raw -m 1M -serial stdio -parallel none -rtc base=localtime -no-fd-bootchk -s -S
