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


LIBGFX_SRC = $(wildcard $(KERNEL_SRC_DIR)/libgfx/*.c)
LIBGFX_SRC_DIR = $(KERNEL_SRC_DIR)/libgfx
LIBGFX_OBJ_DIR = $(BUILD_OBJ_DIR)/kernel/libgfx


KERNEL_OBJ = $(KERNEL_C_SRC:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_OBJ_DIR)/%.o) \
	$(KERNEL_AS_SRC:$(KERNEL_SRC_DIR)/%.s=$(KERNEL_OBJ_DIR)/%.o) \
	$(LIBGFX_SRC:$(LIBGFX_SRC_DIR)/%.c=$(LIBGFX_OBJ_DIR)/%.o)

$(KERNEL_BIN): $(KERNEL_OBJ)
#	$(CC) -T $(KERNEL_LINK) -o $(KERNEL_BIN) -ffreestanding -nostdlib $(KERNEL_OBJ_DIR)/kernel_boot.o -lgcc
	$(LD) -o $(KERNEL_BIN) --oformat binary -T $(KERNEL_LINK) $(KERNEL_OBJ)

$(KERNEL_OBJ_DIR)/%.o: $(KERNEL_SRC_DIR)/%.s | $(KERNEL_OBJ_DIR)
	$(AS) -o $@ $<

$(KERNEL_OBJ_DIR)/%.o: $(KERNEL_SRC_DIR)/%.c | $(KERNEL_OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIBGFX_OBJ_DIR)/%.o: $(LIBGFX_SRC_DIR)/%.c | $(LIBGFX_OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) -c $<

$(KERNEL_OBJ_DIR):
	mkdir -p $(KERNEL_OBJ_DIR)

$(LIBGFX_OBJ_DIR):
	mkdir -p $(LIBGFX_OBJ_DIR)

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

