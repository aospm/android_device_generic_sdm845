include device/generic/sdm845/BoardConfigCommon.mk

# Copied from https://github.com/LineageOS/android_device_oneplus_sdm845-common/blob/lineage-16.0/BoardConfigCommon.mk
# Board Information
TARGET_BOOTLOADER_BOARD_NAME := sdm845
#TARGET_BOARD_PLATFORM := sdm845

# Kernel/boot.img Configuration
BOARD_KERNEL_BASE        := 0x00000000
BOARD_KERNEL_PAGESIZE    := 4096
BOARD_KERNEL_TAGS_OFFSET := 0x00000100
BOARD_RAMDISK_OFFSET     := 0x01000000
BOARD_KERNEL_CMDLINE     := earlycon console=ttyMSM0,115200n8 console=tty0 androidboot.console=ttyMSM0
BOARD_KERNEL_CMDLINE     += androidboot.hardware=sdm845 androidboot.boot_devices=soc@0/1d84000.ufshc
BOARD_KERNEL_CMDLINE     += firmware_class.path=/vendor/firmware/ init=/init printk.devkmsg=on
BOARD_KERNEL_CMDLINE     += deferred_probe_timeout=30
BOARD_KERNEL_CMDLINE     += fw_devlink=on selinux=0

# Image Configuration
BOARD_BOOTIMAGE_PARTITION_SIZE := 67108864 #64M
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 2998927360
BOARD_USERDATAIMAGE_PARTITION_SIZE := 118112366592
BOARD_VENDORIMAGE_PARTITION_SIZE := 536870912
BOARD_FLASH_BLOCK_SIZE := 262144
