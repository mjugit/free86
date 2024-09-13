# Preparing Physical Floppies for free86

Floppy disks, despite being a legacy storage medium, are crucial for working with older systems like the Intel 80386SX. In modern development setups, where physical floppy interfaces are no longer present, **USB floppy drives** are typically used. These devices abstract much of the low-level floppy handling and are recognized as block devices (`/dev/sdX`), similar to USB flash drives.

In this guide, we’ll cover how to properly prepare floppy disks using a USB floppy drive, explain how to identify the device using `lsblk` and `lsusb`, and describe how to perform low-level formatting using `ufiformat`. We will also discuss the necessary configuration (`modprobe sg`) and how to directly write raw data (blobs) to specific sectors.

_**Arch Linux users** can use the [`ufiformat`](https://aur.archlinux.org/packages/ufiformat) AUR package._

## Understanding the Floppy Disk

From a programmer's perspective, a **floppy disk** is structured with:
- **80 tracks** per side (cylinders),
- **2 heads** (for two-sided disks),
- **18 sectors** per track,
- **512 bytes** per sector.

This structure forms the total **1.44 MB (1440 KiB)** capacity of a 3.5-inch floppy disk. When developing for free86, we'll bypass file systems (like FAT12) and often write raw data (blobs) directly to specific sectors on the disk.

## Using a USB Floppy Drive on Modern Systems

When using a **USB floppy drive**, it is recognized as a block device under `/dev/sdX`. Since USB devices abstract the actual floppy drive mechanics, your system interacts with the device in the same way it would with any USB storage device. You can identify and verify your USB floppy drive using two key utilities: `lsblk` and `lsusb`.

### Identifying the Floppy Drive with `lsblk`

`lsblk` (list block devices) is a helpful command to list all block devices attached to your system. You can use it to verify that the floppy drive is detected and identify its device name (e.g., `/dev/sdb`).

```bash
lsblk
```

Example output:
```
NAME   MAJ:MIN RM   SIZE RO TYPE MOUNTPOINT
sda      8:0    0 232.9G  0 disk 
└─sda1   8:1    0 232.9G  0 part /
sdb      8:16   1   1.4M  0 disk 
```

Here, `sdb` is the USB floppy drive, showing the correct size of **1.4 MB**.

### Identifying the USB Floppy Drive with `lsusb`

`lsusb` lists all USB devices connected to your system. You can use this to verify that your USB floppy drive is recognized at the hardware level.

```bash
lsusb
```

Example output:
```
Bus 002 Device 003: ID 1234:5678 Example USB Floppy Drive
```

If you see your USB floppy drive listed here, it confirms that the system recognizes it.

## Preparing the System for Low-Level Floppy Formatting

Before you can use **`ufiformat`** to low-level format the USB floppy, you may need to load the **SCSI generic (sg)** driver using `modprobe`. This is necessary because `ufiformat` interacts with the drive at a lower level, and the `sg` module allows for the necessary communication with the USB device.

To load the module, run:

```bash
sudo modprobe sg
```

After loading the module, you should be able to use `ufiformat` for low-level formatting.

## Low-Level Formatting with `ufiformat`

### Why Low-Level Formatting is Necessary

Low-level formatting is essential for ensuring that a floppy disk is correctly divided into tracks, sectors, and heads, and is ready for writing data. This process prepares the physical media by writing sector markers and ensuring that each sector is correctly aligned and readable. It wipes residual data, ensuring that the disk is in a clean state before use.

### Performing Low-Level Formatting

The `ufiformat` utility is specifically designed for formatting USB floppy drives. It ensures that the disk geometry (tracks, sectors, heads) is correctly set up for legacy systems to access the data. Here’s how to format a 1.44MB disk:

```bash
sudo ufiformat -f 1440 /dev/sdX
```

Where `/dev/sdX` is the USB floppy device identified earlier with `lsblk`. This command performs the following actions:
1. Sets the disk geometry to 80 tracks, 2 heads, and 18 sectors per track.
2. Writes sector markers to prepare the disk for data.
3. Wipes any existing data, leaving the disk ready for writing.

## Writing Raw Data (Blobs) to Specific Sectors

In the **free86** project, we typically avoid using a file system like FAT12 and instead write raw binary data (blobs) directly to specific sectors of the floppy. This method allows for fine-grained control over the data and is often necessary for tasks like writing bootloaders or initializing the system.

To write raw data to specific sectors on the floppy, you can use the `dd` command. Here’s an example of how to write a bootloader to the first sector (MBR) of the floppy:

```bash
sudo dd if=bootloader.bin of=/dev/sdX bs=512 count=1 conv=notrunc
```

In this example:
- **`if=bootloader.bin`** specifies the input file (your bootloader binary).
- **`of=/dev/sdX`** is the USB floppy device.
- **`bs=512`** sets the block size to 512 bytes (one sector).
- **`count=1`** ensures only the first sector is written.
- **`conv=notrunc`** prevents `dd` from truncating the file, ensuring that the data is written without altering the rest of the disk.

You can write other data or blobs to specific sectors by adjusting the `seek` parameter. For example, to write to the second track, you can use:

```bash
sudo dd if=data.bin of=/dev/sdX bs=512 seek=18 count=1
```

Here, `seek=18` moves the pointer to the start of the second track (each track has 18 sectors).

### Example Workflow

1. **Format the floppy**: Ensure that the disk is clean and ready.
2. **Write boot code**: Place your bootloader or system initialization code into the first sector.
3. **Write additional data**: If needed, write other necessary data (e.g., kernel, configuration) to specific sectors.

## Ensuring Compatibility with Legacy Systems

### Disk Geometry

Legacy systems expect a specific disk geometry: 80 tracks, 2 heads, and 18 sectors per track. Always format your floppies with this geometry to ensure compatibility.

### Boot Sector Layout

For legacy systems, the **Master Boot Record (MBR)** in the first sector (Track 0, Head 0, Sector 1) must contain valid boot code and end with the boot signature (`0x55AA`). If the boot sector is not properly formatted, the system BIOS may fail to boot from the disk.

### Media Condition

Floppy disks degrade over time. Always verify that your floppy is in good condition before using it. You can use tools like `badblocks` to check for bad sectors:

```bash
sudo badblocks -w /dev/sdX
```

This will perform a read-write test on the floppy to ensure it has no bad sectors.

### BIOS Access

Legacy systems rely on BIOS interrupts like `INT 13h` to access the floppy disk. Ensuring proper formatting and boot sector layout guarantees that the BIOS can correctly read the disk during the boot process.

