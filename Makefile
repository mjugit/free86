REPOSITORY_PATH = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

BUILD_DIR = $(REPOSITORY_PATH)/build
BUILD_OBJ_DIR = $(BUILD_DIR)/obj
BUILD_BIN_DIR = $(BUILD_DIR)/bin

AS = tools/bin/i386-elf-as
LD = tools/bin/i386-elf-ld



MBR_BIN = $(BUILD_BIN_DIR)/BOOTSECTOR.bin
MBR_OBJ = $(BUILD_OBJ_DIR)/mbr/boot-sector.o
MBR_SRC = $(REPOSITORY_PATH)/boot/boot-sector.s
MBR_LINK = $(REPOSITORY_PATH)/boot/mbr.link.ld

$(MBR_BIN): $(MBR_SRC)
	mkdir -p $(BUILD_BIN_DIR)
	mkdir -p $(BUILD_OBJ_DIR)/mbr
	$(AS) -o $(MBR_OBJ) -c $(MBR_SRC)
	$(LD) -o $(MBR_BIN) --oformat binary -T $(MBR_LINK) $(MBR_OBJ)

.PHONY: clean

clean:
	rm -fr $(BUILD_OBJ_DIR)
	rm -fr $(BUILD_BIN_DIR)
	rm -f $(KICKSTART_FLOPPY)

KICKSTART_FLOPPY = $(BUILD_DIR)/kickstart.img

qemu: $(MBR_BIN)
	rm -f $(KICKSTART_FLOPPY)
	dd if=/dev/zero of=$(KICKSTART_FLOPPY) bs=512 count=2880
	dd if=$(MBR_BIN) of=$(KICKSTART_FLOPPY) bs=512 count=1 seek=0
	qemu-system-i386 -drive if=floppy,file=$(KICKSTART_FLOPPY),format=raw

