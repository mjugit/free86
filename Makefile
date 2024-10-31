REPOSITORY_PATH = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

BUILD_DIR = $(REPOSITORY_PATH)/build
BUILD_OBJ_DIR = $(BUILD_DIR)/obj
BUILD_BIN_DIR = $(BUILD_DIR)/bin

AS = tools/bin/i386-elf-as
LD = tools/bin/i386-elf-ld

CC = tools/bin/i386-elf-gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu99


KICKSTART_FLOPPY = $(BUILD_DIR)/KICKSTART.img


MBR_BIN = $(BUILD_BIN_DIR)/BOOTSECTOR.bin
MBR_OBJ = $(BUILD_OBJ_DIR)/mbr/boot_sector.o
MBR_SRC = $(REPOSITORY_PATH)/boot/boot_sector.s
MBR_LINK = $(REPOSITORY_PATH)/boot/mbr_link.ld

all: $(KICKSTART_FLOPPY)

$(MBR_BIN): $(MBR_SRC)
	mkdir -p $(BUILD_BIN_DIR)
	mkdir -p $(BUILD_OBJ_DIR)/mbr
	$(AS) -o $(MBR_OBJ) -c $(MBR_SRC)
	$(LD) -o $(MBR_BIN) --oformat binary -T $(MBR_LINK) $(MBR_OBJ)



KERNEL_BIN = $(BUILD_BIN_DIR)/KERNEL.bin
KERNEL_LINK = $(REPOSITORY_PATH)/kernel/kernel_link.ld
KERNEL_C_SRC = $(wildcard $(KERNEL_SRC_DIR)/*.c)
KERNEL_AS_SRC = $(wildcard $(KERNEL_SRC_DIR)/*.s)

KERNEL_OBJ_DIR = $(BUILD_OBJ_DIR)/kernel
KERNEL_SRC_DIR = $(REPOSITORY_PATH)/kernel

LIBVGA_SRC = $(wildcard $(KERNEL_SRC_DIR)/libvga/*.c)
LIBVGA_SRC_DIR = $(KERNEL_SRC_DIR)/libvga
LIBVGA_OBJ_DIR = $(BUILD_OBJ_DIR)/kernel/libvga

LIBSTRING_SRC = $(wildcard $(KERNEL_SRC_DIR)/string/*.c)
LIBSTRING_SRC_DIR = $(KERNEL_SRC_DIR)/string
LIBSTRING_OBJ_DIR = $(BUILD_OBJ_DIR)/kernel/string

LIBINT_SRC = $(wildcard $(KERNEL_SRC_DIR)/libint/*.c)
LIBINT_AS_SRC = $(wildcard $(KERNEL_SRC_DIR)/libint/*.s)
LIBINT_SRC_DIR = $(KERNEL_SRC_DIR)/libint
LIBINT_OBJ_DIR = $(BUILD_OBJ_DIR)/kernel/libint


KERNEL_OBJ = $(KERNEL_C_SRC:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_OBJ_DIR)/%.o) \
	$(KERNEL_AS_SRC:$(KERNEL_SRC_DIR)/%.s=$(KERNEL_OBJ_DIR)/%.o) \
	$(LIBVGA_SRC:$(LIBVGA_SRC_DIR)/%.c=$(LIBVGA_OBJ_DIR)/%.o) \
	$(LIBSTRING_SRC:$(LIBSTRING_SRC_DIR)/%.c=$(LIBSTRING_OBJ_DIR)/%.o) \
	$(LIBINT_SRC:$(LIBINT_SRC_DIR)/%.c=$(LIBINT_OBJ_DIR)/%.o) \
	$(LIBINT_AS_SRC:$(LIBINT_SRC_DIR)/%.s=$(LIBINT_OBJ_DIR)/%.o)

$(KERNEL_BIN): $(KERNEL_OBJ)
#	$(CC) -T $(KERNEL_LINK) -o $(KERNEL_BIN) -ffreestanding -nostdlib $(KERNEL_OBJ_DIR)/kernel_boot.o -lgcc
	$(LD) -o $(KERNEL_BIN) --oformat binary -T $(KERNEL_LINK) $(KERNEL_OBJ)

$(KERNEL_OBJ_DIR)/%.o: $(KERNEL_SRC_DIR)/%.s | $(KERNEL_OBJ_DIR)
	$(AS) -o $@ $<

$(KERNEL_OBJ_DIR)/%.o: $(KERNEL_SRC_DIR)/%.c | $(KERNEL_OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIBSTRING_OBJ_DIR)/%.o: $(LIBSTRING_SRC_DIR)/%.c | $(LIBSTRING_OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIBVGA_OBJ_DIR)/%.o: $(LIBVGA_SRC_DIR)/%.c | $(LIBVGA_OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIBINT_OBJ_DIR)/%.o: $(LIBINT_SRC_DIR)/%.c | $(LIBINT_OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIBINT_OBJ_DIR)/%.o: $(LIBINT_SRC_DIR)/%.s | $(LIBINT_OBJ_DIR)
	$(AS) -o $@ $<

$(KERNEL_OBJ_DIR):
	mkdir -p $(KERNEL_OBJ_DIR)

$(LIBVGA_OBJ_DIR):
	mkdir -p $(LIBVGA_OBJ_DIR)

$(LIBSTRING_OBJ_DIR):
	mkdir -p $(LIBSTRING_OBJ_DIR)

$(LIBINT_OBJ_DIR):
	mkdir -p $(LIBINT_OBJ_DIR)

.PHONY: clean

clean:
	rm -fr $(BUILD_OBJ_DIR)
	rm -fr $(BUILD_BIN_DIR)
	rm -f $(KICKSTART_FLOPPY)


$(KICKSTART_FLOPPY): $(MBR_BIN) $(KERNEL_BIN)
	rm -f $(KICKSTART_FLOPPY)
	dd if=/dev/zero of=$(KICKSTART_FLOPPY) bs=512 count=2880
	dd if=$(MBR_BIN) of=$(KICKSTART_FLOPPY) bs=512 count=1 seek=0
	dd if=$(KERNEL_BIN) of=$(KICKSTART_FLOPPY) bs=512 seek=1 count=15

qemu: $(KICKSTART_FLOPPY)
	qemu-system-i386 -drive if=floppy,file=$(KICKSTART_FLOPPY),format=raw

qemu-debug: $(KICKSTART_FLOPPY)
	qemu-system-i386 -drive if=floppy,file=$(KICKSTART_FLOPPY),format=raw -s -S

